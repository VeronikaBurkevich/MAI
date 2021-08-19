#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

typedef unsigned char uchar;
typedef unsigned uint;
typedef double dbl;

#define exit_if(cnd_value, msg) \
    do { \
        if (cnd_value) \
        { \
            if (errno) \
                perror(msg); \
            else \
                fprintf(stderr, "error: %s\n", msg); \
            exit(EXIT_FAILURE); \
        } \
    } while (0)

#define cudaErrorCheck(error) \
    do { \
        cudaError_t result = error; \
        if (result != cudaSuccess) { \
            fprintf(stderr, "cuda %s:%d error: %s\n", __FILE__, __LINE__, \
                cudaGetErrorString(result)); \
            exit(EXIT_FAILURE); \
        } \
    } while(0)

texture<uchar4, cudaTextureType2D, cudaReadModeElementType> tex;

__device__ __host__ int my_min(const int a, const int b) {
    const int diff = a - b, shift = CHAR_BIT * sizeof(int) - 1;
    return b + (diff & (diff >> shift));
}

__device__ dbl filter(const uchar4 pixel) {
    return pixel.x * 0.299 + pixel.y * 0.587 + pixel.z * 0.114;
}

__global__ void kernel(
    uchar4 * const __restrict__ data,
    const uint w, const uint h
) {
    const uint
        idxX = threadIdx.x + blockDim.x * blockIdx.x,
        idxY = threadIdx.y + blockDim.y * blockIdx.y,
        offsetX = blockDim.x * gridDim.x, offsetY = blockDim.y * gridDim.y;
    for (uint x = idxX; x < w; x += offsetX)
        for (uint y = idxY; y < h; y += offsetY) {
            const uchar4 pixel = tex2D(tex, x, y);
            const uchar value = (uchar) my_min((int) filter(pixel), UCHAR_MAX);
            data[y * w + x] = make_uchar4(value, value, value, pixel.w);
        }
}

int main() {
    char fileIn[FILENAME_MAX], fileOut[FILENAME_MAX];
    scanf("%s%s", fileIn, fileOut);

    FILE * const streamIn = fopen(fileIn, "rb");
    exit_if(!streamIn, "fopen()");

    uint w, h;
    fread(&w, sizeof(uint), 1, streamIn);
    fread(&h, sizeof(uint), 1, streamIn);

    uchar4 * const img = (uchar4 *) malloc(sizeof(uchar4) * w * h);
    exit_if(img == NULL, "malloc()");

    fread(img, sizeof(uchar4), h * w, streamIn);
    fclose(streamIn);

    cudaChannelFormatDesc channel = cudaCreateChannelDesc<uchar4>();
    cudaErrorCheck(cudaGetLastError());

    cudaArray *deviceArray;
    cudaErrorCheck(cudaMallocArray(&deviceArray, &channel, w, h));
    cudaErrorCheck(cudaMemcpyToArray(deviceArray, 0, 0, img,
        sizeof(uchar4) * w * h, cudaMemcpyHostToDevice));

    tex.addressMode[0] = cudaAddressModeClamp;
    tex.addressMode[1] = cudaAddressModeClamp;
    tex.channelDesc = channel;
    tex.filterMode = cudaFilterModePoint;
    tex.normalized = false;
    cudaErrorCheck(cudaBindTextureToArray(tex, deviceArray, channel));

    uchar4 *deviceImg;
    cudaErrorCheck(cudaMalloc(&deviceImg, sizeof(uchar4) * w * h));
    kernel<<<dim3(16U, 16U), dim3(16U, 16U)>>>(deviceImg, w, h);
    cudaErrorCheck(cudaGetLastError());
    cudaErrorCheck(cudaMemcpy(img, deviceImg, sizeof(uchar4) * w * h,
        cudaMemcpyDeviceToHost));

    FILE * const streamOut = fopen(fileOut, "wb");
    exit_if(streamOut == NULL, "fopen()");

    fwrite(&w, sizeof(uint), 1, streamOut);
    fwrite(&h, sizeof(uint), 1, streamOut);
    fwrite(img, sizeof(uchar4), w * h, streamOut);
    fclose(streamOut);

    cudaErrorCheck(cudaUnbindTexture(tex));
    cudaErrorCheck(cudaFreeArray(deviceArray));
    cudaErrorCheck(cudaFree(deviceImg));
    free(img);

    return 0;
}

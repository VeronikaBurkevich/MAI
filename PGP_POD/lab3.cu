#include <errno.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned uint;
typedef float flt;

#define exit_if(cnd_value, msg) \
    do { \
        if (cnd_value) { \
            if (errno) \
                perror(msg); \
            else \
                fprintf(stderr, "error: %s\n", msg); \
            exit(EXIT_FAILURE); \
        } \
    } while (0)

#define cudaErrorCheck(error) \
    do { \
        cudaError_t res = error; \
        if (res != cudaSuccess) { \
            fprintf(stderr, "cuda %s:%d error: %s\n", __FILE__, __LINE__, \
                cudaGetErrorString(res)); \
            exit(EXIT_FAILURE); \
        } \
    } while(0)

#define MU_COUNT (3U)
__device__ __constant__ int3 deviceMu[MU_COUNT];

__global__ static void classify(uchar4 * __restrict__, uint, uint);

__host__ static uchar4 *read_file(const char *, uint *, uint *);
__host__ static void write_file(
    const char *, const uchar4 * __restrict__, uint, uint
);

int main(void) {
    char fileIn[FILENAME_MAX], fileOut[FILENAME_MAX];
    scanf("%s%s", fileIn, fileOut);

    uint w, h;
    uchar4 * const __restrict__ data = read_file(fileIn, &w, &h);
    const uint size = sizeof(uchar4) * w * h;

    uchar4 *deviceData;
    cudaErrorCheck(cudaMalloc(&deviceData, size));
    cudaErrorCheck(cudaMemcpy(deviceData, data, size, cudaMemcpyHostToDevice));

    int3 hostMu[MU_COUNT] = {
        make_int3(255, 0, 0),
        make_int3(0, 255, 0),
        make_int3(0, 0, 255)
    };
    cudaErrorCheck(cudaMemcpyToSymbol(deviceMu, hostMu,
        sizeof(int3) * MU_COUNT
    ));

    classify<<<dim3(16U, 16U), dim3(16U, 16U)>>>(deviceData, w, h);
    cudaErrorCheck(cudaGetLastError());
    cudaErrorCheck(cudaMemcpy(data, deviceData, size, cudaMemcpyDeviceToHost));
    cudaErrorCheck(cudaFree(deviceData));

    write_file(fileOut, data, w, h);
    free(data);

    return 0;
}

__global__ static void classify(
    uchar4 * const __restrict__ data,
    const uint w, const uint h
) {
    const uint
        idxX = threadIdx.x + blockDim.x * blockIdx.x,
        idxY = threadIdx.y + blockDim.y * blockIdx.y,
        offsetX = blockDim.x * gridDim.x,
        offsetY = blockDim.y * gridDim.y;
    for (uint j = idxY; j < h; j += offsetY) {
        for (uint i = idxX; i < w; i += offsetX) {
            int min_distance = INT_MAX;
            const uint index = j * w + i;
            const uchar4 data_i_j = data[index];
            for (uint idx = 0; idx < MU_COUNT; ++idx) {
                const int distance =
                    (data_i_j.x - deviceMu[idx].x) * (data_i_j.x - deviceMu[idx].x) +
                    (data_i_j.y - deviceMu[idx].y) * (data_i_j.y - deviceMu[idx].y) +
                    (data_i_j.z - deviceMu[idx].z) * (data_i_j.z - deviceMu[idx].z);

                if (distance < min_distance) {
                    data[index].w = idx;
                    min_distance = distance;
                }
            }
        }
    }
}

__host__ static uchar4 *read_file(
    const char * const file, uint * const w, uint * const h
) {
    FILE * const stream = fopen(file, "rb");
    exit_if(stream == NULL, "fopen()");

    exit_if(fread(w, sizeof(uint), 1, stream) != 1, "fread()");
    exit_if(fread(h, sizeof(uint), 1, stream) != 1, "fread()");

    const uint count = (*w) * (*h);
    uchar4 * const data = (uchar4 *) malloc(sizeof(uchar4) * count);
    exit_if(data == NULL, "malloc()");

    exit_if(fread(data, sizeof(uchar4), count, stream) != count, "fread()");
    exit_if(fclose(stream) != 0, "close()");

    return data;
}

__host__ static void write_file(
    const char * const file, const uchar4 * const __restrict__ data,
    const uint w, const uint h
) {
    FILE * const stream = fopen(file, "wb");
    exit_if(stream == NULL, "fopen()");

    const uint count = w * h;
    exit_if(fwrite(&w, sizeof(uint), 1, stream) != 1, "fwrite()");
    exit_if(fwrite(&h, sizeof(uint), 1, stream) != 1, "fwrite()");
    exit_if(fwrite(data, sizeof(uchar4), count, stream) != count, "fwrite()");
    exit_if(fclose(stream) != 0, "fclose()");
}

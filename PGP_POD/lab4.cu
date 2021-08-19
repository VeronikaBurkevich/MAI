#include <stdio.h>
#include <stdlib.h>

#include <fstream>
#include <iomanip>
#include <iostream>

#include <thrust/device_vector.h>
#include <thrust/host_vector.h>

using namespace std;

typedef unsigned uint;
typedef double dbl;

#define TITLE_DIM (32U)

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

#define CSC(call) \
    do { \
        cudaError_t res = call; \
        if (res != cudaSuccess) { \
            fprintf(stderr, "ERROR in %s:%d. Message: %s\n", \
                __FILE__, __LINE__, cudaGetErrorString(res)); \
            exit(EXIT_FAILURE); \
        } \
    } while(0)

__host__ __device__ inline int mapping(
    const uint x, const uint y, const uint w, const uint h
) {
    return (y % h) * w + (x % w);
}

__global__ static void transpose(
    const dbl * const __restrict__ input, dbl * const __restrict__ output,
    const uint w, const uint h
) {
    __shared__ dbl title[TITLE_DIM][TITLE_DIM];

    const uint
        idxX = blockDim.x * blockIdx.x, idxY = blockDim.y * blockIdx.y,
        offsetX = blockDim.x * gridDim.x, offsetY = blockDim.y * gridDim.y;
    for (uint y = idxY; y < h; y += offsetY) {
        for (uint x = idxX; x < w; x += offsetX) {
            uint i = x + threadIdx.x, j = y + threadIdx.y;
            title[threadIdx.x][threadIdx.y] = input[mapping(i, j, w, h)];

            __syncthreads();

            i = x + threadIdx.y;
            j = y + threadIdx.x;
            if (i < w && j < h)
                output[mapping(j, i, h, w)] = title[threadIdx.y][threadIdx.x];
            __syncthreads();
        }
    }
}

static ostream &outMatrix(
    ostream &os,
    const dbl * const __restrict__ matrix,
    const uint w, const uint h
) {
    for (uint j = 0; j < h; ++j) {
        for (uint i = 0; i < w; ++i)
            os << setprecision(10) << scientific << matrix[j * w + i] << ' ';
        os << endl;
    }
    return os;
}

int main(const int argc, char ** const argv) {
    ios_base::sync_with_stdio(false);
    cerr.tie(nullptr);
    cin.tie(nullptr);

    uint w, h;
    cin >> h >> w;
    const uint size = sizeof(dbl) * w * h;
    dbl * const __restrict__ hostMatrix = (dbl *) malloc(size);
    exit_if(hostMatrix == NULL, "malloc()");

    for (uint j = 0; j < h; ++j)
        for (uint i = 0; i < w; ++i)
            cin >> hostMatrix[j * w + i];

    dbl *deviceInput, *deviceOutput;
    CSC(cudaMalloc(&deviceInput, size));
    CSC(cudaMemcpy(deviceInput, hostMatrix, size, cudaMemcpyHostToDevice));
    CSC(cudaMalloc(&deviceOutput, size));

    transpose<<<dim3(TITLE_DIM, TITLE_DIM), dim3(TITLE_DIM, TITLE_DIM)>>>(
        deviceInput, deviceOutput, w, h
    );
    CSC(cudaGetLastError());

    CSC(cudaMemcpy(hostMatrix, deviceOutput, size, cudaMemcpyDeviceToHost));
    CSC(cudaFree(deviceInput));
    CSC(cudaFree(deviceOutput));

    outMatrix(cout, hostMatrix, w, h);
    free(hostMatrix);

    return 0;
}

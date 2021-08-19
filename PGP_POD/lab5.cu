#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <thrust/device_vector.h>
#include <thrust/execution_policy.h>
#include <thrust/host_vector.h>
#include <thrust/scan.h>

#define MAX_VALUE ((1UL << 24) + 1U)
#define BLOCK_DIM (16U)
#define  GRID_DIM (16U)

typedef unsigned uint;

#define CSC(call) \
    do { \
        cudaError_t res = call; \
        if (res != cudaSuccess) { \
            fprintf(stderr, "ERROR in %s:%d. Message: %s\n", \
                __FILE__, __LINE__, cudaGetErrorString(res)); \
            exit(0); \
        } \
    } while(0)

__global__ static void hist(
    const int * __restrict__ const input,
    int * __restrict__ const counts,
    const uint count
) {
    const uint offset = blockDim.x * gridDim.x;
    uint idx = threadIdx.x + blockIdx.x * blockDim.x;
    for (; idx < count; idx += offset)
	   atomicAdd(counts + input[idx], 1);
}

__global__ static void sort(
    const int * __restrict__ const input,
    int * __restrict__ const counts,
    int * __restrict__ const output,
    const uint count
) {
    const uint offset = blockDim.x * gridDim.x;
    uint idx = threadIdx.x + blockIdx.x * blockDim.x;
    for (; idx < count; idx += offset) {
        const int i = atomicAdd(counts + input[idx], -1) - 1;
        output[i] = input[idx];
    }
}

int main(void) {
    uint count;
    fread(&count, sizeof(uint), 1U, stdin);
    const uint size = sizeof(int) * count;

    int *buffer = (int *) malloc(size);
    fread(buffer, sizeof(int), count, stdin);

    int *deviceInput;
    CSC(cudaMalloc(&deviceInput, size));
    CSC(cudaMemcpy(deviceInput, buffer, size, cudaMemcpyHostToDevice));

    int *deviceCounts;
    CSC(cudaMalloc(&deviceCounts, sizeof(int) * MAX_VALUE));
    CSC(cudaMemset(deviceCounts, 0, sizeof(int) * MAX_VALUE));

    int *deviceOutput;
    CSC(cudaMalloc(&deviceOutput, size));

    cudaEvent_t startTime, endTime;
    CSC(cudaEventCreate(&startTime));
    CSC(cudaEventCreate(&endTime));
    CSC(cudaEventRecord(startTime));

    hist<<<GRID_DIM, BLOCK_DIM>>>(deviceInput, deviceCounts, count);
    thrust::inclusive_scan(thrust::device,
        deviceCounts, deviceCounts + MAX_VALUE, deviceCounts
    );
    sort<<<GRID_DIM, BLOCK_DIM>>>(deviceInput, deviceCounts, deviceOutput, count);
    CSC(cudaGetLastError());

    CSC(cudaEventRecord(endTime));
    CSC(cudaEventSynchronize(endTime));
    float t;
    CSC(cudaEventElapsedTime(&t, startTime, endTime));
    CSC(cudaEventDestroy(startTime));
    CSC(cudaEventDestroy(endTime));

    CSC(cudaMemcpy(buffer, deviceOutput, sizeof(int) * count,
        cudaMemcpyDeviceToHost
    ));
    CSC(cudaFree(deviceInput));
    CSC(cudaFree(deviceOutput));

    fwrite(buffer, sizeof(int), count, stdout);
    free(buffer);

    return 0;
}

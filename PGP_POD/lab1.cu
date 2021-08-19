#include <iostream>
#include <iomanip>

using namespace std;

void Error(cudaError_t error)
{
    if (error != cudaSuccess){ 
        cout << "ERROR:" << cudaGetErrorString(error) << endl; 
        exit(0); 
    }
}
__global__ void sqr_items_vectors(double* a, double* result, int n)
{
    int tid = blockIdx.x * blockDim.x + threadIdx.x;
    while (tid < n) {
        result[tid] = a[tid]*a[tid];
        tid += blockDim.x*gridDim.x;
    }
}

int main()
{
    int n, size;
    double *a, *result;
    double *gpu_a, *gpu_result;

    cin >> n;

    size = sizeof(double) * n;

    a = (double*)malloc(size);
    result = (double*)malloc(size);

    for (int i = 0; i < n; ++i) {
        cin >> a[i];
    }

    Error(cudaMalloc(&gpu_a, size));
    Error(cudaMalloc(&gpu_result, size));
    Error(cudaMemcpy(gpu_a, a, size, cudaMemcpyHostToDevice));

    sqr_items_vectors<<<256, 256>>>(gpu_a, gpu_result, n);

    Error(cudaMemcpy(result, gpu_result, size, cudaMemcpyDeviceToHost));

    for (int i = 0; i < n; ++i) {
        cout << scientific << setprecision(10) << result[i] << " ";
    }
    cout << endl;

    Error(cudaFree(gpu_a));
    Error(cudaFree(gpu_result));
    free(a);
    free(result);

    return 0;
}

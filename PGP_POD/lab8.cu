#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <mpi.h>
#include <omp.h>

typedef unsigned uint;
typedef double dbl;

#include <thrust/extrema.h>
            #include <thrust/device_vector.h>

            #define CSC(call)                                            \
            do {                                                         \
        cudaError_t res = call;                                          \
        if (res != cudaSuccess) {                                        \
            fprintf(stderr, "ERROR in %s:%d. Message: %s\n",             \
                    __FILE__, __LINE__, cudaGetErrorString(res));        \
            exit(0);                                                     \
        }                                                                \
    } while(0)

            #define _i(i, j) (((j) + 1) * (x_block + 2) + (i) + 1)
            #define _ix(id) (((id) % (x_block + 2)) - 1)
            #define _iy(id) (((id) / (y_block + 2)) - 1)

            #define _ib(i, j) ((j) * x_gr + (i))
            #define _ibx(id) ((id) % x_gr)
            #define _iby(id) ((id) / x_gr)

    __global__ static void kernel_copy_x(
        dbl *side, 
        dbl *data, 
        int x_block, 
        int j, 
        bool direction, 
        dbl value
    ) {
        const uint 
            id = blockIdx.x * blockDim.x + threadIdx.x,
            offset = blockDim.x * gridDim.x;
        int i;

        if (!direction) {
            if (!side) {
                i = id;
                while(i < x_block){
                    data[_i(i, j)] = value;
                    i += offset;
                }
            } else{
                i = id;
                while(i < x_block){
                    data[_i(i, j)] = side[i];
                    i += offset;
                }
            }
        } else {
            i = id;
            while(i < x_block){
                side[i] = data[_i(i, j)];
                i += offset;
            }
        }
    }

    __global__ static void kernel_copy_y(
        dbl *side, 
        dbl *data, 
        int y_block,
        int x_block,  
        int i, 
        bool direction, 
        dbl value
    ) {
        const uint 
            id = blockIdx.y * blockDim.y + threadIdx.y,
            offset = blockDim.y * gridDim.y;
        int j;

        if (!direction) {
            if (!side) {
                j = id;
                while(j < y_block){
                    data[_i(i, j)] = value;
                    j += offset;
                }            
            } else{
                j = id;
                while(j < y_block){
                    data[_i(i, j)] = side[j];
                    j += offset;
                }
            }
        } else {
            j = id;
            while(j < y_block){
                side[j] = data[_i(i, j)];
                j += offset;
            }
        }
    }

    __global__ static void kernel_calculate_temp(
        dbl* next, 
        dbl* data, 
        int x_block, 
        int y_block, 
        dbl hx, 
        dbl hy
    ){
        const uint 
            x_id = blockIdx.x * blockDim.x + threadIdx.x,
            y_id = blockIdx.y * blockDim.y + threadIdx.y,
            x_offset = blockDim.x * gridDim.x,
            y_offset = blockDim.y * gridDim.y;
        int i, j;

        for (i = x_id; i < x_block; i += x_offset) {
            for (j = y_id; j < y_block; j += y_offset) {
                dbl 
                    u_i = (data[_i(i + 1, j)] + data[_i(i - 1, j)]) / hx,
                    u_j = (data[_i(i, j + 1)] + data[_i(i, j - 1)]) / hy;

                next[_i(i, j)] = 0.5 * (u_i + u_j) / (1.0 / hx + 1.0 / hy);
            }
        }
    }

    __global__ static void kernel_errors(
        dbl* next, 
        dbl* data, 
        dbl* diff, 
        int x_block, 
        int y_block
    ){
        const uint 
            x_id = blockIdx.x * blockDim.x + threadIdx.x,
            y_id = blockIdx.y * blockDim.y + threadIdx.y,
            x_offset = blockDim.x * gridDim.x,
            y_offset = blockDim.y * gridDim.y;
        int i, j;

        for (i = x_id - 1; i <= x_block; i += x_offset) {
            for (j = y_id - 1; j <= y_block; j += y_offset) {
                diff[_i(i, j)] = (i != -1 && j != -1 && i != x_block && j != y_block) ? abs(next[_i(i, j)] - data[_i(i, j)]) : 0;
            }
        }

    }

    int main(int argc, char *argv[]) {
        int id, 
            x_block, y_block, 
            x_gr, y_gr;
        int i, j;
        int count, outputFileLength = 256;
        char out_file[256];
        dbl lx, ly, 
            backU, frontU, 
            leftU, rightU, 
            startU,
            eps, 
            maxError = 0.0;

        MPI_Status status;
        MPI_Init(&argc, &argv);
        MPI_Comm_size(MPI_COMM_WORLD, &count);
        MPI_Comm_rank(MPI_COMM_WORLD, &id);

        fprintf(stderr, "proc %d(%d)\n", id, count);
        fflush(stderr);

        MPI_Barrier(MPI_COMM_WORLD);

        if (id < 1) {
            std::cin >> x_gr >> y_gr;
            std::cin >> x_block >> y_block;
            std::cin >> out_file;
            std::cin >> eps;
            std::cin >> lx >> ly;
            std::cin >> leftU >> rightU >> frontU >> backU;
            std::cin >> startU;
        }

        MPI_Bcast(&x_gr, 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Bcast(&y_gr, 1, MPI_INT, 0, MPI_COMM_WORLD);

        MPI_Bcast(&x_block, 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Bcast(&y_block, 1, MPI_INT, 0, MPI_COMM_WORLD);

        MPI_Bcast(&eps, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

        MPI_Bcast(&lx, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        MPI_Bcast(&ly, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

        MPI_Bcast(&backU, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        MPI_Bcast(&frontU, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        MPI_Bcast(&leftU, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        MPI_Bcast(&rightU, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

        MPI_Bcast(&startU, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

        outputFileLength = strlen(out_file) + 1;
        MPI_Bcast(&outputFileLength, 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Bcast(out_file, outputFileLength, MPI_CHAR, 0, MPI_COMM_WORLD);

        if (id < 1) {
            fprintf(stderr, "x_gr: %d y_gr: %d\n", x_gr, y_gr);
            fprintf(stderr, "x_block: %d y_block: %d\n", x_block, y_block);
            fprintf(stderr, "eps: %lf\n", eps);
            fprintf(stderr, "lx: %lf ly: %lf\n", lx, ly);
            fprintf(stderr, "backU: %lf frontU: %lf leftU: %lf rightU: %lf\n", backU, frontU, leftU, rightU);
            fprintf(stderr, "startU: %lf\n", startU);
        }

        if (x_gr * y_gr * x_block * y_block == 0) {
            fprintf(stderr, "Error at proc %d\n", id);
            if (x_gr * y_gr != count) {
                fprintf(stderr, "Dead because of blocks\n");
                fprintf(stderr, "x_gr=%d, y_gr=%d, count=%d\n", x_gr, y_gr, count);
            }
            fflush(stderr);
            MPI_Finalize();
            return 0;
        }

        int 
            ib = _ibx(id),
            jb = _iby(id);

        dbl 
            hx = lx / ((dbl) x_gr * (dbl) x_block),
            hy = ly / ((dbl) y_gr * (dbl) y_block);

        hx *= hx;
        hy *= hy;

        dbl size = (x_block + 2) * (y_block + 2);
 
        dbl
            * data = (dbl *)malloc(sizeof(dbl) * size),
            * next = (dbl *)malloc(sizeof(dbl) * size),
            * edge_x = (dbl *)malloc(sizeof(dbl) * x_block),
            * edge_y = (dbl *)malloc(sizeof(dbl) * y_block);

        for (i = 0; i < x_block; i++) {
            for (j = 0; j < y_block; j++) {
                data[_i(i, j)] = startU;
            }
        }

        dbl             //
            *gpu_data,     //
            *gpu_next,     //
            *gpu_error,    //
            *gpu_edge_x,   //
            *gpu_edge_y;   //

        CSC(cudaMalloc(&gpu_data, sizeof(dbl) * size));
        CSC(cudaMemcpy(gpu_data, data, sizeof(dbl) * size, cudaMemcpyHostToDevice));

        CSC(cudaMalloc(&gpu_next, sizeof(dbl) * size));
        CSC(cudaMemcpy(gpu_next, next, sizeof(dbl) * size, cudaMemcpyHostToDevice));

        CSC(cudaMalloc(&gpu_error, sizeof(dbl) * size));

        CSC(cudaMalloc(&gpu_edge_x, sizeof(dbl) * x_block));
        CSC(cudaMemcpy(gpu_edge_x, edge_x, sizeof(dbl) * x_block, cudaMemcpyHostToDevice));

        CSC(cudaMalloc(&gpu_edge_y, sizeof(dbl) * y_block));
        CSC(cudaMemcpy(gpu_edge_y, edge_y, sizeof(dbl) * y_block, cudaMemcpyHostToDevice));

        dim3 blocks(32, 32);
        dim3 threads(32, 32);

        do {
            if (ib < x_gr - 1) {
                kernel_copy_y<<<blocks, threads>>>(gpu_edge_y, gpu_data, y_block, x_block, x_block - 1, true, startU);
                CSC(cudaGetLastError());
                CSC(cudaMemcpy(edge_y, gpu_edge_y, sizeof(dbl) * y_block, cudaMemcpyDeviceToHost));
                MPI_Bsend(edge_y, y_block, MPI_DOUBLE, _ib(ib + 1, jb), 0, MPI_COMM_WORLD);
            }
            if (jb < y_gr - 1) {
                kernel_copy_x<<<blocks, threads>>>(gpu_edge_x, gpu_data, x_block, y_block - 1, true, startU);
                CSC(cudaGetLastError());
                CSC(cudaMemcpy(edge_x, gpu_edge_x, sizeof(dbl) * x_block, cudaMemcpyDeviceToHost));
                MPI_Bsend(edge_x, x_block, MPI_DOUBLE, _ib(ib, jb + 1), 0, MPI_COMM_WORLD);
            }

            if (ib <= 0) {
                kernel_copy_y<<<blocks, threads>>>(NULL, gpu_data, y_block, x_block, -1, false, leftU);
                CSC(cudaGetLastError());
            } else {
                MPI_Recv(edge_y, y_block, MPI_DOUBLE, _ib(ib - 1, jb), 0, MPI_COMM_WORLD, &status);
                CSC(cudaMemcpy(gpu_edge_y, edge_y, sizeof(dbl) * y_block, cudaMemcpyHostToDevice));
                kernel_copy_y<<<blocks, threads>>>(gpu_edge_y, gpu_data, y_block, x_block, -1, false, startU);
                CSC(cudaGetLastError());
            }

            if (jb <= 0) {
                kernel_copy_x<<<blocks, threads>>>(NULL, gpu_data, x_block, -1, false, frontU);
                CSC(cudaGetLastError());
            } else {
                MPI_Recv(edge_x, x_block, MPI_DOUBLE, _ib(ib, jb - 1), 0, MPI_COMM_WORLD, &status);
                CSC(cudaMemcpy(gpu_edge_x, edge_x, sizeof(dbl) * x_block, cudaMemcpyHostToDevice));
                kernel_copy_x<<<blocks, threads>>>(gpu_edge_x, gpu_data, x_block, -1, false, startU);
                CSC(cudaGetLastError());
            }

            if (ib >= 1) {
                kernel_copy_y<<<blocks, threads>>>(gpu_edge_y, gpu_data, y_block, x_block, 0, true, startU);
                CSC(cudaGetLastError());
                CSC(cudaMemcpy(edge_y, gpu_edge_y, sizeof(dbl) * y_block, cudaMemcpyDeviceToHost));
                MPI_Bsend(edge_y, y_block, MPI_DOUBLE, _ib(ib - 1, jb), 0, MPI_COMM_WORLD);
            }

            if (jb >= 1) {
                kernel_copy_x<<<blocks, threads>>>(gpu_edge_x, gpu_data, x_block, 0, true, startU);
                CSC(cudaGetLastError());
                CSC(cudaMemcpy(edge_x, gpu_edge_x, sizeof(dbl) * x_block, cudaMemcpyDeviceToHost));
                MPI_Bsend(edge_x, x_block, MPI_DOUBLE, _ib(ib, jb - 1), 0, MPI_COMM_WORLD);
            }

            if (ib >= x_gr - 1) {
                kernel_copy_y<<<blocks, threads>>>(NULL, gpu_data, y_block, x_block, x_block, false, rightU);
                CSC(cudaGetLastError());
            } else {
                MPI_Recv(edge_y, y_block, MPI_DOUBLE, _ib(ib + 1, jb), 0, MPI_COMM_WORLD, &status);
                cudaMemcpy(gpu_edge_y, edge_y, sizeof(dbl) * y_block, cudaMemcpyHostToDevice);
                kernel_copy_y<<<blocks, threads>>>(gpu_edge_y, gpu_data, y_block, x_block, x_block, false, startU);
                CSC(cudaGetLastError());
            }

            if (jb >= y_gr - 1) {
                kernel_copy_x<<<blocks, threads>>>(NULL, gpu_data, x_block, y_block, false, backU);
                CSC(cudaGetLastError());
            } else {
                MPI_Recv(edge_x, x_block, MPI_DOUBLE, _ib(ib, jb + 1), 0, MPI_COMM_WORLD, &status);
                cudaMemcpy(gpu_edge_x, edge_x, sizeof(dbl) * x_block, cudaMemcpyHostToDevice);
                kernel_copy_x<<<blocks, threads>>>(gpu_edge_x, gpu_data, x_block, y_block, false, startU);
                CSC(cudaGetLastError());
            }

            cudaThreadSynchronize();
            kernel_calculate_temp<<<dim3(8, 8, 8), dim3(32, 4, 4)>>>(gpu_next, gpu_data, x_block, y_block, hx, hy);
            CSC(cudaGetLastError());

            cudaThreadSynchronize();
            kernel_errors<<<dim3(8, 8, 8), dim3(32, 4, 4)>>>(gpu_next, gpu_data, gpu_error, x_block, y_block);
            CSC(cudaGetLastError());

            thrust::device_ptr<dbl> p_arr = thrust::device_pointer_cast(gpu_error);
            thrust::device_ptr<dbl> thrust_max = thrust::max_element(p_arr, p_arr + size);

            dbl gpu_err = 0.0;
            gpu_err = *thrust_max;

            MPI_Allreduce(&gpu_err, &maxError, 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);

            dbl *temp = gpu_data;
            gpu_data = gpu_next;
            gpu_next = temp;

        } while (maxError > eps);

        MPI_Barrier(MPI_COMM_WORLD);

        CSC(cudaMemcpy(data, gpu_data, sizeof(dbl) * size, cudaMemcpyDeviceToHost));
        CSC(cudaFree(gpu_data));
        CSC(cudaFree(gpu_next));
        CSC(cudaFree(gpu_error));
        CSC(cudaFree(gpu_edge_x));
        CSC(cudaFree(gpu_edge_y));

        int 
            nSize = 14,
            b_size = x_block * y_block,
            nBlockSize = b_size * nSize;

        char* buff = (char *)malloc(sizeof(char) * nBlockSize);
        memset(buff, ' ', nBlockSize * sizeof(char));

        for(j = 0; j < y_block; j++) {
            for (i = 0; i < x_block; i++) {
                sprintf(buff + (j * x_block + i) * nSize, " %.6e", data[_i(i, j)]);
            }
            if (ib == x_gr - 1) {
                buff[(j + 1) * x_block * nSize - 1] = '\n';
            }
        }

        i = 0;
        while (i < nBlockSize){
            if (buff[i] == '\0') {
                buff[i] = ' ';
            }
            ++i;
        }

        MPI_Datatype numType, rowType, filetype;

        MPI_Type_contiguous(nSize, MPI_CHAR, &numType);
        MPI_Type_contiguous(x_block, numType, &rowType);

        MPI_Type_create_hvector(y_block, 1, x_block * x_gr * nSize, rowType, &filetype);
        MPI_Type_commit(&filetype);

        MPI_File fp;
        MPI_File_delete(out_file, MPI_INFO_NULL);
        MPI_File_open(MPI_COMM_WORLD, out_file, MPI_MODE_CREATE | MPI_MODE_WRONLY, MPI_INFO_NULL, &fp);

        MPI_File_set_view(fp, (jb * b_size * x_gr + ib * x_block) * nSize, MPI_CHAR, filetype, "native", MPI_INFO_NULL);
        MPI_File_write_all(fp, buff, nBlockSize, MPI_CHAR, MPI_STATUS_IGNORE);

        MPI_File_close(&fp);

        MPI_Finalize();

        free(buff);
        free(data);
        free(next);
        free(edge_x);
        free(edge_y);

        return 0;
}
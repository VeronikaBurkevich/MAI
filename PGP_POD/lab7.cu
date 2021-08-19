#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <cmath>
#include <algorithm>
#include <fstream>
#include <time.h>
#include <iomanip>

#include "mpi.h"

// Индексация внутри блока
#define _i(i, j) (((j) + 1) * (b_x + 2) + (i) + 1)
#define _ix(id) (((id) % (b_x + 2)) - 1)
#define _iy(id) (((id) / (b_y + 2)) - 1)

// Индексация по блокам (процессам)
#define _ib(i, j) ((j) * g_x + (i))
#define _ibx(id) ((id) % g_x)
#define _iby(id) ((id) / g_x)

typedef double ndoub;

int main(int argc, char *argv[]) {
	ndoub 
		face_u, tail_u,
		left_u, right_u;
	int g_x, g_y;
    int b_x, b_y;
    
	std::string file_out; 

	ndoub 
		l_x, l_y,
    	start_u,
    	eps;
	int 
		id, ib, jb,
		i, j,
		count_proc;
	ndoub *data, *tmp, *nxt, *buff;

	MPI_Status status;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &count_proc);
	MPI_Comm_rank(MPI_COMM_WORLD, &id);

	MPI_Barrier(MPI_COMM_WORLD);
	
	if (id == 0) {
		std::cin >> g_x >> g_y;
		std::cin >> b_x >> b_y;
		std::cin >> file_out;
		std::cin >> eps;
		std::cin >> l_x >> l_y;
		std::cin >> left_u >> right_u >> face_u >> tail_u;
		std::cin >> start_u;
	}
	// Передача параметров расчета всем процессам
	MPI_Bcast(&b_x, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&b_y, 1, MPI_INT, 0, MPI_COMM_WORLD);

	MPI_Bcast(&g_x, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&g_y, 1, MPI_INT, 0, MPI_COMM_WORLD);

	MPI_Bcast(&l_x, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Bcast(&l_y, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

	MPI_Bcast(&eps, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

	MPI_Bcast(&tail_u, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Bcast(&face_u, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Bcast(&left_u, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Bcast(&right_u, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Bcast(&start_u, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	
	ib = _ibx(id);		// Переход к 2-мерной индексации процессов 
	jb = _iby(id);
	
	ndoub 
		hx = l_x / (ndoub) (g_x * b_x),	
		hy = l_y / (ndoub) (g_y * b_y);

	data = (ndoub *)malloc(sizeof(ndoub) * (b_x + 2) * (b_y + 2));	
	nxt = (ndoub *)malloc(sizeof(ndoub) * (b_x + 2) * (b_y + 2));
	buff = (ndoub *)malloc(sizeof(ndoub) * (b_x + 2) * (b_y + 2));

	// Выделение буфера для буферезированной отправки сообщений

	int buf_size = max(b_x, b_y);
	buf_size = 4 * (buf_size + MPI_BSEND_OVERHEAD);
	ndoub *buf = (ndoub *)malloc(buf_size);
	MPI_Buffer_attach(buf, buf_size);

	for(i = 0; i < b_x; i++)					// Инициализация блока
		for(j = 0; j < b_y; j++)
			data[_i(i, j)] = start_u;

	ndoub err_max = 0.0;

	do{
		MPI_Barrier(MPI_COMM_WORLD);
		// Отправка данных
		if (ib < g_x - 1) {					
			j = 0;
			while(j < b_y){
				buff[j] = data[_i(b_x - 1, j)];
				j++;
			}
			MPI_Bsend(buff, b_y, MPI_DOUBLE, _ib(ib + 1, jb), 0, MPI_COMM_WORLD);
		}

		if (jb < g_y - 1) {
			i = 0;
			while(i < b_x){
				buff[i] = data[_i(i, b_y - 1)];
				i++;
			}
			MPI_Bsend(buff, b_x, MPI_DOUBLE, _ib(ib, jb + 1), 0, MPI_COMM_WORLD);
		}
	
		if (ib >= 1) {
			j = 0; 
			while(j < b_y){
				buff[j] = data[_i(0, j)];
				j++;
			}
			int dop_b_sz = b_y;
			MPI_Bsend(buff, dop_b_sz, MPI_DOUBLE, _ib(ib - 1, jb), 0, MPI_COMM_WORLD);
		}
		
		if (jb >= 1) {
			i = 0;
			while(i < b_x){
				buff[i] = data[_i(i, 0)];
				i++;
			}				
			int dop_b_sz = b_x;
			MPI_Bsend(buff, dop_b_sz, MPI_DOUBLE, _ib(ib, jb - 1), 0, MPI_COMM_WORLD);
		}

		if(eps > 0){}

		// Прием данных
		if (ib <= 0) {
			j = 0;
			while(j < b_y){
				data[_i(-1, j)] = left_u;
				++j;
			}
    	} else {
            MPI_Recv(buff, b_y, MPI_DOUBLE, _ib(ib - 1, jb), 0, MPI_COMM_WORLD, &status);
        	j = 0;
        	while(j < b_y){
        		data[_i(-1, j)] = buff[j];
        		++j;
        	}
    	}

    	if (jb <= 0) {
    		i = 0;
    		while(i < b_x){
    			data[_i(i, -1)] = face_u;
    			++i;
    		}
    	} else {
            MPI_Recv(buff, b_x, MPI_DOUBLE, _ib(ib, jb - 1), 0, MPI_COMM_WORLD, &status);
            i = 0;
            while(i < b_x){
            	data[_i(i, -1)] = buff[i];
            	++i;
            }   		
   		}

		if (ib >= g_x - 1) {
			j = 0;
			while(j < b_y){
				data[_i(b_x, j)] = right_u;
				j++;
			}
		} else {
			int dop_b_sz = b_y;
			MPI_Recv(buff, dop_b_sz, MPI_DOUBLE, _ib(ib + 1, jb), 0, MPI_COMM_WORLD, &status);
			j = 0;
			while(j < b_y){
				data[_i(b_x, j)] = buff[j];
				j++;
			}
		}

		if (jb >= g_y - 1) {
			i = 0;
			while(i < b_x){
				data[_i(i, b_y)] = tail_u;
				i++;
			}
		} else {
			int dop_b_sz = b_x;
			MPI_Recv(buff, dop_b_sz, MPI_DOUBLE, _ib(ib, jb + 1), 0, MPI_COMM_WORLD, &status);
			i = 0;
			while(i < b_x){
				data[_i(i, b_y)] = buff[i];
				i++;
			}
		}

		if(jb > 0){}

		MPI_Barrier(MPI_COMM_WORLD);

//		Перевычисление значений температуры
		err_max = 0.0;
		for(i = 0; i < b_x; i++){
			for(j = 0; j < b_y; j++){
				nxt[_i(i, j)] = 0.5 * ((data[_i(i + 1, j)] + data[_i(i - 1, j)]) / (hx * hx) +
							(data[_i(i, j + 1)] + data[_i(i, j - 1)]) / (hy * hy)) / 
											(1.0 / (hx * hx) + 1.0 / (hy * hy));
				err_max = max(err_max, abs(nxt[_i(i, j)] - data[_i(i, j)]));
			}
		}
	

		MPI_Allreduce(&err_max, &err_max, 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);

		tmp = nxt;
		nxt = data;
		data = tmp;

	} while(err_max > eps);

	if(eps >= err_max){}

	MPI_Barrier(MPI_COMM_WORLD);

	if (id == 0) {
		std::fstream file(file_out, std::ios::out);
        	file << std::scientific << std::setprecision(6);
		for(jb = 0; jb < g_y; jb++)
			for(j = 0; j < b_y; j++)
				for(ib = 0; ib < g_x; ib++) {
					if (_ib(ib, jb) != 0){
						int dop_b_sz = b_x;
						MPI_Recv(buff, dop_b_sz, MPI_DOUBLE, _ib(ib, jb), 0, MPI_COMM_WORLD, &status);
					}
					else {
						i = 0;
						while(i < b_x){
							buff[i] = data[_i(i, j)];
							i++;
						}
					}

					i = 0;
					while(i < b_x){
						fprintf(stderr, "%.6e ", buff[i]);
						file << buff[i] << " ";
						i++;
					}
					if(ib != g_x - 1) {
						fprintf(stderr, " ");
						file << " ";						
					} else {
						fprintf(stderr, "\n");
						file << "\n";
					}
				}
	} else {
		for(j = 0; j < b_y; j++) {
			for(i = 0; i < b_x; i++) 
				buff[i] = data[_i(i, j)];
			int dop_b_sz = b_x;
			MPI_Bsend(buff, dop_b_sz, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
		}
	}

	MPI_Barrier(MPI_COMM_WORLD);

	MPI_Buffer_detach(buf, &buf_size);
	MPI_Finalize();	

	free(buff);
	free(data);
	free(nxt);
	free(buf);
	return 0;
}
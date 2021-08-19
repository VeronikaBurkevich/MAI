#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <string>
#include <cmath>
#include <iomanip>
#include <fstream>
#include <algorithm>
#include <bits/stdc++.h>
#include <omp.h>
#include "mpi.h"
using namespace std;
// Индексация внутри блока
#define _i(i, j) (((j) + 1) * (x_b + 2) + (i) + 1)
#define _ix(id) (((id) % (x_b + 2)) - 1)
#define _iy(id) (((id) / (y_b + 2)) - 1)
// Индексация по блокам (процессам)
#define _ib(i, j) ((j) * x_g + (i))
#define _ibx(id) ((id) % x_g)
#define _iby(id) ((id) / x_g)

typedef unsigned uint;
typedef double dbl;

int main(int argc, char* argv[]) {
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);
	cout.tie(NULL);

	dbl 
		face_u, 
		tail_u, 
		left_u, 
		right_u,
		first_u,
		lx, ly,
		eps;

	uint x_g, y_g, x_b, y_b;

	string out_file;

	int 
		id, ib, jb,
		i, j,
		count;
	dbl * data, * temp, * next, * buff,
		* x_s, * y_s;

	MPI_Status status;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &count);
	MPI_Comm_rank(MPI_COMM_WORLD, &id);

	MPI_Barrier(MPI_COMM_WORLD);

	if (id < 1) {
		std::cin >> x_g >> y_g;
		std::cin >> x_b >> y_b;
		std::cin >> out_file;
		std::cin >> eps;
		std::cin >> lx >> ly;
		std::cin >> left_u >> right_u >> face_u >> tail_u;
		std::cin >> first_u;
	}
	// Передача параметров расчета всем процессам
	MPI_Bcast(&x_b, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&y_b, 1, MPI_INT, 0, MPI_COMM_WORLD);

	MPI_Bcast(&x_g, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&y_g, 1, MPI_INT, 0, MPI_COMM_WORLD);

	MPI_Bcast(&lx, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Bcast(&ly, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

	MPI_Bcast(&eps, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

	MPI_Bcast(&tail_u, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Bcast(&face_u, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Bcast(&left_u, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Bcast(&right_u, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Bcast(&first_u, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

	ib = _ibx(id);		// Переход к 2-мерной индексации процессов 
	jb = _iby(id);

	dbl 
		hx = lx / (dbl)(x_g * x_b),
		hy = ly / (dbl)(y_g * y_b),
		sz_max,
		err_max;

	data = (dbl*)malloc(sizeof(dbl) * (x_b + 2) * (y_b + 2));
	next = (dbl*)malloc(sizeof(dbl) * (x_b + 2) * (y_b + 2));

	sz_max = max(x_b, y_b);

	x_s = (dbl*)malloc(sizeof(dbl) * sz_max);
	y_s = (dbl*)malloc(sizeof(dbl) * sz_max);

	buff = (dbl*)malloc(sizeof(dbl) * sz_max);

	for (i = 0; i < x_b; i++)					// Инициализация блока
		for (j = 0; j < y_b; j++)
			data[_i(i, j)] = first_u;

	err_max = 0.0;
	omp_set_num_threads(2);
	do {
		MPI_Barrier(MPI_COMM_WORLD);
		// Отправка данных
		if (ib < x_g - 1) {
			j = 0;
			while(j < y_b){
				y_s[j] = data[_i(x_b - 1, j)];
				j++;
			}	
			MPI_Bsend(y_s, y_b, MPI_DOUBLE, _ib(ib + 1, jb), 0, MPI_COMM_WORLD);
		}

		if (jb < y_g - 1) {
			i = 0;
			while(i < x_b){
				x_s[i] = data[_i(i, y_b - 1)];
				i++;
			}	
			MPI_Bsend(x_s, x_b, MPI_DOUBLE, _ib(ib, jb + 1), 0, MPI_COMM_WORLD);
		}
		// Прием данных
		if (ib <= 0) {
			#pragma omp parallel for private(i, j) shared(data)
			for(j = 0; j < y_b; ++j){
				data[_i(-1, j)] = left_u;	
			}	
		}
		else {
			MPI_Recv(y_s, y_b, MPI_DOUBLE, _ib(ib - 1, jb), 0, MPI_COMM_WORLD, &status);
			#pragma omp parallel for private(i, j) shared(data, y_s)
			for(j = 0; j < y_b; ++j){
				data[_i(-1, j)] = y_s[j];
			}	
		}

		if (jb <= 0) {
			#pragma omp parallel for private(i, j) shared(data)
			for(i = 0; i < x_b; ++i){
				data[_i(i, -1)] = face_u;
			}	
		}
		else {
			MPI_Recv(x_s, x_b, MPI_DOUBLE, _ib(ib, jb - 1), 0, MPI_COMM_WORLD, &status);
			#pragma omp parallel for private(i, j) shared(data, x_s)
			for (i = 0; i < x_b; ++i){
				data[_i(i, -1)] = x_s[i];
			}
		}

		//Отправить
		if (ib >= 1) {
			j = 0;
			while(j < y_b){
				y_s[j] = data[_i(0, j)];
				j++;
			}
			int tempBlockSize = y_b;
			MPI_Bsend(y_s, tempBlockSize, MPI_DOUBLE, _ib(ib - 1, jb), 0, MPI_COMM_WORLD);
		}

		if (jb >= 1) {
			i = 0;
			while(i < x_b){
				x_s[i] = data[_i(i, 0)];
				i++;
			}	
			int tempBlockSize = x_b;
			MPI_Bsend(x_s, tempBlockSize, MPI_DOUBLE, _ib(ib, jb - 1), 0, MPI_COMM_WORLD);
		}

		//Принять
		if (ib >= x_g - 1) {
			#pragma omp parallel for private(i, j) shared(data)
			for (j = 0; j < y_b; j++){
				data[_i(x_b, j)] = right_u;				
			}
		}
		else {
			int tempBlockSize = y_b;
			MPI_Recv(y_s, tempBlockSize, MPI_DOUBLE, _ib(ib + 1, jb), 0, MPI_COMM_WORLD, &status);
			#pragma omp parallel for private(i, j) shared(data, y_s)
			for (j = 0; j < y_b; j++){
				data[_i(x_b, j)] = y_s[j];
			}	
		}

		if (jb >= y_g - 1) {
			i = 0;
			while(i < x_b){
				data[_i(i, y_b)] = tail_u;
				i++;
			}		
		}
		else {
			int tempBlockSize = x_b;
			MPI_Recv(x_s, tempBlockSize, MPI_DOUBLE, _ib(ib, jb + 1), 0, MPI_COMM_WORLD, &status);
			#pragma omp parallel for private(i, j) shared(data, x_s)
			for (i = 0; i < x_b; i++){
				data[_i(i, y_b)] = x_s[i];
			}	
		}

		MPI_Barrier(MPI_COMM_WORLD);

		//		Перевычисление значений температуры
		#pragma omp parallel for private(i, j) shared(data, next)
		for (i = 0; i < x_b; i++) {
			for (j = 0; j < y_b; j++) {
				next[_i(i, j)] = 0.5 * ((data[_i(i + 1, j)] + data[_i(i - 1, j)]) / (hx * hx) +
					(data[_i(i, j + 1)] + data[_i(i, j - 1)]) / (hy * hy)) /
					(1.0 / (hx * hx) + 1.0 / (hy * hy));
				err_max = max(err_max, abs(next[_i(i, j)] - data[_i(i, j)]));
			}
		}
		//error
		err_max = 0.0;
		#pragma omp parallel for private(i, j) shared(data, next) reduction(max: err_max)
		for (i = 0; i < x_b; ++i) {
			for (j = 0; j < y_b; ++j) {
				err_max = max(err_max, abs(next[_i(i, j)] - data[_i(i, j)]));
			}
		}
		MPI_Allreduce(&err_max, &err_max, 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);

		temp = next;
		next = data;
		data = temp;
	} while (err_max >= eps);

	MPI_Barrier(MPI_COMM_WORLD);

	if (id == 0) {
		std::fstream file(out_file, std::ios::out);
		file << std::scientific << std::setprecision(6);

		for (jb = 0; jb < y_g; jb++)
			for (j = 0; j < y_b; j++)
				for (ib = 0; ib < x_g; ib++) {
					if (_ib(ib, jb) != 0) {
						int tempBlockSize = x_b;
						MPI_Recv(buff, tempBlockSize, MPI_DOUBLE, _ib(ib, jb), 0, MPI_COMM_WORLD, &status);
					}
					else {
						i = 0;
						while(i < x_b){
							buff[i] = data[_i(i, j)];
							i++;
						}
					}
					i = 0;
					while(i < x_b){
						fprintf(stderr, "%.6e ", buff[i]);
						file << buff[i] << " ";
						i++;
					}
				}	
	}
	else {
		for (j = 0; j < y_b; j++) {
			for (i = 0; i < x_b; i++)
				buff[i] = data[_i(i, j)];
			int tempBlockSize = x_b;
			MPI_Bsend(buff, tempBlockSize, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
		}
	}
	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Finalize();

	free(buff);
	free(data);
	free(next);
	return 0;
}
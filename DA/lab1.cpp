#include <iostream>
#include <cstring>
#include <cmath>
//#include <conio.h>

const int NUMBER = 36;



struct TData{
	char *key;
	char *key1;
	char *key2;
	char *key3;
	char *value;
};

class TVector {
public:
	TVector(int size, int capacity);
	void PushBack(TData elem);
	void Print();
	int Size() const;
	TData &operator[](int index) const;
	~TVector();

private:
	TData *arr;
	int size;
	int capacity;
};


TVector::TVector(int size, int capacity) {
	this->size = size;
	this->capacity = capacity;
	arr = new TData[capacity];
}

void TVector::PushBack(TData elem) {
	if (capacity == size)
	{
		capacity *= 2;
		arr = (TData *)realloc(arr, sizeof(TData) * capacity);
	}
	arr[size++] = elem;

}

int TVector::Size() const {
	return size;
}

void TVector::Print() {
	for (int i = 0; i < size; ++i)
	{
		std::cout << arr[i].key1 << ' ' << arr[i].key2 << ' ' << arr[i].key3 << '\t' << arr[i].value << '\n';
	}
}

TData &TVector::operator[](int index) const {
	return arr[index];
}

TVector::~TVector() {
	for (int i = 0; i < size; ++i) {
		delete this->arr[i].key;
		delete this->arr[i].value;
	}
	delete[] arr;
}




void CountSort(int i, TVector &v) {
	TVector res(0, v.Size());
	int C[NUMBER];

	for (int j = 0; j < NUMBER; ++j){
		C[j] = 0;
	}

	for (int j = v.Size() - 1; j >= 0; --j) {
		if (v[j].key[i] - '0' > 9) {
			C[v[j].key[i] - '0' - 7] += 1;
		}
		else C[v[j].key[i] - '0'] += 1;
	}

	for (int j = 1; j < NUMBER; ++j) {
		C[j] += C[j - 1];
	}

	for (int j = v.Size() - 1; j >= 0; --j) {
		if (v[j].key[i] - '0' > 9) {
			C[v[j].key[i] - '0' - 7] -= 1;
			res[C[v[j].key[i] - '0' - 7]] = v[j];
		}
		else {
			C[v[j].key[i] - '0'] -= 1;
			res[C[v[j].key[i] - '0']] = v[j];
		}
	}

	for (int j = 0; j < v.Size(); ++j) {
		v[j] = res[j];
	}

}

void RadixSort(TVector &v) {
	for (int i = 5; i >= 0; --i) {
		CountSort(i, v);
	}
}

int main()
{
	TVector v(0, 10);
	TData elem;
	elem.key = new char[7];
	elem.key1 = new char[1];
	elem.key2 = new char[3];
	elem.key3 = new char[3];
	while (scanf("%s", elem.key1) == 1)
	{
		scanf("%s", elem.key2);
		scanf("%s", elem.key3);
		strcpy(elem.key, elem.key1);
		strcat(elem.key, elem.key2);
		strcat(elem.key, elem.key3);
		//elem.value = new char[64 + 1];


		char *tmp = new char[2048 + 1];
		scanf("%s", tmp);
		elem.value = new char[strlen(tmp) + 1];
		strcpy(elem.value, tmp);
		delete[] tmp;
		/*
		elem.value = new char[2048];
		scanf("%s", elem.value);
		*/
		v.PushBack(elem);
		elem.key = new char[7];
		elem.key1 = new char[1];
		elem.key2 = new char[3];
		elem.key3 = new char[3];
	}
	delete elem.key;
	delete elem.key1;
	delete elem.key2;
	delete elem.key3;
	RadixSort(v);
	v.Print();

	//_getch();
	return 0;
}
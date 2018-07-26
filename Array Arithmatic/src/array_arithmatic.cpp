#include <iostream>
#include <xmmintrin.h>
#include <omp.h>
#include <time.h>


void add_array_normal(float* a, float* b, float* c, int dataCount);
void add_array_sse(float* a, float* b, float* c, int dataCount);
int main()
{
	std::srand(time(0));

	int count = 0;
	std::cin >> count;
	
	//Initialize three dynamic float arrays
	float* array0 = new float[count];
	float* array1 = new float[count];
	float* array2 = new float[count];

	//16 byte align all the float arrays
	array0 = (float*)_mm_malloc(count * sizeof(float), 16);		
	array1 = (float*)_mm_malloc(count * sizeof(float), 16);
	array2 = (float*)_mm_malloc(count * sizeof(float), 16);

	//Fill two arrays with random numbers in range 36000,36050
	for (int i = 0; i < count; i++)
	{
		array0[i] = array1[i] = rand()%50 + 36000;
	}

	//Variables tp track time elapsed to execute for both functions
	double t0, t1, t;

	//Benchmarking normal implementation
	std::cout << "Benchmarking normal implementation: ";
	t0 = omp_get_wtime();
	add_array_normal(array0, array1, array2, count);
	t1 = omp_get_wtime();
	t = t1 - t0;
	//converting t in seconds to microseconds
	std::cout << t*1000000 << " micro seconds \t"<<std::endl;
	
	//Benchmarking sse implementation
	std::cout << "Benchmarking sse implementation: ";
	t0 = omp_get_wtime();
	add_array_sse(array0, array1, array2, count);
	t1 = omp_get_wtime();
	t = t1 - t0;
	//converting t in seconds to microseconds
	std::cout << t*1000000 << " micro seconds \t"<<std::endl;
	
	//freeing all the arrays
	_mm_free(array0);
	_mm_free(array1);
	_mm_free(array2);

	std::cin.get();
	std::cin.get();
	return 0;
}

// assuming 16 byte aligned arrays
void add_array_sse(float* a, float* b, float* c,int dataCount)
{
	__m128* x0 = (__m128*)a;
	__m128* x1 = (__m128*)b;
	__m128* xr = (__m128*)c;

	for (int i = 0; i < dataCount / 4; i++)
	{
		*xr = _mm_add_ps(*x0, *x1);
		_mm_store_ps(c, *xr);
		x0++;
		x1++;
		xr++;
	}
}

void add_array_normal(float* a, float* b, float* c, int dataCount)
{
	for (int i = 0; i < dataCount; i++)
	{
		c[i] = a[i] * b[i];
	}
}

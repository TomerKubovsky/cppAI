#include <iostream>
#include <cuda_runtime.h>
#include <device_launch_parameters.h>
#include <chrono>
#include <thread>

void fillArr(int* arr, int valTF, int size)
{
	for (int i = 0; i < size; i++)
	{
		arr[i] = valTF;
	}
}


__global__ void addFunc(const int* a1, const int* a2, int* a3, const unsigned long long arrSize)
{

	const unsigned int idx = blockIdx.x * blockDim.x + threadIdx.x;
	// const unsigned int stride = blockDim.x * gridDim.x;

	a3[idx] = a1[threadIdx.x] + a2[threadIdx.x];

	// for (int i = idx; i < arrSize; i += stride)
	// {
		// a3[i] = a1[i] + a2[i];
	// }
}

void cpuAdd(const int* a1, const int* a2, int* a3, const unsigned long long arrSize)
{
	for (int i = 0; i < arrSize; i++)
	{
		a3[i] = a1[i] + a2[i];
	}
}

int main()
{
	const unsigned long long N = 2<<20;
	int *DA1, *DA2, *DA3, *A1(new int[N]), *A2(new int[N]), *A3(new int[N]), *HOA3(new int[N]);


	fillArr(A1, 3, N);
	fillArr(A2, 3, N);

	cudaMalloc((void**)&DA1, N * sizeof(int));
	cudaMalloc((void**)&DA2, N * sizeof(int));
	cudaMalloc((void**)&DA3, N * sizeof(int));

	cudaMemcpy(DA1, A1, N * sizeof(int), cudaMemcpyHostToDevice);
	cudaMemcpy(DA2, A2, N * sizeof(int), cudaMemcpyHostToDevice);

	constexpr unsigned int blockSize = 256;

	constexpr unsigned int blockCount = (N + (blockSize - 1)) / blockSize;

	auto start = std::chrono::high_resolution_clock::now();

	cudaEvent_t DStart, stop;
	float milliseconds = 0.0f;
	cudaEventCreate(&DStart);
	cudaEventCreate(&stop);

	cudaEventRecord(DStart);

	addFunc <<< blockCount, blockSize >>> (DA1, DA2, DA3, N);
	cudaEventRecord(stop);

	cudaEventSynchronize(stop);
	cudaEventElapsedTime(&milliseconds, DStart, stop);
	std::cout << "GPU Task Time: " << milliseconds << std::endl;
	cudaDeviceSynchronize();

	cudaEventDestroy(DStart);
	cudaEventDestroy(stop);


	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> elapsed = end - start;

	// std::cout << "GPU Task Time: " << elapsed.count() << " ms\n";

	cudaMemcpy(HOA3, DA3, N * sizeof(int), cudaMemcpyDeviceToHost);

	start = std::chrono::high_resolution_clock::now();
	cpuAdd(A1, A2, A3, N);
	end = std::chrono::high_resolution_clock::now();
	elapsed = end - start;

	std::cout << "CPU Task Time: " << elapsed.count() << " ms\n";

	printf("o1: %i, o10: %i, o100: %i, o10000: %i", HOA3[0], HOA3[10], HOA3[100], HOA3[10000]);

	delete[] A1;
	delete[] A2;
	delete[] A3;
	delete[] HOA3;

	cudaFree(DA1);
	cudaFree(DA2);
	cudaFree(DA3);
}



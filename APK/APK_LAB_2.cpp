#include <stdio.h> 
#include <conio.h> 
#include <Windows.h> 
#include <math.h> 
#include <time.h> 

int main() {
	int times = 1000000;
	int matrix[8][8];
	int sum;
	double delay;
	LARGE_INTEGER timerFrequency, timerStart, timerStop;

	srand(time(NULL));
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
			matrix[i][j] = rand() % 99 + 1;

	printf("Ininitial matrix:\n\n");
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++)
			printf("%d\t ", matrix[i][j]);
		printf("\n");
	}


	QueryPerformanceFrequency(&timerFrequency);
	QueryPerformanceCounter(&timerStart);
	for (int k = 0; k < times; k++) {
		sum = 0;
		for (int i = 0; i < 8; i++)
			for (int j = 0; j < 8; j++) {
				sum += matrix[i][j];
			}
	}
	QueryPerformanceCounter(&timerStop);
	delay = (double)(timerStop.QuadPart - timerStart.QuadPart) / (double)timerFrequency.QuadPart;
	printf("\n\nC sum result: %d ", sum);
	printf("\nC time: %f\n", delay);


	QueryPerformanceFrequency(&timerFrequency);
	QueryPerformanceCounter(&timerStart);
	for (int k = 0; k < times; k++) {
		sum = 0;
		int col = 64;
		_asm {
			pusha
			xor esi, esi
			loop_1 :
				mov eax, [matrix + esi]
				add sum, eax
				add esi, 4
				sub col, 1
				jnz loop_1
				popa
		}
	}
	QueryPerformanceCounter(&timerStop);
	delay = (double)(timerStop.QuadPart - timerStart.QuadPart) / (double)timerFrequency.QuadPart;
	printf("\nAssembler without MMX sum result: %d ", sum);
	printf("\nAssembler without MMX time: %f\n", delay);


	QueryPerformanceFrequency(&timerFrequency);
	QueryPerformanceCounter(&timerStart);
	for (int k = 0; k < times; k++) {
		sum = 0;
		int col = 64;
		_asm {
			pusha
			xor esi, esi
			pxor MM7, MM7
			loop_1mmx :
			movq MM0, [matrix + esi]
				paddd MM7, MM0
				add esi, 8
				sub col, 2
				jnz loop_1mmx

				movq MM0, MM7
				psrlq MM7, 32
				paddd MM7, MM0
				movd sum, MM7
				emms
				popa
		}
	}
	QueryPerformanceCounter(&timerStop);
	delay = (double)(timerStop.QuadPart - timerStart.QuadPart) / (double)timerFrequency.QuadPart;
	printf("\nAssembler with MMX sum result: %d ", sum);
	printf("\nAssembler with MMX time: %f \n\n", delay);

	system("pause");
	return 0;
}
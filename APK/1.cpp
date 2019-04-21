#include <stdio.h>
#include <math.h>
#include <time.h>
#include <Windows.h>

double evalAsm(double startPoint, double endPoint, double step);
double evalCommon(double startPoint, double endPoint, double step);

int main() {
	double startPoint, endPoint, step, result;

	int isContinue;
	LARGE_INTEGER frequencyTime, startTime, endTime;

	printf("Function for calculation :f(x) = sin(x^2 + 5) * 4\n");

	do {
		do {
			printf("\nStart point: ");
			fflush(stdin);
			scanf_s("%lf", &startPoint);
		} while (startPoint < 0);

		do {
			printf("End point: ");
			fflush(stdin);
			scanf_s("%lf", &endPoint);
		} while (endPoint <= startPoint);

		do {
			printf("Step: ");
			fflush(stdin);
			scanf_s("%lf", &step);
		} while (step <= 0);

		QueryPerformanceFrequency(&frequencyTime);
		QueryPerformanceCounter(&startTime);
		result = evalAsm(startPoint, endPoint, step);
		QueryPerformanceCounter(&endTime);
		printf("\nAsm result: %lf", result);
		printf("\nAsm time: %lf",  (endTime.QuadPart - startTime.QuadPart) * 1000.0f / frequencyTime.QuadPart);

		QueryPerformanceFrequency(&frequencyTime);
		QueryPerformanceCounter(&startTime);
		result = evalCommon(startPoint, endPoint, step);
		QueryPerformanceCounter(&endTime);
		printf("\nC result: %lf", result);
		printf("\nC time: %lf", (endTime.QuadPart - startTime.QuadPart) * 1000.0f / frequencyTime.QuadPart);

		printf("\n\nRepeat? ");
		scanf_s("%d", &isContinue);
	} while (isContinue);
	return 0;
}

double evalCommon(double startPoint,double endPoint,double step) {
	double result = 0;
	double x = startPoint;

	while (x <= endPoint) {
		result += sin(pow(x, 2) + 5) * 4;
		x += step;
	}
	
	return result;
}

double evalAsm(double startPoint, double endPoint, double step) {
	double result = 0;
	double x = startPoint;

	double four = 4, five = 5;
	_asm {
			finit
			fld endPoint
			fld x

		loop_start :
			fcom
			fstsw ax
			and ah, 01000101b
			jz loop_end

			fld x
			fmul x
			fadd five
			fsin
			fmul four

			fadd result
			fstp result
			fadd step
			fst x
			jmp loop_start

		loop_end :
			fwait
	}

	return result;
}

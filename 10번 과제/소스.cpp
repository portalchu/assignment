#include<iostream>
using namespace std;

int sum(int *arr, int num)
{
	int sum = 0;

	for (int i = 0; i < num; i++)
	{
		sum += arr[i];
	}

	return sum;
}

double sum(double *arr, int num)
{
	double sum = 0;

	for (int i = 0; i < num; i++)
	{
		sum += arr[i];
	}

	return sum;
}

long sum(long *arr, int num)
{
	long sum = 0;

	for (int i = 0; i < num; i++)
	{
		sum += arr[i];
	}

	return sum;
}

float sum(float *arr, int num)
{
	float sum = 0;

	for (int i = 0; i < num; i++)
	{
		sum += arr[i];
	}

	return sum;
}

int main()
{
	int intArr[5] = { 10,30,50,40,15 };
	cout << sum(intArr, 5) << endl;

	double doubleArr[7] = { 1.2,3.8,4.5,9.12,9.8,12.88,3.5 };
	cout << sum(doubleArr, 7) << endl;

	long longArr[3] = { 12,45,36 };
	cout << sum(longArr, 3) << endl;

	float floatArr[4] = { 6.12,3.17,9.37 };
	cout << sum(floatArr, 4) << endl;

	return 0;
}
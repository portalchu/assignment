#include<iostream>
#include"add.h"
using namespace std;

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
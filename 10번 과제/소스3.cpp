#include<iostream>
using namespace std;

template <typename T>
T myAbs(T num)
{
	if (num >= 0)
	{
		return num;
	}
	else
	{
		num *= -1;
		return num;
	}
}

int main()
{
	cout << myAbs(-12) << endl;
	cout << myAbs(17.5) << endl;
	cout << myAbs(-1.17) << endl;
	cout << myAbs(0) << endl;

	return 0;
}
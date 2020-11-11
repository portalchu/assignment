#include<iostream>
using namespace std;

template <typename T>
class Counter {
	T value;
public:
	Counter() { value = 0; }
	Counter(T n) { value = n; }
	T getValue() { return value; }
	void operator ++() { ++value; }
	void operator --() { --value; }
};

int main()
{
	Counter <int>intCounter;

	for (int i = 0; i < 10; i++)
	{
		++intCounter;
	}

	cout << intCounter.getValue() << endl;

	Counter <double>doubleCounter(15.35);

	doubleCounter.operator--;
	cout << doubleCounter.getValue() << endl;


	return 0;
}
#include<iostream>
using namespace std;

template <typename T>
class Storage
{
	T m_data;
public:
	Storage() {}
	Storage(T data) { m_data = data; }
	T getData() { return m_data; }
	void setData(T data) { m_data = data; }
};

int main()
{
	Storage <int>intStorage;

	intStorage.setData(100);
	cout << intStorage.getData() << endl;

	Storage <double>doubleStorage(77.7);

	cout << doubleStorage.getData() << endl;

	Storage <const char*>charStorage;

	charStorage.setData("Hello World!!");
	cout << charStorage.getData() << endl;

	return 0;
}
#include <iostream>
using namespace std;

class Something
{
public:
	int m_value;
	Something() : m_value(0) {}
	void setValue(int value) { m_value = value; }

	void resetValue() { m_value = 0; }
	int getValue () const { return m_value; }
};

void printValue(const Something& sData) {
	cout << "value :" << sData.getValue() << endl;
}

int main(int argc, char const* argv[])
{
	Something first;

	first.setValue(3);
	cout << first.getValue() << endl;
	first.resetValue();
	printValue(first);

	return 0;
}

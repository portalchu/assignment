#include <iostream>

class A {
public:
	virtual void foo() = 0; // ���� �����Լ�
};

class B : public A {
	void foo() {}
};

int main(void) {

	A *a;    // (1) ���� �߻�
	B b;    // (2) ���� �߻� 
	return 0;
}

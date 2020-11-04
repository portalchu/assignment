#include <iostream>

class A {
public:
	virtual void foo() = 0; // 순수 가상함수
};

class B : public A {
	void foo() {}
};

int main(void) {

	A *a;    // (1) 에러 발생
	B b;    // (2) 에러 발생 
	return 0;
}

#include <iostream>
using namespace std;

class Shape {
	int x, y;
public:
	Shape(int x = 0, int y = 0) : x(x), y(y) {}
	virtual void draw() { cout << "Shape draw" << endl; }
};

class Rect : public Shape
{
public:
	int width, height;
	Rect (int x, int y, int w = 0, int h = 0) : Shape(x,y), width(w), height(h) {}
	void draw() { cout << "Rect draw" << endl; }
};

class Triangle : public Shape
{
public:
	int base, height;
	Triangle(int x, int y, int b = 0, int h = 0) : Shape(x, y), base(b), height(h) {}
	void draw() { cout << "Triangle draw" << endl; }
};

class Circle : public Shape
{
public:
	int radius;
	Circle(int x, int y, int r = 0) : Shape(x, y), radius(r) {}
	void draw() { cout << "Circle draw" << endl; }
};

int main(int argc, char const *argv[])
{
	Rect rect1(4, 4), rect2(3, 5, 6);

	Triangle tri1(5, 6), tri2(7, 8, 3, 4);

	Circle circle1(1, 2, 3), circle2(5, 5);

	Shape *buf1[10];

	buf1[0] = &rect1;

	buf1[0]->draw();


	return 0;
}
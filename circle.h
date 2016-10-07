#ifndef CIRCLE_H_
#define CIRCLE_H_

#include "misc.h"
#include <iostream>
#include <stdio.h>

using namespace std;

class Circle {

	Point center;
	float radius;
	Color color;

	public:
	Circle(Point center, float r, Color c);
	void draw ();
	void draw (float deltaX, float deltaY);
	Point get_center() const;
	void set_center(Point c);
	void increment_center(float dx, float dy);
	float get_radius() const;
	void set_radius(float r);
	Color get_color() const;
	void set_color(Color c);
	bool outsideOf(Circle* c) const;
	bool insideOf(Circle* c) const;
	void print() const;

	friend ostream& operator<<(ostream& output, const Circle& c);
};

#endif /* CIRCLE_H_ */

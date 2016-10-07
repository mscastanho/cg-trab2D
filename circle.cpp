#include <GL/glut.h>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <map>
#include <cmath>
#include "circle.h"
#include "misc.h"

using namespace std;

Circle::Circle(Point center, float r, Color c){
	this->center = center;
	this->radius = r;
	this->color = c;
}

void Circle::draw (){
	glPushMatrix();

	glColor3f(this->color.r,this->color.g,this->color.b);


	glTranslatef(this->center.x,this->center.y,1);
	// Needed to invert y axis because of SVG and OpenGL's different y-axis
	// orientation

	int i;
	float x,y;
	glBegin(GL_POLYGON);
	// Let`s use 36 vertices to draw the circle as a polygon
	for(i = 0 ; i < 360 ; i += 4 ){
		x = radius*cos(M_PI*i/180.0);
		y = radius*sin(M_PI*i/180.0);
		glVertex3f(x,y,0);
	}
	glEnd();

	glPopMatrix();
}

void Circle::draw (float deltaX, float deltaY){
	glPushMatrix();

	glColor3f(this->color.r,this->color.g,this->color.b);


	glTranslatef(this->center.x + deltaX,-this->center.y + deltaY,1);
	// Needed to invert y axis because of SVG and OpenGL's different y-axis
	// orientation

	int i;
	float x,y;
	glBegin(GL_POLYGON);
	// Let`s use 36 vertices to draw the circle as a polygon
	for(i = 0 ; i < 360 ; i += 4 ){
		x = radius*cos(M_PI*i/180.0);
		y = radius*sin(M_PI*i/180.0);
		glVertex3f(x,y,0);
	}
	glEnd();

	glPopMatrix();
}

Point Circle::get_center() const{
	return this->center;
}

void Circle::set_center(Point c){
	this->center = c;
}

void Circle::increment_center(float dx, float dy){
	this->center.x += dx;
	this->center.y += dy;
}

float Circle::get_radius() const{
	return this->radius;
}

void Circle::set_radius(float r){
	this->radius = r;
}

Color Circle::get_color() const{
	return this->color;
}

void Circle::set_color(Color c){
	this->color = c;
}

// This function calculates if a collision has occurred between this and another
// circle. If one circle is inside another, they have collided.
bool Circle::outsideOf(Circle* c) const{
	float x1,x2,y1,y2,r1,r2;

	x1 = this->center.x;
	y1 = this->center.y;
	r1 = this->radius;

	x2 = c->center.x;
	y2 = c->center.y;
	r2 = c->radius;



	float dist = sqrt(pow(x1-x2,2) + pow(y1-y2,2));

	if(dist >= r1 + r2)
		return true;
	else
		return false;
}

// This function calculates if a collision has occurred between this and another
// circle. In this case a collision only occurs if this circle is partially or
// completely outside of the circle c
bool Circle::insideOf(Circle* c) const{
	float x1,x2,y1,y2,r1,r2;

	x1 = this->center.x;
	y1 = this->center.y;
	r1 = this->radius;

	x2 = c->center.x;
	y2 = c->center.y;
	r2 = c->radius;

	float dist = sqrt(pow(x1-x2,2) + pow(y1-y2,2));

	if(dist <= r2 - r1)
		return true;
	else
		return false;
}



void Circle::print() const{

	float r = this->color.r;
	float g = this->color.g;
	float b = this->color.b;

	cout << "< Circle >" << endl;
	printf("center: (%f,%f)\n",this->center.x,this->center.y);
	printf("radius: %f\n",this->radius);
	printf("Color: (%f,%f,%f)\n",r,g,b);
}

ostream& operator<<(ostream& output, const Circle& c){
	output << "Circle: c=(" << c.center.x << "," << c.center.y << ") r=" << c.radius << endl;
	return output;
}

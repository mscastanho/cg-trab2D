#ifndef CAR_H_
#define CAR_H_

#include <GL/glut.h>
#include "misc.h"
#include <iostream>
#include <stdio.h>

using namespace std;

class Car {

	float size;
  Color bodyColor;
  Color styleColor;

  public:
  Car(float size, Color bodyColor, Color styleColor);
  void draw (Point position, GLfloat wAngle, GLfloat carAngle, GLfloat canonAngle);
	Point getCanonPos (Point pos, float carAngle, float canonAngle);
};

#endif /* CAR_H_ */

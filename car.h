#ifndef CAR_H_
#define CAR_H_

#include <GL/glut.h>
#include "misc.h"
#include <iostream>
#include <stdio.h>

using namespace std;

const float BODY_HEIGHT = 1;
const float BODY_WIDTH = 0.5;
const float CANON_HEIGHT = 0.2;
const float CANON_WIDTH = 0.1;
const float WHEEL_AXIS_WIDTH = 0.075;
const float WHEEL_AXIS_HEIGHT = WHEEL_AXIS_WIDTH;
const float WHEEL_WIDTH = 0.15;
const float WHEEL_HEIGHT = 0.25;
const float WINDSHIELD_WIDTH = BODY_WIDTH;
const float WINDSHIELD_HEIGHT = 0.2;
const float EXHAUST_PIPE_WIDTH = 0.075;
const float EXHAUST_PIPE_HEIGHT = EXHAUST_PIPE_WIDTH;

class Car {

	float size;
  Color bodyColor;
  Color styleColor;
	bool moving;

  public:
  Car(float size, Color bodyColor, Color styleColor);
  void draw (Point position, GLfloat wAngle, GLfloat carAngle, GLfloat canonAngle);
	Point getBulletInitPos (Point pos, float carAngle, float canonAngle);
	void setMoving(bool status);
};

#endif /* CAR_H_ */

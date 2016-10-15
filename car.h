#ifndef CAR_H_
#define CAR_H_

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
  void draw (Point position);
};

#endif /* CAR_H_ */

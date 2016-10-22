#ifndef BULLET_H_
#define BULLET_H_

#include "misc.h"

using namespace std;

const float BULLET_WIDTH = 2;
const float BULLET_HEIGHT = 8;

// Class representing a bullet
class Bullet {

  Point position;
  Color color;
  float angle;

  public:
  Bullet (Point position, Color color, float angle);
  void draw();
  void update(float speed);
  Point get_position ();
};

#endif /* BULLET_H_ */

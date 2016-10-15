#include <GL/glut.h>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <map>
#include <cmath>
#include "circle.h"
#include "rect.h"
#include "misc.h"
#include "car.h"

//Position represents the center of the car

Car::Car(float size, Color bodyColor, Color styleColor){
  this->size = size;
  this->bodyColor = bodyColor;
  this->styleColor = styleColor;
}

void Car::draw (Point position){

  glPushMatrix();

  glTranslatef(position.x,position.y,1);

  Color color;
  float L = this->size;

  // Main body
  color = this->bodyColor;
  float bHeight = L;
  float bWidth = L*0.3;
  drawRect(bWidth,bHeight,color,-bWidth/2,-bHeight/2);
  // ============

  // Wheel axes
  color = GRAY;
  float waWidth = 0.1*L;
  float waHeight = 0.05*L;
  drawRect(waWidth,waHeight,color,0.15*L,0.4*L);
  drawRect(waWidth,waHeight,color,0.15*L,-0.4*L);
  drawRect(waWidth,waHeight,color,-0.15*L,-0.4*L);
  drawRect(waWidth,waHeight,color,-0.15*L,0.4*L);
  // =============

  // Wheels
  color = BLACK;
  float wWidth = 0.15*L;
  float wHeight = 0.25*L;
  drawRect(wWidth,wHeight,color,(bWidth/2+waWidth),0.3*L);
  drawRect(wWidth,wHeight,color,(bWidth/2+waWidth),-0.3*L);
  drawRect(wWidth,wHeight,color,-(bWidth/2+waWidth),-0.3*L);
  drawRect(wWidth,wHeight,color,-(bWidth/2+waWidth),0.3*L);
  // ==============

  // Canon
  color = BLACK;
  float cWidth = 0.05*L;
  float cHeight = 0.2*L;
  drawRect(wWidth,wHeight,color,-cWidth/2,bHeight/2);
  // =============

  //cout << bWidth << endl;
  glPopMatrix();

}

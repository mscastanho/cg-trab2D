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

void Car::draw (Point position, GLfloat wAngle, GLfloat cAngle){

  glPushMatrix();

  glTranslatef(position.x,position.y,0);
  glRotatef(cAngle,0,0,1.0);
  Color color;
  float L = this->size;

  // Main body
  color = this->bodyColor;
  float bHeight = L;
  float bWidth = L*0.5;
  drawRect(bWidth,bHeight,color,-bWidth/2,-bHeight/2);
  // ============

  // Wheel axes
  color = GRAY;
  float waWidth = 0.075*L;
  float waHeight = 0.05*L;
  drawRect(waWidth,waHeight,color,bWidth/2,0.7*bHeight/2);
  drawRect(waWidth,waHeight,color,-(bWidth/2+waWidth),0.7*bHeight/2);
  drawRect(waWidth,waHeight,color,-(bWidth/2+waWidth),-(0.7*bHeight/2 + waHeight));
  drawRect(waWidth,waHeight,color,bWidth/2,-(0.7*bHeight/2 + waHeight));
  // =============

  // Wheels
  color = YELLOW;
  float wWidth = 0.15*L;
  float wHeight = 0.25*L;
  float axisMidY = 0.7*bHeight/2 + waHeight/2;

  //Front Wheels
  glPushMatrix();
  glTranslatef(bWidth/2+waWidth+wWidth/2,axisMidY,0);
  glRotatef(wAngle,0,0,1.0);
  drawRect(wWidth,wHeight,color,-wWidth/2,-wHeight/2);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(-(bWidth/2+waWidth+wWidth/2),axisMidY,0);
  glRotatef(wAngle,0,0,1.0);
  drawRect(wWidth,wHeight,color,-wWidth/2,-wHeight/2);
  glPopMatrix();

  //Back wheels
  drawRect(wWidth,wHeight,color,-(bWidth/2+waWidth+wWidth),-(axisMidY + wHeight/2));
  drawRect(wWidth,wHeight,color,(bWidth/2+waWidth),-(axisMidY + wHeight/2));
  // ==============

  // Canon
  color = YELLOW;
  float cWidth = 0.05*L;
  float cHeight = 0.2*L;
  drawRect(cWidth,cHeight,color,-cWidth/2,bHeight/2);
  // =============

  // Windshield
  color = BLACK;
  float wsWidth = bWidth;
  float wsHeight = 0.2*bHeight;
  drawRect(wsWidth,wsHeight,color,-bWidth/2,bHeight/8);
  // =============

  //cout << bWidth << endl;
  glPopMatrix();

}

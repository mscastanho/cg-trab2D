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

Car::Car(float size, Color bodyColor, Color styleColor){
  this->size = size;
  this->bodyColor = bodyColor;
  this->styleColor = styleColor;
}

void Car::draw (Point position, GLfloat wAngle, GLfloat carAngle, GLfloat canonAngle){

  glPushMatrix();

  glTranslatef(position.x,position.y,0);
  glRotatef(carAngle,0,0,1.0);
  Color color;
  float L = this->size;

  // Main body properties
  float bHeight = L;
  float bWidth = L*0.5;

  // Canon (need to draw canon first to be behind the body)
  glPushMatrix();
  glTranslatef(0,bHeight/2-bHeight/10,0);
  glRotatef(canonAngle,0,0,1.0);
  color = YELLOW;
  float cWidth = 0.1*L;
  float cHeight = 0.3*L;
  drawRect(cWidth,cHeight,color,-cWidth/2,0);
  glPopMatrix();
  // =============

  //Draw main body
  color = this->bodyColor;
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

  // Windshield
  color = BLACK;
  float wsWidth = bWidth;
  float wsHeight = 0.2*bHeight;
  drawRect(wsWidth,wsHeight,color,-bWidth/2,bHeight/8);
  // =============

  //cout << bWidth << endl;
  glPopMatrix();

}

// This function returns the coordinates of the canon`s far end
Point Car::getCanonPos (Point pos, float carAngle, float canonAngle){
  //x and y corresponding to the car`s coordinate system
  float x = 0;
  float y = 0.5*this->size;

  x += -0.2*this->size*sin(M_PI*canonAngle/180.0);
  y += 0.2*this->size*cos(M_PI*canonAngle/180.0);

  float mag = sqrt(x*x + y*y);
  float worldX = pos.x - mag*sin(M_PI*carAngle/180.0);
  float worldY = pos.y + mag*cos(M_PI*carAngle/180.0);

  Point p = {worldX,worldY};

  return p;
}

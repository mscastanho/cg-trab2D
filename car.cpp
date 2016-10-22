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
#include "bullet.h"

Car::Car(float size, Color bodyColor, Color styleColor){
  this->size = size;
  this->bodyColor = bodyColor;
  this->styleColor = styleColor;
  this->moving = false;
}

void Car::draw (Point position, GLfloat wAngle, GLfloat carAngle, GLfloat canonAngle){

  glPushMatrix();

  glTranslatef(position.x,position.y,0);
  glRotatef(carAngle,0,0,1.0);
  Color color;
  float L = this->size;

  // Main body properties
  float bHeight = BODY_HEIGHT*L;
  float bWidth = BODY_WIDTH*L;
  color = this->bodyColor;
  drawRect(bWidth,bHeight,color,-bWidth/2,-bHeight/2);
  // ============

  // Wheel axes
  color = WHITE;
  float waWidth = WHEEL_AXIS_WIDTH*L;
  float waHeight = WHEEL_AXIS_HEIGHT*L;
  drawRect(waWidth,waHeight,color,bWidth/2,0.7*bHeight/2);
  drawRect(waWidth,waHeight,color,-(bWidth/2+waWidth),0.7*bHeight/2);
  drawRect(waWidth,waHeight,color,-(bWidth/2+waWidth),-(0.7*bHeight/2 + waHeight));
  drawRect(waWidth,waHeight,color,bWidth/2,-(0.7*bHeight/2 + waHeight));
  // =============

  // Wheels
  color = GRAY;
  float wWidth = WHEEL_WIDTH*L;
  float wHeight = WHEEL_HEIGHT*L;
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

  // Canon (need to draw canon first to be behind the body)
  glPushMatrix();
  glTranslatef(0,bHeight/2,0);
  glRotatef(canonAngle,0,0,1.0);
  color = YELLOW;
  float cWidth = CANON_WIDTH*L;
  float cHeight = CANON_HEIGHT*L;
  drawRect(cWidth,cHeight,color,-cWidth/2,0);
  glPopMatrix();
  // =============

  // Windshield
  color = BLACK;
  float wsWidth = WINDSHIELD_WIDTH*L;
  float wsHeight = WINDSHIELD_HEIGHT*L;
  drawRect(wsWidth,wsHeight,color,-bWidth/2,bHeight/8);
  // =============

  // Exhaust Pipes
  color = YELLOW;
  float epWidth = EXHAUST_PIPE_WIDTH*L;
  float epHeight = EXHAUST_PIPE_HEIGHT*L;
  drawRect(epWidth,epHeight,color,-0.45*bWidth,-bHeight/2-epHeight);
  drawRect(epWidth,epHeight,color,0.45*bWidth-epWidth,-bHeight/2-epHeight);
  // =============

  // Draw smoke if car is moving
  static int n = 1; //represents the number of circles used to draw the smoke
  Color smokeColors[] = {LIGHT_GRAY,GRAY,DARK_GRAY};
  float radius = epWidth;
  float dy = 0;

  if(this->moving){
    int i;
    for(i = 0 ; i < n ; i++ ){
      Point centerLeft = {-0.45*bWidth+epWidth/2,-bHeight/2-epHeight-radius - dy};
      drawCircle(radius,centerLeft,smokeColors[i]);
      Point centerRight = {0.45*bWidth-epWidth/2,-bHeight/2-epHeight-radius - dy};
      drawCircle(radius,centerRight,smokeColors[i]);

      dy += radius*1.5;
      radius *= 1.2;
    }

    static GLdouble lastDrawTime = 0;
    GLdouble now = glutGet(GLUT_ELAPSED_TIME);;

    if(now - lastDrawTime > 150){
      n = (++n)%4; // Make n vary only between 0 and 3
      lastDrawTime = now;
    }

  }
  glPopMatrix();
}

// This function returns the coordinates of a bullet leaving the canon
Point Car::getBulletInitPos (Point carPos, float carAngle, float canonAngle){

  Point pCanon = {0,0};

  // Translate from the canon's end position
  Point canonEndPos = {0,CANON_HEIGHT*this->size};
  pCanon = translateFrom(pCanon,canonEndPos);

  // Rotate by the canon's rotation
  pCanon = rotateBy(pCanon,canonAngle);

  // Translate from canon position
  Point canonPos = {0,BODY_HEIGHT*this->size/2};
  pCanon = translateFrom(pCanon,canonPos);

  // Rotate by the car's rotation
  pCanon = rotateBy(pCanon,carAngle);

  // First translate from car position
  pCanon = translateFrom(pCanon,carPos);

  return pCanon;
}

void Car::setMoving(bool status){
  this->moving = status;
}

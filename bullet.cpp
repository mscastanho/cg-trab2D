#include <GL/glut.h>
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include "bullet.h"
#include "misc.h"
#include "rect.h"

Bullet::Bullet (Point position, Color color, float angle){
  this->position = position;
  this->color = color;
  this->angle = angle;
}

void Bullet::draw(){
  glPushMatrix();

  glTranslatef(this->position.x,this->position.y,0);
  glRotatef(angle,0,0,1.0);

  drawRect(BULLET_WIDTH,BULLET_HEIGHT,this->color);
  glPopMatrix();
}

void Bullet::update(float speed){
  this->position.x += -speed*sin(M_PI*this->angle/180.0);
  this->position.y += speed*cos(M_PI*this->angle/180.0);
}

Point Bullet::get_position (){
  return this->position;
}

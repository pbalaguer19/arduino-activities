#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

//-----------------------------------------------
#define MOVE 1
#define QUIET 2
enum ParticleDirection{UPDIR, DOWNDIR, RIGHTDIR, LEFTDIR};
//-----------------------------------------------

//-----------------------------------------------

class particle {
  float x,y;   //-- Current position
  float vx,vy; //-- Velocity vector
  int state;

  GLenum light;
  ParticleDirection particleDirection;

  long time_remaining;

public:

  particle();
  void set_position(int x,int y);
  void init_movement(int destination_x,int destination_y,int duration, ParticleDirection direction, GLenum l);
  void integrate(long t);
  void draw(int radius, float r, float g, float b, float bx, float bz);
  bool isReady();
};


//-----------------------------------------------

particle::particle()
{
  state=QUIET;
}

//-----------------------------------------------

void particle::set_position(int x,int y)
{
  this->x = x;
  this->y = y;
}

//-----------------------------------------------

void particle::init_movement(int destination_x,int destination_y,int duration, ParticleDirection direction, GLenum l)
{
  vx = (destination_x - x)/duration;
  vy = (destination_y - y)/duration;

  state=MOVE;
  time_remaining=duration;

  particleDirection = direction;
  light = l;
}

//-----------------------------------------------

void particle::integrate(long t)
{
  if(state==MOVE && t<time_remaining)
    {
      x = x + vx*t;
      y = y + vy*t;
      time_remaining-=t;
    }
  else if(state==MOVE && t>=time_remaining)
    {
      x = x + vx*time_remaining;
      y = y + vy*time_remaining;
      state=QUIET;
    }
}

//-----------------------------------------------

void particle::draw(int radius, float r, float g, float b, float baseX, float baseZ){
  GLint position[4];
  GLfloat color[4];
  GLfloat material[4];
  GLfloat *dir = new GLfloat[3];
  float exponent = 3.0;

  if(particleDirection == UPDIR){
    dir[0] = 0; dir[1] = 1; dir[2] = -1;
  }
  if(particleDirection == DOWNDIR){
    dir[0] = 0; dir[1] = 1; dir[2] = 1;
  }
  if(particleDirection == LEFTDIR){
    dir[0] = -1; dir[1] = 1; dir[2] = 0;
  }
  if(particleDirection == RIGHTDIR){
    dir[0] = 1; dir[1] = 1; dir[2] = 0;
  }

  //-- Spot light
  position[0]=baseX + x; position[1]=radius; position[2]=baseZ - y; position[3]=1;
  glLightiv(light,GL_POSITION,position);

  glLightfv (light,GL_SPOT_DIRECTION, dir);

  color[0]=0.8; color[1]=0.8; color[2]=0.8; color[3]=1;
  if(light != GL_LIGHT1){
    color[0]=0.4; color[1]=0.4; color[2]=0.4; color[3]=1;
    exponent = 5.0;
  }
  glLightfv(light,GL_DIFFUSE,color);

  glLightf(light,GL_CONSTANT_ATTENUATION,0.35);
  glLightf(light,GL_LINEAR_ATTENUATION,0.0);
  glLightf(light,GL_QUADRATIC_ATTENUATION,0.00000002);

  glLightf(light,GL_SPOT_CUTOFF,50.0);
  glLightf(light,GL_SPOT_EXPONENT,exponent);

  glEnable(light);

  glPushMatrix();
  material[0]=r; material[1]=g; material[2]=b; material[3]=1.0;
  glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,material);
  GLUquadric *quad;
  quad = gluNewQuadric();
  glTranslatef(baseX + x, radius, baseZ - y);
  gluSphere(quad,radius,100,20);
  glEnd();
  glPopMatrix();
}

bool particle::isReady(){
  return state == QUIET;
}

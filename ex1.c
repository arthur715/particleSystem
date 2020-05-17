////////////////////////////////////////////////////////////////
// Department of Computer Science
// The University of Manchester
//
// This code is licensed under the terms of the Creative Commons
// Attribution 2.0 Generic (CC BY 3.0) License.
//
// Skeleton code for COMP37111 coursework, 2019-20
//
// Authors: Toby Howard and Arturs Bekasovs
//
/////////////////////////////////////////////////////////////////
#ifdef MACOSX
  #include <OpenGL/gl.h>
  #include <OpenGL/glu.h>
  #include <GLUT/glut.h>
#else
  #include <GL/glut.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <string.h>

#define RUN_SPEED 0.9
#define TURN_ANGLE 2.0
#define DEG_TO_RAD 0.017453293
#define AXIS_SIZE 10000
#define SIZE_OF_GROUND 10000

////////////////////// functions ///////////////////////////////
double myRandom();
double randomBetween(float min, float max);
void setInitialViewParameters();
void setInitialWorldParameters();
void setInitialEmitterParameters();
void setInitialParticleParameters();
void drawPoint();
void animate();
void display();
void keyboard(unsigned char key, int x, int y);
void cursor_keys(int key, int x, int y);
void makeAxes();
void calculate_lookpoint();
double calculateAbsoluteValue(float x, float y, float z);
void calculateFPS();
void reshape(int w, int h);
void drawString(void *font, float x, float y, char *str);
void drawGround();
void drawInfoText();
void viewMenu(int value);
void renderMenu(int value);
void colourMenu(int value);
void noOfParMenu(int value);
void effMenu(int value);
void mainMenu(int value);
void initGraphics(int argc, char* argv[]);
int main(int argc, char** argv);

////////////// particle structure /////////////
typedef struct
{
  GLfloat r, g, b;            // colour
  GLfloat x0, y0, z0;         // initial position
  GLfloat ux, uy, uz;         // initial velocity
  GLfloat initialSpeed;       // initial speed
  GLfloat px, py, pz;         // previous position
  GLfloat sx, sy, sz;         // current position
  GLfloat vx, vy, vz;         // current velocity
  GLfloat currentSpeed;       // current speed
  GLfloat xzPlaneDirection;   // angle from positive x-axis clockwise
  GLfloat xyPlaneDirection;   // angle form positive y-axis to xz-plane
  GLfloat randomInitialTime;  // randomInitialTime
  GLfloat xztimeOfParticle;   // time moving in x-z plane
  GLfloat ytimeOfParticle;    // time moving in x-y plane
  GLfloat mass;               // particle mass
  GLfloat initialEnergy;      // particle initial energy
  GLfloat currentEnergy;      // currentEnergy
} Particle;

/////////////// emitter structure /////////////
typedef struct
{
  GLfloat ex, ey, ez;       // emitter position
  GLfloat angleToXaxisMax;  // range of angle of emit particle in xz-plane direction
  GLfloat angleToXaxisMin;
  GLfloat angleToYaxisMax;  // range of angle of emit particle in y direction
  GLfloat angleToYaxisMin;
  GLfloat spawnSpeedMin;    // range of initial speed to give some randomness
  GLfloat spawnSpeedMax;    // these two spawn speed also determine the initial energy
  GLint numberOfParticles;  // number of particles emit from the emitter
  Particle* particles;      // array to store the particles structure
} Emitter;

/////////////// camera variables //////////////
GLdouble lat,     lon;              // View angles (degrees)
GLfloat  eyex,    eyey,    eyez;    // Eye point
GLfloat  centerx, centery, centerz; // Look point
GLfloat  upx,     upy,     upz;     // View up vector
GLint    axisEnabled;
GLint    groundEnabled;
GLint    pause;

GLint    width = 1600, height = 1000;

Particle singleParticle;
Emitter singleEmitter;

/////////////// Wrold Variables //////////////
GLfloat noOfParticlesInSystem;
GLuint axisList;
GLfloat gravity;
GLfloat red, green, blue;
GLfloat rDecrement, gDecrement, bDecrement;

GLfloat energyLossMin;
GLfloat energyLossMax;

GLfloat respawnAngleToYaxisMin;
GLfloat respawnAngleToYaxisMax;
GLfloat respawnAngleToXaxisMin;
GLfloat respawnAngleToXaxisMax;

GLfloat initialAngleToYaxisMin;
GLfloat initialAngleToYaxisMax;
GLfloat initialAngleToXaxisMin;
GLfloat initialAngleToXaxisMax;

GLfloat renderMode;
GLfloat efficiencyMode;
GLfloat cubeSize;

/////////////// FPS variables ////////////////
GLfloat frameCount = 0.0;
GLfloat currentTime = 0.0;
GLfloat timeInterval = 0.0;
GLfloat lastFPSUpdateTime = 0.0;
GLfloat timeSinceLastFrameMS = 0.0;
GLfloat lastFrameTime = 0.0;
GLfloat deltaTime = 0.0;
GLfloat fps = 0.0;

//////////////////////////////////////////////
double myRandom()
{
  return (rand()/(double)RAND_MAX); //Return random double within range [0,1]
}

//////////////////////////////////////////////
double randomBetween(float min, float max)
{
  double range = max - min;
  return (rand() / (double) RAND_MAX) * range + min;
}

//////////////////////////////////////////////
void setInitialViewParameters()
{
  // Set initial view parameters
  eyex=  -0.2;    // Set eyepoint at eye height within the scene
  eyey=  6.0;
  eyez=  -153.7;

  upx= 0.0;       // Set up direction to the +Y axis
  upy= 1.0;
  upz= 0.0;

  lat= 13.0;      // angle from positive y-axis to xz-plane
  lon= 0.0;       // angle from positive x-axis clockwise

  width = 1600;   // window width
  height = 1000;  // window height

  axisEnabled = 0;
  groundEnabled = 0;
  pause = 0;

  int randNum = myRandom();
}

///////////////////////////////////////////////
void setInitialWorldParameters()
{
  noOfParticlesInSystem = 40000;
  gravity = -110.81;

  red = 0.0;
  green = 0.69;
  blue = 1.0;

  rDecrement = -0.0013;
  gDecrement = 0.0039;
  bDecrement = -0.0676;

  respawnAngleToYaxisMin = 30;
  respawnAngleToYaxisMax = 35;
  respawnAngleToXaxisMin = 0;
  respawnAngleToXaxisMax = 360;

  initialAngleToYaxisMin = 0;
  initialAngleToYaxisMax = 50;
  initialAngleToXaxisMin = 0;
  initialAngleToXaxisMax = 360;

  energyLossMax = 0.0;
  energyLossMin = 0.2;

  renderMode = 1;
  efficiencyMode = 1;
  cubeSize = 0.1;
}

///////////////////////////////////////////////
void setInitialEmitterParameters()
{
  singleEmitter.ex = 0.0;
  singleEmitter.ey = 0.0;
  singleEmitter.ez = 0.0;
  singleEmitter.angleToYaxisMax = initialAngleToYaxisMax;
  singleEmitter.angleToYaxisMin = initialAngleToYaxisMin;
  singleEmitter.angleToXaxisMax = initialAngleToXaxisMax;
  singleEmitter.angleToXaxisMin = initialAngleToXaxisMin;
  singleEmitter.spawnSpeedMin = 100;
  singleEmitter.spawnSpeedMax = 105;
  singleEmitter.numberOfParticles = noOfParticlesInSystem;
  singleEmitter.particles = (Particle*) malloc(sizeof(Particle) * singleEmitter.numberOfParticles);
}

///////////////////////////////////////////////
void setInitialParticleParameters()
{

  int i;
  for(i = 0; i < singleEmitter.numberOfParticles; i++){

    singleEmitter.particles[i].r = red;
    singleEmitter.particles[i].g = green;
    singleEmitter.particles[i].b = blue;

    //set initial position and current position
    singleEmitter.particles[i].x0 = singleEmitter.ex;
    singleEmitter.particles[i].y0 = singleEmitter.ey;
    singleEmitter.particles[i].z0 = singleEmitter.ez;
    singleEmitter.particles[i].sx = singleEmitter.ex;
    singleEmitter.particles[i].sy = singleEmitter.ey;
    singleEmitter.particles[i].sz = singleEmitter.ez;
    singleEmitter.particles[i].px = singleEmitter.ex;
    singleEmitter.particles[i].py = singleEmitter.ez;
    singleEmitter.particles[i].pz = singleEmitter.ez;

    singleEmitter.particles[i].xztimeOfParticle = singleEmitter.particles[i].randomInitialTime;
    singleEmitter.particles[i].ytimeOfParticle = singleEmitter.particles[i].randomInitialTime;

    singleEmitter.particles[i].randomInitialTime = randomBetween(0.0,0.05);
    singleEmitter.particles[i].mass = 3;

    singleEmitter.particles[i].xzPlaneDirection = randomBetween(singleEmitter.angleToXaxisMin, singleEmitter.angleToXaxisMax);
    singleEmitter.particles[i].xyPlaneDirection = randomBetween(singleEmitter.angleToYaxisMin, singleEmitter.angleToYaxisMax);
    singleEmitter.particles[i].initialSpeed = randomBetween(singleEmitter.spawnSpeedMin, singleEmitter.spawnSpeedMax);
    singleEmitter.particles[i].ux = singleEmitter.particles[i].initialSpeed * sin(singleEmitter.particles[i].xyPlaneDirection * DEG_TO_RAD) * cos(singleEmitter.particles[i].xzPlaneDirection * DEG_TO_RAD);
    singleEmitter.particles[i].uy = singleEmitter.particles[i].initialSpeed * cos(singleEmitter.particles[i].xyPlaneDirection * DEG_TO_RAD);
    singleEmitter.particles[i].uz = singleEmitter.particles[i].initialSpeed * sin(singleEmitter.particles[i].xyPlaneDirection * DEG_TO_RAD) * sin(singleEmitter.particles[i].xzPlaneDirection * DEG_TO_RAD);
    singleEmitter.particles[i].vx = singleEmitter.particles[i].ux;
    singleEmitter.particles[i].vy = singleEmitter.particles[i].uy;
    singleEmitter.particles[i].vz = singleEmitter.particles[i].uz;
    singleEmitter.particles[i].currentSpeed = calculateAbsoluteValue(singleEmitter.particles[i].vx, singleEmitter.particles[i].vy, singleEmitter.particles[i].vz);
    singleEmitter.particles[i].initialEnergy = singleEmitter.particles[i].mass * 0.5 * singleEmitter.particles[i].currentSpeed * singleEmitter.particles[i].currentSpeed;
    singleEmitter.particles[i].currentEnergy = singleEmitter.particles[i].initialEnergy;
  }

}

//////////////////////////////////////////////
void drawPoint()
{
  int i;
  for(i = 0; i < singleEmitter.numberOfParticles; i++)
  {
    if(!pause){
        singleEmitter.particles[i].r -= rDecrement;
        singleEmitter.particles[i].g -= gDecrement;
        singleEmitter.particles[i].b -= bDecrement;
        if( singleEmitter.particles[i].currentEnergy > randomBetween(5,10)){
            if(renderMode == 1){
                glBegin(GL_POINTS);
                  glColor3f(singleEmitter.particles[i].r,singleEmitter.particles[i].g,singleEmitter.particles[i].b);
                  glVertex3f(singleEmitter.particles[i].sx, singleEmitter.particles[i].sy , singleEmitter.particles[i].sz);
                glEnd();
                glPointSize(1.0);
            }
            else if(renderMode == 2){
              glLineWidth(2.0);
              glBegin(GL_LINES);
                glColor3f(singleEmitter.particles[i].r, singleEmitter.particles[i].g, singleEmitter.particles[i].b);
                glVertex3f(singleEmitter.particles[i].px, singleEmitter.particles[i].py, singleEmitter.particles[i].pz);
                glVertex3f(singleEmitter.particles[i].sx, singleEmitter.particles[i].sy, singleEmitter.particles[i].sz);
              glEnd();
            }
            else if(renderMode == 3){
              glBegin(GL_QUADS);      // draw a cube with 6 quads

                glColor3f(singleEmitter.particles[i].r, singleEmitter.particles[i].g, singleEmitter.particles[i].b);

                glVertex3f((singleEmitter.particles[i].sx + cubeSize), (singleEmitter.particles[i].sy + cubeSize), (singleEmitter.particles[i].sz - cubeSize) );    // front face
                glVertex3f((singleEmitter.particles[i].sx - cubeSize), (singleEmitter.particles[i].sy + cubeSize), (singleEmitter.particles[i].sz - cubeSize) );
                glVertex3f((singleEmitter.particles[i].sx - cubeSize), (singleEmitter.particles[i].sy - cubeSize), (singleEmitter.particles[i].sz - cubeSize) );
                glVertex3f((singleEmitter.particles[i].sx + cubeSize), (singleEmitter.particles[i].sy - cubeSize), (singleEmitter.particles[i].sz - cubeSize) );

                glVertex3f((singleEmitter.particles[i].sx + cubeSize), (singleEmitter.particles[i].sy + cubeSize), (singleEmitter.particles[i].sz - cubeSize) );    // right face
                glVertex3f((singleEmitter.particles[i].sx + cubeSize), (singleEmitter.particles[i].sy - cubeSize), (singleEmitter.particles[i].sz - cubeSize) );
                glVertex3f((singleEmitter.particles[i].sx + cubeSize), (singleEmitter.particles[i].sy - cubeSize), (singleEmitter.particles[i].sz + cubeSize) );
                glVertex3f((singleEmitter.particles[i].sx + cubeSize), (singleEmitter.particles[i].sy + cubeSize), (singleEmitter.particles[i].sz + cubeSize) );

                glVertex3f((singleEmitter.particles[i].sx + cubeSize), (singleEmitter.particles[i].sy + cubeSize), (singleEmitter.particles[i].sz - cubeSize) );    // up face
                glVertex3f((singleEmitter.particles[i].sx + cubeSize), (singleEmitter.particles[i].sy + cubeSize), (singleEmitter.particles[i].sz + cubeSize) );
                glVertex3f((singleEmitter.particles[i].sx - cubeSize), (singleEmitter.particles[i].sy + cubeSize), (singleEmitter.particles[i].sz + cubeSize) );
                glVertex3f((singleEmitter.particles[i].sx - cubeSize), (singleEmitter.particles[i].sy + cubeSize), (singleEmitter.particles[i].sz - cubeSize) );

                glVertex3f((singleEmitter.particles[i].sx - cubeSize), (singleEmitter.particles[i].sy + cubeSize), (singleEmitter.particles[i].sz - cubeSize) );    // left face
                glVertex3f((singleEmitter.particles[i].sx - cubeSize), (singleEmitter.particles[i].sy - cubeSize), (singleEmitter.particles[i].sz - cubeSize) );
                glVertex3f((singleEmitter.particles[i].sx - cubeSize), (singleEmitter.particles[i].sy - cubeSize), (singleEmitter.particles[i].sz + cubeSize) );
                glVertex3f((singleEmitter.particles[i].sx - cubeSize), (singleEmitter.particles[i].sy + cubeSize), (singleEmitter.particles[i].sz + cubeSize) );

                glVertex3f((singleEmitter.particles[i].sx + cubeSize), (singleEmitter.particles[i].sy + cubeSize), (singleEmitter.particles[i].sz + cubeSize) );    // back face
                glVertex3f((singleEmitter.particles[i].sx - cubeSize), (singleEmitter.particles[i].sy + cubeSize), (singleEmitter.particles[i].sz + cubeSize) );
                glVertex3f((singleEmitter.particles[i].sx - cubeSize), (singleEmitter.particles[i].sy - cubeSize), (singleEmitter.particles[i].sz + cubeSize) );
                glVertex3f((singleEmitter.particles[i].sx + cubeSize), (singleEmitter.particles[i].sy - cubeSize), (singleEmitter.particles[i].sz + cubeSize) );

                glVertex3f((singleEmitter.particles[i].sx - cubeSize), (singleEmitter.particles[i].sy - cubeSize), (singleEmitter.particles[i].sz - cubeSize) );    // bottom face
                glVertex3f((singleEmitter.particles[i].sx + cubeSize), (singleEmitter.particles[i].sy - cubeSize), (singleEmitter.particles[i].sz - cubeSize) );
                glVertex3f((singleEmitter.particles[i].sx + cubeSize), (singleEmitter.particles[i].sy - cubeSize), (singleEmitter.particles[i].sz + cubeSize) );
                glVertex3f((singleEmitter.particles[i].sx - cubeSize), (singleEmitter.particles[i].sy - cubeSize), (singleEmitter.particles[i].sz + cubeSize) );
              glEnd();
            }
        }//if
    }
    // the else handle the sparkling problem when pause
    else{
      if(renderMode == 1){
          glBegin(GL_POINTS);
            glColor3f(singleEmitter.particles[i].r,singleEmitter.particles[i].g,singleEmitter.particles[i].b);
            glVertex3f(singleEmitter.particles[i].sx, singleEmitter.particles[i].sy , singleEmitter.particles[i].sz);
          glEnd();
          glPointSize(1.0);
      }
      else if(renderMode == 2){
        glLineWidth(2.0);
        glBegin(GL_LINES);
          glColor3f(singleEmitter.particles[i].r, singleEmitter.particles[i].g, singleEmitter.particles[i].b);
          glVertex3f(singleEmitter.particles[i].px, singleEmitter.particles[i].py, singleEmitter.particles[i].pz);
          glVertex3f(singleEmitter.particles[i].sx, singleEmitter.particles[i].sy, singleEmitter.particles[i].sz);
        glEnd();
      }
      else if(renderMode == 3){
        glBegin(GL_QUADS);      // draw a cube with 6 quads

          glColor3f(singleEmitter.particles[i].r, singleEmitter.particles[i].g, singleEmitter.particles[i].b);

          glVertex3f((singleEmitter.particles[i].sx + cubeSize), (singleEmitter.particles[i].sy + cubeSize), (singleEmitter.particles[i].sz - cubeSize) );    // front face
          glVertex3f((singleEmitter.particles[i].sx - cubeSize), (singleEmitter.particles[i].sy + cubeSize), (singleEmitter.particles[i].sz - cubeSize) );
          glVertex3f((singleEmitter.particles[i].sx - cubeSize), (singleEmitter.particles[i].sy - cubeSize), (singleEmitter.particles[i].sz - cubeSize) );
          glVertex3f((singleEmitter.particles[i].sx + cubeSize), (singleEmitter.particles[i].sy - cubeSize), (singleEmitter.particles[i].sz - cubeSize) );

          glVertex3f((singleEmitter.particles[i].sx + cubeSize), (singleEmitter.particles[i].sy + cubeSize), (singleEmitter.particles[i].sz - cubeSize) );    // right face
          glVertex3f((singleEmitter.particles[i].sx + cubeSize), (singleEmitter.particles[i].sy - cubeSize), (singleEmitter.particles[i].sz - cubeSize) );
          glVertex3f((singleEmitter.particles[i].sx + cubeSize), (singleEmitter.particles[i].sy - cubeSize), (singleEmitter.particles[i].sz + cubeSize) );
          glVertex3f((singleEmitter.particles[i].sx + cubeSize), (singleEmitter.particles[i].sy + cubeSize), (singleEmitter.particles[i].sz + cubeSize) );

          glVertex3f((singleEmitter.particles[i].sx + cubeSize), (singleEmitter.particles[i].sy + cubeSize), (singleEmitter.particles[i].sz - cubeSize) );    // up face
          glVertex3f((singleEmitter.particles[i].sx + cubeSize), (singleEmitter.particles[i].sy + cubeSize), (singleEmitter.particles[i].sz + cubeSize) );
          glVertex3f((singleEmitter.particles[i].sx - cubeSize), (singleEmitter.particles[i].sy + cubeSize), (singleEmitter.particles[i].sz + cubeSize) );
          glVertex3f((singleEmitter.particles[i].sx - cubeSize), (singleEmitter.particles[i].sy + cubeSize), (singleEmitter.particles[i].sz - cubeSize) );

          glVertex3f((singleEmitter.particles[i].sx - cubeSize), (singleEmitter.particles[i].sy + cubeSize), (singleEmitter.particles[i].sz - cubeSize) );    // left face
          glVertex3f((singleEmitter.particles[i].sx - cubeSize), (singleEmitter.particles[i].sy - cubeSize), (singleEmitter.particles[i].sz - cubeSize) );
          glVertex3f((singleEmitter.particles[i].sx - cubeSize), (singleEmitter.particles[i].sy - cubeSize), (singleEmitter.particles[i].sz + cubeSize) );
          glVertex3f((singleEmitter.particles[i].sx - cubeSize), (singleEmitter.particles[i].sy + cubeSize), (singleEmitter.particles[i].sz + cubeSize) );

          glVertex3f((singleEmitter.particles[i].sx + cubeSize), (singleEmitter.particles[i].sy + cubeSize), (singleEmitter.particles[i].sz + cubeSize) );    // back face
          glVertex3f((singleEmitter.particles[i].sx - cubeSize), (singleEmitter.particles[i].sy + cubeSize), (singleEmitter.particles[i].sz + cubeSize) );
          glVertex3f((singleEmitter.particles[i].sx - cubeSize), (singleEmitter.particles[i].sy - cubeSize), (singleEmitter.particles[i].sz + cubeSize) );
          glVertex3f((singleEmitter.particles[i].sx + cubeSize), (singleEmitter.particles[i].sy - cubeSize), (singleEmitter.particles[i].sz + cubeSize) );

          glVertex3f((singleEmitter.particles[i].sx - cubeSize), (singleEmitter.particles[i].sy - cubeSize), (singleEmitter.particles[i].sz - cubeSize) );    // bottom face
          glVertex3f((singleEmitter.particles[i].sx + cubeSize), (singleEmitter.particles[i].sy - cubeSize), (singleEmitter.particles[i].sz - cubeSize) );
          glVertex3f((singleEmitter.particles[i].sx + cubeSize), (singleEmitter.particles[i].sy - cubeSize), (singleEmitter.particles[i].sz + cubeSize) );
          glVertex3f((singleEmitter.particles[i].sx - cubeSize), (singleEmitter.particles[i].sy - cubeSize), (singleEmitter.particles[i].sz + cubeSize) );
        glEnd();
      }
    }
  }//for
}

///////////////////////////////////////////////
void animate()
{

  int i;
  for(i = 0; i < singleEmitter.numberOfParticles; i++){

      if(!pause){
          if(singleEmitter.numberOfParticles >= 90000 && efficiencyMode == 2){
            singleEmitter.particles[i].xztimeOfParticle += 0.015;
            singleEmitter.particles[i].ytimeOfParticle += 0.015;
          }
          else if(singleEmitter.numberOfParticles >= 300000 && efficiencyMode == 2)
          {
            singleEmitter.particles[i].xztimeOfParticle += deltaTime * 100;
            singleEmitter.particles[i].ytimeOfParticle += deltaTime * 100;
          }
          else{
            singleEmitter.particles[i].xztimeOfParticle += 0.01;
            singleEmitter.particles[i].ytimeOfParticle += 0.01;
          }
          singleEmitter.particles[i].px = singleEmitter.particles[i].sx;
          singleEmitter.particles[i].py = singleEmitter.particles[i].sy;
          singleEmitter.particles[i].pz = singleEmitter.particles[i].sz;

          singleEmitter.particles[i].sx = singleEmitter.particles[i].ux * singleEmitter.particles[i].xztimeOfParticle + singleEmitter.ex;
          singleEmitter.particles[i].sz = singleEmitter.particles[i].uz * singleEmitter.particles[i].xztimeOfParticle + singleEmitter.ez;
          singleEmitter.particles[i].sy = ((singleEmitter.particles[i].vy * singleEmitter.particles[i].ytimeOfParticle) + (0.5 * gravity * singleEmitter.particles[i].ytimeOfParticle * singleEmitter.particles[i].ytimeOfParticle))*2 + singleEmitter.ey;

          // particles lossing energy after every bouncing
          if(singleEmitter.particles[i].sy < 0 && singleEmitter.particles[i].ytimeOfParticle != 0)
          {
            singleEmitter.particles[i].ytimeOfParticle = 0;
            singleEmitter.particles[i].currentEnergy *= randomBetween(energyLossMin, energyLossMax);//random energy loss
            singleEmitter.particles[i].currentSpeed = sqrt(singleEmitter.particles[i].currentEnergy * 2 / singleEmitter.particles[i].mass); // new speed due to energy loss
            singleEmitter.particles[i].vy = singleEmitter.particles[i].currentSpeed * cos(singleEmitter.particles[i].xyPlaneDirection * DEG_TO_RAD); // new vy due to new speed
          }

          // if particle dont have enough energy, re-spawn it
          if(singleEmitter.particles[i].currentEnergy < 5)
          {
            singleEmitter.particles[i].r = red;
            singleEmitter.particles[i].g = green;
            singleEmitter.particles[i].b = blue;

            singleEmitter.particles[i].xztimeOfParticle = singleEmitter.particles[i].randomInitialTime;
            singleEmitter.particles[i].ytimeOfParticle = singleEmitter.particles[i].randomInitialTime;

            singleEmitter.particles[i].px = singleEmitter.ex;
            singleEmitter.particles[i].py = singleEmitter.ey;
            singleEmitter.particles[i].pz = singleEmitter.ez;
            singleEmitter.particles[i].sx = singleEmitter.ex;
            singleEmitter.particles[i].sy = singleEmitter.ey;
            singleEmitter.particles[i].sz = singleEmitter.ez;

            singleEmitter.angleToYaxisMin = respawnAngleToYaxisMin;
            singleEmitter.angleToYaxisMax = respawnAngleToYaxisMax;
            singleEmitter.angleToXaxisMin = respawnAngleToXaxisMin;
            singleEmitter.angleToXaxisMax = respawnAngleToXaxisMax;


            singleEmitter.particles[i].xzPlaneDirection = randomBetween(singleEmitter.angleToXaxisMin, singleEmitter.angleToXaxisMax);
            singleEmitter.particles[i].xyPlaneDirection = randomBetween(singleEmitter.angleToYaxisMin, singleEmitter.angleToYaxisMax);
            singleEmitter.particles[i].initialSpeed = randomBetween(singleEmitter.spawnSpeedMin, singleEmitter.spawnSpeedMax);

            singleEmitter.particles[i].ux = singleEmitter.particles[i].initialSpeed * sin(singleEmitter.particles[i].xyPlaneDirection * DEG_TO_RAD) * cos(singleEmitter.particles[i].xzPlaneDirection * DEG_TO_RAD);
            singleEmitter.particles[i].uy = singleEmitter.particles[i].initialSpeed * cos(singleEmitter.particles[i].xyPlaneDirection * DEG_TO_RAD);
            singleEmitter.particles[i].uz = singleEmitter.particles[i].initialSpeed * sin(singleEmitter.particles[i].xyPlaneDirection * DEG_TO_RAD) * sin(singleEmitter.particles[i].xzPlaneDirection * DEG_TO_RAD);
            singleEmitter.particles[i].vx = singleEmitter.particles[i].ux;
            singleEmitter.particles[i].vy = singleEmitter.particles[i].uy;
            singleEmitter.particles[i].vz = singleEmitter.particles[i].uz;
            singleEmitter.particles[i].currentSpeed = calculateAbsoluteValue(singleEmitter.particles[i].vx, singleEmitter.particles[i].vy, singleEmitter.particles[i].vz);
            singleEmitter.particles[i].initialEnergy = singleEmitter.particles[i].mass * 0.5 * singleEmitter.particles[i].currentSpeed * singleEmitter.particles[i].currentSpeed;
            singleEmitter.particles[i].currentEnergy = singleEmitter.particles[i].initialEnergy;
      }
        }
  }//for

  calculateFPS();
  glutPostRedisplay();
}

//////////////////////////////////////////////
void display()
{
  glLoadIdentity();
  calculate_lookpoint(); // Compute the centre of interest
  gluLookAt(eyex, eyey, eyez, centerx, centery, centerz, upx, upy, upz);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glClearColor(0.0, 0.0, 0.0, 0.0); // Define background colour (0,0,0)=>black ;(1,1,1)=>white

  if (groundEnabled)
  {
    drawGround();
  }//draw ground if enable

  drawPoint(); // draw the position of each particles after calculating the new (sx,sy,sz) in animate function
  drawInfoText();

  if(axisEnabled)
  {
    glCallList(axisList); //draw axis if enable
    makeAxes();
  }

  glutSwapBuffers();
}

///////////////////////////////////////////////
void keyboard(unsigned char key, int x, int y)
{
  switch (key) {

    case 27:            exit(0);  break; //Escape key

    // re-spawn particles
    case 32:
    pause = 0;
    singleEmitter.angleToYaxisMax = initialAngleToYaxisMax;
    singleEmitter.angleToYaxisMin = initialAngleToYaxisMin;
    singleEmitter.angleToXaxisMax = initialAngleToXaxisMax;
    singleEmitter.angleToXaxisMin = initialAngleToXaxisMin;

    for(int i = 0; i < singleEmitter.numberOfParticles; i++)
    {
      singleEmitter.particles[i].xztimeOfParticle = 0.0;
      singleEmitter.particles[i].ytimeOfParticle = 0.0;

      singleEmitter.particles[i].r = red;
      singleEmitter.particles[i].g = green;
      singleEmitter.particles[i].b = blue;

      singleEmitter.particles[i].sx = singleEmitter.ex;
      singleEmitter.particles[i].sy = singleEmitter.ey;
      singleEmitter.particles[i].sz = singleEmitter.ez;

      singleEmitter.particles[i].xzPlaneDirection = randomBetween(singleEmitter.angleToXaxisMin, singleEmitter.angleToXaxisMax);
      singleEmitter.particles[i].xyPlaneDirection = randomBetween(singleEmitter.angleToYaxisMin, singleEmitter.angleToYaxisMax);
      singleEmitter.particles[i].initialSpeed = randomBetween(singleEmitter.spawnSpeedMin, singleEmitter.spawnSpeedMax);

      singleEmitter.particles[i].ux = singleEmitter.particles[i].initialSpeed * sin(singleEmitter.particles[i].xyPlaneDirection * DEG_TO_RAD) * cos(singleEmitter.particles[i].xzPlaneDirection * DEG_TO_RAD);
      singleEmitter.particles[i].uy = singleEmitter.particles[i].initialSpeed * cos(singleEmitter.particles[i].xyPlaneDirection * DEG_TO_RAD);
      singleEmitter.particles[i].uz = singleEmitter.particles[i].initialSpeed * sin(singleEmitter.particles[i].xyPlaneDirection * DEG_TO_RAD) * sin(singleEmitter.particles[i].xzPlaneDirection * DEG_TO_RAD);
      singleEmitter.particles[i].vx = singleEmitter.particles[i].ux;
      singleEmitter.particles[i].vy = singleEmitter.particles[i].uy;
      singleEmitter.particles[i].vz = singleEmitter.particles[i].uz;
      singleEmitter.particles[i].currentSpeed = calculateAbsoluteValue(singleEmitter.particles[i].vx, singleEmitter.particles[i].vy, singleEmitter.particles[i].vz);
      singleEmitter.particles[i].initialEnergy = singleEmitter.particles[i].mass * 0.5 * singleEmitter.particles[i].currentSpeed * singleEmitter.particles[i].currentSpeed;
      singleEmitter.particles[i].currentEnergy = singleEmitter.particles[i].initialEnergy;

    } break;

    // change number of particles in the system
    case 'n':
      if(!pause)  pause = 1;
      else{
        pause = 1;
        noOfParticlesInSystem -= 100;
        if(noOfParticlesInSystem < 1)
          noOfParticlesInSystem = 1;
        free(singleEmitter.particles);
        setInitialEmitterParameters();
        setInitialParticleParameters();
      }
    break;

    case 'N':
    if(!pause)  pause = 1;
    else{
      pause = 1;
      noOfParticlesInSystem += 100;
      if(noOfParticlesInSystem < 0)
        noOfParticlesInSystem = 0;
      free(singleEmitter.particles);
      setInitialEmitterParameters();
      setInitialParticleParameters();
    }
    break;

    ///////////////////// motion of camera //////////////////////
    case ',':
    eyex += RUN_SPEED * cos(lon * DEG_TO_RAD);
    eyez -= RUN_SPEED * sin(lon * DEG_TO_RAD); break;

    case '.':
    eyex -= RUN_SPEED * cos(lon * DEG_TO_RAD);
    eyez += RUN_SPEED * sin(lon * DEG_TO_RAD); break;


    case '=':
    eyey += 1; break;

    case '-':
    eyey -= 1; break;

    ///////////////// change world parameters //////////////

    // gravity
    case 'g':
    gravity += 1; break;

    case 'G':
    gravity -= 1; break;

    // initial velocity
    case '1':
    singleEmitter.spawnSpeedMin -= 1; break;
    case '!':
    singleEmitter.spawnSpeedMin += 1; break;
    case '2':
    singleEmitter.spawnSpeedMax -= 1; break;
    case '@':
    singleEmitter.spawnSpeedMax += 1; break;

    // spawn angles
    case '3':
    singleEmitter.angleToXaxisMin -= 1;
           respawnAngleToXaxisMin -= 1; break;
    case '#':
    singleEmitter.angleToXaxisMin += 1;
           respawnAngleToXaxisMin += 1; break;
    case '4':
    singleEmitter.angleToXaxisMax -= 1;
           respawnAngleToXaxisMax -= 1; break;
    case '$':
    singleEmitter.angleToXaxisMax += 1;
           respawnAngleToXaxisMax += 1; break;

    case '5':
    singleEmitter.angleToYaxisMin -= 1;
           respawnAngleToYaxisMin -= 1; break;
    case '%':
    singleEmitter.angleToYaxisMin += 1;
           respawnAngleToYaxisMin += 1; break;
    case '6':
    singleEmitter.angleToYaxisMax -= 1;
           respawnAngleToYaxisMax -= 1; break;
    case '^':
    singleEmitter.angleToYaxisMax += 1;
           respawnAngleToYaxisMax += 1; break;

    case '7':
    initialAngleToXaxisMin -= 1; break;
    case '&':
    initialAngleToXaxisMin += 1; break;
    case '8':
    initialAngleToXaxisMax -= 1; break;
    case '*':
    initialAngleToXaxisMax += 1; break;
    case '9':
    initialAngleToYaxisMin -= 1; break;
    case '(':
    initialAngleToYaxisMin += 1; break;
    case '0':
    initialAngleToYaxisMax -= 1; break;
    case ')':
    initialAngleToYaxisMax += 1; break;

    // energy loss
    case 'q':
    energyLossMin -= 0.001; break;
    case 'Q':
    energyLossMin += 0.001; break;
    case 'w':
    energyLossMax -= 0.001; break;
    case 'W':
    energyLossMax += 0.001; break;

    // colour of particles_iterator
    case 'a':
    red -= 0.005;
    if(red < 0) red = 0.0; break;
    case 'A':
    red += 0.005;
    if(red > 1) red = 1.0; break;
    case 's':
    green -= 0.005;
    if(green < 0) green = 0.0; break;
    case 'S':
    green += 0.005;
    if(green > 1) green = 1.0; break;
    case 'd':
    blue -= 0.005;
    if(blue < 0) blue = 0.0; break;
    case 'D':
    blue += 0.005;
    if(blue > 1) blue = 1.0; break;

    // pause the motion
    case 'p':
    if(pause)   pause = 0;
    else        pause = 1; break;

    case 'z':
      rDecrement -= 0.0001; break;
    case 'Z':
      rDecrement += 0.0001; break;
    case 'x':
      gDecrement -= 0.0001; break;
    case 'X':
      gDecrement += 0.0001; break;
    case 'c':
      bDecrement -= 0.0001; break;
    case 'C':
      bDecrement += 0.0001; break;

    case 'm':
      //int i = 0;
      for(int i= 0; i < singleEmitter.numberOfParticles; i++){
        singleEmitter.particles[i].mass -= 0.1;
      }
    break;

    case 'M':
      //int i = 0;
      for(int i = 0; i < singleEmitter.numberOfParticles; i++){
        singleEmitter.particles[i].mass += 0.1;
      }
    break;

    case '/':
    if(axisEnabled)   axisEnabled = 0;
    else              axisEnabled = 1; break;

    case '?':
    if(groundEnabled) groundEnabled = 0;
    else              groundEnabled = 1; break;
  }

  //glutPostRedisplay();
}

//////////////////////////////////////////////
void cursor_keys(int key, int x, int y)
{
  switch (key) {

    case GLUT_KEY_LEFT:       lon += TURN_ANGLE; break;
    case GLUT_KEY_RIGHT:      lon -= TURN_ANGLE; break;
    case GLUT_KEY_PAGE_UP:    lat += TURN_ANGLE;
    if(lat>=90)          lat  = 89;         break;
    case GLUT_KEY_PAGE_DOWN:  lat -= TURN_ANGLE;
    if(lat<=-90)         lat  = -89;        break;
    case GLUT_KEY_HOME:       lat  = lon = 0;
    eyex = 0.0; eyey = 1.7; eyez = -30;        break;

    case GLUT_KEY_UP:         eyex += RUN_SPEED * sin(lon * DEG_TO_RAD);
    eyez += RUN_SPEED * cos(lon * DEG_TO_RAD); break;
    case GLUT_KEY_DOWN:       eyex -= RUN_SPEED * sin(lon * DEG_TO_RAD);
    eyez -= RUN_SPEED * cos(lon * DEG_TO_RAD); break;
  }

  //glutPostRedisplay();
}

//////////////////////////////////////////////
void makeAxes()
{
  // Create a display list for drawing coord axis
  axisList = glGenLists(1);
  glNewList(axisList, GL_COMPILE);
  glLineWidth(2.0);
  glBegin(GL_LINES);
  glColor3f(1.0, 0.0, 0.0);       // X axis - red
  glVertex3f(-AXIS_SIZE, 0.0, 0.0);
  glVertex3f(AXIS_SIZE, 0.0, 0.0);
  glColor3f(0.0, 1.0, 0.0);       // Y axis - green
  glVertex3f(0.0, -AXIS_SIZE, 0.0);
  glVertex3f(0.0, AXIS_SIZE, 0.0);
  glColor3f(0.0, 0.0, 1.0);       // Z axis - blue
  glVertex3f(0.0, 0.0, -AXIS_SIZE);
  glVertex3f(0.0, 0.0, AXIS_SIZE);
  glEnd();
  glEndList();
}

//////////////////////////////////////////////
// Given an eyepoint and latitude and longitude angles, willcompute a look point one unit away
void calculate_lookpoint()
{
  float dirx, diry, dirz;

  dirx = cos(lat * DEG_TO_RAD) * sin(lon * DEG_TO_RAD);
  diry = sin(lat * DEG_TO_RAD);
  dirz = cos(lat * DEG_TO_RAD) * cos(lon * DEG_TO_RAD);

  centerx = eyex + dirx;
  centery = eyey + diry;
  centerz = eyez + dirz;
}

//////////////////////////////////////////////
double calculateAbsoluteValue(float x, float y, float z)
{
  return sqrt( x*x + y*y + z*z );
}

//////////////////////////////////////////////
void calculateFPS()
{
  //  Increase frame count
  frameCount++;

  //  Get the number of milliseconds since glutInit called
  //  (or first call to glutGet(GLUT ELAPSED TIME)).
  currentTime = glutGet(GLUT_ELAPSED_TIME);

  //  Calculate time passed
  float timeInterval = currentTime - lastFPSUpdateTime;

  if (timeInterval > 1000) {
    //  calculate the number of frames per second
    fps = frameCount / (timeInterval / 1000.0f);

    //  Set time
    lastFPSUpdateTime = currentTime;

    //  Reset frame count
    frameCount = 0;
  }

  // Calculate time passed since last frame
  float timeSinceLastFrameMS = currentTime - lastFrameTime;

  // Delta time is used for physica
  deltaTime = timeSinceLastFrameMS / (1000.0f);

  lastFrameTime = currentTime;
}

//////////////////////////////////////////////
void reshape(int w, int h)
{
  glViewport (0, 0, (GLsizei) w, (GLsizei) h);
  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();
  gluPerspective (60, (GLfloat) w / (GLfloat) h, 1.0, 10000.0);
  glMatrixMode (GL_MODELVIEW);
  width  = w;   // Record the new width and height
  height = h;
}

//////////////////////////////////////////////
/**
 * Takes a string and draws it "floating" at x-y
 * @param font GLUT bitmap font e.g. GLUT_BITMAP_HELVETICA_18
 * @param x Value between 0->1 representing x position
 * @param y Value between 0->1 representing y position
 * Author: Toby Howard. toby@cs.man.ac.uk.
 */
void drawString(void *font, float x, float y, char *str)
{
  char *ch; //temp pointer to store current char
  GLint matrixMode; //what mode are we in
  GLboolean lightingOn; //is lighting on?

  lightingOn = glIsEnabled(GL_LIGHTING); /* lighting on? */
  if (lightingOn) glDisable(GL_LIGHTING);

  //Borrowed from Toby's code
  glGetIntegerv(GL_MATRIX_MODE, &matrixMode); /* matrix mode atm? */

  glMatrixMode(GL_PROJECTION); //Projection mode
  glPushMatrix(); //New matrix
  glLoadIdentity();
  gluOrtho2D(0.0, 1.0, 0.0, 1.0);
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix(); //Again
  glLoadIdentity();
  glPushAttrib(GL_COLOR_BUFFER_BIT); /* save current colour */
  glColor3f(1.0, 1.0, 1.0); //set to black
  glRasterPos3f(x, y, 0.0); //where to draw?
  for (ch = str; *ch; ch++) { //for all chars...
    glutBitmapCharacter(font, (int) *ch);
  }
  glPopAttrib();
  glPopMatrix();
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(matrixMode);
  if (lightingOn)  glEnable(GL_LIGHTING);
}

//////////////////////////////////////////////
void drawGround()
{

  //Draw ground
  glDepthRange (0.1, 1.0);
  glColor3f(0.03, 0.03, 0.03);
  glBegin(GL_QUADS);
  glVertex3f(-SIZE_OF_GROUND, 0, -SIZE_OF_GROUND);
  glVertex3f(SIZE_OF_GROUND, 0, -SIZE_OF_GROUND);
  glVertex3f(SIZE_OF_GROUND, 0, SIZE_OF_GROUND);
  glVertex3f(-SIZE_OF_GROUND, 0, SIZE_OF_GROUND);
  glEnd();

  // draw grid line
  int x, z;
  glDepthRange (0.0, 0.9);
  glColor3f(0.2, 0.2, 0.2);
  glLineWidth(1.0);
  glBegin(GL_LINES);
  for (x= -SIZE_OF_GROUND; x <= SIZE_OF_GROUND; x++)  {
    glVertex3f((GLfloat) x, 0.01, -SIZE_OF_GROUND);
    glVertex3f((GLfloat) x, 0.01,  SIZE_OF_GROUND);
  }
  for (z= -SIZE_OF_GROUND; z <= SIZE_OF_GROUND; z++)  {
    glVertex3f(-SIZE_OF_GROUND, 0.01, (GLfloat) z);
    glVertex3f( SIZE_OF_GROUND, 0.01, (GLfloat) z);
  }
  glEnd();
}

///////////////////////////////////////////////
void drawInfoText()
{
  char numberStr[20] = {};
  char positionMessage[150] = {};
  char emitterPositionMessage[150] = {};
  char fpsMessage[100] = {};
  char gravityMessage[100] = {};
  char numberOfParticleMessage[100] = {};
  char spawnSpeedMessage[100] = {};
  char xzPlaneAngleMessage[100] = {};
  char xyPlaneAngleMessage[100] = {};
  char reSpawnXZPlaneAngleMessage[100] = {};
  char reSpawnXYPlaneAngleMessage[100] = {};
  char energyLossMessage[100] = {};
  char rgbMessage[100] = {};
  char rgbDecrementMessage[100] = {};
  char massMessage[100] = {};

  //  Print the FPS
  strcat(fpsMessage, "FPS : ");
  sprintf(numberStr, "%.2f", fps);
  strcat(fpsMessage, numberStr);
  strcat(fpsMessage, " ");
  drawString(GLUT_BITMAP_HELVETICA_10, 0.75, 0.95, fpsMessage);

  //  Print the number of particles
  strcat(numberOfParticleMessage, "Number of particles : ");
  sprintf(numberStr, "%d", singleEmitter.numberOfParticles);
  strcat(numberOfParticleMessage, numberStr);
  strcat(numberOfParticleMessage, " ");
  drawString(GLUT_BITMAP_HELVETICA_10, 0.75, 0.93, numberOfParticleMessage);

  //  Print the camera position
  strcat(positionMessage, "Camera Position (x ,y ,z) : (");
  sprintf(numberStr, "%.2f", eyex);
  strcat(positionMessage, numberStr);
  strcat(positionMessage, " ,");
  sprintf(numberStr, "%.2f", eyey);
  strcat(positionMessage, numberStr);
  strcat(positionMessage, " ,");
  sprintf(numberStr, "%.2f", eyez);
  strcat(positionMessage, numberStr);
  strcat(positionMessage, ")");
  drawString(GLUT_BITMAP_HELVETICA_10, 0.75, 0.91, positionMessage);

  //  Print the color of the current emiting particles
  strcat(rgbMessage, "Colour of currently emiting particles(r, g, b) : (");
  sprintf(numberStr, "%.3f", red);
  strcat(rgbMessage, numberStr);
  strcat(rgbMessage, " ,");
  sprintf(numberStr, "%.3f", green);
  strcat(rgbMessage, numberStr);
  strcat(rgbMessage, " ,");
  sprintf(numberStr, "%.3f", blue);
  strcat(rgbMessage, numberStr);
  strcat(rgbMessage, ")");
  drawString(GLUT_BITMAP_HELVETICA_10, 0.75, 0.89, rgbMessage);

  //  Print the color of the current emiting particles
  strcat(rgbDecrementMessage, "RGB decrement value(r, g, b) : (");
  sprintf(numberStr, "%.4f", rDecrement);
  strcat(rgbDecrementMessage, numberStr);
  strcat(rgbDecrementMessage, " ,");
  sprintf(numberStr, "%.4f", gDecrement);
  strcat(rgbDecrementMessage, numberStr);
  strcat(rgbDecrementMessage, " ,");
  sprintf(numberStr, "%.4f", bDecrement);
  strcat(rgbDecrementMessage, numberStr);
  strcat(rgbDecrementMessage, ")");
  drawString(GLUT_BITMAP_HELVETICA_10, 0.75, 0.87, rgbDecrementMessage);

  //  Print the spawn speed
  strcat(spawnSpeedMessage, "Initial speed range (min, max): (");
  sprintf(numberStr, "%.2f", singleEmitter.spawnSpeedMin);
  strcat(spawnSpeedMessage, numberStr);
  strcat(spawnSpeedMessage, " ,");
  sprintf(numberStr, "%.2f", singleEmitter.spawnSpeedMax);
  strcat(spawnSpeedMessage, numberStr);
  strcat(spawnSpeedMessage, ")");
  drawString(GLUT_BITMAP_HELVETICA_10, 0.75, 0.85, spawnSpeedMessage);

  //  Print the range of xz-plane angle
  strcat(xzPlaneAngleMessage, "Angle from x-axis (min, max): (");
  sprintf(numberStr, "%.2f", singleEmitter.angleToXaxisMin);
  strcat(xzPlaneAngleMessage, numberStr);
  strcat(xzPlaneAngleMessage, " ,");
  sprintf(numberStr, "%.2f", singleEmitter.angleToXaxisMax);
  strcat(xzPlaneAngleMessage, numberStr);
  strcat(xzPlaneAngleMessage, ")");
  drawString(GLUT_BITMAP_HELVETICA_10, 0.75, 0.83, xzPlaneAngleMessage);

  //  Print the range of xy-plane angle
  strcat(xyPlaneAngleMessage, "Angle from y-axis (min, max): (");
  sprintf(numberStr, "%.2f", singleEmitter.angleToYaxisMin);
  strcat(xyPlaneAngleMessage, numberStr);
  strcat(xyPlaneAngleMessage, " ,");
  sprintf(numberStr, "%.2f", singleEmitter.angleToYaxisMax);
  strcat(xyPlaneAngleMessage, numberStr);
  strcat(xyPlaneAngleMessage, ")");
  drawString(GLUT_BITMAP_HELVETICA_10, 0.75, 0.81, xyPlaneAngleMessage);

  //  Print the range of xz-plane angle
  strcat(reSpawnXZPlaneAngleMessage, "Initial angle from x-axis (min, max): (");
  sprintf(numberStr, "%.2f", initialAngleToXaxisMin);
  strcat(reSpawnXZPlaneAngleMessage, numberStr);
  strcat(reSpawnXZPlaneAngleMessage, " ,");
  sprintf(numberStr, "%.2f", initialAngleToXaxisMax);
  strcat(reSpawnXZPlaneAngleMessage, numberStr);
  strcat(reSpawnXZPlaneAngleMessage, ")");
  drawString(GLUT_BITMAP_HELVETICA_10, 0.75, 0.79, reSpawnXZPlaneAngleMessage);

  //  Print the range of xy-plane angle
  strcat(reSpawnXYPlaneAngleMessage, "Initial angle from y-axis (min, max): (");
  sprintf(numberStr, "%.2f", initialAngleToYaxisMin);
  strcat(reSpawnXYPlaneAngleMessage, numberStr);
  strcat(reSpawnXYPlaneAngleMessage, " ,");
  sprintf(numberStr, "%.2f", initialAngleToYaxisMax);
  strcat(reSpawnXYPlaneAngleMessage, numberStr);
  strcat(reSpawnXYPlaneAngleMessage, ")");
  drawString(GLUT_BITMAP_HELVETICA_10, 0.75, 0.77, reSpawnXYPlaneAngleMessage);

  //  Print the Gravity
  strcat(gravityMessage, "Gravity : ");
  sprintf(numberStr, "%.2f", gravity);
  strcat(gravityMessage, numberStr);
  strcat(gravityMessage, " ");
  drawString(GLUT_BITMAP_HELVETICA_10, 0.75, 0.75, gravityMessage);

  //  Print the range of energy loss
  strcat(energyLossMessage, "Energy Loss factor (min, max): (");
  sprintf(numberStr, "%.5f", energyLossMin);
  strcat(energyLossMessage, numberStr);
  strcat(energyLossMessage, " ,");
  sprintf(numberStr, "%.5f", energyLossMax);
  strcat(energyLossMessage, numberStr);
  strcat(energyLossMessage, ")");
  drawString(GLUT_BITMAP_HELVETICA_10, 0.75, 0.73, energyLossMessage);

  strcat(massMessage, "Mass : ");
  sprintf(numberStr, "%.2f", singleEmitter.particles[0].mass);
  strcat(massMessage, numberStr);
  drawString(GLUT_BITMAP_HELVETICA_10, 0.75, 0.71, massMessage);

}

//////////////////////////////////////////////
void viewMenu(int value)
{
  switch(value){
    case 1:
      eyex=  -0.2; // Set eyepoint at eye height within the scene
      eyey=  6.0;
      eyez=  -153.7;

      lat = 13.0;
      lon = 0.0;

      upx= 0.0;   // Set up direction to the +Y axis
      upy= 1.0;
      upz= 0.0;

      renderMode = 1;
      energyLossMin = 0.2;
      energyLossMax = 0.0;
    break;

    case 2:
      eyex=  -6.81; // Set eyepoint from the bottom to the center
      eyey=  -600.0;
      eyez=  4.11;

      lat = 89.0;
      lon = 0.0;

      upx= 0.0;   // Set up direction to the +Y axis
      upy= 1.0;
      upz= 0.0;

      renderMode = 1;
      energyLossMin = 0.2;
      energyLossMax = 0.0;
    break;

    case 3:
      eyex=  -34.17; // Set eyepoint from the bottom to the center
      eyey=  1.0;
      eyez=  -43.17;

      lat = 18.0;
      lon = -100.0;

      upx= 0.0;   // Set up direction to the +Y axis
      upy= 1.0;
      upz= 0.0;

      renderMode = 2;
      energyLossMin = 0.076;
      energyLossMax = 0.0;
    break;

    case 4:
      eyex=  -17.3; // Set eyepoint from the bottom to the center
      eyey=  1000.0;
      eyez=  -34.00;

      lat = -89.0;
      lon = 0.0;

      upx= 0.0;   // Set up direction to the +Y axis
      upy= 1.0;
      upz= 0.0;

      renderMode = 1;
      energyLossMin = 0.77299;
      energyLossMax = 0.44400;
    break;

  }
}

//////////////////////////////////////////////
void renderMenu(int value)
{
  renderMode = value;
}

//////////////////////////////////////////////
void colourMenu(int value)
{
  switch(value){
    case 1:
      red = 0.0;
      green = 0.69;
      blue = 1.0;
      rDecrement = -0.0013;
      gDecrement = 0.0039;
      bDecrement = -0.0676;
    break;

    case 2:
      red = 1.0;
      green = 0.69;
      blue = 0.295;
      rDecrement = 0.002;
      gDecrement = 0.0035;
      bDecrement = 0.001;
    break;

    case 3:
    red = 0.935;
    green = 0.770;
    blue = 0.595;
    rDecrement = 0.0072;
    gDecrement = 0.0035;
    bDecrement = 0.0041;
    break;

    case 4:
    red = 0.75;
    green = 0.545;
    blue = 0.885;
    rDecrement = 0.0061;
    gDecrement = 0.0024;
    bDecrement = 0.0034;
    break;

    case 5:
      red = 0.72;
      green = 0.62;
      blue = 0.415;
      rDecrement = -0.0023;
      gDecrement = -0.0015;
      bDecrement = -0.0016;
    break;

  }
}

//////////////////////////////////////////////
void noOfParMenu(int value)
{
  pause = 1;
  noOfParticlesInSystem = value;
  free(singleEmitter.particles);
  setInitialEmitterParameters();
  setInitialParticleParameters();
}

//////////////////////////////////////////////
void effMenu(int value)
{
  efficiencyMode = value;
}

//////////////////////////////////////////////
void mainMenu(int value)
{

}

//////////////////////////////////////////////
void initGraphics(int argc, char* argv[])
{
  setInitialViewParameters();     // Set initial view parameters
  setInitialWorldParameters();    // Set initial world parameters
  setInitialEmitterParameters();  // Set initial emitter paramters
  setInitialParticleParameters(); // Set initial particle parameters

  glutInit(&argc, argv);                                    //initialize the GLUT library
  glutInitWindowSize(width, height);                        //set window size
  glutInitWindowPosition(50, 50);                           //set window position
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); //sets the initial display mode
  glutCreateWindow("COMP37111 Particles");
  glutFullScreen();               //create a windows

  glutDisplayFunc(display);
  glutIdleFunc(animate);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard);
  glutSpecialFunc(cursor_keys);

  int subMenu1 = glutCreateMenu(viewMenu);
    glutAddMenuEntry("Fountain View", 1);
    glutAddMenuEntry("Sun View", 2);
    glutAddMenuEntry("Raining View", 3);
    glutAddMenuEntry("Less energy loss View", 4);
  int subMenu2 = glutCreateMenu(renderMenu);
    glutAddMenuEntry("Point", 1);
    glutAddMenuEntry("Particle Trail", 2);
    glutAddMenuEntry("Cube", 3);
  int subMenu3 = glutCreateMenu(colourMenu);
    glutAddMenuEntry("Blue", 1);
    glutAddMenuEntry("Red", 2);
    glutAddMenuEntry("Green", 3);
    glutAddMenuEntry("Purple", 4);
    glutAddMenuEntry("Bright", 5);
  int subMenu4 = glutCreateMenu(noOfParMenu);
    glutAddMenuEntry("100", 100);
    glutAddMenuEntry("1000", 1000);
    glutAddMenuEntry("10000", 10000);
    glutAddMenuEntry("50000", 50000);
    glutAddMenuEntry("85000", 85000);
    glutAddMenuEntry("90000", 90000);
    glutAddMenuEntry("100000", 100000);
    glutAddMenuEntry("150000", 150000);
    glutAddMenuEntry("300000", 300000);
    glutAddMenuEntry("400000", 400000);
    glutAddMenuEntry("500000", 500000);
    glutAddMenuEntry("1000000", 1000000);
  int subMenu5 = glutCreateMenu(effMenu);
    glutAddMenuEntry("Low efficiency", 1);
    glutAddMenuEntry("High efficiency", 2);

    // Create a menu
    glutCreateMenu(mainMenu);
    // Add menu items
    glutAddSubMenu("View", subMenu1);
    glutAddSubMenu("Rendering mode", subMenu2);
    glutAddSubMenu("Colour", subMenu3);
    glutAddSubMenu("Number of particles", subMenu4);
    glutAddSubMenu("Efficiency mode", subMenu5);
    glutAttachMenu(GLUT_RIGHT_BUTTON);//attached to right click
}

//////////////////////////////////////////////
int main(int argc, char** argv)
{
  srand(time(NULL));          // Intializes random number generator
  initGraphics(argc, argv);   // Initializes the initial graphics
  glEnable(GL_POINT_SMOOTH);  // Enable or disable server-side GL capabilities
  glutMainLoop();             // Enters the GLUT event processing loop
  return 0;
}

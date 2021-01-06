#pragma once
#include <iostream>
#include <fstream>
#include <gl/glut.h>
#include <GL/glext.h>
#include "OBJ_Loader.h"
const double PI = 3.1415926535897;
double r = 20;
int theta = 65, phi = 180;
double cam[3]; //Ä·ÀÇ À§Ä¡
double center[3] = { 0,0,0 };
double up[3] = { 0,1,0 };
double propeller;
objl::Loader Loader;
objl::Loader CityLoader;
objl::Loader OasisLoader;
GLuint flash[1];
GLuint texture[3];
GLuint skyTex[1];
GLuint ground[1];
GLuint cubeTex[1];
GLuint buildTex[2];
GLuint helicopter;
GLuint rotorLeft2;
GLuint rotorLeft;
GLuint rotorRight2;
GLuint rotorRight;
GLuint sky;
GLUquadricObj* lightSphere;
GLUquadricObj* disk;
GLUquadricObj* sphere[3];
GLfloat mat_emission[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat m[16];
GLfloat invertm[16];
bool power;
bool draw1;
bool draw2;
bool draw3;
bool draw4;
bool draw5;
bool clocking;
bool dup;
bool ddown;
int state = -1;
int dtexNum;
int motorSpeed;
double velo;
double zlocation;
double xlocation;
double altitude;
double frontAngle;
double backAngle;
double sideAngle;
double csideAngle;
double skySize = 150;
char h[3] = { '0','0','0' };
char v[3] = { '0','0','0' };
bool lightMode;
bool ngt;
double texAngle;
int droneAngle;
float g_pPosition[6] = { 300.0f, 300.0f, 300.0f, 200.0f, 300.0f, 100.0f };
int g_nX, g_nY;
int g_nSelect = 0;
int g_nGLWidth = 700, g_nGLHeight = 700;
bool pick;
int listindex;
double dx, dy, dz;
int oasisState = 2;
float peak;
bool droneView;
double droneEyey = 2.0;
double droneEyex = 0.0;
double droneEyez = 0.0;
int listIndex;
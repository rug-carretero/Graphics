/*
 * An OpenGL template/framework file for the Computer Graphics course
 * at the University of Groningen.
 */
// If windows is used, windows.h should be included (before gl.h and glu.h)
#if defined(_WIN32)
#include <windows.h>
#endif
// If you use GLUT you do not need to include gl.h and glu.h
// as glut.h guarantees that gl.h and glu.h are properly
// included.
// So if you do not wish to use glut.h, uncomment the following lines.
//#include <GL/gl.h>
//#include <GL/glu.h>
#if defined(NEED_GLEW)
#include "glew.h"
#endif
// Apparently Mac OS X puts the GLUT headers somewhere different.
// For windows we use freeglut.
#if defined(__APPLE__)&& defined(__MACH__)
#include <GLUT/glut.h>
#elif defined(_WIN32)
#include <GL/freeglut.h>
#else
#include <GL/glut.h>
#endif
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "lodepng.h"
#include "glslshaders.h"
#include "glm.h"
#define SPHERE_N (20)
int apertureSamples = 8;
int downwards = 1, leftwards = 1, forwards = 1;
int mouse_left_down = 0;
int mouse_right_down = 0;
float mouse_x = 0;
float mouse_y = 0;
float camera_angle_y = 0.0f;
float camera_angle_x = 0.0f;
float camera_distance = 10.0f;
GLfloat orbit = 0, rotation = 0;;
GLUquadric* quadric;
GLuint earth, venus, mars;
GLfloat cubeVertices[8*3] = {1,1,1, -1,1,1, -1,-1,1, 1,-1,1, 1,-1,-1, 1,1,-1, -1,1,-1, -1,-1,-1};
GLubyte cubeIndices[4*6] = {
  0,1,2,3,
  0,3,4,5,
  0,5,6,1,
  1,6,7,2,
  7,4,3,2,
  4,7,6,5
};
void setGlMaterial(GLfloat r, GLfloat g, GLfloat b, GLfloat ka, GLfloat kd, GLfloat ks, GLfloat n)
{
  GLfloat ambient[] = {ka*r,ka*g,ka*b,1.0};
  GLfloat diffuse[] = {kd*r,kd*g,kd*b,1.0};
  GLfloat specular[] = {ks,ks,ks,1.0};
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
  glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, n);
}
void displayTextureBalls(void)
{
  glEnable(GL_TEXTURE_2D);
  glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
  glBindTexture(GL_TEXTURE_2D,earth);
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
  gluLookAt(200.0,200.0,1000.0,200.0,200.0,0.0,0.0,1.0,0.0);
  glTranslatef(0, 0, -camera_distance);
  glRotatef(camera_angle_x, 1, 0, 0);
  glRotatef(camera_angle_y, 0, 1, 0);
  //setGlMaterial(0.0f,0.0f,1.0f,0.2,0.7,0.5,64);

    glPushMatrix();
    //glRotatef(orbit/2,320,-90,100);
    
    glTranslated(120*sin(orbit),0,100*cos(orbit));

    glRotatef(-40*rotation,-1,1,0);

    glRotated(90,1,0,0);
    glRotated(45,0,1,0);
    

    gluSphere(quadric,50,SPHERE_N,SPHERE_N);
    glPopMatrix();

  //setGlMaterial(0.0f,1.0f,0.0f,0.2,0.3,0.5,8);

    //      glBindTexture(GL_TEXTURE_2D,venus);

    
       glPushMatrix();
    glRotatef(9*orbit,210,270,300);
    glTranslated(-270,210,100);

    gluSphere(quadric,50,SPHERE_N,SPHERE_N);
    glPopMatrix();

    
  //setGlMaterial(1.0f,0.0f,0.0f,0.2,0.7,0.8,32);

       glPushMatrix();

    glRotatef(-orbit,290,170,150);
    glTranslated(-90,150,170);

    gluSphere(quadric,50,SPHERE_N,SPHERE_N);
    glPopMatrix();

  //setGlMaterial(1.0f,0.5f,0.0f,0.2,0.8,0.5,32);

     glPushMatrix();
    glRotatef(5*orbit,140,220,400);
    glTranslated(-140,400,220);

    
    
    gluSphere(quadric,50,SPHERE_N,SPHERE_N);
    glPopMatrix();

       glDisable(GL_TEXTURE_2D);     
    glPushMatrix();
    setGlMaterial(1.0f,1.0f,0.0f,0.2,0.8,0.5,100);
    gluSphere(quadric,50,SPHERE_N,SPHERE_N);
    glPopMatrix();

        setGlMaterial(1.0f,1.0f,1.0f,0.2,0.8,0.5,100);

    
  glutSwapBuffers();
}
void idle(void)
{
  orbit =  (orbit > M_PI) ? -(M_PI) : orbit + 0.01;
  rotation += 0.05;
  displayTextureBalls();
}
void reshapeTextureBalls	(int w, int h)
{
  glViewport(0,0, (GLsizei) w, (GLsizei) h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(2.0*atan2(h/2.0,1000.0)*180.0/M_PI,(GLdouble)w/(GLdouble)h,500,1000);
  glMatrixMode(GL_MODELVIEW);
}
void initTextureBalls(){
  glutDisplayFunc(displayTextureBalls);
  glutReshapeFunc(reshapeTextureBalls);
  glutIdleFunc(idle);
}
void initLights()
{
  GLfloat light_position[] = {-1000.0, 400.0, 0.0, 0.0};
  GLfloat light_asd[] = {1.0, 1.0, 1.0, 1.0};
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  glLightfv(GL_LIGHT0, GL_AMBIENT, light_asd);
  glLightfv(GL_LIGHT0, GL_SPECULAR, light_asd);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_asd);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
}
void mouse(int button, int state, int x, int y)
{
  mouse_x = x;
  mouse_y = y;
  if(button == GLUT_LEFT_BUTTON)
    {
      if(state == GLUT_DOWN)
	mouse_left_down = 1;
      else if(state == GLUT_UP)
	mouse_left_down = 0;
    }
  else if(button == GLUT_RIGHT_BUTTON)
    {
      if(state == GLUT_DOWN)
	mouse_right_down = 1;
      else if(state == GLUT_UP)
	mouse_right_down = 0;
    }
}
void mouseMotion(int x, int y)
{
  if(mouse_left_down)
    {
      camera_angle_y += (x - mouse_x);
      camera_angle_x += (y - mouse_y);
      mouse_x = x;
      mouse_y = y;
    }
  if(mouse_right_down)
    {
      camera_distance -= (y - mouse_y) * 0.2f;
      mouse_y = y;
    }
  glutPostRedisplay();
}
void keyboard(unsigned char key, int x, int y)
{
  switch (key) {
  case 'q':
  case 'Q':
  case 27: // ESC key
    printf("Exiting...\n");
    exit(0);
    break;
  }
}
GLuint initTexture(char* filename) {
  unsigned char* buffer;
  unsigned char* image;
  size_t buffersize, imagesize;
  GLuint texName;
  LodePNG_Decoder decoder;
  LodePNG_loadFile(&buffer, &buffersize, filename);
  LodePNG_Decoder_init(&decoder);
  decoder.infoRaw.color.colorType = 6; /* Load image as RGBA */
  LodePNG_decode(&decoder, &image, &imagesize, buffer, buffersize);
  if(decoder.error) {
    printf("Error reading in png image: %d\n", decoder.error);
    exit(1);
  } else {
    glPixelStorei(GL_UNPACK_ALIGNMENT,1);
    glGenTextures(1,&texName);
    glBindTexture(GL_TEXTURE_2D,texName);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,decoder.infoPng.width,
		 decoder.infoPng.height,0,GL_RGBA,GL_UNSIGNED_BYTE,image);
  }
  return texName;
}
int main(int argc, char** argv)
{
#if defined(NEED_GLEW)
  GLenum err;
#endif
  glutInit(&argc,argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(600,600);
  glutInitWindowPosition(300,250);
  glutCreateWindow("Computer Graphics - OpenGL framework");
#if defined(NEED_GLEW)
  /* Init GLEW if needed */
  err = glewInit();
  if (GLEW_OK != err) {
    /* Problem: glewInit failed, something is seriously wrong. */
    fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
    exit(1);
  }
  fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
#endif
  initLights();
  initGLSLProgram("vertexshader.glsl","fragmentshader.glsl");
  //initGLSLProgram("vertexshader.glsl","goochshader.glsl");
  quadric = gluNewQuadric();
  earth = initTexture("earth.png");
  //venus = initTexture("venus.png");
  //mars = initTexture("earth.jpg");
  gluQuadricDrawStyle(quadric, GLU_FILL);
  gluQuadricOrientation(quadric, GLU_OUTSIDE);
  gluQuadricNormals(quadric, GLU_SMOOTH);
  gluQuadricTexture(quadric, GL_TRUE);
  /* Select clearing (background) color */
  glClearColor(0.0,0.0,0.0,0.0);
  glShadeModel(GL_SMOOTH);
  glEnable(GL_DEPTH_TEST);
  /* Register GLUT callback functions */
  glutKeyboardFunc(keyboard);
  glutMouseFunc(mouse);
  glutMotionFunc(mouseMotion);
  glutIdleFunc(idle);
  initTextureBalls();
  //initObject();
  //initSphere();
  //initCube();
  glutMainLoop();
  return 0;
}


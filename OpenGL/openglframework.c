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
// So if you do not wish to use  glut.h, uncomment the following lines.
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

#define M_PI 3.141592653

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

GLfloat cubeVertices[8*3] = {
	-1,-1,-1,
	-1,-1, 1,
	-1, 1,-1,
	 1,-1,-1,
	-1, 1, 1,
	 1,-1, 1,
	 1, 1,-1,
	 1, 1, 1};
//GLubyte cubeIndices[2*12] = {
//        0,1, 0,2, 0,3,                /* From three minusses to two minusses */
//        1,4, 1,5, 2,4, 2,6, 3,5, 3,6, /* From two minusses to one minus */
//        4,7, 5,7, 6,7                 /* From one minus to zero minusses */
//    };

GLubyte cubeIndices[3*12] = {
//        0,1, 0,2, 0,3,                /* From three minusses to two minusses */
//        1,4, 1,5, 2,4, 2,6, 3,5, 3,6, /* From two minusses to one minus */
//        4,7, 5,7, 6,7                 /* From one minus to zero minusses */
  0,1,2, 4,1,2,//left panel
  5,6,7, 3,5,6,//right panel
  0,1,3, 5,1,3,//bottom panel
  0,2,6, 0,3,6,//back panel
  2,6,7, 2,4,7,//top
  4,7,5, 4,1,5 // front
    };
 
double eyeX = 0.0, eyeY = 0.0, eyeZ = 5.0,
	centerX = 0.0, centerY = 0.0, centerZ = 0.0,
	upX = 0.0, upY = 1.0, upZ = 0.0,
	
	phi = 0.0, theta = 0.0, dist = 5.0;
	
int mouseX = 0, mouseY = 0;
 
void display(void)
{
    /* Clear all pixels */
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
	
	// http://mathworld.wolfram.com/SphericalCoordinates.html
	double x = dist*sin(theta)*sin(phi),
		y = dist*cos(phi),
		z = dist*cos(theta)*sin(phi);
	
    gluLookAt(eyeX, eyeY, eyeZ, centerX, centerY, centerZ, upX, upY, upZ);
    
    glRotated(theta * 180.0/M_PI, upX, upY, upZ);
    glRotated(phi * 180.0/M_PI, 1.0, 0.0, 0.0);

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, cubeVertices);

	// draw a cube
    glColor3f(0.0f,0.0f,1.0f);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, cubeIndices);
	glColor3f(1.0f,0.0f,0.0f);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, cubeIndices+6);
	glColor3f(0.0f,1.0f,0.0f);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, cubeIndices+12);
	glColor3f(1.0f,1.0f,0.0f);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, cubeIndices+18);
	glColor3f(0.0f,1.0f,1.0f);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, cubeIndices+24);
	glColor3f(0.1f,0.0f,1.0f);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, cubeIndices+30);

	// deactivate vertex arrays after drawing
	glDisableClientState(GL_VERTEX_ARRAY);


    glutSwapBuffers();
}

void incTheta(double delta);

void incPhi(double delta){
	phi += delta;
	phi = fmod(phi, 2.0*M_PI);
	/*while(phi > M_PI){
		phi -= M_PI;
	}
	
	while(phi < 0){
		phi += M_PI;
	}*/
}
void incTheta(double delta){
	theta += delta;
	theta = fmod(theta, 2.0*M_PI);
	/*while(theta < 0){
		theta += M_PI*2.0;
	}
	
	while(theta >= M_PI*2.0){
		theta -= M_PI*2.0;
	}*/
}

void keyboard(unsigned char key, int x, int y)
{
	printf("Key: '%c'\n", key);
	
    switch (key) {
        case 'q':
        case 'Q':
        case 27: // ESC key
            printf("Exiting...\n");
            exit(0);
            break;
		case 'a':
			incTheta(-0.1);
			glutPostRedisplay();
			break;
		case 'd':
			incTheta(0.1);
			glutPostRedisplay();
			break;
		case 'w':
			incPhi(0.1);
			glutPostRedisplay();
			break;
		case 's':
			incPhi(-0.1);
			glutPostRedisplay();
			break;
    }
}

void reshape(int w, int h)
{
    glViewport(0,0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0,(GLdouble)w/(GLdouble)h,1.5,20.0);
    glMatrixMode(GL_MODELVIEW);
}

void mouse(int button, int state, int x, int y){
	if(state == GLUT_DOWN){
		mouseX = x;
		mouseY = y;
	}
}

void motion(int x, int y){
	int deltaX = x - mouseX,
		deltaY = y - mouseY;
	
	incTheta((double)deltaX/100.0);
	incPhi((double)deltaY/100.0);
	
	mouseX = x;
	mouseY = y;
	
	glutPostRedisplay();
}

#define SPHERE_N (20)

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

void displaySphere(void)
{
    /* Clear all pixels */
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    
    double dist = 250.0,
		x = dist*sin(theta)*sin(phi),
		y = dist*cos(phi),
		z = dist*cos(theta)*sin(phi);
    
    gluLookAt(200.0, 200.0, 1000.0, 200.0, 200.0, 0.0, 0.0, 1.0, 0.0);

    /* Set up other things you may need */
    /* ... */
    
    GLfloat lightPos[] = {-200.0, 600.0, 1500.0, 1.0};
    GLfloat lightAmbient[] = {1.0, 1.0, 1.0, 1.0};
    
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightAmbient);

    setGlMaterial(0.0f,0.0f,1.0f,0.2,0.7,0.5,64);
    glPushMatrix();
    glTranslated(90,320,100);
    glutSolidSphere(50,SPHERE_N,SPHERE_N);
    glPopMatrix();

    setGlMaterial(0.0f,1.0f,0.0f,0.2,0.3,0.5,8);
    glPushMatrix();
    glTranslated(210,270,300);
    glutSolidSphere(50,SPHERE_N,SPHERE_N);
    glPopMatrix();

    setGlMaterial(1.0f,0.0f,0.0f,0.2,0.7,0.8,32);
    glPushMatrix();
    glTranslated(290,170,150);
    glutSolidSphere(50,SPHERE_N,SPHERE_N);
    glPopMatrix();

    setGlMaterial(1.0f,0.8f,0.0f,0.2,0.8,0.0,1);
    glPushMatrix();
    glTranslated(140,220,400);
    glutSolidSphere(50,SPHERE_N,SPHERE_N);
    glPopMatrix();

    setGlMaterial(1.0f,0.5f,0.0f,0.2,0.8,0.5,32);
    glPushMatrix();
    glTranslated(110,130,200);
    glutSolidSphere(50,SPHERE_N,SPHERE_N);
    glPopMatrix();

    /* Whatever clean up you need */
    /* ... */

    glutSwapBuffers();
}

void reshapeSphere(int w, int h)
{
    glViewport(0,0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(2.0*atan2(h/2.0,1000.0)*180.0/M_PI,(GLdouble)w/(GLdouble)h,500,1000);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv)
{
#if defined(NEED_GLEW)
    GLenum err;
#endif


    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800,600);
    glutInitWindowPosition(220,100);
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

    /* Select clearing (background) color */
    glClearColor(0.0,0.0,0.0,0.0);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    /* Register GLUT callback functions */
    glutDisplayFunc(displaySphere);
    glutKeyboardFunc(keyboard);
    glutReshapeFunc(reshapeSphere);
	glutMotionFunc(motion);
	glutMouseFunc(mouse);

    glutMainLoop();

    return 0;
}

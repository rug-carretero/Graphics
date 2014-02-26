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

#define PI 3.141592653
#define PI_2 (PI*2)

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
	
	phi = PI/2.0, theta = 0.0, dist = 5.0;
	
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
	
    gluLookAt(x, y, z, centerX, centerY, centerZ, upX, upY, upZ);

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

void incPhi(double delta){
	phi += delta;
	while(phi > PI){
		phi -= PI;
	}
	
	while(phi < 0){
		phi += PI;
	}
}
void incTheta(double delta){
	theta += delta;
	theta = fmod(theta, PI_2);
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
		
	printf("@ %f, %f\n", centerX, centerY);
	
	incTheta(-(double)deltaX/100.0);
	incPhi(-(double)deltaY/100.0);
	
	printf("@ %f, %f\n\n", centerX, centerY);
	
	mouseX = x;
	mouseY = y;
	
	glutPostRedisplay();
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
    glShadeModel(GL_FLAT);
    glEnable(GL_DEPTH_TEST);

    /* Register GLUT callback functions */
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutReshapeFunc(reshape);
	glutMotionFunc(motion);
	glutMouseFunc(mouse);

    glutMainLoop();

    return 0;
}

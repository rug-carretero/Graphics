/*
*
* An OpenGL template/framework file for the Computer Graphics course
* at the University of Groningen.
*
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
#define GOLDEN_RATIO (.5 * (1 + sqrt(5.0)))
#define GOLDEN_ANGLE (2 * M_PI / (GOLDEN_RATIO * GOLDEN_RATIO))

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "glslshaders.h"
 
double eyeX = 0.0, eyeY = 0.0, eyeZ = 5.0,
	centerX = 0.0, centerY = 0.0, centerZ = 0.0,
	upX = 0.0, upY = 1.0, upZ = 0.0,
	
	phi = 0.0, theta = 0.0, dist = 5.0,
	fovy = 60.0, vogelRadius = 10.0;
	
int mouseX = 0, mouseY = 0, width, height, apertureSamples = 8;

/*
 * Rotation-helpers
 */
void incPhi(double delta){
	phi += delta;
	phi = fmod(phi, 2.0*M_PI);
}
void incTheta(double delta){
	theta += delta;
	theta = fmod(theta, 2.0*M_PI);
}

/*
 * Input handling
 */

void mouse(int button, int state, int x, int y){
	if(state == GLUT_DOWN){
		mouseX = x;
		mouseY = y;
	}
}

void motion(int x, int y){
	int deltaX = x - mouseX,
		deltaY = y - mouseY;
	
	if(glutGetModifiers() == GLUT_ACTIVE_CTRL){
		centerX -= deltaX;
		eyeX -= deltaX;
		
		centerY += deltaY;
		eyeY += deltaY;
	}else if(glutGetModifiers() == GLUT_ACTIVE_SHIFT){
		fovy += deltaY;
	}else{
		incTheta((double)deltaX/200.0);
		incPhi((double)deltaY/200.0);
	}
	
	mouseX = x;
	mouseY = y;
	
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
			
		case 'a':
			eyeX -= 10.0;
			centerX -= 10.0;
		break;
		case 'd':
			eyeX += 10.0;
			centerX += 10.0;
		break;
		case 'w':
			eyeZ -= 10.0;
		break;
		case 's':
			eyeZ += 10.0;
		break;
    }
	glutPostRedisplay();
}

void specialKeyboard(int key, int x, int y){
	switch(key){
		case GLUT_KEY_UP:
			incPhi(-0.05);
		break;
		case GLUT_KEY_DOWN:
			incPhi(0.05);
		break;
		case GLUT_KEY_LEFT:
			incTheta(-0.05);
		break;
		case GLUT_KEY_RIGHT:
			incTheta(0.05);
		break;
	}
	glutPostRedisplay();
}

/*
 * Cube-drawing
 */
void drawCube(void){
	static const GLfloat cubeVertices[8*3] = {
		-1,-1,-1,
		-1,-1, 1,
		-1, 1,-1,
		 1,-1,-1,
		-1, 1, 1,
		 1,-1, 1,
		 1, 1,-1,
		 1, 1, 1
	};

	static const GLubyte cubeIndices[3*12] = {
		0,1,2, 4,1,2, //left panel
		7,5,6, 3,5,6, //right panel
		0,1,3, 5,1,3, //bottom panel
		0,2,6, 0,3,6, //back panel
		2,6,7, 2,4,7, //top
		4,7,5, 4,1,5  // front
	};
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, cubeVertices);
	
	GLenum mode = GL_TRIANGLES;
	
	// draw a cube
    glColor3f(0.0f,0.0f,1.0f);
	glDrawElements(mode, 6, GL_UNSIGNED_BYTE, cubeIndices);
	glColor3f(1.0f,0.0f,0.0f);
	glDrawElements(mode, 6, GL_UNSIGNED_BYTE, cubeIndices+6);
	glColor3f(0.0f,1.0f,0.0f);
	glDrawElements(mode, 6, GL_UNSIGNED_BYTE, cubeIndices+12);
	glColor3f(1.0f,1.0f,0.0f);
	glDrawElements(mode, 6, GL_UNSIGNED_BYTE, cubeIndices+18);
	glColor3f(0.0f,1.0f,1.0f);
	glDrawElements(mode, 6, GL_UNSIGNED_BYTE, cubeIndices+24);
	glColor3f(1.0f,0.0f,1.0f);
	glDrawElements(mode, 6, GL_UNSIGNED_BYTE, cubeIndices+30);

	// deactivate vertex arrays after drawing
	glDisableClientState(GL_VERTEX_ARRAY);
}
 
void displayCube(void)
{
    /* Clear all pixels */
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
	
    gluLookAt(eyeX, eyeY, eyeZ, centerX, centerY, centerZ, upX, upY, upZ);
    
    glRotated(theta * 180.0/M_PI, upX, upY, upZ);
    glRotated(phi * 180.0/M_PI, 1.0, 0.0, 0.0);
	
	drawCube();

    glutSwapBuffers();
}

void reshapeCube(int w, int h)
{
    glViewport(0,0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fovy,(GLdouble)w/(GLdouble)h,1.5,20.0);
    glMatrixMode(GL_MODELVIEW);
}

/*
 * Sphere-drawing
 */
const int SPHERE_N = 20;

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

void drawSpheres(void){
    const static GLfloat lightPos[] = {-200.0, 600.0, 1500.0, 1.0};
    const static GLfloat lightAmbient[] = {1.0, 1.0, 1.0, 1.0};
    
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
}

void rePerspectifySphere(){
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fovy, (GLdouble)width/(GLdouble)height, 200, 10000);
    glMatrixMode(GL_MODELVIEW);
}

void displaySphere(void)
{
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	
	rePerspectifySphere();
	
    /* Clear all pixels */
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT | GL_ACCUM_BUFFER_BIT);
    glLoadIdentity();
    
    double c = vogelRadius / sqrt(apertureSamples);
        
	for(int i = 0;i < apertureSamples; i++){
		
		double vogelTheta = i * GOLDEN_ANGLE;
		double r = c * sqrt(i); 
		/*ugly hack, sould not be necessary, but it works*/
		glLoadIdentity();
     
		// gluLookAt(200.0, 200.0, 1000.0, 200.0, 200.0, 200.0, 0.0, 1.0, 0.0);
		gluLookAt(eyeX+sin(vogelTheta)*r, eyeY+cos(vogelTheta)*r, eyeZ, centerX, centerY, centerZ, upX, upY, upZ);
		
		glTranslated(eyeX, eyeY, eyeZ);
		glRotated(theta * 180.0/M_PI, upX, upY, upZ);
		glRotated(phi * 180.0/M_PI, 1.0, 0.0, 0.0);
		glTranslated(-eyeX, -eyeY, -eyeZ);
	
		drawSpheres();
		glAccum(i ? GL_ACCUM : GL_LOAD, 1.0 / apertureSamples);
		glFlush();
	}

	glAccum(GL_RETURN, 1);

    glutSwapBuffers();
}

void reshapeSphere(int w, int h)
{
    glViewport(0,0, (GLsizei) w, (GLsizei) h);
    width = w;
    height = h;
    fovy = 2.0*atan2(height/2.0,1000.0)*180.0/M_PI;
    rePerspectifySphere();
}

/*
 * Initializations
 */
void initCube(){
	eyeX = 0.0, eyeY = 0.0, eyeZ = 5.0,
	centerX = 0.0, centerY = 0.0, centerZ = 0.0,
	upX = 0.0, upY = 1.0, upZ = 0.0, fovy = 60.0;
	
	glutDisplayFunc(displayCube);
    glutReshapeFunc(reshapeCube);
}

void initSphere(){ 
	eyeX = 200.0, eyeY = 200.0, eyeZ = 1000.0,
	centerX = 200.0, centerY = 200.0, centerZ = 200.0,
	upX = 0.0, upY = 1.0, upZ = 0.0, 
	fovy = 2.0*atan2(height/2.0,1000.0)*180.0/M_PI;
	
	glutDisplayFunc(displaySphere);
    glutReshapeFunc(reshapeSphere);
}

int main(int argc, char** argv)
{
#if defined(NEED_GLEW)
    GLenum err;
#endif

    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_ACCUM);
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
	
	//initGLSLProgram("normalvertex.glsl","normalfragment.glsl");

    /* Register GLUT callback functions */
    glutKeyboardFunc(keyboard);
	glutSpecialFunc(specialKeyboard);
	glutMotionFunc(motion);
	glutMouseFunc(mouse);
	
	initSphere();

    glutMainLoop();

    return 0;
}

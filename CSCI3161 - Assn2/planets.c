
/************************************************************************************

	File: 			multipleCubes.c

	Description:	A complete OpenGL program to rotate cubes with color interpolation.
					Demonstration of use of homogeneous coordinate transformations and
					simple data structure for representing cube from Chapter 4.

					Mouse buttons control direction of rotation.


	Author:			E. Angel, modified by Stephen Brooks

*************************************************************************************/



/* include the library header files */
#include <stdlib.h>
#include <freeglut.h>

// cube vertices
GLfloat vertices[][3] = { {-1.0,-1.0,-1.0}, {1.0,-1.0,-1.0}, {1.0,1.0,-1.0}, {-1.0,1.0,-1.0},
							{-1.0,-1.0, 1.0}, {1.0,-1.0, 1.0}, {1.0,1.0, 1.0}, {-1.0,1.0, 1.0} };

// colors of the vertices
GLfloat colors[][3] = { {0.0,0.0,0.0}, {1.0,0.0,0.0}, {1.0,1.0,0.0}, {0.0,1.0,0.0},
							{0.0,0.0,1.0}, {1.0,0.0,1.0}, {1.0,1.0,1.0}, {0.0,1.0,1.0} };

// angle of rotation
GLfloat theta = 0.0;
// axis of rotation
GLint   axis = 0;



/************************************************************************

	Function:		polygon

	Description:	Draw a polygon via list of vertices.

*************************************************************************/
void polygon(int a, int b, int c, int d)
{

	glBegin(GL_POLYGON);
	glColor3fv(colors[a]);
	glVertex3fv(vertices[a]);

	glColor3fv(colors[b]);
	glVertex3fv(vertices[b]);

	glColor3fv(colors[c]);
	glVertex3fv(vertices[c]);

	glColor3fv(colors[d]);
	glVertex3fv(vertices[d]);
	glEnd();
}


/************************************************************************

	Function:		colorcube

	Description:	Map the vertices to cube faces.

*************************************************************************/
void colorcube()
{
	polygon(0, 3, 2, 1);
	polygon(2, 3, 7, 6);
	polygon(0, 4, 7, 3);
	polygon(1, 2, 6, 5);
	polygon(4, 5, 6, 7);
	polygon(0, 1, 5, 4);
}


/************************************************************************

	Function:		myDisplay

	Description:	Display callback, clears frame buffer and depth buffer,
					rotates the cube and draws it.

*************************************************************************/
void myDisplay(void)
{

	// clear the screen and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// load the identity matrix into the model view matrix
	glLoadIdentity();

	// push the current transformation state on the stack
	glPushMatrix();

	// rotate depending on the axis
	switch (axis)
	{
	case 0:
		glRotatef(theta, 1.0, 0.0, 0.0);
		break;
	case 1:
		glRotatef(theta, 0.0, 1.0, 0.0);
		break;
	case 2:
		glRotatef(theta, 0.0, 0.0, 1.0);
		break;
	}
	// draw the color cube
	colorcube();

	// pop the previous transformation state from the stack
	glPopMatrix();

	// rotate it, always around X-axis
	glRotatef(theta * 10, 1.0, 0.0, 0.0);

	// move it
	glTranslatef(1.7, 0, 0);

	// scale it 
	glScalef(0.2, 0.2, 0.2);

	// draw the color cube
	colorcube();

	// even smaller
	// rotate it, always around X-axis
	glRotatef(theta * 20, 1.0, 0.0, 0.0);

	// move it
	glTranslatef(0, 2, 0);

	// scale it 
	glScalef(0.1, 0.1, 0.1);

	// draw the color cube
	colorcube();


	// swap the drawing buffers
	glutSwapBuffers();
}


/************************************************************************

	Function:		myIdle

	Description:	Updates the animation when idle.

*************************************************************************/
void myIdle()
{

	// update the rotation around the selected axis 
	theta += 0.01f;

	// redraw the new state
	glutPostRedisplay();
}


/************************************************************************

	Function:		myMouse

	Description:	Selects an axis about which to rotate.

*************************************************************************/
void myMouse(int btn, int state, int x, int y)
{

	// if the left mouse button, then rotate around the X-axis
	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		axis = 0;

	// if the middle mouse button, then rotate around the Y-axis
	if (btn == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN)
		axis = 1;

	// if the right mouse button, then rotate around the Z-axis
	if (btn == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
		axis = 2;
}


/************************************************************************

	Function:		initializeGL

	Description:	Initializes the OpenGL rendering context for display.

*************************************************************************/
void initializeGL(void)
{
	// enable depth testing
	glEnable(GL_DEPTH_TEST);

	// set background color to be black
	glClearColor(0, 0, 0, 1.0);

	// change into projection mode so that we can change the camera properties
	glMatrixMode(GL_PROJECTION);

	// load the identity matrix into the projection matrix
	glLoadIdentity();

	// set window mode to 2D orthographic 
	glOrtho(-2.0, 2.0, -2.0, 2.0, -10, 10.0);

	// change into model-view mode so that we can change the object positions
	glMatrixMode(GL_MODELVIEW);
}



/************************************************************************

	Function:		main

	Description:	Sets up the openGL rendering context and the windowing
					system, then begins the display loop.

*************************************************************************/
void main(int argc, char** argv)
{
	// initialize the toolkit
	glutInit(&argc, argv);
	// set display mode
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	// set window size
	glutInitWindowSize(500, 500);
	// set window position on screen
	glutInitWindowPosition(100, 150);
	// open the screen window
	glutCreateWindow("colorcube");

	// register redraw function
	glutDisplayFunc(myDisplay);
	// register the idle function
	glutIdleFunc(myIdle);
	// register the mouse button function
	glutMouseFunc(myMouse);

	//initialize the rendering context
	initializeGL();
	// go into a perpetual loop
	glutMainLoop();
}

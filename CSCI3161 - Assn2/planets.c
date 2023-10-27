
/************************************************************************************

	File: 			simple.c

	Description:	A complete OpenGL program to draw a square on the screen.

	Author:			Stephen Brooks

*************************************************************************************/


/* include the library header files */
#include <freeglut.h>



/************************************************************************

	Function:		myDisplay

	Description:	Displays a black square on a black background.

*************************************************************************/
void myDisplay(void)
{
	// clear the screen 
	glClear(GL_COLOR_BUFFER_BIT);
	// draw the square
	glBegin(GL_POLYGON);
	glVertex2f(-0.5, -0.5);
	glVertex2f(-0.5, 0.5);
	glVertex2f(0.5, 0.5);
	glVertex2f(0.5, -0.5);
	glEnd();
	// send all output to display 
	glFlush();
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


	// open the screen window
	glutCreateWindow("simple");
	// register redraw function
	glutDisplayFunc(myDisplay);

	// go into a perpetual loop
	glutMainLoop();
}


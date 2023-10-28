


/* include the library header files */
#include <stdlib.h>
#include <stdio.h>
#include <freeglut.h>
#include <time.h>

#define STAR_COUNT 150


void drawStars(void);
void twinkleStars(void);
void printKeyboardControls(void);

GLint windowWidth = 900;
GLint windowHeight = 600;

GLfloat xRange = 2.0f;
GLfloat yRange = 2.0f;
GLfloat zRange = 10.0f;

GLint starsToggled = 0;

GLfloat starPositions[STAR_COUNT][3];
GLfloat starColors[STAR_COUNT][3];

void myDisplay(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	if (starsToggled == 1) {
		drawStars();
	}



	glutSwapBuffers();
}

void myIdle(void) {
	twinkleStars();

	glutPostRedisplay();
}

void drawStars(void) {
	glPointSize(2.0f);
	for (int i = 0; i < STAR_COUNT; i++) {
		glBegin(GL_POINTS);
		glColor3fv(starColors[i]);
		glVertex3fv(starPositions[i]);
		glEnd();
	}
	glPointSize(1.0f);
}

void twinkleStars(void) {
	for (int i = 0; i < STAR_COUNT; i++) {
		GLfloat randColR = (GLfloat)rand() / RAND_MAX;
		GLfloat randColB = (GLfloat)rand() / RAND_MAX;
		GLfloat randColG = (GLfloat)rand() / RAND_MAX;
		starColors[i][0] = randColR;
		starColors[i][1] = randColB;
		starColors[i][2] = randColG;
	}
}

void myKeyboard(unsigned char key, int x, int y) {
	if (key == 's') {
		if (starsToggled == 0) {
			starsToggled = 1;
		}
		else {
			starsToggled = 0;
		}
	}
	glutPostRedisplay();
}

void initializeStars(void) {
	for (int i = 0; i < STAR_COUNT; i++) {
		GLfloat randPosX = 2 * xRange * (GLfloat) rand() / RAND_MAX - xRange;
		GLfloat randPosY = 2 * yRange * (GLfloat) rand() / RAND_MAX - yRange;
		starPositions[i][0] = randPosX;
		starPositions[i][1] = randPosY;
		starPositions[i][2] = -zRange + 1;

		GLfloat randColR = (GLfloat)rand() / RAND_MAX;
		GLfloat randColB = (GLfloat)rand() / RAND_MAX;
		GLfloat randColG = (GLfloat)rand() / RAND_MAX;
		starColors[i][0] = randColR;
		starColors[i][1] = randColB;
		starColors[i][2] = randColG;
	}
}


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
	glOrtho(-xRange, xRange, -yRange, yRange, -zRange, zRange);

	// change into model-view mode so that we can change the object positions
	glMatrixMode(GL_MODELVIEW);
}


void main(int argc, char** argv)
{
	srand(time(NULL));

	initializeStars();
	// initialize the toolkit
	glutInit(&argc, argv);
	// set display mode
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	// set window size
	glutInitWindowSize(windowWidth, windowHeight);
	// set window position on screen
	glutInitWindowPosition(100, 150);
	// open the screen window
	glutCreateWindow("The Hitch Hikers Guide to the Planets");

	// register redraw function
	glutDisplayFunc(myDisplay);
	// register the idle function
	glutIdleFunc(myIdle);

	glutKeyboardFunc(myKeyboard);

	//initialize the rendering context
	initializeGL();

	printKeyboardControls();
	// go into a perpetual loop
	glutMainLoop();
}

void printKeyboardControls(void)
{
	printf("Scene Controls\n-------------------\n\n");
	printf("r \t: toggle rings\n");
	printf("s \t: toggle stars\n");
	printf("c \t: toggle sun's corona\n");
	printf("click \t: shoot lasers\n\n");
	
	printf("Camera Controls\n------------------- \n\n");
	printf("Up \tArrow\t: \tmove up\n");
	printf("Down \tArrow\t: \tmove down\n");
	printf("Right \tArrow\t: \tmove right\n");
	printf("Left \tArrow\t: \tmove left\n");
	printf("PAGE \tUP\t: \tforward\n");
	printf("PAGE \tDOWN\t: \tbackward\n");


	printf("\nNote: May need to use FN key to use Page Up and Page Down on Laptops.\n");
}
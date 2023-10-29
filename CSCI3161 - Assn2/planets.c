


/* include the library header files */
#include <stdlib.h>
#include <stdio.h>
#include <freeglut.h>
#include <time.h>
#include <math.h>

#define STAR_COUNT 500
#define ENTERPRISE_POINT_COUNT 1201
#define ENTERPRISE_TRIANGLE_COUNT 1989


void drawStars(void);
void twinkleStars(void);
void printKeyboardControls(void);
void drawEnterprise(void);
void drawPlanets(void);
void drawAxis(void);
void drawOrbits(void);

GLint windowWidth = 900;
GLint windowHeight = 600;

GLfloat camPos[] = { -1, 2, 5 };
GLfloat theta = 0.0f;

GLfloat xRange = 2.0f;
GLfloat yRange = 2.0f;
GLfloat zRange = 10.0f;

GLint starsToggled = 0;
GLint orbitsToggled = 0;

GLfloat starPositions[STAR_COUNT][3];
GLfloat starColors[STAR_COUNT][3];

GLfloat enterprisePoints[ENTERPRISE_POINT_COUNT][3];
GLint enterpriseTriangles[ENTERPRISE_TRIANGLE_COUNT][3];

void myDisplay(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	gluLookAt(camPos[0], camPos[1], camPos[2], 0, 0, 0, 0, 1, 0);

	drawAxis();

	if (starsToggled == 1) {
		drawStars();
	}
	//drawEnterprise();
	if (orbitsToggled == 1) {
		drawOrbits();
	}

	drawPlanets();







	glutSwapBuffers();
}

void myIdle(void) {
	twinkleStars();
	theta += 0.1f;
	glutPostRedisplay();
}

void drawPlanets() {
	glScalef(0.2, 0.2, 0.2);
	glColor3f(1.0f, 1.0f, 0.0f);
	GLUquadric* quad = gluNewQuadric();
	gluSphere(quad, 1.0, 32, 32);

	glRotatef(theta * 10, 0, 1, 0);
	glTranslatef(4.0, 0.0, 0.0);
	glScalef(0.5, 0.5, 0.5);

	glColor3f(0.0f, 1.0f, 1.0f);

	gluSphere(quad, 1.0, 32, 32);


	glRotatef(theta * 20, 0, 1, 0);
	glTranslatef(2.0, 0.0, 0.0);
	glScalef(0.25, 0.25, 0.25);

	glColor3f(1.0f, 0.0f, 0.0f);
	gluSphere(quad, 1.0, 32, 32);
}

void drawOrbits() {
	glPushMatrix();
	glColor3f(0.0f, 1.0f, 0.0f);
	glScalef(0.2, 0.2, 0.2);

	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 360; i += 5) {
		GLfloat angle = i * 3.141592 / 180.0;
		GLfloat x = 4 * cos(angle);
		GLfloat z = 4 * sin(angle);
		glVertex3f(x, 0.0, z);
	}

	glEnd();
	glPopMatrix();
}

void drawAxis(void) {
	glBegin(GL_LINES);
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-xRange, 0, 0);
	glVertex3f(xRange, 0, 0);

	glVertex3f(0, -yRange, 0);
	glVertex3f(0, yRange, 0);

	glVertex3f(0, 0, -zRange);
	glVertex3f(0, 0, zRange);
	glEnd();
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
	else if (key == 'r') {
		if (orbitsToggled == 0) {
			orbitsToggled = 1;
		}
		else {
			orbitsToggled = 0;
		}
	}
	glutPostRedisplay();
}

void initializeStars(void) {
	for (int i = 0; i < STAR_COUNT; i++) {
		GLfloat randPosX = 10 * xRange * (GLfloat) rand() / RAND_MAX - 5 * xRange;
		GLfloat randPosY = 10 * yRange * (GLfloat) rand() / RAND_MAX - 5 * yRange;
		GLfloat randPosZ = 2 * zRange * (GLfloat)rand() / RAND_MAX - zRange;
		starPositions[i][0] = randPosX;
		starPositions[i][1] = randPosY;
		starPositions[i][2] = randPosZ;

		GLfloat randColR = (GLfloat)rand() / RAND_MAX;
		GLfloat randColB = (GLfloat)rand() / RAND_MAX;
		GLfloat randColG = (GLfloat)rand() / RAND_MAX;
		starColors[i][0] = randColR;
		starColors[i][1] = randColB;
		starColors[i][2] = randColG;
	}
}

void initializeEnterprise(void) {
	FILE* file;
	if (fopen_s(&file, "enterprise.txt", "r") != 0) {
		printf("Error opening file\n");
		return;
	}

	char line[128];
	int pointIndex = 0;
	int triangleIndex = 0;
	
	for (int i = 0; i < 3190; i++) {
		fgets(line, sizeof(line), file);
		if (line[0] == 'v') {
			sscanf_s(line, "v %f %f %f", &enterprisePoints[pointIndex][0], &enterprisePoints[pointIndex][1], &enterprisePoints[pointIndex][2]);
			pointIndex++;
		}
		else if (line[0] == 'f') {
			sscanf_s(line, "f %d %d %d", &enterpriseTriangles[triangleIndex][0], &enterpriseTriangles[triangleIndex][1], &enterpriseTriangles[triangleIndex][2]);
			triangleIndex++;
		}
	}

	fclose(file);

}

void drawEnterprise(void) {
	GLint colorVal = 1989;
	for (int i = 0; i < ENTERPRISE_TRIANGLE_COUNT; i++) {
		glBegin(GL_TRIANGLES);
		glColor3f(colorVal/3978.0f, colorVal/3978.0f, colorVal/3978.0f);
		glVertex3fv(enterprisePoints[enterpriseTriangles[i][0] - 1]);
		glVertex3fv(enterprisePoints[enterpriseTriangles[i][1] - 1]);
		glVertex3fv(enterprisePoints[enterpriseTriangles[i][2] - 1]);
		glEnd();
		colorVal++;
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
	//glOrtho(-xRange, xRange, -yRange, yRange, -zRange, zRange);
	gluPerspective(45, (float)windowWidth / (float)windowHeight, 0.1, 20);

	// change into model-view mode so that we can change the object positions
	glMatrixMode(GL_MODELVIEW);
}


void main(int argc, char** argv)
{
	srand(time(NULL));


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


	initializeStars();
	initializeEnterprise();

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



/* include the library header files */
#include <stdlib.h>
#include <stdio.h>
#include <freeglut.h>
#include <time.h>
#include <math.h>
#include <string.h>

#define STAR_COUNT 500
#define CORONA_LINE_COUNT 160
#define ENTERPRISE_POINT_COUNT 1201
#define ENTERPRISE_TRIANGLE_COUNT 1989
#define PLANET_COUNT 11
#define M_PI 3.141592


void drawStars(void);
void twinkleStars(void);
void printKeyboardControls(void);
void drawEnterprise(void);
void drawPlanets(void);
void drawAxis(void);
void drawOrbits(void);
void moveEnterprise(void);
void positionCamera(void);
void initializePlanets(void);
void drawCorona(void);
void randomizeCorona(void);
void drawLaser(void);

typedef struct {
	GLint isMoon;
	GLfloat size;
	GLfloat offset;
	GLfloat speed;
	GLfloat rotations[3];
	GLfloat colors[3];
	GLint moonIndex;
} Planet;

Planet planets[PLANET_COUNT];

GLint moonSetting[] = { 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1};
GLfloat planetSizes[] = { 0.2, 0.5, 0.6, 0.3, 0.6, 0.5, 0.25, 0.8, 0.4, 0.7, 0.7 };
GLfloat planetOffset[] = { 0.0, 4.0, 2.0, 20.0, 10.0, 15.0, 2.0, 18.0, 2.0, 25.0, 2.0 };
GLfloat planetSpeeds[] = { 0.0, 10.0, 20.0, 5.0, 15.0, 8.0, 5.0, 5.0, 13.0, 3.0, 20.0 };
GLfloat planetColors[][3] = { {1.0, 1.0, 0.0}, {0.0, 1.0, 1.0}, {0.5, 0.5, 0.5}, {1.0, 0.0, 1.0}, {0.75, 0.1, 0.25}, {0.05, 0.8, 0.1}, {0.75, 0.75, 0.75}, {0.0, 0.25, 0.8}, {0.9, 0.9, 0.9}, {0.5, 0.0, 0.5}, {0.25, 0.25, 0.25} };
GLfloat planetRotations[][3] = { {0.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 1.0, 0.0} };
GLint moonIndices[] = { -1, 2, -1, -1, -1, 6, -1, 8, -1, 10, -1 };

GLint tiltedPlanetIndex = 7;

//Third planet moon (2), seventh planet moon (6), 

GLint windowWidth = 900; 
GLint windowHeight = 600;

GLfloat camPosOffset[3] = {0.0, 0.31, 0.72};
GLfloat camPos[3] = { 0.0, 1, 10.0 };
GLfloat theta = 0.0f;

GLfloat xRange = 2.0f;
GLfloat yRange = 2.0f;
GLfloat zRange = 10.0f;

GLint starsToggled = 0;
GLint orbitsToggled = 0;
GLint coronaToggled = 0;

GLfloat starPositions[STAR_COUNT][3];
GLfloat starColors[STAR_COUNT][3];

GLfloat coronaPositions[CORONA_LINE_COUNT];
GLfloat coronaLength = 0.5;

GLfloat enterprisePoints[ENTERPRISE_POINT_COUNT][3];
GLint enterpriseTriangles[ENTERPRISE_TRIANGLE_COUNT][3];
GLfloat enterpriseSpeed = 0.1;
GLfloat enterpriseScale = 0.55;

GLint leftLaser = 0;
GLint rightLaser = 0;
GLfloat laserSpawnPos[3];
GLfloat leftLaserTimer = 0;
GLfloat rightLaserTimer = 0;
GLfloat laserTimer = 2;
GLfloat laserSpeed = 0.25f;

GLfloat leftLaserPos[] = { 0, 0, 0 };
GLfloat rightLaserPos[] = { 0, 0, 0 };

void myDisplay(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();

	positionCamera();
	drawAxis();


	if (starsToggled == 1) {
		drawStars();
	} 
	drawEnterprise();


	if (orbitsToggled == 1) {
		drawOrbits();
	}

	drawPlanets();

	drawLaser();

	if (coronaToggled == 1) {
		drawCorona();
	}

	glutSwapBuffers();
}

void drawLaser(void) {
	glPushMatrix();
	glTranslatef(0, 0, 7);
	glScalef(enterpriseScale, enterpriseScale, enterpriseScale);
	if (leftLaser == 1) {
		glLineWidth(5.0f);
		glBegin(GL_LINES);
		glColor4f(1.0, 0.0, 0.0, 1.0);
		glVertex3fv(leftLaserPos);
		glColor4f(0.0, 0.0, 1.0, 0.25);
		glVertex3f(leftLaserPos[0], leftLaserPos[1], leftLaserPos[2] - 0.5);
		glEnd();
	}
	if (rightLaser == 1) {
		glLineWidth(5.0f);
		glBegin(GL_LINES);
		glColor4f(1.0, 0.0, 0.0, 1.0);
		glVertex3fv(rightLaserPos);
		glColor4f(0.0, 0.0, 1.0, 0.25);
		glVertex3f(rightLaserPos[0], rightLaserPos[1], rightLaserPos[2] - 0.5);
		glEnd();
	}

	glPopMatrix();
	glLineWidth(1.0f);
}


void myIdle(void) {
	twinkleStars();
	randomizeCorona();
	theta += 0.1f;

	if (leftLaserTimer > 0) {
		leftLaserTimer -= 0.1f;
		leftLaserPos[2] -= laserSpeed;
		if (leftLaserTimer <= 0) {
			leftLaserTimer = 0;
			leftLaser = 0;
		}
	}

	if (rightLaserTimer > 0) {
		rightLaserTimer -= 0.1f;
		rightLaserPos[2] -= laserSpeed;
		if (rightLaserTimer <= 0) {
			rightLaserTimer = 0;
			rightLaser = 0;
		}
	}

	glutPostRedisplay();
}

void positionCamera(void) {
	GLfloat enterpriseX = enterprisePoints[ENTERPRISE_POINT_COUNT/2][0] * enterpriseScale;
	GLfloat enterpriseY = enterprisePoints[ENTERPRISE_POINT_COUNT / 2][1] * enterpriseScale;
	GLfloat enterpriseZ = enterprisePoints[ENTERPRISE_POINT_COUNT / 2][2] * enterpriseScale;

	GLfloat cameraX = enterpriseX + camPosOffset[0];
	GLfloat cameraY = enterpriseY + camPosOffset[1];
	GLfloat cameraZ = enterpriseZ + camPosOffset[2] + 2;

	//gluLookAt(cameraX, cameraY, cameraZ, enterpriseX, enterpriseY, enterpriseZ, 0.0, 1.0, 0.0);
	gluLookAt(camPos[0], camPos[1], camPos[2], enterpriseX, enterpriseY, enterpriseZ, 0.0, 1.0, 0.0);
}

void drawPlanets() {
	glPushMatrix();
	GLUquadric* quad = gluNewQuadric();
	glScalef(planets[0].size, planets[0].size, planets[0].size);
	glColor3fv(planets[0].colors);
	gluSphere(quad, 1.0, 32, 32);

	for (int i = 1; i < PLANET_COUNT; i++) {
		glPushMatrix();
		if (planets[i].isMoon == 0) {
			if (i == tiltedPlanetIndex) {
				glRotatef(-25, 0, 0, 1);
			}

			glRotatef(theta * planets[i].speed, planets[i].rotations[0], planets[i].rotations[1], planets[i].rotations[2]);
			glTranslatef(planets[i].offset, 0.0, 0.0);
			glScalef(planets[i].size, planets[i].size, planets[i].size);
			glColor3fv(planets[i].colors);
			gluSphere(quad, 1.0, 32, 32);
			
			if (planets[i].moonIndex != -1) {
				GLint index = planets[i].moonIndex;

				glRotatef(theta * planets[index].speed, planets[index].rotations[0], planets[index].rotations[1], planets[index].rotations[2]);
				glTranslatef(planets[index].offset, 0.0, 0.0);
				glScalef(planets[index].size, planets[index].size, planets[index].size);
				glColor3fv(planets[index].colors);
				gluSphere(quad, 1.0, 32, 32);

			}
		}
		glPopMatrix();
	}



	glPopMatrix();
}


void initializePlanets() {

	for (int i = 0; i < PLANET_COUNT; i++) {
		planets[i].isMoon = moonSetting[i];
		planets[i].size = planetSizes[i];
		planets[i].offset = planetOffset[i];
		planets[i].speed = planetSpeeds[i];
		memcpy(planets[i].rotations, planetRotations[i], 3 * sizeof(GLfloat));
		memcpy(planets[i].colors, planetColors[i], 3 * sizeof(GLfloat));
		planets[i].moonIndex = moonIndices[i];

	}
}


void drawOrbits() {
	glPushMatrix();
	glColor3f(1.0f, 1.0f, 1.0f);
	glScalef(planets[0].size, planets[0].size, planets[0].size);

	for (int i = 0; i < PLANET_COUNT; i++) {
		glPushMatrix();
		if (planets[i].isMoon == 0) {
			if (i == tiltedPlanetIndex) {
				glRotatef(-25, 0, 0, 1);
			}
			glBegin(GL_LINE_LOOP);
			for (int j = 0; j < 360; j += 5) {
				GLfloat angle = j * M_PI / 180.0;
				GLfloat x = planets[i].offset * cos(angle);
				GLfloat z = planets[i].offset * sin(angle);
				glVertex3f(x, 0.0, z);
			}
			glEnd();

			if (planets[i].moonIndex != -1) {

				GLint index = planets[i].moonIndex;
				glRotatef(theta * planets[i].speed, planets[i].rotations[0], planets[i].rotations[1], planets[i].rotations[2]);
				glTranslatef(planets[i].offset, 0.0, 0.0);
				glScalef(planets[i].size, planets[i].size, planets[i].size);

				glBegin(GL_LINE_LOOP);
				for (int j = 0; j < 360; j += 5) {
					GLfloat angle = j * M_PI / 180.0;
					GLfloat x = planets[index].offset * cos(angle);
					GLfloat z = planets[index].offset * sin(angle);
					glVertex3f(x, 0.0, z);
				}
				glEnd();
			}
		}
		glPopMatrix();
	}

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

	//glPointSize(10.0f);
	//glColor3f(1.0, 0.0, 0.0);
	//glBegin(GL_POINTS);
	//glVertex3f(enterprisePoints[1989/2][0], enterprisePoints[1989/2][1], enterprisePoints[1989/2][2]);
	//glEnd();
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

void drawCorona(void) {
	glPushMatrix();
	glTranslatef(0, 0, 0.1);
	for (int i = 0; i < CORONA_LINE_COUNT; i++) {
		glLineWidth(5.0f);
		glBegin(GL_LINES);
		glColor4f(1.0, 1.0, 0.0, 1.0);
		glVertex2f(0.0, 0.0);

		glColor4f(1.0, 0.5, 0.0, 0.0);
		glVertex2f(coronaLength * cos(coronaPositions[i]), coronaLength * sin(coronaPositions[i]));
		glEnd();
	}
	glLineWidth(1.0f);
	glPopMatrix();
}

void randomizeCorona(void) {
	for (int i = 0; i < CORONA_LINE_COUNT; i++) {
		coronaPositions[i] = (GLfloat)rand() / RAND_MAX * 2.0 * M_PI;
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
	else if (key == 'c') {
		if (coronaToggled == 0) {
			coronaToggled = 1;
		}
		else {
			coronaToggled = 0;
		}
	}
	else if (key == 'q') {
		camPosOffset[0] -= 0.01;
	}
	else if (key == 'w') {
		camPosOffset[1] -= 0.01;
	}
	else if (key == 'e') {
		camPosOffset[2] -= 0.01;
	}
	else if (key == 'z') {
		camPosOffset[0] += 0.01;
	}
	else if (key == 'x') {
		camPosOffset[1] += 0.01;
	}
	else if (key == 'v') {
		camPosOffset[2] += 0.01;
	}


	glutPostRedisplay();
}

void myMouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		if (leftLaserTimer == 0) {
			leftLaser = 1;
			leftLaserTimer = laserTimer;

			laserSpawnPos[0] = enterprisePoints[1989 / 2][0];
			laserSpawnPos[1] = enterprisePoints[1989 / 2][1];
			laserSpawnPos[2] = enterprisePoints[1989 / 2][2];

			leftLaserPos[0] = laserSpawnPos[0] - 0.1f;
			leftLaserPos[1] = laserSpawnPos[1];
			leftLaserPos[2] = laserSpawnPos[2] - 0.1f;
		}

	}
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {

		if (rightLaserTimer == 0) {
			rightLaser = 1;
			rightLaserTimer = laserTimer;


			laserSpawnPos[0] = enterprisePoints[1989 / 2][0];
			laserSpawnPos[1] = enterprisePoints[1989 / 2][1];
			laserSpawnPos[2] = enterprisePoints[1989 / 2][2];

			rightLaserPos[0] = laserSpawnPos[0] + 0.1f;
			rightLaserPos[1] = laserSpawnPos[1];
			rightLaserPos[2] = laserSpawnPos[2] - 0.1f;
		}
	}
	glutPostRedisplay();
}

void mySpecialKeyboard(int key, int x, int y) {
	switch (key) {
		case GLUT_KEY_UP:
			moveEnterprise(0, 1, 0);
			break;
		case GLUT_KEY_DOWN:
			moveEnterprise(0, -1, 0);
			break;
		case GLUT_KEY_RIGHT:
			moveEnterprise(1, 0, 0);
			break;
		case GLUT_KEY_LEFT:
			moveEnterprise(-1, 0, 0);
			break;
		case GLUT_KEY_PAGE_UP:
			moveEnterprise(0, 0, -1);
			break;
		case GLUT_KEY_PAGE_DOWN:
			moveEnterprise(0, 0, 1);
			break;
	}

	glutPostRedisplay();
}

void moveEnterprise(int x, int y, int z) {

	GLfloat rotationFactor = 0.5f;

	for (int i = 0; i < ENTERPRISE_POINT_COUNT; i++) {
		enterprisePoints[i][0] += x * enterpriseSpeed;
		enterprisePoints[i][1] += y * enterpriseSpeed;
		enterprisePoints[i][2] += z * enterpriseSpeed;

	}
	camPos[0] += x * enterpriseSpeed * enterpriseScale;
	camPos[1] += y * enterpriseSpeed * enterpriseScale;
	camPos[2] += z * enterpriseSpeed * enterpriseScale;

	printf("%f, %f, %f\n", enterprisePoints[1989 / 2][0], enterprisePoints[1989 / 2][1], enterprisePoints[1989 / 2][2]);
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

	//for (int i = 0; i < ENTERPRISE_POINT_COUNT; i++) {
	//	enterprisePoints[i][1] += 20;
	//	enterprisePoints[i][2] += 100;
	//}

	fclose(file);

}

void drawEnterprise(void) {
	glPushMatrix();
	glTranslatef(0, 0, 7.5);
	//glRotatef(15*, 0.0, 1.0, 0.0);
	glScalef(enterpriseScale, enterpriseScale, enterpriseScale);
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
	glPopMatrix();


}

void initializeGL(void)
{
	// enable depth testing
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);

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
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	// set window size
	glutInitWindowSize(windowWidth, windowHeight);
	// set window position on screen
	glutInitWindowPosition(100, 150);
	// open the screen window
	glutCreateWindow("The Hitch Hikers Guide to the Planets");


	initializeStars();
	initializeEnterprise();
	initializePlanets();

	// register redraw function
	glutDisplayFunc(myDisplay);
	// register the idle function
	glutIdleFunc(myIdle);

	glutKeyboardFunc(myKeyboard);

	glutMouseFunc(myMouse);

	glutSpecialFunc(mySpecialKeyboard);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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
	printf("LClick \t: shoot left lasers\n");
	printf("RClick \t: shoot right lasers\n\n");

	printf("Camera Controls\n------------------- \n\n");
	printf("Up \tArrow\t: \tmove up\n");
	printf("Down \tArrow\t: \tmove down\n");
	printf("Right \tArrow\t: \tmove right\n");
	printf("Left \tArrow\t: \tmove left\n");
	printf("PAGE \tUP\t: \tforward\n");
	printf("PAGE \tDOWN\t: \tbackward\n");


	printf("\nNote: May need to use FN key to use Page Up and Page Down on Laptops.\n");
}
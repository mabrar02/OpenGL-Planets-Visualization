/************************************************************************************

	File: 			planets.c

	Description:	A complete OpenGL program simulating a fictitious planetary system,
					including interesting orbits, moons, a moveable enterprise, and 
					togglable scene elements. CSCI 3161 - Assignment 2



	Author:			Mahdeen Abrar (B00860738)
	Date:			Oct. 31, 2023

*************************************************************************************/


/* include the library header files */
#include <stdlib.h>
#include <stdio.h>
#include <freeglut.h>
#include <time.h>
#include <math.h>
#include <string.h>

/* defining constants, 11 planets includes the sun, 6 planets, and 4 moons (one extra later for eliptical orbit) */
#define STAR_COUNT 500
#define CORONA_LINE_COUNT 160
#define BOOSTER_LINE_COUNT 20
#define ENTERPRISE_POINT_COUNT 1201
#define ENTERPRISE_TRIANGLE_COUNT 1989
#define PLANET_COUNT 11
#define M_PI 3.141592

/* function signature defintions */
void drawStars(void);
void twinkleStars(void);
void printKeyboardControls(void);
void drawEnterprise(void);
void drawPlanets(void);
void drawOrbits(void);
void moveEnterprise(void);
void positionCamera(void);
void initializePlanets(void);
void drawCorona(void);
void randomizeCorona(void);
void drawLaser(void);
void drawBoosters(void);
void randomizeBoosters(void);
void drawElipticalPlanet(void);
void drawElipticalOrbit(void);

/* struct to define a planet for easy iteration*/
typedef struct {
	GLint isMoon;
	GLfloat size;
	GLfloat offset;
	GLfloat speed;
	GLfloat rotations[3];
	GLfloat colors[3];
	GLint moonIndex;
} Planet;

/* array used to hold all the planet structs */
Planet planets[PLANET_COUNT];

/* series of arrays to corespond to planet[i]'s properties. moonIndicies tells you if the planet has a moon, if so
which planet in the planet array is the moon, otherwise -1 */

GLint moonSetting[] = { 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1};
GLfloat planetSizes[] = { 0.2, 0.5, 0.6, 0.3, 0.6, 0.5, 0.25, 0.8, 0.4, 0.7, 0.7 };
GLfloat planetOffset[] = { 0.0, 4.0, 2.0, 20.0, 10.0, 15.0, 2.0, 18.0, 2.0, 25.0, 2.0 };
GLfloat planetSpeeds[] = { 0.0, 10.0, 20.0, 5.0, 15.0, 8.0, 5.0, 5.0, 13.0, 3.0, 20.0 };
GLfloat planetColors[][3] = { {1.0, 1.0, 0.0}, {0.0, 1.0, 1.0}, {0.5, 0.5, 0.5}, {1.0, 0.0, 1.0}, {0.75, 0.1, 0.25}, {0.05, 0.8, 0.1}, {0.75, 0.75, 0.75}, {0.0, 0.25, 0.8}, {0.9, 0.9, 0.9}, {0.5, 0.0, 0.5}, {0.25, 0.25, 0.25} };
GLfloat planetRotations[][3] = { {0.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 1.0, 0.0} };
GLint moonIndices[] = { -1, 2, -1, -1, -1, 6, -1, 8, -1, 10, -1 };

// used later to give special properties to 8th planet in the array
GLint tiltedPlanetIndex = 7;

// window variables
GLint windowWidth = 900; 
GLint windowHeight = 600;
GLfloat xRange = 2.0f;
GLfloat yRange = 2.0f;
GLfloat zRange = 10.0f;

// camera variables
GLfloat camPosOffset[3] = {0.0, 0.31, 0.72};
GLfloat camPos[3] = { 0.0, 1, 10.0 };

// orbit variables
GLfloat theta = 0.0f;

// toggle variables
GLint starsToggled = 0;
GLint orbitsToggled = 0;
GLint coronaToggled = 0;
GLint boostersToggled = 0;

// star variables
GLfloat starPositions[STAR_COUNT][3];
GLfloat starColors[STAR_COUNT][3];

// corona variables
GLfloat coronaPositions[CORONA_LINE_COUNT];
GLfloat coronaLength = 0.5;

// enterprise variables
GLfloat enterprisePoints[ENTERPRISE_POINT_COUNT][3];
GLint enterpriseTriangles[ENTERPRISE_TRIANGLE_COUNT][3];
GLfloat enterpriseSpeed = 0.1;
GLfloat enterpriseScale = 0.55;

// booster variables
GLfloat moveDir[] = { 0, 0, 0 };
GLfloat boosterPositions[BOOSTER_LINE_COUNT];
GLfloat boosterLength = 0.15;

// laser variables
GLint leftLaser = 0;
GLint rightLaser = 0;
GLfloat laserSpawnPos[3];
GLfloat leftLaserTimer = 0;
GLfloat rightLaserTimer = 0;
GLfloat laserTimer = 2;
GLfloat laserSpeed = 0.25f;
GLfloat leftLaserPos[] = { 0, 0, 0 };
GLfloat rightLaserPos[] = { 0, 0, 0 };


/************************************************************************

	Function:		myDisplay

	Description:	Display callback, clears frame buffer and depth buffer,
					positions the camera and draws the various scene elements 
					depending if they are toggled on or off

*************************************************************************/
void myDisplay(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// load identity matrix to clear any transformations
	glLoadIdentity();

	// position the camera
	positionCamera();

	// depending if stars are toggled, draw them
	if (starsToggled == 1) {
		drawStars();
	}

	// draw the enterprise
	drawEnterprise();

	// depending if boosters are toggled, draw them
	if (boostersToggled == 1) {
		drawBoosters();
	}

	// depending if orbits are toggled, draw them
	if (orbitsToggled == 1) {
		drawOrbits();
	}

	// draw the planets
	drawPlanets();

	// draw the lasers, which will only be active when fired
	drawLaser();

	// depending if corona are toggled, draw them
	if (coronaToggled == 1) {
		drawCorona();
	}

	// use swap buffers instead of flush() b/c of double buffering
	glutSwapBuffers();
}


/************************************************************************

	Function:		drawLaser

	Description:	Aligns the laser point with the enterprise, then draws 
					the left and/or right laser if currently active

*************************************************************************/
void drawLaser(void) {
	
	// push matrix to not affect the current transformation matrix
	glPushMatrix();
	
	// move lasers to be right under the enterprise
	glTranslatef(0, 0, 7);

	// scale lasers with enterprise 
	glScalef(enterpriseScale, enterpriseScale, enterpriseScale);

	// if a laser is active, we should draw it, otherwise don't show it on the screen
	if (leftLaser == 1) {
		
		// give laser a bigger width
		glLineWidth(5.0f);
		
		// create a laser which starts as red and fades to blue and lowers opacity, starts from left position and shoots outwards in the z direction
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
		
		// same as left, but spawning from the right laser spawn location
		glVertex3fv(rightLaserPos);
		glColor4f(0.0, 0.0, 1.0, 0.25);
		glVertex3f(rightLaserPos[0], rightLaserPos[1], rightLaserPos[2] - 0.5);
		glEnd();
	}

	// pop to return to original transformation matrix
	glPopMatrix();

	// reset line width
	glLineWidth(1.0f);
}


/************************************************************************

	Function:		drawBoosters

	Description:	Aligns the laser boosters with the enterprise, then 
					draws them pointing in a particular location based on
					enterprise's movement

*************************************************************************/
void drawBoosters(void) {
	
	// push matrix to not affect the current transformation matrix
	glPushMatrix();

	// move boosters to be at the tips of the enterprise
	glTranslatef(0, 0, 8.05);

	// scale boosters with the enterprise 
	glScalef(enterpriseScale, enterpriseScale, enterpriseScale);

	// given all the random angles for the booster, we want to draw various lines pointing in those directions starting from the booster location
	for (int i = 0; i < BOOSTER_LINE_COUNT; i++) {

		// no boosters shown for moving backwards
		if (moveDir[2] == 1) break; 

		// give boosters more width
		glLineWidth(2.0f);

		// if no directional input is present, the boosters won't be either. initialize booster locations on top of the booster spawns
		GLfloat boosterX = enterprisePoints[0][0];
		GLfloat boosterY = enterprisePoints[0][1];
		GLfloat boosterZ = enterprisePoints[0][2];
		GLfloat boosterX2 = enterprisePoints[1200][0];;
		GLfloat boosterY2 = enterprisePoints[1200][1];;
		GLfloat boosterZ2 = enterprisePoints[1200][2];

		// if moving in x direction, the boosters should point left or right accordingly
		if (moveDir[0] != 0) {

			// when moving on the x axis, boosters should only be concerned with x y plane, similar to corona 
			// use PI/2 to push the upwards facing cone to the left or right respectively
			boosterX = enterprisePoints[0][0] + boosterLength * cos(boosterPositions[i] + M_PI/2 * moveDir[0]);
			boosterY = enterprisePoints[0][1] + boosterLength * sin(boosterPositions[i] + M_PI/2 * moveDir[0]);

			boosterX2 = enterprisePoints[1200][0] + boosterLength * cos(boosterPositions[i] + M_PI / 2 * moveDir[0]);
			boosterY2 = enterprisePoints[1200][1] + boosterLength * sin(boosterPositions[i] + M_PI / 2 * moveDir[0]);
		}

		// if moving in y direction, boosters should point up or down
		else if (moveDir[1] != 0) {

			// again, only concerned with x y plane. flip the booster cone up or down respectively using the movement direction
			boosterX = enterprisePoints[0][0] + boosterLength * cos(boosterPositions[i]) * -moveDir[1];
			boosterY = enterprisePoints[0][1] + boosterLength * sin(boosterPositions[i]) * -moveDir[1];

			boosterX2 = enterprisePoints[1200][0] + boosterLength * cos(boosterPositions[i]) *  -moveDir[1];
			boosterY2 = enterprisePoints[1200][1] + boosterLength * sin(boosterPositions[i]) * -moveDir[1];
		}

		// if moving in the -z direction, boosters should point outwards towards the camera
		else if (moveDir[2] == -1) {
			
			// boosters should lie on the x and z axis this time, and are given a 1.5 scale for increased booster effect

			boosterX = enterprisePoints[0][0] + boosterLength * cos(boosterPositions[i]) * 1.5;
			boosterZ = enterprisePoints[0][2] + boosterLength * sin(boosterPositions[i]) * 1.5;

			boosterX2 = enterprisePoints[1200][0] + boosterLength * cos(boosterPositions[i]) * 1.5;
			boosterZ2 = enterprisePoints[1200][2] + boosterLength * sin(boosterPositions[i]) * 1.5;
		}

		// draw two sets of boosters, one right one left, starting from orange to more red and fading out slightly
		glBegin(GL_LINES);
		glColor4f(1.0, 0.75, 0.0, 1.0);
		glVertex3fv(enterprisePoints[0]);
		glColor4f(1.0, 0.25, 0.0, 0.25);
		glVertex3f(boosterX, boosterY, boosterZ);

		glColor4f(1.0, 0.75, 0.0, 1.0);
		glVertex3fv(enterprisePoints[1200]);
		glColor4f(1.0, 0.25, 0.0, 0.25);
		glVertex3f(boosterX2, boosterY2, boosterZ2);
		glEnd();
	}


	// pop to return to original transformation matrix
	glPopMatrix();
}


/************************************************************************

	Function:		randomizeBoosters

	Description:	Randomizes the direcitonal angles for the boosters to
					give a sporadic effect when drawn

*************************************************************************/
void randomizeBoosters(void) {
	for (int i = 0; i < BOOSTER_LINE_COUNT; i++) {
		
		// pick a random direction between PI/4 and 3PI/4, resulting in an upwards cone shape which we can orient to give booster effect
		boosterPositions[i] = ((GLfloat)rand() / RAND_MAX) * (M_PI / 2.0) + (M_PI / 4.0);
	}
}


/************************************************************************

	Function:		myIdle

	Description:	idle function used for nonevents for GLUT, handles
					twinkling of stars, randomization of corona and boosters,
					and motion of lasers.

*************************************************************************/
void myIdle(void) {
	
	// change the colors of the stars
	twinkleStars();

	// change the angle directions of the corona lines
	randomizeCorona();

	// change the angle directions of the booster lines
	randomizeBoosters();

	// increase theta to rotate planets
	theta += 0.1f;

	// if a laser's duration is still valid, decrement the duration and increment it's distance away from the camera to effectively "fire" the laser
	if (leftLaserTimer > 0) {
		leftLaserTimer -= 0.1f;
		leftLaserPos[2] -= laserSpeed;
		if (leftLaserTimer <= 0) {
			leftLaserTimer = 0;
			leftLaser = 0;
		}
	}

	// because each laser is independent of eachother, need it's own set of variables each
	if (rightLaserTimer > 0) {
		rightLaserTimer -= 0.1f;
		rightLaserPos[2] -= laserSpeed;
		if (rightLaserTimer <= 0) {
			rightLaserTimer = 0;
			rightLaser = 0;
		}
	}

	// force OpenGL to redraw the changes
	glutPostRedisplay();
}


/************************************************************************

	Function:		positionCamera

	Description:	function used to position the camera to follow the 
					enterprise

*************************************************************************/
void positionCamera(void) {
	
	// grab all the enterprise position values to make sure camera always centered on it
	GLfloat enterpriseX = enterprisePoints[ENTERPRISE_POINT_COUNT/2][0] * enterpriseScale;
	GLfloat enterpriseY = enterprisePoints[ENTERPRISE_POINT_COUNT / 2][1] * enterpriseScale;
	GLfloat enterpriseZ = enterprisePoints[ENTERPRISE_POINT_COUNT / 2][2] * enterpriseScale;

	// use separate vector for camera position so the translation of the enterprise doesn't mess up tracking
	gluLookAt(camPos[0], camPos[1], camPos[2], enterpriseX, enterpriseY, enterpriseZ, 0.0, 1.0, 0.0);
}


/************************************************************************

	Function:		drawPlanets

	Description:	function used in myDisplay() to draw all of the planets
					in the planet array

*************************************************************************/
void drawPlanets() {
	// push matrix to not affect the current transformation matrix
	glPushMatrix();
	
	// initially create the sun planet, who's scale factor will affect the surrounding planets
	GLUquadric* quad = gluNewQuadric();
	glScalef(planets[0].size, planets[0].size, planets[0].size);
	glColor3fv(planets[0].colors);
	gluSphere(quad, 1.0, 32, 32);

	/* for every planet in the array, we check if it's a moon. If not, draw it then draw its moon if it has one,
	   ensuring the moon inherits the parent planet's transformations. If it is a moon, skip over it. */
	for (int i = 1; i < PLANET_COUNT; i++) {
		
		// using a second push to further scope each planet's transformation, allows for moons to inherit transformations
		glPushMatrix();

		// don't draw right away if the planet is a moon
		if (planets[i].isMoon == 0) {

			// if our planet is the special tilted one, we rotate the planets entire path AFTER the rest of the transformations are completed
			if (i == tiltedPlanetIndex) {
				glRotatef(-25, 0, 0, 1);
			}

			// give our planet a color, scale it down, translate it away from the sun, then rotate by theta which is always changing, giving the planet an orbit
			glRotatef(theta * planets[i].speed, planets[i].rotations[0], planets[i].rotations[1], planets[i].rotations[2]);
			glTranslatef(planets[i].offset, 0.0, 0.0);
			glScalef(planets[i].size, planets[i].size, planets[i].size);
			glColor3fv(planets[i].colors);
			gluSphere(quad, 1.0, 32, 32);
			
			// if the planet has a moon, we create a second planet without reseting our transformation matrix
			if (planets[i].moonIndex != -1) {
				GLint index = planets[i].moonIndex;

				// do the same as above, give the moon a color, scale it, offset it, then rotate 
				glRotatef(theta * planets[index].speed, planets[index].rotations[0], planets[index].rotations[1], planets[index].rotations[2]);
				glTranslatef(planets[index].offset, 0.0, 0.0);
				glScalef(planets[index].size, planets[index].size, planets[index].size);
				glColor3fv(planets[index].colors);
				gluSphere(quad, 1.0, 32, 32);

			}
		}

		// second pop matrix to return to sun's transformation matrix
		glPopMatrix();
	}

	drawElipticalPlanet();


	// pop matrix to return to original matrix
	glPopMatrix();
}


/************************************************************************

	Function:		drawElipticalPlanet

	Description:	Function used to draw the singular eliptical planet. Calculations
					referenced from https://math.etsu.edu/multicalc/prealpha/chap3/chap3-2/part4.htm

*************************************************************************/
void drawElipticalPlanet(void) {

	// arbitrary paramaeter and eccentricity variables chosen for eliptical orbit
	GLfloat p = 1;
	GLfloat e = 0.9;

	// find the radius and get the x and z components by using cos and sin, where the constant multiplied represents the speed of the orbit
	GLfloat r = p / (1 - e * cos(theta * 0.2));
	GLfloat x = r * cos(theta * -0.2);
	GLfloat z = r * sin(theta * -0.2);

	// create a sphere which is translated to the corresponding x and z to simulate orbitting around theta
	GLUquadric* quad = gluNewQuadric();
	glTranslatef(x-2, 0.0, z);
	glScalef(0.4, 0.4, 0.4);
	glColor3f(0.2, 0.2, 0.5);
	gluSphere(quad, 1.0, 32, 32);
}


/************************************************************************

	Function:		drawElipticalOrbit

	Description:	Function used to draw the singular eliptical orbit. Calculations
					referenced from https://math.etsu.edu/multicalc/prealpha/chap3/chap3-2/part4.htm

*************************************************************************/
void drawElipticalOrbit(void) {

	// same parameter and eccentricity variable as elipitical planet
	GLfloat p = 1;
	GLfloat e = 0.9;


	glColor3f(1.0, 1.0, 1.0);

	// draw many little lines at the points on the elipitical to simulate drawing the entire elipsis
	glBegin(GL_LINE_LOOP);
	for (int j = 0; j < 360; j += 5) {
		GLfloat angle = j * M_PI / 180.0;
		GLfloat r = p / (1 - e * cos(angle));
		GLfloat x = r * cos(angle) - 2;
		GLfloat z = r * sin(angle);
		glVertex3f(x, 0.0, z);
	}
	glEnd();
}


/************************************************************************

	Function:		initializePlanets

	Description:	Function used to instantiate all the planets in our 
					planet array with the values from all of the property
					arrays

*************************************************************************/
void initializePlanets() {

	for (int i = 0; i < PLANET_COUNT; i++) {

		// determine if the planet is a moon, what scale to use, what radius from the sun, it's speed, rotation, color, and if it has a moon
		planets[i].isMoon = moonSetting[i];
		planets[i].size = planetSizes[i];
		planets[i].offset = planetOffset[i];
		planets[i].speed = planetSpeeds[i];
		memcpy(planets[i].rotations, planetRotations[i], 3 * sizeof(GLfloat));
		memcpy(planets[i].colors, planetColors[i], 3 * sizeof(GLfloat));
		planets[i].moonIndex = moonIndices[i];

	}
}


/************************************************************************

	Function:		drawOrbits

	Description:	function used in myDisplay() to draw all of the orbits
					from the planet array, when toggled on

*************************************************************************/
void drawOrbits() {
	
	// push matrix to not affect the current transformation matrix
	glPushMatrix();

	// color all orbits white
	glColor3f(1.0f, 1.0f, 1.0f);
	
	// all planets are affected by sun scale, so the orbits should be as well
	glScalef(planets[0].size, planets[0].size, planets[0].size);

	// for every planet in the planet array, we will draw a circle whose radius equals the planet's offset from the sun, using a line loop
	for (int i = 0; i < PLANET_COUNT; i++) {

		// use push matrix to further scope our planets so they don't affect eachother but allow for moon orbits to follow
		glPushMatrix();
		if (planets[i].isMoon == 0) {

			// if the planet is our special tilted one, we want to tilt the orbit AFTER all the other transformations are done
			if (i == tiltedPlanetIndex) {
				glRotatef(-25, 0, 0, 1);
			}

			// draw many little lines which lie on a circles circumference to simulate drawing an orbit circle
			glBegin(GL_LINE_LOOP);
			for (int j = 0; j < 360; j += 5) {
				GLfloat angle = j * M_PI / 180.0;
				GLfloat x = planets[i].offset * cos(angle);
				GLfloat z = planets[i].offset * sin(angle);
				glVertex3f(x, 0.0, z);
			}
			glEnd();

			// if we have a moon, we draw their orbit but inherit the parent planet's transformation to move the orbit with the planet
			if (planets[i].moonIndex != -1) {

				// move orbit with parents transformations
				GLint index = planets[i].moonIndex;
				glRotatef(theta * planets[i].speed, planets[i].rotations[0], planets[i].rotations[1], planets[i].rotations[2]);
				glTranslatef(planets[i].offset, 0.0, 0.0);
				glScalef(planets[i].size, planets[i].size, planets[i].size);

				// same as above, draw many little lines around moons orbit circumference to simulate a circle
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
		
		// pop matrix to return to the sun's orbit transformation matrix
		glPopMatrix();
	}

	drawElipticalOrbit();

	// pop to return to original transformation matrix
	glPopMatrix();
}


/************************************************************************

	Function:		drawStars

	Description:	Function used to draw all the stars in the star array
					when toggled on

*************************************************************************/
void drawStars(void) {

	// increase size of star
	glPointSize(2.0f);

	// for every star, draw it at the location it was randomly generated at the start, then show it's current color (which is always changing)
	for (int i = 0; i < STAR_COUNT; i++) {
		glBegin(GL_POINTS);
		glColor3fv(starColors[i]);
		glVertex3fv(starPositions[i]);
		glEnd();
	}
	
	// reset the point size
	glPointSize(1.0f);
}


/************************************************************************

	Function:		twinkleStars

	Description:	Function used to repeatedly change the colors of the 
					stars to give a twinkling effect

*************************************************************************/
void twinkleStars(void) {

	// randomize the color of every star and store in an array to be read when drawing
	for (int i = 0; i < STAR_COUNT; i++) {
		GLfloat randColR = (GLfloat)rand() / RAND_MAX;
		GLfloat randColB = (GLfloat)rand() / RAND_MAX;
		GLfloat randColG = (GLfloat)rand() / RAND_MAX;
		starColors[i][0] = randColR;
		starColors[i][1] = randColB;
		starColors[i][2] = randColG;
	}
}


/************************************************************************

	Function:		drawCorona

	Description:	Function used to draw the corona around the sun when
					toggled on

*************************************************************************/
void drawCorona(void) {

	// push matrix to not affect the current transformation matrix
	glPushMatrix();
	
	// push slightly outward to extenuate the corona effect
	glTranslatef(0, 0, 0.1);

	// for every random angle, create a line starting from the center of the sun and pointing outwards towards that angle
	for (int i = 0; i < CORONA_LINE_COUNT; i++) {
		// give corona more width
		glLineWidth(5.0f);

		// start as yellow and go to a more orange color which fades out
		glBegin(GL_LINES);
		glColor4f(1.0, 1.0, 0.0, 1.0);
		glVertex2f(0.0, 0.0);

		glColor4f(1.0, 0.5, 0.0, 0.0);
		glVertex2f(coronaLength * cos(coronaPositions[i]), coronaLength * sin(coronaPositions[i]));
		glEnd();
	}

	// reset line width
	glLineWidth(1.0f);

	// pop matrix to return to original transformation matrix
	glPopMatrix();
}


/************************************************************************

	Function:		randomizeCorona

	Description:	function used to randomize the angle directions of the 
					suns corona to give it a sporadic effect

*************************************************************************/
void randomizeCorona(void) {
	// for every line, give a random radian angle to point the line towards when drawing
	for (int i = 0; i < CORONA_LINE_COUNT; i++) {
		coronaPositions[i] = (GLfloat)rand() / RAND_MAX * 2.0 * M_PI;
	}
}


/************************************************************************

	Function:		myKeyboard

	Description:	Handles the functionality of normal key keyboard
					presses by the user

*************************************************************************/
void myKeyboard(unsigned char key, int x, int y) {

	// if s should toggle stars, r should toggle the orbits, c should toggle the corona
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


	// force OpenGL to redraw the change
	glutPostRedisplay();
}


/************************************************************************

	Function:		myMouse

	Description:	Handles the functionality of the left and right click
					from the mouse

*************************************************************************/
void myMouse(int button, int state, int x, int y) {

	// if we press the left mouse button we want to see the left laser be fired
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {

		// only fire a laser if there is not one curently being shot
		if (leftLaserTimer == 0) {

			// set the left laser active
			leftLaser = 1;
			leftLaserTimer = laserTimer;

			// spawn the laser slightly left from the center of the enterprise
			laserSpawnPos[0] = enterprisePoints[1989 / 2][0];
			laserSpawnPos[1] = enterprisePoints[1989 / 2][1];
			laserSpawnPos[2] = enterprisePoints[1989 / 2][2];

			leftLaserPos[0] = laserSpawnPos[0] - 0.1f;
			leftLaserPos[1] = laserSpawnPos[1];
			leftLaserPos[2] = laserSpawnPos[2] - 0.1f;
		}

	}

	// if we press the right mouse button we want to see the right laser be fired
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {

		// only fire a laser if there is not one currently being shot
		if (rightLaserTimer == 0) {

			// set the right laser active
			rightLaser = 1;
			rightLaserTimer = laserTimer;

			// spawn the laser slightly right from the center of the enterprise
			laserSpawnPos[0] = enterprisePoints[1989 / 2][0];
			laserSpawnPos[1] = enterprisePoints[1989 / 2][1];
			laserSpawnPos[2] = enterprisePoints[1989 / 2][2];

			rightLaserPos[0] = laserSpawnPos[0] + 0.1f;
			rightLaserPos[1] = laserSpawnPos[1];
			rightLaserPos[2] = laserSpawnPos[2] - 0.1f;
		}
	}

	// force OpenGL to redraw the change
	glutPostRedisplay();
}


/************************************************************************

	Function:		mySpecialKeyboard

	Description:	Handles the functionality of special key keyboard
					presses by the user

*************************************************************************/
void mySpecialKeyboard(int key, int x, int y) {

	// depending on the directional key pressed, we will move the enterprise in that direction
	// right +x, left -x, up +y, down -y, forward -z, backward +z
	switch (key) {
		case GLUT_KEY_UP:
			moveEnterprise(0, 1, 0);
			boostersToggled = 1;
			break;
		case GLUT_KEY_DOWN:
			moveEnterprise(0, -1, 0);
			boostersToggled = 1;
			break;
		case GLUT_KEY_RIGHT:
			moveEnterprise(1, 0, 0);
			boostersToggled = 1;
			break;
		case GLUT_KEY_LEFT:
			moveEnterprise(-1, 0, 0);
			boostersToggled = 1;
			break;
		case GLUT_KEY_PAGE_UP:
			moveEnterprise(0, 0, -1);
			boostersToggled = 1;
			break;
		case GLUT_KEY_PAGE_DOWN:
			moveEnterprise(0, 0, 1);
			boostersToggled = 1;
			break;
	}

	// force OpenGL to redraw the change
	glutPostRedisplay();
}


/************************************************************************

	Function:		mySpecialKeyboardUp

	Description:	Handles the functionality of special key keyboard
					releases by the user

*************************************************************************/
void mySpecialKeyboardUp(int key, int x, int y) {

	// when the user stops moving, the boosters should be reset along with the directional vector to represent staying still
	if (key == GLUT_KEY_UP || key == GLUT_KEY_DOWN || key == GLUT_KEY_RIGHT || key == GLUT_KEY_LEFT || key == GLUT_KEY_PAGE_UP || key == GLUT_KEY_PAGE_DOWN) {
		boostersToggled = 0;
		moveDir[0] = 0;
		moveDir[1] = 0;
		moveDir[2] = 0;
	}

	// force OpenGL to redraw the change
	glutPostRedisplay();
}


/************************************************************************

	Function:		moveEnterprise

	Description:	Function called by the keyboard movement buttons to
					move the enterprise in the direction pressed

*************************************************************************/

void moveEnterprise(int x, int y, int z) {

	// alter each point by directional vector * speed to get the new location of the point
	for (int i = 0; i < ENTERPRISE_POINT_COUNT; i++) {
		enterprisePoints[i][0] += x * enterpriseSpeed;
		enterprisePoints[i][1] += y * enterpriseSpeed;
		enterprisePoints[i][2] += z * enterpriseSpeed;

	}

	// move camera along with enterprise, scaled to enterprise to track accordingly
	camPos[0] += x * enterpriseSpeed * enterpriseScale;
	camPos[1] += y * enterpriseSpeed * enterpriseScale;
	camPos[2] += z * enterpriseSpeed * enterpriseScale;

	// set the directional vector to be used by the boosters
	moveDir[0] = x;
	moveDir[1] = y;
	moveDir[2] = z;
}


/************************************************************************

	Function:		initializeStars

	Description:	Function used to randomly get initial positions and colors for the 
					stars to stay.

*************************************************************************/

void initializeStars(void) {
	for (int i = 0; i < STAR_COUNT; i++) {
		
		// getting random numbers between each of the ranges specified to spawn the stars in
		GLfloat randPosX = 10 * xRange * (GLfloat) rand() / RAND_MAX - 5 * xRange;
		GLfloat randPosY = 10 * yRange * (GLfloat) rand() / RAND_MAX - 5 * yRange;
		GLfloat randPosZ = 2 * zRange * (GLfloat)rand() / RAND_MAX - zRange;
		starPositions[i][0] = randPosX;
		starPositions[i][1] = randPosY;
		starPositions[i][2] = randPosZ;
		
		// set random initial color
		GLfloat randColR = (GLfloat)rand() / RAND_MAX;
		GLfloat randColB = (GLfloat)rand() / RAND_MAX;
		GLfloat randColG = (GLfloat)rand() / RAND_MAX;
		starColors[i][0] = randColR;
		starColors[i][1] = randColB;
		starColors[i][2] = randColG;
	}
}


/************************************************************************

	Function:		initializeEnterprise

	Description:	Function reads the enterprise.txt file and saves the 
					point vectors and face vectors

*************************************************************************/

void initializeEnterprise(void) {

	// attemp to open the enterprise.txt file, returning an error if it's not found
	FILE* file;
	if (fopen_s(&file, "enterprise.txt", "r") != 0) {
		printf("Error opening file\n");
		return;
	}

	// use counters to read the file in one go, hardcoded value as we know the end of the file
	char line[128];
	int pointIndex = 0;
	int triangleIndex = 0;
	
	for (int i = 0; i < 3190; i++) {
		fgets(line, sizeof(line), file);
		// a v indicates a vertice so we add the trio to the enterprisePoints array which will be read to create faces
		if (line[0] == 'v') {
			sscanf_s(line, "v %f %f %f", &enterprisePoints[pointIndex][0], &enterprisePoints[pointIndex][1], &enterprisePoints[pointIndex][2]);
			pointIndex++;
		}
		// an f indicates a face so we add the trio to the enterpriseTriangles array which will be used to index from the enterprisePoints and create triangles
		else if (line[0] == 'f') {
			sscanf_s(line, "f %d %d %d", &enterpriseTriangles[triangleIndex][0], &enterpriseTriangles[triangleIndex][1], &enterpriseTriangles[triangleIndex][2]);
			triangleIndex++;
		}
	}

	// close the file once you're done reading
	fclose(file);

}


/************************************************************************

	Function:		drawEnterprise

	Description:	Draws the enterprise, colors it,
					scales it, and moves it infront of the camera

*************************************************************************/

void drawEnterprise(void) {
	
	// push matrix to not affect the current transformation matrix
	glPushMatrix();
	
	// first scale then move in front of the camera
	glTranslatef(0, 0, 7.5);
	glScalef(enterpriseScale, enterpriseScale, enterpriseScale);

	// repeatedly increase a value for colour to give the enterprise a more dynamic colour scheme
	GLint colorVal = 1989;
	for (int i = 0; i < ENTERPRISE_TRIANGLE_COUNT; i++) {

		// for every face identified in the triangle array, we create a new triangle from the enterprisePoints array
		glBegin(GL_TRIANGLES);
		glColor3f(colorVal/3978.0f, colorVal/3978.0f, colorVal/3978.0f);
		glVertex3fv(enterprisePoints[enterpriseTriangles[i][0] - 1]);
		glVertex3fv(enterprisePoints[enterpriseTriangles[i][1] - 1]);
		glVertex3fv(enterprisePoints[enterpriseTriangles[i][2] - 1]);
		glEnd();
		colorVal++;
	}

	// pop matrix to return to original transformation matrix
	glPopMatrix();
}


/************************************************************************

	Function:		initializeGL

	Description:	Initializes the OpenGL rendering context for display

*************************************************************************/
void initializeGL(void)
{
	// enable depth testing
	glEnable(GL_DEPTH_TEST);

	// enable blending for fading opacity
	glEnable(GL_BLEND);

	// set background color to be black
	glClearColor(0, 0, 0, 1.0);

	// change into projection mode so that we can change the camera properties
	glMatrixMode(GL_PROJECTION);

	// load the identity matrix into the projection matrix
	glLoadIdentity();

	// set window mode to perspective
	gluPerspective(45, (float)windowWidth / (float)windowHeight, 0.1, 20);

	// change into model-view mode so that we can change the object positions
	glMatrixMode(GL_MODELVIEW);
}


/************************************************************************

	Function:		main

	Description:	Sets up the openGL rendering context and the windowing
					system, then begins the display loop

*************************************************************************/
void main(int argc, char** argv)
{
	// seed random number generator
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

	// initialize the star positions
	initializeStars();

	// read the enterprise.txt file
	initializeEnterprise();
	
	// set up the planets array with the correct properties for each planet
	initializePlanets();

	// register redraw function
	glutDisplayFunc(myDisplay);

	// register the idle function
	glutIdleFunc(myIdle);
	
	// register the keyboard function
	glutKeyboardFunc(myKeyboard);

	// register the mouse function
	glutMouseFunc(myMouse);

	// register the special keyboard function
	glutSpecialFunc(mySpecialKeyboard);

	// register the special keyboard release function
	glutSpecialUpFunc(mySpecialKeyboardUp);

	// register the blend function to enable opacity fading
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// initialize the rendering context
	initializeGL();

	// print the controls on the console for the user
	printKeyboardControls();

	// go into a perpetual loop
	glutMainLoop();
}


/************************************************************************

	Function:		printKeyboardControls

	Description:	prints the necessary user key controls to the console
					for ease of understanding the program

*************************************************************************/
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
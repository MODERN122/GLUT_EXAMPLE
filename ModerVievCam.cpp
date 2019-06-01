
/* Copyright (c) Mark J. Kilgard, 1994. */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>

#define GLEW_STATIC
#include <vector>
#include <GL/glew.h>
#include <GL/glut.h>
#include <SOIL.h>
#include <GL/freeglut.h>

  /* function declarations */
void
animation_go(void), setMatrix(void), initLight(void), 
Skybox(void),processSpecialKeys(int key, int xx, int yy), draw(), stay(void), resize(int w, int h), menu(int choice), keyboard(unsigned char c, int x, int y);

/* global variables */


float angle = 0.0;
// координаты вектора направления движения камеры
float lx = 0.0f, lz = -1.0f;
// XZ позиция камеры
float x = 0.0f, z = 5.0f;
/* angles for animation */
GLUquadricObj* quadObj; /* used in animation_go */

static float lmodel_oneside[] =
{ GL_FALSE }; 
static float lmodel_twoside[] =
{ GL_TRUE };

GLuint Texture[5];
double width=800, height=600;
int
main(int argc, char** argv)
{
	glutInit(&argc, argv);
	quadObj = gluNewQuadric();  // this will be used in animation_go
								 
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow("Semestrovaia FILIPPOV M VIP-308");
	glutReshapeWindow(width, height);
	Texture[1] = SOIL_load_OGL_cubemap
	(
		"posx.jpg",
		"negx.jpg",
		"posy.jpg",
		"negy.jpg",
		"posz.jpg",
		"negz.jpg",
		SOIL_LOAD_RGB,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS
	);
	Texture[0] = SOIL_load_OGL_texture("kek.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS);
	initLight();
	glutDisplayFunc(animation_go);
	glutReshapeFunc(resize);
	glutCreateMenu(menu);
	glutAddMenuEntry("Start position", 1);
	glutAddMenuEntry("End position", 2);
	glutAddMenuEntry("One-sided lighting", 3);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	glutKeyboardFunc(keyboard); 
	glutSpecialFunc(processSpecialKeys);
	glutMainLoop();
	return 0;             /* ANSI C requires main to return int. */
}
void rand_sphr_for_tree(double x, double y, double z, double radius)
{
	glTranslated(x, y, z);
	gluSphere(quadObj, radius, 20, 8);
}

GLfloat green_color[] = { 0.0, 0.9, 0.0, 0.5 };
GLfloat green_dark_color[] = { 0.0, 0.6, 0.01, 0.5 };
GLfloat brown_color[] = { 0.5, 0.25, 0.01, 1 };
GLfloat white_color[] = { 1.0, 1, 1, 1 };
void tree()
{
	double rad_mn_sphere;
	double radius_spheres;
	double size_tree;
	glPushMatrix();

	size_tree = rand()%11+8.0;
	double ret = (double)rand() / ((double)rand()+ 0.1);
	rad_mn_sphere =size_tree/8.0 + 2 + ret - floor(ret);
	glRotatef(90, -1, 0, 0);
	glColor3ub(139, 69, 19);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, brown_color);
	gluCylinder(quadObj, ret-floor(ret)+1, 1, size_tree, 20,8);
	glTranslated(0,0,size_tree);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, green_color);
	glColor3d(0, 0.9, 0); 
	gluSphere(quadObj, rad_mn_sphere, 20, 8);
	glRotated(90, 0, 0, 1);
	for (double little_sphere = rad_mn_sphere - 0.5, y = -little_sphere; y < little_sphere; y += 1)
	{
		for (double circle = sqrt(pow(little_sphere, 2) - pow(y, 2)), x = -circle; x < circle; x += 1)
		{
			glPushMatrix();
			double ret = (double)rand() / ((double)rand() + 0.1);
			radius_spheres = 1.5 + ret-floor(ret);
			double z = sqrt(pow(circle, 2) - pow(x, 2));
			rand_sphr_for_tree(x, y, z, radius_spheres);
			glPopMatrix();
			glPushMatrix();
			rand_sphr_for_tree(x, y, -z, radius_spheres);
			glPopMatrix();
		}
	}
	glPopMatrix();
}
void christmas_tree()
{
	glPushMatrix();
	int height = 17;
	int width = 0;
	glRotatef(90, -1, 0, 0);
	glScaled(1.5, 1.5, 1.5);
	glColor3ub(139, 69, 19);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, brown_color);
	gluCylinder(quadObj, 1.3, 0, height, 13, 5);
	glColor3ub(0, 100, 0);
	while (height-4>0)
	{
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, green_color);
		width += 1;
		int step = rand() % 4 + 2;
		height -= step;
		glPushMatrix();
		glTranslated(0, 0, height);
		gluCylinder(quadObj, width, 0, step, 10, 4);
		glPopMatrix();
	}
	glPopMatrix();

}

double DegToRad(double D)
{
	return D / 180 * M_PI;
}
void line_mebius()
{
	glBegin(GL_POLYGON);
	glNormal3f(0, 1, 0);
	for (double u = 0; u < 360; u += 10)
	{
		for (double v = -1; v < 1; v += 0.03)
		{
			glColor3d(0.9, 0, 0);
			glVertex3d(cos(u) * (1 + v / 2 * cos(u / 2)), sin(u)*(1+v/2*cos(u/2)), v/2*sin(u/2));
		}
	}
	glEnd();
}
void grass()
{
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_NORMAL_MAP);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_NORMAL_MAP);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, Texture[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
/*
	glRotated(0, 1, 0, 0);
	glBegin(GL_QUADS);
	glTexCoord2d(0, 0);
	glVertex3d(-180, 0, -180);
	glTexCoord2d(1, 0);
	glVertex3d(180, 0, -180);
	glTexCoord2d(1,1);
	glVertex3d(180, 0, 180);
	glTexCoord2d(0, 1);
	glVertex3d(-180, 0, 180);
	glEnd();*/
}
void SkyBox(void)
{
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE,	GL_NORMAL_MAP);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_NORMAL_MAP);
	glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_NORMAL_MAP);

	glEnable(GL_TEXTURE_CUBE_MAP);

	glBindTexture(GL_TEXTURE_CUBE_MAP, Texture[1]);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glEnable(GL_TEXTURE_GEN_R);
	glColor3f(1, 1, 1);
	glutSolidSphere(200, 80, 40);
}
void polyana()
{
	glPushMatrix();
	glClearColor(0, 0, 0, 0);
	glRotated(-90, 1, 0, 0);
	glRotated(45, 0, 0, 1);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, green_dark_color);
	for (int i = -170; i < 170; i += 3)
	{
		bool flag = true;
		glBegin(GL_QUADS);
		float last_top;
		for (int j = -170; j < 170; j += 1)
		{
			glVertex3d(i, j, abs(sin(j)) * 3);
			glVertex3d(i, j, 0);
		}
		glEnd();
		glBegin(GL_QUADS);
		for (int s = -170; s < 170; s += 1)
		{
			glVertex3d(i, s, abs(sin(s)) * 3);
			glVertex3d(i, s, 0);
		}
		glEnd();
	}
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, green_color);
	glColor3f(0.03, 0.8, 0.04);
	glRectd(-180, -180, 180, 180);
	glPopMatrix();

}
void forest()
{
	glPushMatrix();
	glTranslated(-60, 0, -30);
	for (int i = 0; i < 100; i += 5)
	{
		double ret = (double)rand();
		glColor3d(0, ret-floor(ret), 0);
		glTranslated(5, 0, 40 * sin(i));
		tree();
		
	}
	glPopMatrix();
}

void lenta(double size)
{
	glColor3d(0.9, 0, 0);
	glBegin(GL_QUAD_STRIP);
	glVertex3d(-size / 2 - 0.2, 0.1, 0.5);
	glVertex3d(-size / 2 - 0.2, 0.1, -0.5);
	glVertex3d(size / 2 + 0.2, 0.1, 0.5);
	glVertex3d(size / 2 + 0.2, 0.1, -0.5);
	glVertex3d(size / 2 + 0.2, -size, 0.5);
	glVertex3d(size / 2 + 0.2, -size, -0.5);
	glVertex3d(-size / 2 - 0.2, -size, 0.5);
	glVertex3d(-size / 2 - 0.2, -size, -0.5);
	glVertex3d(-size / 2 - 0.2, 0.1, 0.5);
	glVertex3d(-size / 2 - 0.2, 0.1, -0.5);
	glEnd();
}
void present(double size, GLdouble *color)
{
	glPushMatrix();
	glTranslated(0, size / 2, 0);
	GLfloat present_color[] = { color[0],color[1],color[2],0.8 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, present_color);
	glColor3d(color[0],color[1],color[2]);
	glutSolidCube(size);
	glTranslated(0, size/2, 0);
	GLfloat lenta_color[] = { 1,0,0,0.8 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, lenta_color);
	lenta(size);
	glRotated(90, 0, 1, 0);
	lenta(size);
	glRotated(90, 1, 0, 0);
	glScaled(size/3, size/3, size/3);
	line_mebius();
	glPopMatrix();
}
 

GLfloat AnimationStart[25][2] =
{
	4,16,
	8,16,
	12,16,
	8, 20,
	4, 20,
	-4, 16,
	3, 10,
	10, 5,
	12, 21,
	-5, 22,
	-10, 13,
	-5, 7,
	13, 14,
	11, 20,
	-10, 20,
	-13, 10,
	-7, 4,
	15, 6,
	14, 16,
	-15, 20,
	-20, -4,
	-5, -8,
	18, 0,
	19, 13,
	-18, 14,

};

bool flag = false;


void
setMatrix(void)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//glOrtho(-89.0, 89.0, -65.0, 65.0, -80.0, 80.0);
	double aspect = (double)(width) / (double)(height);
	gluPerspective(70.0, 1, 1.0, 1000.0);
	glMatrixMode(GL_MODELVIEW);
	glTranslated(0, -8, -25);
}
int way = 0;

void
animation_go(void)
{
	glClearColor(0.0, 1.0, 1.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	gluLookAt(x, 1.0f, z,
		x + lx, 1.0f, z + lz,
		0.0f, 1.0f, 0.0f);
	draw();
	way += 5;
	if (way == 25)
	{
		way = 20;
		glutIdleFunc(stay);
	}
	glPopMatrix();
	Sleep(200);
	glutSwapBuffers();
}

void
animation_back(void)
{
	glClearColor(0.0, 1.0, 1.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	gluLookAt(x, 1.0f, z,
		x + lx, 1.0f, z + lz,
		0.0f, 1.0f, 0.0f);
	draw();
	way -= 5;
	if (way == -5)
	{
		way = 0;
		glutIdleFunc(stay);
	}
	glPopMatrix();
	Sleep(200);
	glutSwapBuffers();
}
GLdouble ColorPresents[5][3] =
{
	1, 0, 1,
	0, 1, 1,
	1, 1, 1,
	1, 1, 0,
	1, 0.73, 0
};

void draw()
{
	glClearColor(0.0, 1.0, 1.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	polyana();
	glDisable(GL_TEXTURE_2D);
	glPushMatrix();
	glLoadIdentity();
	glTranslated(0, -90, -90);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, white_color);
	SkyBox();
	glPopMatrix();
	forest();
	gluQuadricDrawStyle(quadObj, GLU_FILL);
	glTranslated(4, 0, 16);
	christmas_tree();
	glDisable(GL_TEXTURE_CUBE_MAP);
	for (int i = 0; i < 5; i++)
	{
		glPushMatrix();
		glTranslatef(AnimationStart[way + i][0], 0, AnimationStart[way + i][1]);
		present(4,ColorPresents[i]);
		glPopMatrix();
	}
}
void stay(void)
{

}

/* ARGSUSED1 */
void
keyboard(unsigned char c, int x, int y)
{
	switch (c) {
	case 27:
		exit(0);
		break;
	default:
		break;
	}
}

void
menu(int choice)
{
	switch (choice) {
	case 1:
		flag = true;
		glutIdleFunc(animation_go);
		break;
	case 2:
		flag = false;
		glutIdleFunc(animation_back);
		break;
	case 3:
		glLightModelfv(GL_LIGHT_MODEL_TWO_SIDE, lmodel_oneside);
		glutPostRedisplay();
		break;
	default: stay();
	}
}

void
resize(int w, int h)
{
	glViewport(0, 0, w, h);
	setMatrix();
}
void translate()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	gluLookAt(x, 1.0f, z,
		x + lx, 1.0f, z + lz,
		0.0f, 1.0f, 0.0f);
	draw();
	glPopMatrix();
	glutSwapBuffers();
}
void processSpecialKeys(int key, int xx, int yy) {
	float fraction = 0.3f;
	switch (key) {
	case GLUT_KEY_LEFT:
		angle += 0.05f;
		lx = sin(angle);
		lz = -cos(angle);
		translate();

		break;
	case GLUT_KEY_RIGHT:
		angle -= 0.05f;
		lx = sin(angle);
		lz = -cos(angle);
		translate();
		break;
	case GLUT_KEY_UP:
		x += lx * fraction;
		z += lz * fraction;
		translate();
		break;
	case GLUT_KEY_DOWN:
		x -= lx * fraction;
		z -= lz * fraction;
		translate();
		break;
	}
}

void
initLight(void)
{
	static float ambient[] =
	{ 0.1, 0.1, 0.1, 1.0 };
	static float diffuse[] =
	{ 1, 1.0, 1.0, 1.0 };
	static float position[] =
	{ 30.0, 90.0, 100.0, 0.0 };
	static float position1[] =
	{ 0, 0.0, -100.0, 0.0 };
	
	static float lmodel_ambient[] =
	{ 1.0, 1.0, 1.0, 1.0 };

	setMatrix();
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, position);


	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT1, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT1, GL_POSITION, position1);


	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);

	glEnable(GL_LIGHT1);
	glShadeModel(GL_SMOOTH);
}
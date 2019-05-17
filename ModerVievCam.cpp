
/* Copyright (c) Mark J. Kilgard, 1994. */

/**
 * (c) Copyright 1993, 1994, Silicon Graphics, Inc.
 * ALL RIGHTS RESERVED
 * Permission to use, copy, modify, and distribute this software for
 * any purpose and without fee is hereby granted, provided that the above
 * copyright notice appear in all copies and that both the copyright notice
 * and this permission notice appear in supporting documentation, and that
 * the name of Silicon Graphics, Inc. not be used in advertising
 * or publicity pertaining to distribution of the software without specific,
 * written prior permission.
 *
 * THE MATERIAL EMBODIED ON THIS SOFTWARE IS PROVIDED TO YOU "AS-IS"
 * AND WITHOUT WARRANTY OF ANY KIND, EXPRESS, IMPLIED OR OTHERWISE,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY OR
 * FITNESS FOR A PARTICULAR PURPOSE.  IN NO EVENT SHALL SILICON
 * GRAPHICS, INC.  BE LIABLE TO YOU OR ANYONE ELSE FOR ANY DIRECT,
 * SPECIAL, INCIDENTAL, INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY
 * KIND, OR ANY DAMAGES WHATSOEVER, INCLUDING WITHOUT LIMITATION,
 * LOSS OF PROFIT, LOSS OF USE, SAVINGS OR REVENUE, OR THE CLAIMS OF
 * THIRD PARTIES, WHETHER OR NOT SILICON GRAPHICS, INC.  HAS BEEN
 * ADVISED OF THE POSSIBILITY OF SUCH LOSS, HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, ARISING OUT OF OR IN CONNECTION WITH THE
 * POSSESSION, USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * US Government Users Restricted Rights
 * Use, duplication, or disclosure by the Government is subject to
 * restrictions set forth in FAR 52.227.19(c)(2) or subparagraph
 * (c)(1)(ii) of the Rights in Technical Data and Computer Software
 * clause at DFARS 252.227-7013 and/or in similar or successor
 * clauses in the FAR or the DOD or NASA FAR Supplement.
 * Unpublished-- rights reserved under the copyright laws of the
 * United States.  Contractor/manufacturer is Silicon Graphics,
 * Inc., 2011 N.  Shoreline Blvd., Mountain View, CA 94039-7311.
 *
 * OpenGL(TM) is a trademark of Silicon Graphics, Inc.
 */
 /*----------------------------------------------------------------------------
  *
  * olight.c : openGL (motif) example showing how to do hardware lighting
  *            including two_sided lighting.
  *
  * Author : Yusuf Attarwala
  *          SGI - Applications
  * Date   : Mar 93
  *
  *    press  left   button for animation
  *           middle button for two sided lighting (default)
  *           right  button for single sided lighting
  *
  *
  *---------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>


#include <GL/glut.h>
#include <GL/freeglut.h>
#include <GL/GLU.h>

  /* function declarations */
void
drawScene(void), setMatrix(void), initLightAndMaterial(void),
animation(void), resize(int w, int h), menu(int choice), keyboard(unsigned char c, int x, int y);


/* global variables */

float ax, ay, az;       /* angles for animation */
GLUquadricObj* quadObj; /* used in drawscene */
static float lmodel_twoside[] =
{ GL_TRUE };
static float lmodel_oneside[] =
{ GL_FALSE };
double width=800, height=600;
int
main(int argc, char** argv)
{
	glutInit(&argc, argv);
	quadObj = gluNewQuadric();  /* this will be used in drawScene
								 */
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow("Two-sided lighting");
	glutReshapeWindow(width, height);
	ax = 0.0;
	ay = 0.0;
	az = 0.0;

	initLightAndMaterial();
	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);
	glutCreateMenu(menu);
	glutAddMenuEntry("Motion", 3);
	glutAddMenuEntry("Two-sided lighting", 1);
	glutAddMenuEntry("One-sided lighting", 2);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
	return 0;             /* ANSI C requires main to return int. */
}

void rand_sphr_for_tree(double x, double y, double z, double radius)
{
	glTranslated(x, y, z);
	gluSphere(quadObj, radius, 20, 8);
}

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
	gluCylinder(quadObj, ret-floor(ret)+1, 1, size_tree, 20,8);
	glTranslated(0,0,size_tree);
	gluSphere(quadObj, rad_mn_sphere, 20, 8);
	for (double little_sphere = rad_mn_sphere - 0.5, y = -little_sphere; y < little_sphere; y += 1)
	{
		for (double circle = sqrt(pow(little_sphere, 2) - pow(y, 2)), x = -circle; x < circle; x += 0.7)
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
	gluCylinder(quadObj, 1.3, 0, height, 13, 5);
	while (height-4>0)
	{
		width += 1;
		int step = rand() % 4 + 2;
		height -= step;
		glPushMatrix();
		glTranslated(0, 0, height-1.0);
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
	glColor3b(1, 0, 0);
	glBegin(GL_POLYGON);
	for (double u = 0; u < 360; u += 10)
	{
		for (double v = -1; v < 1; v += 0.03)
		{
			glVertex3d(cos(u) * (1 + v / 2 * cos(u / 2)), sin(u)*(1+v/2*cos(u/2)), v/2*sin(u/2));
		}
	}
	glEnd();
}

GLfloat VertexArrayPresent[23][36]=
{
	0, 0, 0,
	0, 0, 1,
	0, 1, 0,
	0, 1, 1,
	1, 0, 0, 
	1, 0, 1,
	1, 1, 0,
	1, 1, 1,
};
GLubyte IndexArray[12][2]=
{
	0, 1,
	1, 2,
	3, 4, 
	5, 6,
	7, 8
};



void present(double size)
{
	glutSolidCube(size);
	glTranslated(0, size/2, 0);
	glRotated(90, 1, 0, 0);
	glScaled(size/3, size/3, size/3);
	line_mebius();
}

void
drawScene(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();
	gluQuadricDrawStyle(quadObj, GLU_FILL);
	glColor3f(1.0, 1.0, 0.0);
	glRotatef(ax, 1.0, 0.0, 0.0);
	glRotatef(-ay, 0.0, 1.0, 0.0);
	christmas_tree();
	glTranslated(40, 3, -10);
	present(4);
	glPopMatrix();

	glutSwapBuffers();
}

void
setMatrix(void)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//glOrtho(-89.0, 89.0, -65.0, 65.0, -80.0, 80.0);
	double aspect = (double)(width) / (double)(height);
	gluPerspective(90.0, 1, 1.0, 1000.0);
	glMatrixMode(GL_MODELVIEW);
	glTranslated(0, 0, -30);
}

int count = 0;

void
animation(void)
{
	glPushMatrix();
	ax += 5.0;
	ay -= 2.0;
	az += 5.0;
	if (ax >= 360)
		ax = 0.0;
	if (ay <= -360)
		ay = 0.0;
	if (az >= 360)
		az = 0.0;
	glPopMatrix();
	drawScene();
	count++;
	if (count >= 60)
		glutIdleFunc(NULL);
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
	case 3:
		count = 0;
		glutIdleFunc(animation);
		break;
	case 2:
		glLightModelfv(GL_LIGHT_MODEL_TWO_SIDE, lmodel_oneside);
		glutSetWindowTitle("One-sided lighting");
		glutPostRedisplay();
		break;
	case 1:
		glLightModelfv(GL_LIGHT_MODEL_TWO_SIDE, lmodel_twoside);
		glutSetWindowTitle("Two-sided lighting");
		glutPostRedisplay();
		break;
	}
}

void
resize(int w, int h)
{
	glViewport(0, 0, w, h);
	setMatrix();
}

void
initLightAndMaterial(void)
{
	static float ambient[] =
	{ 0.1, 0.1, 0.1, 1.0 };
	static float diffuse[] =
	{ 0.5, 1.0, 1.0, 1.0 };
	static float position[] =
	{ 90.0, 90.0, 150.0, 0.0 };

	static float front_mat_shininess[] =
	{ 60.0 };
	static float front_mat_specular[] =
	{ 0.2, 0.2, 0.2, 1.0 };
	static float front_mat_diffuse[] =
	{ 0.5, 0.5, 0.28, 1.0 };
	static float back_mat_shininess[] =
	{ 60.0 };
	static float back_mat_specular[] =
	{ 0.5, 0.5, 0.2, 1.0 };
	static float back_mat_diffuse[] =
	{ 1.0, 0.9, 0.2, 1.0 };

	static float lmodel_ambient[] =
	{ 1.0, 1.0, 1.0, 1.0 };

	setMatrix();
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, position);

	glMaterialfv(GL_FRONT, GL_SHININESS, front_mat_shininess);
	glMaterialfv(GL_FRONT, GL_SPECULAR, front_mat_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, front_mat_diffuse);
	glMaterialfv(GL_BACK, GL_SHININESS, back_mat_shininess);
	glMaterialfv(GL_BACK, GL_SPECULAR, back_mat_specular);
	glMaterialfv(GL_BACK, GL_DIFFUSE, back_mat_diffuse);

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
	glLightModelfv(GL_LIGHT_MODEL_TWO_SIDE, lmodel_twoside);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glShadeModel(GL_SMOOTH);
}
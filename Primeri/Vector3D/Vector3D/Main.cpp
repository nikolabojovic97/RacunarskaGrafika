#include <GL/freeglut.h>
#include <vector>
#include <iostream>
#include "Vector3D.h"
#include "Matrix3x3.h"

#define PI 3.1415926
#define FPS 60.0
double w = 20.0;
double alpha = w / FPS;

vector<Vector3D> vectors;
Vector3D center(1.5, 1.5, 0.0);
Matrix3x3 MT, MTcenter;

void createVectors() {
	vectors.resize(4);
	vectors[0] = Vector3D(1.0, 1.0, 0.0);
	vectors[1] = Vector3D(2.0, 1.0, 0.0);
	vectors[2] = Vector3D(2.0, 2.0, 0.0);
	vectors[3] = Vector3D(1.0, 2.0, 0.0);
}


void createTransformMatrixCenter(Matrix3x3& MTcenter, Vector3D &C) {
	Matrix3x3 MTranslateCO, MTranslateOC, MRotate;
	MTranslateCO.LoadTranslate(-C.X(), -C.Y());
	MTranslateOC.LoadTranslate(C.X(), C.Y());
	MRotate.LoadRotateAroundOrigin(alpha);
	MT = MTranslateOC * MRotate * MTranslateCO;
}

void Transform(Matrix3x3& MT, vector<Vector3D>& v) {
	for (int i = 0; i < v.size(); i++)
		v[i] = MT * v[i];
}

void drawRect(vector<Vector3D> v) {
	glBegin(GL_POLYGON);
	for (int i = 0; i < v.size(); i++)
		glVertex3f(v[i].X(), v[i].Y(), v[i].Z());
	glEnd();
}



void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);
	drawRect(vectors);
	glFlush();
}

void timer(int v) {
	Transform(MTcenter, vectors);
	glutTimerFunc(1000 / FPS, timer, v);
	glutPostRedisplay();
}

void init(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, 3.0, 0.0, 3.0, -1.0, 1.0);
}
int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("hello");
	init();

	createVectors();
	createTransformMatrixCenter(MTcenter, center);

	glutDisplayFunc(display);
	glutTimerFunc(1000 / FPS, timer, 0);
	glutMainLoop();
	
	return 0;
}
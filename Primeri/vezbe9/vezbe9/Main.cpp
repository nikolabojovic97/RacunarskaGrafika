#include "GL/freeglut.h"
#include "Vector3D.h"
#include "Vector4D.h"
#include "Matrix4x4.h"
#include "Matrix3x3.h"
#include <vector>
#include <iostream>

using namespace std;

#define PI 3.14159265359

#define MOVING_CONST 0.1
#define ROTATION_CONST 3.14/180.0
#define SPEED_CONST_NORMAL 1
#define SPEED_CONST_FAST 10

int WINDOW_HEIGHT = 600;
double ASP_RAT = 4.0 / 3.0;
int FPS = 60;

int speedIndicator;
double rotationUpDown;

Vector4D CameraPosition(10.0, 0.0, 10.0, 1.0);
Vector4D LookAt_vector(0.0, 0.0, 0.0, 1.0);
Vector4D LookUp_vector(0.0, 1.0, 0.0, 1.0);
Vector3D center;

Vector3D x(1.0, 0.0, 0.0);
Vector3D y(0.0, 1.0, 0.0);
Vector3D z(0.0, 0.0, 1.0);


void drawPoly(vector<Vector3D>& poly) {
	glBegin(GL_POLYGON);
	for (int i = 0; i < poly.size(); i++)
		glVertex3f(poly[i].m_x[0], poly[i].m_x[1], poly[i].m_x[2]);
	glEnd();
}

void setCamera() {
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(CameraPosition.m_x[0], CameraPosition.m_x[1], CameraPosition.m_x[2],
		LookAt_vector.m_x[0], LookAt_vector.m_x[1], LookAt_vector.m_x[2],
		LookUp_vector.m_x[0], LookUp_vector.m_x[1], LookUp_vector.m_x[2]);
}

void MoveForward_CameraTransform()
{
	Matrix4x4 MT;
	Vector4D v = LookAt_vector - CameraPosition;
	v.m_x[1] = 0.0;
	v.normalize();
	double speed = SPEED_CONST_NORMAL;
	if (speedIndicator == 1)
		speed = SPEED_CONST_FAST;
	v = v * speed * MOVING_CONST;
	MT.loadTranslate(v.m_x[0], v.m_x[1], v.m_x[2]);
	CameraPosition = MT.transformed(CameraPosition);
	LookAt_vector = MT.transformed(LookAt_vector);
}

void MoveBackward_CameraTransform()
{
	Matrix4x4 MT;
	Vector4D v = LookAt_vector - CameraPosition;
	v.m_x[1] = 0.0;
	v.normalize();
	double speed = SPEED_CONST_NORMAL;
	if (speedIndicator == 1)
		speed = SPEED_CONST_FAST;
	v = -v * speed * MOVING_CONST;
	MT.loadTranslate(v.m_x[0], v.m_x[1], v.m_x[2]);
	CameraPosition = MT.transformed(CameraPosition);
	LookAt_vector = MT.transformed(LookAt_vector);
}

void MoveLeft_CameraTransform()
{
	Matrix4x4 MT;
	Vector4D f = LookAt_vector - CameraPosition;
	Vector4D w = LookUp_vector.crossProduct(f);
	w.normalize();
	w = w * SPEED_CONST_NORMAL * MOVING_CONST;
	MT.loadTranslate(w.m_x[0], w.m_x[1], w.m_x[2]);
	CameraPosition = MT.transformed(CameraPosition);
	LookAt_vector = MT.transformed(LookAt_vector);
}
void MoveRight_CameraTransform()
{
	Matrix4x4 MT;
	Vector4D f = LookAt_vector - CameraPosition;
	Vector4D w = LookUp_vector.crossProduct(f);
	w.normalize();
	w = -w * SPEED_CONST_NORMAL * MOVING_CONST;
	MT.loadTranslate(w.m_x[0], w.m_x[1], w.m_x[2]);
	CameraPosition = MT.transformed(CameraPosition);
	LookAt_vector = MT.transformed(LookAt_vector);
}

void TurnLeft_CameraTransform()
{
	Matrix4x4 MR, MT1, MT2, MT;
	MT1.loadTranslate(-CameraPosition.m_x[0], 0.0, -CameraPosition.m_x[2]);
	MT2.loadTranslate(CameraPosition.m_x[0], 0.0, CameraPosition.m_x[2]);
	MR.loadRotateY(ROTATION_CONST);
	MT = MT2 * MR * MT1;
	LookAt_vector = MT.transformed(LookAt_vector);
}

void TurnRight_CameraTransform()
{
	Matrix4x4 MR, MT1, MT2, MT;
	MT1.loadTranslate(-CameraPosition.m_x[0], 0.0, -CameraPosition.m_x[2]);
	MT2.loadTranslate(CameraPosition.m_x[0], 0.0, CameraPosition.m_x[2]);
	MR.loadRotateY(-ROTATION_CONST);
	MT = MT2 * MR * MT1;
	LookAt_vector = MT.transformed(LookAt_vector);
}

void LookUp_CameraTransform()
{
	Matrix4x4 MR;
	Vector4D f = LookAt_vector - CameraPosition;
	Vector4D w = LookUp_vector.crossProduct(f);
	if (rotationUpDown + ROTATION_CONST < 0.5 * PI)
	{
		MR.loadRotateAxis(w, -ROTATION_CONST, CameraPosition);
		LookAt_vector = MR.transformed(LookAt_vector);
		Vector4D LookUp_vector4d = LookUp_vector;
		LookUp_vector4d.m_x[3] = 0.0;
		LookUp_vector4d = MR.transformed(LookUp_vector4d);
		LookUp_vector = LookUp_vector4d;
		rotationUpDown += ROTATION_CONST;
	}
}

void LookDown_CameraTransform()
{
	Matrix4x4 MR;
	Vector4D f = LookAt_vector - CameraPosition;
	Vector4D w = LookUp_vector.crossProduct(f);
	if (rotationUpDown - ROTATION_CONST > -0.5 * PI)
	{
		MR.loadRotateAxis(w, ROTATION_CONST, CameraPosition);
		LookAt_vector = MR.transformed(LookAt_vector);
		Vector4D LookUp_vector4d = LookUp_vector;
		LookUp_vector4d.m_x[3] = 0.0;
		LookUp_vector4d = MR.transformed(LookUp_vector4d);
		LookUp_vector = LookUp_vector4d;
		rotationUpDown -= ROTATION_CONST;
	}
}


void KeyboardKeyPressed(unsigned char key, int x, int y)
{
	Matrix4x4 MT;
	Vector3D V;
	Vector3D L, T;
	switch (key)
	{
	case 27: //ESC key
		exit(0);
		break;
	case 'a':
		cout << "a pressed -> moving left" << endl;
		MoveLeft_CameraTransform();
		break;
	case 'd':
		cout << "d pressed -> moving right" << endl;
		MoveRight_CameraTransform();
		break;
	case 'w':
		cout << "w pressed -> moving forward" << endl;
		MoveForward_CameraTransform();
		break;
	case 's':
		cout << "s pressed -> moving backward" << endl;
		MoveBackward_CameraTransform();
		break;
	case 52:
		cout << "4 pressed -> turning left" << endl;
		TurnLeft_CameraTransform();
		break;
	case 54:
		cout << "6 pressed -> turning right" << endl;
		TurnRight_CameraTransform();
		break;
	case 56:
		cout << "8 pressed -> look up" << endl;
		LookUp_CameraTransform();
		break;
	case 50:
		cout << "2 pressed -> look down" << endl;
		LookDown_CameraTransform();
		break;
	}
	setCamera();
	glutPostRedisplay();
}

void mousePress(int button, int state, int x, int y)
{
	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN)
			speedIndicator = 1;
		else
			speedIndicator = 0;
		break;
	default:
		break;
	}
}


void drawAxis() {
	//X
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_LINES);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(10.0, 0.0, 0.0);
	glEnd();

	//Y
	glColor3f(0.0, 1.0, 0.0);
	glBegin(GL_LINES);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 10.0, 0.0);
	glEnd();

	//Z
	glColor3f(0.0, 0.0, 1.0);
	glBegin(GL_LINES);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 0.0, 10.0);
	glEnd();
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLineWidth(1);
	drawAxis();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


	glutSwapBuffers();
}


void reshape(int w, int h) {
	if (w < h)
		WINDOW_HEIGHT = w / ASP_RAT;
	else
		WINDOW_HEIGHT = h;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40.0, ASP_RAT, 0.1, 50.0);

	setCamera();
}

void timer(int v) {

}

void init() {
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glShadeModel(GL_FLAT);
	glEnable(GL_DEPTH_TEST);
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitWindowSize(WINDOW_HEIGHT * ASP_RAT, WINDOW_HEIGHT);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutCreateWindow("klk");

	init();


	speedIndicator = 0;
	rotationUpDown = 0.0;
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(mousePress);
	glutKeyboardFunc(KeyboardKeyPressed);
	glutTimerFunc(1000 / FPS, timer, 0);
	glutMainLoop();

	return 0;
}
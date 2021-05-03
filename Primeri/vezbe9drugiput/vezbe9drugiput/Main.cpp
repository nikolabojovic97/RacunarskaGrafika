#include "GL/freeglut.h"
#include "Vector4D.h"
#include "Matrix4x4.h"
#include <vector>
using namespace std;

#define PI 3.14159265359

#define SPEED_CONST_NORMAL 1.0
#define SPEED_CONST_FAST 10.0
#define MOVING_CONST 0.1
#define ROT_CONST PI / 180.0

int WINDOW_HEIGHT = 600;
double ASP_RAT = 4.0 / 3.0;
int FPS = 60;

Vector4D cameraPosition(10.0, 0.0, 10.0, 1.0);
Vector4D lookAt(0.0, 0.0, 0.0, 1.0);
Vector4D lookUp(0.0, 1.0, 0.0, 1.0);
Vector4D center;

Vector4D x(1.0, 0.0, 0.0, 0.0);
Vector4D y(0.0, 1.0, 0.0, 0.0);
Vector4D z(0.0, 0.0, 1.0, 0.0);

int speedIndicator;
double rotationUpDown;


void drawPoly(vector<Vector4D>& poly) {
	glBegin(GL_POLYGON);
	for (int i = 0; i < poly.size(); i++)
		glVertex3f(poly[i].x(), poly[i].y(), poly[i].z());
	glEnd();
}

void drawAxis() {
	//X
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_LINES);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(1.0, 0.0, 0.0);
	glEnd();

	//Y
	glColor3f(0.0, 1.0, 0.0);
	glBegin(GL_LINES);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 1.0, 0.0);
	glEnd();

	//Z
	glColor3f(0.0, 0.0, 1.0);
	glBegin(GL_LINES);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 0.0, 1.0);
	glEnd();
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLineWidth(1);
	drawAxis();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


	glutSwapBuffers();
}

void setCamera() {
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(cameraPosition.x(), cameraPosition.y(), cameraPosition.z(),
		lookAt.x(), lookAt.y(), lookAt.z(),
		lookUp.x(), lookUp.y(), lookUp.z());
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

void mouseClicked(int button, int state, int x, int y) {
	switch (button) {
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

void MoveForward() {
	Matrix4x4 MT;
	Vector4D f = lookAt - cameraPosition;
	f.m_x[1] = 0;
	f.normalize();

	double speed = SPEED_CONST_NORMAL;
	if (speedIndicator == 1)
		speed = SPEED_CONST_FAST;
	
	f = f * speed * MOVING_CONST;
	MT.loadTranslate(f);
	cameraPosition = MT.transformed(cameraPosition);
	lookAt = MT.transformed(lookAt);
}

void MoveBackward() {
	Matrix4x4 MT;
	Vector4D f = lookAt - cameraPosition;
	f.m_x[1] = 0;
	f.normalize();

	double speed = SPEED_CONST_NORMAL;
	if (speedIndicator == 1)
		speed = SPEED_CONST_FAST;
	
	f = -f * speed * MOVING_CONST;
	MT.loadTranslate(f);
	cameraPosition = MT.transformed(cameraPosition);
	lookAt = MT.transformed(lookAt);
}

void MoveLeft() {
	Matrix4x4 MT;
	Vector4D f = lookAt - cameraPosition;
	Vector4D w = lookUp.crossProduct(f);
	w.m_x[1] = 0.0;
	w.normalize();
	
	w = w * SPEED_CONST_NORMAL * MOVING_CONST;
	MT.loadTranslate(w);
	cameraPosition = MT.transformed(cameraPosition);
	lookAt = MT.transformed(lookAt);
}

void MoveRight() {
	Matrix4x4 MT;
	Vector4D f = lookAt - cameraPosition;
	Vector4D w = lookUp.crossProduct(f);
	w.m_x[1] = 0;
	w.normalize();

	w = -w * SPEED_CONST_NORMAL * MOVING_CONST;
	MT.loadTranslate(w);
	cameraPosition = MT.transformed(cameraPosition);
	lookAt = MT.transformed(lookAt);
}

void LookUp() {
	Matrix4x4 MR;
	Vector4D f = lookAt - cameraPosition;
	Vector4D w = lookUp.crossProduct(f);

	if (rotationUpDown + ROT_CONST < 0.5 * PI) {
		MR.loadRotateAxis(w, -ROT_CONST, cameraPosition);
		lookAt = MR.transformed(lookAt);
		lookUp = MR.transformed(lookUp);

		rotationUpDown += ROT_CONST;
	}
}

void LookDown() {
	Matrix4x4 MR;
	Vector4D f = lookAt - cameraPosition;
	Vector4D w = lookUp.crossProduct(f);

	if (rotationUpDown - ROT_CONST > -0.5 * PI) {
		MR.loadRotateAxis(w, ROT_CONST, cameraPosition);
		lookAt = MR.transformed(lookAt);
		lookUp = MR.transformed(lookUp);

		rotationUpDown -= ROT_CONST;
	}
}

void TurnLeft() {
	Matrix4x4 MT, MT1, MT2, MR;
	MT1.loadTranslate(-cameraPosition);
	MT2.loadTranslate(cameraPosition);
	MR.loadRotateY(ROT_CONST);

	MT = MT2 * MR * MT1;
	lookAt = MT.transformed(lookAt);
}

void TurnRight() {
	Matrix4x4 MT, MT1, MT2, MR;
	MT1.loadTranslate(-cameraPosition);
	MT2.loadTranslate(cameraPosition);
	MR.loadRotateY(-ROT_CONST);

	MT = MT2 * MR * MT1;
	lookAt = MT.transformed(lookAt);
}

void keyPressed(unsigned char key, int x, int y) {
	switch (key) {
	case 'w':
		printf("W pressed.\n");
		printf("%.2f %.2f %.2f\n", cameraPosition.x(), cameraPosition.y(), cameraPosition.z());
		MoveForward();
		break;
	case 's':
		printf("S pressed.\n");
		MoveBackward();
		break;
	case 'a':
		printf("A pressed.\n");
		MoveLeft();
		break;
	case 'd':
		printf("D pressed.\n");
		MoveRight();
		break;
	case '8':
		printf("8 pressed.\n");
		LookUp();
		break;
	case '5':
		printf("5 pressed.\n");
		LookDown();
		break;
	case '4':
		printf("4 pressed.\n");
		TurnLeft();
		break;
	case '6':
		printf("6 pressed.\n");
		TurnRight();
		break;
	}

	setCamera();
	glutPostRedisplay();
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
	rotationUpDown = 0;
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouseClicked);
	glutKeyboardFunc(keyPressed);
	glutTimerFunc(1000 / FPS, timer, 0);
	glutMainLoop();

	return 0;
}
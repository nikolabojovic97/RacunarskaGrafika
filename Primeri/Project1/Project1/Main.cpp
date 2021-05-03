#include "GL/freeglut.h"
#include "Vector4D.h"
#include "Matrix4x4.h"
#include <vector>
using namespace std;

#define PI 3.14159265359

#define MOVING_CONST 0.1;
#define ROTATION_CONST PI / 180.0

int WINDOW_HEIGHT = 800;
double ASP_RAT = 9.0 / 9.0;
int FPS = 60;

Vector4D cameraPosition(50.0, 5.0, 50.0, 1.0);
Vector4D lookAt(0.0, 5.0, 0.0, 1.0);
Vector4D lookUp(0.0, 1.0, 0.0, 1.0);
Vector4D center;

Vector4D x(1.0, 0.0, 0.0, 0.0);
Vector4D y(0.0, 1.0, 0.0, 0.0);
Vector4D z(0.0, 0.0, 1.0, 0.0);

double alpha = 90;

double rotationLeftRight = 0;
double rotationUpDown = 0;
double speed = 0;


double scale(double min, double max, double a, double b, double x) {
	return (b - a) * (x - min) / (max - min) + a;
}

void Transform(vector<Vector4D>& a, Matrix4x4& MT) {
	for (int i = 0; i < a.size(); i++)
		a[i] = MT.transformed(a[i]);
}

void Transform(vector<vector<Vector4D>>& a, Matrix4x4& MT) {
	for (int i = 0; i < a.size(); i++)
		Transform(a[i], MT);
}

void Draw(vector<Vector4D>& poly, int mode) {
	glBegin(mode);
	for (int i = 0; i < poly.size(); i++)
		glVertex3f(poly[i].x(), poly[i].y(), poly[i].z());
	glEnd();
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

void setCamera() {
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(cameraPosition.x(), cameraPosition.y(), cameraPosition.z(),
		lookAt.x(), lookAt.y(), lookAt.z(),
		lookUp.x(), lookUp.y(), lookUp.z());
}

void PushVertex(Vector4D& a) {
	glVertex3f(a.x(), a.y(), a.z());
}

void PushVertexRatio(Vector4D& a) {
	glVertex3f(a.x(), a.y() * ASP_RAT, a.z());
}


void RenderString(double x, double y, void* font, double r, double g, double b) {
	glColor3f(r, g, b);
	char s[100];

	sprintf_s(s, "x = %.2f\ny = %.2f\nz = %.2f", cameraPosition.x(), cameraPosition.y(), cameraPosition.z());
	glRasterPos2f(x, y);
	glutBitmapString(font, (const unsigned char*)s);
}

void DrawCube() {
	vector<Vector4D> cube;
	cube.resize(8);

	double a = 120.0;
	cube[0] = Vector4D(-a, -a, -a);
	cube[1] = Vector4D(-a, -a, a);
	cube[2] = Vector4D(a, -a, a);
	cube[3] = Vector4D(a, -a, -a);
	cube[4] = Vector4D(-a, 0.0, -a);
	cube[5] = Vector4D(-a, 0.0, a);
	cube[6] = Vector4D(a, 0.0, a);
	cube[7] = Vector4D(a, 0.0, -a);

	vector<Vector4D> poly;
	poly.resize(4);

	glColor3f(0.3, 0.3, 0.8);

	poly[0] = cube[0];
	poly[1] = cube[1];
	poly[2] = cube[2];
	poly[3] = cube[3];
	Draw(poly, GL_POLYGON);

	poly[0] = cube[4];
	poly[1] = cube[5];
	poly[2] = cube[6];
	poly[3] = cube[7];
	Draw(poly, GL_POLYGON);

	poly[0] = cube[0];
	poly[1] = cube[1];
	poly[2] = cube[5];
	poly[3] = cube[4];
	Draw(poly, GL_POLYGON);

	poly[0] = cube[1];
	poly[1] = cube[2];
	poly[2] = cube[6];
	poly[3] = cube[5];
	Draw(poly, GL_POLYGON);

	poly[0] = cube[2];
	poly[1] = cube[3];
	poly[2] = cube[7];
	poly[3] = cube[6];
	Draw(poly, GL_POLYGON);

	poly[0] = cube[3];
	poly[1] = cube[0];
	poly[2] = cube[4];
	poly[3] = cube[7];
	Draw(poly, GL_POLYGON);

}

void DrawRect() {
	double xmax = 1.0;
	double ymax = 1.0;

	vector<Vector4D> poly;
	poly.resize(4);

	poly[0] = Vector4D(-xmax, -ymax, 0.0);
	poly[1] = Vector4D(xmax, -ymax, 0.0);
	poly[2] = Vector4D(xmax, ymax, 0.0);
	poly[3] = Vector4D(-xmax, ymax, 0.0);


	double nPoints = 30;
	double alpha = 2.0 * PI / nPoints;
	double r = 0.1;
	vector<Vector4D> circle;
	circle.resize(nPoints);
	for (int i = 0; i < nPoints; i++)
		circle[i] = Vector4D(cos(i * alpha) * r, sin(i * alpha) * r, 0.0);


	glColor3f(0.8, 0.8, 0.8);
	Draw(poly, GL_POLYGON);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ZERO, GL_SRC_COLOR);
	
	Draw(circle, GL_POLYGON);
	glDisable(GL_BLEND);
}



void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_VIEWPORT);
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(alpha, ASP_RAT, 0.1, 40.0);
	setCamera();

	DrawCube();

	glMatrixMode(GL_VIEWPORT);
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();


	glDisable(GL_DEPTH_TEST);
	DrawRect();

	glEnable(GL_DEPTH_TEST);
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

void mouseClicked(int button, int state, int x, int y) {

}




void RotateLeft() {
	Vector4D f = lookAt - cameraPosition;
	f.normalize();

	if (rotationLeftRight + ROTATION_CONST < 0.25 * PI) {
		Matrix4x4 MT;
		MT.loadRotateAxis(f, ROTATION_CONST);
		lookUp = MT.transformed(lookUp);
		/*
			Transform UI
		*/
		rotationLeftRight += ROTATION_CONST;
	}
}

void RotateRight() {
	Vector4D f = lookAt - cameraPosition;
	f.normalize();

	if (rotationLeftRight - ROTATION_CONST > -0.25 * PI) {
		Matrix4x4 MT;
		MT.loadRotateAxis(f, -ROTATION_CONST);
		lookUp = MT.transformed(lookUp);
		/*
			Transform UI
		*/
		rotationLeftRight -= ROTATION_CONST;
	}
}

void TurnLeft() {
	Vector4D f = lookAt - cameraPosition;
	Matrix4x4 MT, MT1, MT2, MR;

	MT1.loadTranslate(-cameraPosition);
	MT2.loadTranslate(cameraPosition);
	MR.loadRotateY(ROTATION_CONST);

	MT = MT2 * MR * MT1;
	lookAt = MT.transformed(lookAt);
}

void TurnRight() {
	Vector4D f = lookAt - cameraPosition;
	Matrix4x4 MT, MT1, MT2, MR;

	MT1.loadTranslate(-cameraPosition);
	MT2.loadTranslate(cameraPosition);
	MR.loadRotateY(-ROTATION_CONST);

	MT = MT2 * MR * MT1;
	lookAt = MT.transformed(lookAt);
}

void MoveForward() {
	Vector4D f = lookAt - cameraPosition;
	f.normalize();

	f = f * speed * MOVING_CONST;
	Matrix4x4 MT;
	MT.loadTranslate(f);
	MT.print();
	cameraPosition = MT.transformed(cameraPosition);
	lookAt = MT.transformed(lookAt);
}

void MoveBackward(){
	Vector4D f = lookAt - cameraPosition;
	f.normalize();

	f = -f * speed * MOVING_CONST;
	Matrix4x4 MT;
	MT.loadTranslate(f);
	MT.print();
	cameraPosition = MT.transformed(cameraPosition);
	lookAt = MT.transformed(lookAt);
}

void GoUp() {
	Vector4D f = lookAt - cameraPosition;
	Vector4D w = lookUp.crossProduct(f);
	w.normalize();

	if (rotationUpDown + ROTATION_CONST < 0.25 * PI) {
		Matrix4x4 MT;
		MT.loadRotateAxis(w, -ROTATION_CONST, cameraPosition);
		lookAt = MT.transformed(lookAt);
		/*
			Transform UI
		*/
		rotationUpDown += ROTATION_CONST;
	}
}

void GoDown() {
	Vector4D f = lookAt - cameraPosition;
	Vector4D w = lookUp.crossProduct(f);
	w.normalize();

	if (rotationUpDown - ROTATION_CONST > -0.25 * PI) {
		Matrix4x4 MT;
		MT.loadRotateAxis(w, ROTATION_CONST, cameraPosition);
		lookAt = MT.transformed(lookAt);

		MT.loadTranslate(0.0, ROTATION_CONST * 0.5, 0.0);
		/*
			Transform UI
		*/
		rotationUpDown -= ROTATION_CONST;
	}
}

void keyPressed(unsigned char key, int x, int y) {
	switch (key) {
	case 'w':

		break;
	case 's':

		break;
	case 'a':

		break;
	case 'd':
	
		break;
	case '4':

		break;
	case '6':

		break;
	case '8':

		break;
	case '5':

		break;
	}

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
	glutCreateWindow("Kolokvijum");

	init();


	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouseClicked);
	glutKeyboardFunc(keyPressed);
	glutTimerFunc(1000 / FPS, timer, 0);
	glutMainLoop();

	return 0;
}
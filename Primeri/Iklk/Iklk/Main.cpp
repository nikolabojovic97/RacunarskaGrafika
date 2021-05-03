#include "GL/freeglut.h"
#include "Vector4D.h"
#include "Matrix4x4.h"
#include <vector>
using namespace std;

#define PI 3.14159265359

#define MOVING_CONST 0.1;
#define ROTATION_CONST PI / 180.0

int WINDOW_HEIGHT = 800;
double ASP_RAT = 16.0 / 9.0;
int FPS = 60;

Vector4D cameraPosition(5.0, 5.0, 5.0, 1.0);
Vector4D lookAt(0.0, 0.0, 0.0, 1.0);
Vector4D lookUp(0.0, 1.0, 0.0, 1.0);
Vector4D center(0.0, 0.0, 0.0, 1.0);

Vector4D x(1.0, 0.0, 0.0, 0.0);
Vector4D y(0.0, 1.0, 0.0, 0.0);
Vector4D z(0.0, 0.0, 1.0, 0.0);

vector<Vector4D> meta1;
vector<Vector4D> meta2;
vector<Vector4D> kupa1;
vector<Vector4D> valjak1;
vector<Vector4D> valjak2;

int nPoints = 30;
double rMete = 4.0;
double hMete = 1.0;

double d1 = 1.0;
double d2 = 2.0;
double d3 = 0.75;
double d4 = 10.0;
double d5 = 8.0;
double d6 = 7.0;
double d7 = 11.0;

double r1 = 0.1;
double r2 = 1;
double r3 = 0.6;


double alpha = 2.0 * PI / (nPoints * 1.0);



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

void CreateMeta() {
	meta1.resize(nPoints + 2);
	meta2.resize(nPoints + 2);

	meta1[0] = Vector4D(0.0, 0.0, 0.0, 1.0);
	for (int i = 0; i <= nPoints; i++)
		meta1[i + 1] = Vector4D(cos(i * alpha) * rMete, 0.0, sin(i * alpha) * rMete, 1.0);
	
	meta2 = meta1;
	Matrix4x4 MT;
	MT.loadTranslate(0.0, hMete, 0.0);
	Transform(meta2, MT);
}

void DrawMeta() {
	glColor3f(1.0, 0.0, 0.0);
	Draw(meta1, GL_POLYGON);
	Draw(meta2, GL_POLYGON);

	vector<Vector4D> poly;
	poly.resize(4);
	glColor3f(0.9, 0.1, 0.1);
	for (int i = 1; i <= nPoints; i++) {
		poly[0] = meta1[i];
		poly[1] = meta2[i];
		poly[2] = meta2[i+1];
		poly[3] = meta1[i+1];
		Draw(poly, GL_POLYGON);
	}
}

void CreateKupa() {
	kupa1.resize(nPoints + 2);

	kupa1[0] = Vector4D(0.0, 0.0, 0.0, 1.0);
	for(int i = 0; i <= nPoints; i++)
		kupa1[i + 1] = Vector4D(cos(i * alpha) * r1, d1, sin(i * alpha) * r1, 1.0);
}

void DrawKupa(vector<Vector4D> kupa1) {
	vector<Vector4D> poly;
	poly.resize(3);

	glColor3f(0.9, 0.1, 0.1);
	for (int i = 1; i <= nPoints; i++) {
		poly[0] = kupa1[i];
		poly[1] = kupa1[0];
		poly[2] = kupa1[i + 1];
		Draw(poly, GL_POLYGON);
	}
}

void CreateValjak() {
	valjak1.resize(nPoints + 2);
	valjak2.resize(nPoints + 2);

	valjak1[0] = Vector4D(0.0, 0.0, 0.0, 1.0);
	for (int i = 0; i <= nPoints; i++)
		valjak1[i + 1] = Vector4D(cos(i * alpha) * r1, 0.0, sin(i * alpha) * r1, 1.0);

	valjak2 = valjak1;
	Matrix4x4 MT;
	MT.loadTranslate(0.0, d1, 0.0);
	Transform(valjak1, MT);
	MT.loadTranslate(0.0, d1 + d2, 0.0);
	Transform(valjak2, MT);
}

void DrawValjak(vector<Vector4D> valjak1, vector<Vector4D> valjak2) {
	vector<Vector4D> poly;
	poly.resize(4);

	glColor3f(0.9, 0.1, 0.1);
	for (int i = 1; i <= nPoints; i++) {
		poly[0] = valjak1[i];
		poly[1] = valjak2[i];
		poly[2] = valjak2[i + 1];
		poly[3] = valjak1[i + 1];
		Draw(poly, GL_POLYGON);
	}
}

void DrawKupa2() {

}



void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_VIEWPORT);
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(80.0f, 1.0, 0.1f, 50.0f);

	setCamera();

	drawAxis();
	//DrawMeta();
	DrawKupa(kupa1);
	DrawValjak(valjak1, valjak2);

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
	CreateMeta();
	CreateKupa();
	CreateValjak();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouseClicked);
	glutKeyboardFunc(keyPressed);
	glutTimerFunc(1000 / FPS, timer, 0);
	glutMainLoop();

	return 0;
}
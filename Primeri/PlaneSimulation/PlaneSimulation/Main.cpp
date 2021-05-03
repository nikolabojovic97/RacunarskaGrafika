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

Vector4D cameraPosition(50.0, 5.0, 50.0, 1.0);
Vector4D lookAt(0.0, 5.0, 0.0, 1.0);
Vector4D lookUp(0.0, 1.0, 0.0, 1.0);
Vector4D center;

Vector4D x(1.0, 0.0, 0.0, 0.0);
Vector4D y(0.0, 1.0, 0.0, 0.0);
Vector4D z(0.0, 0.0, 1.0, 0.0);

double alpha = 90;

vector<vector<Vector4D>> horizon;
vector<vector<Vector4D>> verticals;
vector<Vector4D> speedV;
vector<Vector4D> heightV;

int speed = 1;
double rotationLeftRight = 0.0;
double rotationUpDown = 0.0;

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

void drawPoly(vector<Vector4D>& poly, int mode) {
	glBegin(mode);
	for (int i = 0; i < poly.size(); i++)
		glVertex3f(poly[i].x(), poly[i].y(), poly[i].z());
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

void DrawGround() {
	glColor3f(1.0, 1.0, 1.0);
	Vector4D a(-100.0, 0.0, -100.0);
	Vector4D b(100.0, 0.0, -100.0);
	Vector4D addZ(0.0, 0.0, 1.0);

	for (int i = 0; i < 200; i++) {
		glBegin(GL_LINES);
		PushVertex(a);
		PushVertex(b);
		glEnd();
		a += addZ;
		b += addZ;
	}

	a = Vector4D(-100.0, 0.0, -100.0);
	b = Vector4D(-100.0, 0.0, 100.0);
	Vector4D addX(1.0, 0.0, 0.0);

	for (int i = 0; i < 200; i++) {
		glBegin(GL_LINES);
		PushVertex(a);
		PushVertex(b);
		glEnd();
		a += addX;
		b += addX;
	}

}

void CreateHorizon() {
	horizon.resize(5);
	for (int i = 0; i < horizon.size(); i++)
		horizon[i].resize(2);

	horizon[0][0] = Vector4D(-0.6, 0.4, 0.0);
	horizon[0][1] = Vector4D(0.6, 0.4, 0.0);

	Matrix4x4 MT;
	MT.loadTranslate(0.0, -0.2, 0.0);
	for (int i = 1; i < horizon.size(); i++)
		horizon[i] = MT.transformed(horizon[i - 1]);
}

void DrawHorizon() {
	for (int i = 0; i < horizon.size(); i++)
	{
		glBegin(GL_LINES);
		PushVertexRatio(horizon[i][0]);
		PushVertexRatio(horizon[i][1]);
		glEnd();
	}
}

void CreateVerticals() {
	verticals.resize(2);
	for (int i = 0; i < verticals.size(); i++)
		verticals[i].resize(2);
	verticals[0][0] = Vector4D(-0.7, -0.7, 0.0);
	verticals[0][1] = Vector4D(-0.7, 0.7, 0.0);
	verticals[1][0] = Vector4D(0.7, -0.7, 0.0);
	verticals[1][1] = Vector4D(0.7, 0.7, 0.0);
}

void DrawVerticals() {
	for (int i = 0; i < verticals.size(); i++)
		drawPoly(verticals[i], GL_LINES);
}

void DrawSpeed() {
	double s = scale(0.0, 10.0, -0.65, 0.65, speed);
	Vector4D a(-0.7, s, 0.0);
	Vector4D b(-0.65, s, 0.0);
	glBegin(GL_LINES);
	PushVertex(a);
	PushVertex(b);
	glEnd();
}

void DrawHeight() {
	double h = cameraPosition.y();
	h = scale(0, 20, -0.65, 0.65, h);
	Vector4D a(0.65, h, 0.0);
	Vector4D b(0.7, h, 0.0);
	glBegin(GL_LINES);
	PushVertex(a);
	PushVertex(b);
	glEnd();
}

void DrawCross() {
	Vector4D a(-0.1, 0.0, 0.0);
	Vector4D b(0.1, 0.0, 0.0);
	Vector4D c(0.0, -0.05, 0.0);
	Vector4D d(0.0, 0.05, 0.0);

	glBegin(GL_LINES);
	PushVertex(a);
	PushVertex(b);
	PushVertexRatio(c);
	PushVertexRatio(d);
	glEnd();
}

void RenderString(double x, double y, void* font, double r, double g, double b) {
	glColor3f(r, g, b);
	char s[100];

	sprintf_s(s, "x = %.2f\ny = %.2f\nz = %.2f", cameraPosition.x(), cameraPosition.y(), cameraPosition.z());
	glRasterPos2f(x, y);
	glutBitmapString(font, (const unsigned char*)s);
}

void DrawInterface() {
	glColor3f(0.0, 0.5, 0.0);
	glLineWidth(3);

	DrawHorizon();
	DrawVerticals();
	DrawSpeed();
	DrawHeight();

	glColor3f(0.8, 0.0, 0.0);
	DrawCross();

	glLineWidth(1);

	RenderString(-0.9, -0.8, GLUT_BITMAP_TIMES_ROMAN_24, 1.0, 0.0, 0.0);
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_VIEWPORT);
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(alpha, ASP_RAT, 0.1, 40.0);
	setCamera();

	DrawGround();

	glMatrixMode(GL_VIEWPORT);
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();


	glDisable(GL_DEPTH_TEST);
	DrawInterface();

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

void SpeedUp() {
	if (speed < 10)
		speed++;
}

void SlowDown() {
	if (speed > 1)
		speed--;
}

void RotateLeft() {
	Vector4D f = lookAt - cameraPosition;
	f.normalize();

	if (rotationLeftRight + ROTATION_CONST < 0.25 * PI) {
		Matrix4x4 MT;
		MT.loadRotateAxis(f, ROTATION_CONST);
		lookUp = MT.transformed(lookUp);
		Transform(horizon, MT);
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
		Transform(horizon, MT);
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

void Move() {
	Vector4D f = lookAt - cameraPosition;
	f.normalize();

	f = f * speed * MOVING_CONST;
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

	//if (rotationUpDown + ROTATION_CONST < 0.25 * PI) {
		Matrix4x4 MT;
		MT.loadRotateAxis(w, -ROTATION_CONST * 0.5, cameraPosition);
		//cameraPosition = MT.transformed(cameraPosition);
		lookAt = MT.transformed(lookAt);
		Transform(horizon, MT);
		rotationUpDown += ROTATION_CONST;
	//}
}

void GoDown() {
	Vector4D f = lookAt - cameraPosition;
	Vector4D w = lookUp.crossProduct(f);
	w.normalize();

	//if (rotationUpDown - ROTATION_CONST > -0.25 * PI) {
		Matrix4x4 MT;
		MT.loadRotateAxis(w, ROTATION_CONST, cameraPosition);
		lookAt = MT.transformed(lookAt);

		MT.loadTranslate(0.0, ROTATION_CONST * 0.5, 0.0);
		Transform(horizon, MT);
		rotationUpDown -= ROTATION_CONST;
	//}
}

void keyPressed(unsigned char key, int x, int y) {
	switch (key) {
	case 'w':
		SpeedUp();
		break;
	case 's':
		SlowDown();
		break;
	case 'a':
		RotateLeft();
		break;
	case 'd':
		RotateRight();
		break;
	case '4':
		TurnLeft();
		break;
	case '6':
		TurnRight();
		break;
	case '8':
		GoUp();
		break;
	case '5':
		GoDown();
		break;
	}

	glutPostRedisplay();
}


void timer(int v) {
	Move();
	printf("%.2f %.2f %.2f\n", cameraPosition.x(), cameraPosition.y(), cameraPosition.z());
	glutTimerFunc(1000 / FPS, timer, 0);
	glutPostRedisplay();
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

	CreateHorizon();
	CreateVerticals();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouseClicked);
	glutKeyboardFunc(keyPressed);
	glutTimerFunc(1000 / FPS, timer, 0);
	glutMainLoop();

	return 0;
}
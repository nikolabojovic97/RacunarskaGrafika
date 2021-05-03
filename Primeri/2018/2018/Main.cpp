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

Vector4D cameraPosition(5.0, 1.0, 0.0, 1.0);
Vector4D lookAt(0.0, 0.0, -8.0, 1.0);
Vector4D lookUp(0.0, 1.0, 0.0, 1.0);
Vector4D center;

Vector4D x(1.0, 0.0, 0.0, 0.0);
Vector4D y(0.0, 1.0, 0.0, 0.0);
Vector4D z(0.0, 0.0, 1.0, 0.0);

double alpha = 90;

vector<vector<Vector4D>> speedometer;
vector<vector<Vector4D>> map;
vector<Vector4D> compassBase;
vector<Vector4D> compassArrow;

int speed = 1;
int maxSpeed = 5;
double phi = 0.0;
double cameraH = 1.0;

int nPoints = 50;
double r = 0.12;
double r1 = 4;
double r2 = 5;
double r3 = 6;


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

void SetCameraStart() {
	cameraPosition.m_x[0] = r2;
	cameraPosition.m_x[1] = cameraH;
	cameraPosition.m_x[2] = 0.0;

	Vector4D f = lookUp.crossProduct(cameraPosition);
	lookAt = f + cameraPosition;
}

void RenderString(double x, double y, void* font, double r, double g, double b) {
	glColor3f(r, g, b);
	char s[100];

	sprintf_s(s, "x = %.2f\ny = %.2f\nz = %.2f", cameraPosition.x(), cameraPosition.y(), cameraPosition.z());
	glRasterPos2f(x, y);
	glutBitmapString(font, (const unsigned char*)s);
}

void DrawPanel() {
	glColor3f(0.8, 0.8, 0.8);
	vector<Vector4D> poly;
	poly.resize(4);
	poly[0] = Vector4D(-1.0, -1.0, 0.0);
	poly[1] = Vector4D(1.0, -1.0, 0.0);
	poly[2] = Vector4D(1.0, -0.3, 0.0);
	poly[3] = Vector4D(-1.0, -0.3, 0.0);
	Draw(poly, GL_POLYGON);
}

void CreateSpeedometer() {
	speedometer.resize(5);
	for (int i = 0; i < speedometer.size(); i++)
		speedometer[i].resize(4);

	speedometer[0][0] = Vector4D(-0.05, -0.01, 0.0);
	speedometer[0][1] = Vector4D(0.05, -0.01, 0.0);
	speedometer[0][2] = Vector4D(0.05, 0.01, 0.0);
	speedometer[0][3] = Vector4D(-0.05, 0.01, 0.0);

	Matrix4x4 MT;
	MT.loadTranslate(0.0, 0.03, 0.0);
	for (int i = 1; i < speedometer.size(); i++)
		speedometer[i] = MT.transformed(speedometer[i - 1]);

	MT.loadTranslate(-0.8, -0.7, 0.0);
	Transform(speedometer, MT);
}

void DrawSpeed() {
	for (int i = 1; i <= speedometer.size(); i++) {
		glColor3f(0.0, 0.0, 0.0);
		if (scale(0, maxSpeed, 0, maxSpeed, speed) >= i)
			glColor3f(1.0, 0.0, 0.0);
		Draw(speedometer[i-1], GL_POLYGON);
	}
}

void CreateMap() {
	map.resize(3);
	for (int i = 0; i < map.size(); i++)
		map[i].resize(nPoints);

	double alpha = 2 * PI / nPoints;
	for (int i = 0; i < nPoints; i++)
		map[0][i] = Vector4D(cos(i * alpha) * r1, 0.0, sin(i * alpha) * r1);

	for (int i = 0; i < nPoints; i++)
		map[1][i] = Vector4D(cos(i * alpha) * r2, 0.0, sin(i * alpha) * r2);

	for (int i = 0; i < nPoints; i++)
		map[2][i] = Vector4D(cos(i * alpha) * r3, 0.0, sin(i * alpha) * r3);
}

void DrawMap() {
	glColor3f(0.0, 0.5, 0.0);
	for (int i = 0; i < map.size(); i++)
		Draw(map[i], GL_LINE_LOOP);
}

void DrawMiniMap() {
	Matrix4x4 MT, MR, MS, MT1;
	MR.loadRotateX(PI * 0.5);
	MS.loadScale(0.02, 0.02 * ASP_RAT, 0.0);
	MT1.loadTranslate(0.0, -0.65, 0.0);

	MT = MT1 * MS * MR;
	vector<vector<Vector4D>> miniMap = map;
	Transform(miniMap, MT);
	glColor3f(0.0, 0.5, 0.0);
	for (int i = 0; i < miniMap.size(); i++)
		Draw(miniMap[i], GL_LINE_LOOP);

	Vector4D position;
	position = MT.transformed(cameraPosition);

	glColor3f(1.0, 0.0, 0.0);
	glPointSize(10);
	glBegin(GL_POINTS);
	glVertex2f(position.x(), position.y());
	glEnd();
	glLineWidth(1);
}

void CreateCompassBase() {
	compassBase.resize(nPoints);
	double alpha = 2.0 * PI / nPoints;
	double rCompass = 0.13;
	for (int i = 0; i < nPoints; i++)
		compassBase[i] = Vector4D(cos(i * alpha) * rCompass, sin(i * alpha) * rCompass * ASP_RAT, 0.0);

	Matrix4x4 MT;
	MT.loadTranslate(0.6, -0.65, 0.0);
	Transform(compassBase, MT);
}

void CreateCompassArrow(double phi) {
	compassArrow.resize(4);
	compassArrow[0] = Vector4D(r * 0.3, 0.0, 0.0);
	compassArrow[1] = Vector4D(0.0, r * ASP_RAT, 0.0);
	compassArrow[2] = Vector4D(-r * 0.3, 0.0, 0.0);
	compassArrow[3] = Vector4D(0.0, -r * ASP_RAT, 0.0);

	Matrix4x4 MR;
	MR.loadRotateZ(phi);
	Transform(compassArrow, MR);
}

void DrawArrow() {
	CreateCompassArrow(phi);
	Vector4D N = compassArrow[1];
	Vector4D S = compassArrow[3];
	Matrix4x4 MT;
	MT.loadTranslate(0.6, -0.65, 0.0);


	Transform(compassArrow, MT);
	vector<Vector4D> poly;
	poly.resize(3);
	poly[0] = compassArrow[0];
	poly[1] = compassArrow[1];
	poly[2] = compassArrow[2];
	glColor3f(1.0, 0.0, 0.0);
	Draw(poly, GL_POLYGON);

	poly[0] = compassArrow[0];
	poly[1] = compassArrow[3];
	poly[2] = compassArrow[2];
	glColor3f(0.0, 0.0, 1.0);
	Draw(poly, GL_POLYGON);

	Vector4D n = N;
	n.normalize();
	N = n * N.intensity() * 1.2;

	Vector4D s = S;
	s.normalize();
	S = s * S.intensity() * 1.2;
	N.m_x[3] = 1.0;
	S.m_x[3] = 1.0;
	MT.loadTranslate(0.6, -0.67, 0.0);
	N = MT.transformed(N);
	S = MT.transformed(S);

	glColor3f(1.0, 0.0, 0.0);
	glRasterPos2f(N.x(), N.y());
	glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char*)"N");
	glColor3f(0.0, 0.0, 1.0);
	glRasterPos2f(S.x(), S.y());
	glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char*)"S");
}

void DrawCompass() {
	glColor3f(0.0, 0.0, 0.0);
	Draw(compassBase, GL_POLYGON);
	DrawArrow();
}

void DrawUI() {
	DrawPanel();
	DrawSpeed();
	RenderString(-0.5, -0.6, GLUT_BITMAP_TIMES_ROMAN_24, 1.0, 0.0, 0.0);
	DrawMiniMap();
	DrawCompass();
}


void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_VIEWPORT);
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(alpha, ASP_RAT, 0.1, 40.0);
	setCamera();

	DrawMap();

	glMatrixMode(GL_VIEWPORT);
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();


	glDisable(GL_DEPTH_TEST);
	DrawUI();

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


void MoveForward() {
	Vector4D f = lookAt - cameraPosition;
	f.normalize();

	f = f * speed * MOVING_CONST;
	Matrix4x4 MT;
	MT.loadTranslate(f);
	cameraPosition = MT.transformed(cameraPosition);
	lookAt = MT.transformed(lookAt);
}

void MoveBackward() {
	Vector4D f = lookAt - cameraPosition;
	f.normalize();

	f = -f * speed * MOVING_CONST;
	Matrix4x4 MT;
	MT.loadTranslate(f);
	cameraPosition = MT.transformed(cameraPosition);
	lookAt = MT.transformed(lookAt);
}

void MoveLeft() {
	Vector4D f = lookAt - cameraPosition;
	Vector4D w = lookUp.crossProduct(f);
	w.normalize();

	w = w * speed * MOVING_CONST;
	Matrix4x4 MT;
	MT.loadTranslate(w);
	cameraPosition = MT.transformed(cameraPosition);
	lookAt = MT.transformed(lookAt);

}

void MoveRight() {
	Vector4D f = lookAt - cameraPosition;
	Vector4D w = lookUp.crossProduct(f);
	w.normalize();

	w = -w * speed * MOVING_CONST;
	Matrix4x4 MT;
	MT.loadTranslate(w);
	cameraPosition = MT.transformed(cameraPosition);
	lookAt = MT.transformed(lookAt);
}

void TurnLeft() {

}

void TurnRight() {

}


void keyPressed(unsigned char key, int x, int y) {
	switch (key) {
	case 'w':
		MoveForward();
		break;
	case 's':
		MoveBackward();
		break;
	case 'a':
		MoveLeft();
		break;
	case 'd':
		MoveRight();
		break;
	case '4':
		TurnLeft();
		break;
	case '6':
		TurnRight();
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

	SetCameraStart();

	CreateSpeedometer();
	CreateMap();
	CreateCompassBase();
	CreateCompassArrow(phi);

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouseClicked);
	glutKeyboardFunc(keyPressed);
	glutTimerFunc(1000 / FPS, timer, 0);
	glutMainLoop();

	return 0;
}
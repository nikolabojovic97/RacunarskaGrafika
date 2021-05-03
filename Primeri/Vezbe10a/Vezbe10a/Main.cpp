#include "GL/freeglut.h"
#include "Vector4D.h"
#include "Matrix4x4.h"
#include <vector>
using namespace std;

#define PI 3.14159265359

#define MOVING_CONST 0.05;
#define ROTATION_CONST PI / 180.0

int WINDOW_HEIGHT = 800;
double ASP_RAT = 16.0 / 9.0;
int FPS = 60;

Vector4D cameraPosition(6.0, 1.0, 0.0, 1.0);
Vector4D lookAt(0.0, 0.0, -5.0, 1.0);
Vector4D lookUp(0.0, 1.0, 0.0, 0.0);
Vector4D center(0.0, 0.0, 0.0, 1.0);

Vector4D x(1.0, 0.0, 0.0, 1.0);
Vector4D y(0.0, 1.0, 0.0, 1.0);
Vector4D z(0.0, 0.0, 1.0, 1.0);

double alpha = 90;

vector<vector<Vector4D>> speedometer;
vector<Vector4D> coordinateSystem;
vector<Vector4D> compassArrow;
Vector4D N;
Vector4D S;
vector<vector<Vector4D>> map;


int speed = 0;
double rCompass = 0.12;
vector<double> r = { 15.0, 16.0, 17.0 };
double nPoints = 50;
double rotationUpDown = 0.0;
double rotationLeftRight = 0.0;


double Scale(double min, double max, double a, double b, double x) {
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

void SetCamera() {
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

void DrawPlaneWindow() {
	vector<Vector4D> planeWindow;
	planeWindow.resize(4);

	double xmax = 1.0;
	double ymax = 1.0;
	double side = 0.1;
	double bottom = 0.5;
	planeWindow[0] = Vector4D(-xmax + xmax * side, -ymax + ymax * bottom, 0.0);
	planeWindow[1] = Vector4D(xmax - xmax * side, -ymax + ymax * bottom, 0.0);
	planeWindow[2] = Vector4D(xmax - xmax * side, ymax - ymax * side, 0.0);
	planeWindow[3] = Vector4D(-xmax + xmax * side, ymax - ymax * side, 0.0);

	vector<Vector4D> poly;
	poly.resize(4);
	poly[0] = planeWindow[0];
	poly[1] = planeWindow[1];
	poly[2] = Vector4D(xmax, -ymax, 0.0);
	poly[3] = Vector4D(-xmax, -ymax, 0.0);
	glColor3f(0.5, 0.5, 0.5);
	Draw(poly, GL_POLYGON);

	poly[0] = planeWindow[1];
	poly[1] = planeWindow[2];
	poly[2] = Vector4D(xmax, ymax, 0.0);
	poly[3] = Vector4D(xmax, -ymax, 0.0);
	glColor3f(0.8, 0.8, 0.8);
	Draw(poly, GL_POLYGON);

	poly[0] = planeWindow[2];
	poly[1] = planeWindow[3];
	poly[2] = Vector4D(-xmax, ymax, 0.0);
	poly[3] = Vector4D(xmax, ymax, 0.0);
	Draw(poly, GL_POLYGON);

	poly[0] = planeWindow[3];
	poly[1] = planeWindow[0];
	poly[2] = Vector4D(-xmax, -ymax, 0.0);
	poly[3] = Vector4D(-xmax, ymax, 0.0);
	Draw(poly, GL_POLYGON);
}

void RenderString(double x, double y, void* font, double r, double b, double g) {
	glColor3f(r, g, b);
	char s[100];

	sprintf_s(s, "x = %.2f\ny = %.2f\nz = %.2f", cameraPosition.x(), cameraPosition.y(), cameraPosition.z());
	glRasterPos2f(x, y);
	glutBitmapString(font, (const unsigned char*)s);
}

void CreateSpeedometer() {
	speedometer.resize(5);
	for (int i = 0; i < speedometer.size(); i++)
		speedometer[i].resize(4);

	double xmax = 0.05;
	double ymax = 0.01;
	speedometer[0][0] = Vector4D(-xmax, -ymax, 0.0);
	speedometer[0][1] = Vector4D(xmax, -ymax, 0.0);
	speedometer[0][2] = Vector4D(xmax, ymax, 0.0);
	speedometer[0][3] = Vector4D(-xmax, ymax, 0.0);

	Matrix4x4 MT;
	MT.loadTranslate(0.0, 0.03, 0.0);

	for (int i = 1; i < 5; i++)
		speedometer[i] = MT.transformed(speedometer[i - 1]);

	MT.loadTranslate(-0.55, -0.8, 0.0);
	Transform(speedometer, MT);
}

void DrawSpeedometer() {
	for (int i = 1; i <= speedometer.size(); i++) {
		glColor3f(0.0, 0.0, 0.0);
		if(Scale(0.0, 5.0, 0.0, 5.0, speed) >= i)
			glColor3f(1.0, 0.0, 0.0);
		Draw(speedometer[i-1], GL_POLYGON);
	}
}

void CreateCoordinateSystem() {
	coordinateSystem.resize(4);

	coordinateSystem[0] = center;
	coordinateSystem[1] = x * 0.15;
	coordinateSystem[2] = y * 0.15;
	coordinateSystem[3] = z * 0.15;

	for (int i = 0; i < coordinateSystem.size(); i++)
		coordinateSystem[i].m_x[3] = 1.0;

	Matrix4x4 MT;
	MT.loadTranslate(-0.3, -0.47, 0.0);
	Transform(coordinateSystem, MT);
}

void DrawCoordinateSystem() {
	glBegin(GL_LINES);
	glColor3f(1.0, 0.0, 0.0);
	PushVertexRatio(coordinateSystem[0]);
	PushVertexRatio(coordinateSystem[1]);

	glColor3f(0.0, 1.0, 0.0);
	PushVertexRatio(coordinateSystem[0]);
	PushVertexRatio(coordinateSystem[2]);

	glColor3f(0.0, 0.0, 1.0);
	PushVertexRatio(coordinateSystem[0]);
	PushVertexRatio(coordinateSystem[3]);
	glEnd();
}

void DrawCompassBase() {
	vector<Vector4D> compassBase;
	compassBase.resize(nPoints);
	double alpha = 2.0 * PI / nPoints;

	for (int i = 0; i < nPoints; i++)
		compassBase[i] = Vector4D(cos(i * alpha) * rCompass, sin(i * alpha) * rCompass * ASP_RAT, 0.0);

	Matrix4x4 MT;
	MT.loadTranslate(0.2, -0.75, 0.0);
	compassBase = MT.transformed(compassBase);
	glColor3f(0.0, 0.0, 0.0);
	Draw(compassBase, GL_POLYGON);
}

void CreateCompassArrow() {
	compassArrow.resize(5);

	compassArrow[0] = Vector4D(rCompass * 0.3, 0.0, 0.0);
	compassArrow[1] = Vector4D(0.0, rCompass * ASP_RAT, 0.0);
	compassArrow[2] = Vector4D(-rCompass * 0.3, 0.0, 0.0);
	compassArrow[3] = Vector4D(0.0, -rCompass * ASP_RAT, 0.0);
	compassArrow[4] = Vector4D(0.0, 0.0, 0.0, 1.0);

	Vector4D n = compassArrow[1];
	n.normalize();
	N = n * compassArrow[1].intensity() * 1.1;

	Vector4D s = compassArrow[3];
	s.normalize();
	S = s * compassArrow[3].intensity() * 1.1;

	Matrix4x4 MT;
	MT.loadTranslate(0.2, -0.75, 0.0);
	compassArrow = MT.transformed(compassArrow);

	MT.loadTranslate(0.18, -0.69, 0.0);
	N = MT.transformed(N);
	S = MT.transformed(S);
}

void DrawCompassArrow() {
	vector<Vector4D> poly;
	poly.resize(3);

	poly[0] = compassArrow[0];
	poly[1] = compassArrow[1];
	poly[2] = compassArrow[2];
	glColor3f(0.0, 0.0, 1.0);
	Draw(poly, GL_POLYGON);
	glRasterPos2f(N.x(), N.y());
	glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_10, (const unsigned char*)"N");

	poly[0] = compassArrow[0];
	poly[1] = compassArrow[2];
	poly[2] = compassArrow[3];
	glColor3f(1.0, 0.0, 0.0);
	Draw(poly, GL_POLYGON);
	glRasterPos2f(S.x(), S.y());
	glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_10, (const unsigned char*)"S");
}

void CreateMap() {
	map.resize(3);
	for (int i = 0; i < map.size(); i++)
		map[i].resize(nPoints);
	
	double alpha = 2.0 * PI / nPoints;
	for (int i = 0; i < r.size(); i++)
		for (int j = 0; j < nPoints; j++)
			map[i][j] = Vector4D(cos(j * alpha) * r[i], 0.0, sin(j * alpha) * r[i]);
}

void DrawMap() {
	glLineWidth(3);
	glColor3f(0.0, 0.6, 0.0);
	for (int i = 0; i < map.size(); i++)
		Draw(map[i], GL_LINE_LOOP);
	glLineWidth(1);
}

void DrawMiniMap() {
	vector<vector<Vector4D>> miniMap = map;
	Matrix4x4 MT, MR, MS, MT1;
	MR.loadRotateX(0.5 * PI);
	MS.loadScale(0.0075, 0.0075 * ASP_RAT, 1.0);
	MT1.loadTranslate(0.6, -0.75, 0.0);

	MT = MT1 * MS * MR;
	Transform(miniMap, MT);
	glColor3f(0.0, 0.4, 0.0);
	glLineWidth(3.0);
	for (int i = 0; i < miniMap.size(); i++)
		Draw(miniMap[i], GL_LINE_LOOP);
	glLineWidth(1.0);

	Vector4D position = MT.transformed(cameraPosition);
	glPointSize(10.0);
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_POINTS);
	glVertex2f(position.x(), position.y());
	glEnd();
}

void SetUpCamera() {
	cameraPosition.m_x[0] = r[1];
	cameraPosition.m_x[1] = 1.0;
	cameraPosition.m_x[2] = 0.0;

	Vector4D f = lookUp.crossProduct(cameraPosition);
	lookAt = cameraPosition + f;
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_VIEWPORT);
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(alpha, ASP_RAT, 0.1, 40.0);
	SetCamera();

	DrawMap();

	glMatrixMode(GL_VIEWPORT);
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();


	glDisable(GL_DEPTH_TEST);
	DrawPlaneWindow();
	RenderString(-0.8, -0.7, GLUT_BITMAP_TIMES_ROMAN_24, 1.0, 0.0, 0.0);
	DrawSpeedometer();
	DrawCoordinateSystem();
	DrawCompassBase();
	DrawCompassArrow();
	DrawMiniMap();

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

	SetCamera();
}

void mouseClicked(int button, int state, int x, int y) {

}

void Move() {
	Vector4D f = lookAt - cameraPosition;
	f.normalize();

	f = f * speed * MOVING_CONST;
	Matrix4x4 MT;
	MT.loadTranslate(f);
	cameraPosition = MT.transformed(cameraPosition);
	lookAt = MT.transformed(lookAt);
}

void RotateLeft() {
	Vector4D f = lookAt - cameraPosition;
	f.normalize();

	Matrix4x4 MT;
	MT.loadRotateAxis(f, ROTATION_CONST);
	lookUp = MT.transformed(lookUp);

	MT.loadRotateAxis(f, -ROTATION_CONST, coordinateSystem[0]);
	Transform(coordinateSystem, MT);
}

void RotateRight() {
	Vector4D f = lookAt - cameraPosition;
	f.normalize();

	Matrix4x4 MT;
	MT.loadRotateAxis(f, -ROTATION_CONST);
	lookUp = MT.transformed(lookUp);

	MT.loadRotateAxis(f, ROTATION_CONST, coordinateSystem[0]);
	Transform(coordinateSystem, MT);
}

void GoUp() {
	Vector4D f = lookAt - cameraPosition;
	Vector4D w = lookUp.crossProduct(f);
	w.normalize();

	if (rotationUpDown + ROTATION_CONST < 0.3 * PI) {
		Matrix4x4 MT;
		MT.loadRotateAxis(w, -ROTATION_CONST, cameraPosition);
		lookAt = MT.transformed(lookAt);

		Vector4D LookUp_vector4d = lookUp;
		LookUp_vector4d.m_x[3] = 0.0;
		LookUp_vector4d = MT.transformed(LookUp_vector4d);
		lookUp = LookUp_vector4d;

		MT.loadRotateAxis(w, ROTATION_CONST, coordinateSystem[0]);
		Transform(coordinateSystem, MT);
		rotationUpDown += ROTATION_CONST;
	}
}

void GoDown() {
	Vector4D f = lookAt - cameraPosition;
	Vector4D w = lookUp.crossProduct(f);
	w.normalize();

	if (rotationUpDown - ROTATION_CONST > -0.3 * PI) {
		Matrix4x4 MT;
		MT.loadRotateAxis(w, ROTATION_CONST, cameraPosition);
		lookAt = MT.transformed(lookAt);

		Vector4D LookUp_vector4d = lookUp;
		LookUp_vector4d.m_x[3] = 0.0;
		LookUp_vector4d = MT.transformed(LookUp_vector4d);
		lookUp = LookUp_vector4d;

		MT.loadRotateAxis(w, -ROTATION_CONST, coordinateSystem[0]);
		Transform(coordinateSystem, MT);
		rotationUpDown -= ROTATION_CONST;
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

	MT.loadRotateAxis(lookUp, -ROTATION_CONST, coordinateSystem[0]);
	Transform(coordinateSystem, MT);

	MT.loadRotateAxis(z, -ROTATION_CONST, compassArrow[4]);
	Transform(compassArrow, MT);
}

void TurnRight() {
	Vector4D f = lookAt - cameraPosition;


	Matrix4x4 MT;
	MT.loadRotateAxis(lookUp, -ROTATION_CONST, cameraPosition);
	lookAt = MT.transformed(lookAt);

	MT.loadRotateAxis(lookUp, ROTATION_CONST, coordinateSystem[0]);
	Transform(coordinateSystem, MT);

	MT.loadRotateAxis(z, ROTATION_CONST, compassArrow[4]);
	Transform(compassArrow, MT);
}

void SpeedUp() {
	if (speed < 5)
		speed++;
}

void SlowDown() {
	if (speed > 1)
		speed--;
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
	glutCreateWindow("Kolokvijum");

	init();

	CreateSpeedometer();
	CreateCoordinateSystem();
	CreateCompassArrow();
	CreateMap();
	SetUpCamera();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouseClicked);
	glutKeyboardFunc(keyPressed);
	glutTimerFunc(1000 / FPS, timer, 0);
	glutMainLoop();

	return 0;
}
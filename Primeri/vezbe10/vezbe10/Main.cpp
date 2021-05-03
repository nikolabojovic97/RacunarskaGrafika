#include "GL/freeglut.h"
#include "Vector4D.h"
#include "Matrix4x4.h"
#include <vector>
using namespace std;


#define PI 3.14159265359

#define MOVING_CONST 0.1
#define ROTATION_CONST PI / 180.0
#define LOOK_MOVEMENT_COSNT 0.1

int WINDOW_HEIGHT = 800;
double ASP_RAT = 16.0 / 9.0;
int FPS = 60;

Vector4D cameraPosition(15.0, 0.0, 15.0, 1.0);
Vector4D lookAt(0.0, 0.0, 0.0, 1.0);
Vector4D lookUp(0.0, 1.0, 0.0, 1.0);
Vector4D center(0.0, 0.0, 0.0, 1.0);

Vector4D x(1.0, 0.0, 0.0, 1.0);
Vector4D y(0.0, 1.0, 0.0, 1.0);
Vector4D z(0.0, 0.0, 1.0, 1.0);

vector<Vector4D> cube;
vector<Vector4D> coordinateSystem;
vector<Vector4D> planePanel;
vector<Vector4D> compassBase;
vector<Vector4D> compassArrow;
vector<vector<Vector4D>> speedometer;

double alpha = 90;
double phi = 0.0;
double rCompass = 0.1;
double rCompassReal = 5.0;
int circlePointsCompass = 50;

int speedIndicator = 1;
double arrowRotate = 0.0;
double rotationUpDown = 0.0;

void Transform(vector<Vector4D>& A, Matrix4x4 MT) {
	for (int i = 0; i < A.size(); i++)
		A[i] = MT.transformed(A[i]);
}

void DrawPoly(vector<Vector4D>& poly, int mode) {
	glBegin(mode);
	for (int i = 0; i < poly.size(); i++)
		glVertex3f(poly[i].x(), poly[i].y(), poly[i].z());
	glEnd();
}

void setCamera() {
	/*glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();*/
	gluLookAt(cameraPosition.x(), cameraPosition.y(), cameraPosition.z(),
		lookAt.x(), lookAt.y(), lookAt.z(),
		lookUp.x(), lookUp.y(), lookUp.z());
}

void CreateCube(double a) {
	cube.resize(8);

	double b = a / 2.0;

	cube[0] = Vector4D(-b, -b, -b, 1.0);
	cube[1] = Vector4D(-b, b, -b, 1.0);
	cube[2] = Vector4D(b, b, -b, 1.0);
	cube[3] = Vector4D(b, -b, -b, 1.0);
	cube[4] = Vector4D(-b, -b, b, 1.0);
	cube[5] = Vector4D(-b, b, b, 1.0);
	cube[6] = Vector4D(b, b, b, 1.0);
	cube[7] = Vector4D(b, -b, b, 1.0);
}

void DrawCube() {
	vector<Vector4D> poly;
	poly.resize(4);

	poly[0] = cube[0];
	poly[1] = cube[1];
	poly[2] = cube[2];
	poly[3] = cube[3];
	glColor3f(0.0, 0.5, 0.0);
	DrawPoly(poly, GL_POLYGON);
	glColor3f(0.8, 0.0, 0.0);
	DrawPoly(poly, GL_LINE_LOOP);

	poly[0] = cube[4];
	poly[1] = cube[5];
	poly[2] = cube[6];
	poly[3] = cube[7];
	glColor3f(0.0, 0.5, 0.0);
	DrawPoly(poly, GL_POLYGON);
	glColor3f(0.8, 0.0, 0.0);
	DrawPoly(poly, GL_LINE_LOOP);


	poly[0] = cube[0];
	poly[1] = cube[1];
	poly[2] = cube[5];
	poly[3] = cube[4];
	glColor3f(0.0, 0.5, 0.0);
	DrawPoly(poly, GL_POLYGON);
	glColor3f(0.8, 0.0, 0.0);
	DrawPoly(poly, GL_LINE_LOOP);


	poly[0] = cube[1];
	poly[1] = cube[2];
	poly[2] = cube[6];
	poly[3] = cube[5];
	glColor3f(0.0, 0.5, 0.0);
	DrawPoly(poly, GL_POLYGON);
	glColor3f(0.8, 0.0, 0.0);
	DrawPoly(poly, GL_LINE_LOOP);

	poly[0] = cube[2];
	poly[1] = cube[3];
	poly[2] = cube[7];
	poly[3] = cube[6];
	glColor3f(0.0, 0.5, 0.0);
	DrawPoly(poly, GL_POLYGON);
	glColor3f(0.8, 0.0, 0.0);
	DrawPoly(poly, GL_LINE_LOOP);

	poly[0] = cube[3];
	poly[1] = cube[0];
	poly[2] = cube[4];
	poly[3] = cube[7];
	glColor3f(0.0, 0.5, 0.0);
	DrawPoly(poly, GL_POLYGON);
	glColor3f(0.8, 0.0, 0.0);
	DrawPoly(poly, GL_LINE_LOOP);
}

void CreatePlanePanel() {
	planePanel.resize(12);

	double xmax = 1;
	double ymax = 1;

	double side = 20;
	double bottom = 50;

	planePanel[0] = Vector4D(-xmax, -ymax, 0.0);
	planePanel[1] = Vector4D(xmax, -ymax, 0.0);
	planePanel[2] = Vector4D(xmax, ymax, 0.0);
	planePanel[3] = Vector4D(-xmax, ymax, 0.0);

	planePanel[4] = Vector4D(-xmax + xmax * side / 100.0, -ymax + ymax * bottom / 100, 0.0, 1.0);
	planePanel[5] = Vector4D(xmax - xmax * side / 100.0, -ymax + ymax * bottom / 100, 0.0, 1.0);
	planePanel[6] = Vector4D(xmax - xmax * side / 100.0, ymax - ymax * side / 100, 0.0, 1.0);
	planePanel[7] = Vector4D(-xmax + xmax * side / 100.0, ymax - ymax * side / 100, 0.0, 1.0);
}

void DrawPlanePanel() {
	vector<Vector4D> poly;
	poly.resize(4);

	double width = WINDOW_HEIGHT * ASP_RAT;

	glPolygonMode(GL_FRONT, GL_FILL);

	poly[0] = planePanel[0];
	poly[1] = planePanel[1];
	poly[2] = planePanel[5];
	poly[3] = planePanel[4];
	glColor3f(0.5, 0.5, 0.5);
	DrawPoly(poly, GL_POLYGON);

	poly[0] = planePanel[1];
	poly[1] = planePanel[2];
	poly[2] = planePanel[6];
	poly[3] = planePanel[5];
	glColor3f(0.3, 0.3, 0.3);
	DrawPoly(poly, GL_POLYGON);

	poly[0] = planePanel[2];
	poly[1] = planePanel[3];
	poly[2] = planePanel[7];
	poly[3] = planePanel[6];
	DrawPoly(poly, GL_POLYGON);

	poly[0] = planePanel[3];
	poly[1] = planePanel[0];
	poly[2] = planePanel[4];
	poly[3] = planePanel[7];
	DrawPoly(poly, GL_POLYGON);
}

void RenderString(double x, double y, void* font, double r, double g, double b) {
	glColor3f(r, g, b);
	glRasterPos2f(x, y);
	char s[100];

	sprintf_s(s, "x = %.2f\ny = %.2f\nz = %.2f", cameraPosition.x(), cameraPosition.y(), cameraPosition.z());
	glutBitmapString(font, (const unsigned char*)s);
}

void CreateSpeedometer() {
	speedometer.resize(5);
	for (int i = 0; i < speedometer.size(); i++) 
		speedometer[i].resize(4);

	double x = 0.05;
	double y = 0.01;

	speedometer[0][0] = Vector4D(-x, -y, 0.0, 1.0);
	speedometer[0][1] = Vector4D(x, -y, 0.0, 1.0);
	speedometer[0][2] = Vector4D(x, y, 0.0, 1.0);
	speedometer[0][3] = Vector4D(-x, y, 0.0, 1.0);

	Matrix4x4 MT;
	MT.loadTranslate(0.0, 0.025	, 0.0);

	for (int i = 1; i < speedometer.size(); i++)
		speedometer[i] = MT.transformed(speedometer[i - 1]);

	MT.loadTranslate(-0.4, -0.8, 0.0);
	for (int i = 0; i < speedometer.size(); i++)
		Transform(speedometer[i], MT);
}

void DrawSpeedometer() {
	for (int i = 0; i < speedometer.size(); i++) {
		if (i < speedIndicator)
			glColor3f(1.0, 0.0, 0.0);
		else
			glColor3f(0.0, 0.0, 0.0);
		DrawPoly(speedometer[i], GL_POLYGON);
	}
}

void CreateCoordinateSystem() {
	coordinateSystem.resize(4);

	coordinateSystem[0] = center;
	coordinateSystem[1] = Vector4D(0.2, 0.0, 0.0, 1.0);
	coordinateSystem[2] = Vector4D(0.0, 0.2, 0.0, 1.0);
	coordinateSystem[3] = Vector4D(0.0, 0.0, 0.2, 1.0);

	Matrix4x4 MT;
	MT.loadTranslate(0.0, -0.85, 0.0);
	Transform(coordinateSystem, MT);
}

void DrawCoordinateSystem() {
	glBegin(GL_LINES);
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(coordinateSystem[0].x(), coordinateSystem[0].y(), coordinateSystem[0].z());
	glVertex3f(coordinateSystem[1].x(), coordinateSystem[1].y(), coordinateSystem[1].z());

	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(coordinateSystem[0].x(), coordinateSystem[0].y(), coordinateSystem[0].z());
	glVertex3f(coordinateSystem[2].x(), coordinateSystem[2].y(), coordinateSystem[2].z());

	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(coordinateSystem[0].x(), coordinateSystem[0].y(), coordinateSystem[0].z());
	glVertex3f(coordinateSystem[3].x(), coordinateSystem[3].y(), coordinateSystem[3].z());

	glEnd();
}

void CreateCompassBase() {
	compassBase.resize(circlePointsCompass);

	double alpha = 2.0 * PI / circlePointsCompass;
	for (int i = 0; i < circlePointsCompass; i++)
		compassBase[i] = Vector4D(cos(i * alpha) * rCompass, sin(i * alpha) * rCompass * ASP_RAT, 0.0, 1.0);

	Matrix4x4 MT;
	MT.loadTranslate(0.61, -0.75, 0.0);
	Transform(compassBase, MT);
}

void DrawCompassBase() {
	glColor3f(0.0, 0.0, 0.0);
	DrawPoly(compassBase, GL_POLYGON);
}

void CreateCompassArrow(double phi) {
	compassArrow.resize(4);

	compassArrow[0] = Vector4D(rCompass * 0.3, 0.0, 0.0, 1.0);
	compassArrow[1] = Vector4D(0.0, rCompass * ASP_RAT, 0.0, 1.0);
	compassArrow[2] = Vector4D(-rCompass * 0.3, 0.0, 1.0);
	compassArrow[3] = Vector4D(0.0, -rCompass * ASP_RAT, 0.0, 1.0);

	Matrix4x4 MR;
	MR.loadRotateZ(phi);
	Transform(compassArrow, MR);

	Matrix4x4 MT;
	MT.loadTranslate(0.61, -0.75, 0.0);
	Transform(compassArrow, MT);
}

void DrawCompassArrow(double phi) {
	CreateCompassArrow(phi);
	vector<Vector4D> poly;
	poly.resize(3);

	poly[0] = compassArrow[0];
	poly[1] = compassArrow[1];
	poly[2] = compassArrow[2];
	glColor3f(0.0, 0.0, 1.0);
	DrawPoly(poly, GL_POLYGON);

	poly[0] = compassArrow[0];
	poly[1] = compassArrow[2];
	poly[2] = compassArrow[3];
	glColor3f(1.0, 0.0, 0.0);
	DrawPoly(poly, GL_POLYGON);
}

void DrawCompass() {
	DrawCompassBase();
	DrawCompassArrow(arrowRotate);

	glColor3f(0.0, 0.0, 1.0);
	glRasterPos2f(0.59, -0.55);
	glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char*)"N");

	glColor3f(1.0, 0.0, 0.0);
	glRasterPos2f(0.6, -0.99);
	glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char*)"S");
}


void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(alpha, ASP_RAT, 0.1, 30.0);

	setCamera();
	DrawCube();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glDisable(GL_DEPTH_TEST);
	DrawPlanePanel();
	RenderString(-0.8, -0.7, GLUT_BITMAP_TIMES_ROMAN_24, 1.0, 0.0, 0.0);
	DrawSpeedometer();
	DrawCoordinateSystem();
	DrawCompass();

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
	f.m_x[1] = 0.0;
	f.normalize();
	f = f * (speedIndicator * MOVING_CONST);

	Matrix4x4 MT;
	MT.loadTranslate(f);
	cameraPosition = MT.transformed(cameraPosition);
	lookAt = MT.transformed(lookAt);
}

void MoveBackward() {
	Vector4D f = lookAt - cameraPosition;
	f.m_x[1] = 0.0;
	f.normalize();	
	f = -f * (speedIndicator * MOVING_CONST);

	Matrix4x4 MT;
	MT.loadTranslate(f);
	cameraPosition = MT.transformed(cameraPosition);
	lookAt = MT.transformed(lookAt);
}

void TurnLeft() {
	Matrix4x4 MT, MT1, MT2, MR;
	MT1.loadTranslate(-cameraPosition);
	MT2.loadTranslate(cameraPosition);
	MR.loadRotateX(ROTATION_CONST);

	MT = MT2 * MR * MT1;

	lookAt = MT.transformed(lookAt);
	lookUp = MT.transformed(lookUp);

	arrowRotate += ROTATION_CONST;

	MT1.loadTranslate(-coordinateSystem[0]);
	MT2.loadTranslate(coordinateSystem[0]);
	MR.loadRotateY(ROTATION_CONST);

	MT = MT2 * MR * MT1;

	Transform(coordinateSystem, MT);
}

void TurnRight() {
	Matrix4x4 MT, MT1, MT2, MR;
	MT1.loadTranslate(-cameraPosition);
	MT2.loadTranslate(cameraPosition);
	MR.loadRotateY(-ROTATION_CONST);

	MT = MT2 * MR * MT1;

	lookAt = MT.transformed(lookAt);
	lookUp = MT.transformed(lookUp);

	arrowRotate -= ROTATION_CONST;

	MT1.loadTranslate(-coordinateSystem[0]);
	MT2.loadTranslate(coordinateSystem[0]);
	MR.loadRotateY(-ROTATION_CONST);

	MT = MT2 * MR * MT1;

	Transform(coordinateSystem, MT);
}

void LookUp() {
	Vector4D f = lookAt - cameraPosition;
	Vector4D w = lookUp.crossProduct(f);
	w.normalize();

	if (rotationUpDown + ROTATION_CONST < 0.5 * PI) {
		Matrix4x4 MT;
		MT.loadRotateAxis(w, -ROTATION_CONST, cameraPosition);
		lookAt = MT.transformed(lookAt);
		lookUp = MT.transformed(lookUp);
		rotationUpDown += ROTATION_CONST;

		Vector4D axis = coordinateSystem[1] - coordinateSystem[0];
		MT.loadRotateAxis(axis, -ROTATION_CONST, coordinateSystem[0]);
		Transform(coordinateSystem, MT);
	}
}

void LookDown() {
	Vector4D f = lookAt - cameraPosition;
	Vector4D w = lookUp.crossProduct(f);
	w.normalize();

	if (rotationUpDown - ROTATION_CONST > -0.5 * PI) {
		Matrix4x4 MT;
		MT.loadRotateAxis(w, ROTATION_CONST, cameraPosition);
		lookAt = MT.transformed(lookAt);
		lookUp = MT.transformed(lookUp);
		rotationUpDown -= ROTATION_CONST;

		Vector4D axis = coordinateSystem[1] - coordinateSystem[0];
		MT.loadRotateAxis(axis, ROTATION_CONST, coordinateSystem[0]);
		Transform(coordinateSystem, MT);
	}
}

void SpeedUp() {
	if (speedIndicator < 5)
		speedIndicator++;
}

void SpeedDown() {
	if (speedIndicator > 1)
		speedIndicator--;
}

void keyPressed(unsigned char key, int x, int y) {
	switch (key) {
	case 'w':
		printf("W pressed.\n");
		MoveForward();
		break;
	case 's':
		printf("S pressed.\n");
		MoveBackward();
		break;
	case '4':
		printf("4 pressed.\n");
		TurnLeft();
		break;
	case '6':
		printf("6 pressed.\n");
		TurnRight();
		break;
	case '8':
		printf("8 pressed.\n");
		LookUp();
		break;
	case '5':
		printf("5 pressed.\n");
		LookDown();
		break;
	case 'u':
		printf("U pressed.\n");
		SpeedUp();
		break;
	case 'j':
		printf("J pressed.\n");
		SpeedDown();
		break;
	default:
		break;
	}

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
	glutCreateWindow("Plane");

	init();

	CreateCube(5);
	CreatePlanePanel();
	CreateSpeedometer();
	CreateCoordinateSystem();
	CreateCompassBase();
	CreateCompassArrow(arrowRotate);

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouseClicked);
	glutKeyboardFunc(keyPressed);
	glutMainLoop();

	return 0;
}
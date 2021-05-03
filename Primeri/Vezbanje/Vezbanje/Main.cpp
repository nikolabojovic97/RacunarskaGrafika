#include <GL/freeglut.h>
#include <vector>
#include "Vector3D.h"
#include "Vector4D.h"
#include "Matrix4x4.h"

using namespace std;

#define PI 3.1415926

vector<vector<Vector3D>> cube;
vector<Vector3D> pyramid;
Vector3D coneCenter(0.0, 0.0, 0.0);
vector<Vector3D> cone;
vector<vector<Vector4D>> cylinder;
Vector3D rectCenter(1.0, 1.0, 0.0);
Vector4D cylinderCenter(0.0, 0.5, 0.0, 0.0);
Vector3D normalVector(0.0, 0.0, 1.0);
double a = 0.75;
double b = 0.5;
Matrix4x4 MT;

double w = 2.0;
int FPS = 30;
double alpha = w / 60;

void createCube(vector<vector<Vector3D>>& cube) {
	cube.resize(2);
	cube[0].resize(4);
	cube[1].resize(4);

	cube[0][0] = Vector3D(-1.0, -1.0, -1.0);
	cube[0][1] = Vector3D(-1.0, -1.0, 1.0);
	cube[0][2] = Vector3D(1.0, -1.0, 1.0);
	cube[0][3] = Vector3D(1.0, -1.0, -1.0);
	cube[1][0] = Vector3D(-1.0, 1.0, -1.0);
	cube[1][1] = Vector3D(-1.0, 1.0, 1.0);
	cube[1][2] = Vector3D(1.0, 1.0, 1.0);
	cube[1][3] = Vector3D(1.0, 1.0, -1.0);
}

void DrawPolygon(vector<Vector4D>& polygon)
{
	glBegin(GL_POLYGON);
	for (unsigned int i = 0; i < polygon.size(); i++)
		glVertex3d(polygon[i].x, polygon[i].y, polygon[i].z);
	glEnd();
}
void DrawPolygon(vector<Vector3D>& polygon)
{
	glBegin(GL_POLYGON);
	for (unsigned int i = 0; i < polygon.size(); i++)
		glVertex3d(polygon[i].x, polygon[i].y, polygon[i].z);
	glEnd();
}

void DrawCube(vector< vector<Vector3D> >& cube)
{
	vector<Vector3D> poly;
	poly.resize(4);
	poly[0] = cube[0][0];
	poly[1] = cube[0][1];
	poly[2] = cube[0][2];
	poly[3] = cube[0][3];
	DrawPolygon(poly);
	poly[0] = cube[1][0];
	poly[1] = cube[1][1];
	poly[2] = cube[1][2];
	poly[3] = cube[1][3];
	DrawPolygon(poly);
	for (unsigned int i = 0; i < cube[0].size() - 1; i++)
	{
		poly[0] = cube[0][i];
		poly[1] = cube[0][i + 1];
		poly[2] = cube[1][i + 1];
		poly[3] = cube[1][i];
		DrawPolygon(poly);
	}
}

void createPyramid(vector<Vector3D>& pyramid) {
	pyramid.resize(5);
	pyramid[0] = Vector3D(-1.0, 0.0, -1.0);
	pyramid[1] = Vector3D(-1.0, 0.0, 1.0);
	pyramid[2] = Vector3D(1.0, 0.0, 1.0);
	pyramid[3] = Vector3D(1.0, 0.0, -1.0);
	pyramid[4] = Vector3D(0.0, 2.0, 0.0);
}

void drawPyramid(vector<Vector3D>& pyramid) {
	vector<Vector3D> poly;
	poly.resize(4);
	poly[0] = pyramid[0];
	poly[1] = pyramid[1];
	poly[2] = pyramid[2];
	poly[3] = pyramid[3];
	DrawPolygon(poly);

	poly.resize(3);
	for (int i = 0; i < 3; i++) {
		poly[0] = pyramid[i];
		poly[1] = pyramid[i + 1];
		poly[2] = pyramid[4];
		DrawPolygon(poly);
	}

	poly[0] = pyramid[3];
	poly[1] = pyramid[0];
	poly[2] = pyramid[4];
	DrawPolygon(poly);
}

void createCone(vector<Vector3D>& cone, int n) {
	cone.resize(n + 1);
	int j = 0;
	for (int i = 0; i < 360; i += 360 / n)
		cone[j++] = Vector3D(coneCenter.x + cos(i * PI / 180), 0.0, coneCenter.z + sin(i * PI / 180));
	cone[j] = Vector3D(0.0, 1.0, 0.0);
}

void drawCone(vector<Vector3D>& cone) {
	vector<Vector3D> poly;
	poly = cone;
	poly.pop_back();
	DrawPolygon(poly);

	int size = cone.size();

	poly.empty();
	poly.resize(size - 1);
	for (int i = 0; i < size - 2; i++) {
		poly[0] = cone[0];
		poly[1] = cone[i + 1];
		poly[2] = cone[size - 1];
		DrawPolygon(poly);
	}

	poly[0] = cone[size - 2];
	poly[1] = cone[0];
	poly[2] = cone[size - 1];
	DrawPolygon(poly);
}

void drawRect(Vector3D& center, Vector3D& N, double a, double b) {
	Vector3D Y(0.0, 1.0, 0.0);
	//Vector3D A = Y.CrossProductIntensity(N) / (Y.Intensity() * N.Intensity());
}

void createCylinder(vector<vector<Vector4D>>& cylinder, int n) {
	cylinder.resize(2);
	cylinder[0].resize(n);
	cylinder[1].resize(n);
	
	int j = 0;
	for (int i = 0; i < 360; i += 360 / n) {
		cylinder[0][j] = Vector4D(cylinderCenter.x + cos(i * PI / 180), 0.0, cylinderCenter.z + sin(i * PI / 180), 0.0);
		cylinder[1][j++] = Vector4D(cylinderCenter.x + cos(i * PI / 180), 1.0, cylinderCenter.z + sin(i * PI / 180), 0.0);
	}
}

void drawCylinder(vector<vector<Vector4D>>& cylinder) {
	vector<Vector4D> poly;
	int n = cylinder[0].size();
	poly.resize(n);
	poly = cylinder[0];
	DrawPolygon(poly);
	poly = cylinder[1];
	DrawPolygon(poly);
	
	poly.empty();
	poly.resize(4);
	for (int i = 0; i < n-1; i++) {
		poly[0] = cylinder[0][i];
		poly[1] = cylinder[1][i];
		poly[2] = cylinder[1][i + 1];
		poly[3] = cylinder[0][i + 1];
		DrawPolygon(poly);
	}
	
	poly[0] = cylinder[0][n - 1];
	poly[1] = cylinder[1][n - 1];
	poly[2] = cylinder[1][0];
	poly[3] = cylinder[0][0];
	DrawPolygon(poly);
}

void createTransformationMatrix(Matrix4x4& MT, Vector4D C) {
	Matrix4x4 MTranslateCO, MTranslateOC, MRotate;
	MTranslateCO.LoadTranslate(-C.X(), -C.Y(), -C.Z());
	MTranslateOC.LoadTranslate(-C.X(), C.Y(), -C.Z());
	MRotate.LoadRotateZ(alpha);
	MT = MTranslateOC * MRotate * MTranslateCO;
}

void Transform(vector<vector<Vector4D>>& cylinder, Matrix4x4& MT) {
	int n = cylinder[0].size();
	for (int i = 0; i < n; i++) {
		cylinder[0][i] = MT * cylinder[0][i];
		cylinder[1][i] = MT * cylinder[1][i];
	}
}

void timer(int v) {
	Transform(cylinder, MT);
	glutTimerFunc(1000 / FPS, timer, v);
	glutPostRedisplay();
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);
	drawCylinder(cylinder);
	glFlush();
}
void init(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-3.0, 3.0, -3.0, 3.0, -3.0, 3.0);
}
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 80);
	glutCreateWindow("hello");
	init();
	//createCube(cube);
	//createPyramid(pyramid);
	//createCone(cone, 20);
	createCylinder(cylinder, 5);
	createTransformationMatrix(MT, cylinderCenter);
	glutDisplayFunc(display);
	glutTimerFunc(1000 / FPS, timer, 0);
	glutMainLoop();
	return 0;
}
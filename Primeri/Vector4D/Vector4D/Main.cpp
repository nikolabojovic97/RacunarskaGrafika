#include <GL/freeglut.h>
#include <vector>
#include "Vector4D.h"

using namespace std;

vector<vector<Vector4D>> cube;


void createCube(vector<vector<Vector4D>>& cube) {
	cube.resize(2);
	cube[0].resize(4);
	cube[1].resize(4);

	cube[0][0] = Vector4D(-1.0, -1.0, -1.0,0);
	cube[0][1] = Vector4D(-1.0, -1.0, 1.0,0);
	cube[0][2] = Vector4D(1.0, -1.0, 1.0,0);
	cube[0][3] = Vector4D(1.0, -1.0, -1.0,0);
	cube[1][0] = Vector4D(-1.0, 1.0, -1.0,0);
	cube[1][1] = Vector4D(-1.0, 1.0, 1.0,0);
	cube[1][2] = Vector4D(1.0, 1.0, 1.0,0);
	cube[1][3] = Vector4D(1.0, 1.0, -1.0,0);
}

void DrawPolygon(vector<Vector4D>& polygon)
{
	glBegin(GL_POLYGON);
	for (unsigned int i = 0; i < polygon.size(); i++)
		glVertex3d(polygon[i].x, polygon[i].y, polygon[i].z);
	glEnd();
}

void DrawCube(vector< vector<Vector4D> >& cube)
{
	vector<Vector4D> poly;
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


void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_POLYGON);
	glVertex3f(0.25, 0.25, 0.0);
	glVertex3f(0.75, 0.25, 0.0);
	glVertex3f(0.75, 0.75, 0.0);
	glVertex3f(0.25, 0.75, 0.0);
	glEnd();
	glFlush();
}
void init(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-2.0, 2.0, -2.0, 2.0, -2.0, 2.0);
}
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 80);
	glutCreateWindow("hello");
	init();
	createCube(cube);
	DrawCube(cube);
	glutDisplayFunc(display);
	glutMainLoop();
	return 0;
}
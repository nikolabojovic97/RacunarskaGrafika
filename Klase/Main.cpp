#include "GL/freeglut.h"
#include "Vector4D.h"
#include "Matrix4x4.h"
#include <vector>

#define PI 3.14159265359

int WINDOW_HEIGHT = 600;
double ASP_RAT = 4.0 / 3.0;
int FPS = 60;

Vector4D camera(10.0, 8.0, 10.0, 0.0);
Vector4D center;

Vector4D x(1.0, 0.0, 0.0, 0.0);
Vector4D y(0.0, 1.0, 0.0, 0.0);
Vector4D z(0.0, 0.0, 1.0, 0.0);


void drawPoly(vector<Vector4D>& poly) {
	glBegin(GL_POLYGON);
	for (int i = 0; i < poly.size(); i++)
		glVertex3f(poly[i].x, poly[i].y, poly[i].z);
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
	gluLookAt(camera.x, camera.y, camera.z,
		0.0, 0.0, 0.0,
		0.0, 1.0, 0.0);
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


	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouseClicked);
	glutKeyboardFunc(keyPressed);
	glutTimerFunc(1000 / FPS, timer, 0);
	glutMainLoop();

	return 0;
}
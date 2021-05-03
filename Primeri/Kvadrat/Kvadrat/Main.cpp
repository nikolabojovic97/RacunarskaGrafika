#include "GL/freeglut.h"
#include "Vector4D.h"
#include "Matrix4x4.h"
#include <vector>

#define PI 3.14159265359

int WINDOW_HEIGHT = 600;
double ASP_RAT = 4.0 / 3.0;
int FPS = 60;

Vector4D camera(10.0, 4.0, 10.0, 0.0);
Vector4D center;

Vector4D normalVector(1.0, 1.0, 1.0, 0.0);

Vector4D newCenter(2.0, 2.0, 2.0, 0.0);

Vector4D x(1.0, 0.0, 0.0, 0.0);
Vector4D y(0.0, 1.0, 0.0, 0.0);
Vector4D z(0.0, 0.0, 1.0, 0.0);

double d = 2.0;

vector<Vector4D> block;
vector<Vector4D> block2;

void createBlock(vector<Vector4D>& block) {
	block.resize(4);
	double phi = 2 * PI / 4.0;
	for (int i = 0; i < 4; i++)
		block[i] = Vector4D(center.x + cos(i * phi) * d, center.y, center.z + sin(i * phi) * d, 1.0);
}

void moveToNewCenter(vector<Vector4D>& block) {
	Matrix4x4 MT;
	MT.LoadTranslate(newCenter.x, newCenter.y, newCenter.z);
	MT.Print();
	for (int i = 0; i < block.size(); i++)
		block[i] = MT * block[i];
}

void setObjectToNormalVector(vector<Vector4D>& block) {
	Matrix4x4 MT;
	Vector4D v;
	v = y.CrossProduct3D(normalVector);
	double phi = acos(y * normalVector / (y.Intensity() * normalVector.Intensity()));
	MT.LoadRotateAboutAxisThroughOrigin(v, phi);
	for (int i = 0; i < block.size(); i++)
		block[i] = MT * block[i];
}

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
	glColor3f(1.0, 1.0, 1.0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	drawPoly(block);

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

void rotiraj(vector<Vector4D>& block) {
	Matrix4x4 MT;
	MT.LoadRotateZ(0.07);
	for (int i = 0; i < block.size(); i++)
		block[i] = MT * block[i];
}

void timer(int v) {
	/*rotiraj(block);
	glutTimerFunc(1000 / FPS, timer, 0);
	glutPostRedisplay();*/
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
	createBlock(block);
	createBlock(block2);
	//moveToNewCenter(block2);
	setObjectToNormalVector(block);

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouseClicked);
	glutKeyboardFunc(keyPressed);
	glutTimerFunc(1000 / FPS, timer, 0);
	glutMainLoop();

	return 0;
}
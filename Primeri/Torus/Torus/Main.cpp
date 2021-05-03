#include "GL\freeglut.h"
#include "Vector4D.h"
#include "Matrix4x4.h"
#include <vector>

double WINDOW_HEIGHT = 480;
#define PI 3.14159265359

float ASP_RAT = 4.0 / 3.0;
int FPS = 10;

Vector4D camera(10.0, 4.0, 10.0, 0.0);
Vector4D zoom_step = camera * 0.1;

Vector4D center;

vector<Vector4D> prsten;
vector<vector<Vector4D>> torus;
Matrix4x4 MT;
Matrix4x4 MTp;

double a = 3.0;
double h = 2.0;
vector<Vector4D> piramida;

double r2 = 5.0;
double r1 = 4.0;
int m = 30;
int n = 150;

void init() {
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glShadeModel(GL_FLAT);
	glEnable(GL_DEPTH_TEST);
}

void set_camera() {
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(camera.x, camera.y, camera.z,
		0.0, 0.0, 0.0,
		0.0, 1.0, 0.0);
}

void draw_axis() {
	//X
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_LINES);
	glVertex3d(0, 0, 0);
	glVertex3d(15, 0, 0);
	glEnd();

	//Y
	glColor3f(0.0, 1.0, 0.0);
	glBegin(GL_LINES);
	glVertex3d(0, 0, 0);
	glVertex3d(0, 15, 0);
	glEnd();

	//Z
	glColor3f(0.0, 0.0, 1.0);
	glBegin(GL_LINES);
	glVertex3d(0, 0, 0);
	glVertex3d(0, 0, 15);
	glEnd();
}

void create_prsten(vector<Vector4D>& prsten) {
	prsten.resize(n);
	double fi = 2 * PI / (n * 1.0);
	double r = r2 + (r2 - r1) / 2.0;
	for (int i = 0; i < n; i++)
		prsten[i] = Vector4D(center.x + cos(fi * i) * r, 0.0, center.z + sin(fi * i) * r, 0.0);
}

void draw_prsten(vector<Vector4D>& prsten) {
	glBegin(GL_POLYGON);
	for (int i = 0; i < n; i++)
		glVertex3f(prsten[i].x, prsten[i].y, prsten[i].z);
	glEnd();
}

void create_torus(vector<vector<Vector4D>>& torus, vector<Vector4D>& prsten) {
	torus.resize(n);
	double fi = 2 * PI / (m * 1.0);
	double r = (r2 - r1) / 2.0;
	for (int i = 0; i < n; i++) {
		torus[i].resize(m);
		for (int j = 0; j < m; j++)
			torus[i][j] = Vector4D(prsten[i].x + cos(j * fi) * r, prsten[i].y + sin(j * fi) * r, prsten[i].z, 0.0);
	}
}

void draw_torus(vector<vector<Vector4D>>& torus) {
	for (int i = 0; i < n; i++) {
		glBegin(GL_LINE_LOOP);
		for (int j = 0; j < m; j++)
			glVertex3f(torus[i][j].x, torus[i][j].y, torus[i][j].z);
		glEnd();
	}
}

void create_piramida(vector<Vector4D>& piramida) {
	piramida.resize(5);
	piramida[0] = Vector4D(0.0, h, 0.0, 0.0);
	double fi = 2 * PI / 4.0;
	double r = a * sqrt(2) / 2.0;
	for (int i = 1; i < 5; i++)
		piramida[i] = Vector4D(center.x + cos(i * fi) * r, 0.0, center.z + sin(i * fi) * r, 0.0);
}

void draw_piramida(vector<Vector4D>& piramida) {
	glBegin(GL_POLYGON);
	for (int i = 1; i < 5; i++)
		glVertex3f(piramida[i].x, piramida[i].y, piramida[i].z);
	glEnd();

	for (int i = 1; i < 4; i++) {
		glBegin(GL_POLYGON);
		glVertex3f(piramida[i].x, piramida[i].y, piramida[i].z);
		glVertex3f(piramida[i + 1].x, piramida[i + 1].y, piramida[i + 1].z);
		glVertex3f(piramida[0].x, piramida[0].y, piramida[0].z);
		glEnd();
	}

	glBegin(GL_POLYGON);
	glVertex3f(piramida[1].x, piramida[1].y, piramida[1].z);
	glVertex3f(piramida[4].x, piramida[4].y, piramida[4].z);
	glVertex3f(piramida[0].x, piramida[0].y, piramida[0].z);
	glEnd();
	
}

void CreateTransformMatrix(Matrix4x4& MT) {
	MT.LoadRotateAboutAxisThroughOrigin(Vector4D(0.0, 0.0, 1.0, 0.0), 0.1);
}

void CreateTransformMatrixP(Matrix4x4& MTp) {
	MTp.LoadRotateAboutAxisThroughOrigin(Vector4D(0.0, 1.0, 0.0, 0.0), 0.1);
}

void Transform(vector<vector<Vector4D>>& torus) {
	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++)
			torus[i][j] = MT * torus[i][j];
}

void TransformP(vector<Vector4D>& piramida) {
	for (int i = 0; i < 5; i++)
		piramida[i] = MTp * piramida[i];
}


void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLineWidth(1);
	draw_axis();

	glColor3f(1.0, 1.0, 1.0);
	glLineWidth(1);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	//draw_torus(torus);

	glColor3f(1.0, 1.0, 1.0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	draw_torus(torus);
	draw_piramida(piramida);

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

	set_camera();
}

void timer(int v) {
	Transform(torus);
	//TransformP(piramida);
	glutTimerFunc(1000 / FPS, timer, 0);
	glutPostRedisplay();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitWindowSize(ASP_RAT * WINDOW_HEIGHT, WINDOW_HEIGHT);
	glutInitWindowPosition(100, 80);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutCreateWindow("Torus");

	init();
	CreateTransformMatrix(MT);
	CreateTransformMatrixP(MTp);
	create_prsten(prsten);
	create_torus(torus, prsten);
	create_piramida(piramida);

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutTimerFunc(1000 / FPS, timer, 0);
	glutMainLoop();

	return 0;
}

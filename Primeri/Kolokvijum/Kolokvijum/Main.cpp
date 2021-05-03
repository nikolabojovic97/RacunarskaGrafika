#include "GL/freeglut.h"
#include "Vector4D.h"
#include "Matrix4x4.h"
#include <vector>

#define PI 3.14159265359

double ASP_RAT = 4.0 / 3.0;
int WINDOW_HEIGHT = 500;

Vector4D camera(10.0, 8.0, 10.0, 0.0);


double r = 2.0;
double R = 3.0;
double l = 3.9;
double k = 3.0;
int n = 12;
Vector4D center;
Vector4D center1(0.2, 0.5 - k, 0.2, 0.0);

Vector4D normalVector(0.0, 1.0, 0.0, 0.0);

vector<Vector4D> block;
vector<Vector4D> block2;
vector<Vector4D> gear;
vector<Vector4D>gear2;
vector<Vector4D> clearance;
vector<Vector4D> clearance2;

vector<vector<Vector4D>> blocks;
vector<vector<Vector4D>> blocks2;
vector<vector<Vector4D>> gears;
vector<vector<Vector4D>> gears2;

Matrix4x4 MR;

void Transform(vector<Vector4D> a, Matrix4x4 MT) {
	for (int i = 0; i < a.size(); i++)
		a[i] = MT * a[i];
}

void Transform(vector<vector<Vector4D>> a, Matrix4x4 MT) {
	for (int i = 0; i < a.size(); i++)
		Transform(a[i], MT);
}

void createBlock() {
	block.resize(4);
	double fi = 2 * PI / (n * 1.0);
	block[0] = Vector4D(center.x + cos(0) * r, center.y, center.z + sin(0) * r, 1.0);
	block[1] = Vector4D(center.x + cos(0) * R, center.y, center.z + sin(0) * R, 1.0);
	block[2] = Vector4D(center.x + cos(fi) * R, center.y, center.z + sin(fi) * R, 1.0);
	block[3] = Vector4D(center.x + cos(fi) * r, center.y, center.z + sin(fi) * r, 1.0);
}

void createBlock2() {
	block2.resize(4);
	Matrix4x4 MT;
	MT.LoadTranslate(0.0, k, 0.0);
	block2 = block;
	Transform(block2, MT);
}

void createBlocks() {
	blocks.resize(n+1);
	Matrix4x4 MT;
	double fi = 2 * PI / (n * 1.0);
	MT.LoadRotateAboutAxisThroughOrigin(normalVector, fi);
	blocks[0] = block;
	for (int i = 1; i <= n; i++) {
		blocks[i].resize(4);
		for (int j = 0; j < 4; j++)
			blocks[i][j] = MT * blocks[i - 1][j];
	}
}

void drawBlock(vector<Vector4D> block) {
	glColor3ub(190, 190, 190);
	glBegin(GL_POLYGON);
	for (int i = 0; i < 4; i++)
		glVertex3f(block[i].x, block[i].y, block[i].z);
	glEnd();
}

void drawBlocks(vector<vector<Vector4D>> blocks) {
	for (int i = 0; i < n; i++)
		drawBlock(blocks[i]);
}

void createGear() {
	gear.resize(4);
	double alpha = 2 * PI / (n * 2.0);
	double fi = 2 * PI / (n * 10.0);
	double nR = R - 0.2;
	gear[0] = Vector4D(center.x + cos(alpha - 4 * fi) * nR, center.y, center.z + sin(alpha - 4 * fi) * nR, 1.0);
	gear[1] = Vector4D(center.x + cos(alpha - fi) * l, center.y, center.z + sin(alpha - fi) * l, 1.0);
	gear[2] = Vector4D(center.x + cos(alpha + fi) * l, center.y, center.z + sin(alpha + fi) * l, 1.0);
	gear[3] = Vector4D(center.x + cos(alpha + 4 * fi) * nR, center.y, center.z + sin(alpha + 4 * fi) * nR, 1.0);
}

void createGear2() {
	Matrix4x4 MT;
	MT.LoadTranslate(0.0, k, 0.0);
	gear2.resize(4);
	gear2 = gear;
	Transform(gear2, MT);
}

void createGears(vector<vector<Vector4D>>& gears, vector<Vector4D> gear, Vector4D normalVector) {
	gears.resize(n+1);
	Matrix4x4 MT;
	double fi = 2 * PI / (n * 1.0);
	MT.LoadRotateAboutAxisThroughOrigin(normalVector, fi);
	gears[0] = gear;
	for (int i = 1; i <= n; i++) {
		gears[i].resize(4);
		for (int j = 0; j < 4; j++)
			gears[i][j] = MT * gears[i - 1][j];
	}
}

void drawGear(vector<Vector4D> gear) {
	glColor3ub(190, 190, 190);
	glBegin(GL_POLYGON);
	for (int i = 0; i < 4; i++)
		glVertex3f(gear[i].x, gear[i].y, gear[i].z);
	glEnd();
}

void drawGears(vector<vector<Vector4D>> gears) {
	for (int i = 0; i < n; i++)
		drawGear(gears[i]);
}

void drawInnerTube() {
	glColor3ub(215, 215, 215);
	for (int i = 0; i < n; i++) {
		glBegin(GL_POLYGON);
		glVertex3f(blocks[i][0].x, blocks[i][0].y, blocks[i][0].z);
		glVertex3f(blocks[i+1][0].x, blocks[i+1][0].y, blocks[i+1][0].z);
		glVertex3f(blocks2[i+1][0].x, blocks2[i+1][0].y, blocks2[i+1][0].z);
		glVertex3f(blocks2[i][0].x, blocks2[i][0].y, blocks2[i][0].z);
		glEnd();
	}
}

void drawOuterTube() {
	glColor3ub(205, 205, 205);
	for (int i = 0; i < n; i++) {
		glBegin(GL_POLYGON);
		glVertex3f(blocks[i][2].x, blocks[i][2].y, blocks[i][2].z);
		glVertex3f(blocks[i + 1][2].x, blocks[i + 1][2].y, blocks[i + 1][2].z);
		glVertex3f(blocks2[i + 1][2].x, blocks2[i + 1][2].y, blocks2[i + 1][2].z);
		glVertex3f(blocks2[i][2].x, blocks2[i][2].y, blocks2[i][2].z);
		glEnd();
	}
}

void drawPoly(vector<Vector4D> poly) {
	glBegin(GL_POLYGON);
	for (int i = 0; i < poly.size(); i++)
		glVertex3f(poly[i].x, poly[i].y, poly[i].z);
	glEnd();
}

void drawGears3D() {
	vector<Vector4D> poly;
	poly.resize(4);
	for (int i = 0; i < n; i++) {
		glColor3ub(200, 200, 200);
		poly.empty();
		poly[0] = gears[i][0];
		poly[1] = gears[i][1];
		poly[2] = gears2[i][1];
		poly[3] = gears2[i][0];
		drawPoly(poly);

		glColor3ub(190, 190, 190);
		poly.empty();
		poly[0] = gears[i][1];
		poly[1] = gears[i][2];
		poly[2] = gears2[i][2];
		poly[3] = gears2[i][1];
		drawPoly(poly);

		glColor3ub(195, 195, 195);
		poly.empty();
		poly[0] = gears[i][2];
		poly[1] = gears[i][3];
		poly[2] = gears2[i][3];
		poly[3] = gears2[i][2];
		drawPoly(poly);
	}
}

void draw3DModel() {
	drawBlocks(blocks);
	drawBlocks(blocks2);
	drawGears(gears);
	drawGears(gears2);
	drawInnerTube();
	drawOuterTube();
	drawGears3D();
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
	draw3DModel();

	glutSwapBuffers();
}

void setCamera() {
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(camera.x, camera.y, camera.z,
		0.0, 0.0, 0.0,
		0.0, 1.0, 0.0);
}

void init() {
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glShadeModel(GL_FLAT);
	glEnable(GL_DEPTH_TEST);
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

void createRotationMatrix(Matrix4x4& MR, Vector4D normalVector) {
	Matrix4x4 MCO, MRot, MOC;
	MCO.LoadTranslate(-center.x, -center.y, -center.z);
	MRot.LoadRotateAboutAxisThroughOrigin(normalVector, 1);
	MOC.LoadTranslate(center.x, center.y, center.z);
	MR = MOC * MRot * MCO;
}

void moveToNewCenter(vector<vector<Vector4D>>& blocks, vector<vector<Vector4D>>& blocks2, vector<vector<Vector4D>>& gears, vector<vector<Vector4D>>& gears2) {
	Matrix4x4 MT;
	MT.LoadTranslate(center1.x, center1.y, center1.z);
	for (int i = 0; i <= n; i++)
		for (int j = 0; j < 4; j++)
			blocks[i][j] = MT * blocks[i][j];
	for (int i = 0; i <= n; i++)
		for (int j = 0; j < 4; j++)
			blocks2[i][j] = MT * blocks2[i][j];
	for (int i = 0; i <= n; i++)
		for (int j = 0; j < 4; j++)
			gears[i][j] = MT * gears[i][j];
	for (int i = 0; i <= n; i++)
		for (int j = 0; j < 4; j++)
			gears2[i][j] = MT * gears2[i][j];
	glutPostRedisplay();
}


void rotiraj() {
	for (int i = 0; i < 4; i++) {
		block[i] = MR * block[i];
		gear[i] = MR * gear[i];
	}
	createBlock2();
	createGear2();
	createBlocks();
	createBlocks();
	createGears(gears, gear, normalVector);
	createGears(gears2, gear2, normalVector);
	moveToNewCenter(blocks, blocks2, gears, gears2);
	glutPostRedisplay();
}

void mouseClicked(int button, int state, int x, int y) {
	switch (button) {
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_UP)
			glutIdleFunc(rotiraj);
		break;
	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_UP)
			glutIdleFunc(NULL);
		break;
	default:
		break;
	}
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitWindowSize(WINDOW_HEIGHT * ASP_RAT, WINDOW_HEIGHT);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutCreateWindow("klk");

	init();

	createBlock();
	createBlock2();
	createGear();
	createGear2();

	createBlocks();
	createBlocks();
	createGears(gears, gear, normalVector);
	createGears(gears2, gear2, normalVector);

	moveToNewCenter(blocks, blocks2, gears, gears2);
	createRotationMatrix(MR, normalVector);

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouseClicked);
	glutMainLoop();

	return 0;
}
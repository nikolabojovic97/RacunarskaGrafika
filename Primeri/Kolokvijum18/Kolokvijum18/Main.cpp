#include "GL/freeglut.h"
#include "Vector4D.h"
#include "Matrix4x4.h"
#include <vector>

#define PI 3.14159265359
#define UGAO_MALI 0
#define UGAO_VELIKI 0

double ASP_RATIO = 16.0 / 9.0;
int WINDOW_HEIGHT = 720;

Vector4D camera(11.0, 6.0, 0.0, 0.0);

float r = 1.5;
float R = 2.25;
float l = 3.0;
float k = 2.5;
int n_zupcanika = 12;

Vector4D center;

vector<Vector4D> uKrug;
vector<Vector4D> uKrug2;
vector<Vector4D> sKrug;
vector<Vector4D> sKrug2;
vector<vector<Vector4D>> zupcanici;
vector<vector<Vector4D>> zupcanici2;
vector<Vector4D> zupcanik;
Matrix4x4 MT;
Matrix4x4 MT1;
Matrix4x4 MR;

Vector4D vNormale(0.0, 1.0, 0.0, 0.0);

void create_uKrug(vector<Vector4D>& uKrug) {
	uKrug.resize(n_zupcanika+1);
	double fi = 2 * PI / (n_zupcanika * 1.0);
	for (int i = 0; i <= n_zupcanika; i++)
		uKrug[i] = Vector4D(center.x + cos(i * fi) * r, center.y, center.z + sin(i * fi) * r, 1.0);
}

void create_sKrug(vector<Vector4D>& sKrug) {
	sKrug.resize(n_zupcanika+1);
	double fi = 2 * PI / (n_zupcanika * 1.0);
	for (int i = 0; i <= n_zupcanika; i++)
		sKrug[i] = Vector4D(center.x + cos(i * fi) * R, center.y, center.z + sin(i * fi) * R, 1.0);
}

void create_zupcanik(vector<Vector4D>& zupcanik, Matrix4x4& MT) {
	zupcanik.resize(4);
	double x = 0.2;
	double fi = 2 * PI / (n_zupcanika * 10.0);
	zupcanik[0] = Vector4D(center.x + cos(-5 * fi) * (R - x), center.y, center.z + sin(-5 * fi) * (R - x), 1.0);
	zupcanik[1] = Vector4D(center.x + cos(-fi) * l, center.y, center.z + sin(-fi) * l, 1.0);
	zupcanik[2] = Vector4D(center.x + cos(fi) * l, center.y, center.z + sin(fi) * l, 1.0);
	zupcanik[3] = Vector4D(center.x + cos(5 * fi) * (R - x), center.y, center.z + sin(5 * fi) * (R - x), 1.0);

	MT.LoadRotateAboutAxisThroughOrigin(vNormale, (360.0 / (n_zupcanika * 2.0)) * PI / 180.0);
	for (int i = 0; i < 4; i++)
		zupcanik[i] = MT * zupcanik[i];
	MT.LoadRotateAboutAxisThroughOrigin(Vector4D(0.0, 1.0, 0.0, 0.0), (360.0 / (n_zupcanika * 1.0)) * PI / 180.0);
}

vector<Vector4D> rotate_zupcanik(vector<Vector4D>& zupcanik) {
	for (int i = 0; i < 4; i++)
		zupcanik[i] = MT * zupcanik[i];
	return zupcanik;
}

void create_zupcanici(vector<vector<Vector4D>>& zupcanici) {
	zupcanici.resize(n_zupcanika);
	for (int i = 0; i < n_zupcanika; i++) {
		zupcanici[i] = zupcanik;
		zupcanik = rotate_zupcanik(zupcanik);
	}
}


void draw_usKrug(vector<Vector4D>& uKrug, vector<Vector4D>& sKrug) {
	glBegin(GL_QUAD_STRIP);
	int i;
	for (i = 0; i <= n_zupcanika; i++) {
		glVertex3f(uKrug[i].x, uKrug[i].y, uKrug[i].z);
		glVertex3f(sKrug[i].x, sKrug[i].y, sKrug[i].z);
		printf("%.2f %.2f %.2f\n", sKrug[i].x, sKrug[i].y, sKrug[i].z);
	}
	printf("\n\n");
	glEnd();
}

void draw_zupcanik(vector<Vector4D> zupcanik) {
	glBegin(GL_POLYGON);
	for (int i = 0; i < 4; i++)
		glVertex3f(zupcanik[i].x, zupcanik[i].y, zupcanik[i].z);
	glEnd();
}

void draw_zupcanici(vector<vector<Vector4D>> zupcanici) {
	for (int i = 0; i < n_zupcanika; i++)
		draw_zupcanik(zupcanici[i]);
}

void create_trans_matrix(Matrix4x4& MT1) {
	MT1.LoadTranslate(0.0, k, 0.0);
}

void create_uKrug2(vector<Vector4D>& uKrug2) {
	uKrug2.resize(n_zupcanika + 1);
	for (int i = 0; i <= n_zupcanika; i++)
		uKrug2[i] = MT1 * uKrug[i];
}

void create_sKrug2(vector<Vector4D>& sKrug2) {
	sKrug2.resize(n_zupcanika+1);
	for (int i = 0; i <= n_zupcanika; i++)
		sKrug2[i] = MT1 * sKrug[i];
}

void create_zupcanici2(vector<vector<Vector4D>>& zupcanici2) {
	zupcanici2.resize(n_zupcanika);
	for (int i = 0; i < n_zupcanika; i++) {
		zupcanici2[i].resize(4);
		for (int j = 0; j < 4; j++)
			zupcanici2[i][j] = MT1 * zupcanici[i][j];
	}
}

void draw_zupcanik3D() {
	draw_usKrug(uKrug, sKrug);
	draw_zupcanici(zupcanici);

	draw_usKrug(uKrug2, sKrug2);
	draw_zupcanici(zupcanici2);

	glColor3ub(220, 220, 220);
	for (int i = 0; i < n_zupcanika; i++){ 
		glBegin(GL_POLYGON);
		glVertex3f(uKrug[i].x, uKrug[i].y, uKrug[i].z);
		glVertex3f(uKrug[i+1].x, uKrug[i+1].y, uKrug[i+1].z);
		glVertex3f(uKrug2[i+1].x, uKrug2[i+1].y, uKrug2[i+1].z);
		glVertex3f(uKrug2[i].x, uKrug2[i].y, uKrug2[i].z);
		glEnd();
	}
	
	glColor3ub(185, 185, 185);
	for (int i = 0; i < n_zupcanika; i++) {
		glBegin(GL_POLYGON);
		glVertex3f(sKrug[i].x, sKrug[i].y, sKrug[i].z);
		glVertex3f(sKrug[i + 1].x, sKrug[i + 1].y, sKrug[i + 1].z);
		glVertex3f(sKrug2[i + 1].x, sKrug2[i + 1].y, sKrug2[i + 1].z);
		glVertex3f(sKrug2[i].x, sKrug2[i].y, sKrug2[i].z);
		glEnd();
	}	

	
	for (int i = 0; i < n_zupcanika; i++) {
		for (int j = 0; j < 3; j++) {
			if (j == 1)
				glColor3ub(190, 190, 190);
			else
				glColor3ub(200, 200, 200);
			glBegin(GL_POLYGON);
			glVertex3f(zupcanici[i][j].x, zupcanici[i][j].y, zupcanici[i][j].z);
			glVertex3f(zupcanici2[i][j].x, zupcanici2[i][j].y, zupcanici2[i][j].z);
			glVertex3f(zupcanici2[i][j+1].x, zupcanici2[i][j+1].y, zupcanici2[i][j+1].z);
			glVertex3f(zupcanici[i][j+1].x, zupcanici[i][j+1].y, zupcanici[i][j+1].z);
			glEnd();
		}	
	}
}


void draw_axis() {
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

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLineWidth(1);
	draw_axis();

	glColor3ub(132, 138, 138);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	draw_zupcanik3D();

	glutSwapBuffers();
}

void reshape(int w, int h) {
	if (w < h)
		WINDOW_HEIGHT = w / ASP_RATIO;
	else
		WINDOW_HEIGHT = h;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40.0, ASP_RATIO, 0.1, 50.0);

	set_camera();
}

void create_rotation_matrix(Matrix4x4& MR) {
	MR.LoadRotateAboutAxisThroughOrigin(vNormale, 0.07);
}

void rotiraj() {
	for (int i = 0; i <= n_zupcanika; i++)
		uKrug[i] = MR * uKrug[i];

	for (int i = 0; i <= n_zupcanika; i++)
		uKrug2[i] = MR * uKrug2[i];

	for (int i = 0; i <= n_zupcanika; i++)
		sKrug[i] = MR * sKrug[i];

	for (int i = 0; i <= n_zupcanika; i++)
		sKrug2[i] = MR * sKrug2[i];

	for (int i = 0; i < n_zupcanika; i++) {
		for (int j = 0; j < 4; j++)
			zupcanici[i][j] = MR * zupcanici[i][j];
	}

	for (int i = 0; i < n_zupcanika; i++) {
		for (int j = 0; j < 4; j++)
			zupcanici2[i][j] = MR * zupcanici2[i][j];
	}
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
	glutInitWindowSize(ASP_RATIO * WINDOW_HEIGHT, WINDOW_HEIGHT);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutCreateWindow("Kolokvijum");

	init();

	create_trans_matrix(MT1);
	create_rotation_matrix(MR);
	create_uKrug(uKrug);
	create_sKrug(sKrug);
	create_zupcanik(zupcanik, MT);
	create_zupcanici(zupcanici);

	create_uKrug2(uKrug2);
	create_sKrug2(sKrug2);
	create_zupcanici2(zupcanici2);

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouseClicked);
	glutMainLoop();

	return 0;
}
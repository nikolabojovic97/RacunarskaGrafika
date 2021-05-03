#include <GL/freeglut.h>
#include "Vector2D.h"
#include <vector>

#define WINDOW_WIDTH 700
#define WINDOW_HEIGHT 500
#define PI 3.1415926

int i = 0;

void nacrtajPoligon(double a, double fi, Vector2D &A) {
	Vector2D ABnorm(cos(fi), sin(fi));
	Vector2D ADnorm(cos(PI / 2 + fi), sin(PI / 2 + fi));

	Vector2D AB = ABnorm * a;
	Vector2D AD = ADnorm * a;

	Vector2D AC = AB + AD;

	Vector2D xAxis(1.0, 0.0);

	double shadow = AB.ProjectionVector(xAxis).Intensity() + AD.ProjectionVector(xAxis).Intensity();

	cout << "Vektorski: Senka ce biti duzine " << shadow << "\n";
	cout << "Povrsina poligona: " << fabs(AB.CrossProductIntensity(AD)) << "\n";

	Vector2D B = A + AB;
	Vector2D C = A + AC;
	Vector2D D = A + AD;

	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_POLYGON);

	glVertex2f(A.x, A.y);
	glVertex2f(B.x, B.y);
	glVertex2f(C.x, C.y);
	glVertex2f(D.x, D.y);

	glEnd();
}

void nacrtajPoligonCentar(double a, double fi, Vector2D& O) {
	double d = a * sqrt(2);

	Vector2D OAnorm(cos(PI / 4 + fi), sin(PI / 4 + fi));
	Vector2D OCnorm = OAnorm * (-1);
	Vector2D OBnorm = OAnorm.Transpose();
	Vector2D ODnorm = OBnorm * (-1);

	Vector2D OA = OAnorm * (d / 2);
	Vector2D OC = OCnorm * (d / 2);
	Vector2D OB = OBnorm * (d / 2);
	Vector2D OD = ODnorm * (d / 2);

	Vector2D A = O + OA;
	Vector2D B = O + OB;
	Vector2D C = O + OC;
	Vector2D D = O + OD;

	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_POLYGON);

	glVertex2f(A.x, A.y);
	glVertex2f(B.x, B.y);
	glVertex2f(C.x, C.y);
	glVertex2f(D.x, D.y);

	glEnd();
}

void drawPolygon(vector<Vector2D>& points, int n) {
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_POLYGON);

	for (int i = 0; i < n; i++)
		glVertex2f(points[i].x, points[i].y);

	double p = 0.0;
	for (int i = 1; i < n - 1; i++) {
		Vector2D v1(points[i] - points[0]);
		Vector2D v2(points[i + 1] - points[0]);
		p += (0.5 * fabs(v1.CrossProductIntensity(v2)));
	}
	cout << "Povrsina poligona: " << p << "\n";
	glEnd();
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	Vector2D A(120.0, 120.0);
	
	vector<Vector2D> points[5];
	points->push_back(Vector2D(10, 10));
	points->push_back(Vector2D(100, 10));
	points->push_back(Vector2D(120, 50));
	points->push_back(Vector2D(80, 100));
	points->push_back(Vector2D(40, 40));

	drawPolygon(*points, 5);

	glFlush();
	glutPostRedisplay();
}

void init(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, WINDOW_WIDTH, 0.0, WINDOW_HEIGHT, -1.0, 1.0);
}


int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInitWindowPosition(100, 80);
	glutCreateWindow("hello");
	init();
	glutDisplayFunc(display);
	glutMainLoop();
	return 0;
}
#include <GL/freeglut.h>
#include <math.h>
#include <iostream>
using namespace std;

#define PI 3.14159265

class Tacka {
public:
	double x;
	double y;
	double z;
};

float udaljenostTacaka(Tacka a, Tacka b) {
	return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_POLYGON);

	double d = 100.0;

	Tacka t1;
	t1.x = 100.0;
	t1.y = 100.0;
	t1.z = 0.0;

	Tacka t2;
	t2.x = t1.x + d;
	t2.y = t1.y;
	t2.z = 0.0;


	double b = d / (sin(36 * PI / 180) * 2);
	double h = sqrt(b * b - pow(d / 2, 2));
	cout << b;
	cout << "\n";
	cout << h;

	Tacka t3;
	t3.x = t1.x + d / 2;
	t3.y = t1.y + h;
	t3.z = 0;

	double lB = b * sin(18 * PI / 180);
	double lD = d * sin(18 * PI / 180);
	Tacka t4;
	t4.x = t2.x + lD;
	t4.y = t3.y + lB;
	t4.z = 0;

	Tacka t5;
	t5.x = t1.x + d / 2;
	t5.y = t3.y + b;
	t5.z = 0.0;

	Tacka t6;
	t6.x = t1.x - lD;
	t6.y = t4.y;
	t6.z = 0.0;

	glVertex3f(t1.x, t1.y, t1.z);
	glVertex3f(t2.x, t2.y, t2.z);
	//glVertex3f(t3.x, t3.y, t3.z);
	glVertex3f(t4.x, t4.y, t4.z);
	glVertex3f(t5.x, t5.y, t5.z);
	glVertex3f(t6.x, t6.y, t6.z);
								 

	glEnd();
	glFlush();
}
void init(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, 500, 0.0, 500.0, 1.0, -1.0);
}
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 80);
	glutCreateWindow("hello");
	init(); 
	glutDisplayFunc(display);
	glutMainLoop();
	return 0;
}
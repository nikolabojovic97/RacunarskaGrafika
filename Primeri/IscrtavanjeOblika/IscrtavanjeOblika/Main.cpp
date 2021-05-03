#include <GL/freeglut.h>
#include <math.h>

#define WINDOW_WIDTH 500
#define WINDOW_HEIGHT 500
#define PI 3.1415926

class Tacka {
public:
	float x;
	float y;
	float z;
};

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_POLYGON);
	
	float r = 100.0;

	Tacka o;
	o.x = 150.0;
	o.y = 150.0;
	o.z = 0.0;
	
	for (int i = 0; i < 360; i+=1)
		glVertex3f(o.x + cos(i * PI / 180) * r, o.y + sin(i * PI / 180) * r, 0.0);

	glEnd();
	glFlush();
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
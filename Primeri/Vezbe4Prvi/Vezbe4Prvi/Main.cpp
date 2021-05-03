#include <GL/freeglut.h>
#include <math.h>

#define PI 3.1415926
#define WINDOW_WIDTH 700
#define WINDOW_HEIGHT 500

class Point {
public:
	float x;
	float y;
	float z;
	Point(float x, float y) {
		this->x = x;
		this->y = y;
		z = 0;
	}
};

void drawRect()

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);



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
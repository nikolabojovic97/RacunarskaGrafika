#include <GL/freeglut.h>
#include "Vector2D.h"

#define WINDOW_WIDTH 1500
#define WINDOW_HEIGHT 800
#define PI 3.1415926
#define FPS 60

Vector2D O(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
double r = 50.0;
double l = 120.0;
int n = 5;
double phi = 0.0;

int direction = 1;

void drawStar(Vector2D O, double r, double l, int n, double phi) {
	glColor3f(1.0, 1.0, 1.0);
	glLineWidth(10);
	glBegin(GL_LINE_LOOP);
	
	double littleStep = 360 / (n * 2);

	for (double i = phi; i <= phi + 360; i += 360 / n) {
		glColor3f(1.0, 1.0, 1.0);
		glVertex2f(O.x + l * cos(i * PI / 180), O.y + l * sin(i * PI / 180));
		glColor3f(1.0, 0.0, 0.0);
		glVertex2f(O.x + r * cos((i + littleStep) * PI / 180), O.y + r * sin((i + littleStep) * PI / 180));
	}

	glEnd();
}

void rotate() {
	phi += direction;
	glutPostRedisplay();
}

void keyPressed(unsigned char key, int x, int y) {
	switch (key) {
	case ' ':
		direction = -direction;
		break;
	case 'w':
		if (O.y + l < WINDOW_HEIGHT)
			O.y += 5;
		break;
	case 's':
		if (O.y - l > 0)
			O.y -= 5;
		break;
	case 'a':
		if (O.x - l > 0)
			O.x -= 5;
		break;
	case 'd':
		if (O.x + l < WINDOW_WIDTH)
			O.x += 5;
		break;
	default:
		break;
	}
	glutPostRedisplay();
}

void mousePressed(int button, int state, int x, int y) {
	switch (button) {
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_UP)
			n++;
		break;
	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_UP && n > 3)
			n--;
		break;
	default:
		break;
	}
}

void timer(int v) {
	rotate();
	glutTimerFunc(1000 / FPS, timer, v);
	glutPostRedisplay();
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	drawStar(O, r, l, n, phi);

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
	glutInitWindowPosition(20, 20);
	glutCreateWindow("hello");
	init();
	glutDisplayFunc(display);
	glutKeyboardFunc(keyPressed);
	glutMouseFunc(mousePressed);
	timer(0);
	glutMainLoop();
	return 0;
}
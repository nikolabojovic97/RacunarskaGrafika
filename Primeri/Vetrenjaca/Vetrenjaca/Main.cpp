#include <GL/freeglut.h>
#include <math.h>
#include <stdio.h>

#define WINDOW_WIDTH 700
#define WINDOW_HEIGHT 500
#define PI 3.1415926
#define ELISA_HEIGHT 180
#define ELISA_LEN 100


class Point {
public:
	float x;
	float y;
	float z;
	Point(float x, float y) {
		this->x = x;
		this->y = y;
		z = 0.0;
	}
};

float angleWindmill = 0.0;

float r = 20.0;
float angleSun = 180;

float sunStart = WINDOW_WIDTH - r/2;
float sunEnd = WINDOW_WIDTH + r/2;

Point windmillCenter(WINDOW_WIDTH / 2, ELISA_HEIGHT);
Point sunCenter(WINDOW_WIDTH / 2, 0.0);

void drawCirc(Point center, float r) {
	glColor3ub(235, 232, 52);
	glBegin(GL_POLYGON);
	for (int i = 0; i < 360; i++)
		glVertex3f(center.x + cos(i * 180.0 / PI) * r, center.y + sin(i * 180.0 / PI) * r, 0.0);
	glEnd();
}

void drawSun() {
	drawCirc(Point(sunCenter.x + cos(angleSun * 180 / PI) * (WINDOW_WIDTH / 2), sunCenter.y + sin(angleSun * 180 / PI) * (WINDOW_WIDTH / 2)), r);
}

void drawWindmill() {
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_LINES);
	glVertex3f(windmillCenter.x + cos(angleWindmill * 180.0 / PI) * ELISA_LEN, windmillCenter.y + sin(angleWindmill * 180.0 / PI) * ELISA_LEN, 0.0);
	glVertex3f(windmillCenter.x - cos(angleWindmill * 180.0 / PI) * ELISA_LEN, windmillCenter.y - sin(angleWindmill * 180.0 / PI) * ELISA_LEN, 0.0);
	glVertex3f(windmillCenter.x + cos(angleWindmill * 180.0 / PI + PI / 2) * ELISA_LEN, windmillCenter.y + sin(angleWindmill * 180.0 / PI + PI / 2) * ELISA_LEN, 0.0);
	glVertex3f(windmillCenter.x - cos(angleWindmill * 180.0 / PI + PI / 2) * ELISA_LEN, windmillCenter.y - sin(angleWindmill * 180.0 / PI + PI / 2) * ELISA_LEN, 0.0);
	glEnd();
}

void drawBackround() {
	//white background
	glColor3f(1.0, 1.0, 1.0);
	glRectd(0.0, 0.0, WINDOW_WIDTH, WINDOW_HEIGHT);

	//ground
	glLineWidth(2);
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_LINES);
	glVertex3f(0.0, 40.0, 0.0);
	glVertex3f(WINDOW_WIDTH, 40.0, 0.0);
	glEnd();

	//windmill
	glBegin(GL_LINE_STRIP);
	glVertex3f(WINDOW_WIDTH / 2 - 50.0, 40.0, 0.0);
	glVertex3f(WINDOW_WIDTH / 2 - 20.0, 200.0, 0.0);
	glVertex3f(WINDOW_WIDTH / 2 + 20.0, 200.0, 0.0);
	glVertex3f(WINDOW_WIDTH / 2 + 50.0, 40.0, 0.0);
	glEnd();

	drawWindmill();

	drawSun();
}



void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	
	drawBackround();

	glFlush();
}

void init(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, WINDOW_WIDTH, 0.0, WINDOW_HEIGHT, -1.0, 1.0);
}

void animationWorks() {
	angleWindmill -= 1;
	angleSun -= 1;
	printf("%f\n", angleSun);
	if (angleSun < 0)
		angleSun = 180;
	Sleep(50);
	glutPostRedisplay();
}

void mouseHandler(int button, int state, int x, int y) {
	switch (button) {
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN)
			glutIdleFunc(animationWorks);
		else
			glutIdleFunc(NULL);
		break;
	default:
		break;
	}
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInitWindowPosition(100, 80);
	glutCreateWindow("Vetrenjaca");
	init();
	glutDisplayFunc(display);
	glutMouseFunc(mouseHandler);
	glutMainLoop();
	printf("%f\n", angleSun);
	return 0;
}
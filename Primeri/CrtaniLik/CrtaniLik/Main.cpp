#include <GL/freeglut.h>
#include <math.h>

#define WINDOW_WIDTH 700.0
#define WINDOW_HEIGHT 500.0
#define PI 3.1415926

class Point {
public:
	float x;
	float y;
	float z;

	Point(float x, float y) {
		this->x = x;
		this->y = y;
		this->z = 0.0;
	}
};

void drawArc(Point o, int startAngle, int endAngle, float r, GLenum mode) {
	glBegin(mode);

	for (int i = startAngle; i <= endAngle; i++)
		glVertex3f(o.x + cos(i * PI / 180) * r, o.y + sin(i * PI / 180) * r, 0.0);


	glEnd();
}

void drawBackground() {

	//green floor
	float floorHeight = 40.0;
	glColor3b(10, 41, 11);
	glRectf(0.0, 0.0, WINDOW_WIDTH, floorHeight);

	//texture on floor
	glColor3b(0, 0, 0);
	glLineWidth(2);
	glBegin(GL_LINES);
	int r = 1000;
	
	for (int i = 180; i < 360; i += 5) {
		glVertex3f(WINDOW_WIDTH/2, WINDOW_HEIGHT/2, 0.0);
		glVertex3f(WINDOW_WIDTH / 2 + cos(i * PI / 180) * r, WINDOW_HEIGHT / 2 + sin(i * PI / 180) * r, 0.0);
	}
	glEnd();

	//blue sky
	float skyHeight = 190.0;
	glColor3ub(174, 230, 245);
	glRectf(0.0, WINDOW_HEIGHT - skyHeight, WINDOW_WIDTH, WINDOW_HEIGHT);

	//sand
	glColor3ub(166, 151, 85);
	glRectf(0.0, floorHeight, WINDOW_WIDTH, WINDOW_HEIGHT - skyHeight);

	//road
	glColor3ub(94, 86, 76);
	glRectf(0.0, 225.0, WINDOW_WIDTH, 300.0);
	glBegin(GL_POLYGON);
	glVertex3f(floorHeight + 15, floorHeight, 0.0);
	glVertex3f(WINDOW_WIDTH - floorHeight - 15, floorHeight, 0.0);
	glVertex3f(WINDOW_WIDTH - floorHeight - 150, 225.0, 0.0);
	glVertex3f(floorHeight + 150, 225.0, 0.0);
	glEnd();

	//brown doors
	glColor3ub(31, 20, 6);
	glBegin(GL_POLYGON);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(floorHeight + 25, floorHeight, 0.0);
	glVertex3f(floorHeight + 25, WINDOW_WIDTH, 0.0);
	glVertex3f(0.0, WINDOW_WIDTH, 0.0);
	glEnd();
	
	glBegin(GL_POLYGON);
	glVertex3f(WINDOW_WIDTH - floorHeight - 25, floorHeight, 0.0);
	glVertex3f(WINDOW_WIDTH, 0.0, 0.0);
	glVertex3f(WINDOW_WIDTH, WINDOW_HEIGHT, 0.0);
	glVertex3f(WINDOW_WIDTH - floorHeight - 25, WINDOW_HEIGHT, 0.0);
	glEnd();
}

void drawClothes() {

	//pants
	glColor3ub(156, 107, 47);
	Point p1(WINDOW_WIDTH / 2 - 75, 120);
	Point p2(WINDOW_WIDTH / 2 + 75, 150);
	glRectf(p1.x, p1.y, p2.x, p2.y);

	glColor3b(0, 0, 0);
	glBegin(GL_LINE_LOOP);
	glVertex3f(p1.x, p1.y, 0.0);
	glVertex3f(p2.x, p1.y, 0.0);
	glVertex3f(p2.x, p2.y, 0.0);
	glVertex3f(p1.x, p2.y, 0.0);
	glEnd();

	//belt
	glEnable(GL_LINE_STIPPLE);
	glLineWidth(4);
	glEnable(GL_LINE_SMOOTH);
	glLineStipple(10, 0xAAAA);
	glBegin(GL_LINES);
	glVertex3f(p1.x + 10, p2.y - 10, 0.0);
	glVertex3f(p2.x - 10, p2.y - 10, 0.0);
	glEnd();
	glDisable(GL_LINE_STIPPLE);
	glDisable(GL_LINE_SMOOTH);

	glColor3ub(156, 107, 47);
	float part = 5.0;
	float width = 20.0;
	Point pp1(p1.x + 25, 120);//upper left 1st 
	Point pp2(p1.x + 25 + width - part, 30);//lower right 1st
	Point pp3(p2.x - 25 - width, 120);//upper left 2nd
	Point pp4(p2.x - 25 + part, 30);//lower right 2nd

	glBegin(GL_POLYGON);
	glVertex3f(pp1.x, pp1.y, 0.0);
	glVertex3f(pp1.x + width, pp1.y, 0.0);
	glVertex3f(pp2.x - part, pp2.y, 0.0);
	glVertex3f(pp2.x - width - part, pp2.y, 0.0);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(pp3.x, pp3.y, 0.0);
	glVertex3f(pp3.x + width, pp3.y, 0.0);
	glVertex3f(pp4.x + part, pp4.y, 0.0);
	glVertex3f(pp4.x - width + part, pp4.y, 0.0);
	glEnd();

	glColor3b(0, 0, 0);
	glLineWidth(2);
	glDisable(GL_LINE_SMOOTH);
	glBegin(GL_LINE_LOOP);
	glVertex3f(pp1.x, pp1.y, 0.0);
	glVertex3f(pp1.x + width, pp1.y, 0.0);
	glVertex3f(pp2.x - part, pp2.y, 0.0);
	glVertex3f(pp2.x - width - part, pp2.y, 0.0);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glVertex3f(pp3.x, pp3.y, 0.0);
	glVertex3f(pp3.x + width, pp3.y, 0.0);
	glVertex3f(pp4.x + part, pp4.y, 0.0);
	glVertex3f(pp4.x - width + part, pp4.y, 0.0);
	glEnd();

	//shoes
	Point p1Shoes(pp2.x - 30, pp2.y - 10);
	Point p2Shoes(pp4.x + 10, pp4.y - 10);

	glBegin(GL_POLYGON);
	glVertex3f(pp2.x - part, pp2.y, 0.0);
	glVertex3f(pp2.x - width - part, pp2.y, 0.0);
	glVertex3f(p1Shoes.x, p1Shoes.y, 0.0);
	glEnd();


	glBegin(GL_POLYGON);
	glVertex3f(pp4.x + part, pp4.y, 0.0);
	glVertex3f(pp4.x- width + part, pp4.y, 0.0);
	glVertex3f(p2Shoes.x, p2Shoes.y, 0.0);
	glEnd();

	drawArc(Point(p1Shoes.x, p1Shoes.y + 10), 0, 360, 10, GL_POLYGON);
	drawArc(Point(p2Shoes.x, p2Shoes.y + 10), 0, 360, 10, GL_POLYGON);

	glColor3ub(255, 255, 255);
	drawArc(Point(p1Shoes.x, p1Shoes.y + 12), 0, 360, 4, GL_POLYGON);
	drawArc(Point(p2Shoes.x, p2Shoes.y + 12), 0, 360, 4, GL_POLYGON);

	//shirt
	Point pShirt1(WINDOW_WIDTH / 2 - 75, 150);
	Point pShirt2(WINDOW_WIDTH / 2 + 75, 170);
	glRectf(pShirt1.x, pShirt1.y, pShirt2.x, pShirt2.y);

	glColor3ub(0, 0, 0);
	glBegin(GL_LINE_LOOP);
	glVertex3f(pShirt1.x, pShirt1.y, 0.0);
	glVertex3f(pShirt2.x, pShirt1.y, 0.0);
	glVertex3f(pShirt2.x, pShirt2.y, 0.0);
	glVertex3f(pShirt1.x, pShirt2.y, 0.0);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glVertex3f(WINDOW_WIDTH / 2 - 40, pShirt2.y, 0.0);
	glVertex3f(WINDOW_WIDTH / 2 - 15, pShirt2.y, 0.0);
	glVertex3f(WINDOW_WIDTH / 2 - 20, pShirt2.y - 10, 0.0);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glVertex3f(WINDOW_WIDTH / 2 + 40, pShirt2.y, 0.0);
	glVertex3f(WINDOW_WIDTH / 2 + 15, pShirt2.y, 0.0);
	glVertex3f(WINDOW_WIDTH / 2 + 20, pShirt2.y - 10, 0.0);
	glEnd();

	//tie
	glColor3ub(255, 0, 0);
	glRectf(WINDOW_WIDTH / 2 - 7, pShirt2.y - 7, WINDOW_WIDTH / 2 + 7, pShirt2.y);

	glColor3ub(0, 0, 0);
	glBegin(GL_LINE_LOOP);
	glVertex3f(WINDOW_WIDTH / 2 - 7, pShirt2.y - 7, 0.0);
	glVertex3f(WINDOW_WIDTH / 2 + 7, pShirt2.y - 7, 0.0);
	glVertex3f(WINDOW_WIDTH / 2 + 7, pShirt2.y, 0.0);
	glVertex3f(WINDOW_WIDTH / 2 - 7, pShirt2.y, 0.0);
	glEnd();

	Point pTie1(WINDOW_WIDTH / 2 - 5, pShirt2.y - 7);
	Point pTie2(WINDOW_WIDTH / 2 - 10, pShirt2.y - 25);
	Point pTie3(WINDOW_WIDTH / 2 , pShirt2.y - 40);
	Point pTie4(WINDOW_WIDTH / 2 + 10, pShirt2.y - 25);
	Point pTie5(WINDOW_WIDTH / 2 + 5, pShirt2.y - 7);

	glColor3ub(255, 0, 0);
	glBegin(GL_POLYGON);
	glVertex3f(pTie1.x, pTie1.y, 0.0);
	glVertex3f(pTie2.x, pTie2.y, 0.0);
	glVertex3f(pTie3.x, pTie3.y, 0.0);
	glVertex3f(pTie4.x, pTie4.y, 0.0);
	glVertex3f(pTie5.x, pTie5.y, 0.0);
	glEnd();

	glColor3ub(0, 0, 0);
	glBegin(GL_LINE_LOOP);
	glVertex3f(pTie1.x, pTie1.y, 0.0);
	glVertex3f(pTie2.x, pTie2.y, 0.0);
	glVertex3f(pTie3.x, pTie3.y, 0.0);
	glVertex3f(pTie4.x, pTie4.y, 0.0);
	glVertex3f(pTie5.x, pTie5.y, 0.0);
	glEnd();
}

void drawBody() {
	glColor3ub(237, 240, 77);
	float bodyHeight = 180;
	Point pBody1(WINDOW_WIDTH / 2 - 78, 170);
	Point pBody2(WINDOW_WIDTH / 2 + 78, 170);
	Point pBody3(WINDOW_WIDTH / 2 + 82, 170 + bodyHeight);
	Point pBody4(WINDOW_WIDTH / 2 - 82, 170 + bodyHeight);
	glBegin(GL_POLYGON);
	glVertex3f(pBody1.x, pBody1.y, 0.0);
	glVertex3f(pBody2.x, pBody2.y, 0.0);
	glVertex3f(pBody3.x, pBody3.y, 0.0);
	glVertex3f(pBody4.x, pBody4.y, 0.0);
	glEnd();

	glColor3b(0, 0, 0);
	glBegin(GL_LINE_LOOP);
	glVertex3f(pBody1.x, pBody1.y, 0.0);
	glVertex3f(pBody2.x, pBody2.y, 0.0);
	glVertex3f(pBody3.x, pBody3.y, 0.0);
	glVertex3f(pBody4.x, pBody4.y, 0.0);
	glEnd();

	//freckles
	glColor3ub(110, 108, 32);
	drawArc(Point(pBody1.x + 25, pBody1.y + 20), 0, 360, 15, GL_POLYGON);
	drawArc(Point(pBody2.x - 20, pBody2.y + 55), 0, 360, 15, GL_POLYGON);
	drawArc(Point(pBody4.x + 27, pBody4.y - 30), 0, 360, 15, GL_POLYGON);
	drawArc(Point(pBody1.x + 10, pBody1.y + 50), 0, 360, 7, GL_POLYGON);
	drawArc(Point(pBody2.x - 10, pBody2.y + 15), 0, 360, 7, GL_POLYGON);
	drawArc(Point(pBody3.x - 14, pBody3.y - 10), 0, 360, 7, GL_POLYGON);

	//eyes
	glColor3ub(255, 255, 255);
	float eyesHeight= 120;
	Point pEye1(WINDOW_WIDTH / 2 - 30, 170 + eyesHeight);
	Point pEye2(WINDOW_WIDTH / 2 + 30, 170 + eyesHeight);

	drawArc(pEye1, 0, 360, 30, GL_POLYGON);
	drawArc(pEye2, 0, 360, 30, GL_POLYGON);

	glColor3b(0, 0, 0);
	drawArc(pEye1, 0, 360, 30, GL_LINE_LOOP);
	drawArc(pEye2, 0, 360, 30, GL_LINE_LOOP);

	glColor3ub(77, 93, 201);
	drawArc(pEye1, 0, 360, 10, GL_POLYGON);
	drawArc(pEye2, 0, 360, 10, GL_POLYGON);

	glColor3b(0, 0, 0);
	drawArc(pEye1, 0, 360, 5, GL_POLYGON);
	drawArc(pEye2, 0, 360, 5, GL_POLYGON);

	glColor3ub(237, 240, 77);
	drawArc(pEye1, 10, 180, 30, GL_POLYGON);
	drawArc(pEye2, 0, 170, 30, GL_POLYGON);

	glColor3b(0, 0, 0);
	drawArc(pEye1, 10, 180, 30, GL_LINE_LOOP);
	drawArc(pEye2, 0, 170, 30, GL_LINE_LOOP);

	//mouth
	drawArc(Point(WINDOW_WIDTH / 2, 170 + bodyHeight - 70), 220, 320, 60, GL_LINE_STRIP);
	
	glColor3ub(237, 240, 77);
	drawArc(Point(WINDOW_WIDTH / 2 - 50, 170 + bodyHeight - 100), -90, 160, 18, GL_POLYGON);
	drawArc(Point(WINDOW_WIDTH / 2 + 50, 170 + bodyHeight - 100), 20, 270, 18, GL_POLYGON);
	glColor3ub(237, 147, 28);
	drawArc(Point(WINDOW_WIDTH / 2 - 50, 170 + bodyHeight - 100), -90, 160, 18, GL_LINE_STRIP);
	drawArc(Point(WINDOW_WIDTH / 2 + 50, 170 + bodyHeight - 100), 20, 270, 18, GL_LINE_STRIP);

	//freckles on cheeks
	glPointSize(3);
	glBegin(GL_POINTS);
	Point pFreckles1(WINDOW_WIDTH / 2 - 50, 170 + bodyHeight - 100);
	glVertex3f(pFreckles1.x + 5, pFreckles1.y + 2, 0.0);
	glVertex3f(pFreckles1.x - 5, pFreckles1.y + 2, 0.0);
	glVertex3f(pFreckles1.x, pFreckles1.y - 5, 0.0);
	glEnd();

	glBegin(GL_POINTS);
	Point pFreckles2(WINDOW_WIDTH / 2 + 50, 170 + bodyHeight - 100);
	glVertex3f(pFreckles2.x + 5, pFreckles2.y + 2, 0.0);
	glVertex3f(pFreckles2.x - 5, pFreckles2.y + 2, 0.0);
	glVertex3f(pFreckles2.x, pFreckles2.y - 5, 0.0);
	glEnd();

	//eyebrows
	glColor3b(0, 0, 0);
	Point pEyebrows1(WINDOW_WIDTH / 2 - 50, 170 + bodyHeight - 4);
	Point pEyebrows2(WINDOW_WIDTH / 2 + 55, 170 + bodyHeight);
	drawArc(pEyebrows1, 260, 320, 30, GL_LINE_STRIP);
	drawArc(pEyebrows2, 200, 280, 30, GL_LINE_STRIP);

	//nose
	Point pNose1(WINDOW_WIDTH / 2, 170 + 80);
	drawArc(pNose1, -30, 210, 12, GL_LINE_STRIP);

	//sleeves
	glColor3ub(255, 255, 255);
	glRectf(pBody1.x, pBody1.y + 70, pBody1.x - 15, pBody1.y + 45);
	glRectf(pBody2.x, pBody1.y + 70, pBody2.x + 15, pBody2.y + 45);

	glColor3b(0, 0, 0);
	glBegin(GL_LINE_LOOP);
	glVertex3f(pBody1.x, pBody1.y + 70, 0.0);
	glVertex3f(pBody1.x - 15, pBody1.y + 70, 0.0);
	glVertex3f(pBody1.x - 15, pBody1.y + 45, 0.0);
	glVertex3f(pBody1.x, pBody1.y + 45, 0.0);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glVertex3f(pBody2.x, pBody2.y + 70, 0.0);
	glVertex3f(pBody2.x + 15, pBody2.y + 70, 0.0);
	glVertex3f(pBody2.x + 15, pBody2.y + 45, 0.0);
	glVertex3f(pBody2.x, pBody2.y + 45, 0.0);
	glEnd();

	//hands
	glColor3ub(237, 240, 77);
	glRectf(pBody1.x - 15, pBody1.y + 60, pBody1.x - 90, pBody1.y + 55);
	glRectf(pBody2.x + 15, pBody2.y + 60, pBody2.x + 90, pBody2.y + 55);

	glRectf(pBody1.x - 90, pBody1.y + 70, pBody1.x - 110, pBody1.y + 50);
	glRectf(pBody2.x + 90, pBody2.y + 70, pBody2.x + 110, pBody2.y + 50);

	glColor3b(0, 0, 0);
	glBegin(GL_LINE_LOOP);
	glVertex3f(pBody1.x - 15, pBody1.y + 60, 0.0);
	glVertex3f(pBody1.x - 90, pBody1.y + 60, 0.0);
	glVertex3f(pBody1.x - 90, pBody1.y + 55, 0.0);
	glVertex3f(pBody1.x - 15, pBody1.y + 55, 0.0);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glVertex3f(pBody2.x + 15, pBody2.y + 60, 0.0);
	glVertex3f(pBody2.x + 90, pBody2.y + 60, 0.0);
	glVertex3f(pBody2.x + 90, pBody2.y + 55, 0.0);
	glVertex3f(pBody2.x + 15, pBody2.y + 55, 0.0);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glVertex3f(pBody1.x - 90, pBody1.y + 70, 0.0);
	glVertex3f(pBody1.x - 110, pBody1.y + 70, 0.0);
	glVertex3f(pBody1.x - 110, pBody1.y + 50, 0.0);
	glVertex3f(pBody1.x - 90, pBody1.y + 50, 0.0);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glVertex3f(pBody2.x + 90, pBody2.y + 70, 0.0);
	glVertex3f(pBody2.x + 110, pBody2.y + 70, 0.0);
	glVertex3f(pBody2.x + 110, pBody2.y + 50, 0.0);
	glVertex3f(pBody2.x + 90, pBody2.y + 50, 0.0);
	glEnd();
}


void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);


	drawBackground();
	drawClothes();
	drawBody();

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
	glutCreateWindow("SpongeBob");
	init();
	glutDisplayFunc(display);
	glutMainLoop();
	return 0;
}
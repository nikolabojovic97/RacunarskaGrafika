#include <GL/freeglut.h>  
#include <math.h>
#include "vector3d.h"
#include "vector4d.h"
#include "matrix4x4.h"
#include <vector>

#define M_PI 3.14
using namespace std;

/* Global variables */
char title[] = "Helikopter";

Vector3D camera(0.0, 0.0, 0.0);
Vector3D up(0.0, 1.0, 0.0);
Vector3D lookAt(0.0, 0.0, -3.0);

int FPS = 60;
double WINDOW_HEIGHT = 480;
double aspectRatio = 640.0f / 480.0f;
double speed = 0.05;

vector<vector<Vector3D>> zgrade;
vector<vector<Vector3D>> controlPanel;
vector<vector<Vector3D>> radar;
vector<Vector3D> coords;

void TransformV1(Matrix4x4 &M, vector<Vector3D> &v)
{
    for(unsigned int i = 0; i < v.size(); i++)
        v[i] = M.Transform(v[i]);
}

void TransformV2(Matrix4x4 &M, vector< vector<Vector3D> > &v)
{
    for(unsigned int i = 0; i < v.size(); i++)
        TransformV1(M,v[i]);
}

void createBuildings()
{
	vector<Vector3D> zgrada;

	zgrada.push_back(Vector3D(0.0, 0.0, 0.0));
	zgrada.push_back(Vector3D(1.0, 0.0, 0.0));
	zgrada.push_back(Vector3D(1.0, 0.0, 1.0));
	zgrada.push_back(Vector3D(0.0, 0.0, 1.0));

	zgrada.push_back(Vector3D(0.0, 2.0, 0.0));
	zgrada.push_back(Vector3D(1.0, 2.0, 0.0));
	zgrada.push_back(Vector3D(1.0, 2.0, 1.0));
	zgrada.push_back(Vector3D(0.0, 2.0, 1.0));

	zgrade.push_back(zgrada);

	Matrix4x4 m;
	m.loadTranslate(1.0, 0.0, 2.0);
	zgrada = m.Transform(zgrada);
	zgrade.push_back(zgrada);

	m.loadTranslate(-2.0, 0.0, 2.0);
	zgrada = m.Transform(zgrada);
	zgrade.push_back(zgrada);

	m.loadTranslate(3.0, 0.0, -3.0);
	zgrada = m.Transform(zgrada);
	zgrade.push_back(zgrada);

	m.loadTranslate(5.0, 0.0, -5.0);
	zgrada = m.Transform(zgrada);
	zgrade.push_back(zgrada);

	m.loadTranslate(-3.0, 0.0, -3.0);
	zgrada = m.Transform(zgrada);
	zgrade.push_back(zgrada);
}

void createControlPanel()
{
	vector<Vector3D> object;

	object.push_back(Vector3D(-1.0, 1.0, 0.0));
	object.push_back(Vector3D(-1.0, -0.1, 0.0));
	object.push_back(Vector3D(-0.9, -0.1, 0.0));
	object.push_back(Vector3D(-0.5, 0.9, 0.0));
	object.push_back(Vector3D(-0.5, 1.0, 0.0));

	controlPanel.push_back(object);

	object.clear();
	object.push_back(Vector3D(1.0, 1.0, 0.0));
	object.push_back(Vector3D(1.0, -0.1, 0.0));
	object.push_back(Vector3D(0.9, -0.1, 0.0));
	object.push_back(Vector3D(0.5, 0.9, 0.0));
	object.push_back(Vector3D(0.5, 1.0, 0.0));

	controlPanel.push_back(object);

	object.clear();
	object.push_back(Vector3D(-0.5, 1.0, 0.0));
	object.push_back(Vector3D(-0.5, 0.9, 0.0));
	object.push_back(Vector3D(0.5, 0.9, 0.0));
	object.push_back(Vector3D(0.5, 1.0, 0.0));

	controlPanel.push_back(object);

	object.clear();
	object.push_back(Vector3D(-0.1, 0.9, 0.0));
	object.push_back(Vector3D(-0.1, -0.1, 0.0));
	object.push_back(Vector3D(0.1, -0.1, 0.0));
	object.push_back(Vector3D(0.1, 0.9, 0.0));

	controlPanel.push_back(object);

	object.clear();
	object.push_back(Vector3D(-1.0, -0.1, 0.0));
	object.push_back(Vector3D(-1.0, -1.0, 0.0));
	object.push_back(Vector3D(1.0, -1.0, 0.0));
	object.push_back(Vector3D(1.0, -0.1, 0.0));

	controlPanel.push_back(object);
}

void createCoordinateSystem()
{
	Matrix4x4 m;
	m.loadTranslate(-0.7, 0.7, 0.0);

	coords.push_back(Vector3D(0.0, 0.0, 0.0));
	coords.push_back(Vector3D::AxisX*0.1);
	coords.push_back(Vector3D::AxisY*0.1);
	coords.push_back(Vector3D::AxisZ*0.1);

	coords = m.Transform(coords);
}

void createRadar()
{
	Matrix4x4 m;
	m.loadRotateZ(0.1);
	vector<Vector3D> object;
	object.push_back(Vector3D(0.0, 0.0, 0.0));
	Vector3D tren(0.2, 0.0, 0.0);
	int n = 2 * M_PI / 0.1;

	for (int i = 1; i < n; i++)
	{
		object.push_back(Vector3D(tren.X(), tren.Y(), tren.Z()));
		tren = m.Transform(tren);
	}
	
	radar.push_back(object);

	object.clear();

	object.push_back(Vector3D(0.2, 0.0, 0.0));

	radar.push_back(object);

	object.clear();

	for (int i = 0; i < zgrade.size(); i++)
	{
		Vector3D tacka((zgrade[i][0].X() + zgrade[i][1].X()) / 2, (zgrade[i][1].Z() + zgrade[i][2].Z()) / 2,0.0);

		Matrix4x4 m1, m2;
		m2.loadTranslate(-camera.X(), -camera.Z(), 0.0);
		tacka = m2.Transform(tacka);
		tacka = Vector3D(tacka.X()*0.2 / (10.0), -tacka.Y()*0.2 / (10.0), 0.0);
		m1.loadTranslate(radar[0][0].X(), radar[0][0].Y(), 0.0);
			
		object.push_back(tacka);
	}
	radar.push_back(object);
	m.loadTranslate(0.0, -0.5, 0.0);

	radar = m.Transform(radar);

	
}

void drawControlPanel()
{
	glColor3f(0.8, 0.8, 0.8);

	for (int i = 0; i < controlPanel.size(); i++)
	{
		glBegin(GL_POLYGON);

		for (int j = 0; j < controlPanel[i].size(); j++)
			glVertex3f(controlPanel[i][j].X(), controlPanel[i][j].Y(), controlPanel[i][j].Z());

		glEnd();
	}
}

void drawCoordinateSystem()
{
	for (int i = 1; i < 4; i++)
	{
		if (i == 1) glColor3f(1.0, 0.0, 0.0);
		else if (i == 2) glColor3f(0.0, 1.0, 0.0);
		else glColor3f(0.0, 0.0, 1.0);

		glBegin(GL_LINES);

		glVertex3f(coords[0].X(), coords[0].Y(), coords[0].Z());
		glVertex3f(coords[i].X(), coords[i].Y(), coords[i].Z());

		glEnd();
	}
}

void drawBuildings()
{
	glColor3f(1.0, 1.0, 0.0);

	for (int i = 0; i < zgrade.size(); i++)
	{
		glBegin(GL_POLYGON);

		for (int j = 0; j < 4; j++)
			glVertex3f(zgrade[i][j].X(), zgrade[i][j].Y(), zgrade[i][j].Z());

		glEnd();

		glBegin(GL_POLYGON);

		for (int j = 4; j < 8; j++)
			glVertex3f(zgrade[i][j].X(), zgrade[i][j].Y(), zgrade[i][j].Z());

		glEnd();

		for (int j = 1; j < 4; j++)
		{
			glBegin(GL_POLYGON);

			glVertex3f(zgrade[i][j - 1].X(), zgrade[i][j - 1].Y(), zgrade[i][j - 1].Z());
			glVertex3f(zgrade[i][j].X(), zgrade[i][j].Y(), zgrade[i][j].Z());
			glVertex3f(zgrade[i][j + 4].X(), zgrade[i][j + 4].Y(), zgrade[i][j + 4].Z());
			glVertex3f(zgrade[i][j + 4 - 1].X(), zgrade[i][j + 4 - 1].Y(), zgrade[i][j + 4 - 1].Z());

			glEnd();
		}

		glBegin(GL_POLYGON);

		glVertex3f(zgrade[i][0].X(), zgrade[i][0].Y(), zgrade[i][0].Z());
		glVertex3f(zgrade[i][3].X(), zgrade[i][3].Y(), zgrade[i][3].Z());
		glVertex3f(zgrade[i][7].X(), zgrade[i][7].Y(), zgrade[i][7].Z());
		glVertex3f(zgrade[i][4].X(), zgrade[i][4].Y(), zgrade[i][4].Z());

		glEnd();
	}
}

void drawRadar()
{
	Matrix4x4 m1, m2, m3, m4;
	m1.loadTranslate(-radar[0][0].X(), -radar[0][0].Y(), -radar[0][0].Z());
	m2.loadRotateZ(-0.001);
	m3.loadTranslate(radar[0][0].X(), radar[0][0].Y(), radar[0][0].Z());

	m4 = m3*m2*m1;

	radar[1][0] = m4.Transform(radar[1][0]);

	glColor3f(0.0, 0.0, 0.0);

	glBegin(GL_POLYGON);

	for (int i = 1; i < radar[0].size(); i++)
		glVertex3f(radar[0][i].X(), radar[0][i].Y()*aspectRatio, radar[0][i].Z());

	glEnd();
	
	double rastojanje = 2.0;

	glColor3f(1.0, 0.0, 0.0);
	glPointSize(4);
	glEnable(GL_POINT_SMOOTH);
	glBegin(GL_POINTS);

	for (int i = 0; i < radar[2].size(); i++)
	{
		Vector3D tacka(radar[2][i].X(),radar[2][i].Y(), 0.0);
		if (fabs(sqrt(pow(tacka.X() - radar[0][0].X(), 2) + pow(tacka.Y() - radar[0][0].Y(), 2)))<0.2)
		{
			glVertex3f(tacka.X(), tacka.Y()*aspectRatio, 0.0);
		}
	}

	glEnd();
	
	glColor3f(0.0, 1.0, 0.0);
	glLineWidth(2);
	glBegin(GL_LINES);

	glVertex3f(radar[0][0].X(), radar[0][0].Y()*aspectRatio, radar[0][0].Z());
	glVertex3f(radar[1][0].X(), radar[1][0].Y()*aspectRatio, radar[1][0].Z());

	glEnd();
}

void drawText()
{
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_POLYGON);
	
	glVertex3f(0.7, 0.9, 0.0);
	glVertex3f(0.7, 0.8, 0.0);
	glVertex3f(0.9, 0.8, 0.0);
	glVertex3f(0.9, 0.9, 0.0);

	glEnd();

	glColor3f(0.0, 0.0, 0.0);
	glRasterPos2f(0.72, 0.85);
	char s[20];
	sprintf_s(s, "H = %.2lf", camera.Y()*100);
	glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_10, (const unsigned char*)s);
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffers

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glColor3f(1.0, 1.0, 0.0);

	gluPerspective(80.0f, aspectRatio, 0.1f, 50.0f);
	gluLookAt(camera.m_x, camera.m_y, camera.m_z,
		lookAt.X(), lookAt.Y(), lookAt.Z(),
		up.X(), up.Y(), up.Z());

	drawBuildings();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glDisable(GL_DEPTH_TEST);

	drawControlPanel();
	drawCoordinateSystem();
	drawText();
	drawRadar();

	glEnable(GL_DEPTH_TEST);
    

    glutSwapBuffers();  // Swap the front and back frame buffers (double buffering)
	
	glutPostRedisplay();
}

void SetCamera()
{
	glViewport(0, 0, WINDOW_HEIGHT*aspectRatio, WINDOW_HEIGHT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(80.0f, aspectRatio, 0.1f, 50.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(camera.m_x, camera.m_y, camera.m_z,
		lookAt.X(), lookAt.Y(), lookAt.Z(),
		up.X(), up.Y(), up.Z());
}

void turnLeft()
{
	Matrix4x4 m, m2, m3, m4, m5, m6;
	m.loadRotationMatrix(camera, up, speed);

	lookAt = m.Transform(lookAt);
	

	m.loadRotationMatrix(camera, up, -speed);
	m2.loadTranslate(-coords[0].X(), -coords[0].Y(), -coords[0].Z());
	m3.loadTranslate(camera.X(), camera.Y(), camera.Z());
	m4.loadTranslate(-camera.X(), -camera.Y(), -camera.Z());
	m5.loadTranslate(coords[0].X(), coords[0].Y(), coords[0].Z());

	m6 = m5*m4*m*m3*m2;

	coords = m6.Transform(coords);

	m.loadRotateZ(-speed);
	m2.loadTranslate(-radar[0][0].X(), -radar[0][0].Y(), -radar[0][0].Z());
	m5.loadTranslate(radar[0][0].X(), radar[0][0].Y(), radar[0][0].Z());

	m6 = m5*m*m2;

	radar[2] = m6.Transform(radar[2]);

}

void turnRight()
{
	Matrix4x4 m, m2, m3, m4, m5, m6;
	m.loadRotationMatrix(camera, up, -speed);

	lookAt = m.Transform(lookAt);

	m.loadRotationMatrix(camera, up, speed);
	m2.loadTranslate(-coords[0].X(), -coords[0].Y(), -coords[0].Z());
	m3.loadTranslate(camera.X(), camera.Y(), camera.Z());
	m4.loadTranslate(-camera.X(), -camera.Y(), -camera.Z());
	m5.loadTranslate(coords[0].X(), coords[0].Y(), coords[0].Z());

	m6 = m5*m4*m*m3*m2;

	coords = m6.Transform(coords);

	m.loadRotateZ(speed);
	m2.loadTranslate(-radar[0][0].X(), -radar[0][0].Y(), -radar[0][0].Z());
	m5.loadTranslate(radar[0][0].X(), radar[0][0].Y(), radar[0][0].Z());

	m6 = m5*m*m2;
	
	radar[2] = m6.Transform(radar[2]);
}

void moveUp()
{
	Vector3D v = up;
	v.Normalize();
	v *= speed;

	Matrix4x4 m;
	m.loadTranslate(v.X(), v.Y(), v.Z());

	lookAt = m.Transform(lookAt);
	camera = m.Transform(camera);
}

void moveDown()
{
	Vector3D v = up;
	v.Normalize();
	v *= speed;

	Matrix4x4 m;
	m.loadTranslate(-v.X(), -v.Y(), -v.Z());

	lookAt = m.Transform(lookAt);
	camera = m.Transform(camera);
}

void moveForward()
{
	Vector3D v = lookAt - camera;
	v.Normalize();
	v.m_y = 0.0;
	v *= speed;

	Matrix4x4 m;
	m.loadTranslate(v.X(), v.Y(), v.Z());

	lookAt = m.Transform(lookAt);
	camera = m.Transform(camera);

	v = Vector3D(radar[0][0].X(), radar[0][0].Y() - 1.0, 0.0) - radar[0][0];

	v.Normalize();
	v *= speed*0.2/10.0;

	m.loadTranslate(v.X(), v.Y(), v.Z());

	radar[2] = m.Transform(radar[2]);
}

void move(unsigned char key, int x, int y)
{
	if (key == 27) exit(0);
	else if (key == 'a') turnLeft();
	else if (key == 'd') turnRight();
	else if (key == 56) moveUp();
	else if (key == 50) moveDown();
	else if (key == 53) moveForward();
}

void initGL()
{
   glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
   glShadeModel(GL_FLAT);

   glEnable(GL_DEPTH_TEST);   // Enable depth testing for z-culling
}

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_DOUBLE);
   glutInitWindowSize(WINDOW_HEIGHT * aspectRatio, WINDOW_HEIGHT);
   glutInitWindowPosition(50, 50);
   glutCreateWindow(title);
   createBuildings();
   createControlPanel();
   createCoordinateSystem();
   createRadar();
   
   glutKeyboardFunc(move);

   glutDisplayFunc(display);
   SetCamera();
   initGL();
   glutMainLoop();

   return 0;
}

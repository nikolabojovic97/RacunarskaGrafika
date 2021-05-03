#include <Gl\freeglut.h>
#include <math.h>
#include "vector3d.h"
#include "vector4d.h"
#include "matrix4x4.h"
#include <vector>

#define M_PI 3.14
using namespace std;

char title[] = "II kolokvijum";

#define MOVING_CONST 0.1
#define ROTATION_CONST 3.14/180.0

Vector3D CameraPosition(7.0, 7.0, 7.0);
#define LOOK_MOVEMENT_CONST 0.1
Vector3D LookAt_vector(1.0, 0.0, 1.0);
Vector3D LookUp_vector(0.0, 1.0, 0.0);


double alpha = 120;


double visina = 480;
double ratio = 16.0 / 9.0;


int speedIndicator = 0;
double streliceRotate = 0.0;
double rotationUpDown = 0.0;


vector<vector<Vector3D>> staza;
vector<vector<Vector3D>> staza_mapa;
Matrix4x4 MzaMapu;

vector<Vector3D> panel;

double r1 = 4.0;
double r2 = 5.0;
double r3 = 6.0;
double cameraH = 1.0;

vector< vector<Vector3D>> meracBrzine;



vector<Vector3D> kompasOsnova;
vector<Vector3D> kompasStrelica;

double r_kompas = 1.0;
double r_kompas_real = 5.0;
int circle_points_kompas = 50;


vector<Vector3D> createCircle(double r, int n)
{
	double phi = 360.0 / n;
	Vector3D radius(r, 0, 0);
	vector<Vector3D> circle;
	Vector3D centar(0.0, 0.0, 0.0);
	circle.push_back(radius);
	for (int i = 0; i < n; i++)
	{
		Vector3D v = Matrix4x4::Rotate(circle[i], centar, 2, M_PI / 180.0 * phi);
		circle.push_back(v);
	}

	return circle;
}

void napraviStazu()
{


	staza.push_back(createCircle(r1, 50));
	staza.push_back(createCircle(r2, 50));
	staza.push_back(createCircle(r3, 50));

	Matrix4x4 m1, m11, m2, m3;
	m1.loadRotateX(90.0 * M_PI / 180.0);
	m2.loadScale(0.04 / ratio, 0.04, 0.0);
	m3.loadTranslate(0, -0.67, 0);
	MzaMapu = m3 * m2 * m1;
	staza_mapa = MzaMapu.Transform(staza);

}

void crtajStazu(vector<vector<Vector3D>> s)
{
	for (int i = 0; i < s.size(); i++)
	{
		glBegin(GL_LINE_LOOP);
		for (int j = 0; j < s[i].size(); j++)
		{
			glVertex3f(s[i][j].X(), s[i][j].Y(), s[i][j].Z());
		}
		glEnd();
	}
}

void crtajPozicijuKamere()
{
	Vector3D c_mapa = MzaMapu.Transform(CameraPosition);
	glColor3f(1, 0, 0);
	glPointSize(10);
	glBegin(GL_POINTS);
		glVertex2f(c_mapa.X(), c_mapa.Y());
	glEnd();
}


void kreirajPanel()
{
	Vector3D a(-1.0, -1.0, 0.0);
	Vector3D b(1.0, -1.0, 0.0);
	Vector3D c(1.0, -0.3, 0.0);
	Vector3D d(-1.0, -0.3, 0.0);

	panel.push_back(a);
	panel.push_back(b);
	panel.push_back(c);
	panel.push_back(d);


}

void nacrtajPanel()
{
	glColor3f(0.80f, 0.80f, 0.80f);
	glBegin(GL_POLYGON);
	for (int j = 0; j < panel.size(); j++)
	{
		glVertex3f(panel[j].X(), panel[j].Y(), panel[j].Z());
	}
	glEnd();
}

void pocetnaPozicijaKamere()
{
	//Vector3D pravac = Vector3D::AxisZ;
	CameraPosition = staza[1][0];
	CameraPosition.m_y = cameraH;
	Vector3D pravac  =  - CameraPosition.Cross(LookUp_vector);
	LookAt_vector = CameraPosition + pravac;


}

void Transform(vector<Vector3D>&a, Matrix4x4 &T)
{
	for (int i = 0; i < (int)a.size(); i++)
		a[i] = T.Transform(a[i]);
}

void createMeracBrzine()
{
	int i;
	meracBrzine.resize(5);
	for (i = 0; i < 2; i++)
		meracBrzine[i].resize(4);

	meracBrzine[0][0] = Vector3D(-0.05, -0.01, 0.0);
	meracBrzine[0][1] = Vector3D(0.05, -0.01, 0.0);
	meracBrzine[0][2] = Vector3D(0.05, 0.01, 0.0);
	meracBrzine[0][3] = Vector3D(-0.05, 0.01, 0.0);

	Matrix4x4 MT;
	MT.loadTranslate(0.0, 0.025, 0.0);
	for (i = 1; i < 5; i++)
	{
		meracBrzine[i] = meracBrzine[i - 1];
		Transform(meracBrzine[i], MT);
	}

	MT.loadTranslate(-0.9, -0.8, 0.0);
	for (i = 0; i < 5; i++)
		Transform(meracBrzine[i], MT);
}

void drawMeracBrzine()
{
	glBegin(GL_QUADS);
	for (int i = 0; i < 5; i++)
	{
		if (i < speedIndicator)
			glColor3d(1.0, 0.0, 0.0);
		else
			glColor3d(0.0, 0.0, 0.0);
		glVertex3f(meracBrzine[i][0].m_x, meracBrzine[i][0].m_y, meracBrzine[i][0].m_z);
		glVertex3f(meracBrzine[i][1].m_x, meracBrzine[i][1].m_y, meracBrzine[i][1].m_z);
		glVertex3f(meracBrzine[i][2].m_x, meracBrzine[i][2].m_y, meracBrzine[i][2].m_z);
		glVertex3f(meracBrzine[i][3].m_x, meracBrzine[i][3].m_y, meracBrzine[i][3].m_z);
	}
	glEnd();
}




void createKompas()
{
	kompasOsnova.resize(circle_points_kompas);

	r_kompas = 0.12;

	for (int i = 0; i < circle_points_kompas; i++)
	{
		double angle = 2 * M_PI*i / circle_points_kompas;
		kompasOsnova[i] = Vector3D(r_kompas*cos(angle), r_kompas*sin(angle)*ratio, 0.0);
	}

	Matrix4x4 MT;
	MT.loadTranslate(0.67, -0.64, 0.0);
	Transform(kompasOsnova, MT);
}

void createStrelice(double phi)
{
	kompasStrelica.resize(5);

	r_kompas = 0.12;

	kompasStrelica[0] = Vector3D(0.0, r_kompas, 0.0);
	kompasStrelica[1] = Vector3D(-r_kompas * 0.3, 0.0, 0.0);
	kompasStrelica[2] = Vector3D(r_kompas*0.3, 0.0, 0.0);
	kompasStrelica[3] = Vector3D(0.0, -r_kompas, 0.0);
	kompasStrelica[4] = Vector3D(0.0, 0.0, 0.0);

	Matrix4x4 MR;
	MR.loadRotateZ(-phi);
	Transform(kompasStrelica, MR);
}

void DrawStrelice()
{
	createStrelice(streliceRotate);

	kompasStrelica[0].m_y *= ratio;
	kompasStrelica[1].m_y *= ratio;
	kompasStrelica[2].m_y *= ratio;
	kompasStrelica[3].m_y *= ratio;

	Matrix4x4 MT;
	MT.loadTranslate(0.67, -0.64, 0.0);
	Transform(kompasStrelica, MT);

	glPolygonMode(GL_FRONT, GL_FILL);

	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_POLYGON);
	glVertex3f(kompasStrelica[0].m_x, kompasStrelica[0].m_y, kompasStrelica[0].m_z);
	glVertex3f(kompasStrelica[1].m_x, kompasStrelica[1].m_y, kompasStrelica[1].m_z);
	glVertex3f(kompasStrelica[2].m_x, kompasStrelica[2].m_y, kompasStrelica[2].m_z);
	glEnd();

	glColor3f(0.0, 0.0, 1.0);
	glBegin(GL_POLYGON);
	glVertex3f(kompasStrelica[1].m_x, kompasStrelica[1].m_y, kompasStrelica[1].m_z);
	glVertex3f(kompasStrelica[3].m_x, kompasStrelica[3].m_y, kompasStrelica[3].m_z);
	glVertex3f(kompasStrelica[2].m_x, kompasStrelica[2].m_y, kompasStrelica[2].m_z);
	glEnd();
}

void crtajOsnovuKompasa()
{
	glPolygonMode(GL_FRONT, GL_FILL);
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_POLYGON);
	for (unsigned int i = 0; i < kompasOsnova.size(); i++)
		glVertex3f(kompasOsnova[i].m_x, kompasOsnova[i].m_y, kompasOsnova[i].m_z);
	glEnd();

}


void OznakeKompas()
{
	glPolygonMode(GL_FRONT, GL_FILL);


	Vector3D kompasCentar = kompasStrelica[1] + (kompasStrelica[2] - kompasStrelica[1]) * 0.5;
	Vector3D kompasN = kompasCentar + (kompasStrelica[0] - kompasCentar) * 1.25;
	Vector3D kompasS = kompasCentar + (kompasStrelica[3] - kompasCentar) * 1.25;


	glColor3f(1.0, 0.0, 0.0);
	glRasterPos2f(kompasN.m_x, kompasN.m_y);
	glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_10, (const unsigned char *)"N");
	glColor3f(0.0, 0.0, 1.0);
	glRasterPos2f(kompasS.m_x, kompasS.m_y);
	glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_10, (const unsigned char *)"S");
}

void RenderString(float x, float y, void *font, double r, double g, double b)
{
	glColor3f(r, g, b);
	glRasterPos2f(x, y);

	char s[100];
	sprintf_s(s, "x = %.2lf\ny = %.2lf\nz = %.2lf", CameraPosition.m_x, CameraPosition.m_y, CameraPosition.m_z);
	glutBitmapString(font, (const unsigned char *)s);
}



void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffers

	gluPerspective(alpha, ratio, 0.1f, 20.0f);

	gluLookAt(CameraPosition.m_x, CameraPosition.m_y, CameraPosition.m_z,
		LookAt_vector.m_x, LookAt_vector.m_y, LookAt_vector.m_z,
		LookUp_vector.m_x, LookUp_vector.m_y, LookUp_vector.m_z);

	glColor3f(0.0, 0.5, 0.0);
	
	crtajStazu(staza);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glDisable(GL_DEPTH_TEST);

	nacrtajPanel();
	
	crtajOsnovuKompasa();
	DrawStrelice();
	OznakeKompas();

	RenderString(-0.6, -0.6, GLUT_BITMAP_TIMES_ROMAN_24, 1.0, 0.0, 0.0);
	drawMeracBrzine();

	glColor3f(0.0, 0.5, 0.0);
	crtajStazu(staza_mapa);
	crtajPozicijuKamere();
	
	glEnable(GL_DEPTH_TEST);

	glutSwapBuffers();  // Swap the front and back frame buffers (double buffering)
}

void reshape(GLsizei width, GLsizei height)
{
	if (height * ratio <= width)
		width = ratio * height;
	else
		height = width / ratio;

	glViewport(0, 0, width, height);
}

void moveForward()
{
	Matrix4x4 MT;
	Vector3D V;
	Vector3D L, T;

	V = LookAt_vector - CameraPosition;
	V.m_y = 0.0;
	V.Normalize();
	V = V * (speedIndicator*MOVING_CONST);
	MT.loadTranslate(V.m_x, V.m_y, V.m_z);

	CameraPosition = MT.Transform(CameraPosition);
	LookAt_vector = MT.Transform(LookAt_vector);
}

void moveBackward()
{
	Matrix4x4 MT;
	Vector3D V;
	Vector3D L, T;

	V = LookAt_vector - CameraPosition;
	V.Normalize();
	V.m_y = 0.0;
	V = -V * (speedIndicator*MOVING_CONST);
	MT.loadTranslate(V.m_x, V.m_y, V.m_z);

	CameraPosition = MT.Transform(CameraPosition);
	LookAt_vector = MT.Transform(LookAt_vector);
}

void speedUp()
{
	speedIndicator++;
	if (speedIndicator == 6)
		speedIndicator--;
}

void speedDown()
{
	speedIndicator--;
	if (speedIndicator == -1)
		speedIndicator++;
}

void turnLeft()
{
	Matrix4x4 MT;
	Matrix4x4 Mr, Mtr1, Mtr2;

	Mtr1.loadTranslate(-CameraPosition.m_x, -CameraPosition.m_y, -CameraPosition.m_z);
	Mtr2.loadTranslate(CameraPosition.m_x, CameraPosition.m_y, CameraPosition.m_z);
	Mr.loadRotateY(ROTATION_CONST);

	MT = Mtr2 * Mr * Mtr1;

	LookAt_vector = MT.Transform(LookAt_vector);

	Vector4D LookUp_vector4d = LookUp_vector;
	LookUp_vector4d.m_w = 0.0;
	LookUp_vector4d = MT.Transform(LookUp_vector4d);
	LookUp_vector = LookUp_vector4d;

	streliceRotate += ROTATION_CONST;


}

void turnRight()
{
	Matrix4x4 MT;
	Matrix4x4 Mr, Mtr1, Mtr2;

	Mtr1.loadTranslate(-CameraPosition.m_x, -CameraPosition.m_y, -CameraPosition.m_z);
	Mtr2.loadTranslate(CameraPosition.m_x, CameraPosition.m_y, CameraPosition.m_z);
	Mr.loadRotateY(-ROTATION_CONST);

	MT = Mtr2 * Mr * Mtr1;

	LookAt_vector = MT.Transform(LookAt_vector);

	Vector4D LookUp_vector4d = LookUp_vector;
	LookUp_vector4d.m_w = 0.0;
	LookUp_vector4d = MT.Transform(LookUp_vector4d);
	LookUp_vector = LookUp_vector4d;

	streliceRotate -= ROTATION_CONST;

}

void lookUp()
{
	Matrix4x4 MT;
	Vector3D f = LookAt_vector - CameraPosition;
	Vector3D w = LookUp_vector.Cross(f);

	w.Normalize();

	//if (rotationUpDown + ROTATION_CONST < 0.5*M_PI)
	//{
	//	MT.loadRotate(CameraPosition, w, -ROTATION_CONST);

	//	LookAt_vector = MT.Transform(LookAt_vector);

	//	Vector4D LookUp_vector4d = LookUp_vector;
	//	LookUp_vector4d.m_w = 0.0;
	//	LookUp_vector4d = MT.Transform(LookUp_vector4d);
	//	LookUp_vector = LookUp_vector4d;

	//	rotationUpDown += ROTATION_CONST;

	//	Matrix4x4 Mr;

	//	Vector3D axis = coordinateSystem[1] - coordinateSystem[0];
	//	Mr.loadRotate(coordinateSystem[0], axis, ROTATION_CONST);

	//	Transform(coordinateSystem, Mr);
	//}
}

void lookDown()
{
	Matrix4x4 MT;
	Vector3D f = LookAt_vector - CameraPosition;
	Vector3D w = LookUp_vector.Cross(f);

	w.Normalize();

	/*if (rotationUpDown + ROTATION_CONST > -0.5*M_PI)
	{
		MT.loadRotate(CameraPosition, w, ROTATION_CONST);

		LookAt_vector = MT.Transform(LookAt_vector);

		Vector4D LookUp_vector4d = LookUp_vector;
		LookUp_vector4d.m_w = 0.0;
		LookUp_vector4d = MT.Transform(LookUp_vector4d);
		LookUp_vector = LookUp_vector4d;

		rotationUpDown -= ROTATION_CONST;

		Matrix4x4 Mr;

		Vector3D axis = coordinateSystem[1] - coordinateSystem[0];
		Mr.loadRotate(coordinateSystem[0], axis, -ROTATION_CONST);

		Transform(coordinateSystem, Mr);
	}*/
}

void KeyboardKeyPressed(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:    //ESC key
		exit(0);
		break;
	case 'w':
		cout << "w pressed -> moving forward" << endl;
		moveForward();
		break;
	case 's':
		cout << "s pressed -> moving backward" << endl;
		moveBackward();
		break;
	case 'u':
		cout << "u pressed -> SpeedUp" << endl;
		speedUp();
		break;
	case 'i':
		cout << "j pressed -> SpeedDown" << endl;
		speedDown();
		break;
	case 52:
		cout << "4 pressed -> turning left" << endl;
		turnLeft();
		break;
	case 54:
		cout << "6 pressed -> turning right" << endl;
		turnRight();
		break;
	case 50:
		cout << "2 pressed -> look up" << endl;
		lookDown();
		break;
	case 56:
		cout << "8 pressed -> look down" << endl;
		lookUp();
		break;
	}

	glutPostRedisplay();
}

/* Initialize OpenGL Graphics */
void initGL()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
	glShadeModel(GL_FLAT);

	glEnable(GL_DEPTH_TEST);   // Enable depth testing for z-culling
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);            // Initialize GLUT
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL); // Enable double buffered mode
	glutInitWindowSize(ratio * visina, visina);   // Set the window's initial width & height
	glutInitWindowPosition(50, 50); // Position the window's initial top-left corner
	glutCreateWindow(title);          // Create window with the given title
	napraviStazu();
	pocetnaPozicijaKamere();
	kreirajPanel();
	createMeracBrzine();
	createKompas();

	//createCubeCoordinates(2.0);
	//createPlanePanel();
	//createKompas();
	//createStrelice(streliceRotate);
	//createMeracBrzine();
	//createCoordinates();

	glutDisplayFunc(display);       // Register callback handler for window re-paint event
	glutReshapeFunc(reshape);       // Register callback handler for window re-size event

	glutKeyboardFunc(KeyboardKeyPressed);

	initGL();                       // Our own OpenGL initialization
	glutMainLoop();                 // Enter the infinite event-processing loop
	return 0;
}

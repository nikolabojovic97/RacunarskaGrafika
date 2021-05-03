#include <Gl\freeglut.h>
#include <math.h>
#include "vector3d.h"
#include "vector4d.h"
#include "matrix4x4.h"
#include <vector>

#define M_PI 3.14
using namespace std;

char title[] = "II kolokvijum popravni";

#define MOVING_CONST 0.3
#define ROTATION_CONST 3.14/180.0

Vector3D CameraPosition(50.0, 10.0, 50.0);
#define LOOK_MOVEMENT_CONST 0.1
Vector3D LookAt_vector(0.0, 10.0, 0.0);
Vector3D LookUp_vector(0.0, 1.0, 0.0);


double alpha = 120;




#define SPEED_INCREMENT 0.005;
double speed = 0;

double streliceRotate = 0.0;
double rotationUpDown = 0.0;



double visina = 480;
double ratio = 16.0 / 9.0;



vector<vector<Vector3D>> horizon;


double scale(double min, double max, double a, double b, double x)
{
	return (b - a)*(x - min) / (max - min) + a;
}

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



void Transform(vector<Vector3D>&a, Matrix4x4 &T)
{
	for (int i = 0; i < (int)a.size(); i++)
		a[i] = T.Transform(a[i]);
}


void createHorison()
{
	Vector3D a(-0.5, 0.4, 0);
	Vector3D b(0.5, 0.4, 0);
	vector<Vector3D> line;
	line.push_back(a);
	line.push_back(b);
	horizon.push_back(line);
	Matrix4x4 Mt;
	Mt.loadTranslate(0, -0.2, 0);
	for (int i = 0; i < 4; i++)
	{
		horizon.push_back(Mt.Transform(horizon[i]));
	}
}

void RenderString(float x, float y, void *font, double r, double g, double b)
{
	glColor3f(r, g, b);
	glRasterPos2f(x, y);

	char s[100];
	sprintf_s(s, "x = %.2lf\ny = %.2lf\nz = %.2lf", CameraPosition.m_x, CameraPosition.m_y, CameraPosition.m_z);
	glutBitmapString(font, (const unsigned char *)s);
}

void pushVertex(Vector3D &v)
{
	glVertex3f(v.X(), v.Y(), v.Z());
}

void pushVertexRatio(Vector3D &v)
{
	glVertex3f(v.X(), v.Y() * ratio, v.Z());
}

void drawGround()
{
	glColor3f(0.4, 0.4, 0.4);

	Vector3D a(-100, 0, -100);
	Vector3D b(100, 0, -100);

	Vector3D addZ(0, 0, 1);
	for (int i = 0; i < 200; i++)
	{
		glBegin(GL_LINES);
		pushVertex(a);
		pushVertex(b);
		glEnd();
		a = a + addZ;
		b = b + addZ;
	}

	Vector3D c(-100, 0, -100);
	Vector3D d(-100, 0, 100);
	Vector3D addX(1, 0, 0);

	for (int i = 0; i < 200; i++)
	{
		glBegin(GL_LINES);
		pushVertex(c);
		pushVertex(d);
		glEnd();
		c = c + addX;
		d = d + addX;
	}

}
void drawHorizon()
{
	for (int i = 0; i < horizon.size(); i++)
	{
		glBegin(GL_LINES);
		pushVertexRatio(horizon[i][0]);
		pushVertexRatio(horizon[i][1]);
		glEnd();
	}
}


void crtajBrzinu()
{
	Vector3D start(-0.7, speed * 3, 0);
	Vector3D end(-0.65, speed * 3, 0);
	glBegin(GL_LINES);
	pushVertex(start);
	pushVertex(end);
	glEnd();
}

void crtajVisinu()
{
	double h = CameraPosition.Y();
	h = scale(0, 20, -0.65, 0.65, h);
	Vector3D start(0.7, h, 0);
	Vector3D end(0.65, h, 0);
	glBegin(GL_LINES);
	pushVertex(start);
	pushVertex(end);
	glEnd();
}

void crtajUspravne()
{
	Vector3D gore(0, 0.7, 0);
	Vector3D dole(0, -0.7, 0);
	Vector3D a, b, c(0.7, 0,0);

	a = gore - c;
	b = dole - c;
	glBegin(GL_LINES);
	pushVertex(a);
	pushVertex(b);
	glEnd();

	a = gore + c;
	b = dole + c;
	glBegin(GL_LINES);
	pushVertex(a);
	pushVertex(b);
	glEnd();
	crtajBrzinu();
	crtajVisinu();
}

void crtajNisan()
{
	Vector3D gore(0, 0.1, 0);
	Vector3D dole(0, -0.1, 0);
	Vector3D levo(-0.1, 0, 0);
	Vector3D desno(0.1, 0, 0);

	glBegin(GL_LINES);
	pushVertex(gore);
	pushVertex(dole);
	glEnd();

	glBegin(GL_LINES);
	pushVertex(levo);
	pushVertex(desno);
	glEnd();

}


void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffers

	gluPerspective(alpha, ratio, 0.1f, 20.0f);

	gluLookAt(CameraPosition.m_x, CameraPosition.m_y, CameraPosition.m_z,
		LookAt_vector.m_x, LookAt_vector.m_y, LookAt_vector.m_z,
		LookUp_vector.m_x, LookUp_vector.m_y, LookUp_vector.m_z);

	drawGround();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glDisable(GL_DEPTH_TEST);

	glColor3f(0.0, 1.0, 0.0);
	crtajUspravne();
	drawHorizon();

	glColor3f(1.0, 0.0, 0.0);
	crtajNisan();

	RenderString(-0.95, -0.6, GLUT_BITMAP_HELVETICA_18, 1.0, 0.0, 0.0);

	glColor3f(0.0, 0.5, 0.0);
	
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
	//V.m_y = 0.0;
	V.Normalize();
	V = V * (speed + MOVING_CONST);
	MT.loadTranslate(V.m_x, V.m_y, V.m_z);

	CameraPosition = MT.Transform(CameraPosition);
	LookAt_vector = MT.Transform(LookAt_vector);
}


void speedUp()
{
	speed += SPEED_INCREMENT;
	if (speed > 0.2)
	{
		speed = 0.2;
	}
}

void speedDown()
{
	speed -= SPEED_INCREMENT;
	if (speed < -0.2)
	{
		speed = -0.2;
	}
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

}

void lookUp()
{
	Matrix4x4 MT;
	Vector3D f = LookAt_vector - CameraPosition;
	Vector3D w = LookUp_vector.Cross(f);

	w.Normalize();

	//if (rotationUpDown + ROTATION_CONST < 0.5*M_PI)
	//{

		MT = Matrix4x4::CreateRotationMatrix(CameraPosition, w, -ROTATION_CONST);

		LookAt_vector = MT.Transform(LookAt_vector);

		Vector4D LookUp_vector4d = LookUp_vector;
		LookUp_vector4d.m_w = 0.0;
		LookUp_vector4d = MT.Transform(LookUp_vector4d);
		LookUp_vector = LookUp_vector4d;

		rotationUpDown += ROTATION_CONST;

		Matrix4x4 Mt;
		Mt.loadTranslate(0,- ROTATION_CONST*0.5, 0);
		horizon = Mt.Transform(horizon);


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

	//if (rotationUpDown + ROTATION_CONST > -0.5*M_PI)
	//{
	MT = Matrix4x4::CreateRotationMatrix(CameraPosition, w, ROTATION_CONST);

	LookAt_vector = MT.Transform(LookAt_vector);

	Vector4D LookUp_vector4d = LookUp_vector;
	LookUp_vector4d.m_w = 0.0;
	LookUp_vector4d = MT.Transform(LookUp_vector4d);
	LookUp_vector = LookUp_vector4d;

	rotationUpDown -= ROTATION_CONST;

	///*	Matrix4x4 Mr;

	//	Vector3D axis = coordinateSystem[1] - coordinateSystem[0];
	//	Mr.loadRotate(coordinateSystem[0], axis, -ROTATION_CONST);

	//	Transform(coordinateSystem, Mr);*/
	//}

	Matrix4x4 Mt;
	Mt.loadTranslate(0, ROTATION_CONST*0.5, 0);
	horizon = Mt.Transform(horizon);

}

void roll(int side)
{

	Matrix4x4 MT;
	Vector3D f = LookAt_vector - CameraPosition;

	f.Normalize();

	MT = Matrix4x4::CreateRotationMatrix(CameraPosition, f, ROTATION_CONST * side);
	Vector4D LookUp_vector4d = LookUp_vector;
	LookUp_vector4d.m_w = 0.0;
	LookUp_vector4d = MT.Transform(LookUp_vector4d);
	LookUp_vector = LookUp_vector4d;


	Matrix4x4 MrotateH;
	MrotateH.loadRotateZ(-ROTATION_CONST * side);
	horizon = MrotateH.Transform(horizon);
}




void KeyboardKeyPressed(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:    //ESC key
		exit(0);
		break;
	case 'w':
		cout << "w pressed -> moving forward, speed up" << endl;
		speedUp();
		break;
	case 's':
		cout << "s pressed -> moving forward, speed down" << endl;
		speedDown();
		break;
	case 'a':
		cout << "a pressed -> roll left" << endl;
		roll(1);
		break;
	case 'd':
		cout << "a pressed -> roll right" << endl;
		roll(-1);
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



void timer(int v)
{
	moveForward();
	glutTimerFunc(1000 / 30, timer, v);
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

	createHorison();

	//createCubeCoordinates(2.0);
	//createPlanePanel();
	//createKompas();
	//createStrelice(streliceRotate);
	//createMeracBrzine();
	//createCoordinates();

	glutDisplayFunc(display);       // Register callback handler for window re-paint event
	glutReshapeFunc(reshape);       // Register callback handler for window re-size event

	glutKeyboardFunc(KeyboardKeyPressed);
	glutTimerFunc(1000 / 30, timer, 1);

	initGL();                       // Our own OpenGL initialization
	glutMainLoop();                 // Enter the infinite event-processing loop
	return 0;
}

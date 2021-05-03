#include "Vector2D.h"
#include <math.h>

using namespace std;

Vector2D::Vector2D() {}
Vector2D::Vector2D(const Vector2D &A) {
	Vector2D::Set(A.x, A.y);
}

Vector2D Vector2D::operator+(Vector2D& A) {
	return Vector2D(x + A.x, y + A.y);
}
Vector2D Vector2D::operator-(Vector2D& A) {
	return Vector2D(x - A.x, y - A.y);
}
double Vector2D::operator*(Vector2D& A) {
	return X() * A.x + Y() * A.y;
}
void Vector2D::operator+=(Vector2D& A) {
	x += A.x;
	y += A.y;
}
double Vector2D::CrossProductIntensity(Vector2D& A) {
	return X() * A.y - Y() * A.x;
}

void Vector2D::operator*=(double k) {
	x *= k;
	y *= k;
}
Vector2D Vector2D::operator*(double k) {
	return Vector2D(x * k, y * k);
}

double Vector2D::Intensity() {
	return sqrt(x * x + y * y);
}

Vector2D Vector2D::Normalize() {
	double intensity = Intensity();
	Vector2D v(0.0, 0.0);
	if (intensity > 1.0e-7) {
		v.x = x / intensity;
		v.y = y / intensity;
	}
	return v;
}

Vector2D Vector2D::Transpose() {
	return Vector2D(-y, x);
}

double Vector2D::ProjectionIntensity(Vector2D& A) {
	return ProjectionVector(A).Intensity();
}

Vector2D Vector2D::ProjectionVector(Vector2D& A) {
	Vector2D B(x, y);
	double cosPhi = (A * B) / (Intensity() * A.Intensity());
	double sinPhi = CrossProductIntensity(A) / (Intensity() * A.Intensity());

	double Phi = atan2(sinPhi, cosPhi);
	double intensity = Intensity() * cos(Phi);

	return Normalize() * intensity;
}

ostream& operator <<(ostream& out, const Vector2D& A) {
	cout << "(" << A.x << ", " << A.y << ")";
	return out;
}



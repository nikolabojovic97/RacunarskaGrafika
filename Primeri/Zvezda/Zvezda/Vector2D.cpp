#include "Vector2D.h"

using namespace std;

Vector2D::Vector2D(){}
Vector2D::Vector2D(const Vector2D& A) {
	Set(A.x, A.y);
}

Vector2D Vector2D::operator +(Vector2D& A) {
	return Vector2D(x + A.x, y + A.y);
}
Vector2D Vector2D::operator -(Vector2D& A) {
	return Vector2D(x - A.x, y - A.y);
}
double Vector2D::operator *(Vector2D& A) {
	return x * A.x + y * A.y;
}
void Vector2D::operator +=(Vector2D& A) {
	Set(x + A.x, y + A.y);
}
double Vector2D::CrossProductIntensity(Vector2D& A) {
	return x * A.y - y * A.x;
}

void Vector2D::operator *=(double k) {
	Set(x * k, y * k);
}
Vector2D Vector2D::operator *(double k) {
	return Vector2D(k * x, k * y);
}

double Vector2D::Intensity() {
	return sqrt(x * x + y * y);
}
Vector2D Vector2D::Normalize() {
	return Vector2D(x / Intensity(), y / Intensity());
}
Vector2D Vector2D::Transpose() {
	return Vector2D(-y, x);
}


Vector2D Vector2D::ProjectionVector(Vector2D& A) {
	Vector2D B(x, y);

	double cosFi = (A * B) / (A.Intensity() * B.Intensity());
	double sinFi = A.CrossProductIntensity(B) / (A.Intensity() * B.Intensity());

	double fi = atan2(sinFi, cosFi);

	double intensity = B.Intensity() * cos(fi);

	return B.Normalize() * intensity;
}

double Vector2D::ProjectionIntensity(Vector2D& A) {
	return ProjectionVector(A).Intensity();
}

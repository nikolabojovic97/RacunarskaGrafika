#include "Vector3D.h"

using namespace std;

Vector3D::Vector3D() { }
Vector3D::Vector3D(double x, double y, double z) {
	Set(x, y, z);
}
Vector3D::Vector3D(const Vector3D& A) {
	x = A.x;
	y = A.y;
	z = A.z;
}

double Vector3D::X() {
	return x;
}
double Vector3D::Y() {
	return y;
}
double Vector3D::Z	() {
	return z;
}

Vector3D Vector3D::operator +(Vector3D& A) {
	return Vector3D(x + A.x, y + A.y, z + A.z);
}
void Vector3D::operator +=(Vector3D& A) {
	x += A.x;
	y += A.y;
	z += A.z;
}
Vector3D Vector3D::operator -(Vector3D& A) {
	return Vector3D(x - A.x, y - A.y, z - A.z);
}
double Vector3D::operator *(Vector3D& A) {
	return x * A.x + y * A.y + z * A.z;
}
double Vector3D::CrossProductIntensity(Vector3D& A) {
	return y * A.z + z * A.x + x * A.y - y * A.x - z * A.y - x * A.z;
}

void Vector3D::operator *=(double k) {
	x *= k;
	y *= k;
	z *= k;
}
Vector3D Vector3D::operator *(double k) {
	return Vector3D(x * k, y * k, z * k);
}

double Vector3D::Intensity() {
	return sqrt(x * x + y * y + z * z);
}

Vector3D Vector3D::Normalize() {
	double intensity = Intensity();
	Vector3D A(0, 0, 0);
	if (fabs(intensity) > 1.0e-7) {
		A.x = x / intensity;
		A.y = y / intensity;
		A.z = z / intensity;
	}
	return A;
}

Vector3D Vector3D::ProjectionVector(Vector3D& A) {
	Vector3D B(x, y, z);

	double cosFi = (A * B) / (A.Intensity() * B.Intensity());
	double sinFi = A.CrossProductIntensity(B) / (A.Intensity() * B.Intensity());
	double fi = atan2(sinFi, cosFi);
	double intensity = B.Intensity() * cos(fi);
	
	return A.Normalize() * intensity;
}

double Vector3D::ProjectionIntensity(Vector3D& A) {
	return ProjectionVector(A).Intensity();
}

ostream& operator <<(ostream& out, const Vector3D& A) {
	cout << "(" << A.x << "," << A.y << "," << A.z << ")";
	return out;
}



#include "Vector4D.h"
#include <iostream>

Vector4D::Vector4D() {
	Zero();
}
Vector4D::Vector4D(double x, double y, double z, double t) {
	Set(x, y, z, t);
}
Vector4D::Vector4D(Vector4D& A) {
	Set(A.x, A.y, A.z, A.t);	
}

Vector4D Vector4D::operator +(Vector4D& A) {
	Vector4D B(x + A.x, y + A.y, z + A.z, t + A.t);
	return B;
}
void Vector4D::operator +=(Vector4D& A) {
	x += A.x;
	y += A.y;
	z += A.z;
	t += A.t;
}
Vector4D Vector4D::operator -(Vector4D& A) {
	Vector4D B(x - A.x, y - A.y, z - A.z, t - A.t);
	return B;
}
void Vector4D::operator -=(Vector4D& A) {
	x -= A.x;
	y -= A.y;
	z -= A.z;
	t -= A.t;
}
double Vector4D::operator *(Vector4D& A) {
	return x * A.x + y * A.y + z * A.z + t * A.t;
}
double Vector4D::CrossProductIntensity(Vector4D& A) {
	return x * A.y + y * A.z + z * A.t + t * A.x - z * A.y - t * A.z - x * A.t - y * A.x;
}

Vector4D Vector4D::operator *(double k) {
	Vector4D A(x * k, y * k, z * k, y * k);
	return A;
}
void Vector4D::operator *=(double k) {
	x *= k;
	y *= k;
	z *= k;
	t *= k;
}

double Vector4D::Intensity() {
	return sqrt(x*x + y*y + z*z + t*t);
}

Vector4D Vector4D::Normalize() {
	double intensity = Intensity();
	Vector4D V(0.0, 0.0, 0.0, 0.0);
	if (intensity > EPSILON) {
		V.x = x / intensity;
		V.y = y / intensity;
		V.z = z / intensity;
		V.t = t / intensity;
	}
	return V;
}

Vector4D Vector4D::ProjectionVector(Vector4D& A) {
	Vector4D B(x, y, z, t);
	
	double cosPhi = (A * B) / (A.Intensity() * B.Intensity());
	double sinPhi = A.CrossProductIntensity(B) / (A.Intensity() * B.Intensity());
	double phi = atan2(sinPhi, cosPhi);
	double intensity = B.Intensity() * cos(phi);
	Vector4D C;
	C = A.Normalize() * intensity;
	return C;
}
double Vector4D::ProjectionIntensity(Vector4D& A) {
	return ProjectionVector(A).Intensity();
}

ostream& operator <<(ostream& out, const Vector4D& A) {
	cout << "(" << A.x << ", " << A.y << ", " << A.z << ", " << A.t << ")";
	return out;
}


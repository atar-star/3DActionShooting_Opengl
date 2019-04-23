#include <math.h>
#include <sstream>
#include <iostream>
#include <GL/glut.h>
#include <ctype.h>
#include <Vectors.h>

Vec3::Vec3(double x, double y, double z) { this->x = x; this->y = y; this->z = z; }
Vec3 Vec3::operator +(Vec3 &vec) { return Vec3(this->x + vec.x, this->y + vec.y, this->z + vec.z); }
Vec3 Vec3::operator -(Vec3 &vec) { return Vec3(this->x - vec.x, this->y - vec.y, this->z - vec.z); }
Vec3 Vec3::operator *(double f) { return Vec3(this->x*f, this->y*f, this->z*f); }
Vec3 Vec3::operator /(double f) { return Vec3(this->x / f, this->y / f, this->z / f); }
Vec3 Vec3::operator *(Vec3 &vec) { return Vec3(this->x*vec.x, this->y*vec.y, this->z*vec.z); }
Vec3 Vec3::operator /(Vec3 &vec) {
	Vec3 div(this->x, this->y, this->z);
	if (vec.x != 0) div.x / vec.x;
	if (vec.y != 0) div.y / vec.y;
	if (vec.z != 0) div.z / vec.z;
	return div;
}
Vec3 Vec3::operator =(Vec3 &vec) { return Vec3(this->x = vec.x, this->y = vec.y, this->z = vec.z); }
Vec3 Vec3::operator ==(Vec3 &vec) { return (this->x == vec.x && this->y == vec.y && this->z == vec.z); }
Vec3 Vec3::operator !=(Vec3 &vec) { return (this->x != vec.x || this->y != vec.y || this->z != vec.z); }
void Vec3::Set(double x, double y, double z) { this->x = x; this->y = y; this->z = z; }

double Vec3::LengthSq() { return x*x + y*y + z*z; }
double Vec3::Length() { return sqrtf(LengthSq()); }

Vec3 Vec3::Normalize() {
	double m = LengthSq();
	if (m > 0.0f) m = 1.0f / m;	else m = 0.0f;
	return Vec3(x*m, y*m, z*m);
}

double Vec3::Dot(Vec3 &vec) {
	return (this->x*vec.x + this->y*vec.y + this->z*vec.z);
}

Vec3 Vec3::Cross(Vec3 &vec) {
	return Vec3(
		this->y*vec.z - this->z*vec.y,
		this->z*vec.x - this->x*vec.z,
		this->x*vec.y - this->y*vec.x);
}

string Vec3::Debug() {
	char tmp[256];
	sprintf(tmp, "(x,y,z)=(%.2f,%.2f,%.2f)", x, y, z);
	string ret = tmp;
	return ret;
}

Vec3 Vec3::to_rad() {
	return Vec3(DegToRad(this->x), DegToRad(this->y), DegToRad(this->z));
}

Vec3 Vec3::Clip(char axis, double min, double max) {
	Vec3 v(this->x, this->y, this->z);
	switch (axis) {
	case 'x':
	case 'X':
		if (this->x < min) v.x = min; else if (this->x > max) v.x = max;
		break;

	case 'y':
	case 'Y':
		if (this->y < min) v.y = min; else if (this->y > max) v.y = max;
		break;

	case 'z':
	case 'Z':
		if (this->z < min) v.z = min; else if (this->z > max) v.z = max;
		break;
	}

	return v;
}

bool Vec3::isZero() {
	return this->x == 0 && this->y == 0 && this->z == 0;
}

Vec4::Vec4(double x, double y, double z, double w) { this->x = x; this->y = y; this->z = z; this->w = w; }
Vec4::Vec4(Vec3 v, double w) { this->x = v.x; this->y = v.y; this->z = v.z; this->w = w; }
Vec4 Vec4::operator +(Vec4 &vec) { return Vec4(this->x + vec.x, this->y + vec.y, this->z + vec.z, this->w + vec.w); }
Vec4 Vec4::operator -(Vec4 &vec) { return Vec4(this->x - vec.x, this->y - vec.y, this->z - vec.z, this->w - vec.w); }
Vec4 Vec4::operator *(Vec4 &vec) { return Vec4(this->x*vec.x, this->y*vec.y, this->z*vec.z, this->w*vec.w); }
Vec4 Vec4::operator /(Vec4 &vec) {
	Vec4 div(this->x, this->y, this->z, this->w);
	if (vec.x != 0) div.x / vec.x;
	if (vec.y != 0) div.y / vec.y;
	if (vec.z != 0) div.z / vec.z;
	if (vec.w != 0) div.w / vec.w;
	return div;
}
Vec4 Vec4::operator *(double f) { return Vec4(this->x*f, this->y*f, this->z*f, this->w*f); }
Vec4 Vec4::operator /(double f) { return Vec4(this->x / f, this->y / f, this->z / f, this->w / f); }
Vec4 Vec4::operator =(Vec4 &vec) { return Vec4(this->x = vec.x, this->y = vec.y, this->z = vec.z, this->w = vec.w); }
Vec4 Vec4::operator ==(Vec4 &vec) { return (this->x == vec.x && this->y == vec.y && this->z == vec.z && this->w == vec.w); }
Vec4 Vec4::operator !=(Vec4 &vec) { return (this->x != vec.x || this->y != vec.y || this->z != vec.z || this->w != vec.w); }
void Vec4::Set(double x, double y, double z, double w) { this->x = x; this->y = y; this->z = z; this->w = w; }
void Vec4::Set(Vec3 pos) { this->x = pos.x; this->y = pos.y; this->z = pos.z; this->w = 1.0; }

void Vec4::Set(int i, double param) {
	if (i == 0)      x = param;
	else if (i == 1) y = param;
	else if (i == 2) z = param;
	else if (i == 3) w = param;
}

double Vec4::Get(int i) {
	if (i == 0)      return x;
	else if (i == 1) return y;
	else if (i == 2) return z;
	else if (i == 3) return w;
	else            return 0;
}

double Vec4::LengthSq() { return x*x + y*y + z*z + w*w; }
double Vec4::Length() { return sqrt(LengthSq()); }

Vec4 Vec4::Normalize() {
	double m = LengthSq();
	if (m > 0.0f) m = 1.0f / m; else m = 0.0f;
	return Vec4(x *= m, y *= m, z *= m, w *= m);
}

double Vec4::Dot(Vec4 &vec) {
	return (this->x*vec.x + this->y*vec.y + this->z*vec.z + this->w*vec.w);
}

Vec3 Vec4::Pos() {
	return Vec3(this->x, this->y, this->z);
}

Vec4 Vec4::operator *(Mat44 &v) {
	Vec4 ret;
	ret.x = this->x * v.col[0].x + this->y * v.col[1].x + this->z * v.col[2].x + this->w * v.col[3].x;
	ret.y = this->x * v.col[0].y + this->y * v.col[1].y + this->z * v.col[2].y + this->w * v.col[3].y;
	ret.z = this->x * v.col[0].z + this->y * v.col[1].z + this->z * v.col[2].z + this->w * v.col[3].z;
	ret.w = this->x * v.col[0].w + this->y * v.col[1].w + this->z * v.col[2].w + this->w * v.col[3].w;
	return ret;
}

string Vec4::Debug() {
	char tmp[256];
	sprintf(tmp, "(x,y,z,w)=(%.2f,%.2f,%.2f,%.2f)", x, y, z, w);
	string ret = tmp;
	return ret;
}

Mat44::Mat44(Vec4 v0, Vec4 v1, Vec4 v2, Vec4 v3) {
	col[0] = v0; col[1] = v1; col[2] = v2; col[3] = v3;
}

Mat44 Mat44::operator *(Mat44 &m) {
	Mat44 t;
	for (int r = 0; r < 4; r++) {
		for (int c = 0; c < 4; c++) {
			double f = 0;

			f += (Get(0, r) * m.Get(c, 0));
			f += (Get(1, r) * m.Get(c, 1));
			f += (Get(2, r) * m.Get(c, 2));
			f += (Get(3, r) * m.Get(c, 3));

			t.Set(c, r, f);
		}
	}
	return t;
}

Vec3 Mat44::operator *(Vec3 &v) {
	Vec3 ret;
	ret.x = (v.x * this->col[0].x) + (v.y * this->col[1].x) + (v.z * this->col[2].x) + (this->col[3].x);
	ret.y = (v.x * this->col[0].y) + (v.y * this->col[1].y) + (v.z * this->col[2].y) + (this->col[3].y);
	ret.z = (v.x * this->col[0].z) + (v.y * this->col[1].z) + (v.z * this->col[2].z) + (this->col[3].z);
	return ret;
}

Vec4 Mat44::operator *(Vec4 &v) {
	Vec4 ret;
	ret.x = v.x * this->col[0].x + v.y * this->col[1].x + v.z * this->col[2].x + v.w * this->col[3].x;
	ret.y = v.x * this->col[0].y + v.y * this->col[1].y + v.z * this->col[2].y + v.w * this->col[3].y;
	ret.z = v.x * this->col[0].z + v.y * this->col[1].z + v.z * this->col[2].z + v.w * this->col[3].z;
	ret.w = v.x * this->col[0].w + v.y * this->col[1].w + v.z * this->col[2].w + v.w * this->col[3].w;
	return ret;
}

double Mat44::Get(int c, int r) { return col[c].Get(r); }
void Mat44::Set(int c, int r, double i) { col[c].Set(r, i); }

Mat44 Mat44::Identity() {
	return Mat44(Vec4(1, 0, 0, 0), Vec4(0, 1, 0, 0), Vec4(0, 0, 1, 0), Vec4(0, 0, 0, 1));
}

Mat44 Mat44::Translate(double x, double y, double z) {
	Mat44 ret;
	ret = ret.Identity();
	ret.col[3].x = x;
	ret.col[3].y = y;
	ret.col[3].z = z;
	return ret;
}
Mat44 Mat44::Translate(Vec3 v) {
	Mat44 ret;
	ret = ret.Identity();
	ret.col[3].x = v.x;
	ret.col[3].y = v.y;
	ret.col[3].z = v.z;
	return ret;
}

Mat44 Mat44::Rotate(char axis, double rad) {
	Mat44 ret;
	double sinA = sinf(rad);
	double cosA = cosf(rad);
	switch (axis) {
	case 'x':
	case 'X':
		ret.col[0].x = 1.0F; ret.col[1].x = 0.0F; ret.col[2].x = 0.0F;
		ret.col[0].y = 0.0F; ret.col[1].y = cosA; ret.col[2].y = -sinA;
		ret.col[0].z = 0.0F; ret.col[1].z = sinA; ret.col[2].z = cosA;
		break;

	case 'y':
	case 'Y':
		ret.col[0].x = cosA; ret.col[1].x = 0.0F; ret.col[2].x = sinA;
		ret.col[0].y = 0.0F; ret.col[1].y = 1.0F; ret.col[2].y = 0.0F;
		ret.col[0].z = -sinA; ret.col[1].z = 0.0F; ret.col[2].z = cosA;
		break;

	case 'z':
	case 'Z':
		ret.col[0].x = cosA; ret.col[1].x = -sinA; ret.col[2].x = 0.0F;
		ret.col[0].y = sinA; ret.col[1].y = cosA; ret.col[2].y = 0.0F;
		ret.col[0].z = 0.0F; ret.col[1].z = 0.0F; ret.col[2].z = 1.0F;
		break;
	}

	ret.col[0].w = 0.0F; ret.col[1].w = 0.0F; ret.col[2].w = 0.0F;
	ret.col[3].x = 0.0F;
	ret.col[3].y = 0.0F;
	ret.col[3].z = 0.0F;
	ret.col[3].w = 1.0F;

	return ret;
}

Mat44 Mat44::YawPitchRoll(double y, double x, double z) {
	Mat44 ret;
	Mat44 mY = ret.Rotate('y', y);
	Mat44 mX = ret.Rotate('x', x);
	Mat44 mZ = ret.Rotate('z', z);
	return mZ*mX*mY;
}

Mat44 Mat44::YawPitchRoll(Vec3 rot) {
	Mat44 ret;
	Mat44 mY = ret.Rotate('y', rot.y);
	Mat44 mX = ret.Rotate('x', rot.x);
	Mat44 mZ = ret.Rotate('z', rot.z);
	return mZ*mX*mY;
}

Mat44 Mat44::Scale(double x, double y, double z) {
	Mat44 ret;
	ret = ret.Identity();
	ret.col[0].x = x;
	ret.col[1].y = y;
	ret.col[2].z = z;
	return ret;
}

string Mat44::Debug() {
	char tmp[1000];
	sprintf(tmp, "%7.3f,%7.3f,%7.3f,%7.3f\n%7.3f,%7.3f,%7.3f,%7.3f\n%7.3f,%7.3f,%7.3f,%7.3f\n5f,%7.3f,%7.3f,%7.3f\n",
		col[0].x, col[1].x, col[2].x, col[3].x, col[0].y, col[1].y, col[2].y, col[3].y,
		col[0].z, col[1].z, col[2].z, col[3].z, col[0].w, col[1].w, col[2].w, col[3].w);

	string ret = tmp;
	return ret;
}


Quaternion::Quaternion(double w, double x, double y, double z) {
	this->w = w; this->x = x; this->y = y; this->z = z;
}

Quaternion::Quaternion(Vec3 ax, double w) {
	this->w = w; this->x = ax.x; this->y = ax.y; this->z = ax.z;
}

Quaternion::Quaternion(Vec4 v) {
	this->w = v.w; this->x = v.x; this->y = v.y; this->z = v.z;
}

void Quaternion::setParam(Quaternion q) {
	this->w = q.w; this->x = q.x; this->y = q.y; this->z = q.z;
}

Vec3 Quaternion::getImaginary() {
	return Vec3(this->x, this->y, this->z);
}

double Quaternion::getReal() {
	return this->w;
}

Vec4 Quaternion::toVec4() {
	return Vec4(this->x, this->y, this->z, this->w);
}

Mat44 Quaternion::qtor(void) {
	Mat44 r;
	double x2 = this->x * this->x * 2.0;
	double y2 = this->y * this->y * 2.0;
	double z2 = this->z * this->z * 2.0;
	double xy = this->x * this->y * 2.0;
	double yz = this->y * this->z * 2.0;
	double zx = this->z * this->x * 2.0;
	double xw = this->x * this->w * 2.0;
	double yw = this->y * this->w * 2.0;
	double zw = this->z * this->w * 2.0;

	r.col[0].x = 1.0 - y2 - z2;
	r.col[0].y = xy + zw;
	r.col[0].z = zx - yw;

	r.col[1].x = xy - zw;
	r.col[1].y = 1.0 - z2 - x2;
	r.col[1].z = yz + xw;

	r.col[2].x = zx + yw;
	r.col[2].y = yz - xw;
	r.col[2].z = 1.0 - x2 - y2;

	r.col[0].w = r.col[1].w = r.col[2].w = r.col[3].x = r.col[3].y = r.col[3].z = 0.0;
	r.col[3].w = 1.0;
	return r;
}

string Quaternion::Debug() {
	char tmp[256];
	sprintf(tmp, "(w,x,y,z)=(%.2f,%.2f,%.2f,%.2f)", w, x, y, z);
	string ret = tmp;
	return ret;
}

Quaternion Quaternion::qrot(Vec3 ax, double rad) {
	double ar = rad*M_PI / 2.0;
	Quaternion dq(cos(ar), ax.x * sin(ar), ax.y * sin(ar), ax.z * sin(ar)), th(this->w, this->x, this->y, this->z);
	Quaternion q = qmul(dq, th);
	return q;
}

Vec4 Quaternion::getAxis() {
	Vec4 axis;
	double wc = acos(this->w);
	double wcs = asin(wc);
	axis.x = this->x / wcs;
	axis.y = this->y / wcs;
	axis.z = this->z / wcs;
	axis.w = wc * 2;
	if (axis.w < 0) axis.w += 360;	else if (axis.w > 360) axis.w -= 360;
	cout << wc << ", " << wcs << "|(" << axis.x << ", " << axis.y << ", " << axis.z << ", " << axis.w << ", " << ")" << endl;
	return axis;
}

Quaternion qmul(Quaternion p, Quaternion q){
	Quaternion r;
	r.w = p.w * q.w - p.x * q.x - p.y * q.y - p.z * q.z;
	r.x = p.w * q.x + p.x * q.w + p.y * q.z - p.z * q.y;
	r.y = p.w * q.y - p.x * q.z + p.y * q.w + p.z * q.x;
	r.z = p.w * q.z + p.x * q.y - p.y * q.x + p.z * q.w;
	return r;
}
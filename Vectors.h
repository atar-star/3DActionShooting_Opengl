#pragma once

#include <string>
#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;

struct Mat44;
struct Quaternion;

struct Vec3 {
	double x, y, z;
	Vec3(double x = 0, double y = 0, double z = 0);
	Vec3 operator +(Vec3 &vec);
	Vec3 operator -(Vec3 &vec);
	Vec3 operator *(double);
	Vec3 operator /(double);
	Vec3 Vec3::operator *(Vec3 &vec);
	Vec3 Vec3::operator /(Vec3 &vec);
	Vec3 operator =(Vec3 &vec);
	Vec3 operator ==(Vec3 &vec);
	Vec3 operator !=(Vec3 &vec);
	void Set(double, double, double);
	Vec3 to_rad(void);
	Vec3 Clip(char axis, double min, double max);

	double LengthSq(void);
	double Length(void);

	Vec3 Normalize(void);
	double Dot(Vec3 &vec);
	Vec3 Cross(Vec3 &vec);

	string Debug(void);
	bool isZero(void);
};

struct Vec4 {
	double x, y, z, w;
	Vec4(double x = 0, double y = 0, double z = 0, double w = 0);
	Vec4(Vec3 v, double w = 0);
	Vec4 operator +(Vec4 &vec);
	Vec4 operator -(Vec4 &vec);
	Vec4 operator *(Vec4 &vec);
	Vec4 operator /(Vec4 &vec);
	Vec4 operator *(double f);
	Vec4 operator /(double f);
	Vec4 operator =(Vec4 &vec);
	Vec4 operator ==(Vec4 &vec);
	Vec4 operator !=(Vec4 &vec);
	Vec4 operator *(Mat44 &v);

	void Set(double x, double y, double z, double w);
	void Set(Vec3 pos);
	void Set(int i, double param);
	double Get(int i);

	Vec3 Pos();

	double LengthSq();
	double Length();

	Vec4 Normalize();

	double Dot(Vec4 &vec);

	string Debug(void);
};

struct Mat44 {
	Vec4 col[4];

	Mat44(Vec4 v0 = Vec4(1, 0, 0, 0), Vec4 v1 = Vec4(0, 1, 0, 0), Vec4 v2 = Vec4(0, 0, 1, 0), Vec4 v3 = Vec4(0, 0, 0, 1));

	Mat44 operator *(Mat44 &m);
	Vec3 operator *(Vec3 &v);
	Vec4 operator *(Vec4 &v);

	double Get(int c, int r);
	void Set(int c, int r, double i);

	Mat44 Identity();

	Mat44 Translate(double x, double y, double z);
	Mat44 Translate(Vec3 v);

	Mat44 Rotate(char axis, double rad);

	Mat44 YawPitchRoll(double y, double x, double z);
	Mat44 YawPitchRoll(Vec3 rot);

	Mat44 Scale(double x, double y, double z);

	string Debug(void);
};

struct Quaternion {

	double w, x, y, z;
	Quaternion(double w = 1, double x = 0, double y = 0, double z = 0);
	Quaternion(Vec3,double w = 1);
	Quaternion(Vec4);

	void setParam(Quaternion);
	Vec3 getImaginary(void);
	double getReal(void);
	Vec4 toVec4(void);

	Mat44 qtor(void);

	Quaternion qrot(Vec3, double);

	Vec4 getAxis(void);

	string Debug(void);
};

Quaternion qmul(Quaternion, Quaternion);

static double DegToRad(double deg) {
	return deg* M_PI / 180.0;
}
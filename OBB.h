#pragma once

#include <Vectors.h>

class OBBCube {
public:
	Vec3 pos;    // íÜêSç¿ïW
	Vec3 radius; // îºåa
	Vec3 rot;    // âÒì]äpìx
	Vec3 axisX;  // ï™ó£é≤X
	Vec3 axisY;  // ï™ó£é≤Y
	Vec3 axisZ;  // ï™ó£é≤Z
	Vec3 vel;
	Vec4 color;
	float weight;
	OBBCube() {};

	OBBCube(Vec3 p, Vec3 r, Vec3 a, Vec4 col, float wei) {
		pos = p;
		radius = r;
		rot = a;
		color = col;
		weight = wei;
	}

	void setPos(Vec3 p) {
		pos.x = p.x;
		pos.y = p.y;
		pos.z = p.z;
	}

	void setColor(Vec4 col) {
		color = col;
	}

	Vec3 GetMinVec3() {
		return Vec3(
			pos.x - radius.x,
			pos.y - radius.y,
			pos.z - radius.z);
	}
	Vec3 GetMaxVec3() {
		return Vec3(
			pos.x + radius.x,
			pos.y + radius.y,
			pos.z + radius.z);
	}

	void UpdateAxisAll() {
		Mat44 mRot = Mat44().YawPitchRoll(DegToRad(rot.y), DegToRad(rot.x), DegToRad(rot.z));
		axisX = mRot*Vec3(1, 0, 0);
		axisY = mRot*Vec3(0, 1, 0);
		axisZ = mRot*Vec3(0, 0, 1);
	}

	void DrawOBBCube(bool);
};

bool CompareLengthOBB(OBBCube&, OBBCube&, Vec3, Vec3);
bool IsCollideBoxOBB(OBBCube cA, OBBCube cB);
Vec3 OBBvsPlane(OBBCube my, OBBCube pl);
void DrawOBBCube(Vec3 pos, Vec3 radius, Vec3 rot, float red, float green, float blue, float alpha);

void setGLColor(GLfloat color[4], float r, float g, float b, float a);
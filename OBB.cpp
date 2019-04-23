//#define OBB
#ifndef OBB
#include <math.h>
#include <sstream>
#include <iostream>
#include <GL/glut.h>
#include <ctype.h>
#include <OBB.h>

bool CompareLengthOBB(OBBCube &cA, OBBCube &cB, Vec3 vSep, Vec3 vDistance) {
	// 分離軸上のAからBの距離
	float length = fabsf(vSep.Dot(vDistance));

	// 分離軸上でAから最も遠いAの頂点までの距離
	float lenA = fabsf(cA.axisX.Dot(vSep)*cA.radius.x)
			   + fabsf(cA.axisY.Dot(vSep)*cA.radius.y)
			   + fabsf(cA.axisZ.Dot(vSep)*cA.radius.z);

	// 分離軸上でBから最も遠いBの頂点までの距離
	float lenB = fabsf(cB.axisX.Dot(vSep)*cB.radius.x) 
			   + fabsf(cB.axisY.Dot(vSep)*cB.radius.y) 
			   + fabsf(cB.axisZ.Dot(vSep)*cB.radius.z);

	if (length > lenA + lenB) return false;
	return true;
}

bool IsCollideBoxOBB(OBBCube cA, OBBCube cB) {
	// 中心間の距離ベクトル算出
	Vec3 vDistance = cB.pos - cA.pos;
	cA.UpdateAxisAll();
	cB.UpdateAxisAll();

	if (!CompareLengthOBB(cA, cB, cA.axisX, vDistance)) return false;
	if (!CompareLengthOBB(cA, cB, cA.axisY, vDistance)) return false;
	if (!CompareLengthOBB(cA, cB, cA.axisZ, vDistance)) return false;
	if (!CompareLengthOBB(cA, cB, cB.axisX, vDistance)) return false;
	if (!CompareLengthOBB(cA, cB, cB.axisY, vDistance)) return false;
	if (!CompareLengthOBB(cA, cB, cB.axisZ, vDistance)) return false;

	Vec3 vSep;
	vSep = cA.axisX.Cross(cB.axisX);
	if (!CompareLengthOBB(cA, cB, vSep, vDistance)) return false;

	vSep = cA.axisX.Cross(cB.axisY);
	if (!CompareLengthOBB(cA, cB, vSep, vDistance)) return false;
	
	vSep = cA.axisX.Cross(cB.axisZ);
	if (!CompareLengthOBB(cA, cB, vSep, vDistance)) return false;
	
	vSep = cA.axisY.Cross(cB.axisX);
	if (!CompareLengthOBB(cA, cB, vSep, vDistance)) return false;
	
	vSep = cA.axisY.Cross(cB.axisY);
	if (!CompareLengthOBB(cA, cB, vSep, vDistance)) return false;
	
	vSep = cA.axisY.Cross(cB.axisZ);
	if (!CompareLengthOBB(cA, cB, vSep, vDistance)) return false;
	
	vSep = cA.axisZ.Cross(cB.axisX);
	if (!CompareLengthOBB(cA, cB, vSep, vDistance)) return false;
	
	vSep = cA.axisZ.Cross(cB.axisY);
	if (!CompareLengthOBB(cA, cB, vSep, vDistance)) return false;
	
	vSep = cA.axisZ.Cross(cB.axisZ);
	if (!CompareLengthOBB(cA, cB, vSep, vDistance)) return false;

	return true;
}

Vec3 OBBvsPlane(OBBCube my, OBBCube pl) {
	my.UpdateAxisAll();
	pl.UpdateAxisAll();
	float r = fabs((my.axisX*my.radius.x).Dot(pl.axisX))
		+ fabs((my.axisY*my.radius.y).Dot(pl.axisX))
		+ fabs((my.axisZ*my.radius.z).Dot(pl.axisX));

	Vec3 dis = (my.pos - (pl.pos + pl.axisX*pl.radius.x));
	//cout << dis.Debug() << endl;

	float s = dis.Dot(pl.axisX);

	//cout << my.pos.Debug() << endl;
	//cout << (my.axisX*my.radius.x).Debug() << endl;
	//cout << (my.axisY*my.radius.y).Debug() << endl;
	//cout << (my.axisZ*my.radius.z).Debug() << endl;
	//cout << pl.axisX.Debug() << endl;
	//cout << pl.axisY.Debug() << endl;
	//cout << pl.axisZ.Debug() << endl;
	//printf_s("%.4f,%.4f,%d\n", r, s, (fabs(s) - r < 0.0f));

	if (fabs(s) - r < 0.0f) {
		if (s > 0) return pl.axisX*(r - fabs(s)); else return pl.axisX*(r + fabs(s));
		//cout << offset << endl << endl;
	}
	//puts("\n");
	return Vec3(0,0,0);
}

void DrawOBBCube(Vec3 pos, Vec3 radius, Vec3 rot, float red, float green, float blue, float alpha) {
	GLfloat colors[4] = { red, green, blue, alpha };

	glPushMatrix(); {
		glTranslatef(pos.x, pos.y, pos.z);
		glRotatef(rot.z, 0, 0, 1);
		glRotatef(rot.x, 1, 0, 0);
		glRotatef(rot.y, 0, 1, 0);
		glScaled(radius.x * 2, radius.y * 2, radius.z * 2);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, colors);
		glutSolidCube(1);
	}
	glPopMatrix();
}

void OBBCube::DrawOBBCube(bool isSolid) {
	GLfloat colors[4] = { color.x, color.y, color.z, color.w };

	glPushMatrix(); {
		glTranslatef(pos.x, pos.y, pos.z);
		glRotatef(rot.z, 0, 0, 1);
		glRotatef(rot.x, 1, 0, 0);
		glRotatef(rot.y, 0, 1, 0);
		glScaled(radius.x * 2, radius.y * 2, radius.z * 2);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, colors);
		if(isSolid)	glutSolidCube(1); else glutWireCube(1);
	}
	glPopMatrix();
}

void setGLColor(GLfloat color[4], float r, float g, float b, float a) {
	color[0] = r;
	color[1] = g;
	color[2] = b;
	color[3] = a;
}
#endif
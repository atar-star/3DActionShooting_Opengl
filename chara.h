#pragma once

#include <vector>
#include <GL/glut.h>
#include <iostream>
#include <random>
#include <ctime>

#include "OBB.h"

class model : public OBBCube{
public:
	string kind,name;
	Vec4 color;
	vector<model> child;

	model(string name, string kind, Vec3 pos, Vec3 rot, Vec3 radius, Vec4 color) {
		this->kind = kind;
		this->pos = pos;
		this->rot = rot;
		this->radius = radius;
		this->color = color;
		weight = 0;
	}
};

class Chara : public OBBCube {
public:
	vector<model> mm;
	float HP;
	float damage;

	Chara(){}
	Chara(Vec3 p, Vec3 r, Vec3 a, Vec4 col,float wei) {
		pos = p;
		rot = a;
		radius = r;
		weight = wei;
	}

	void setHP(float hp) {
		HP = hp;
	}

	void setDamage(float dam) {
		damage = dam;
	}

	void Damage(float dam) {
		HP -= dam;
		if (HP < 0) HP = 0;
	}

	void setOBBColor(Vec4 col) {
		color = col;
	}

	void addModel(string name, string kind, Vec3 pos, Vec3 rot, Vec3 radius, Vec4 color) {
		mm.push_back(model(name, kind,pos,rot,radius,color));
	}

	void addChild(string pare_nm, string name, string kind, Vec3 pos, Vec3 rot, Vec3 radius, Vec4 color) {
		for (model parent : mm) {
			if (pare_nm == parent.name) parent.child.push_back(model(name, kind, pos, rot, radius, color));
		}
	}

	void render(bool isobb) {
		if (isobb) this->DrawOBBCube(false);
		
		glPushMatrix(); {
			glTranslatef(pos.x, pos.y, pos.z);
			glRotatef(rot.z, 0, 0, 1);
			glRotatef(rot.x, 1, 0, 0);
			glRotatef(rot.y, 0, 1, 0);

			render_models(mm);
		}glPopMatrix();
	}

	void render_models(vector<model> ms) {
		GLfloat glcolor[4];
		for (int i = 0;i < ms.size();i++) {
			glPushMatrix(); {
				glTranslatef(ms[i].pos.x, ms[i].pos.y, ms[i].pos.z);
				glRotatef(ms[i].rot.z, 0, 0, 1);
				glRotatef(ms[i].rot.x, 1, 0, 0);
				glRotatef(ms[i].rot.y, 0, 1, 0);
				glScaled(ms[i].radius.x, ms[i].radius.y, ms[i].radius.z);
				setGLColor(glcolor, ms[i].color.x, ms[i].color.y, ms[i].color.z, ms[i].color.w);
				glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, glcolor);
				if (ms[i].kind == "Cube") {
					glutSolidCube(1);
				}
				else if (ms[i].kind == "Sphere") {
					glutSolidSphere(1, 16, 16);
				}
				else if (ms[i].kind == "Cone") {
					glutSolidCone(1, 1, 16, 16);
				}
				if(!ms[i].child.empty())render_models(ms[i].child);
			}glPopMatrix();
		}
	}
};

class mychr : public Chara {
public:
	mychr(float hp, float wei) {
		HP = hp;
		weight = wei;
		radius = Vec3(3, 2.5, 3);

		addModel("body", "Sphere", Vec3(0, 0, 0), Vec3(0, 0, 0), Vec3(2.5, 2.5, 2.5), Vec4(1, 0, 0, 1));
		addModel("right_arm", "Cube", Vec3(0, 0, 2), Vec3(0, 0, 45), Vec3(0.5, 2, 2), Vec4(1, 1, 0, 1));
		addModel("left_arm", "Cube", Vec3(0, 0, -2), Vec3(0, 0, 45), Vec3(0.5, 2, 2), Vec4(1, 1, 0, 1));
		addModel("right_leg", "Cone", Vec3(0, -1.7, 1.6), Vec3(30, 0, 0), Vec3(1, 1, 2), Vec4(1, 1, 0, 1));
		addModel("left_leg", "Cone", Vec3(0, -1.7, -1.6), Vec3(-30, 180, 0), Vec3(1, 1, 2), Vec4(1, 1, 0, 1));
	}
};

class enemy : public Chara {
public:
	float point;
	enemy(float hp,float wei, float dam, float point) {
		HP = hp;
		weight = wei;
		damage = dam;
		radius = Vec3(3, 2.5, 3);
		this->point = point;

		addModel("body", "Sphere", Vec3(0, 0, 0), Vec3(0, 0, 0), Vec3(2.5, 2.5, 2.5), Vec4(1, 0, 1, 1));
		addModel("canon1", "Cube", Vec3(0, 0, 2), Vec3(0, 0, 0), Vec3(1, 1, 1), Vec4(0, 1, 1, 1));
		addModel("canon2", "Cube", Vec3(0, 0, -2), Vec3(0, 0, 0), Vec3(1, 1, 1), Vec4(0, 1, 1, 1));
		addModel("canon3", "Cube", Vec3(2, 0, 0), Vec3(0, 0, 0), Vec3(1, 1, 1), Vec4(0, 1, 1, 1));
		addModel("canon4", "Cube", Vec3(-2, 0, 0), Vec3(0, 0, 0), Vec3(1, 1, 1), Vec4(0, 1, 1, 1));
	}
};

class Shot : public Chara {
public:
	Shot(float dam, float wei) {
		damage = dam;
		weight = wei;
		radius = Vec3(1,0.5,0.5);

		setOBBColor(Vec4(1, 1, 1, 1));

		addModel("s1", "Sphere", Vec3(0, 0, 0), Vec3(0, 0, 0), Vec3(1, 0.5, 0.5), Vec4(0.8, 0.8, 0.8, 1));
		//addModel("s2", "Cube", Vec3(-0.5, 0, 0), Vec3(0, 0, 0), Vec3(2, 1, 1), Vec4(0.8, 0.8, 0.8, 1));
	}
};

class Ene_Shot : public Chara {
public:
	Ene_Shot(float dam, float wei) {
		damage = dam;
		weight = wei;
		radius = Vec3(2, 1, 1)*0.7;

		setOBBColor(Vec4(0, 0, 0, 1));
		addModel("s1", "Sphere", Vec3(0, 0, 0), Vec3(0, 0, 0), Vec3(2, 1, 1)*0.7, Vec4(0.2, 0.2, 0.2, 1));
	}
};

class burst : public Chara {
public:
	mt19937 mt;

	burst() {
		//uniform_int_distribution<> rand100(0, 100);
		//mt.seed(static_cast<unsigned int>(time(nullptr)));
		addModel("b1", "Sphere", Vec3(0, 0, 0), Vec3(0, 0, 0), Vec3(2.5, 2.5, 2.5), Vec4(1, 0, 0, 1));
	}

	void render(bool isobb) {
		if (isobb) this->DrawOBBCube(false);

		glPushMatrix(); {
			glTranslatef(pos.x, pos.y, pos.z);
			glRotatef(rot.z, 0, 0, 1);
			glRotatef(rot.x, 1, 0, 0);
			glRotatef(rot.y, 0, 1, 0);

			render_models(mm);
		}glPopMatrix();
	}
};
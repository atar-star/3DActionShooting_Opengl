//#define MAIN
#ifndef MAIN

#include <math.h>
#include <string>
#include <sstream>
#include <iostream>
#include <GL/glut.h>
#include <ctype.h>
#include <vector>
#include <windows.h>
#include <random>
#include "FPS.h"
#include "OBB.h"
#include "chara.h"

using namespace std;

#define WIDTH 640
#define HEIGHT 480
#define MIN(X,Y) (((X) < (Y))? (X) : (Y))

void init_charas(void);

int wid = WIDTH;
int hei = HEIGHT;
int state = 0;


GLfloat lightAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat lightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat lightSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat lightPosition[] = { 0.0f, 50.f, 0.0f, 1.0f };

std::random_device rnd;
std::mt19937 mt(rnd());
std::uniform_int_distribution<> rand350(0, 350);

void DrawBitmapString(string str, int x0, int y0, int ch_sz, int red, int green, int blue) {
	glDisable(GL_LIGHTING);

	glColor3d(red, green, blue);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix(); {
		glLoadIdentity();
		gluOrtho2D(0, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT), 0);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix(); {
			glLoadIdentity();

			glRasterPos2f(x0, y0);
			int size = (int)str.size();
			for (int i = 0; i < size; ++i) {
				char ic = str[i];
				if (ic == '\n') {
					y0 += ch_sz;
					glRasterPos2f(x0, y0);
				}
				else {
					//glutBitmapCharacter(GLUT_BITMAP_9_BY_15, ic);
					glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, ic);
				}
			}

		}glPopMatrix();
		glMatrixMode(GL_PROJECTION);
	}glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

void DrawStrokeString(string str, int x0, int y0, int ch_sz, Vec4 cl, int px0, int py0, int p_x, int p_y, Vec4 p_cl) {
	glDisable(GL_LIGHTING);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix(); {
		glLoadIdentity();
		gluOrtho2D(0, 300, 0, 300);
		//gluOrtho2D(0, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT), 0);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix(); {
			int size = (int)str.size();
			glLoadIdentity();
			if (p_cl.w != 0) {
				glPushMatrix(); {
					glTranslated(px0, py0 + 10, -1);
					glColor4d(p_cl.x, p_cl.y, p_cl.z, p_cl.w);
					glScalef(p_x, p_y, 1);//200,50,1);
					glutSolidCube(1);
				}glPopMatrix();
			}
			glColor4d(cl.x, cl.y, cl.z, cl.w);
			glTranslated(x0 - size * 10, y0, 0);
			glScalef(0.05f*ch_sz, 0.05f*ch_sz, 0.05f*ch_sz);
			glLineWidth(5.f);
			for (int i = 0; i < size; ++i) {
				char ic = str[i];
				glutStrokeCharacter(GLUT_STROKE_ROMAN, ic);
			}
			glLineWidth(1.f);
		}glPopMatrix();
		glMatrixMode(GL_PROJECTION);
	}glPopMatrix();
}

void setColor(GLfloat color[4], float r, float g, float b, float a) {
	color[0] = r;
	color[1] = g;
	color[2] = b;
	color[3] = a;
}

mychr myc(5,3);
OBBCube plane;
vector<OBBCube> walls;
vector<enemy> targets;
vector<Shot> shots;
vector<Ene_Shot> ene_shots;
vector<int> en_sh_times;
int destroy = 0;
int points = 0;

int fps_n;

OBBCube eye;
Vec3 vEye; // カメラ座標

FPS *fps = new FPS(60, false);

// 法線ベクトルは全て X = 1
void Init(void) {
	glClearColor(0.0, 0.0, 1.0, 0.0);
	glEnable(GL_DEPTH_TEST);

	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glEnable(GL_LIGHT0);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	plane.radius = Vec3(0.01f, 250.f, 250.f);
	plane.rot = Vec3(0, 0, 90);
	plane.pos = (Mat44().YawPitchRoll(plane.rot).Translate(0, -plane.radius.x, 0)*Vec4(0, 0, 0, 1)).Pos();
	plane.color = Vec4(1, 1, 0, 1);
	plane.UpdateAxisAll();

	walls.push_back(OBBCube(Vec3(400, 0, 0), Vec3(200, 400, 200), Vec3(0, 180, 0), Vec4(1, 0, 0, 0.6), 99999));
	walls.push_back(OBBCube(Vec3(-400, 0, 0), Vec3(200, 400, 200), Vec3(0, 0, 0), Vec4(1, 0, 0, 0.6), 99999));
	walls.push_back(OBBCube(Vec3(0, 400, 0), Vec3(100, 200, 200), Vec3(0, 0, -90), Vec4(0, 1, 0, 0.6), 99999));
	walls.push_back(OBBCube(Vec3(0, 0, 400), Vec3(200, 400, 200), Vec3(0, 90, 0), Vec4(1, 0, 1, 0.6), 99999));
	walls.push_back(OBBCube(Vec3(0, 0, -400), Vec3(200, 400, 200), Vec3(0, -90, 0), Vec4(1, 0, 1, 0.6), 99999));
	walls.push_back(OBBCube(Vec3(50, 30, 50), Vec3(0.15, 40, 40), Vec3(0, 0, 90), Vec4(0, 1, 0, 0.6), 99999));
	walls.push_back(OBBCube(Vec3(50, 29.7, 50), Vec3(0.15, 40, 40), Vec3(0, 0, -90), Vec4(0, 1, 0, 0.6), 99999));
	walls.push_back(OBBCube(Vec3(-70, 30, -70), Vec3(0.15, 40, 40), Vec3(0, 0, 90), Vec4(0, 1, 0, 0.6), 99999));
	walls.push_back(OBBCube(Vec3(-70, 29.7, -70), Vec3(0.15, 40, 40), Vec3(0, 0, -90), Vec4(0, 1, 0, 0.6), 99999));
	for (OBBCube wall : walls) wall.UpdateAxisAll();

	init_charas();
	
	vEye = (Mat44().YawPitchRoll(myc.rot).Translate(0, 5, -30)*Vec4(myc.pos, 1)).Pos();
	eye.pos = vEye;
	eye.radius = Vec3(0.5, 0.5, 0.5);

	glEnable(GL_NORMALIZE);
}

void init_charas() {
	destroy = 0;
	points = 0;

	myc.setHP(5);
	myc.pos = Vec3(-70, myc.radius.y, 0);
	myc.rot = Vec3(0, 0, 0);
	myc.setOBBColor(Vec4(0, 0, 1, 1));

	//target.vel = Vec3(0, -500, 0);
	if (!targets.empty()) {
		targets.clear();
		targets.shrink_to_fit();
	}

	enemy target(5, 3, 0,5);
	target.radius = Vec3(2.5f, 2.5f, 2.5f);
	target.pos = Vec3(70, 30 + target.radius.y, 0);
	target.rot = Vec3(0, 30, 0);
	target.color = Vec4(0.2, 0.8, 0.8, 1);
	targets.push_back(target);
	en_sh_times.clear();
	en_sh_times.push_back(timeGetTime());
	if (!shots.empty()) {
		shots.clear();
		shots.shrink_to_fit();
	}
	if (!ene_shots.empty()) {
		ene_shots.clear();
		ene_shots.shrink_to_fit();
	}
}

int mouse_x = WIDTH/2;
bool debug = false, p_ctrl = false;
bool pause = false, p_space = false;

bool drag_l = false, drag_r = false;

bool sh_f = false;
DWORD sh_time = 0;

BYTE KeyTbl[256];
#define K_V 1
#define Push(X) (KeyTbl[X] & 0x80)
void MultiKeyPush() {

	GetKeyboardState(KeyTbl);
	if (Push(VK_ESCAPE)) exit(0);
	if (Push(VK_SHIFT)) {
		if (!p_space) {
			pause = !pause;
			p_space = true;
		}
	}
	else {
		p_space = false;
	}

	if (!pause) {
		Vec3 vRot;
		if (Push('A')) vRot = vRot + myc.axisY;
		if (Push('D')) vRot = vRot - myc.axisY;
		if (!vRot.isZero()) {
			myc.rot = myc.rot + vRot;

			if (myc.rot.x < 0)   myc.rot.x += 360;	else if (myc.rot.x > 360) myc.rot.x -= 360;
			if (myc.rot.y < 0)   myc.rot.y += 360;	else if (myc.rot.y > 360) myc.rot.y -= 360;
			if (myc.rot.z < 0)   myc.rot.z += 360;	else if (myc.rot.z > 360) myc.rot.z -= 360;

			myc.UpdateAxisAll();
		}

		if (drag_r || Push('S')) myc.vel.y = myc.vel.y - 0.2 < -0.4 ? -0.4 : myc.vel.y - 0.2;

		Vec3 vAxis;
		if (Push(VK_UP)) vAxis = vAxis - myc.axisX;//Mat44().Rotate('Y', myc.rot.to_rad().y)*Vec3(-1, 0, 0);
		if (Push(VK_DOWN))	vAxis = vAxis + myc.axisX;//+ Mat44().Rotate('Y', myc.rot.to_rad().y)*Vec3(1, 0, 0);
		if (Push(VK_LEFT))	vAxis = vAxis + myc.axisZ;//Mat44().Rotate('Y', myc.rot.to_rad().y)*Vec3(0, 0, 1);
		if (Push(VK_RIGHT)) vAxis = vAxis - myc.axisZ;//+ Mat44().Rotate('Y', myc.rot.to_rad().y)*Vec3(0, 0, -1);
		if (!vAxis.isZero()) {
			myc.vel = myc.vel + vAxis.Normalize()*K_V;
			if (myc.vel.Length() > 2.0) myc.vel = myc.vel.Normalize()*Vec3(2.0, 0, 2.0) + Vec3(0, myc.vel.y, 0);
		}

		if (Push('W')||drag_l) {
			if (timeGetTime() - sh_time > 50) {
				sh_time = timeGetTime();
				if (debug)cout << "shoot!" << endl;
				Mat44 smat_d = Mat44().Translate(myc.radius.x, 0, 0);
				Mat44 smat_r = Mat44().YawPitchRoll(myc.rot.to_rad());
				Mat44 smat_sp = Mat44().Translate(myc.pos);
				Vec3 sh = smat_sp*smat_r*smat_d*Vec3(0, 0, 0);
				//OBBCube shoot(sh, Vec3(3, 1, 1), myc.rot, Vec4(0.8, 0.8, 0.8, 1),0.1);
				Shot shoot(1, 0.1);
				shoot.pos = sh;
				shoot.rot = myc.rot;
				shoot.UpdateAxisAll();
				shoot.vel = shoot.axisX * 5;
				shots.push_back(shoot);
				//shot.push_back(shoot);
			}
		}

		if (Push(VK_RETURN)) {
			if (!p_ctrl) {
				debug = !debug;
				p_ctrl = true;
			}
		}
		else {
			p_ctrl = false;
		}
	}
}

void mouse(int button, int state, int mx, int my) {
	if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN)) {
		drag_l = true;
		mouse_x = mx;
	}else if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_UP)) {
		drag_l = false;
		mouse_x = wid / 2;
	}

	if ((button == GLUT_RIGHT_BUTTON) && (state == GLUT_DOWN)) {
		drag_r = true;
		mouse_x = mx;
	}else if ((button == GLUT_RIGHT_BUTTON) && (state == GLUT_UP)) {
		drag_r = false;
		mouse_x = wid / 2;
	}
}

void motion(int mx, int my) {
	mouse_x = mx;
}

void DrawLife() {
	DrawBitmapString("HP: ", 20, 25, 18, 1.0, 1.0, 0.0);

	glDisable(GL_LIGHTING);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix(); {
		glLoadIdentity();
		gluOrtho2D(0, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT), 0);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix(); {
			glLoadIdentity();
			glTranslated(60+15*myc.HP, 19, 0);
			glColor4d(1 - myc.HP / 5, myc.HP / 5, 0.5 - myc.HP / 10, 1);
			glScalef(30*myc.HP, 20, 1);
			glutSolidCube(1);
		}glPopMatrix();
		glMatrixMode(GL_PROJECTION);
	}glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

float start_time = 0;
void Display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_BLEND);
	glEnable(GL_LIGHTING);
	//glDisable(GL_LIGHT0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	Mat44 emat_d = Mat44().Translate(-30, 5, 0);
	Mat44 emat_r = Mat44().YawPitchRoll(myc.rot.to_rad());
	Mat44 emat_sp = Mat44().Translate(myc.pos);
	vEye = (emat_sp*emat_r*emat_d)*Vec3(0, 0, 0);
	gluLookAt(vEye.x, vEye.y, vEye.z, myc.pos.x, myc.pos.y, myc.pos.z, 0, 1, 0);// 0, 0, 0, 0, 1, 0);//

	plane.DrawOBBCube(true);
	for (OBBCube wall : walls) wall.DrawOBBCube(true);

	GLfloat colors[4] = { 1.0, 1.0, 0.0, 1.0 };
	glLineWidth(5);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, colors);
	glBegin(GL_LINES); {
		for (int i = -20; i <= 20; i++) {
			glVertex3d((GLdouble)i * 10, 0, -200.0);
			glVertex3d((GLdouble)i * 10, 0, 200.0);
			glVertex3d(-200.0, 0, (GLdouble)i * 10);
			glVertex3d(200.0, 0, (GLdouble)i * 10);
		}
	} glEnd();

	glLineWidth(5);
	Vec3 v_vec = myc.axisX * 100;
	glPushMatrix(); {
		glTranslatef(myc.pos.x, myc.pos.y, myc.pos.z);
		glNormal3f(1.0, 0.0, 0.0);
		glBegin(GL_LINES); {
			glVertex3d(0, 0, 0.0);
			glVertex3d(v_vec.x, v_vec.y, v_vec.z);
		} glEnd();
	}glPopMatrix();

	glLineWidth(5);
	glPushMatrix(); {
		glTranslatef(myc.pos.x, myc.pos.y, myc.pos.z);
		glNormal3f(1.0, 0.0, 0.0);
		if (myc.rot.z != 0) glRotatef(myc.rot.z, 0, 0, 1);
		if (myc.rot.x != 0) glRotatef(myc.rot.x, 1, 0, 0);
		if (myc.rot.y != 0) glRotatef(myc.rot.y, 0, 1, 0);
		glBegin(GL_LINES); {
			glVertex3d(0, 0, 0.0);
			glVertex3d(100.0, 0, 0.0);
		} glEnd();
	}glPopMatrix();

	for (Shot sh : shots) sh.render(debug);
	for (Ene_Shot ensh : ene_shots) ensh.render(debug);
	myc.render(debug);
	for (enemy tar : targets) if (tar.HP > 0) tar.render(debug);
	
	if (state == 0) {
		DrawStrokeString("Start", 160, 80, 5, Vec4(1, 1, 0, 1), 150, 80, 100, 50, Vec4(0, 0, 0, 0));
		int stt = 3 - int(start_time / 60);
		if (stt == -1) state = 1;
		else DrawStrokeString(to_string(stt), 150, 30, 5, Vec4(1, 1, 0, 1), 150, 80, 100, 50, Vec4(0, 0, 0, 0));
	}else if (state == 2) {
		DrawStrokeString("Game Over", 150, 150, 5, Vec4(1, 1, 0, 1), 150, 150, 200, 50, Vec4(0, 0, 0, 0));
		if (start_time > 60) {
			init_charas();
			state = 0;
		}
	}else if (pause) {
		DrawStrokeString("Pause", 150, 150, 5, Vec4(1, 1, 0, 1), 150, 150, 200, 50, Vec4(0, 0, 0, 0));
	}
	DrawLife();

	stringstream st;
	st << "FPS " << fps_n << endl;
	DrawBitmapString(st.str(), glutGet(GLUT_WINDOW_WIDTH) - 80, 20, 18, 1.0, 1.0, 0.0);

	st.str("");
	st << "Point " << points << endl;
	DrawBitmapString(st.str(), 20, 50, 18, 1.0, 1.0, 0.0);

	if (debug) {
		st.str("");
		st << "Destroy: " << destroy << endl;
		st << "Pos: " << myc.pos.Debug() << endl;
		st << "Vel: " << myc.vel.Debug() << endl;
		st << "Ang: " << myc.rot.Debug() << endl;
		st << "Num of Enemies: " << targets.size() << endl;
		for (enemy tar:targets)st << "Targets Pos: " << tar.pos.Debug() << endl;
		DrawBitmapString(st.str(), 20, 70, 18, 1.0, 1.0, 0.0);
	}

	glDisable(GL_BLEND);
	glutSwapBuffers();
}

void Resize(int w, int h) {
	wid = glutGet(GLUT_WINDOW_WIDTH);
	hei = glutGet(GLUT_WINDOW_HEIGHT);

	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(30.0, (double)w / (double)h, 1.0, 1000.0);

	glMatrixMode(GL_MODELVIEW);
}

float e = 1;
float m = 1, M = 5;

vector<OBBCube> cu;
void next_state() {
	myc.UpdateAxisAll();
	//target.UpdateAxisAll();

	if (myc.HP > 0) {
		if (1 || drag_l) {
			Vec3 vRot = myc.axisY*(wid / 2 - mouse_x)*0.005;
			myc.rot = myc.rot + vRot;

			if (myc.rot.x < 0)   myc.rot.x += 360;
			else if (myc.rot.x > 360) myc.rot.x -= 360;
			if (myc.rot.y < 0)   myc.rot.y += 360;
			else if (myc.rot.y > 360) myc.rot.y -= 360;
			if (myc.rot.z < 0)   myc.rot.z += 360;
			else if (myc.rot.z > 360) myc.rot.z -= 360;

			myc.UpdateAxisAll();
		}

		// mychr
		{
			myc.vel.y = myc.vel.y + 0.0098 > 0.6 ? 0.6 : myc.vel.y + 0.0098;

			myc.pos = myc.pos - myc.vel;

			float fric = 1.1;
			//if (myc.pos.y < 0)myc.pos.y = 0;

			float off;
			Vec3 offs;

			//bool hit = false;
			for (enemy target:targets)
			while (IsCollideBoxOBB(myc, target)) {
				//cout << "Hit!" << endl;
				//offs = OBBvsPlane(myc, target);
				//cout << offs.Debug() << endl;
				//myc.pos = myc.pos + offs;
				myc.pos = myc.pos + myc.vel*0.01;
				//target.pos = target.pos + target.vel*0.01;

				//target.vel = myc.vel / (myc.vel + target.vel);
				//myc.vel = target.vel / (myc.vel + target.vel);
				target.vel = (myc.vel*myc.weight*(1 + 1) + target.vel*(target.weight - e*myc.weight)) / (myc.weight + target.weight);
				myc.vel = (myc.vel*(myc.weight - 1*target.weight) + target.vel*target.weight*(1 + 1)) / (myc.weight + target.weight);
				//myc.pos = myc.pos + Vec3(0, 1, 0)*(myc.pos - target.pos).Dot(Vec3(0,0.01,0));
				//target.pos = target.pos + Vec3(0, 1, 0)*(target.pos - myc.pos).Dot(Vec3(0, 0.01, 0));
				//hit = true;
			}
			//if (hit) {
			//	float e = 1;
			//	float m = 1, M = 5;
			//	target.vel = (myc.vel*m*(e+1) + target.vel*(M-e*m))/(m+M);
			//	myc.vel = (myc.vel*(m-e*M) + target.vel*M*(e+1))/(m+M);
			//}

			for (OBBCube wall : walls) {
				if (IsCollideBoxOBB(myc, wall)) {
					offs = OBBvsPlane(myc, wall);
					myc.pos = myc.pos + offs;

					if (offs.x != 0)myc.vel.x = 0;
					if (offs.y != 0)myc.vel.y = 0;
					if (offs.z != 0)myc.vel.z = 0;

					myc.vel = (myc.vel*(myc.weight - 0*wall.weight) + wall.vel*wall.weight*(0 + 1)) / (myc.weight + wall.weight);
				}
				//cout << "Wall[" << wall.pos.Debug() << "] : " << offs.Debug() << endl;
			}

			if (IsCollideBoxOBB(myc, plane)) {
				offs = OBBvsPlane(myc, plane);
				myc.pos = myc.pos + offs;
				//cout << "Plane : " << offs.Debug() << endl;
				myc.vel.y *= -0.5;
				fric = 1.1;
			}

			if (fabs(myc.vel.x*myc.vel.x + myc.vel.z*myc.vel.z) > 0.5) {
				myc.vel.x /= fric;
				myc.vel.z /= fric;
			}
			else {
				myc.vel.x = 0;
				myc.vel.z = 0;
			}
		}
	}
	else {
		cout << "gameover" << endl;
	}
	/////////////////

	// target
	//cout << targets.size() << endl;
	if (targets.empty()) {
		en_sh_times.clear();
		en_sh_times.push_back(timeGetTime());
		
		if (destroy < 2) {
			enemy target(5,1,0,5);
			target.pos = Vec3(rand350(mt) % 350 - 175, 30 + target.radius.y, rand350(mt) % 350 - 175);
			target.vel = Vec3(0, 0, 0);
			target.color = Vec4(0.2, 0.8, 0.8, 1);
			targets.push_back(target);
		}
		else {
			int e_num;
			if (destroy < 8) e_num = 5;
			else if (destroy < 13) e_num = 6; 
			else e_num = 10;
			cu.clear();
			cu.shrink_to_fit();
			for (int i = 0;i < e_num;i++) {
				enemy target(5, 1, 0, 5);
				target.pos = Vec3(rand350(mt) % 350 - 175, 30 + target.radius.y, rand350(mt) % 350 - 175);
				target.vel = Vec3(0, 0, 0);
				target.color = Vec4(0.2, 0.8, 0.8, 1);
				if (i != 0) {
					bool hit;
					do {
						hit = false;
						for (int j = 0;j < cu.size();j++) {
							hit |= IsCollideBoxOBB(target, cu[j]);
							//cout << "hit: " << hit << endl;
						}
						if(hit)	target.pos = Vec3(rand350(mt) % 350 - 175, 30 + target.radius.y, rand350(mt) % 350 - 175);
					} while (hit);
				}
				cu.push_back(target);
				targets.push_back(target);
			}
		}
	}
	else if (destroy >= 13 && targets.size() < 8) {
		cu.clear();
		cu.shrink_to_fit();
		int n_sz = targets.size();
		for (int i = 0;i < 15 - n_sz;i++) {
			enemy target(5, 1, 0, 5);
			target.pos = Vec3(rand350(mt) % 350 - 175, 30 + target.radius.y, rand350(mt) % 350 - 175);
			target.vel = Vec3(0, 0, 0);
			target.color = Vec4(0.2, 0.8, 0.8, 1);
			if (i != 0) {
				bool hit;
				do {
					hit = false;
					for (int j = 0;j < cu.size();j++) {
						hit |= IsCollideBoxOBB(target, cu[j]);
						//cout << "hit: " << hit << endl;
					}
					if (hit)	target.pos = Vec3(rand350(mt) % 350 - 175, 30 + target.radius.y, rand350(mt) % 350 - 175);
				} while (hit);
			}
			cu.push_back(target);
			targets.push_back(target);
		}

		en_sh_times.clear();
		for (int i=0;i<int(MIN(15,destroy/5));i++)en_sh_times.push_back(timeGetTime());
	}

	{
		for (int i = 0; i < targets.size(); i++) {
			targets[i].vel.y = targets[i].vel.y + 0.0098 > 0.6 ? 0.6 : targets[i].vel.y + 0.0098;
			targets[i].pos = targets[i].pos - targets[i].vel;

			float fric = 1.1;

			float off;
			Vec3 offs;

			for (OBBCube wall : walls) {
				if (IsCollideBoxOBB(targets[i], wall)) {
					offs = OBBvsPlane(targets[i], wall);
					targets[i].pos = targets[i].pos + offs;
					//cout << "Wall[" << wall.pos.Debug() << "] : " << offs.Debug() << endl << endl;
				}
			}

			if (IsCollideBoxOBB(targets[i], plane)) {
				offs = OBBvsPlane(targets[i], plane);
				//cout << "Plane : " << offs.Debug() << endl;
				targets[i].pos = targets[i].pos + offs;
				targets[i].vel.y *= -0.5;
				fric = 1.3;
			}

			if (fabs(targets[i].vel.x*targets[i].vel.x + targets[i].vel.z*targets[i].vel.z) > 1) {
				targets[i].vel.x /= fric;
				targets[i].vel.z /= fric;
			}
			else {
				targets[i].vel.x = 0;
				targets[i].vel.z = 0;
			}

			targets[i].rot.y += 5;
			//cout << "targets[i]: " << targets[i].pos.Debug() << "," << targets[i].vel.Debug() << "," << targets[i].rot.Debug() << endl;
			if (i < en_sh_times.size()) {
				if (timeGetTime() - en_sh_times[i] > 500) {//!sh_f) {
					en_sh_times[i] = timeGetTime();
					//cout << "ensh" << endl;

					Mat44 smat_r;
					Mat44 smat_sp;
					Vec3 sh, rr;
					for (int j = 0;j < 8;j++) {
						//cout << "sh" << endl;
						rr = targets[i].rot;
						rr.y += 45 * j;
						smat_sp = Mat44().Translate(targets[i].pos);
						sh = smat_sp*Vec3(0, 0, 0);
						//OBBCube en_sht(sh, Vec3(3, 1, 1), rr, Vec4(0.2, 0.2, 0.2, 1),0.1);
						Ene_Shot en_sht(0.5, 0.1);
						en_sht.pos = sh;
						en_sht.rot = rr;
						en_sht.UpdateAxisAll();
						en_sht.vel = en_sht.axisX * 5;
						//ene_shot.push_back(en_sht);
						ene_shots.push_back(en_sht);
					}
				}
			}
		}
	}
	/////////////////////////////////
	
	stringstream ss;

	bool del_fl = false;
	if (shots.size() != 0) {
		for (int i = 0; i < shots.size(); i++) {
			del_fl = false;
			shots[i].pos = shots[i].pos + shots[i].vel;
			//cout << "shoot :" << shot[i].pos.Debug() << endl;
			//ss << "shoot[" << i << "]:" << shot[i].pos.Debug() << endl;
			for (int j = 0; j < targets.size(); j++) {
				if (IsCollideBoxOBB(shots[i], targets[j])) {
					targets[j].Damage(shots[j].damage);
					targets[j].color = targets[j].color*Vec4(1.5, 1, 0.5, 1);
					if (targets[j].HP == 0){
						destroy++;
						points += targets[j].point;
						targets.erase(targets.begin() + j);
						j--;
						continue;
					}
					shots.erase(shots.begin() + i);
					i--;
					del_fl = true;
					break;
				}
			}
			if (del_fl) continue;
			for (OBBCube wall : walls) {
				if (IsCollideBoxOBB(shots[i], wall)) {
					shots.erase(shots.begin() + i);
					i--;
					break;
				}
			}

		}
	}

	if (ene_shots.size() != 0) {
		for (int i = 0; i < ene_shots.size(); i++) {
			ene_shots[i].pos = ene_shots[i].pos + ene_shots[i].vel;
			//ss << "ene_shoot[" << i << "]:" << ene_shot[i].pos.Debug() << endl;
			
			if (IsCollideBoxOBB(ene_shots[i], myc)) {
				myc.Damage(ene_shots[i].damage);
				myc.color = myc.color*Vec4(1, 1.5, 0.5, 1);
				ene_shots.erase(ene_shots.begin() + i);
				i--;
				continue;
			}

			for (OBBCube wall : walls) {
				if (IsCollideBoxOBB(ene_shots[i], wall)) {
					ene_shots.erase(ene_shots.begin() + i);
					i--;
				}
			}

		}
	}

	if (myc.HP == 0 && state != 2) {
		state = 2;
		start_time = 0;
	}
	//cout << ss.str();
}

bool fin_calc = false;
void Idle() {
	if (!fin_calc) {
		if (state != 0 && state != 2) MultiKeyPush();
		if (state != 0 && !pause) next_state();
		fin_calc = true;
	}

	fps_n = fps->GetFPS();
	if (fin_calc&&fps->draw) {
		if (state != 1) start_time++;
		glutPostRedisplay();
		fin_calc = false;
	}
}

int main(int argc, char *argv[]) {
	glutInit(&argc, argv);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutCreateWindow(argv[0]);
	glutSetWindowTitle("CG3DShooting!");
	glutDisplayFunc(Display);
	glutReshapeFunc(Resize);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);

	glutIdleFunc(Idle);
	Init();

	glutMainLoop();
	return 0;
}
#endif

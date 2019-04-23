#pragma once
#pragma comment(lib, "winmm.lib")

#include <Windows.h>
#include <string>
#include <mmsystem.h>
#include <iostream>

using namespace std;

#define CPUMax 30000
#define CPUMin 10000

class FPS {
public:
	int LoopCount, Frame, sleep, Skip, FrameSkip, Fps, LoopMax, CurrentFps;
	bool draw;
	DWORD Time, Oldtime;
	float FrameTime, FrameTimeO;

	FPS(int,bool);
	void SetFPS(int);
	int GetFPS();

private:
	bool deb;
	string str;
	char buf[255];
};

FPS::FPS(int fps, bool debug) {
	draw = false;
	LoopCount = 0;
	Frame = 0;
	sleep = 15;
	Skip = 0;
	Fps = fps;
	FrameSkip = 0;
	LoopMax = 0;
	CurrentFps = 0;
	FrameTime = 1000.0f / fps - 0.000001f;
	FrameTimeO = 1000.0f / fps + 0.000001f;
	deb = debug;
}

void FPS::SetFPS(int fps) {
	Fps = fps;
	FrameTime = 1000.0f / fps - 0.000001f;
	FrameTimeO = 1000.0f / fps + 0.000001f;
}

int FPS::GetFPS(){
	draw = false;
	LoopCount++;
	Time = timeGetTime();
	if (Time - Oldtime>Frame*FrameTime) {
		Frame++;
		if (Time - Oldtime<Frame*FrameTimeO) {//描画タイミングに間に合った場合
			if (deb) {
				cout << "LOOP_MAX:[" << LoopMax << "] FPS:[" << CurrentFps - FrameSkip
					<< "] FRAME:[" << Frame << "] FRAMESKIP:[" << FrameSkip << "] SLEEP:["
					<< sleep << "]" << endl;
			}
			draw = true;
			Sleep(sleep);
			return CurrentFps - FrameSkip;
		}else {//描画タイミングに間に合わなかった場合：フレームスキップ
			if (sleep>1)sleep--;//ウェイトを減らす
			Skip++;
		}
	}

	if (Time - Oldtime>1000 - FrameTime) {//一秒おきの処理
		LoopMax = LoopCount;
		if (LoopMax>CPUMax && sleep<15)sleep++; //十分に余力がある場合ウェイトを増やす
		else if (LoopMax<CPUMin && sleep>1)sleep--; //余裕が無い場合ウェイトを減らす
		
		CurrentFps = Frame;
		FrameSkip = Skip;
		Skip = 0;
		LoopCount = 0;
		Frame = 0;
		Oldtime = Time;
	}

	return 0;
}
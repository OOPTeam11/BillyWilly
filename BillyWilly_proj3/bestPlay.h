#pragma once

#include "virtualLego.h"
#include <iostream>
#include <vector>
using namespace std;

#define RED1BALL 0
#define RED2BALL 1
#define YELLOWBALL 2
#define WHITEBALL 3
#define WALL 5

/*
쓰리쿠션의 순간을 판단하여 이 경우 게임 플레이 장면을 다시 보여주는 기능을 담은 함수이다.
*/

class bestPlay{
public:
	bestPlay();
	//
	~bestPlay();
	void saveLastStatus(float timeDelta, CSphere *g_sphere, CWall *g_legowall, CWall g_legoPlane, CSphere g_target_blueball, CLight g_light); // 출발위치로 공을 되돌리기 위해 현재 공, 벽의 상태를 저장
	void savePower(float m_velocity_x, float m_velocity_z); // 출발하는 공의 x축, z축 velocity값을 저장
	float getVelocity_X(); // m_velocity_x 값을 반환
	float getVelocity_Z(); // m_velocity_z 값을 반환
	void showStartPos(IDirect3DDevice9** Device, D3DXMATRIX* g_mWorld,
					CSphere	*g_sphere, CWall *g_legowall, CWall *g_legoPlane, CSphere *g_target_blueball, CLight *g_light); // 현재 공 상태를 저장된 공 상태로 바꾸고 화면에 출력
	bool threeCushion(); // 쓰리쿠션 상황판단
	vector<int> cusionCount;
private:
	float lastTimeDelta;
	float m_velocity_x;
	float m_velocity_z;
	CSphere last_g_sphere[4];
	CSphere	last_g_target_blueball, cur_g_target_blueball;
	CWall	last_g_legoPlane;
	CWall	last_g_legowall[4];
	CLight	last_g_light;
	CSphere * currentBall;

};
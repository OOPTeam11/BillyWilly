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
��������� ������ �Ǵ��Ͽ� �� ��� ���� �÷��� ����� �ٽ� �����ִ� ����� ���� �Լ��̴�.
*/

class bestPlay{
public:
	bestPlay();
	//
	~bestPlay();
	void saveLastStatus(float timeDelta, CSphere *g_sphere, CWall *g_legowall, CWall g_legoPlane, CSphere g_target_blueball, CLight g_light); // �����ġ�� ���� �ǵ����� ���� ���� ��, ���� ���¸� ����
	void savePower(float m_velocity_x, float m_velocity_z); // ����ϴ� ���� x��, z�� velocity���� ����
	float getVelocity_X(); // m_velocity_x ���� ��ȯ
	float getVelocity_Z(); // m_velocity_z ���� ��ȯ
	void showStartPos(IDirect3DDevice9** Device, D3DXMATRIX* g_mWorld,
					CSphere	*g_sphere, CWall *g_legowall, CWall *g_legoPlane, CSphere *g_target_blueball, CLight *g_light); // ���� �� ���¸� ����� �� ���·� �ٲٰ� ȭ�鿡 ���
	bool threeCushion(); // ������� ��Ȳ�Ǵ�
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
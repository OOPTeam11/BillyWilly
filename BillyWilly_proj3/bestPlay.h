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

class bestPlay{
public:
	bestPlay();
	~bestPlay();
	void saveLastStatus(float timeDelta, CSphere *g_sphere, CWall *g_legowall, CWall g_legoPlane, CSphere g_target_blueball, CLight g_light); // 스페이스바가 눌렸을 때 출발할 때의 4개의 공 상태 저장.
	void saveCurStatus(float timeDelta, CSphere	*g_sphere, CWall *g_legowall, CWall g_legoPlane, CSphere g_target_blueball, CLight g_light); // 스페이스바가 눌렸을 때 출발할 때의 4개의 공 상태 저장.
	void getLastStatus(CSphere	*g_sphere, CWall *g_legowall, CWall *g_legoPlane, CSphere *g_target_blueball, CLight *g_light);
	void getCurStatus(CSphere	*g_sphere, CWall *g_legowall, CWall *g_legoPlane, CSphere *g_target_blueball, CLight *g_light);
	void showStartPos(float timeDelta, IDirect3DDevice9** Device, D3DXMATRIX* g_mWorld,
					CSphere	*g_sphere, CWall *g_legowall, CWall *g_legoPlane, CSphere *g_target_blueball, CLight *g_light);
	void showReplay(float timeDelta, IDirect3DDevice9** Device, D3DXMATRIX* g_mWorld,
					CSphere	*g_sphere, CWall *g_legowall, CWall *g_legoPlane, CSphere *g_target_blueball, CLight *g_light);
	bool threeCushion();
	vector<int> cusionCount;
private:
	float timeDelta;
	CSphere last_g_sphere[4], cur_g_sphere[4];
	CSphere	last_g_target_blueball, cur_g_target_blueball;
	CWall	last_g_legoPlane, cur_g_legoPlane;
	CWall	last_g_legowall[4], cur_g_legowall[4];
	CLight	last_g_light, cur_g_light;
	CSphere * currentBall;

};
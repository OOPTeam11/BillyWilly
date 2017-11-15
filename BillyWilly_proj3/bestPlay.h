#pragma once

#include "virtualLego.h"
#include <iostream>

using namespace std;

class bestPlay{
public:
	bestPlay();
	~bestPlay();
	void saveLastStatus(CSphere	*g_sphere, CWall *g_legowall, CWall g_legoPlane, CSphere g_target_blueball, CLight g_light); // 스페이스바가 눌렸을 때 출발할 때의 4개의 공 상태 저장.
	void saveCurStatus(CSphere	*g_sphere, CWall *g_legowall, CWall g_legoPlane, CSphere g_target_blueball, CLight g_light); // 스페이스바가 눌렸을 때 출발할 때의 4개의 공 상태 저장.
	void getLastStatus(CSphere	*g_sphere, CWall *g_legowall, CWall *g_legoPlane, CSphere *g_target_blueball, CLight *g_light);
	void getCurStatus(CSphere	*g_sphere, CWall *g_legowall, CWall *g_legoPlane, CSphere *g_target_blueball, CLight *g_light);
	bool threeCushion();
	void showStartPos(IDirect3DDevice9** Device, D3DXMATRIX* g_mWorld);
	void showReplay(IDirect3DDevice9** Device, D3DXMATRIX* g_mWorld);
	int cusionCount;
private:
	CSphere last_g_sphere[4], cur_g_sphere[4];
	CSphere	last_g_target_blueball, cur_g_target_blueball;
	CWall	last_g_legoPlane, cur_g_legoPlane;
	CWall	last_g_legowall[4], cur_g_legowall[4];
	CLight	last_g_light, cur_g_light;
	CSphere * currentBall;
};
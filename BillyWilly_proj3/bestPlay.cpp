#pragma once

#include "bestPlay.h"

bestPlay::bestPlay() {
	
}

void bestPlay::saveLastStatus(float timeDelta, CSphere *g_sphere, CWall *g_legowall, CWall g_legoPlane, CSphere g_target_blueball, CLight g_light) {
	this->timeDelta = timeDelta;
	for (int i = 0; i < 4; i++) {
		this->last_g_sphere[i] = g_sphere[i];
		this->last_g_legowall[i] = g_legowall[i];
	}
	this->last_g_legoPlane = g_legoPlane;
	this->last_g_target_blueball = g_target_blueball;
	this->last_g_light = g_light;
}

void bestPlay::saveCurStatus(float timeDelta, CSphere *g_sphere, CWall *g_legowall, CWall g_legoPlane, CSphere g_target_blueball, CLight g_light) {
	this->timeDelta = timeDelta; 
	for (int i = 0; i < 4; i++) {
		this->cur_g_sphere[i] = g_sphere[i];
		this->cur_g_legowall[i] = g_legowall[i];
	}
	this->cur_g_legoPlane = g_legoPlane;
	this->cur_g_target_blueball = g_target_blueball;
	this->cur_g_light = g_light;
}
void bestPlay::getLastStatus(CSphere *g_sphere, CWall *g_legowall, CWall *g_legoPlane, CSphere *g_target_blueball, CLight *g_light){
	for (int i = 0; i < 4; i++) {
		g_sphere[i] = this->last_g_sphere[i];
		g_legowall[i] = this->last_g_legowall[i];
	}
	*g_legoPlane = this->last_g_legoPlane;
	*g_target_blueball = this->last_g_target_blueball;
	*g_light = this->last_g_light;
}

void bestPlay::getCurStatus(CSphere	*g_sphere, CWall *g_legowall, CWall *g_legoPlane, CSphere *g_target_blueball, CLight *g_light){
	for (int i = 0; i < 4; i++) {
		g_sphere[i] = this->cur_g_sphere[i];
		g_legowall[i] = this->cur_g_legowall[i];
	}
	*g_legoPlane = this->cur_g_legoPlane;
	*g_target_blueball = this->cur_g_target_blueball;
	*g_light = this->cur_g_light;
}

void bestPlay::showStartPos(float timeDelta, IDirect3DDevice9** Device, D3DXMATRIX* g_mWorld, CSphere	*g_sphere, CWall *g_legowall, CWall *g_legoPlane, CSphere *g_target_blueball, CLight *g_light) {
	for (int i = 0; i < 4; i++) {
		g_sphere[i] = this->last_g_sphere[i];
		g_legowall[i] = this->last_g_legowall[i];
	}
	*g_legoPlane = this->last_g_legoPlane;
	*g_target_blueball = this->last_g_target_blueball;
	*g_light = this->last_g_light;

	if (*Device)
	{
		(*Device)->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00afafaf, 1.0f, 0);
		(*Device)->BeginScene();

		// update the position of each ball. during update, check whether each ball hit by walls.
		for (int i = 0; i < 4; i++) {
			g_sphere[i].ballUpdate(timeDelta);
			for (int j = 0; j < 4; j++){ g_legowall[i].hitBy(g_sphere[j]); }
		}

		// check whether any two balls hit together and update the direction of balls
		for (int i = 0; i < 4; i++){
			for (int j = 0; j < 4; j++) {
				if (i < j)
					g_sphere[i].hitBy(g_sphere[j]);
			}
		}

		// draw plane, walls, and spheres
		(*g_legoPlane).draw(*Device, *g_mWorld);
		for (int i = 0; i<4; i++) 	{
			g_legowall[i].draw(*Device, *g_mWorld);
			g_sphere[i].draw(*Device, *g_mWorld);
		}
		(*g_target_blueball).draw(*Device, *g_mWorld);
		(*g_light).draw(*Device);

		(*Device)->EndScene();
		(*Device)->Present(0, 0, 0, 0);
		(*Device)->SetTexture(0, NULL);
	}
}

void bestPlay::showReplay(float timeDelta, IDirect3DDevice9** Device, D3DXMATRIX* g_mWorld, CSphere	*g_sphere, CWall *g_legowall, CWall *g_legoPlane, CSphere *g_target_blueball, CLight *g_light) {

	for (int i = 0; i < 4; i++) {
		g_sphere[i] = this->cur_g_sphere[i];
		g_legowall[i] = this->last_g_legowall[i];
	}
	*g_legoPlane = this->cur_g_legoPlane;
	*g_target_blueball = this->cur_g_target_blueball;
	*g_light = this->cur_g_light;

	if (*Device)
	{
		(*Device)->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00afafaf, 1.0f, 0);
		(*Device)->BeginScene();

		// update the position of each ball. during update, check whether each ball hit by walls.
		for (int i = 0; i < 4; i++) {
			g_sphere[i].ballUpdate(0.001);
			for (int j = 0; j < 4; j++){ g_legowall[i].hitBy(g_sphere[j]); }
		}

		// check whether any two balls hit together and update the direction of balls
		for (int i = 0; i < 4; i++){
			for (int j = 0; j < 4; j++) {
				if (i < j)
					g_sphere[i].hitBy(g_sphere[j]);
			}
		}

		// draw plane, walls, and spheres
		(*g_legoPlane).draw(*Device, *g_mWorld);
		for (int i = 0; i<4; i++) 	{
			g_legowall[i].draw(*Device, *g_mWorld);
			g_sphere[i].draw(*Device, *g_mWorld);
		}
		(*g_target_blueball).draw(*Device, *g_mWorld);
		(*g_light).draw(*Device);

		(*Device)->EndScene();
		(*Device)->Present(0, 0, 0, 0);
		(*Device)->SetTexture(0, NULL);
	}

	for (int i = 0; i < 4; i++) {
		last_g_sphere[i] = this->cur_g_sphere[i];
		last_g_legowall[i] = this->cur_g_legowall[i];
	}
	last_g_legoPlane = this->cur_g_legoPlane;
	last_g_target_blueball = this->cur_g_target_blueball;
	last_g_light = this->cur_g_light;
}

bool bestPlay::threeCushion() {
	int redBallCount = 0;
	int wallCount = 0;
	bool temp;
	for (int i = 0; i < cusionCount.size(); i++) {
		if (cusionCount[i] == RED1BALL || cusionCount[i] == RED2BALL) {
			redBallCount++;
		}
		if (cusionCount[i] == WALL) {
			wallCount++;
		}
		if (redBallCount == 2) {
			if (wallCount < 1) {
				return false;
			}
			else return true;
		}
	}
}
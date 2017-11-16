﻿////////////////////////////////////////////////////////////////////////////////
//
// File: virtualLego.cpp
//
// Original Author: 박창현 Chang-hyeon Park, 
// Modified by Bong-Soo Sohn and Dong-Jun Kim
// 
// Originally programmed for Virtual LEGO. 
// Modified later to program for Virtual Billiard.
//        
////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "virtualLego.h"
#include "bestPlay.h"
#include <vector>
#include <ctime>
#include <cstdlib>
#include <cstdio>
#include <cassert>

// bestPlay instance
bestPlay *best = new bestPlay();

IDirect3DDevice9* Device = NULL;

// window size
const int Width = 1024;
const int Height = 768;

// There are four balls
// initialize the position (coordinate) of each ball (ball0 ~ ball3)
//const float spherePos[4][2] = { { -2.7f, 0 }, { +2.4f, 0 }, { 3.3f, 0 }, { -2.7f, -0.9f } };
const float spherePos[4][2] = { { -3.2f, 1.0f }, { -3.0f, 0.3f }, { 3.3f, 0 }, { 0, -1.5f } };
// initialize the color of each ball (ball0 ~ ball3)
const D3DXCOLOR sphereColor[4] = { d3d::RED, d3d::RED, d3d::YELLOW, d3d::WHITE };

// -----------------------------------------------------------------------------
// Transform matrices
// -----------------------------------------------------------------------------
D3DXMATRIX g_mWorld;
D3DXMATRIX g_mView;
D3DXMATRIX g_mProj;

#define M_RADIUS 0.21   // ball radius
#define PI 3.14159265
#define M_HEIGHT 0.01
#define DECREASE_RATE 0.9982

// -----------------------------------------------------------------------------
// CSphere class definition
// -----------------------------------------------------------------------------

CSphere::CSphere(void)
{
	D3DXMatrixIdentity(&m_mLocal);
	ZeroMemory(&m_mtrl, sizeof(m_mtrl));
	m_radius = 0;
	m_velocity_x = 0;
	m_velocity_z = 0;
	m_pSphereMesh = NULL;
}
CSphere::~CSphere(void) {}


bool CSphere::create(IDirect3DDevice9* pDevice, int index, D3DXCOLOR color = d3d::WHITE)
{
	if (NULL == pDevice)
		return false;

	this->index = index;
	m_mtrl.Ambient = color;
	m_mtrl.Diffuse = color;
	m_mtrl.Specular = color;
	m_mtrl.Emissive = d3d::BLACK;
	m_mtrl.Power = 5.0f;

	if (FAILED(D3DXCreateSphere(pDevice, getRadius(), 50, 50, &m_pSphereMesh, NULL)))
		return false;
	return true;
}

void CSphere::destroy(void)
{
	if (m_pSphereMesh != NULL) {
		m_pSphereMesh->Release();
		m_pSphereMesh = NULL;
	}
}

void CSphere::draw(IDirect3DDevice9* pDevice, const D3DXMATRIX& mWorld)
{
	if (NULL == pDevice)
		return;
	pDevice->SetTransform(D3DTS_WORLD, &mWorld);
	pDevice->MultiplyTransform(D3DTS_WORLD, &m_mLocal);
	pDevice->SetMaterial(&m_mtrl);
	m_pSphereMesh->DrawSubset(0);
}

bool CSphere::hasIntersected(CSphere& ball)
{
	D3DXVECTOR3 position = ball.getCenter();
	if (pow(abs(center_x - position.x), 2) + pow(abs(center_z - position.z), 2) < pow(1.999999999 * M_RADIUS, 2)) {
		setHasCollided(ball.getIndex(), true);
		ball.setHasCollided(this->getIndex(), true);

		return true;
	}

	return false;
}

void CSphere::hitBy(CSphere& ball)
{
	if (hasIntersected(ball)) {
		D3DXVECTOR2 ballToThis(center_x - ball.center_x, center_z - ball.center_z);		//ball의 중심으로부터 this의 중심까지의 위치벡터

		D3DXVec2Normalize(&ballToThis, &ballToThis);									//단위벡터로 전환
		double gap = M_RADIUS - (sqrt(pow(center_x - ball.center_x, 2) + pow(center_z - ball.center_z, 2)) / 2);	//공을 움직여야 하는 거리를 구함

		setCenter(center_x + (gap * ballToThis.x), center_y, center_z + (gap * ballToThis.y));						//겹친부분만큼 뒤로 이동
		ball.setCenter(ball.center_x - (gap * ballToThis.x), ball.center_y, ball.center_z - (gap * ballToThis.y));

		D3DXVECTOR2 this_vector(m_velocity_x, m_velocity_z);				//this의 속도벡터
		D3DXVECTOR2 ball_vector(ball.m_velocity_x, ball.m_velocity_z);		//ball의 속도벡터

		D3DXVECTOR2 ballToThis_perpend(-ballToThis.y, ballToThis.x);				//ballToThis의 수직벡터

		D3DXVECTOR2 this_vector_f;													//this의 나중 속도벡터
		D3DXVECTOR2 ball_vector_f;													//ball의 나중 속도벡터

		this_vector_f = D3DXVec2Dot(&this_vector, &ballToThis_perpend) * ballToThis_perpend + D3DXVec2Dot(&ball_vector, &ballToThis) * ballToThis;
		ball_vector_f = D3DXVec2Dot(&this_vector, &ballToThis) * ballToThis + D3DXVec2Dot(&ball_vector, &ballToThis_perpend) * ballToThis_perpend;

		setPower(this_vector_f.x, this_vector_f.y);
		ball.setPower(ball_vector_f.x, ball_vector_f.y);
	}
}

void CSphere::ballUpdate(float timeDiff)
{
	const float TIME_SCALE = 3.3;
	D3DXVECTOR3 cord = this->getCenter();
	double vx = abs(this->getVelocity_X());
	double vz = abs(this->getVelocity_Z());

	if (vx > 0.01 || vz > 0.01)
	{
		float tX = cord.x + TIME_SCALE*timeDiff*m_velocity_x;
		float tZ = cord.z + TIME_SCALE*timeDiff*m_velocity_z;

		//correction of position of ball
		// Please uncomment this part because this correction of ball position is necessary when a ball collides with a wall
		/*if(tX >= (4.5 - M_RADIUS))
		tX = 4.5 - M_RADIUS;
		else if(tX <=(-4.5 + M_RADIUS))
		tX = -4.5 + M_RADIUS;
		else if(tZ <= (-3 + M_RADIUS))
		tZ = -3 + M_RADIUS;
		else if(tZ >= (3 - M_RADIUS))
		tZ = 3 - M_RADIUS;*/

		this->setCenter(tX, cord.y, tZ);
	}
	else { this->setPower(0, 0); }
	//this->setPower(this->getVelocity_X() * DECREASE_RATE, this->getVelocity_Z() * DECREASE_RATE);
	double rate = 1 - (1 - DECREASE_RATE)*timeDiff * 400;
	if (rate < 0)
		rate = 0;
	this->setPower(getVelocity_X() * rate, getVelocity_Z() * rate);
}

double CSphere::getVelocity_X() { return this->m_velocity_x; }
double CSphere::getVelocity_Z() { return this->m_velocity_z; }

void CSphere::setPower(double vx, double vz)
{
	this->m_velocity_x = vx;
	this->m_velocity_z = vz;
}

void CSphere::setCenter(float x, float y, float z)
{
	D3DXMATRIX m;
	center_x = x;	center_y = y;	center_z = z;
	D3DXMatrixTranslation(&m, x, y, z);
	setLocalTransform(m);
}

float CSphere::getRadius(void)  const { return (float)(M_RADIUS); }
const D3DXMATRIX& CSphere::getLocalTransform(void) const { return m_mLocal; }
void CSphere::setLocalTransform(const D3DXMATRIX& mLocal) { m_mLocal = mLocal; }
D3DXVECTOR3 CSphere::getCenter(void) const
{
	D3DXVECTOR3 org(center_x, center_y, center_z);
	return org;
}

bool CSphere::getHasCollided(int idx) {
	return hasCollided[idx];
}

void CSphere::setHasCollided(int idx, bool bln) {
	hasCollided[idx] = bln;
}

int CSphere::getIndex() {
	return index;
}

int CSphere::getSign(double num) {
	if (num >= 0)
		return 1;

	return -1;
}

double CSphere::getSpeed(double X, double Y) {
	return sqrt(pow(X, 2) + pow(Y, 2));
}




// -----------------------------------------------------------------------------
// CWall class definition
// -----------------------------------------------------------------------------

CWall::CWall(void)
{
	D3DXMatrixIdentity(&m_mLocal);
	ZeroMemory(&m_mtrl, sizeof(m_mtrl));
	m_width = 0;
	m_depth = 0;
	m_pBoundMesh = NULL;
}
CWall::~CWall(void) {}
bool CWall::create(IDirect3DDevice9* pDevice, float ix, float iz, float iwidth, float iheight, float idepth, D3DXCOLOR color = d3d::WHITE)
{
	if (NULL == pDevice)
		return false;

	m_mtrl.Ambient = color;
	m_mtrl.Diffuse = color;
	m_mtrl.Specular = color;
	m_mtrl.Emissive = d3d::BLACK;
	m_mtrl.Power = 5.0f;

	m_width = iwidth;
	m_depth = idepth;

	if (FAILED(D3DXCreateBox(pDevice, iwidth, iheight, idepth, &m_pBoundMesh, NULL)))
		return false;
	return true;
}
void CWall::destroy(void)
{
	if (m_pBoundMesh != NULL) {
		m_pBoundMesh->Release();
		m_pBoundMesh = NULL;
	}
}
void CWall::draw(IDirect3DDevice9* pDevice, const D3DXMATRIX& mWorld)
{
	if (NULL == pDevice)
		return;
	pDevice->SetTransform(D3DTS_WORLD, &mWorld);
	pDevice->MultiplyTransform(D3DTS_WORLD, &m_mLocal);
	pDevice->SetMaterial(&m_mtrl);
	m_pBoundMesh->DrawSubset(0);
}

bool CWall::hasIntersected(CSphere& ball)
{
	D3DXVECTOR3 position = ball.getCenter();

	if (m_width > m_depth) {	//벽의 가로길이가 세로길이보다 길 
		if (abs(position.z - m_z) <= M_RADIUS + 0.06f)
			return true;
	}
	else {					//벽의 세로길이가 가로길이보다 길 때
		if (abs(position.x - m_x) <= M_RADIUS + 0.06f)
			return true;
	}

	return false;
}

void CWall::hitBy(CSphere& ball)
{
	double vx, vz;
	double tangent;
	double gap;
	D3DXVECTOR3 position = ball.getCenter();

	if (m_width > m_depth) {

	}

	if (hasIntersected(ball)) {
		// cusion hit count ++
		//best->cusionCount++;

		vx = ball.getVelocity_X();
		vz = ball.getVelocity_Z();
		tangent = abs(vz / vx);

		if (m_width > m_depth) {
			gap = M_RADIUS + 0.06f - abs(position.z - m_z);

			ball.setCenter(position.x - gap / tangent * getSign(vx), (float)M_RADIUS, position.z - gap * getSign(vz));
			ball.setPower(ball.getVelocity_X(), -ball.getVelocity_Z());
		}
		else {
			gap = M_RADIUS + 0.06f - abs(position.x - m_x);

			ball.setCenter(position.x - gap * getSign(vx), (float)M_RADIUS, position.z - gap * tangent * getSign(vz));
			ball.setPower(-ball.getVelocity_X(), ball.getVelocity_Z());
		}
	}
}

void CWall::setPosition(float x, float y, float z)
{
	D3DXMATRIX m;
	this->m_x = x;
	this->m_z = z;

	D3DXMatrixTranslation(&m, x, y, z);
	setLocalTransform(m);
}

float CWall::getHeight(void) const { return M_HEIGHT; }



void CWall::setLocalTransform(const D3DXMATRIX& mLocal) { m_mLocal = mLocal; }

int CWall::getSign(double num) {
	if (num >= 0)
		return 1;

	return -1;
}

// -----------------------------------------------------------------------------
// CLight class definition
// -----------------------------------------------------------------------------

CLight::CLight(void)
{
	static DWORD i = 0;
	m_index = i++;
	D3DXMatrixIdentity(&m_mLocal);
	::ZeroMemory(&m_lit, sizeof(m_lit));
	m_pMesh = NULL;
	m_bound._center = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_bound._radius = 0.0f;
}
CLight::~CLight(void) {}
bool CLight::create(IDirect3DDevice9* pDevice, const D3DLIGHT9& lit, float radius = 0.1f)
{
	if (NULL == pDevice)
		return false;
	if (FAILED(D3DXCreateSphere(pDevice, radius, 10, 10, &m_pMesh, NULL)))
		return false;

	m_bound._center = lit.Position;
	m_bound._radius = radius;

	m_lit.Type = lit.Type;
	m_lit.Diffuse = lit.Diffuse;
	m_lit.Specular = lit.Specular;
	m_lit.Ambient = lit.Ambient;
	m_lit.Position = lit.Position;
	m_lit.Direction = lit.Direction;
	m_lit.Range = lit.Range;
	m_lit.Falloff = lit.Falloff;
	m_lit.Attenuation0 = lit.Attenuation0;
	m_lit.Attenuation1 = lit.Attenuation1;
	m_lit.Attenuation2 = lit.Attenuation2;
	m_lit.Theta = lit.Theta;
	m_lit.Phi = lit.Phi;
	return true;
}
void CLight::destroy(void)
{
	if (m_pMesh != NULL) {
		m_pMesh->Release();
		m_pMesh = NULL;
	}
}
bool CLight::setLight(IDirect3DDevice9* pDevice, const D3DXMATRIX& mWorld)
{
	if (NULL == pDevice)
		return false;

	D3DXVECTOR3 pos(m_bound._center);
	D3DXVec3TransformCoord(&pos, &pos, &m_mLocal);
	D3DXVec3TransformCoord(&pos, &pos, &mWorld);
	m_lit.Position = pos;

	pDevice->SetLight(m_index, &m_lit);
	pDevice->LightEnable(m_index, TRUE);
	return true;
}

void CLight::draw(IDirect3DDevice9* pDevice)
{
	if (NULL == pDevice)
		return;
	D3DXMATRIX m;
	D3DXMatrixTranslation(&m, m_lit.Position.x, m_lit.Position.y, m_lit.Position.z);
	pDevice->SetTransform(D3DTS_WORLD, &m);
	pDevice->SetMaterial(&d3d::WHITE_MTRL);
	m_pMesh->DrawSubset(0);
}

D3DXVECTOR3 CLight::getPosition(void) const { return D3DXVECTOR3(m_lit.Position); }

void EveryBallVelocity::setStatus(CSphere* g_sphere){
	status = true;
	for (int i = 0; i < 4; i++){
		if (g_sphere[i].getVelocity_X() || g_sphere[i].getVelocity_Z()){
			status = false;
			break;
		}
	}
}

// -----------------------------------------------------------------------------
// Global variables
// -----------------------------------------------------------------------------
CWall	g_legoPlane;
CWall	g_legowall[4];
CSphere	g_sphere[4];
CSphere	g_target_blueball;
CSphere * currentBall;
CLight	g_light;
EveryBallVelocity everyBallVelocity;

double g_camera_pos[3] = { 0.0, 5.0, -8.0 };

// -----------------------------------------------------------------------------
// Functions
// -----------------------------------------------------------------------------


void destroyAllLegoBlock(void)
{

}

// initialization
bool Setup()
{
	int i;

	D3DXMatrixIdentity(&g_mWorld);
	D3DXMatrixIdentity(&g_mView);
	D3DXMatrixIdentity(&g_mProj);

	// create plane and set the position
	if (false == g_legoPlane.create(Device, -1, -1, 9, 0.03f, 6, d3d::GREEN)) return false;
	g_legoPlane.setPosition(0.0f, -0.0006f / 5, 0.0f);

	// create walls and set the position. note that there are four walls
	if (false == g_legowall[0].create(Device, -1, -1, 9, 0.3f, 0.12f, d3d::DARKRED)) return false;
	g_legowall[0].setPosition(0.0f, 0.12f, 3.06f);
	if (false == g_legowall[1].create(Device, -1, -1, 9, 0.3f, 0.12f, d3d::DARKRED)) return false;
	g_legowall[1].setPosition(0.0f, 0.12f, -3.06f);
	if (false == g_legowall[2].create(Device, -1, -1, 0.12f, 0.3f, 6.24f, d3d::DARKRED)) return false;
	g_legowall[2].setPosition(4.56f, 0.12f, 0.0f);
	if (false == g_legowall[3].create(Device, -1, -1, 0.12f, 0.3f, 6.24f, d3d::DARKRED)) return false;
	g_legowall[3].setPosition(-4.56f, 0.12f, 0.0f);

	// create four balls and set the position
	for (i = 0; i<4; i++) {
		if (false == g_sphere[i].create(Device, i, sphereColor[i])) return false;
		g_sphere[i].setCenter(spherePos[i][0], (float)M_RADIUS, spherePos[i][1]);
		g_sphere[i].setPower(0, 0);
		for (int j = 0; j < 4; j++) {
			g_sphere[i].setHasCollided(j, false);
		}
	}

	currentBall = &(g_sphere[2]);	//처음에 공이 한번 바뀌므로 노란공으로 설정(스페이스를 눌렀을 때 한번 바뀌므로 흰공으로 시작)

	// create blue ball for set direction
	if (false == g_target_blueball.create(Device, 4, d3d::BLUE)) return false;
	g_target_blueball.setCenter(.0f, (float)M_RADIUS, .0f);

	// light setting 
	D3DLIGHT9 lit;
	::ZeroMemory(&lit, sizeof(lit));
	lit.Type = D3DLIGHT_POINT;
	lit.Diffuse = d3d::WHITE;
	lit.Specular = d3d::WHITE * 0.9f;
	lit.Ambient = d3d::WHITE * 0.9f;
	lit.Position = D3DXVECTOR3(0.0f, 3.0f, 0.0f);
	lit.Range = 100.0f;
	lit.Attenuation0 = 0.0f;
	lit.Attenuation1 = 0.9f;
	lit.Attenuation2 = 0.0f;
	if (false == g_light.create(Device, lit))
		return false;

	// Position and aim the camera.
	D3DXVECTOR3 pos(0.0f, 5.0f, -8.0f);
	D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 up(0.0f, 2.0f, 0.0f);
	D3DXMatrixLookAtLH(&g_mView, &pos, &target, &up);
	Device->SetTransform(D3DTS_VIEW, &g_mView);

	// Set the projection matrix.
	D3DXMatrixPerspectiveFovLH(&g_mProj, D3DX_PI / 4,
		(float)Width / (float)Height, 1.0f, 100.0f);
	Device->SetTransform(D3DTS_PROJECTION, &g_mProj);

	// Set render states.
	Device->SetRenderState(D3DRS_LIGHTING, TRUE);
	Device->SetRenderState(D3DRS_SPECULARENABLE, TRUE);
	Device->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);

	g_light.setLight(Device, g_mWorld);
	return true;
}

void Cleanup(void)
{
	g_legoPlane.destroy();
	for (int i = 0; i < 4; i++) {
		g_legowall[i].destroy();
	}
	destroyAllLegoBlock();
	g_light.destroy();
}


// timeDelta represents the time between the current image frame and the last image frame.
// the distance of moving balls should be "velocity * timeDelta"
bool Display(float timeDelta)
{
	int i = 0;
	int j = 0;


	if (Device)
	{
		Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00afafaf, 1.0f, 0);
		Device->BeginScene();

		// update the position of each ball. during update, check whether each ball hit by walls.
		for (i = 0; i < 4; i++) {
			g_sphere[i].ballUpdate(timeDelta);
			for (j = 0; j < 4; j++){ g_legowall[i].hitBy(g_sphere[j]); }
		}

		// check whether any two balls hit together and update the direction of balls
		for (i = 0; i < 4; i++){
			for (j = 0; j < 4; j++) {
				if (i < j)
					g_sphere[i].hitBy(g_sphere[j]);
			}
		}

		// draw plane, walls, and spheres
		g_legoPlane.draw(Device, g_mWorld);
		for (i = 0; i<4; i++) 	{
			g_legowall[i].draw(Device, g_mWorld);
			g_sphere[i].draw(Device, g_mWorld);
		}
		g_target_blueball.draw(Device, g_mWorld);
		g_light.draw(Device);

		Device->EndScene();
		Device->Present(0, 0, 0, 0);
		Device->SetTexture(0, NULL);


	}
	return true;
}

void changeBall() {
	if (currentBall == &(g_sphere[3])) {
		currentBall = &(g_sphere[2]);
	}
	else {
		currentBall = &(g_sphere[3]);
	}
}

LRESULT CALLBACK d3d::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static bool wire = false;
	static bool isReset = true;
	static int old_x = 0;
	static int old_y = 0;
	static enum { WORLD_MOVE, LIGHT_MOVE, BLOCK_MOVE } move = WORLD_MOVE;

	everyBallVelocity.setStatus(g_sphere);

	switch (msg) {
	case WM_DESTROY:
	{
					   ::PostQuitMessage(0);
					   break;
	}
	case WM_KEYDOWN:
	{
					   switch (wParam) {
					   case VK_ESCAPE:
						   ::DestroyWindow(hwnd);
						   break;
					   case VK_RETURN:
						   if (NULL != Device) {
							   wire = !wire;
							   Device->SetRenderState(D3DRS_FILLMODE,
								   (wire ? D3DFILL_WIREFRAME : D3DFILL_SOLID));
						   }
						   break;
					   case VK_F1:
						   // 출발 상태 디스플레이 띄우기
						   best->showStartPos(&Device, &g_mWorld);
						   break;
					   case VK_F2:
						   // 이동 정보 디스플레이 띄우기
						   best->showReplay(&Device, &g_mWorld);
						   break;
					   case VK_SPACE:
						   if (everyBallVelocity.isZero()){//공이 멈춰있는지 확인
							   // 출발 상태 저장
							   best->saveLastStatus(g_sphere, g_legowall, g_legoPlane, g_target_blueball, g_light);

							   //CSphere * currentBall;
							   //현재 공이 빨간공 두개만 맞췄을 경우가 아니면 공을 바꾼다
							   if (currentBall->getHasCollided(0) && currentBall->getHasCollided(1) && !currentBall->getHasCollided(2) && !currentBall->getHasCollided(3)) {
							   }
							   else {
								   changeBall();
							   }
							   D3DXVECTOR3 targetpos = g_target_blueball.getCenter();
							   D3DXVECTOR3	currentpos = currentBall->getCenter();
							   double theta = acos(sqrt(pow(targetpos.x - currentpos.x, 2)) / sqrt(pow(targetpos.x - currentpos.x, 2) +
								   pow(targetpos.z - currentpos.z, 2)));		// 기본 1 사분면
							   if (targetpos.z - currentpos.z <= 0 && targetpos.x - currentpos.x >= 0) { theta = -theta; }	//4 사분면
							   if (targetpos.z - currentpos.z >= 0 && targetpos.x - currentpos.x <= 0) { theta = PI - theta; } //2 사분면
							   if (targetpos.z - currentpos.z <= 0 && targetpos.x - currentpos.x <= 0){ theta = PI + theta; } // 3 사분면
							   double distance = sqrt(pow(targetpos.x - currentpos.x, 2) + pow(targetpos.z - currentpos.z, 2));
							   currentBall->setPower(distance * cos(theta), distance * sin(theta));

							   for (int i = 0; i < 4; i++) {
								   for (int j = 0; j < 4; j++) {
									   g_sphere[i].setHasCollided(j, false);
								   }
							   }
							   // 이동 상태 저장 (Replay 용)
							   best->saveCurStatus(g_sphere, g_legowall, g_legoPlane, g_target_blueball, g_light);
							   break;
						   }
						   break;
					   }
	}

	case WM_MOUSEMOVE:
	{
						 int new_x = LOWORD(lParam);
						 int new_y = HIWORD(lParam);
						 float dx;
						 float dy;

						 if (LOWORD(wParam) & MK_LBUTTON) {

							 if (isReset) {
								 isReset = false;
							 }
							 else {
								 D3DXVECTOR3 vDist;
								 D3DXVECTOR3 vTrans;
								 D3DXMATRIX mTrans;
								 D3DXMATRIX mX;
								 D3DXMATRIX mY;

								 switch (move) {
								 case WORLD_MOVE:
									 dx = (old_x - new_x) * 0.01f;
									 dy = (old_y - new_y) * 0.01f;
									 D3DXMatrixRotationY(&mX, dx);
									 D3DXMatrixRotationX(&mY, dy);
									 g_mWorld = g_mWorld * mX * mY;

									 break;
								 }
							 }

							 old_x = new_x;
							 old_y = new_y;

						 }
						 else {
							 isReset = true;

							 if (LOWORD(wParam) & MK_RBUTTON) {
								 dx = (old_x - new_x);// * 0.01f;
								 dy = (old_y - new_y);// * 0.01f;

								 D3DXVECTOR3 coord3d = g_target_blueball.getCenter();
								 g_target_blueball.setCenter(coord3d.x + dx*(-0.007f), coord3d.y, coord3d.z + dy*0.007f);
							 }
							 old_x = new_x;
							 old_y = new_y;

							 move = WORLD_MOVE;
						 }
						 break;
	}
	}

	return ::DefWindowProc(hwnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hinstance,
	HINSTANCE prevInstance,
	PSTR cmdLine,
	int showCmd)
{
	srand(static_cast<unsigned int>(time(NULL)));

	if (!d3d::InitD3D(hinstance,
		Width, Height, true, D3DDEVTYPE_HAL, &Device))
	{
		::MessageBox(0, "InitD3D() - FAILED", 0, 0);
		return 0;
	}

	if (!Setup())
	{
		::MessageBox(0, "Setup() - FAILED", 0, 0);
		return 0;
	}
	d3d::EnterMsgLoop(Display);

	Cleanup();

	Device->Release();

	return 0;
}

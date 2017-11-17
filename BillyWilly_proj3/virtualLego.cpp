////////////////////////////////////////////////////////////////////////////////
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
#include "d3dUtility.h"
#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <cstdio>
#include <cassert>

// bestPlay instance
bestPlay *best = new bestPlay();
// mingyu part
#include "Game.h"
#include "ScoreManager.h"
// mingyu part

#include<Windows.h>                        // 소리 재생을 위한 헤더파일 !
#include<MMSystem.h>
#pragma comment(lib,"Winmm.lib")
#include <iostream>
#include "resource.h"

int Sound = 0;                                   // 소리 켜져있을 때는 0 !


// Juhyeon part header
#include "C3DText.h"		//3D Text 표시를 위해서 정의
#include "CText.h"			//Text 표시를 위해서 새로운 자료구조 정의
#include "Rect.h"			//사각형 및 원 정의
// 헤더에 Define 도 필수 추가 

// Juhyeon part header

IDirect3DDevice9* Device = NULL;

// window size
const int Width = 1024;
const int Height = 768;

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

// mingyu part
Game* game = new Game();
// mingyu part

bool VK_SPACE_interrupt = false;

//JuHyeon Part
Circle Ball[8];
C3DText intro;
C3DText intro2;
CText Mode[8];
int State = 0;  // 0은 처음화면(사운드에 따라 2화면) 1~4는 mode 1~4 5는 rank 6은 도움말 

//JuHyeon Part


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
#define RED1BALL 0
#define RED2BALL 1
#define YELLOWBALL 2
#define WHITEBALL 3
#define WALL 5

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
		
		// 추가 !
		float ball_vx = ball.getVelocity_X();
		float ball_vz = ball.getVelocity_Z();
		float ball_v_result = (pow(ball_vx, 2) + pow(ball_vz, 2)) * 100;

		if (Sound == 0)                             // 소리가 켜져 있을때
		{

			if (ball_v_result>800) {
				PlaySound(MAKEINTRESOURCE(IDR_WAVE9), NULL, SND_RESOURCE | SND_ASYNC | SND_NOSTOP);						// 부딪히는 소리 재생         // 5단계
			}

			else if (ball_v_result > 600)
			{
				PlaySound(MAKEINTRESOURCE(IDR_WAVE8), NULL, SND_RESOURCE | SND_ASYNC | SND_NOSTOP);                   // 부딪히는 소리 재생         // 4단계		
			}
			else if (ball_v_result > 400)
			{
				PlaySound(MAKEINTRESOURCE(IDR_WAVE7), NULL, SND_RESOURCE | SND_ASYNC | SND_NOSTOP);                   // 부딪히는 소리 재생         // 3단계
			}
			else if (ball_v_result > 200)
			{
				PlaySound(MAKEINTRESOURCE(IDR_WAVE6), NULL, SND_RESOURCE | SND_ASYNC | SND_NOSTOP);                // 부딪히는 소리 재생         // 2단계
			}
			else
			{
				PlaySound(MAKEINTRESOURCE(IDR_WAVE5), NULL, SND_RESOURCE | SND_ASYNC | SND_NOSTOP);           // 부딪히는 소리 재생         // 1단계
			}

		}
		
		
		setHasCollided(ball.getIndex(), true);
		ball.setHasCollided(this->getIndex(), true);

		// 지금 움직이는 공인 흰색 or 노란색인데 빨간 공을 쳤을 경우
		if (ball.getIndex() == currentBall->getIndex() && (this->getIndex() == RED1BALL || this->getIndex() == RED2BALL)) {
			best->cusionCount.push_back(RED1BALL);
		}
		return true;
	}

	return false;
}

void CSphere::hitBy(CSphere& ball)
{
	if (hasIntersected(ball)) {

		if (this->deductScore(ball) && Sound == 0)                     // 빡이 난 경우    !
		{
			PlaySound(MAKEINTRESOURCE(IDR_WAVE11), NULL, SND_RESOURCE | SND_ASYNC | SND_NOSTOP);
		}

		this->hit_count++;                                // 충돌했을 경우 추가 !


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

bool CSphere::deductScore(CSphere &ball)                                     // 빡이 났는지 검사하는 함수 !
{
	D3DXVECTOR3 position = ball.getCenter();

	if (this->index == 2)
	{
		if (pow(abs(center_x - position.x), 2) + pow(abs(center_z - position.z), 2) < pow(1.999999999 * M_RADIUS, 2) && ball.index == 3)
		{
			return true;
		}

	}

	else if (this->index == 3)
	{
		if (pow(abs(center_x - position.x), 2) + pow(abs(center_z - position.z), 2) < pow(1.999999999 * M_RADIUS, 2) && ball.index == 2)
		{
			return true;
		}
	}


	return false;
}

void CSphere::sethit_count(int num)              // hit_count를 set하는 함수!
{
	this->hit_count = num;
}

int CSphere::gethit_count()              // hit_count를 get하는 함수!
{
	return (this->hit_count);
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
		if (ball.getIndex() == currentBall->getIndex()){
			best->cusionCount.push_back(WALL);
		}
		
		// 추가 !

		float ball_vx = ball.getVelocity_X();
		float ball_vz = ball.getVelocity_Z();
		float ball_v_result = (pow(ball_vx, 2) + pow(ball_vz, 2)) * 100;


		if (Sound == 0)
		{
			if (ball_v_result>800) {
				PlaySound(MAKEINTRESOURCE(IDR_WAVE4), NULL, SND_RESOURCE | SND_ASYNC | SND_NOSTOP);						// 부딪히는 소리 재생         // 3단계
			}
			else if (ball_v_result > 400)
			{
				PlaySound(MAKEINTRESOURCE(IDR_WAVE3), NULL, SND_RESOURCE | SND_ASYNC | SND_NOSTOP);						// 부딪히는 소리 재생         // 2단계
			}
			else
			{
				PlaySound(MAKEINTRESOURCE(IDR_WAVE2), NULL, SND_RESOURCE | SND_ASYNC | SND_NOSTOP);						// 부딪히는 소리 재생         // 1단계
			}
		}
		
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
	cur_status = true;
	for (int i = 0; i < 4; i++){
		if (abs(g_sphere[i].getVelocity_X()) >0.0001 || abs(g_sphere[i].getVelocity_Z()) > 0.0001) { // i번째 공이 멈춰있나?
			cur_status = false;
			break;
		}
	}
	
	if (pre_status == false && cur_status == true){
		finishTurn = true;
	}
	else{
		finishTurn = false;
	}

	pre_status = cur_status;
}

bool EveryBallVelocity::isFinishTurn(){
	return finishTurn;
}

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

	currentBall = &(g_sphere[3]);	//처음에 공이 한번 바뀌므로 노란공으로 설정(스페이스를 눌렀을 때 한번 바뀌므로 흰공으로 시작)

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

	//Juhyeon Part
	intro.create(Device, 40, 20, 90, "Times New Roman");
	intro.Set(Device, "Billy Willy!");
	intro.setPosition(-4.5, 2.5f, 0);
	intro2.create(Device, 40, 20, 90, "Times New Roman");
	intro2.Set(Device, "Welcome MY GAME!");
	intro2.setPosition(-3.82, 1.5f, 0);

	Mode[0].Init(Device, 27, 10, 700, "Airal");
	Mode[1].Init(Device, 27, 10, 700, "Airal");
	Mode[2].Init(Device, 27, 10, 700, "Airal");
	Mode[3].Init(Device, 27, 10, 700, "Airal");
	Mode[4].Init(Device, 27, 10, 700, "Airal");
	Mode[5].Init(Device, 27, 10, 700, "Airal");
	Mode[6].Init(Device, 27, 10, 700, "Airal");
	Mode[7].Init(Device, 27, 10, 700, "Airal");

	Ball[0].InitVB(Device, 180, 450, 60, d3d::RED);
	Ball[1].InitVB(Device, 380, 450, 60, d3d::RED);
	Ball[2].InitVB(Device, 580, 450, 60, d3d::RED);
	Ball[3].InitVB(Device, 780, 450, 60, d3d::RED);
	Ball[4].InitVB(Device, 280, 550, 60, d3d::BLUE);
	Ball[5].InitVB(Device, 480, 550, 60, d3d::BLUE);
	Ball[6].InitVB(Device, 680, 550, 60, d3d::BLUE);
	Ball[7].InitVB(Device, 680, 550, 60, d3d::RED);
	//Juhyeon Part


	// Set render states.
	Device->SetRenderState(D3DRS_LIGHTING, TRUE);
	Device->SetRenderState(D3DRS_SPECULARENABLE, TRUE);
	Device->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);

	g_light.setLight(Device, g_mWorld);

	// mingyu part
	if ((ScoreManager::getInstance())->loadRank() == false){
		return false;
	}
	// mingyu part

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

	if (State == 0 && Sound ==0) {  // 소리 켜져잇고 첫화면  주현
		Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00afafaf, 1.0f, 0);
		Device->BeginScene();
		

		D3DXVECTOR3 pos(0.0f, 0.0f, -10.0f);
		D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
		D3DXMatrixLookAtLH(&g_mView, &pos, &target, &up);
		Device->SetTransform(D3DTS_VIEW, &g_mView);


		D3DXMatrixPerspectiveFovLH(&g_mProj, D3DX_PI / 3.2,
			(float)Width / (float)Height, 0.f, 10000.0f);
		Device->SetTransform(D3DTS_PROJECTION, &g_mProj);

		Ball[0].Draw(Device);
		Ball[1].Draw(Device);
		Ball[2].Draw(Device);
		Ball[3].Draw(Device);
		Ball[4].Draw(Device);
		Ball[5].Draw(Device);
		Ball[6].Draw(Device);

		Mode[0].Print("Mode1", 150, 440, d3d::WHITE);
		Mode[1].Print("Mode2", 350, 440, d3d::WHITE);
		Mode[2].Print("Mode3", 550, 440, d3d::WHITE);
		Mode[3].Print("Mode4", 750, 440, d3d::WHITE);
		Mode[4].Print("RANKING", 235, 540, d3d::WHITE);
		Mode[5].Print("HELP", 455, 540, d3d::WHITE);
		Mode[6].Print("SOUND", 645, 540, d3d::WHITE);

		intro.draw(Device, g_mWorld);
		intro2.draw(Device, g_mWorld);

		Device->EndScene();
		Device->Present(0, 0, 0, 0);
		Device->SetTexture(0, NULL);
	}
	else if (State == 0 && Sound == 1) {		//소리 꺼져있고 첫화면 주현
		Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00afafaf, 1.0f, 0);
		Device->BeginScene();



		D3DXVECTOR3 pos(0.0f, 0.0f, -10.0f);
		D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
		D3DXMatrixLookAtLH(&g_mView, &pos, &target, &up);
		Device->SetTransform(D3DTS_VIEW, &g_mView);


		D3DXMatrixPerspectiveFovLH(&g_mProj, D3DX_PI / 3.2,
			(float)Width / (float)Height, 0.f, 10000.0f);
		Device->SetTransform(D3DTS_PROJECTION, &g_mProj);

		Ball[0].Draw(Device);
		Ball[1].Draw(Device);
		Ball[2].Draw(Device);
		Ball[3].Draw(Device);
		Ball[4].Draw(Device);
		Ball[5].Draw(Device);
		Ball[7].Draw(Device);

		Mode[0].Print("Mode1", 150, 440, d3d::WHITE);
		Mode[1].Print("Mode2", 350, 440, d3d::WHITE);
		Mode[2].Print("Mode3", 550, 440, d3d::WHITE);
		Mode[3].Print("Mode4", 750, 440, d3d::WHITE);
		Mode[4].Print("RANKING", 235, 540, d3d::WHITE);
		Mode[5].Print("HELP", 455, 540, d3d::WHITE);
		Mode[7].Print("MUTE", 650, 540, d3d::WHITE);

		intro.draw(Device, g_mWorld);
		intro2.draw(Device, g_mWorld);

		Device->EndScene();
		Device->Present(0, 0, 0, 0);
		Device->SetTexture(0, NULL);
	}
	else if (State == 1) { //게임모드 1 주현
		if (Device)
		{
			Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00afafaf, 1.0f, 0);
			Device->BeginScene();

			// update the position of each ball. during update, check whether each ball hit by walls.
			for (i = 0; i < 4; i++) {
				g_sphere[i].ballUpdate(timeDelta);
				for (j = 0; j < 4; j++) { g_legowall[i].hitBy(g_sphere[j]); }
			}

			// check whether any two balls hit together and update the direction of balls
			for (i = 0; i < 4; i++) {
				for (j = 0; j < 4; j++) {
					if (i < j)
						g_sphere[i].hitBy(g_sphere[j]);
				}
			}

			// draw plane, walls, and spheres
			g_legoPlane.draw(Device, g_mWorld);
			for (i = 0; i < 4; i++) {
				g_legowall[i].draw(Device, g_mWorld);
				g_sphere[i].draw(Device, g_mWorld);
			}
			g_target_blueball.draw(Device, g_mWorld);
			g_light.draw(Device);

			IDirect3DDevice9* g_pd3dDevice;
			//ID3DXLine* g_pLine;
			//D3DXCreateLine(g_pd3dDevice, &g_pLine); // Line 생성
			//g_pLine->SetWidth(2); // 라인의 굵기를 2로 설정
			//D3DXVECTOR3 lines[] = { currentBall->getCenter(), g_target_blueball.getCenter() };

			Device->EndScene();
			Device->Present(0, 0, 0, 0);
			Device->SetTexture(0, NULL);

			//mingyu part -debug
			//const char str[100] = "";;
			string str = "v_x:" + std::to_string(currentBall->getVelocity_X()) + ", v_z:" + std::to_string(currentBall->getVelocity_Z()) + "\n";
			OutputDebugString(str.c_str());
			//mingyu part
		}
	}
	else if (State == 2) { //게임모드 2 주현
		
	}
	else if (State == 3) { //게임모드 3 주현

	}
	else if (State == 4) { //게임모드 4 주현

	}
	else if (State == 5) { // 랭킹 볼수있는 화면 주현
		Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00afafaf, 1.0f, 0);
		Device->BeginScene();



		D3DXVECTOR3 pos(0.0f, 0.0f, -10.0f);
		D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
		D3DXMatrixLookAtLH(&g_mView, &pos, &target, &up);
		Device->SetTransform(D3DTS_VIEW, &g_mView);


		D3DXMatrixPerspectiveFovLH(&g_mProj, D3DX_PI / 3.2,
			(float)Width / (float)Height, 0.f, 10000.0f);
		Device->SetTransform(D3DTS_PROJECTION, &g_mProj);

		Device->EndScene();
		Device->Present(0, 0, 0, 0);
		Device->SetTexture(0, NULL);

	}
	else if (State == 6) { // 도움말 설명 주현
		Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00afafaf, 1.0f, 0);
		Device->BeginScene();



		D3DXVECTOR3 pos(0.0f, 0.0f, -10.0f);
		D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
		D3DXMatrixLookAtLH(&g_mView, &pos, &target, &up);
		Device->SetTransform(D3DTS_VIEW, &g_mView);


		D3DXMatrixPerspectiveFovLH(&g_mProj, D3DX_PI / 3.2,
			(float)Width / (float)Height, 0.f, 10000.0f);
		Device->SetTransform(D3DTS_PROJECTION, &g_mProj);

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

	currentBall->sethit_count(0);                      // hit_count 초기화 !
}

void drawLine(CSphere *currentball, CSphere redball1, CSphere redball2, CSphere yellowball, CSphere whiteball, CSphere blueball){
	if (currentBall->getIndex() == yellowball.getIndex()){

	}
	else if (currentBall->getIndex() == whiteball.getIndex()){

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


		// 턴 끝날때 호출되는 call back
		if (everyBallVelocity.isFinishTurn()) {

			//::MessageBox(0, "isFinishTurn", 0, 0);
			// 빨간 공 두개를 맞추고나서 세번째로 벽을 쳤을 경우 제외
			if (best->threeCushion()) {

				if (Sound == 0)             // 최고의 플레이 ~!
				{
					PlaySound(MAKEINTRESOURCE(IDR_WAVE14), NULL, SND_RESOURCE | SND_ASYNC | SND_NOSTOP);
				}

				best->showStartPos(&Device, &g_mWorld, g_sphere, g_legowall, &g_legoPlane, &g_target_blueball, &g_light);

				best->showReplay(&Device, &g_mWorld, g_sphere, g_legowall, &g_legoPlane, &g_target_blueball, &g_light);

			}


			for (int i = 0; i < best->cusionCount.size(); i++) {
				best->cusionCount[i] = 0;
			}

			// mingyu part
			// question: isFinishTurn() 이 true 가 되는 시점이 정확히 언제인가?
			bool isTurnChange = true;
			bool hasCollided[4];
			for (int i = 0; i < 4; i++) {
				hasCollided[i] = currentBall->getHasCollided(i);
			}
			game->onTurnEnd(currentBall->getIndex(), hasCollided, isTurnChange);
			if (isTurnChange == true) {
				//::MessageBox(0, "changeBall", 0, 0);
				changeBall();

				if (currentBall->getIndex() == 3 && Sound == 0)                  // 바뀐 공이 흰색이면 ! 마이턴 ! 1 ~ 4 모드 !
				{
					PlaySound(MAKEINTRESOURCE(IDR_WAVE1), NULL, SND_RESOURCE | SND_ASYNC | SND_NOSTOP);
				}

			}

		}
		
		switch (msg) {

		case WM_DESTROY:
		{
			::PostQuitMessage(0);
			break;
		}
		case WM_KEYDOWN:
		{
			switch (wParam) {
			case VK_TAB:		//주현 Tap 누르면 첫화면
				State = 0;
				if (Sound == 0)                        // 빌리 윌리!
				{
					PlaySound(MAKEINTRESOURCE(IDR_WAVE10), NULL, SND_RESOURCE | SND_ASYNC | SND_NOSTOP);
				}
				break;

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
				best->showStartPos(&Device, &g_mWorld, g_sphere, g_legowall, &g_legoPlane, &g_target_blueball, &g_light);
				break;
			case VK_F2:
				best->showReplay(&Device, &g_mWorld, g_sphere, g_legowall, &g_legoPlane, &g_target_blueball, &g_light);
				break;
			case VK_SPACE:
				if (everyBallVelocity.isZero()) {
					// 출발 상태 저장
					best->saveLastStatus(d3d::getTimeGap(), g_sphere, g_legowall, g_legoPlane, g_target_blueball, g_light);

					VK_SPACE_interrupt = true;

					//CSphere * currentBall;
					//현재 공이 빨간공 두개만 맞췄을 경우가 아니면 공을 바꾼다
					if (currentBall->getHasCollided(0) && currentBall->getHasCollided(1) && !currentBall->getHasCollided(2) && !currentBall->getHasCollided(3)) {

					}
					else {
						// mingyu 가 지웠어요!
						// changeBall();

					}
					D3DXVECTOR3 targetpos = g_target_blueball.getCenter();
					D3DXVECTOR3	currentpos = currentBall->getCenter();

					double theta = acos(sqrt(pow(targetpos.x - currentpos.x, 2)) / sqrt(pow(targetpos.x - currentpos.x, 2) +
						pow(targetpos.z - currentpos.z, 2)));		// 기본 1 사분면
					if (targetpos.z - currentpos.z <= 0 && targetpos.x - currentpos.x >= 0) { theta = -theta; }	//4 사분면
					if (targetpos.z - currentpos.z >= 0 && targetpos.x - currentpos.x <= 0) { theta = PI - theta; } //2 사분면
					if (targetpos.z - currentpos.z <= 0 && targetpos.x - currentpos.x <= 0) { theta = PI + theta; } // 3 사분면
					double distance = sqrt(pow(targetpos.x - currentpos.x, 2) + pow(targetpos.z - currentpos.z, 2));
					currentBall->setPower(distance * cos(theta), distance * sin(theta));

					for (int i = 0; i < 4; i++) {
						for (int j = 0; j < 4; j++) {
							g_sphere[i].setHasCollided(j, false);
						}
					}
					// 이동 상태 저장 (Replay 용)
					best->saveCurStatus(d3d::getTimeGap(), g_sphere, g_legowall, g_legoPlane, g_target_blueball, g_light);

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
		case WM_LBUTTONDOWN:
			if (State == 0)
			{
				int new_x = LOWORD(lParam);
				int new_y = HIWORD(lParam);
				if (pow(new_x - 180, 2) + pow(new_y - 450, 2) <= 3600)
				{
					//Game Mode 1
					D3DXVECTOR3 pos(0.0f, 7, -8);
					D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
					D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
					D3DXMatrixLookAtLH(&g_mView, &pos, &target, &up);

					Device->SetTransform(D3DTS_VIEW, &g_mView);

					// Set the projection matrix.
					D3DXMatrixPerspectiveFovLH(&g_mProj, D3DX_PI / 3.5,
						(float)Width / (float)Height, 1.f, 10000.0f);
					Device->SetTransform(D3DTS_PROJECTION, &g_mProj);
					State = 1;
				}
				if (pow(new_x - 380, 2) + pow(new_y - 450, 2) <= 3600)
				{
					//Game Mode 2
					D3DXVECTOR3 pos(0.0f, 7, -8);
					D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
					D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
					D3DXMatrixLookAtLH(&g_mView, &pos, &target, &up);

					Device->SetTransform(D3DTS_VIEW, &g_mView);

					// Set the projection matrix.
					D3DXMatrixPerspectiveFovLH(&g_mProj, D3DX_PI / 3.5,
						(float)Width / (float)Height, 1.f, 10000.0f);
					Device->SetTransform(D3DTS_PROJECTION, &g_mProj);
					State = 2;

				}
				if (pow(new_x - 580, 2) + pow(new_y - 450, 2) <= 3600)
				{
					//Game Mode 3
					D3DXVECTOR3 pos(0.0f, 7, -8);
					D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
					D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
					D3DXMatrixLookAtLH(&g_mView, &pos, &target, &up);

					Device->SetTransform(D3DTS_VIEW, &g_mView);

					// Set the projection matrix.
					D3DXMatrixPerspectiveFovLH(&g_mProj, D3DX_PI / 3.5,
						(float)Width / (float)Height, 1.f, 10000.0f);
					Device->SetTransform(D3DTS_PROJECTION, &g_mProj);
					State = 3;

				}
				if (pow(new_x - 780, 2) + pow(new_y - 450, 2) <= 3600)
				{
					//Game Mode 4
					D3DXVECTOR3 pos(0.0f, 7, -8);
					D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
					D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
					D3DXMatrixLookAtLH(&g_mView, &pos, &target, &up);

					Device->SetTransform(D3DTS_VIEW, &g_mView);

					// Set the projection matrix.
					D3DXMatrixPerspectiveFovLH(&g_mProj, D3DX_PI / 3.5,
						(float)Width / (float)Height, 1.f, 10000.0f);
					Device->SetTransform(D3DTS_PROJECTION, &g_mProj);
					State = 4;

				}

				if (pow(new_x - 280, 2) + pow(new_y - 550, 2) <= 3600)
				{
					//Ranking
					D3DXVECTOR3 pos(0.0f, 7, -8);
					D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
					D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
					D3DXMatrixLookAtLH(&g_mView, &pos, &target, &up);

					Device->SetTransform(D3DTS_VIEW, &g_mView);

					// Set the projection matrix.
					D3DXMatrixPerspectiveFovLH(&g_mProj, D3DX_PI / 3.5,
						(float)Width / (float)Height, 1.f, 10000.0f);
					Device->SetTransform(D3DTS_PROJECTION, &g_mProj);
					State = 5;

				}
				if (pow(new_x - 480, 2) + pow(new_y - 550, 2) <= 3600)
				{
					//Help
					D3DXVECTOR3 pos(0.0f, 7, -8);
					D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
					D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
					D3DXMatrixLookAtLH(&g_mView, &pos, &target, &up);

					Device->SetTransform(D3DTS_VIEW, &g_mView);

					// Set the projection matrix.
					D3DXMatrixPerspectiveFovLH(&g_mProj, D3DX_PI / 3.5,
						(float)Width / (float)Height, 1.f, 10000.0f);
					Device->SetTransform(D3DTS_PROJECTION, &g_mProj);
					State = 6;

				}
				if (pow(new_x - 680, 2) + pow(new_y - 550, 2) <= 3600)
				{
					//Sound
					D3DXVECTOR3 pos(0.0f, 7, -8);
					D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
					D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
					D3DXMatrixLookAtLH(&g_mView, &pos, &target, &up);

					Device->SetTransform(D3DTS_VIEW, &g_mView);

					// Set the projection matrix.
					D3DXMatrixPerspectiveFovLH(&g_mProj, D3DX_PI / 3.5,
						(float)Width / (float)Height, 1.f, 10000.0f);
					Device->SetTransform(D3DTS_PROJECTION, &g_mProj);
					if (Sound == 0) {
						Sound = 1;
					}
					else {
						Sound = 0;
					}
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

	if(Sound == 0)                        // 빌리 윌리!
	{
		PlaySound(MAKEINTRESOURCE(IDR_WAVE10), NULL, SND_RESOURCE | SND_ASYNC | SND_NOSTOP);
	}


	d3d::EnterMsgLoop(Display);

	Cleanup();

	Device->Release();

	return 0;
}

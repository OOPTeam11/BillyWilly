#pragma once

#include "d3dUtility.h"

// bestPlay predeclaration
class best;

class CSphere {
private:
	float					center_x, center_y, center_z;
	float                   m_radius;
	float					m_velocity_x;
	float					m_velocity_z;
	int						index;
	bool					hasCollided[4];				//다음 공을 치기 전까지 다른 공들과 부딪혔는지 여부를 알려줌


public:
	CSphere(void);
	~CSphere(void);

public:
	bool create(IDirect3DDevice9* pDevice, int index, D3DXCOLOR color);
	void destroy(void);
	void draw(IDirect3DDevice9* pDevice, const D3DXMATRIX& mWorld);
	bool hasIntersected(CSphere& ball);
	void hitBy(CSphere& ball);
	void ballUpdate(float timeDiff);
	double getVelocity_X();
	double getVelocity_Z();
	void setPower(double vx, double vz);
	void setCenter(float x, float y, float z);
	float getRadius(void) const;
	const D3DXMATRIX& getLocalTransform(void) const;
	void setLocalTransform(const D3DXMATRIX& mLocal);
	D3DXVECTOR3 getCenter(void) const;
	bool getHasCollided(int idx);
	void setHasCollided(int idx, bool bln);
	int getIndex();
	bool isFinishTurn();
private:
	D3DXMATRIX              m_mLocal;
	D3DMATERIAL9            m_mtrl;
	ID3DXMesh*              m_pSphereMesh;
	int getSign(double num);
	double getSpeed(double X, double Y);
};

class CWall {

private:

	float					m_x;
	float					m_z;
	float                   m_width;
	float                   m_depth;
	float					m_height;

public:
	CWall(void);
	~CWall(void);
public:
	bool create(IDirect3DDevice9* pDevice, float ix, float iz, float iwidth, float iheight, float idepth, D3DXCOLOR color);
	void destroy(void);
	void draw(IDirect3DDevice9* pDevice, const D3DXMATRIX& mWorld);
	bool hasIntersected(CSphere& ball);
	void hitBy(CSphere& ball);
	void setPosition(float x, float y, float z);
	float getHeight(void) const;

private:
	void setLocalTransform(const D3DXMATRIX& mLocal);
	int getSign(double num);
	D3DXMATRIX              m_mLocal;
	D3DMATERIAL9            m_mtrl;
	ID3DXMesh*              m_pBoundMesh;
};


class CLight {
public:
	CLight(void);
	~CLight(void);
public:
	bool create(IDirect3DDevice9* pDevice, const D3DLIGHT9& lit, float radius);
	void destroy(void);
	bool setLight(IDirect3DDevice9* pDevice, const D3DXMATRIX& mWorld);
	void draw(IDirect3DDevice9* pDevice);
	D3DXVECTOR3 getPosition(void) const;

private:
	DWORD               m_index;
	D3DXMATRIX          m_mLocal;
	D3DLIGHT9           m_lit;
	ID3DXMesh*          m_pMesh;
	d3d::BoundingSphere m_bound;
};

class EveryBallVelocity{
public:
	EveryBallVelocity(){}
	~EveryBallVelocity(){}
	bool isZero(){
		return status;
	}
	void setStatus(CSphere*);
private:
	bool status;
};
#include "d3dUtility.h"
#include <vector>
#include <ctime>
#include <cstdlib>
#include <cstdio>
#include "Define.h"
#include <cassert>
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZRHW | D3DFVF_DIFFUSE )


class Rect
{
	struct CUSTOMVERTEX
	{
		FLOAT X, Y, Z, RHW;
		DWORD color;
	};
private:
	LPDIRECT3DVERTEXBUFFER9 g_pVB = NULL;

public:
	HRESULT InitVB(IDirect3DDevice9 * pDevice, float left, float up, float right, float down, D3DCOLOR rgb = d3d::WHITE)
	{
		CUSTOMVERTEX vertices[] = { { left, down, 0.5, 1.0, rgb },
		{ left, up, 0.5, 1.0, rgb },
		{ right, up, 0.5, 1.0, rgb },
		{ left, down, 0.5, 1.0, rgb },
		{ right, up, 0.5, 1.0, rgb },
		{ right, down, 0.5, 1.0, rgb } };

		if (FAILED(pDevice->CreateVertexBuffer(4 * sizeof(CUSTOMVERTEX), 0, D3DFVF_CUSTOMVERTEX, D3DPOOL_DEFAULT, &g_pVB, NULL)))
			return E_FAIL;

		void *pVertices;
		if (FAILED(g_pVB->Lock(0, sizeof(vertices), (void**)&pVertices, 0)))
			return E_FAIL;
		memcpy(pVertices, vertices, sizeof(vertices));
		g_pVB->Unlock();

		return S_OK;
	}
	void Draw(IDirect3DDevice9 * pDevice)
	{
		pDevice->SetStreamSource(0, g_pVB, 0, sizeof(CUSTOMVERTEX));
		pDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
		pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);
	}
	VOID CleanUP()
	{
		if (g_pVB != NULL)
			g_pVB->Release();
	}
};


class Circle
{
	struct CUSTOMVERTEX
	{
		FLOAT X, Y, Z, RHW;
		DWORD color;
	};
private:
	LPDIRECT3DVERTEXBUFFER9 g_pVB = NULL;
	float angle;

	CUSTOMVERTEX line[3];
	D3DXVECTOR3 center;

public:
	CUSTOMVERTEX vertices[39];

	HRESULT InitVB(IDirect3DDevice9 * pDevice, float centerX, float centerZ, float Radius, D3DCOLOR rgb = d3d::WHITE)
	{
		angle = 0;


		/*{ left, down, 0.5, 1.0, rgb },
		{ left, up, 0.5, 1.0, rgb },
		{ right, up, 0.5, 1.0, rgb },
		{ left, down, 0.5, 1.0, rgb },
		{ right, up, 0.5, 1.0, rgb },
		{ right, down, 0.5, 1.0, rgb }*/
		vertices[0].X = centerX;
		vertices[0].Y = centerZ;
		vertices[0].Z = 0.2;
		vertices[0].color = rgb;
		vertices[0].RHW = 1.0;
		for (int i = 1; i < 39; i++)
		{
			vertices[i].X = centerX + Radius * cos(angle);
			vertices[i].Y = centerZ + Radius * sin(angle);
			vertices[i].Z = 0.2;
			vertices[i].color = rgb;
			vertices[i].RHW = 1.0;
			angle += 2 * D3DX_PI / 36;
		}

		if (FAILED(pDevice->CreateVertexBuffer(37 * sizeof(CUSTOMVERTEX), 0, D3DFVF_CUSTOMVERTEX, D3DPOOL_DEFAULT, &g_pVB, NULL)))
			return E_FAIL;

		void *pVertices;
		if (FAILED(g_pVB->Lock(0, sizeof(vertices), (void**)&pVertices, 0)))
			return E_FAIL;
		memcpy(pVertices, vertices, sizeof(vertices));
		g_pVB->Unlock();

		return S_OK;
	}
	void Draw(IDirect3DDevice9 * pDevice)
	{
		for (int i = 1; i < 37; i++)
		{
			line[0] = vertices[i];
			line[1] = vertices[i + 1];
			line[2] = vertices[0];
			pDevice->SetStreamSource(0, g_pVB, 0, sizeof(CUSTOMVERTEX));
			pDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
			//pDevice->DrawPrimitiveUP(D3DPT_LINELIST, 1, line, sizeof(CUSTOMVERTEX));
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, line, sizeof(CUSTOMVERTEX));
		}

		line[0] = vertices[1];
		//line[0].color = d3d::RED;
		line[0] = vertices[19];
		//line[1].color = d3d::RED;
		pDevice->SetStreamSource(0, g_pVB, 0, sizeof(CUSTOMVERTEX));
		pDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
		//pDevice->DrawPrimitiveUP(D3DPT_LINELIST, 1, line, sizeof(CUSTOMVERTEX));
		pDevice->DrawPrimitiveUP(D3DPT_LINELIST, 1, line, sizeof(CUSTOMVERTEX));

	}

	D3DXVECTOR3 getCenter()
	{

	}
	VOID CleanUP()
	{
		if (g_pVB != NULL)
			g_pVB->Release();
	}
};
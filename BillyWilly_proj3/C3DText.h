#include "d3dUtility.h"

class C3DText
{
private:
	D3DXMATRIX              m_mLocal;
	D3DMATERIAL9            m_mtrl;
	ID3DXMesh*              m_TextMesh;
	HDC hdc;
	HFONT hFont;
	HFONT hFontOld;
	float m_x, m_z;
public:
	C3DText(void)
	{
		D3DXMatrixIdentity(&m_mLocal);
		ZeroMemory(&m_mtrl, sizeof(m_mtrl));
		m_TextMesh = NULL;
	}
	~C3DText(void) {}

	bool create(IDirect3DDevice9 * pDevice, int height, int width, int weight, LPCSTR Face)
	{
		if (NULL == pDevice)
			return false;

		hdc = CreateCompatibleDC(0);

		m_mtrl.Ambient = d3d::BLUE;
		m_mtrl.Diffuse = d3d::WHITE;
		m_mtrl.Specular = d3d::WHITE;
		m_mtrl.Emissive = d3d::BLACK;
		m_mtrl.Power = 1.0f;

		LOGFONT lf;
		ZeroMemory(&lf, sizeof(D3DXFONT_DESC));

		lf.lfHeight = height;
		lf.lfWidth = width;
		lf.lfWeight = weight;
		//lf.MipLevels = D3DX_DEFAULT;
		lf.lfItalic = true;
		lf.lfCharSet = DEFAULT_CHARSET;
		lf.lfUnderline = false;
		lf.lfStrikeOut = false;
		//lf.OutputPrecision = OUT_DEFAULT_PRECIS;
		//lf.Quality = DEFAULT_QUALITY;
		//lf.PitchAndFamily = DEFAULT_PITCH;
		lf.lfFaceName, TEXT(Face);

		hFont = CreateFontIndirect(&lf);
		hFontOld = (HFONT)SelectObject(hdc, hFont);
	}
	void Set(IDirect3DDevice9 * pDevice, LPCSTR cSTR)
	{
		D3DXCreateText(pDevice, hdc, cSTR, 0.001f, 0.4f, &m_TextMesh, 0, 0);

		SelectObject(hdc, hFontOld);
		DeleteObject(hFont);
		DeleteDC(hdc);

	}
	void destroy(void)
	{
		if (m_TextMesh != NULL)
		{
			m_TextMesh->Release();
			m_TextMesh = NULL;
		}
	}
	void draw(IDirect3DDevice9 * pDevice, const D3DXMATRIX & mWorld)
	{
		if (NULL == pDevice)
			return;

		pDevice->SetTransform(D3DTS_WORLD, &mWorld);
		pDevice->MultiplyTransform(D3DTS_WORLD, &m_mLocal);
		pDevice->SetMaterial(&m_mtrl);
		m_TextMesh->DrawSubset(0);
	}
	void setPosition(float x, float y, float z)
	{
		D3DXMATRIX m;
		this->m_x = x;
		this->m_z = z;

		D3DXMatrixTranslation(&m, x, y, z);
		setLocalTransform(m);
	}
	void setLocalTransform(const D3DXMATRIX& mLocal) { m_mLocal = mLocal; }

};
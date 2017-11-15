#include "d3dUtility.h"
#include <vector>
#include <ctime>
#include <cstdlib>
#include <cstdio>
#include "Define.h"
#include <cassert>
#include <cstring>

class CText
{
private:
	CText * TextInst;
	ID3DXFont * m_pFont;
	int m_nMax_x;
	int m_nMax_y;


public:
	CText(void)
	{
	}
	~CText(void)
	{
	}
	void Init(IDirect3DDevice9 * pDevice, int height, int width, int weight, LPCSTR Face)
	{
		D3DXFONT_DESC DXFont_DESC;
		ZeroMemory(&DXFont_DESC, sizeof(D3DXFONT_DESC));

		DXFont_DESC.Height = height;                            // 전체 글자 높이
		DXFont_DESC.Width = width;                             // 전체 글자 넓이
		DXFont_DESC.Weight = FW_BOLD;                     // 긁자 굵기 ( FW_BOLD 하면 굵음 )
		DXFont_DESC.MipLevels = D3DX_DEFAULT;
		DXFont_DESC.Italic = false;                         // 이텔릭
		DXFont_DESC.CharSet = DEFAULT_CHARSET;
		DXFont_DESC.OutputPrecision = OUT_DEFAULT_PRECIS;
		DXFont_DESC.Quality = DEFAULT_QUALITY;
		DXFont_DESC.PitchAndFamily = DEFAULT_PITCH;
		DXFont_DESC.FaceName, TEXT("돋움체");              // 글씨체
		m_nMax_x = 1920;                               // 윈도우 최대 X
		m_nMax_y = 1080;                                // 윈도우 최대 Y
		D3DXCreateFontIndirect(pDevice, &DXFont_DESC, &m_pFont);
	}
	void Print(LPCSTR cSTR, int nX/*문자열의 왼쪽좌표*/ = 0, int nY/*문자열의 윗쪽좌표*/ = 0, D3DXCOLOR rgb = D3DCOLOR_XRGB(0, 0, 0))
	{
		RECT rt = { nX, nY, m_nMax_x, m_nMax_y };

		m_pFont->DrawTextA(0, cSTR, -1, &rt, DT_TOP | DT_LEFT, rgb);
	}
	CText * GetInst(void)
	{
		if (!TextInst)
		{
			TextInst = new CText;
		}
		return TextInst;
	}
	void FreeInst(void)
	{
		if (TextInst)
		{
			m_pFont->Release();
			delete TextInst;
			TextInst = NULL;
		}
	}

};
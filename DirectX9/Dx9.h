#ifndef DX9_H
#define DX9_H

#include <Windows.h>
#include <d3dx9.h>
#include <dinput.h>
#include <D3dx9core.h>
#include <unordered_map>

#include <tchar.h>

namespace DirectX9Wrapper{

#ifdef _UNICODE
#define tstring std::wstring
#else
#define tstring std::string
#endif
struct Dx9
{
public:

	LPDIRECT3D9 pDirect3D;		//	Direct3D�̃C���^�[�t�F�C�X
	IDirect3DDevice9* pD3Device;		//	Direct3D�̃f�o�C�X
	D3DPRESENT_PARAMETERS D3dPresentParameters;

	LPDIRECTINPUTDEVICE8 pDxIKeyDevice;//DirectInput�̃L�[�{�[�h�f�o�C�X
	LPDIRECTINPUT8 pDinput;

	std::unordered_map<tstring, LPDIRECT3DTEXTURE9>	  pTexture;	//	�摜�̏������Ă����ׂ̃|�C���^�z��
	std::unordered_map<tstring, LPD3DXFONT> pFont;	//Font

	static 	const int MAX_KEY_NUMBER = 256;
	const int MASK_NUM = 0x80;
	BYTE KeyState[MAX_KEY_NUMBER];

	const int D3DFVF_CUSTOMVERTEX = (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);

	Dx9();
	~Dx9();

	HRESULT BuildDXDevice(HWND hWnd, const TCHAR* FilePath);
	HRESULT InitializeDx3Device(HWND hWnd, const TCHAR* FilePath);
	void InitPresentParameters(HWND hWnd);
	HRESULT InitDinput(HWND hWnd);

	void UpdateKeyStatus();
	bool GetKeyState(int keyNumber);

	void Release();
	void ClearTexture();
	void ClearFont();
};
#endif
}

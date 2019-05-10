#include "Dx9.h"


#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dinput8.lib")

namespace DirectX9Wrapper {

	using std::string;

Dx9::Dx9()
{
}


Dx9::~Dx9()
{
	Release();
}

HRESULT Dx9::BuildDXDevice(HWND hWnd, const TCHAR* filePath) {
	//ダイレクト３Dの初期化関数を呼ぶ
	if (FAILED(InitializeDx3Device(hWnd, filePath)))
	{
		return E_FAIL;
	}
	//ダイレクトインプットの初期化関数を呼ぶ
	if (FAILED(InitDinput(hWnd)))
	{
		MessageBox(0, _T("DirectInputDeviceの作成に失敗しました"), _T(""), MB_OK);
		return E_FAIL;
	}

	//DirectX オブジェクトの生成
	pDirect3D = Direct3DCreate9(
		D3D_SDK_VERSION);
	//成功チェック
	if (pDirect3D == NULL)
	{
		//生成に失敗したら終了する
		MessageBox(0, _T("Direct3Dの作成に失敗しました"), _T(""), MB_OK);
		return E_FAIL;
	}


	//アルファブレンドの設定
	pD3Device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);//透過処理を使用するか設定
	pD3Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);  //SRCブレンドの設定
	pD3Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);//DESTブレンドの設定

	pD3Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);//カリングの設定
	//テクスチャステージの設定
	pD3Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	pD3Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	pD3Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	pD3Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	//頂点に入れるデータを設定
	pD3Device->SetFVF(D3DFVF_CUSTOMVERTEX);
	return S_OK;
}


HRESULT Dx9::InitializeDx3Device(HWND hWnd,const TCHAR* filePath)
{
	// 「Direct3D」オブジェクトの作成
	if (NULL == (pDirect3D = Direct3DCreate9(D3D_SDK_VERSION)))
	{
		MessageBox(0, _T("Direct3Dの作成に失敗しました"), _T(""), MB_OK);
		return E_FAIL;
	}
	// 「DIRECT3Dデバイス」オブジェクトの作成
	InitPresentParameters(hWnd);

	if (FAILED(pDirect3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_MIXED_VERTEXPROCESSING,
		&D3dPresentParameters, &pD3Device)))
	{
		MessageBox(0, _T("HALモードでDIRECT3Dデバイスを作成できません\nREFモードで再試行します"), NULL, MB_OK);
		if (FAILED(pDirect3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd,
			D3DCREATE_MIXED_VERTEXPROCESSING,
			&D3dPresentParameters, &pD3Device)))
		{
			MessageBox(0, _T("DIRECT3Dデバイスの作成に失敗しました"), NULL, MB_OK);
			return E_FAIL;
		}
	}
	//「テクスチャオブジェクト」の作成
	if (FAILED(D3DXCreateTextureFromFileEx(pD3Device, filePath, 100, 100, 0, 0, D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_DEFAULT,
		0xff000000, NULL, NULL, &pTexture[_T("Test")])))
	{
		MessageBox(0, _T("テクスチャの作成に失敗しました"), _T(""), MB_OK);
		return E_FAIL;
	}
	ClearTexture();

	return S_OK;
}

void Dx9::InitPresentParameters(HWND hWnd) {
	//WindowMode
	ZeroMemory(&D3dPresentParameters, sizeof(D3DPRESENT_PARAMETERS));
	D3dPresentParameters.BackBufferWidth = 0;
	D3dPresentParameters.BackBufferHeight = 0;
	D3dPresentParameters.BackBufferFormat = D3DFMT_UNKNOWN;
	D3dPresentParameters.BackBufferCount = 1;
	D3dPresentParameters.MultiSampleType = D3DMULTISAMPLE_NONE;
	D3dPresentParameters.MultiSampleQuality = 0;
	D3dPresentParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
	D3dPresentParameters.hDeviceWindow = hWnd;
	D3dPresentParameters.Windowed = TRUE;
	D3dPresentParameters.EnableAutoDepthStencil = FALSE;
	D3dPresentParameters.AutoDepthStencilFormat = D3DFMT_A1R5G5B5;
	D3dPresentParameters.Flags = 0;
	D3dPresentParameters.FullScreen_RefreshRateInHz = 0;
	D3dPresentParameters.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
}

HRESULT Dx9::InitDinput(HWND hWnd) {
	HRESULT hr;
	// 「DirectInput」オブジェクトの作成
	if (FAILED(hr = DirectInput8Create(GetModuleHandle(NULL),
		DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID * *)& pDinput, NULL)))
	{
		return hr;
	}
	// 「DirectInputデバイス」オブジェクトの作成
	if (FAILED(hr = pDinput->CreateDevice(GUID_SysKeyboard,
		&pDxIKeyDevice, NULL)))
	{
		return hr;
	}
	// デバイスをキーボードに設定
	if (FAILED(hr = pDxIKeyDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		return hr;
	}
	// 協調レベルの設定
	if (FAILED(hr = pDxIKeyDevice->SetCooperativeLevel(
		hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND)))
	{
		return hr;
	}
	// デバイスを「取得」する
	pDxIKeyDevice->Acquire();
	return S_OK;
}

void Dx9::UpdateKeyStatus() {
	//キーボードの押下状態を保存
	HRESULT hr = pDxIKeyDevice->Acquire();
	if ((hr == DI_OK) || (hr == S_FALSE)) {
		pDxIKeyDevice->GetDeviceState(sizeof(KeyState), &KeyState);
	}
}

bool Dx9::GetKeyState(int keyNumber) {
	if (KeyState[keyNumber] & 0x80)
	{
		return true;
	}
	return false;
}

void Dx9::Release()
{
	if (!pDirect3D) return;
	ClearTexture();
	ClearFont();
	if (pDxIKeyDevice)
	{
		pDxIKeyDevice->Unacquire();
	}
	pDxIKeyDevice->Release();
	pDxIKeyDevice = nullptr;
	pDinput->Release();
	pDinput = nullptr;
	pD3Device->Release();
	pD3Device = nullptr;
	pDirect3D->Release();
	pDirect3D = nullptr;
}

void Dx9::ClearTexture() {
	if (!pTexture.size()) {
		return;
	}
	for (auto i : pTexture) {
		if (!i.second) continue;
		(i.second)->Release();
	}
	pTexture.clear();
	std::unordered_map<tstring, LPDIRECT3DTEXTURE9>().swap(pTexture);
}
void Dx9::ClearFont() {
	if (!pFont.size()) {
		return;
	}
	for (auto i : pFont) {
		if (!i.second) continue;
		(i.second)->Release();
	}
	pFont.clear();
	std::unordered_map<tstring, LPD3DXFONT>().swap(pFont);
}

}

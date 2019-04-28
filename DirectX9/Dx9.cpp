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
}

HRESULT Dx9::BuildDXDevice(HWND hWnd, const TCHAR* FilePath) {
	//�_�C���N�g�RD�̏������֐����Ă�
	if (FAILED(InitializeDx3Device(hWnd, FilePath)))
	{
		return E_FAIL;
	}
	//�_�C���N�g�C���v�b�g�̏������֐����Ă�
	if (FAILED(InitDinput(hWnd)))
	{
		MessageBox(0, _T("DirectInputDevice�̍쐬�Ɏ��s���܂���"), _T(""), MB_OK);
		return E_FAIL;
	}

	//DirectX �I�u�W�F�N�g�̐���
	pDirect3D = Direct3DCreate9(
		D3D_SDK_VERSION);
	//�����`�F�b�N
	if (pDirect3D == NULL)
	{
		//�����Ɏ��s������I������
		MessageBox(0, _T("Direct3D�̍쐬�Ɏ��s���܂���"), _T(""), MB_OK);
		return E_FAIL;
	}

	InitPresentParameters(hWnd);

	pDirect3D->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,
		&D3dPresentParameters, &pD3Device);

	//�����`�F�b�N
	if (pD3Device == NULL)
	{
		//�����Ɏ��s������DirectX�I�u�W�F�N�g���J�����ďI������
		pDirect3D->Release();
		MessageBox(0, _T("Direct3D�̍쐬�Ɏ��s���܂���"), _T(""), MB_OK);
		return E_FAIL;
	}
	//�`��ݒ�
	pD3Device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	pD3Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);  //SRC�̐ݒ�
	pD3Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	//pD3Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);//�J�����O�̐ݒ�

	pD3Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	pD3Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	pD3Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	pD3Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	//���_�ɓ����f�[�^��ݒ�
	pD3Device->SetFVF(D3DFVF_CUSTOMVERTEX);
	return S_OK;
}


HRESULT Dx9::InitializeDx3Device(HWND hWnd,const TCHAR* FilePath)
{
	// �uDirect3D�v�I�u�W�F�N�g�̍쐬
	if (NULL == (pDirect3D = Direct3DCreate9(D3D_SDK_VERSION)))
	{
		MessageBox(0, _T("Direct3D�̍쐬�Ɏ��s���܂���"), _T(""), MB_OK);
		return E_FAIL;
	}
	// �uDIRECT3D�f�o�C�X�v�I�u�W�F�N�g�̍쐬
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));

	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.BackBufferCount = 1;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.Windowed = TRUE;

	if (FAILED(pDirect3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_MIXED_VERTEXPROCESSING,
		&d3dpp, &pD3Device)))
	{
		MessageBox(0, _T("HAL���[�h��DIRECT3D�f�o�C�X���쐬�ł��܂���\nREF���[�h�ōĎ��s���܂�"), NULL, MB_OK);
		if (FAILED(pDirect3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd,
			D3DCREATE_MIXED_VERTEXPROCESSING,
			&d3dpp, &pD3Device)))
		{
			MessageBox(0, _T("DIRECT3D�f�o�C�X�̍쐬�Ɏ��s���܂���"), NULL, MB_OK);
			return E_FAIL;
		}
	}
	//�u�e�N�X�`���I�u�W�F�N�g�v�̍쐬
	if (FAILED(D3DXCreateTextureFromFileEx(pD3Device, FilePath, 100, 100, 0, 0, D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_DEFAULT,
		0xff000000, NULL, NULL, &pTexture[_T("Test")])))
	{
		MessageBox(0, _T("�e�N�X�`���̍쐬�Ɏ��s���܂���"), _T(""), MB_OK);
		return E_FAIL;
	}
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
	// �uDirectInput�v�I�u�W�F�N�g�̍쐬
	if (FAILED(hr = DirectInput8Create(GetModuleHandle(NULL),
		DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID * *)& pDinput, NULL)))
	{
		return hr;
	}
	// �uDirectInput�f�o�C�X�v�I�u�W�F�N�g�̍쐬
	if (FAILED(hr = pDinput->CreateDevice(GUID_SysKeyboard,
		&pDxIKeyDevice, NULL)))
	{
		return hr;
	}
	// �f�o�C�X���L�[�{�[�h�ɐݒ�
	if (FAILED(hr = pDxIKeyDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		return hr;
	}
	// �������x���̐ݒ�
	if (FAILED(hr = pDxIKeyDevice->SetCooperativeLevel(
		hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND)))
	{
		return hr;
	}
	// �f�o�C�X���u�擾�v����
	pDxIKeyDevice->Acquire();
	return S_OK;
}

void Dx9::UpdateKeyStatus() {
	//�L�[�{�[�h�̉�����Ԃ�ۑ�
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
	ClearTexture();
	ClearFont();
	if (pDxIKeyDevice)
	{
		pDxIKeyDevice->Unacquire();
	}
	pDxIKeyDevice->Release();
	pDinput->Release();
	pD3Device->Release();
	pDirect3D->Release();
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

#include "../DirectX9/Dx9.h"

#include <ctype.h>
#include <cstdio>
#include <cstdlib>

#include "Graphic.h"

#pragma comment(lib, "../x64/Debug/DirectX9.lib")

#define _CRTDBG_MAP_ALLOC
#define new ::new(_NORMAL_BLOCK, __FILE__, __LINE__)

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
void SettingWindow(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow);
int MessageLoop();

CUSTOMVERTEX customVertex[4]{
	{10, 10, 0,1,0xFFFFFFFF,0,0},
	{200,10, 0,1,0xFF00FFFF,0,0},
	{200,200,0,1,0xFFFF00FF,0,0},
	{10, 200,0,1,0xFFFFFF00,0,0}
};

using DirectX9Wrapper::Dx9;

const TCHAR API_NAME[10] = _T("Sample");

static HWND hWnd;
Dx9 dx9;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	SettingWindow(hInstance,hPrevInstance,lpCmdLine,nCmdShow);
	dx9.BuildDXDevice(hWnd,_T("Blank.jpg"));

	D3DXCreateTextureFromFile(
		dx9.pD3Device,
		_T("Blank.jpg"),
		&dx9.pTexture[_T("test")]);

	MessageLoop();

	dx9.Release();
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
		//! Alt + 特殊キーの処理に使う
	case WM_SYSKEYDOWN:
		switch (wp) {
			//! Alt + Enterを押すと切り替え
		case VK_RETURN:
			return 0;
			//! Alt + F4を押すと実行終了
		case VK_F4:
			PostMessage(hWnd, WM_CLOSE, 0, 0);
			return 0;
		default:
			return 0;
		}
		return 0;
	}
	return DefWindowProc(hWnd, msg, wp, lp);
}


void SettingWindow(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow)
{
	WNDCLASS Wndclass;

	//Windows初期化情報の設定
	Wndclass.style = CS_HREDRAW | CS_VREDRAW;
	Wndclass.lpfnWndProc = WndProc;
	Wndclass.cbClsExtra = Wndclass.cbWndExtra = 0;
	Wndclass.hInstance = hInstance;
	Wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	Wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	Wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	Wndclass.lpszMenuName = NULL;
	Wndclass.lpszClassName = API_NAME;	//クラス名
	//Windowの登録
	RegisterClass(&Wndclass);
	//Windowの生成
	hWnd = CreateWindow(
		API_NAME,					//ウィンドウのクラス名
		API_NAME, 					//ウィンドウのタイトル
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,	//ウィンドウスタイル
		CW_USEDEFAULT,						// ウィンドウの横方向の位置x
		CW_USEDEFAULT,						// ウィンドウの縦方向の位置y
		600,						// Width（幅）
		400,						// Height（高さ）
		NULL,
		NULL,
		hInstance,							// アプリケーションインスタンスのハンドル
		NULL
	);
	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);
}

int MessageLoop()
{

	MSG msg;
	DWORD SyncPrev = timeGetTime();	//	システム時間を取得
	DWORD SyncCurr;
	timeBeginPeriod(1);
	ZeroMemory(&msg, sizeof(msg));
	while (msg.message != WM_QUIT)
	{
		Sleep(1);
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			SyncCurr = timeGetTime();
			if (SyncCurr - SyncPrev >= 1000 / 60)//1秒間に60回この中に入る
			{
				//ウィンドウを黒色でクリア
				dx9.pD3Device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0x00, 0x00, 0x00), 1.0, 0);
				//テクスチャ貼り付け開始
				dx9.pD3Device->BeginScene();
				
				//エンター押下でループを抜ける
				dx9.UpdateKeyStatus();
				if (dx9.GetKeyState(DIK_RETURN)) {
					break;
				}

				//テクスチャの貼り付け
				dx9.pD3Device->SetTexture(0, dx9.pTexture[_T("test")]);
				dx9.pD3Device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, customVertex, sizeof(CUSTOMVERTEX));

				//テクスチャの貼り付け終了
				dx9.pD3Device->EndScene();
				//ウィンドウに表示
				dx9.pD3Device->Present(0, 0, 0, 0);

				SyncPrev = SyncCurr;
			}
		}
	}
	timeEndPeriod(1);
	return (int)msg.wParam;
}


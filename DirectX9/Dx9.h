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
	//	Direct3Dのインターフェイス
	LPDIRECT3D9 pDirect3D;
	//	Direct3Dのデバイス
	IDirect3DDevice9* pD3Device;
	//	ウィンドウ設定
	D3DPRESENT_PARAMETERS D3dPresentParameters;
	//	DirectInputのキーボードデバイス
	LPDIRECTINPUTDEVICE8 pDxIKeyDevice;
	//DirectInputのインターフェイス
	LPDIRECTINPUT8 pDinput;

	//	画像の情報を入れておく為のポインタ配列
	std::unordered_map<tstring, LPDIRECT3DTEXTURE9>	  pTexture;
	//フォントの情報を入れておく為のポインタ配列
	std::unordered_map<tstring, LPD3DXFONT> pFont;

	static 	const int MAX_KEY_NUMBER = 256;
	//	キーの押下判定用のマスク値
	const int MASK_NUM = 0x80;
	//	キーの入力状態の保存先
	BYTE KeyState[MAX_KEY_NUMBER];

	//	頂点の情報数定義
	const int D3DFVF_CUSTOMVERTEX = (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);

	Dx9();
	~Dx9();

	/**
	* @brief DirectXグラフィック及びインプットデバイスの生成を行う
	* @param hwnd 描画先のウィンドウハンドル
	* @param filePath テクスチャ生成テスト用の画像パス
	*/
	HRESULT BuildDXDevice(HWND hWnd, const TCHAR* filePath);

	/**
	* @brief DirectXのグラフィックデバイスの生成初期化
	* @param hwnd 描画先のウィンドウハンドル
	* @param filePath テクスチャ生成テスト用の画像パス
	*/
	HRESULT InitializeDx3Device(HWND hWnd, const TCHAR* filePath);

	/**
	* @brief ウィンドウの設定項目をウィンドウモードで初期化する
	*/
	void InitPresentParameters(HWND hWnd);

	/**
	* @brief DirextInputのキーボードデバイスの生成初期化
	*/
	HRESULT InitDinput(HWND hWnd);

	/**
	* @brief キーボードの状態更新
	*/
	void UpdateKeyStatus();

	/**
	* @brief 指定のキーの押下状態の取得
	* @param keyNumber キーボード番号
	*/
	bool GetKeyState(int keyNumber);

	/**
	* @brief メモリ解放
	* @details 一度呼んだら再生成するまで呼ばないこと
	*/
	void Release();

	/**
	* @brief テクスチャの解放
	*/
	void ClearTexture();

	/**
	* @brief フォント解放
	*/
	void ClearFont();
};
#endif
}

#include "DirectGraphics.h"
#include <d3dx9.h>

#pragma comment(lib, "d3dx9.lib")

// グローバル変数
static LPDIRECT3D9 g_pD3DInterface;		// DirectGraphicsインターフェース
static LPDIRECT3DDEVICE9 g_pD3DDevice;	// DirectGraphicsデバイス

bool InitDirectGraphics(HWND window_handle)
{
	D3DPRESENT_PARAMETERS present_param;

	// インターフェース作成
	g_pD3DInterface = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3DInterface == NULL)
	{
		// 作成失敗
		return false;
	}

	ZeroMemory(&present_param, sizeof(D3DPRESENT_PARAMETERS));

	// バックバッファの数 => 1
	present_param.BackBufferCount = 1;
	// バックバッファのフォーマット => D3DFMT_UNKNOWN(フォーマットを知りません)
	present_param.BackBufferFormat = D3DFMT_UNKNOWN;
	// ウィンドウモード設定 => 定数で切り替え(true:ウィンドウ false:フルスクリーン)
	present_param.Windowed = true;

	// スワップエフェクト => バックバッファとフロントバッファへの切り替え方法
	present_param.SwapEffect = D3DSWAPEFFECT_DISCARD;

	// DirectGraphicsDeviceの作成
	if (FAILED(g_pD3DInterface->CreateDevice(
		D3DADAPTER_DEFAULT,						//ディスプレイアダプタの種類
		D3DDEVTYPE_HAL,							//デバイスの種類（HALデバイス（ハードウェアごとの違いを吸収してくれるもの）を活用）
		window_handle,							//ウィンドウのハンドル
		D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,	//デバイス制御の組み合わせ基本的にはこれを使う
		&present_param,							//デバイスを設定するための構造体
		&g_pD3DDevice							//設定されたデバイス内容を保存するアドレス
	)))
	{
		return false;
	}

	// ビューポートパラメータ（描画範囲）
	D3DVIEWPORT9 view_port;

	// ビューポートの左上座標
	view_port.X = 0;
	view_port.Y = 0;
	// ビューポートの幅
	view_port.Width = present_param.BackBufferWidth;
	// ビューポートの高さ
	view_port.Height = present_param.BackBufferHeight;
	// ビューポート深度設定
	view_port.MinZ = 0.0f;
	view_port.MaxZ = 1.0f;

	// ビューポート設定
	if (FAILED(g_pD3DDevice->SetViewport(&view_port)))
	{
		return false;
	}

	return true;
}

void ReleaseDirectGraphics()
{
	g_pD3DDevice->Release();
	g_pD3DInterface->Release();
}

bool StartDraw()
{
	//シーンクリア
	g_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 0.0f, 0);

	//シーン描画開始
	if (D3D_OK == g_pD3DDevice->BeginScene())
	{
		return true;
	}

	return false;
}

void EndDraw()
{
	//シーン描画終了
	g_pD3DDevice->EndScene();
	//バッファ転送
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

#include <windows.h>
#include <d3dx9.h>
#include "DirectGraphics.h"

//ウィンドウプロシージャ作成
LRESULT CALLBACK WindowProcedure(HWND window_handle, UINT message_id, WPARAM wparam, LPARAM lparam)
{
	switch (message_id)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	default:
		// メッセージの対応をしない
		return DefWindowProc(window_handle, message_id, wparam, lparam);
		break;
	}

	// メッセージの対応をした
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmpLine,INT nCmdShow)
{
	int width = 400;
	int height = 400;

	//構造体の登録
	HWND hWnd;
	WNDCLASSEX window_class = {
		sizeof(WNDCLASSEX),				// 構造体のサイズ
		CS_HREDRAW | CS_VREDRAW,		// クラスのスタイル
		WindowProcedure,				// ウィンドウプロシージャ
		0,								// 補助メモリ
		0,								// 補助メモリ
		hInstance,						// このアプリのインスタンスハンドル
		LoadIcon(NULL, IDI_APPLICATION),// アイコン画像
		LoadCursor(NULL, IDC_ARROW),	// カーソル画像
		NULL,							// 背景ブラシ(背景色)
		NULL,							// メニュー名
		TEXT("CreateWindow"),			// クラス名									
		NULL							// 小さいアイコン
	};

	if (RegisterClassEx(&window_class) == 0)
	{
		return 0;
	}

	//ウィンドウ作成
	hWnd = CreateWindow( TEXT("CreateWindow"), TEXT("DirectXEayTracing"),
		(WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME), CW_USEDEFAULT, 0,
		width, height, NULL, NULL, hInstance, NULL);

	if (hWnd == NULL)
	{
		return 0;
	}

	//ウィンドウ表示
	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

	// DirectGraphicsの初期化
	InitDirectGraphics(hWnd);


	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	//メインループ
	while (msg.message != WM_QUIT)
	{
		//メッセージ受信
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			//メッセージ変換
			TranslateMessage(&msg);

			//ウィンドウプロシージャにメッセージ送信
			DispatchMessage(&msg);

			continue;
		}

		// 描画開始
		if (StartDraw() == true)
		{
			// 描画終了
			EndDraw();
		}
	}

	// DirectGraphicsの解放
	ReleaseDirectGraphics();

	return (int)msg.wParam;
}

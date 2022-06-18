#include <windows.h>
#include <d3dx9.h>

#pragma comment(lib, "d3dxq.lib")

LPDIRECT3D9 pD3d;
LPDIRECT3DDEVICE9 pDevice;

//関数のプロトタイプ宣言
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//ダイレクト3Dの初期化
HRESULT InitD3d(HWND hWnd)
{
	//DirectX3Dオブジェクトの作成
	pD3d = Direct3DCreate9(D3D_SDK_VERSION);
	if (pD3d == NULL)
	{
		MessageBox(0, "Direct3Dの作成に失敗しました", "", MB_OK);
		return E_FAIL;
	}

	//Direct3Dデバイスオブジェクトの作成
	D3DPRESENT_PARAMETERS d3pp;
	ZeroMemory(&d3pp, sizeof(d3pp));

	//バックバッファの設定
	d3pp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3pp.BackBufferCount = 1;
	//バックバッファとフロントのスワップ形式
	d3pp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	//ウィンドウのモード（true:ウィンドウ false:フルスクリーン）
	d3pp.Windowed = TRUE;

	//DirectDeviceの作成
	if (!pD3d->CreateDevice(
		D3DADAPTER_DEFAULT,		//ディスプレイアダプタの種類
		D3DDEVTYPE_HAL,			//デバイスの種類（HALデバイス（ハードウェアごとの違いを吸収してくれるもの）を活用）
		hWnd,					//ウィンドウのハンドル
		D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,	//デバイス制御の組み合わせ基本的にはこれを使う
		&d3pp,					//デバイスを設定するための構造体
		&pDevice				//設定されたデバイス内容を保存するアドレス
	))
	{
		return false;
	}
}



VOID DrawSprite(VOID);

// ①．ウィンドウプロシージャ作成
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

	//ダイレクト3Dの初期化
	if (!InitD3d(hWnd))
	{
		return 0;
	}

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

		DrawSprite();
	}

	return msg.wParam;
}

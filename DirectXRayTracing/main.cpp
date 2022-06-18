#include <windows.h>
#include <d3dx9.h>

#pragma comment(lib, "d3dxq.lib")

LPDIRECT3D9 pD3d;
LPDIRECT3DDEVICE9 pDevice;

//�֐��̃v���g�^�C�v�錾
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//�_�C���N�g3D�̏�����
HRESULT InitD3d(HWND hWnd)
{
	//DirectX3D�I�u�W�F�N�g�̍쐬
	pD3d = Direct3DCreate9(D3D_SDK_VERSION);
	if (pD3d == NULL)
	{
		MessageBox(0, "Direct3D�̍쐬�Ɏ��s���܂���", "", MB_OK);
		return E_FAIL;
	}

	//Direct3D�f�o�C�X�I�u�W�F�N�g�̍쐬
	D3DPRESENT_PARAMETERS d3pp;
	ZeroMemory(&d3pp, sizeof(d3pp));

	//�o�b�N�o�b�t�@�̐ݒ�
	d3pp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3pp.BackBufferCount = 1;
	//�o�b�N�o�b�t�@�ƃt�����g�̃X���b�v�`��
	d3pp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	//�E�B���h�E�̃��[�h�itrue:�E�B���h�E false:�t���X�N���[���j
	d3pp.Windowed = TRUE;

	//DirectDevice�̍쐬
	if (!pD3d->CreateDevice(
		D3DADAPTER_DEFAULT,		//�f�B�X�v���C�A�_�v�^�̎��
		D3DDEVTYPE_HAL,			//�f�o�C�X�̎�ށiHAL�f�o�C�X�i�n�[�h�E�F�A���Ƃ̈Ⴂ���z�����Ă������́j�����p�j
		hWnd,					//�E�B���h�E�̃n���h��
		D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,	//�f�o�C�X����̑g�ݍ��킹��{�I�ɂ͂�����g��
		&d3pp,					//�f�o�C�X��ݒ肷�邽�߂̍\����
		&pDevice				//�ݒ肳�ꂽ�f�o�C�X���e��ۑ�����A�h���X
	))
	{
		return false;
	}
}



VOID DrawSprite(VOID);

// �@�D�E�B���h�E�v���V�[�W���쐬
LRESULT CALLBACK WindowProcedure(HWND window_handle, UINT message_id, WPARAM wparam, LPARAM lparam)
{
	switch (message_id)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	default:
		// ���b�Z�[�W�̑Ή������Ȃ�
		return DefWindowProc(window_handle, message_id, wparam, lparam);
		break;
	}

	// ���b�Z�[�W�̑Ή�������
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmpLine,INT nCmdShow)
{
	int width = 400;
	int height = 400;

	//�\���̂̓o�^
	HWND hWnd;
	WNDCLASSEX window_class = {
		sizeof(WNDCLASSEX),				// �\���̂̃T�C�Y
		CS_HREDRAW | CS_VREDRAW,		// �N���X�̃X�^�C��
		WindowProcedure,				// �E�B���h�E�v���V�[�W��
		0,								// �⏕������
		0,								// �⏕������
		hInstance,						// ���̃A�v���̃C���X�^���X�n���h��
		LoadIcon(NULL, IDI_APPLICATION),// �A�C�R���摜
		LoadCursor(NULL, IDC_ARROW),	// �J�[�\���摜
		NULL,							// �w�i�u���V(�w�i�F)
		NULL,							// ���j���[��
		TEXT("CreateWindow"),			// �N���X��									
		NULL							// �������A�C�R��
	};

	if (RegisterClassEx(&window_class) == 0)
	{
		return 0;
	}

	//�E�B���h�E�쐬
	hWnd = CreateWindow( TEXT("CreateWindow"), TEXT("DirectXEayTracing"),
		(WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME), CW_USEDEFAULT, 0,
		width, height, NULL, NULL, hInstance, NULL);

	if (hWnd == NULL)
	{
		return 0;
	}

	//�E�B���h�E�\��
	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

	//�_�C���N�g3D�̏�����
	if (!InitD3d(hWnd))
	{
		return 0;
	}

	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	//���C�����[�v
	while (msg.message != WM_QUIT)
	{
		//���b�Z�[�W��M
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			//���b�Z�[�W�ϊ�
			TranslateMessage(&msg);

			//�E�B���h�E�v���V�[�W���Ƀ��b�Z�[�W���M
			DispatchMessage(&msg);

			continue;
		}

		DrawSprite();
	}

	return msg.wParam;
}

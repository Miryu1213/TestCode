#include <windows.h>
#include <d3dx9.h>
#include "DirectGraphics.h"

//�E�B���h�E�v���V�[�W���쐬
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

	// DirectGraphics�̏�����
	InitDirectGraphics(hWnd);


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

		// �`��J�n
		if (StartDraw() == true)
		{
			// �`��I��
			EndDraw();
		}
	}

	// DirectGraphics�̉��
	ReleaseDirectGraphics();

	return (int)msg.wParam;
}

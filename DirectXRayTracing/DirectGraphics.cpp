#include "DirectGraphics.h"
#include <d3dx9.h>

#pragma comment(lib, "d3dx9.lib")

// �O���[�o���ϐ�
static LPDIRECT3D9 g_pD3DInterface;		// DirectGraphics�C���^�[�t�F�[�X
static LPDIRECT3DDEVICE9 g_pD3DDevice;	// DirectGraphics�f�o�C�X

bool InitDirectGraphics(HWND window_handle)
{
	D3DPRESENT_PARAMETERS present_param;

	// �C���^�[�t�F�[�X�쐬
	g_pD3DInterface = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3DInterface == NULL)
	{
		// �쐬���s
		return false;
	}

	ZeroMemory(&present_param, sizeof(D3DPRESENT_PARAMETERS));

	// �o�b�N�o�b�t�@�̐� => 1
	present_param.BackBufferCount = 1;
	// �o�b�N�o�b�t�@�̃t�H�[�}�b�g => D3DFMT_UNKNOWN(�t�H�[�}�b�g��m��܂���)
	present_param.BackBufferFormat = D3DFMT_UNKNOWN;
	// �E�B���h�E���[�h�ݒ� => �萔�Ő؂�ւ�(true:�E�B���h�E false:�t���X�N���[��)
	present_param.Windowed = true;

	// �X���b�v�G�t�F�N�g => �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�ւ̐؂�ւ����@
	present_param.SwapEffect = D3DSWAPEFFECT_DISCARD;

	// DirectGraphicsDevice�̍쐬
	if (FAILED(g_pD3DInterface->CreateDevice(
		D3DADAPTER_DEFAULT,						//�f�B�X�v���C�A�_�v�^�̎��
		D3DDEVTYPE_HAL,							//�f�o�C�X�̎�ށiHAL�f�o�C�X�i�n�[�h�E�F�A���Ƃ̈Ⴂ���z�����Ă������́j�����p�j
		window_handle,							//�E�B���h�E�̃n���h��
		D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,	//�f�o�C�X����̑g�ݍ��킹��{�I�ɂ͂�����g��
		&present_param,							//�f�o�C�X��ݒ肷�邽�߂̍\����
		&g_pD3DDevice							//�ݒ肳�ꂽ�f�o�C�X���e��ۑ�����A�h���X
	)))
	{
		return false;
	}

	// �r���[�|�[�g�p�����[�^�i�`��͈́j
	D3DVIEWPORT9 view_port;

	// �r���[�|�[�g�̍�����W
	view_port.X = 0;
	view_port.Y = 0;
	// �r���[�|�[�g�̕�
	view_port.Width = present_param.BackBufferWidth;
	// �r���[�|�[�g�̍���
	view_port.Height = present_param.BackBufferHeight;
	// �r���[�|�[�g�[�x�ݒ�
	view_port.MinZ = 0.0f;
	view_port.MaxZ = 1.0f;

	// �r���[�|�[�g�ݒ�
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
	//�V�[���N���A
	g_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 0.0f, 0);

	//�V�[���`��J�n
	if (D3D_OK == g_pD3DDevice->BeginScene())
	{
		return true;
	}

	return false;
}

void EndDraw()
{
	//�V�[���`��I��
	g_pD3DDevice->EndScene();
	//�o�b�t�@�]��
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

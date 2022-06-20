#pragma once

#include <Windows.h>

/**
* @brief DirectGraphics�������֐�
* @retval true ����
* @retval false ���s
* @param[in] window_handle �Q�[���Ŏg�p����E�B���h�E�̃n���h��
* @details DirectGraphics�̏��������s��@n�K���Q�[�����[�v�̑O��1�x�������s���邱��
*/
bool InitDirectGraphics(HWND window_handle);

/**
* @brief DirectGraphics�I���֐�
* @details DirectGraphics���I������@n�Q�[�����[�v�𔲂�����K���P�x�������s���邱��
*/
void ReleaseDirectGraphics();

/**
* @brief �`��J�n�錾�֐�
* @retval true �錾����
* @retval false �錾���s
* @details �`��J�n��錾����@n���t���[���ň�x�������s����
*/
bool StartDraw();

/**
* @brief �`��I���錾�֐�
* @retval true �錾����
* @retval false �錾���s
* @details �`��I����錾����@n���t���[���AStartDraw�̌�Ɉ�x�������s����
*/
void EndDraw();



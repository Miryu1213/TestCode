#pragma once

#include <Windows.h>

/**
* @brief DirectGraphics初期化関数
* @retval true 成功
* @retval false 失敗
* @param[in] window_handle ゲームで使用するウィンドウのハンドル
* @details DirectGraphicsの初期化を行う@n必ずゲームループの前に1度だけ実行すること
*/
bool InitDirectGraphics(HWND window_handle);

/**
* @brief DirectGraphics終了関数
* @details DirectGraphicsを終了する@nゲームループを抜けた後必ず１度だけ実行すること
*/
void ReleaseDirectGraphics();

/**
* @brief 描画開始宣言関数
* @retval true 宣言成功
* @retval false 宣言失敗
* @details 描画開始を宣言する@n毎フレームで一度だけ実行する
*/
bool StartDraw();

/**
* @brief 描画終了宣言関数
* @retval true 宣言成功
* @retval false 宣言失敗
* @details 描画終了を宣言する@n毎フレーム、StartDrawの後に一度だけ実行する
*/
void EndDraw();



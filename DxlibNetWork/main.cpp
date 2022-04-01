#include <DxLib.h>
#include <string>

void ConnectNetWorkforDxLibHost(void)
{
    char StrBuf[256];       // データバッファ
    IPDATA Ip;              // 接続用ＩＰアドレスデータ
    int NetHandle;          // ネットワークハンドル
    int DataLength;         // 受信データ量保存用変数

    // ＩＰアドレスを設定( ここにある４つのＩＰ値は仮です )
    Ip.d1 = 192;
    Ip.d2 = 168;
    Ip.d3 = 5;
    Ip.d4 = 227;

    // 通信を確立
    NetHandle = ConnectNetWork(Ip, 9850);

    // 確立が成功した場合のみ中の処理をする
    if (NetHandle != -1)
    {
        // データ送信
        NetWorkSend(NetHandle, "繋がったか～！？", 17);

        // データがくるのを待つ
        while (!ProcessMessage())
        {
            // 取得していない受信データ量を得る
            DataLength = GetNetWorkDataLength(NetHandle);

            // 取得してない受信データ量が０じゃない場合はループを抜ける
            if (DataLength != 0) break;
        }

        // データ受信
        NetWorkRecv(NetHandle, StrBuf, DataLength);    // データをバッファに取得

        // 受信したデータを描画
        DrawString(0, 0, StrBuf, GetColor(255, 255, 255));

        // キー入力待ち
        WaitKey();

        // 接続を断つ
        CloseNetWork(NetHandle);
    }
}

void ConnectNetWorkforDxLibGuest(void)
{
    char StrBuf[256];               // データバッファ
    int NetHandle, LostHandle;      // ネットワークハンドル
    int DataLength;                 // 受信データ量保存用変数
    IPDATA Ip;                      // 接続先ＩＰアドレスデータ

    // 接続してくるのを待つ状態にする
    PreparationListenNetWork(9850);

    // 接続してくるかＥＳＣキーが押されるまでループ
    NetHandle = -1;
    while (!ProcessMessage() && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
    {
        // 新しい接続があったらそのネットワークハンドルを得る
        NetHandle = GetNewAcceptNetWork();
        if (NetHandle != -1)
        {
            break;
        }
    }

    // 接続されていたら次に進む
    if (NetHandle != -1)
    {
        // 接続の受付を終了する
        StopListenNetWork();

        // 接続してきたマシンのＩＰアドレスを得る
        GetNetWorkIP(NetHandle, &Ip);

        // データが送られて来るまで待つ
        while (!ProcessMessage())
        {
            // 取得していない受信データ量が０以外のときはループから抜ける
            if (GetNetWorkDataLength(NetHandle) != 0)
            {
                break;
            }
        }

        // データ受信
        DataLength = GetNetWorkDataLength(NetHandle);    // データの量を取得
        NetWorkRecv(NetHandle, StrBuf, DataLength);      // データをバッファに取得

        // 受信したデータを描画
        DrawString(0, 0, StrBuf, GetColor(255, 255, 255));

        // 受信成功のデータを送信
        NetWorkSend(NetHandle, "繋がったぞ～！！", 17);

        // 相手が通信を切断するまで待つ
        while (!ProcessMessage())
        {
            // 新たに切断されたネットワークハンドルを得る
            LostHandle = GetLostNetWork();

            // 切断された接続が今まで通信してた相手だった場合ループを抜ける
            if (LostHandle == NetHandle)
            {
                break;
            }
        }

        // 切断確認表示
        DrawString(0, 16, "切断しました", GetColor(255, 255, 255));

        // キー入力待ち
        WaitKey();
    }
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    ChangeWindowMode(true);
    SetGraphMode(1024, 768, 32);
    SetWindowText("DxLibNetWork");

    //ＤＸライブラリ初期化処理
    if (DxLib_Init() == -1)
    {
        return -1;
    }

    SetDrawScreen(DX_SCREEN_BACK);
    ClsDrawScreen();

    //ScreenFlip();

    while (!ProcessMessage() && !CheckHitKey(KEY_INPUT_ESCAPE))
    {
        if (CheckHitKey(KEY_INPUT_H))
        {
            ConnectNetWorkforDxLibHost();
            break;
        }
        if (CheckHitKey(KEY_INPUT_G))
        {
            ConnectNetWorkforDxLibGuest();
            break;
        }
    }

    //ＤＸライブラリ使用の終了処理
    DxLib_End();

    return 0; 
}
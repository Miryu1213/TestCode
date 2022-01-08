#include <iostream>
//#include <algorithm>
//#include <experimental/coroutine>
#include <experimental/generator>

//コンパイラオプションに/awaitの追加が必要

std::experimental::generator<int> TestCoroutine()
{
    for (int i = 0; i < 10; i++)
    {
        if (i > 5)
        {
            //コルーチン終了
            co_return;
        }
        //コルーチン一時停止
        co_yield i;
    }
    //終了時に暗黙的にco_returnは呼ばれる
}

int main()
{
    int num = 0;
    for (auto n : TestCoroutine())
    {
        printf("%d回目:%d\n",num, n);
        num++;
    }
}
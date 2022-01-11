#include <cstdio>
#include <thread>
#include <mutex>

std::mutex mtx_; // 排他制御用ミューテックス
int count_;

void AddCount()
{
	// count_を加算する前にミューテックスを取得しAddCountの所有権を持つ
	std::lock_guard<std::mutex> lock(mtx_);
	count_++;
	printf("%d\n", count_);
}

void ThreadA(int id)
{
	for (int a = 0; a < 100000;)
	{
		a++;
	}
	printf("%d:スレッドA終了\n",id);
}

void ThreadB(int id)
{
	for (int b = 0; b < 100;)
	{
		AddCount();
		b++;
	}
	printf("%d:スレッドB終了\n",id);
}

int main()
{
	count_ = 0;
	//ハードウェアによってサポートされているスレッド数
	printf("サポートスレッド数:%d\n", std::thread::hardware_concurrency());

	//スレッドの作成
	std::thread threadA(ThreadB,1);
	std::thread threadB(ThreadB,2);
	std::thread threadC(ThreadB,3);
	std::thread threadD(ThreadB,4);
	std::thread threadE(ThreadB,5);

	for (int b = 0; b < 1000000;)
	{
		b++;
	}

	printf("main\n");

	//スレッド終了待機
	//スレッド実行中にメモリ上から排除させるとエラー
	threadA.join();
	threadB.join();
	threadC.join();
	threadD.join();
	threadE.join();

	return 0;
}
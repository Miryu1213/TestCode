#include <cstdio>
#include <thread>
#include <mutex>

std::mutex mtx_; // �r������p�~���[�e�b�N�X
int count_;

void AddCount()
{
	// count_�����Z����O�Ƀ~���[�e�b�N�X���擾��AddCount�̏��L��������
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
	printf("%d:�X���b�hA�I��\n",id);
}

void ThreadB(int id)
{
	for (int b = 0; b < 100;)
	{
		AddCount();
		b++;
	}
	printf("%d:�X���b�hB�I��\n",id);
}

int main()
{
	count_ = 0;
	//�n�[�h�E�F�A�ɂ���ăT�|�[�g����Ă���X���b�h��
	printf("�T�|�[�g�X���b�h��:%d\n", std::thread::hardware_concurrency());

	//�X���b�h�̍쐬
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

	//�X���b�h�I���ҋ@
	//�X���b�h���s���Ƀ������ォ��r��������ƃG���[
	threadA.join();
	threadB.join();
	threadC.join();
	threadD.join();
	threadE.join();

	return 0;
}
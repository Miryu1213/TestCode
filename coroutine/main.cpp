#include <iostream>
//#include <algorithm>
//#include <experimental/coroutine>
#include <experimental/generator>

//�R���p�C���I�v�V������/await�̒ǉ����K�v

std::experimental::generator<int> TestCoroutine()
{
    for (int i = 0; i < 10; i++)
    {
        if (i > 5)
        {
            //�R���[�`���I��
            co_return;
        }
        //�R���[�`���ꎞ��~
        co_yield i;
    }
    //�I�����ɈÖٓI��co_return�͌Ă΂��
}

int main()
{
    int num = 0;
    for (auto n : TestCoroutine())
    {
        printf("%d���:%d\n",num, n);
        num++;
    }
}
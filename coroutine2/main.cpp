#include <iostream>
#include <experimental/coroutine>
//#include <experimental/generator>
#include <iostream>

using std::experimental::coroutine_handle;
using std::experimental::suspend_always;
using std::experimental::suspend_never;

struct generator
{
	struct promise_type
	{
		int value_;
		//getReturnObj�͕K������
		generator get_return_object()
		{
			std::cout << "get_return_object�Ăяo��" << std::endl;
			return generator(*this);
		}

		//���߂�Resume��Ԃɂ���Anever�͂����Ɏ��ɐi��
		suspend_always initial_suspend()
		{
			std::cout << "initial_suspend�Ăяo��" << std::endl;
			return {};
		}

		//�I�������Resume��ԁA�I���㎩���j�����Ȃ� noexcept�͗�O��߂��Ȃ����߂̂���
		suspend_always final_suspend() noexcept
		{
			std::cout << "final_suspend�Ăяo��" << std::endl;
			return {};
		}

		//co_yield�̂��тɌĂ΂��A�l���R�s�[���邽�߂̃��\�b�h
		suspend_always yield_value(int value)
		{
			std::cout << "yield_value�Ăяo�� �l " << value << " ���R�s�[" << std::endl;

			value_ = value;
			return {};
		}

		void return_void()
		{
			std::cout << "co_void�ŏI������Ăяo��" << std::endl;
		}

		//reeturn_void�ƕЕ��̂�
		/*int return_value(int value)
		{
			std::cout << "co_return�Ăяo��" << std::endl;
			return value;
		}*/

		void unhandled_exception()
		{
			//��O����
			std::cout << "call unhandled_exception" << std::endl;
			throw;
		}
	};

	generator() = default;
	generator(generator const&) = delete;			//copy�֎~
	generator& operator=(generator const&) = delete;

	//Move
	generator(generator&& other) noexcept : handle(other.handle)
	{
		other.handle = nullptr;
	}

	generator& operator=(generator&& other) noexcept
	{
		if (this != &other)
		{
			handle = other.handle;
			other.handle = nullptr;
		}

		return *this;
	}

	~generator()
	{
		if (handle != nullptr)//generator��Handle����������
		{
			handle.destroy();
		}
	}

	//Main����handle.resume()���ĂԂ��߂̃��\�b�h���`
	//moveNext��
	bool move_next()
	{
		std::cout << std::endl;
		std::cout << "generator move_next coroutine_handle Resume�Ăяo��" << std::endl;
		handle.resume();

		return !handle.done();
	}

	int currentValue()
	{
		return handle.promise().value_;
	}

private:
	explicit generator(promise_type& p)
		: handle(coroutine_handle<promise_type>::from_promise(p)) {}

	coroutine_handle<promise_type> handle;

};

generator iota(int end)
{
	for (int index = 0; index < end; ++index)
	{
		std::cout << "�A�v���� co_yield �Ăяo�� �l " << index << std::endl;
		//co_yield����memo�̒ʂ�ɓW�J����� �W�J������̓���iinitial_suspend()�Ȃǁj�͏�L�̒ʂ�J�X�^�}�C�Y�\
		co_yield index;
	}

	//co_return -1;
	//return_value�̂Ƃ���co_return
}

generator reiota(int start ,int end)
{
	for (int index = start; end < index; --index)
	{
		std::cout << "�A�v���� co_yield �Ăяo�� �l " << index << std::endl;
		//co_yield����memo�̒ʂ�ɓW�J����� �W�J������̓���iinitial_suspend()�Ȃǁj�͏�L�̒ʂ�J�X�^�}�C�Y�\
		co_yield index;
	}

	//co_return -1;
	//return_value�̂Ƃ���co_return
}

int main()
{

	auto g = reiota(10, 5);

	std::cout << "MoveNext���R���[�`�����s" << std::endl;


	while (g.move_next())
	{
		int value = g.currentValue();
		std::cout << "GetCurrent�ł̒l " << value << std::endl;
	}


}
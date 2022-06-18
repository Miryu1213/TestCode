#include <iostream>
#include <functional>

template<class Rtn,class ...Args>
class UI
{
public:
	UI() {};
	UI(std::function<Rtn(Args...)> function)
	{
		function_ = std::move(function);
	}
	std::function<Rtn(Args...)> function_;
protected:
	int x;
};

template<class Rtn, class ...Args>
class Box : public UI<Rtn,Args...>
{
public:
	Box(std::function<Rtn(Args...)> function)
	{
		this->function_ = std::move(function);
		this->x = 0;
		x;
	};
};

int main(void)
{
	auto func = [] {};

	auto func2 = [](int i) {
		printf("OK");
		return true; };

	
	UI<bool,int>* testUi = new Box<bool,int>(func2);
	int a = 0;
	testUi->function_(a);
	
	return 0;
}
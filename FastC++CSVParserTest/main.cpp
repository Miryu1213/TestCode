#include <iostream>
#include "csv.h"

//�ǂݍ��ݗ�
constexpr int COLUMNS = 2;

int main() {
	std::string name;
	double num;

	//�ǂݍ���
	io::CSVReader<COLUMNS> in("test.csv");
	//�w�b�_�[���̃Z�b�g(�w�b�_�͂P�s�ڂ̖��O�𐳂����L��)
	in.read_header(io::ignore_extra_column, "name", "number");

	//1�s���ǂݍ���
	while (in.read_row(name, num))
	{
		std::cout << "Name: " << name << std::endl;
		std::cout << "Number: " << num << std::endl;
		std::cout << std::endl;
	}
}

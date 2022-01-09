#include <iostream>
#include "csv.h"

//読み込み列数
constexpr int COLUMNS = 2;

int main() {
	std::string name;
	double num;

	//読み込み
	io::CSVReader<COLUMNS> in("test.csv");
	//ヘッダー情報のセット(ヘッダは１行目の名前を正しく記入)
	in.read_header(io::ignore_extra_column, "name", "number");

	//1行ずつ読み込む
	while (in.read_row(name, num))
	{
		std::cout << "Name: " << name << std::endl;
		std::cout << "Number: " << num << std::endl;
		std::cout << std::endl;
	}
}

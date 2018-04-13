#include <iostream>
#include <string>
#include <cmath>
#include <cstdio>

using namespace std;


class ShannonCode {
	struct Shannon {
		char c; // символ
		double p; // вероятность встретить этот символ (частота встречаемости
		double b; // коээфициент для алгоритма
		int L; // длинна двоичного кода
		string code; // код символа
	};

	Shannon *dictionary;
	int dictionaryLength;

	string convert(Shannon elem);
	void sortByProbability();

public:
	ShannonCode(const string &msg);
	void printTable() const;
	string codeMessage(const string &msg);
};

// перевод вещественного числа в двоичное представление с заданным числом разрядов (symbol.L)
string ShannonCode::convert(Shannon elem) {
	string result = "";

	for (int i = 0; i < elem.L; i++) {
		elem.b *= 2;  // умножаем на 2 для получения нового старшего разряда

		result += elem.b >= 1 ? "1" : "0"; // если после умножения вышла единица в значащий разряд, то прибавляем строку с 1 к результату и вычитаем единицу из числа, чтобы не портить данные

		if (elem.b >= 1)
			elem.b -= 1; // зануляем старший разряд, если он стал равен 1
	}

	return result;
}

// сортируем по возрастанию вероятности символов
void ShannonCode::sortByProbability() {
	for (int i = 0; i < dictionaryLength; i++) {
		for (int j = 0; j < dictionaryLength - 1; j++) {
			if ((dictionary[j].p < dictionary[j + 1].p) || (dictionary[j].p == dictionary[j + 1].p && dictionary[j].c > dictionary[j + 1].c)) {
				Shannon tmp = dictionary[j];
				dictionary[j] = dictionary[j + 1];
				dictionary[j + 1] = tmp;
			}
		}
	}
}

ShannonCode::ShannonCode(const string &msg) {
	dictionary = new Shannon[msg.length()]; // создаём массив структур типа Shannon для вычисления кодов

	dictionaryLength = 0; // число уникальных символов для кодирования

	for (size_t i = 0; i < msg.length(); i++) { // проходимся по всем символам строки
		int j = 0; // начинаем искать в созданном массиве символов с нулевого элемента

		// идём до тех пор, пока не кончится массив или пока не встретим такой символ в массиве
		while (j < dictionaryLength && dictionary[j].c != msg[i])
			j++;

		// если индекс стал равен размеру массива, то символа в массиве нет и нужно его туда добавить
		if (j == dictionaryLength) {
			dictionary[dictionaryLength].c = msg[i];
			dictionary[j].p = 1.0 / msg.length(); // вычисляем вероятность
			dictionaryLength++;
		}
		else
			dictionary[j].p += 1.0 / msg.length(); // увеличиваем значение вкероятности

		dictionary[j].L = -log2(dictionary[j].p) + 1; // считаем число бит для кода
	}

	sortByProbability(); // сортируем по частоте встречания символов

	// считаем числа bx
	dictionary[0].b = 0;
	dictionary[0].code = convert(dictionary[0]);

	for (int i = 1; i < dictionaryLength; i++) {
		dictionary[i].b = dictionary[i - 1].b + dictionary[i - 1].p;
		dictionary[i].code = convert(dictionary[i]);
	}
}

void ShannonCode::printTable() const {
	cout << "+-----+------+------+-----+--------+" << endl;
	cout << "|  A  |  P   |  B   |  L  |  Code  |" << endl;
	cout << "+-----+------+------+-----+--------+" << endl;

	for (int i = 0; i < dictionaryLength; i++)
		printf("|  %c  | %4.2lf | %4.2lf | %3d | %6s |\n", dictionary[i].c, dictionary[i].p, dictionary[i].b, dictionary[i].L, dictionary[i].code.c_str()); // выводим полученные данные в табличку

	cout << "+-----+------+------+-----+--------+" << endl;
}

string ShannonCode::codeMessage(const string& msg) {
	string code;

	for (size_t i = 0; i < msg.length(); i++) {
		int j = 0;

		while (j < dictionaryLength && dictionary[j].c != msg[i])
			j++;

		code += dictionary[j].code;
	}

	return code;
}

int main() {
	string msg; // строка для ввода данных
	cout << "Enter message: "; // выводим сообщение - приглашение ко вводу
	getline(cin, msg); // считыфваем строку

	ShannonCode code(msg);

	// выводим полученные значения
	code.printTable();

	cout << endl << "Coded message: " << code.codeMessage(msg) << endl;
}

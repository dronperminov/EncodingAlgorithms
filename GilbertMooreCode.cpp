#include <iostream>
#include <string>
#include <cmath>
#include <cstdio>

using namespace std;


class GilbertMooreCode {
	struct GilbertMoore {
		char c; // символ
		double p; // вероятность встретить этот символ (частота встречаемости
		double Q; // коээфициент для алгоритма
		int L; // длинна двоичного кода
		string code; // код символа
	};

	GilbertMoore *dictionary;
	int dictionaryLength;

	string convert(GilbertMoore elem);

public:
	GilbertMooreCode(const string &msg);
	void printTable() const;
	string codeMessage(const string &msg);
};

// перевод вещественного числа в двоичное представление с заданным числом разрядов (symbol.L)
string GilbertMooreCode::convert(GilbertMoore elem) {
	string result = "";

	for (int i = 0; i < elem.L; i++) {
		elem.Q *= 2;  // умножаем на 2 для получения нового старшего разряда

		result += elem.Q >= 1 ? "1" : "0"; // если после умножения вышла единица в значащий разряд, то прибавляем строку с 1 к результату и вычитаем единицу из числа, чтобы не портить данные

		if (elem.Q >= 1)
			elem.Q -= 1; // зануляем старший разряд, если он стал равен 1
	}

	return result;
}

GilbertMooreCode::GilbertMooreCode(const string &msg) {
	dictionary = new GilbertMoore[msg.length()]; // создаём массив структур типа GilbertMoore для вычисления кодов

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

	// считаем коэффициенты
	double pr = 0;
	for (int i = 0; i < dictionaryLength; i++) {
		dictionary[i].Q = pr + dictionary[i].p / 2;
		pr += dictionary[i].p;
		dictionary[i].L = -log2(dictionary[i].p) + 1;
		dictionary[i].code = convert(dictionary[i]);
	}
}

void GilbertMooreCode::printTable() const {
	cout << "+-----+------+-------+-----+--------+" << endl;
	cout << "|  A  |  Px  |   Q   |  L  |  Code  |" << endl;
	cout << "+-----+------+-------+-----+--------+" << endl;

	for (int i = 0; i < dictionaryLength; i++)
		printf("|  %c  | %4.2lf | %5.3lf | %3d | %6s |\n", dictionary[i].c, dictionary[i].p, dictionary[i].Q, dictionary[i].L, dictionary[i].code.c_str()); // выводим полученные данные в табличку

	cout << "+-----+------+-------+-----+--------+" << endl;
}

string GilbertMooreCode::codeMessage(const string& msg) {
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

	GilbertMooreCode gilbertMur(msg);

	// выводим полученные значения
	gilbertMur.printTable();

	cout << endl << "Coded message: " << gilbertMur.codeMessage(msg) << endl;
}

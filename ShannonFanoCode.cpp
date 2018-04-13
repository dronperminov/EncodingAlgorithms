#include <iostream>
#include <string>
#include <cmath>
#include <cstdio>

using namespace std;


class ShannonFanoCode {
	struct ShannonFano {
		char c; // символ
		int freq; // частота появления символа
		string code; // код символа
	};

	ShannonFano *dictionary;
	int dictionaryLength;

	void sortByProbability();
	void setCodes(int end, int start = 0, const string &code = "");

public:
	ShannonFanoCode(const string &msg);
	void printTable() const;
	string codeMessage(const string &msg);
};

// сортируем по возрастанию вероятности символов
void ShannonFanoCode::sortByProbability() {
	for (int i = 0; i < dictionaryLength; i++) {
		for (int j = 0; j < dictionaryLength - 1; j++) {
			if (dictionary[j].freq < dictionary[j + 1].freq) {
				ShannonFano tmp = dictionary[j];
				dictionary[j] = dictionary[j + 1];
				dictionary[j + 1] = tmp;
			}
		}
	}
}

void ShannonFanoCode::setCodes(int end, int start, const string &code) {
	if (start == end) {
		dictionary[start].code = code;
	    return;
	}

	int sum = 0; // сумма частот
	// считаем сумму частот от начала до конца
	for (int i = start; i <= end; i++)
	    sum += dictionary[i].freq;

	sum /= 2; // делим пополам, чтобы получить идеал равных частот
	int i, sum2 = dictionary[start].freq;

	// ищем границу раздела массива частот для наиболее равномерного разделения
	for (i = start + 1; abs(sum - (sum2 + dictionary[i].freq)) < abs(sum - sum2) && i < end; i++)
	    sum2 += dictionary[i].freq;

	setCodes(i - 1, start, code + "0");
	setCodes(end, i, code + "1");
}

ShannonFanoCode::ShannonFanoCode(const string &msg) {
	dictionary = new ShannonFano[msg.length()]; // создаём массив структур типа ShannonFano для вычисления кодов

	dictionaryLength = 0; // число уникальных символов для кодирования

	for (size_t i = 0; i < msg.length(); i++) { // проходимся по всем символам строки
		int j = 0; // начинаем искать в созданном массиве символов с нулевого элемента

		// идём до тех пор, пока не кончится массив или пока не встретим такой символ в массиве
		while (j < dictionaryLength && dictionary[j].c != msg[i])
			j++;

		// если индекс стал равен размеру массива, то символа в массиве нет и нужно его туда добавить
		if (j == dictionaryLength) {
			dictionary[j].c = msg[i];
			dictionary[j].freq = 1;
			dictionaryLength++;
		}
		else
			dictionary[j].freq++; // увеличиваем значение частоты
	}

	sortByProbability(); // сортируем по частоте появления символов
	setCodes(dictionaryLength - 1);
}

void ShannonFanoCode::printTable() const {
	cout << "+-----+-----+--------+" << endl;
	cout << "|  A  |  F  |  Code  |" << endl;
	cout << "+-----+-----+--------+" << endl;

	for (int i = 0; i < dictionaryLength; i++)
		printf("|  %c  | %3d | %6s |\n", dictionary[i].c, dictionary[i].freq, dictionary[i].code.c_str()); // выводим полученные данные в табличку

	cout << "+-----+------+--------+" << endl;
}

string ShannonFanoCode::codeMessage(const string& msg) {
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

	ShannonFanoCode code(msg);

	// выводим полученные значения
	code.printTable();

	cout << endl << "Coded message: " << code.codeMessage(msg) << endl;
}

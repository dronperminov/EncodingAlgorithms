#include <iostream>
#include <string>
#include <cmath>

using namespace std;

class AriphmeticCode {
	struct Segment {
		char c;
		double p;
		double left;
		double right;
	};

	Segment *segments;
	int segmentsCount;

public:
	AriphmeticCode(const string& msg);
	void printTable() const;

	double* code(const string& msg);
	string decode(double *code, int size);
};


AriphmeticCode::AriphmeticCode(const string& msg) {
	segments = new Segment[msg.length()]; // создаём массив структур типа ShennonT для вычисления кодов

	segmentsCount = 0; // число уникальных символов для кодирования

	for (size_t i = 0; i < msg.length(); i++) { // проходимся по всем символам строки
		int j = 0; // начинаем искать в созданном массиве символов с нулевого элемента

		// идём до тех пор, пока не кончится массив или пока не встретим такой символ в массиве
		while (j < segmentsCount && segments[j].c != msg[i])
			j++;

		// если индекс стал равен размеру массива, то символа в массиве нет и нужно его туда добавить
		if (j == segmentsCount) {
			segments[j].c = msg[i];
			segments[j].p = 1.0 / msg.length(); // вычисляем вероятность
			segmentsCount++;
		}
		else
			segments[j].p += 1.0 / msg.length(); // увеличиваем значение вкероятности
	}

	// определяем границы сегментов
	double l = 0;

	for (int i = 0; i < segmentsCount; i++) {
		segments[i].left = l;
		segments[i].right = l + segments[i].p;
		l = segments[i].right;
	}
}

void AriphmeticCode::printTable() const {
	cout << "+---+---------+-----------+-----------+" << endl;
	cout << "| A |    P    |     L     |     R     |" << endl;
	cout << "+---+---------+-----------+-----------+" << endl;
	
	for (int i = 0; i < segmentsCount; i++)
		printf("| %c | %7.5lf | %9.7lf | %9.7lf |\n", segments[i].c, segments[i].p, segments[i].left, segments[i].right);

	cout << "+---+---------+-----------+-----------+" << endl;
}

double* AriphmeticCode::code(const string& msg) {
	double *code = new double[msg.length()];

	for (size_t i = 0; i < msg.length(); i++) {
		int j = 0;

		while (j < segmentsCount && segments[j].c != msg[i])
			j++;

		code[i] = (segments[j].left + segments[j].right) / 2;
	}
    
    return code;
}

string AriphmeticCode::decode(double *code, int size) {
	string decode = "";

	for (int i = 0; i < size; i++) {
		int j = 0;

		while (j < segmentsCount && (code[i] < segments[j].left || code[i] > segments[j].right))
			j++;

		decode += segments[j].c;
	}

	return decode;
}

int main() {
	string msg; // строка для ввода данных
	cout << "Enter message: "; // выводим сообщение - приглашение ко вводу
	getline(cin, msg); // считыфваем строку

	AriphmeticCode ariphmetic(msg);

	ariphmetic.printTable(); // выводим получившийся результат

	double *code = ariphmetic.code(msg);
	string decode = ariphmetic.decode(code, msg.length());

	cout << endl << "Coding of string: ";
	for (size_t i = 0; i < msg.length(); i++)
		cout << code[i] << " ";

	cout << endl;

	cout << "Decoded string: ";
	for (size_t i = 0; i < msg.length(); i++)
		cout << decode[i];

	cout << endl;
}
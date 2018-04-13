#include <iostream>
#include <string>
#include <list>

using namespace std;

class HaffmanCode {
	struct Haffman {
		char c;
		int p;
		string code;
	};

	struct Tree {
		Haffman value;
		Tree *left;
		Tree *right;
	};

	Haffman* dictionary;
	int dictionaryLength;

	Tree *generateTree();
	void setCodes(Tree *tree, string path = "");

public:
	HaffmanCode(const string& msg);
	void printTable() const;
	string codeMessage(const string &msg);
};

// формирование дерева Хаффмана
HaffmanCode::Tree *HaffmanCode::generateTree() {
	list<Tree *> lst; // создаём список вершин дерева

	for (int i = 0; i < dictionaryLength; i++) {
		Tree *node = new Tree;
		node->value = dictionary[i];
		node->left = NULL;
		node->right = NULL;

		lst.push_back(node);
	}
	
	// пока в списке не останется верхняя вершина
	while (lst.size() != 1) {
		lst.sort([this](const Tree* t1, const Tree* t2) { return t1->value.p < t2->value.p; }); // сортируем список

		Tree *tree = new Tree; // создаём новую вершину
		tree->left = lst.front(); // слева будет первый минимум
		lst.pop_front();
		tree->right = lst.front(); // справа будет второй минимум
		lst.pop_front();
		tree->value.p = tree->left->value.p + tree->right->value.p; // значение частоты есть сумма подвершин

		lst.push_front(tree); // вставляем новую вершину в список
	}

	Tree *tree = lst.front(); // получаем элемент из списка
	lst.pop_front(); // выталкиваем его

	return tree; // возвращаем корень дерева
}

void HaffmanCode::setCodes(Tree *tree, string path) {
	if (!tree->left && !tree->right) {
		int index = 0;

		while (dictionary[index].c != tree->value.c)
			index++;

		dictionary[index].code = path;
		return;
	}

	setCodes(tree->left, path + "0");
	setCodes(tree->right, path + "1");
}

HaffmanCode::HaffmanCode(const string& msg) {
	dictionary = new Haffman[msg.length()]; // создаём массив структур типа Haffman для вычисления кодов

	dictionaryLength = 0; // число уникальных символов для кодирования

	for (size_t i = 0; i < msg.length(); i++) { // проходимся по всем символам строки
		int j = 0; // начинаем искать в созданном массиве символов с нулевого элемента

		// идём до тех пор, пока не кончится массив или пока не встретим такой символ в массиве
		while (j < dictionaryLength && dictionary[j].c != msg[i])
			j++;

		// если индекс стал равен размеру массива, то символа в массиве нет и нужно его туда добавить
		if (j == dictionaryLength) {
			dictionary[j].c = msg[i];
			dictionary[j].p = 1; // вычисляем вероятность
			dictionaryLength++;
		}
		else
			dictionary[j].p++; // увеличиваем значение вкероятности
	}

	Tree *tree = generateTree(); // формируем дерево Хаффмана
	setCodes(tree);
}

void HaffmanCode::printTable() const {
	cout << "+-----+---------+--------------+" << endl;
	cout << "|  A  |    F    |     code     |" << endl;
	cout << "+-----+---------+--------------+" << endl;

	for (int i = 0; i < dictionaryLength; i++)
		printf("|  %c  | %7d | %12s |\n", dictionary[i].c, dictionary[i].p, dictionary[i].code.c_str());

	cout << "+-----+---------+--------------+" << endl;
}

string HaffmanCode::codeMessage(const string &msg) {
	string code = "";

	for (size_t i = 0; i < msg.length(); i++) {
		int j = 0;

		while (dictionary[j].c != msg[i])
			j++;

		code += dictionary[j].code;
	}

	return code;
}

int main() {
	string msg; // строка для ввода данных
	cout << "Enter message: "; // выводим сообщение - приглашение ко вводу
	getline(cin, msg); // считыфваем строку

	HaffmanCode haffman(msg);
	haffman.printTable();

	cout << endl << "Coded message: " << haffman.codeMessage(msg) << endl;
}
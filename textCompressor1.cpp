#include "iostream"
#include "vector"
#include "list"
#include "map"
using namespace std;

//---------------------------Classes-------------------------------
class Node {
 public:
	int data;
	char symbol;
	Node *left, *right;

	Node(Node *l, Node *r) {
		left = l;
		right = r;
		data = l->data + r->data;
		symbol = '\0';
	}

	Node() = default;
};

// Компаратор для узлов
struct MyCompare {
	bool operator()(Node *l, Node *r) {
		return (l->data < r->data);
	}
};
//-----------------------------------------------------------------

void countSymbols(map<char, int> &m, const string &s) {
	for (int i = 0; i < s.length(); i++) {
		char c = s[i];
		m[c]++;
	}
}

//----------------------------Outers-------------------------------
void out(vector<bool> &coded) {
	for (vector<bool>::iterator itr = coded.begin(); itr != coded.end(); itr++) { cout << *itr; }
}

void out(map<char, vector<bool>> &m) {
	for (auto itr = m.begin(); itr != m.end(); itr++) {
		cout << itr->first << " ";
		for (int k = 0; k < itr->second.size(); k++) { cout << itr->second[k]; }
		cout << endl;
	}
}

void out(Node *root, unsigned int k = 0) {
	if (root != nullptr) {
		out(root->left, k + 3);

		for (unsigned i = 0; i < k; i++) {
			cout << " ";
		}
		if (root->symbol) cout << root->data << "(" << root->symbol << ")" << endl;
		else (cout << root->data << endl);
		out(root->right, k + 3);
	}
}
//-----------------------------------------------------------------

//----------------------------Behind the scenes--------------------
void map2list(map<char, int> &m, list<Node *> &t) {
	for (map<char, int>::iterator i = m.begin(); i != m.end(); i++) {
		Node *p = new Node;
		p->symbol = i->first;
		p->data = i->second;
		t.push_back(p);
	}
}

void makeTree(list<Node *> &t) {
	// Пока в списке не останется один элемент
	while (t.size() > 1) {
		// Мы его сортируем
		t.sort(MyCompare());
		// Вытаскиваем оттуда 2 первых элемента
		Node *l = t.front();
		t.pop_front();
		Node *r = t.front();
		t.pop_front();
		// Делаем из них родителя
		Node *parent = new Node(l, r);
		// И пихаем назад в список
		t.push_front(parent);
		// Таким образом получаем дерево
	}
}

void buildTable(Node *root, vector<bool> &code, map<char, vector<bool>> &table) {
	// Если есть, куда идти влево, то в вектор кода идет 0
	if (root->left != nullptr) {
		code.push_back(0);
		buildTable(root->left, code, table);
	}
	// Когда некуда идти влево, идет вправо и пихает в код 1
	if (root->right != nullptr) {
		code.push_back(1);
		buildTable(root->right, code, table);
	}
	// Если встретился узел, в котором есть символ, то этот символ будет обладать кодом,
	// равным пути к символу от корня (влево - 0, вправо - 1)
	if (root->symbol) { table[root->symbol] = code; }
	// Возвращаемся на шаг назад, чтоб пойти дальше
	code.pop_back();
}
//-----------------------------------------------------------------

//----------------------------Code and decode----------------------
vector<bool> encode(string &s, map<char, vector<bool>> &table) {
	vector<bool> coded;
	// Проходит по каждому символу
	for (int i = 0; i < s.length(); i++) {
		// По каждому биту из символа
		for (int k = 0; k < table[s[i]].size(); k++) {
			// В закодированный вектор вставляет код символа
			coded.push_back(table[s[i]][k]);
		}
	}
	return coded;
}

string decode(vector<bool> &coded, map<char, vector<bool>> &table, Node *root) {
	int i = 0;
	string decoded;
	while (i < coded.size()) {
		// Сохраняем значение корня
		Node *safeRoot = root;
		// Пока в узле не содержится символ
		while (!root->symbol) {
			// Если в закодированном виде бит == 1, то идем вправо
			if (coded[i]) root = root->right;
				// Если нет, то влево
			else root = root->left;
			i++;
		}
		// Прибавляем к выходной строке символ, который нашли
		decoded += root->symbol;
		// И восстанавливаем корень
		root = safeRoot;
	}
	return decoded;
}
//-----------------------------------------------------------------

int main() {
	string s = "im is y striiiiing!!!!";
	// Создаю ассоциативный массив для связи символа и количества его вхождений
	map<char, int> m;
	// Считаю кол-во вхождений каждого символа
	countSymbols(m, s);
	// Создаю список, чтобы на нем позже создать дерево
	list<Node *> tree;
	// Заполняю список
	map2list(m, tree);
	// Создаю дерево из списка
	makeTree(tree);
	// Корень дерева - единственный оставшийся в списке элемент
	Node *root = tree.front();
	//out(root);	//Выводит все дерево целиком
	// Вектор, который хранит в себе код одного из элементов
	vector<bool> code;
	// Ассоциативный массив для связи символа и его кода
	map<char, vector<bool>> table;
	// Строю таблицу символ-код
	buildTable(root, code, table);
	out(table);  //Выводит таблицу кодировки
	// Кодирую свое сообщение
	vector<bool> coded = encode(s, table);
	// Выводит закодированное сообщение
	out(coded);
	cout << "			Coded size: " << coded.size() / 8 + 1;
	// Декодирование
	string decoded = decode(coded, table, root);
	cout << endl << decoded << "		Decoded size: " << decoded.length();
	return 0;
}
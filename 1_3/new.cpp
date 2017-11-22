#include <iostream>
#include <stack>
#include <queue>
using namespace std;

struct Node {
	int key;
	Node *left, *right;
	explicit Node(int k) : key(k), left(), right() {}
	Node() {}
	explicit Node(Node *n) {
		key = n->key;
		left = new Node(n->left);
		right = new Node(n->right);
	}
};

Node *add(int k, Node *root) {
	stack<Node *> stack;  // В стэке храню путь от корня до нужного элемента
	Node *node = root;    // Временная переменная для поиска места вставки
	while (node) {        // Пока не лист
		stack.push(node);
		if (k < node->key) { node = node->left; }
		else { node = node->right; }
	}
	node = new Node(k);    // Когда лист, создаю новую ноду
	Node *top;            // Переменная, указывающая на top стэка
	if (!stack.empty()) {
		top = stack.top();
		stack.pop();
	}
	else { return node; }  // Если стэк пуст, node == корень, возвращаем его
	while (!stack.empty()) {
		if (node->key < top->key) { top->left = node; }  // Прохожу по пути из стэка и присваиваю элементам из пути детей
		else { top->right = node; }
		node = top;
		top = stack.top();
		stack.pop();
	}
	if (node->key < top->key) { top->left = node; }
	else { top->right = node; }
	return top;            // Возвращаю после подъема по пути наверх, т.е. top == root
}

void postOrder(Node *root) {
	if (!root) return;
	stack<Node *> stack;
	do {
		while (root) {
			if (root->right) stack.push(root->right);
			stack.push(root);
			root = root->left;
		}
		root = stack.top();
		stack.pop();
		if (root->right && !stack.empty() && stack.top() == root->right) {
			stack.pop();
			stack.push(root);
			root = root->right;
		} else {
			cout << root->key << " ";
			root = nullptr;
		}
	} while (!stack.empty());
}

void deleting(Node *&root) {
	queue<Node *> q;
	q.push(root);
	while (!q.empty()) {
		Node *node = q.front();
		q.pop();
		if (node->left != nullptr) { q.push(node->left); }
		if (node->right != nullptr) { q.push(node->right); }
		delete node;
	}
}

int main() {
	Node *root = nullptr;
	int n;
	cin >> n;
	int k;
	for (int i = 0; i < n; ++i) {
		cin >> k;
		root = add(k, root);
	}
	postOrder(root);
	deleting(root);
	return 0;
}
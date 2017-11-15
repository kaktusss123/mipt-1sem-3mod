#include <iostream>
#include <stack>

using namespace std;

struct DecartNode {
	DecartNode() = default;
	DecartNode(int k, int p) : key(k), priority(p), left(), right() {}
	~DecartNode() {
		delete left;
		delete right;
	}

	int key;
	int priority;
	DecartNode *left;
	DecartNode *right;
};

struct NaiveNode {
	NaiveNode() : key(0), left(), right() {}
	explicit NaiveNode(int k) : key(k), left(), right() {}
	~NaiveNode() {
		delete left;
		delete right;
	}

	int key;
	NaiveNode *left;
	NaiveNode *right;
};

class Treap {
 public:
	Treap() : root() {}

	Treap(int key, int priority) { root = new DecartNode(key, priority); }

	~Treap() { delete root; }

	void Insert(int key, int priority) {
		DecartNode *parrentNow = nullptr;
		DecartNode *head = root;
		while (head != nullptr && priority < head->priority) {
			if (head->key < key) {
				parrentNow = head;
				head = head->right;
			} else {
				parrentNow = head;
				head = head->left;
			}
		}
		auto *nNode = new DecartNode(key, priority);
		Split(head, key, nNode->left, nNode->right);
		if (parrentNow == nullptr) {
			root = nNode;
			return;
		}
		if (parrentNow->key < key) {
			parrentNow->right = nNode;
		} else {
			parrentNow->left = nNode;
		}
	}

	DecartNode *getRoot() { return root; }

 private:
	DecartNode *root;

	void Split(DecartNode *treap, int key, DecartNode *&treap1, DecartNode *&treap2) {
		if (treap == nullptr) {
			treap1 = nullptr;
			treap2 = nullptr;
			return;
		}
		if (treap->key < key) {
			Split(treap->right, key, treap->right, treap2);
			treap1 = treap;
			return;
		} else {
			Split(treap->left, key, treap1, treap->left);
			treap2 = treap;
		}
	}
};

class NaiveTree {

 public:
	NaiveTree() : root() {}
	explicit NaiveTree(int key) : root() { Insert(key); }
	~NaiveTree() { delete root; }

	void Insert(int key) {
		insert(key, root);
	}

	NaiveNode *getRoot() { return root; }

 private:
	NaiveNode *root;

	void insert(int key, NaiveNode *&node) {
		if (node == nullptr) { node = new NaiveNode(key); }
		else {
			if (key < node->key) { insert(key, node->left); }
			else { insert(key, node->right); }
		}
	}
};

void input(Treap &treap, NaiveTree &naive, int size) {
	int k, p;
	for (int i = 0; i < size; i++) {
		cin >> k >> p;
		naive.Insert(k);
		treap.Insert(k, p);
	}
}

template<class T>
int getWidth(T *node) {
	int width = 0;
	stack<T *> oldStack;
	oldStack.push(node);
	while (!oldStack.empty()) {
		if (width < oldStack.size()) { width = oldStack.size(); }
		stack<T *> newStack;
		while (!oldStack.empty()) {
			T *tmp = oldStack.top();
			oldStack.pop();
			if (tmp->left) { newStack.push(tmp->left); }
			if (tmp->right) { newStack.push(tmp->right); }
		}
		swap(newStack, oldStack);
	}
	return width;
}

int main() {
	int N;
	cin >> N;
	Treap treap;
	NaiveTree naive;
	input(treap, naive, N);
	cout << getWidth(treap.getRoot()) - getWidth(naive.getRoot());
	return 0;
}
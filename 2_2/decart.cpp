#include "iostream"
#include "stack"
using namespace std;

struct naiveNode {
	naiveNode *left = nullptr, *right = nullptr;
	int val;
};

struct decNode {
	int key, priority;
	decNode *left, *right;

	decNode(int k, int p) {
		key = k;
		priority = p;
		left = nullptr;
		right = nullptr;
	}
};

class Treap {
 private:
	decNode *root;

	// Код из лекции
	void Split(decNode *currentNode, int key, decNode *&left, decNode *&right) {
		if (currentNode == nullptr) {
			left = nullptr;
			right = nullptr;
		} else if (currentNode->key <= key) {
			Split(currentNode->right, key, currentNode->right, right);
			left = currentNode;
		} else {
			Split(currentNode->left, key, left, currentNode->left);
			right = currentNode;
		}
	}

 public:
	Treap() = default;
	void deleting(naiveNode *&node) {
		if (node->left != nullptr) deleting(node->left);
		if (node->right != nullptr) deleting(node->right);
		delete node;
	}

	decNode *getRoot() { return root; }

	//~Treap() { deleting(root); }

	void add(int k, int p) {
		auto *newNode = new decNode(k, p);
		if (root == nullptr) {
			root = newNode;
		} else {
			decNode *currentNode = root;
			decNode *currentParent = nullptr;
			while (newNode->priority <= currentNode->priority) {
				if (newNode->key < currentNode->key) {
					if (currentNode->left != nullptr) {
						currentParent = currentNode;
						currentNode = currentNode->left;
					} else {
						currentNode->left = newNode;
						return;
					}
				} else {
					if (currentNode->right != nullptr) {
						currentParent = currentNode;
						currentNode = currentNode->right;
					} else {
						currentNode->right = newNode;
						return;
					}
				}
			}

			if (currentParent != nullptr) {
				if (currentParent->left == currentNode) {
					currentParent->left = newNode;
				} else {
					currentParent->right = newNode;
				}
			} else {
				root = newNode;
			}

			Split(currentNode, newNode->key, newNode->left, newNode->right);
		}
	}
};

template<class T>
unsigned int getWidth(T *&root) {
	unsigned int max = 0;

	stack<T *> oldLayer;
	oldLayer.push(root);
	while (!oldLayer.empty()) {
		if (max < oldLayer.size()) { max = oldLayer.size(); }
		stack<T *> newLayer;
		while (!oldLayer.empty()) {
			T *node = oldLayer.top();
			oldLayer.pop();
			if (node->left != nullptr) { newLayer.push(node->left); }
			if (node->right != nullptr) { newLayer.push(node->right); }
		}
		swap(oldLayer, newLayer);
	}
	return max;
}

void _naive_add(int val_, naiveNode *&node) {
	if (node != nullptr) {
		if (val_ > node->val) {
			_naive_add(val_, node->left);
		} else {
			_naive_add(val_, node->right);
		}
	} else {
		auto *tmp = new naiveNode;
		node = tmp;
		node->val = val_;
	}
}

void naive_add(int val_, naiveNode *&root) {
	if (root == nullptr) {
		auto *tmp = new naiveNode;
		root = tmp;
		root->val = val_;
	} else {
		_naive_add(val_, root);
	}
}

void inputTrees(naiveNode *&root, Treap &treap, int size) {
	int k, p;
	for (int i = 0; i < size; i++) {
		cin >> p >> k;
		if (root == nullptr) {
			auto *tmp = new naiveNode;
			root = tmp;
			root->val = k;
		} else { naive_add(k, root); }
		treap.add(k, p);
	}
}

int main() {
	naiveNode *naiveTree = nullptr;
	Treap treap;
	int N;
	cin >> N;
	inputTrees(naiveTree, treap, N);
	decNode *tmp = treap.getRoot();
	int first = getWidth(tmp);
	int second = getWidth(naiveTree);
	cout << abs(first - second);
	return 0;
}
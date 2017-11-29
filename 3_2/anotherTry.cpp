#include <iostream>
using namespace std;

struct Node {
	int key, size;
	int balance;
	Node *left, *right, *parent;
	Node(int k, Node *par) : key(k), size(1), balance(0), left(), right(), parent(par) {}
	explicit Node(Node *&n) {
		if (n) {
			this->key = n->key;
			this->size = n->size;
			this->balance = n->balance;
			if (n->left) this->left = new Node(n->left);
			if (n->right) this->right = new Node(n->right);
			this->parent = n->parent;
		}
	}
};

class AVLTree {
 public:
	AVLTree() : root() {}
	~AVLTree() { deleting(root); }
	void insert(int key) {
		if (!root) {
			root = new Node(key, nullptr);
			return;
		}
		Node *par = findPlace(root, key);
		rebalance(par);
	}
	int findKStat(int num) {
		return _findKStat(num, root);
	}

	void remove(int k) {
		Node *delNode = find(k);
		Node *parent;
		if (delNode->parent) {      /// Если не корень
			parent = delNode->parent;
			/// Если лист
			if (!delNode->left && !delNode->right) {
				if (parent->left == delNode) {
					parent->left = nullptr;
					delete delNode;
				} else {
					parent->right = nullptr;    // Тут возможна ошибка
					delete delNode;
				}
				return;
			}
			if (!delNode->right) {
				auto *l = delNode->left;
				if (parent->left == delNode) {
					parent->left = l;
					l->parent = parent;
					delete delNode;
					rebalance(parent->left);
				} else {
					parent->right = l;    // Тут возможна ошибка
					l->parent = parent;
					delete delNode;
					rebalance(parent->right);
				}
				return;
			}
			if (!delNode->left) {
				auto *r = delNode->right;
				if (parent->left == delNode) {
					parent->left = r;
					r->parent = parent;
					delete delNode;
					rebalance(parent->left);
				} else {
					parent->right = r;    // Тут возможна ошибка
					r->parent = parent;
					delete delNode;
					rebalance(parent->right);
				}
				return;
			}
/// Если не лист
			Node *closest = findClosest(delNode->left);
			int clK = closest->key;
			remove(closest->key);
			delNode->key = clK;
			rebalance(delNode);
		} else {
/// Если корень
			if (!delNode->left && !delNode->right) {
				delete delNode;
				root = nullptr;
				return;
			}
			if (!delNode->right) {
				auto *l = delNode->left;
				root = l;
				delete delNode;
				return;
			}
			if (!delNode->left) {
				auto *r = delNode->right;
				root = r;
				delete delNode;
				return;
			}
			Node *closest = findClosest(delNode->left);
			int clK = closest->key;
			remove(closest->key);
			delNode->key = clK;
			rebalance(delNode);
		}
	}

 private:
	Node *root;

	int _findKStat(int num, Node *node) {
		if (num == size(node->left) + 1) { return node->key; } // Если корень
		if (num < size(node->left) + 1) { return _findKStat(num, node->left); }
		else { return _findKStat(num - size(node->left) - 1, node->right); }
	}
	Node *findClosest(Node *p) {
		return p->right ? findClosest(p->right) : p;
	}
	Node *find(int key, Node *node = nullptr) {
		if (!node) { node = root; }
		if (key < node->key) {
			--node->size;
			return find(key, node->left);
		} else if (key > node->key) {
			--node->size;
			return find(key, node->right);
		} else { return node; }
	}
	Node *findPlace(Node *&node, int key, Node *parrent = nullptr) {
		if (!node) {
			node = new Node(key, parrent);
			return parrent;
		}
		if (key < node->key) {
			++node->size;
			return findPlace(node->left, key, node);
		} else {
			++node->size;
			return findPlace(node->right, key, node);
		}
	}
	Node *rotateRight(Node *a) {
		Node *b = a->right;
		b->parent = a->parent;
		a->right = b->left;
		if (a->right) { a->right->parent = a; }
		b->left = a;
		a->parent = b;
		if (b->parent) {
			if (b->parent->right == a) { b->parent->right = b; }
			else { b->parent->left = b; }
		}
		setBalance(a);
		setSize(a);
		setBalance(b);
		setSize(b);
		return b;
	}
	Node *rotateLeft(Node *a) {
		Node *b = a->left;
		b->parent = a->parent;
		a->left = b->right;
		if (a->left) { a->left->parent = a; }
		b->right = a;
		a->parent = b;
		if (b->parent) {
			if (b->parent->right == a) { b->parent->right = b; }
			else { b->parent->left = b; }
		}
		setBalance(a);
		setSize(a);
		setBalance(b);
		setSize(b);
		return b;
	}
	Node *rotateLeftRight(Node *n) {
		n->left = rotateLeft(n->left);
		return rotateRight(n);
	}
	Node *rotateRightLeft(Node *n) {
		n->right = rotateRight(n->right);
		return rotateLeft(n);
	}
	void setSize(Node *&n) {
		n->size = size(n->left) + size(n->right) + 1;
	}
	void rebalance(Node *n) {
		setBalance(n);
		setSize(n);
		if (n->balance == -2) {
			if (height(n->left->left) >= height(n->left->right)) { n = rotateLeft(n); }
			else { n = rotateLeftRight(n); }
		} else if (n->balance == 2) {
			if (height(n->right->right) >= height(n->right->left)) { n = rotateRight(n); }
			else { n = rotateRightLeft(n); }
		}
		if (n->left == root || n->right == root) { n->parent = nullptr; }
		if (n->parent) { rebalance(n->parent); }
		else { root = n; }
	}
	int height(Node *&n) {
		if (!n) { return -1; }
		else { return max(height(n->left), height(n->right)) + 1; }
	}
	int size(Node *&n) { return n ? n->size : 0; }
	void setBalance(Node *&n) {
		n->balance = height(n->right) - height(n->left);
	}
	void deleting(Node *&node) {
		if (node->left) {
			deleting(node->left);
		}
		if (node->right) {
			deleting(node->right);
		}
		delete (node);
	}
};

int main() {
	AVLTree avl;
	int n, key, num;
	cin >> n;
	for (int i = 0; i < n; i++) {
		cin >> key >> num;
		if (key > 0) { avl.insert(key); }
		else { avl.remove(-key); }
		cout << avl.findKStat(num + 1) << " ";
	}
	return 0;
}
#include <iostream>
using namespace std;

struct binaryNode {
	binaryNode *left = nullptr, *right = nullptr;
	long val;

	binaryNode &operator=(binaryNode const &second) {
		left = second.left;
		right = second.right;
		val = second.val;
		return *this;
	}
};

class BinaryTree {
 private:
	binaryNode *root;

	void _add(long val_, binaryNode *&node) {
		if (node != nullptr) {
			if (val_ > node->val) {
				_add(val_, node->left);
			} else {
				_add(val_, node->right);
			}
		} else {
			auto *tmp = new binaryNode;
			node = tmp;
			node->val = val_;
		}
	}

 public:
	BinaryTree() = default;

	void add(long val_) {
		if (root == nullptr) {
			auto *tmp = new binaryNode;
			root = tmp;
			root->val = val_;
		} else {
			_add(val_, root);
		}
	}

	void deleting(binaryNode *&node) {
		if (node->left != nullptr) deleting(node->left);
		if (node->right != nullptr) deleting(node->right);
		delete node;
	};

	~BinaryTree() {
		deleting(root);
	}

	binaryNode *getRoot() {
		return root;
	}
};

void inputTree(BinaryTree &tree, int size) {
	long k;
	for (int i = 0; i < size; i++) {
		cin >> k;
		tree.add(k);
	}
}

void postOrder(binaryNode *node) {
	if (node == nullptr) { return; }
	if (node->right != nullptr) { postOrder(node->right); }
	if (node->left != nullptr) { postOrder(node->left); }
	cout << node->val << " ";
}

int main() {
	BinaryTree tree = {};
	int N;
	cin >> N;
	inputTree(tree, N);
	postOrder(tree.getRoot());
	return 0;
}
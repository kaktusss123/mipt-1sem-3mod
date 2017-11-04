
#include <iostream>
using namespace std;

struct binaryNode {
	binaryNode *left, *right;
	int val;
	binaryNode(int val_) {
		left = nullptr;
		right = nullptr;
		val = val_;
	}
};

class BinaryTree {
 private:
	binaryNode *root;

	void _add(int val, binaryNode *node) {
		if (val < node->val) {
			if (node->left != nullptr) { _add(val, node->left); }
			else {
				binaryNode *less = new binaryNode(val);
				node->left = less;
			}
		} else {
			if (node->right != nullptr) { _add(val, node->right); }
			else {
				binaryNode *more = new binaryNode(val);
				node->right = more;
			}
		}
	}

	//       3
	//    0     4
	//       2     8

	void _printTree(binaryNode *node) {
		if (node != nullptr) {
			_printTree(node->left);
			cout << node->val << " ";
			_printTree(node->right);
		}
	}

 public:
	BinaryTree() { root = nullptr; }

	binaryNode *getRoot() { return root; }

	void add(int val_) {
		if (root == nullptr) {
			binaryNode tmp(val_);
			root = &tmp;
		} else _add(val_, root);
	}

	void printTree() {
		if (root != nullptr) {
			_printTree(root);
		}
	}

};

int main() {
	BinaryTree tree;
	tree.add(3);
	tree.add(4);
	tree.add(0);
	tree.add(8);
	tree.add(2);
	tree.printTree();

	return 0;
}
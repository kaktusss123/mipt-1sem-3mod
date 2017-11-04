#include <iostream>
using namespace std;

struct binaryNode
{
	binaryNode* left;
	binaryNode* right;
	int data;

	explicit binaryNode(int val)
	{
		left = nullptr;
		right = nullptr;
		data = val;
	}

};

void add(int val, binaryNode*& parent, bool less = true)
{
	if (parent != nullptr) {
		if (val < parent->data) {
			less = true;
			add(val, parent->left, less);
		}
		else {
			less = false;
			add(val, parent->right, less);
		}
	} else {
		if (less)
		{
			binaryNode nodeLess(val);
			parent = &nodeLess;
		}
		else
		{
			binaryNode nodeMore(val);
			parent = &nodeMore;
		}
		}
}

int main()
{
	binaryNode* root = nullptr;
	add(5, root);
	add(1, root);
	add(13, root);
	return 0;
}
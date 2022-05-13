#pragma once
#include "BNode.h"
#include <ostream>

class BTree
{
	public:
		const static int T = 3;
	private:
		BNode* root;

	public:
		BTree();
		pair<BNode*, int> search(int key);
		void insert(int key);
		void remove(int key);
		void print(ostream& os);
};


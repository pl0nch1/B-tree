#include "BTree.h"

BTree::BTree()
{
	root = new BNode();
}

pair<BNode*, int> BTree::search(int key)
{
	return root->search(key);
}

void BTree::insert(int key)
{
	BNode* r = root;
	if (r->keys.size() == 2 * T - 1)
	{
		BNode* s = new BNode();
		root = s;
		s->leaf = false;
		s->childs.push_back(r);
		s->split(0);
		s->insert(key); //B_TREE_INSERT_NON_FULL;
	}
	else
		r->insert(key); //B_TREE_INSERT_NON_FULL;
}

void BTree::remove(int key)
{
	root->remove(key);
	if (root->keys.size() == 0)
	{
		BNode* toRemove = root;
		root = root->childs[0];
		delete toRemove;
	}
}

void BTree::print(ostream& os)
{
	root->print(os, 0);
}

#include "BTree.h"

void BNode::print(ostream& os, int depth)
{
	for (int j = 0; j < depth; j++)
		os << "   ";
	for (int i = 0; i < keys.size(); i++) 
	{
		os << keys[i] << " ";
	}
	os << endl;
	for (int i = 0; i < childs.size(); i++)
	{
		childs[i]->print(os, depth + 1);
	}
}

BNode::BNode()
{
	leaf = true;
}

pair<BNode*, int> BNode::search(int key)
{
	int i = 0;
	while ((i < keys.size()) && (key > keys[i]))
		i++;
	if ((i < keys.size()) && (key == keys[i]))
		return {childs[i], i};
	if (isLeaf())
		return { NULL, 0 };
	else
		return childs[i]->search(i);

	return pair<BNode*, int>();
}


void BNode::split(int i)
{
	BNode* z = new BNode();
	BNode* y = childs[i];
	z->leaf = y->leaf;
	for (int j = 0; j < BTree::T - 1; j++)
	{
		z->keys.push_back(y->keys[BTree::T + j]);
	}

	if (!y->isLeaf())
		for (int j = 0; j < BTree::T; j++) {
			z->childs.push_back(y->childs[BTree::T + j]);
		}

	childs.insert(childs.begin() + i + 1, z);
	keys.insert(keys.begin() + i, y->keys[BTree::T - 1]);

	for (int i = 0; i < BTree::T; i++) {
		y->keys.pop_back();
		if (!y->isLeaf())
			y->childs.pop_back();
	}
}

bool BNode::isLeaf()
{
	return leaf;
}

void BNode::insert(int key)
{
	if (isLeaf()) 
	{
		int i = 0;
		while ((i < keys.size()) && (key > keys[i]))
			i++;
		keys.insert(keys.begin() + i, key);
	}
	else 
	{
		int i = 0;
		while ((i < keys.size()) && (key > keys[i]))
			i++;
		if (childs[i]->keys.size() == 2 * BTree::T - 1) {
			split(i);
			if (key > keys[i])
				i++;
		}
		childs[i]->insert(key);
	}
}

int BNode::getGreatest()
{
	if (!leaf)
		return childs.end()[0]->getGreatest();
	return keys[keys.size() - 1];
}

int BNode::getLowest()
{
	if (!leaf)
		return childs[0]->getLowest();
	return keys[0];
}

void BNode::remove(int key)
{
	int i = 0;
	while ((i < keys.size()) && (keys[i] < key))
		i++;

	if (leaf) {
		if ((i < keys.size()) && (keys[i] == key))
			keys.erase(keys.begin() + i);
		return;
	}

	if ((i < keys.size()) && (keys[i] == key))
	{
		BNode* y = childs[i]; // To the left of target key
		BNode* z = childs[i + 1]; // To the right of target key
		if (y->keys.size() >= BTree::T) {
			int newKey = y->getGreatest();
			keys[i] = newKey;
			y->remove(newKey);
		}
		else if (z->keys.size() >= BTree::T)
		{
			int newKey = z->getLowest();
			keys[i] = newKey;
			z->remove(newKey);
		}
		else
		{
			merge(i);
			
			// ATTENTION: z node no longer exists: z merged into y
			y->remove(key);
		}
		return;
	}

	//The third way starts here
	//'i' is insertion group index (target child index)
	if (childs[i]->keys.size() < BTree::T) {
		if ((i > 0) && (childs[i - 1]->keys.size() >= BTree::T))
			rotate(i - 1, true);
		else if ((i < childs.size() - 1) && (childs[i + 1]->keys.size() >= BTree::T))
			rotate(i, false);
		else if (i > 0)
			merge(i - 1);
		else
			merge(i);
	}

	childs[i]->remove(key);
}

void BNode::merge(int i)
{
	BNode* y = childs[i]; // To the left of target key
	BNode* z = childs[i + 1]; // To the right of target key
	y->keys.push_back(keys[i]);
	for (int j = 0; j < z->keys.size(); j++)
	{
		y->keys.push_back(z->keys[j]);
	}

	for (int j = 0; j < z->childs.size(); j++)
	{
		y->childs.push_back(z->childs[j]);
	}

	childs.erase(childs.begin() + i + 1);
	keys.erase(keys.begin() + i);

	delete z;
}

void BNode::rotate(int i, bool toRight)
{
	BNode* right = childs[i + 1];
	BNode* left = childs[i];

	if (toRight) 
	{
		int leftChildSize = left->childs.size();
		int leftKeySize = left->keys.size();
		right->keys.insert(right->keys.begin(), keys[i]);
		right->childs.insert(right->childs.begin(), left->childs[leftChildSize - 1]);
		keys[i] = left->keys[leftKeySize - 1];
		left->keys.erase(left->keys.begin() + leftKeySize - 1);
		left->childs.erase(left->childs.begin() + leftChildSize - 1);
	}
	else 
	{
		left->keys.push_back(keys[i]);
		left->childs.push_back(right->childs[0]);
		keys[i] = right->keys[0];
		right->keys.erase(right->keys.begin());
		right->childs.erase(right->childs.begin());
	}
}

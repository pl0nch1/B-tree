#include "BTree.h"
#include <iostream>

int main() 
{
	BTree tree;
	for (int i=1; i < 22; i++)
		tree.insert(i);
	tree.remove(7);
	tree.remove(12);
	tree.remove(15);
	tree.print(cout);
	return 0;
}
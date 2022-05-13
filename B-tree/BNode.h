#include <vector>
#include <ostream>

using namespace std;


class BNode 
{
	friend class BTree;
	private:
		vector<int> keys;
		vector<BNode*> childs;
		bool leaf;
	
	private:
		void print(ostream& os, int depth);
		BNode();
		int getGreatest();
		int getLowest();
		void remove(int key);
	public:
		pair<BNode*, int> search(int key);
		void split(int i);
		void merge(int i);
		void rotate(int i, bool toRight);
		bool isLeaf();
		void insert(int key); //INSERT_NON_FULL
};
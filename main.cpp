#include "BPTree.h"


int main(int argc, char** argv){
	BPTree *bpt = new BPTree();
	bpt->buildTree();
	bpt->printTree();
	delete bpt;
	return 0;
	//BPTree *bpt = new BPTree();
	//int i;
	//time_t t1 = time(NULL);
	//for ( i = 0; i < 100; i++)
	//{
	//	bpt->insert(&i, &i);
	//}
	//time_t t2 = time(NULL);
	//KeyType *pos = new KeyType;
	//*pos = 20;
	//DataType *data = new DataType();
	////search
	//bpt->search(pos, data);
	//cout << "get:" << *data << endl;

	////modify
	//*data = 350;
	//bpt->modify(pos, data);
	//cout << "change key == 20 -> data to :350" << endl;
	////remove
	//*pos = 88;
	//bpt->remove(pos);
	//cout << "remove(88)" << endl;
	////print data in leaves
	////Node *p;
	////p = bpt->getHead();
	////while (p)
	////{
	////	int num = p->getNum();
	////	for (int i = 0; i < num; i++)
	////	{
	////		cout << *(p->getData(i)) << " ";
	////	}
	////	cout << endl;
	////	p = p->getNext();
	////}
	//cout << "inserted "<< i <<" keys   time£º" << t2 - t1 << 's' << endl;
	//// print
	//bpt->printTree();
	//// save
	//bpt->save();
	//delete bpt;
	//return 0;
}
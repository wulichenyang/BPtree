#include "Node.h"
#include "LeafNode.h"
#include "InNode.h"

void Node::initKeys()
{
	keys = new KeyType[MAX_KEY_NUM];
}

Node::Node()
{
	nodeType = INTERNAL;
	keyNum = 0;
	initKeys();
}


Node::~Node()
{
}

bool Node::isLeaf()
{
	return nodeType == LEAF;
}

int Node::getNum() const
{
	return keyNum;
}

bool Node::setNum(int _num)
{
	if (_num < 0 || _num > MAX_KEY_NUM)
	{
		printf("set num error");
		return false;
	}
	keyNum = _num;
	return true;
}

bool Node::addNum()
{
	if (keyNum + 1 > MAX_KEY_NUM)
	{
		printf("add num error");
		return false;
	}
	keyNum++;
	return true;
}

bool Node::subNum()
{
	if (this->getNum() < 1)
	{
		printf("sub num error");
		return false;
	}
	keyNum--;
	return true;
}

const KeyType * Node::getKey(int _index) const
{
	if (_index < 0 || _index >= this->getNum())
	{
		printf("get key error\n");
		return NULL;
	}
	return keys + _index;
}

bool Node::setKey(int _index, const KeyType * key)
{
	if (_index < 0 || _index >= this->getNum())
	{
		printf("set key error in key:%d\n", *key);
		return false;
	}
	*(keys + _index) = *key;
	return true;
}

bool Node::addKey(int _index, const KeyType * key)
{
	int num = this->getNum();
	if (_index < 0 || _index > num)
	{
		printf("add key error");
		return false;
	}
	for (int i =  num - 1; i >= _index; --i)
	{
		keys[i + 1] = keys[i];
	}
	keys[_index] = *key;
	return true;
}

bool Node::subKey(int _index)
{
	int num = this->getNum();
	if (_index < 0 || _index >= num)
	{
		printf("sub key error");
		return false;
	}
	for (int i = _index; i < num - 1; ++i)
	{
		keys[i] = keys[i + 1];
	}
	return true;
}


bool Node::writeNode(Node * p, FILE *fp)
{
	if (p == NULL)
		return false;
	int isLeaf = p->isLeaf();
	int n = p->getNum();
	fwrite(&isLeaf, sizeof(int), 1, fp);
	fwrite(&n, sizeof(int), 1, fp);
	for (size_t i = 0; i < n; i++)
	{
		fwrite(p->getKey(i), sizeof(KeyType), 1, fp);
	}
	// InNode recursive , write childs
	if (!isLeaf)
	{
		for (size_t i = 0; i < n + 1; i++)
		{
			writeNode(p->getChild(i), fp);
		}
	}
	// LeafNode write data
	else
	{
		for (size_t i = 0; i < n; i++)
		{
			fwrite(p->getData(i), sizeof(DataType), 1, fp);
		}
	}
	return true;
}

bool Node::readNode(FILE *fp, Node *&fap, int pos)
{
	static int flag = 0;
	int isLeaf;
	int n;
	if (fread(&isLeaf, sizeof(int), 1, fp) != NULL)
	{
		fread(&n, sizeof(int), 1, fp);
		// InNode
		if (!isLeaf)
		{
			Node *chp = new InNode();
			chp->setNum(n);
			
			if (flag == 0)
			{
				flag = 1;
				fap = chp; // root
			}
			else
			{
				fap->setChild(pos, chp);
				fap = fap->getChild(pos);
			}
			// loop to set key
			for (size_t i = 0; i < n; i++)
			{
				if (fread((void*)(chp->getKey(i)), sizeof(KeyType), 1, fp) == NULL)
					return false;
			}
			// recursively to set child
			for (size_t i = 0; i < n + 1; i++)
			{
				// input address??????
				if (readNode(fp, fap, i) == false)
					return false;
			}
		}
		// LeafNode
		else 
		{
			Node *chp = new LeafNode();
			chp->setNum(n);
			fap->setChild(pos, chp);
			if (flag == 0)
			{
				flag = 1;
				fap = chp;
			}
			// loop to read key
			for (size_t i = 0; i < n; i++)
			{
				if (fread((void*)(chp->getKey(i)), sizeof(KeyType), 1, fp) == NULL)
					return false;
			}
			// loop to read data
			for (size_t i = 0; i < n; i++)
			{
				if (fread((void*)(chp->getData(i)), sizeof(DataType), 1, fp) == NULL)
					return false;
			}
			// set prev next
		}
	}
	return true;
}

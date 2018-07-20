#include "BPTree.h"



void BPTree::release(Node * _np) const
{
	if (_np == NULL)
	{
		return;
	}
	if (_np->isLeaf())
	{
		delete _np;
		return;
	}
	int cnt = _np->getNum();
	for (; cnt >= 0; --cnt)
		release(_np->getChild(cnt));
	delete _np;
	return;
}

BPTree::BPTree()
{
	root = NULL;
	leaves_head = NULL;
	leaves_tail = NULL;
	initFile();
}


BPTree::~BPTree()
{
	release(root);
}

Node * BPTree::getRoot() const
{
	return root;
}
Node * BPTree::getHead() const
{
	return leaves_head;
}
Node * BPTree::getTail() const
{
	return leaves_tail;
}
/**
* @fun: 查询key索引的data值
* @_key: 查询数据的key值
* @_data: 将要查找的data返回该指针
* @return：空树或没找到key则返回false
* 1）如果是空树返回false
* 2）从根节点循环p直到叶节点
* 3）提取叶节点相等key块的data值
*/
bool BPTree::search(const KeyType * _key, DataType * _data)
{
	Node *p = root;
	int i, pNum;
	// empty tree
	bool ifFind = false;
	if (root == NULL)
	{
		_data = NULL;
		return false;
	}
	// loop to leaves
	while (!p->isLeaf())
	{
		pNum = p->getNum();
		for (i = 0; i < pNum; i++)
			if (*_key < *(p->getKey(i)))
				break;
		p = p->getChild(i);
	}
	// p arrives at leaves
	pNum = p->getNum();
	for (i = 0; i < pNum; i++)
	{
		if (*_key == *(p->getKey(i)))
		{
			*_data = *(p->getData(i));
			ifFind = true;
			break;
		}
	}
	return ifFind;
}
/**
* @fun: 插入data值到key存储节点的位置
* @_key: key的值
* @_data: 将要插入的data值
* @return：成功的标识
* 1）如果是空树，则初始化tail/head/root
* 2）如果根节点key数==MAX_KEY_NUM，新建父节点，建立父子关系，分裂，如果rp是叶节点结尾，设置tail
* 3）从根节点循环p直到叶节点（如果过程中的子代key数目==MAX_KEY_NUM，分裂,
*	 如果rp是叶节点结尾，设置tail）,不分裂可直接继续循环
* 4）将键/值插入叶节点的相应区域
*/
bool BPTree::insert(const KeyType * _key, const DataType * _data)
{
	Node* ret;
	if (root == NULL)
	{
		leaves_tail = leaves_head = root = new LeafNode();
	}
	if (root->getNum() == Node::MAX_KEY_NUM)
	{
		Node* fp = new InNode();
		fp->addChild(0, root);
		ret = root->split(fp, 0);
		if (ret->isLeaf() && ret->getNext() == NULL)
			leaves_tail = ret;
		root = fp;
	}
	Node *p = this->root;
	// 在循环外声明变量
	int pNum, i;
	Node *q;
	while (!p->isLeaf())
	{
		pNum = p->getNum();
		for (i = 0; i < pNum; i++)
		{
			if (*_key < *(p->getKey(i)))
				break;
		}
		// 找到子代
		q = p->getChild(i);
		// 分裂子代
		if (q->getNum() == Node::MAX_KEY_NUM)
		{
			ret = q->split(p, i);
			if (ret->isLeaf() && ret->getNext() == NULL)
				this->leaves_tail = ret;
			if (*_key < *(p->getKey(i)))
				p = q;
			else
				p = ret;
		}
		// 不分裂
		else
			p = q;
	}

	// insert
	pNum = p->getNum();
	for (i = 0; i < pNum; i++)
		if (*_key < *(p->getKey(i)))
			break;
	// 判断是否有重复插入的键
	if (i > 0 && *_key == *(p->getKey(i - 1)))
		return false;
	else
	{
		p->addKey(i, _key);
		p->addData(i, _data);
		p->addNum();
		return true;
	}
}
/**
* @fun: 修改key索引的data值
* @_key: 查询数据的key值
* @_data: 将要修改成的data值
* @return：空树/没有该键返回false
* 1）如果是空树返回false
* 2）从根节点循环p直到叶节点
* 3）修改叶节点相等key块的data值
*/
bool BPTree::modify(const KeyType * _key, const DataType * _data)
{
	Node *p = root;
	int i, pNum;
	bool ifModified = false;
	// empty tree
	if (root == NULL)
		return false;

	// loop to leaves
	while (!p->isLeaf())
	{
		pNum = p->getNum();
		for (i = 0; i < pNum; i++)
			if (*_key < *(p->getKey(i)))
				break;
		p = p->getChild(i);
	}
	// p arrives at leaves
	pNum = p->getNum();
	for (i = 0; i < pNum; i++)
	{
		if (*_key == *(p->getKey(i)))
		{
			p->setData(i, _data);
			ifModified = true;
			break;
		}
	}
	return ifModified;
}
/**
* @fun: 将key存储节点的位置的块删除
* @_key: key的值
* @return：空树/key不存在 返回false
* 1）如果是空树，则初始化tail/head/root
* 2）如果根节点key数==MIN_KEY_NUM，合并节点，如果是叶节点重置head/tail
* 3）从根节点循环p直到叶节点（如果过程中的子代key数目==MAX_KEY_NUM，分裂,
*	 如果rp是叶节点结尾，设置tail）,不分裂可直接继续循环
* 4）将键/值插入叶节点的相应区域
*/
bool BPTree::remove(const KeyType * _key)
{	
	bool ifRemoved = false;
	Node *p, *q, *ret;
	int i, pNum;
	//tree is empty
	if (this->root == NULL)
		return false;
	p = this->root;
	// loop to leaves
	while (!p->isLeaf())
	{
		pNum = p->getNum();
		for (i = 0; i < pNum; i++)
			if (*_key < *(p->getKey(i)))
				break;
		// set child
		q = p->getChild(i);
		// merge
		if (q->getNum() == Node::MIN_KEY_NUM)
		{
			ret = q->merge(p, i);
			// leaf => set head/tail
			if (q->isLeaf())
			{
				if (q->getPrev() == NULL)
					this->leaves_head = q;
				if (q->getNext() == NULL)
					this->leaves_tail = q;
			}
		}
		//root shrinks
		if (p->getNum() == 0)
			this->root = q;
		p = q;
	}
	// reach leaves
	pNum = p->getNum();
	for (i = 0; i < pNum; i++)
		if (*_key == *(p->getKey(i)))
		{
			p->subKey(i);
			p->subData(i);
			p->subNum();
			//root leaf 0 key
			if (p->getNum() == 0)
			{
				this->root = NULL;
				this->leaves_head = NULL;
				this->leaves_tail = NULL;
			}
			ifRemoved = true;
			break;
		}
	return ifRemoved;
}

bool BPTree::save()
{
	if (this->root->writeNode(this->root, this->fp))
	{
		cout << "succeed in saving bptree :)" << endl;
		return true;
	}
	cout << "failed to save" << endl;
	return false;
}

bool BPTree::buildTree()
{
	Node *p = new InNode();
	if (p->readNode(this->fp, this->root)) 
		return true;
	cout << "failed to build tree" << endl;
	return false;
}

// BFS
void BPTree::printTree() const
{
	if (this->root == NULL)
	{
		printf("empty tree\n");
		return;
	}
	// use queue to BFS
	queue<Node*> nodeQueue;
	nodeQueue.push(this->root);
	while (!nodeQueue.empty())
	{
		Node *p = nodeQueue.front();
		int num = p->getNum();
		p->printNode();
		nodeQueue.pop();
		if (!p->isLeaf())
			for (size_t i = 0; i < num + 1; i++)
			{
				Node *child = p->getChild(i);
				nodeQueue.push(child);
			}
	}
}

bool BPTree::initFile()
{
	// 未包含存储文件 create
	if (-1 == access("./bptree.bdb", F_OK))
	{
		if ((this->fp = fopen("bptree.bdb", "w")) == NULL) {
			cout << "error in file creation" << endl;
			return false;
		}
		cout << "succeed in creating file" << endl;
	}
	// 已包含存储文件 read
	else
	{
		if ((this->fp = fopen("bptree.bdb", "r+")) == NULL)
		{
			cout << "failed to open file" << endl;
			return false;
		}
		cout << "succeed in opening file" << endl;
	}
	return true;
}

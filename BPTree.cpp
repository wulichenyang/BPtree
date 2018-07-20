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
* @fun: ��ѯkey������dataֵ
* @_key: ��ѯ���ݵ�keyֵ
* @_data: ��Ҫ���ҵ�data���ظ�ָ��
* @return��������û�ҵ�key�򷵻�false
* 1������ǿ�������false
* 2���Ӹ��ڵ�ѭ��pֱ��Ҷ�ڵ�
* 3����ȡҶ�ڵ����key���dataֵ
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
* @fun: ����dataֵ��key�洢�ڵ��λ��
* @_key: key��ֵ
* @_data: ��Ҫ�����dataֵ
* @return���ɹ��ı�ʶ
* 1������ǿ��������ʼ��tail/head/root
* 2��������ڵ�key��==MAX_KEY_NUM���½����ڵ㣬�������ӹ�ϵ�����ѣ����rp��Ҷ�ڵ��β������tail
* 3���Ӹ��ڵ�ѭ��pֱ��Ҷ�ڵ㣨��������е��Ӵ�key��Ŀ==MAX_KEY_NUM������,
*	 ���rp��Ҷ�ڵ��β������tail��,�����ѿ�ֱ�Ӽ���ѭ��
* 4������/ֵ����Ҷ�ڵ����Ӧ����
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
	// ��ѭ������������
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
		// �ҵ��Ӵ�
		q = p->getChild(i);
		// �����Ӵ�
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
		// ������
		else
			p = q;
	}

	// insert
	pNum = p->getNum();
	for (i = 0; i < pNum; i++)
		if (*_key < *(p->getKey(i)))
			break;
	// �ж��Ƿ����ظ�����ļ�
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
* @fun: �޸�key������dataֵ
* @_key: ��ѯ���ݵ�keyֵ
* @_data: ��Ҫ�޸ĳɵ�dataֵ
* @return������/û�иü�����false
* 1������ǿ�������false
* 2���Ӹ��ڵ�ѭ��pֱ��Ҷ�ڵ�
* 3���޸�Ҷ�ڵ����key���dataֵ
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
* @fun: ��key�洢�ڵ��λ�õĿ�ɾ��
* @_key: key��ֵ
* @return������/key������ ����false
* 1������ǿ��������ʼ��tail/head/root
* 2��������ڵ�key��==MIN_KEY_NUM���ϲ��ڵ㣬�����Ҷ�ڵ�����head/tail
* 3���Ӹ��ڵ�ѭ��pֱ��Ҷ�ڵ㣨��������е��Ӵ�key��Ŀ==MAX_KEY_NUM������,
*	 ���rp��Ҷ�ڵ��β������tail��,�����ѿ�ֱ�Ӽ���ѭ��
* 4������/ֵ����Ҷ�ڵ����Ӧ����
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
	// δ�����洢�ļ� create
	if (-1 == access("./bptree.bdb", F_OK))
	{
		if ((this->fp = fopen("bptree.bdb", "w")) == NULL) {
			cout << "error in file creation" << endl;
			return false;
		}
		cout << "succeed in creating file" << endl;
	}
	// �Ѱ����洢�ļ� read
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

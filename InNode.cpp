#include "InNode.h"



InNode::InNode()
{
	nodeType = INTERNAL;
	allocChilds();
}


InNode::~InNode()
{
	release();
	free(childs);
}

void InNode::allocChilds()
{
	childs = (Node**)malloc(sizeof(Node*) * (MAX_CHILD_NUM));
	memset(childs, 0, sizeof(Node*) * MAX_CHILD_NUM);
}

Node * InNode::getChild(int _index) const
{
	int num = this->getNum();
	if (_index < 0 || _index > num)
	{
		printf("getChild error");
		return NULL;
	}
	return childs[_index];
}

bool InNode::setChild(int _index, Node * _child)
{
	int num = this->getNum();
	if (_index < 0 || _index > num)
	{
		printf("error in setChild: Invalid index ");
		return false;
	}
	this->childs[_index] = _child;
	return true;
}

bool InNode::addChild(int _index, Node * _child)
{
	int num = this->getNum();
	if (_index < 0 || _index > num + 1)
	{
		printf("error in addChild: Invalid index ");
		return false;
	}
	int i;
	for (i = num; i >= _index; --i)	//DEBUG: right bounder!!!
		childs[i + 1] = childs[i];
	childs[_index] = _child;
	return true;
}

bool InNode::subChild(int _index)
{
	int num = this->getNum();
	if (_index < 0 || _index > num)
	{
		printf("error in subchild: Invalid index ");
		return false;
	}
	int i;
	for (i = _index; i < num; ++i) //DEBUG: right bounder!!!
		childs[i] = childs[i + 1];
	return true;
}
/**
* @fun: �ýڵ㽫����key��δ�ɹ���ʱ��keys == MAX_KEY_NUM, ����
* @_father: �����ӽڵ�ĸ��ڵ�
* @_index: �����ڸ��׽ڵ��е�λ��
* @return���½����������ڵ�
* 1���½�һ���������ڵ�
* 2����С��������������ת�Ƶ�������
* 3��������������MIN_KEY_NUM��keys
* 3������index��������������������һ��key��pos == MIN_KEY_NUM������ָ���ҽڵ�
*/
Node * InNode::split(Node * _father, int _index) // DEBUG
{
	Node* p = new InNode(); // right child
	int num = this->getNum();
	p->setNum(MIN_KEY_NUM);
	int i, j;
	// ��child��벿�ֵ�key�������½ڵ� 
	for (i = MIN_CHILD_NUM, j = 0; i < num; i++, j++)
	{
		p->setKey(j, keys + i);
		// ���child����Ҷ�ӣ�����Ҫ��ָ�뿽��ȥ��ָ��Ƚڵ��1
		p->setChild(j, childs[i]);
	}
	p->setChild(j, childs[i]);

	this->setNum(MIN_KEY_NUM);
	// child���м�ڵ���Ҫ����parent��index��������parent��key��pointer
	// ������ΪʲôҪ��ȡ����
	const DataType * key = this->keys + MIN_KEY_NUM;
	_father->addKey(_index, key);
	//error ����NULL//_father->addKey(_index, this->getKey(MIN_KEY_NUM)); 
	_father->addChild(_index + 1, p);
	_father->addNum();
	return p;
}
/**
* @fun: �ýڵ㽫ɾ��key���h��ǰ��ʱ��keys == MIN_KEY_NUM, �ϲ�
* @_father: �ϲ��ӽڵ�ĸ��ڵ�
* @_index: �����ڸ��׽ڵ��е�λ��
* @return���ϲ���������ڵ��NULL
* 1�����ҽڵ��������ж��ҽڵ���Ŀ
* 2������ڵ��������ж���ڵ���Ŀ
* 3��������Ӧswitch
* ���ȼ�����1����1�������У������У��ϲ���
* case 1:�ϲ�������
*	   2:ժȡ������
*      3:�ϲ�������
*      4:ժȡ������
*/
Node * InNode::merge(Node * _father, int _index) // DEBUG
{
	// �ֵ�ָ��
	Node *p;
	// switch
	int sCase = 0;
	int pNum, tNum, lNum;
	int fNum = _father->getNum();
	// ��������(�ƶ�һ�������ȼ���)
	if (_index < fNum) 
	{
		p = _father->getChild(_index + 1);
		if (p->getNum() > MIN_KEY_NUM)
			// һ��
			sCase = 2;
		else
			// ȫ��
			sCase = 1;
	}
	// ��������
	if (_index > 0)
	{
		Node *tp = _father->getChild(_index - 1);
		if (sCase != 2)
		{
			if (p->getNum() > MIN_KEY_NUM)
				sCase = 4;
			else if (sCase != 1)// 0
				sCase = 3;
		}
		// ѡ����ȷ���ֵܽڵ�
		if (sCase > 2)
		{
			p = tp;
		}
	}
	switch (sCase)
	{
	int i, j;
	case 1:// union the right to this 
		   // �ȰѸ��ڵ��е�index��key����������
		   // ���ڵ������key��num
		   // ��ֱ�Ӹ���������������key��child�����(��һ��child)
		   // num���ͷ��ҽڵ�
		pNum = p->getNum();
		this->setKey(MIN_KEY_NUM, _father->getKey(_index));
		this->addNum();

		// ������ڵ��е�index key
		_father->subKey(_index);
		_father->subChild(_index + 1);
		_father->subNum();

		for (i = 0, j = MIN_CHILD_NUM; i < pNum; i++)
		{
			this->setKey(j, p->getKey(i));
			this->setChild(j, p->getChild(i));
		}
		this->setChild(j, p->getChild(i));
		this->setNum(this->getNum() + pNum);

		p->setNum(0);
		delete p;
		break;
	case 2:// move one form right 
		   // ���ƶ�����_index key�����,p��child����ߣ�
		   // ���ƶ��ұ�first_Key������
		   // �����ڵ�����num
		tNum = this->getNum();
		this->setKey(tNum, _father->getKey(_index));
		this->setChild(tNum + 1, p->getChild(0));
		this->addNum();

		_father->setKey(_index, p->getKey(0));

		p->subKey(0);
		p->subChild(0);
		p->subNum();
		break;
	case 3:// union left to this  
		   // �Ƚ����ڵ�index-1��key�������������ٽ�������ͷ�巨����������
		lNum = p->getNum();
		tNum = this->getNum();
		this->addKey(0,_father->getKey(_index - 1));

		for (i = lNum - 1; i >= 0; i--)
		{
			this->addKey(0, p->getKey(i));
			this->addChild(0, p->getChild(i + 1));
		}
		this->addChild(0, p->getChild(0));
		this->setNum(tNum + 1 + lNum);
		
		_father->subKey(_index - 1);
		_father->subChild(_index - 1);
		_father->subNum();
		p->setNum(0);
		delete p;
		break;
	case 4:// move one from left 
		   // �Ƚ����ڵ�index-1 �Ƹ��������������������ֵ�������ڵ㣬
		   // �����������������������
		lNum = p->getNum();
		
		this->addKey(0, _father->getKey(_index - 1));
		this->addChild(0, p->getChild(lNum));
		this->addNum();

		_father->setKey(_index - 1, p->getKey(lNum - 1));
		
		p->subKey(lNum - 1);
		p->subChild(lNum);
		p->subNum();
		break;
	default:
		printf("error in coalesce: Invalid case!");
		break;
	}
	if (sCase == 1 || sCase == 3)
		return p;
	else
		return NULL;
}

void InNode::release()
{
	delete[] keys;
}

void InNode::printNode() const
{

	int num = this->getNum();
	for (size_t i = 0; i < num; i++)
	{
		if (i == 0)
			cout << "[ ";
		cout << *(this->getKey(i)) << " ";
		if (i == num - 1)
			cout << "] " << endl;;
	}
}

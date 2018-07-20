#include "LeafNode.h"



void LeafNode::allocData()
{
	data = new DataType[MAX_KEY_NUM];// data��Ŀ���ڼ���
}

LeafNode::LeafNode()
{
	nodeType = LEAF;
	prev = next = NULL;
	allocData();
}


LeafNode::~LeafNode()
{
	release();
	prev = next = NULL;
}

Node * LeafNode::getPrev() const
{
	return prev;
}

Node * LeafNode::getNext() const
{
	return next;
}

void LeafNode::setPrev(Node * _prev)
{
	prev = _prev;
}

void LeafNode::setNext(Node * _next)
{
	next = _next;
}

const DataType * LeafNode::getData(int _index) const
{
	int num = this->getNum();
	if (_index < 0 || _index > num - 1)
	{
		printf("error in getData");
		return NULL;
	}
	else
		return data + _index;
}

bool LeafNode::setData(int _index, const DataType * _data)
{
	int num = this->getNum();
	if (_index < 0 || _index > num - 1)
	{
		printf("error in setData:%d\n", *_data);
		return false;
	}
	else
		this->data[_index] = *_data;
	return true;
}

bool LeafNode::addData(int _index, const DataType * _data)
{
	int num = this->getNum();
	if (_index < 0 || _index > num)
	{
		printf("error in addValue: Invalid index ");
		return false;
	}
	int i;
	for (i = num - 1; i >= _index; --i)
		this->data[i + 1] = this->data[i];
	this->data[_index] = *_data;
	return true;
}

bool LeafNode::subData(int _index)
{
	int num = this->getNum();
	if (_index < 0 || _index >= num)
	{
		printf("error in subValue: Invalid index ");
		return false;
	}
	int i;
	for (i = _index; i < num - 1; ++i)
		this->data[i] = this->data[i + 1];
	return true;
}
/**
* @fun: �ýڵ㽫����key��δ�ɹ���ʱ��keys == MAX_KEY_NUM, ����
* @_father: �����ӽڵ�ĸ��ڵ�
* @_index: �����ڸ��׽ڵ��е�λ��
* @return���½���������Ҷ�ڵ�
* 1���½�һ��������Ҷ�ڵ�
* 2����������Ҷ�ڵ�
* 3����С��������������ת�Ƶ�������Ҷ�ڵ㣨keys��data��
* 4��������������MIN_KEY_NUM��keys
* 5������index������������Ҷ�ڵ��������һ��key��pos == MIN_KEY_NUM������ָ����Ҷ�ڵ�
*/
Node * LeafNode::split(Node * _father, int _index)
{
	Node* p = new LeafNode(); // right child
	int num = this->getNum();
	p->setNum(MIN_CHILD_NUM);
	p->setPrev(this);
	p->setNext(this->getNext());
	this->setNext(p);

	int i, j;
	// ��child��벿�ֵ�key/data��������Ҷ�ڵ� 
	for (i = MIN_KEY_NUM, j = 0; i < num; i++, j++)
	{

		p->setKey(j, keys + i);
		// ��Ҷ�ӣ�ֻ��Ҫ��data����ȥ����Ŀ��keys���
		p->setData(j, data + i);
	}

	this->setNum(MIN_KEY_NUM);
	// child�ķ��ѽڵ���Ҫ����parent��index��������parent��key��pointer
	const KeyType * key = this->keys + MIN_KEY_NUM;
	_father->addKey(_index, key);
	_father->addChild(_index + 1, p);
	_father->addNum();
	return p;
}
/**
* @fun: �ýڵ㽫ɾ��key���h��ǰ��ʱ��keys == MIN_KEY_NUM, �ϲ�����Ҷ�ӽڵ�/
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
Node * LeafNode::merge(Node * _father, int _index)
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
		   // �����ƶ����ڵ�
		   // ���ڵ������key��num
		   // ��ֱ�Ӹ���������������key��data�����
		   // ����next/prev
		   // num���ͷ��ҽڵ�
		pNum = p->getNum();

		// ������ڵ��е�index key
		_father->subKey(_index);
		_father->subChild(_index + 1);
		_father->subNum();

		for (i = 0, j = MIN_CHILD_NUM; i < pNum; i++)
		{
			this->setKey(j, p->getKey(i));
			this->setData(j, p->getData(i));
		}
		// next
		if (p->getNext() == NULL)
			this->setNext(NULL);
		else
		{
			this->setNext(p->getNext());
			p->getNext()->setPrev(this);
		}
		this->setNum(this->getNum() + pNum);
		p->setNum(0);
		delete p;
		break;
	case 2:// move one form right 
		   // p��key/data�����һ��
		   // �������ұ�secont_Key�����ף�Ҷ�ڵ��һ��key���ڵ��У�
		   // �����ڵ�����num
		tNum = this->getNum();
		this->addNum();
		this->setKey(tNum, _father->getKey(_index));
		this->setData(tNum, p->getData(0));

		_father->setKey(_index, p->getKey(1));

		p->subKey(0);
		p->subData(0);
		p->subNum();
		break;
	case 3:// union left to this 
		   // �ٽ�������ͷ�巨����������
		   // ɾ�����ڵ��index-1��key��child
		   // ����num������prev/next
		   // ɾ���ڵ�p
		lNum = p->getNum();
		tNum = this->getNum();

		for (i = lNum - 1; i >= 0; i--)
		{
			this->addKey(0, p->getKey(i));
			this->addData(0, p->getData(i));
		}
		this->setNum(tNum + lNum);

		_father->subKey(_index - 1);
		_father->subChild(_index - 1);
		_father->subNum();

		if (p->getPrev() == NULL)
			this->setPrev(NULL);
		else
		{
			p->getPrev()->setNext(this);
			this->setPrev(p->getPrev());
		}
		p->setNum(0);
		delete p;
		break;
	case 4:// move one from left 
		   // �����������key/dataͷ������Ҷ�ڵ㣬
		   // ɾ������������key/data
		   // ���ø��ڵ�index-1����keyΪthis��firstKey
		   // ����num
		lNum = p->getNum();

		this->addKey(0, p->getKey(lNum - 1));
		this->addData(0, p->getData(lNum - 1));
		this->addNum();

		_father->setKey(_index - 1, this->getKey(0));

		p->subKey(lNum - 1);
		p->subData(lNum - 1 );
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

void LeafNode::release()
{
	delete []keys;
	delete []data;
}

void LeafNode::printNode() const
{
	int num = this->getNum();
	for (int i = 0; i < num; i++)
	{
		if (i == 0)
			cout << "[ ";
		cout << "   k: " << *(this->getKey(i)) << " ";
		cout << "d: " << *(this->getData(i)) << "    ";
		if (i == num - 1)
			cout << "] " << endl;
	}
}

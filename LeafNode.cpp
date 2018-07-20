#include "LeafNode.h"



void LeafNode::allocData()
{
	data = new DataType[MAX_KEY_NUM];// data数目等于键数
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
* @fun: 该节点将插入key（未成功）时，keys == MAX_KEY_NUM, 分裂
* @_father: 分裂子节点的父节点
* @_index: 子树在父亲节点中的位置
* @return：新建的右子树叶节点
* 1）新建一个右子树叶节点
* 2）连接两个叶节点
* 3）最小子树数超出部分转移到右子树叶节点（keys和data）
* 4）两个子树保留MIN_KEY_NUM个keys
* 5）父节index处插入左子树叶节点多余的最后一个key（pos == MIN_KEY_NUM），并指向右叶节点
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
	// 将child后半部分的key/data拷贝给新叶节点 
	for (i = MIN_KEY_NUM, j = 0; i < num; i++, j++)
	{

		p->setKey(j, keys + i);
		// 是叶子，只需要把data拷过去，数目和keys相等
		p->setData(j, data + i);
	}

	this->setNum(MIN_KEY_NUM);
	// child的分裂节点需要插入parent的index处，更新parent的key和pointer
	const KeyType * key = this->keys + MIN_KEY_NUM;
	_father->addKey(_index, key);
	_father->addChild(_index + 1, p);
	_father->addNum();
	return p;
}
/**
* @fun: 该节点将删除key（刪除前）时，keys == MIN_KEY_NUM, 合并两个叶子节点/
* @_father: 合并子节点的父节点
* @_index: 子树在父亲节点中的位置
* @return：合并后的子树节点或NULL
* 1）有右节点子树，判断右节点数目
* 2）有左节点子树，判断左节点数目
* 3）进入相应switch
* 优先级：右1，左1，右所有，左所有（合并）
* case 1:合并右子树
*	   2:摘取右子树
*      3:合并左子树
*      4:摘取左子树
*/
Node * LeafNode::merge(Node * _father, int _index)
{
	// 兄弟指针
	Node *p;
	// switch
	int sCase = 0;
	int pNum, tNum, lNum;
	int fNum = _father->getNum();
	// 有右子树(移动一个的优先级高)
	if (_index < fNum)
	{
		p = _father->getChild(_index + 1);
		if (p->getNum() > MIN_KEY_NUM)
			// 一个
			sCase = 2;
		else
			// 全部
			sCase = 1;
	}
	// 有左子树
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
		// 选择正确的兄弟节点
		if (sCase > 2)
		{
			p = tp;
		}
	}
	switch (sCase)
	{
	int i, j;
	case 1:// union the right to this 
		   // 不用移动父节点
		   // 父节点清除该key，num
		   // 再直接复制所有右子树的key和data到左边
		   // 设置next/prev
		   // num和释放右节点
		pNum = p->getNum();

		// 清除父节点中的index key
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
		   // p的key/data给左边一个
		   // 再设置右边secont_Key给父亲（叶节点第一个key父节点有）
		   // 两个节点增减num
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
		   // 再将左子树头插法插入右子树
		   // 删除父节点的index-1处key和child
		   // 整理num和设置prev/next
		   // 删除节点p
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
		   // 把左子树最大key/data头插入右叶节点，
		   // 删除左子树最后的key/data
		   // 设置父节点index-1处的key为this的firstKey
		   // 设置num
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

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
* @fun: 该节点将插入key（未成功）时，keys == MAX_KEY_NUM, 分裂
* @_father: 分裂子节点的父节点
* @_index: 子树在父亲节点中的位置
* @return：新建的右子树节点
* 1）新建一个右子树节点
* 2）最小子树数超出部分转移到右子树
* 3）两个子树保留MIN_KEY_NUM个keys
* 3）父节index处插入左子树多余的最后一个key（pos == MIN_KEY_NUM），并指向右节点
*/
Node * InNode::split(Node * _father, int _index) // DEBUG
{
	Node* p = new InNode(); // right child
	int num = this->getNum();
	p->setNum(MIN_KEY_NUM);
	int i, j;
	// 将child后半部分的key拷贝给新节点 
	for (i = MIN_CHILD_NUM, j = 0; i < num; i++, j++)
	{
		p->setKey(j, keys + i);
		// 如果child不是叶子，还需要把指针拷过去，指针比节点多1
		p->setChild(j, childs[i]);
	}
	p->setChild(j, childs[i]);

	this->setNum(MIN_KEY_NUM);
	// child的中间节点需要插入parent的index处，更新parent的key和pointer
	// ？？？为什么要提取出来
	const DataType * key = this->keys + MIN_KEY_NUM;
	_father->addKey(_index, key);
	//error 返回NULL//_father->addKey(_index, this->getKey(MIN_KEY_NUM)); 
	_father->addChild(_index + 1, p);
	_father->addNum();
	return p;
}
/**
* @fun: 该节点将删除key（刪除前）时，keys == MIN_KEY_NUM, 合并
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
Node * InNode::merge(Node * _father, int _index) // DEBUG
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
		   // 先把父节点中的index处key拉到左子树
		   // 父节点清除该key，num
		   // 再直接复制所有右子树的key和child到左边(多一个child)
		   // num和释放右节点
		pNum = p->getNum();
		this->setKey(MIN_KEY_NUM, _father->getKey(_index));
		this->addNum();

		// 清除父节点中的index key
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
		   // 先移动父亲_index key给左边,p的child给左边，
		   // 再移动右边first_Key给父亲
		   // 两个节点增减num
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
		   // 先将父节点index-1处key插入右子树，再将左子树头插法插入右子树
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
		   // 先将父节点index-1 移给右子树，把左子树最大值赋给父节点，
		   // 左子树最后子树赋给右子树
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

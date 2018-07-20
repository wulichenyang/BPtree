#pragma once
#ifndef _NODE_H
#define _NODE_H

#include "Util.h"

typedef int KeyType;
typedef int DataType;
enum NODE_TYPE{INTERNAL, LEAF};

class InNode;
class LeafNode;

class Node
{
public:
	static const unsigned DEGREE = 7 * 2; // 树的度数
	static const unsigned MIN_CHILD_NUM = DEGREE >> 1; // 最小最大子树数
	static const unsigned MAX_CHILD_NUM = DEGREE;
	static const unsigned MIN_KEY_NUM = MIN_CHILD_NUM - 1; // 最小最大键数
	static const unsigned MAX_KEY_NUM = MAX_CHILD_NUM - 1;
protected:
	NODE_TYPE nodeType;
	int keyNum;
	KeyType *keys;
	void initKeys();
public:
	Node();
	virtual ~Node();
	bool isLeaf();
	int getNum() const; // key num
	bool setNum(int _num);
	bool addNum();
	bool subNum();
	const KeyType *getKey(int _index) const;
	bool setKey(int _index, const KeyType *key);
	bool addKey(int _index, const KeyType *key);
	bool subKey(int _index);
	// virtual
	virtual Node* getChild(int _index) const { return NULL; };
	virtual bool setChild(int _index, Node* _child) { return true; };
	virtual bool addChild(int _index, Node* _child) { return true; };
	virtual bool subChild(int _index) { return true; };

	virtual Node* getPrev() const { return NULL; };
	virtual Node* getNext() const { return NULL; };
	virtual void setPrev(Node* _prev) {};
	virtual void setNext(Node* _next) {};

	virtual const DataType* getData(int _index) const { return NULL; };
	virtual bool setData(int _index, const DataType* _data) { return true; };
	virtual bool addData(int _index, const DataType* _data) { return true; };
	virtual bool subData(int _index) { return true; };

	// read/write Node
	virtual bool writeNode(Node* p, FILE *fp);
	virtual bool readNode(FILE *fp, Node *&fap, int pos = 0);
	
	// pure virtual -> you must override
	virtual Node * split(Node* _father, int _index) = 0;
	virtual Node * merge(Node* _father, int _index) = 0;
	virtual void release() = 0;
	virtual void printNode() const = 0;
};

#endif // !_NODE_H

#pragma once
#ifndef _LEAFNODE_H
#define _LEAFNODE_H

#include "Util.h"
#include "Node.h"

class LeafNode : public Node
{
protected:
	Node * prev;
	Node * next;
	DataType * data;
	void allocData();
public:
	LeafNode();
	~LeafNode();
	Node * getPrev() const;
	Node * getNext() const;
	void setPrev( Node* _prev);
	void setNext( Node* _next);
	const DataType * getData(int _index) const;
	bool setData(int _index, const DataType * _data);
	bool addData(int _index, const DataType * _data);
	bool subData(int _index);
	Node* split(Node * _father, int _index);
	Node* merge(Node * _father, int _index);
	void release();
	void printNode() const;
};

#endif
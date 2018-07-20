#pragma once
#ifndef _INNODE_H
#define _INNODE_H
#include "Util.h"
#include "Node.h"

class InNode : public Node
{
public:
	InNode();
	~InNode();

	void allocChilds();
	Node* getChild(int _index) const;
	bool setChild(int _index, Node* _child);
	bool addChild(int _index, Node* _child);
	bool subChild(int _index);

	Node* split(Node* _father, int _index); // index of child;
	Node* merge(Node* _father, int _index);
	void release();
	void printNode() const;
protected:
	Node ** childs;
};

#endif
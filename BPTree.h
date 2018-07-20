#pragma once
#ifndef _BPTREE_H
#define _BPTREE_H

#include "Util.h"
#include "LeafNode.h"
#include "InNode.h"

class BPTree
{
private:
	Node* root;
	Node* leaves_head; //the head of LeafNode-list
	Node* leaves_tail; //the tail of LeafNode-list
	// É¾³ýÄ³¸ö½Úµã
	void release(Node* _np) const;
	FILE *fp;
public:
	BPTree();
	~BPTree();
	Node *getRoot() const;
	Node *getHead() const;
	Node *getTail() const;
	bool search(const KeyType * _key, DataType *_data);
	bool insert(const KeyType * _key, const DataType *_data);
	bool modify(const KeyType * _key, const DataType *_data);
	bool remove(const KeyType * _key);
	// write into disk
	bool save();
	// read from disk
	bool buildTree();
	void printTree() const;
	// open or create a file
	bool initFile();
};

#endif
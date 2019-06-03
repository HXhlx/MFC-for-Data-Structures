#pragma once
#ifndef TREE
#define TREE
#include <fstream>
typedef char DataType;
typedef struct BiTNode
{
	int adress, level;
	DataType data;			//结点数据
	struct BiTNode* lchild; //左孩子指针
	struct BiTNode* rchild; //右孩子指针
} BiTNode, * BiTree;
void Preorder(BiTree T), Inorder(BiTree T), Postorder(BiTree T), CreateBt(BiTree& T, int level = 1), Close(), BiTreeDepth(BiTree T, int level, int& depth), Open();
extern std::ifstream is;
#endif // !TREE
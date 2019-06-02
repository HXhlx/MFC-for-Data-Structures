#pragma once
#ifndef TREE
#define TREE
#include <fstream>
typedef char DataType;
typedef struct BiTNode
{
	DataType data;			//结点数据
	struct BiTNode* lchild; //左孩子指针
	struct BiTNode* rchild; //右孩子指针
} BiTNode, * BiTree;
void Preorder(BiTree T, int level), Inorder(BiTree T), Postorder(BiTree T), CreateBt(BiTree& T), Close(), BiTreeDepth(BiTree T, int level, int& depth), Restore();
extern std::ifstream is;
#endif // !TREE
#pragma once
#ifndef TREE
#define TREE
#define BiTNode Tree
#define BiTree Tree*
typedef char DataType;
class Tree
{
	DataType data;			//结点数据
	BiTNode* lchild; //左孩子指针
	BiTNode* rchild; //右孩子指针
public:
	void Preorder(BiTree T);
	void Inorder(BiTree T);
	void Postorder(BiTree T);
	static void CreateBt(BiTree& T);
};
#endif // !TREE
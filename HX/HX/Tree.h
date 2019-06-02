#pragma once
#define BiTNode Tree
#define BiTree Tree*
typedef char DataType;
class Tree
{
	DataType data;			//结点数据
	struct BiTNode* lchild; //左孩子指针
	struct BiTNode* rchild; //右孩子指针
	void Preorder(BiTree T);
	void Inorder(BiTree T);
	void Postorder(BiTree T);
	void CreateBt(BiTree& T);
};
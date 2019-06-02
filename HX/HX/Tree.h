#pragma once
#ifndef TREE
#define TREE
#define BiTNode Tree
#define BiTree Tree*
typedef char DataType;
class Tree
{
	DataType data;			//�������
	BiTNode* lchild; //����ָ��
	BiTNode* rchild; //�Һ���ָ��
public:
	void Preorder(BiTree T);
	void Inorder(BiTree T);
	void Postorder(BiTree T);
	static void CreateBt(BiTree& T);
};
#endif // !TREE
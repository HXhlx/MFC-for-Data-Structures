#pragma once
#define BiTNode Tree
#define BiTree Tree*
typedef char DataType;
class Tree
{
	DataType data;			//�������
	struct BiTNode* lchild; //����ָ��
	struct BiTNode* rchild; //�Һ���ָ��
	void Preorder(BiTree T);
	void Inorder(BiTree T);
	void Postorder(BiTree T);
	void CreateBt(BiTree& T);
};
#pragma once
#ifndef TREE
#define TREE
#include <fstream>
typedef char DataType;
typedef struct BiTNode
{
	//BiTNode();
	DataType data;			//�������
	struct BiTNode* lchild; //����ָ��
	struct BiTNode* rchild; //�Һ���ָ��
} BiTNode, * BiTree;
void Preorder(BiTree T), Inorder(BiTree T), Postorder(BiTree T), CreateBt(BiTree& T), Close(), BiTreeDepth(BiTree T, int level, int& depth), Open();
extern std::ifstream is;
#endif // !TREE
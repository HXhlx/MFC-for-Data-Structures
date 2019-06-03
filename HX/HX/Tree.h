#pragma once
#ifndef TREE
#define TREE
#include <fstream>
typedef char DataType;
typedef struct BiTNode
{
	int adress, level;
	DataType data;			//�������
	struct BiTNode* lchild; //����ָ��
	struct BiTNode* rchild; //�Һ���ָ��
} BiTNode, * BiTree;
void Preorder(BiTree T), Inorder(BiTree T), Postorder(BiTree T), CreateBt(BiTree& T, int level = 1), Close(), BiTreeDepth(BiTree T, int level, int& depth), Open();
extern std::ifstream is;
#endif // !TREE
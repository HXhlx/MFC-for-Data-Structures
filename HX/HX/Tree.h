#pragma once
#ifndef TREE
#define TREE
#include <fstream>
typedef char DataType;
typedef struct BiTNode
{
	DataType data;			//�������
	struct BiTNode* lchild; //����ָ��
	struct BiTNode* rchild; //�Һ���ָ��
} BiTNode, * BiTree;
void Preorder(BiTree T, int level), Inorder(BiTree T), Postorder(BiTree T), CreateBt(BiTree& T), Close(), BiTreeDepth(BiTree T, int level, int& depth), Restore();
extern std::ifstream is;
#endif // !TREE
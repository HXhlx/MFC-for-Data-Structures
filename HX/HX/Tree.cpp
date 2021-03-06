#include "stdafx.h"
#include "Tree.h"
#include <stdio.h>
#include <stdlib.h>
std::ifstream is;
void Preorder(BiTree T)
{
	if (T)
	{
		printf("%c ", T->data);
		Preorder(T->lchild);
		Preorder(T->rchild);
	}
}

void Inorder(BiTree T)
{
	if (T)
	{
		Inorder(T->lchild);
		printf("%c ", T->data);
		Inorder(T->rchild);
	}
}

void Postorder(BiTree T)
{
	if (T)
	{
		Postorder(T->lchild);
		Postorder(T->rchild);
		printf("%c ", T->data);
	}
}

//

void CreateBt(BiTree& T, int level)
{
	char ch;
	is >> ch;
	if (ch == '#')
		T = NULL;
	else
	{
		T = (BiTree)malloc(sizeof(BiTNode));
		T->data = ch;
		T->adress = 1;
		T->level = level;
		CreateBt(T->lchild, level + 1);
		CreateBt(T->rchild, level + 1);
	}
}

void BiTreeDepth(BiTree T, int level, int& depth)
{
	if (T)
	{
		if (level > depth)depth = level;
		BiTreeDepth(T->lchild, level + 1, depth);
		BiTreeDepth(T->rchild, level + 1, depth);
	}
}

void Close()
{
	is.close();
}

void Open()
{
	is.open("tree.txt");
}
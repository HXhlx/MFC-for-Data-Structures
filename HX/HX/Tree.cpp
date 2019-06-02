#include "StdAfx.h"
#include "Tree.h"
#include <stdio.h>
void Tree::Preorder(BiTree T)
{
	if (T)
	{
		printf("%c ", T->data);
		Preorder(T->lchild);
		Preorder(T->rchild);
	}
}

void Tree::Inorder(BiTree T)
{
	if (T)
	{
		Inorder(T->lchild);
		printf("%c ", T->data);
		Inorder(T->rchild);
	}
}

void Tree::Postorder(BiTree T)
{
	if (T)
	{
		Postorder(T->lchild);
		Postorder(T->rchild);
		printf("%c ", T->data);
	}
}

void Tree::CreateBt(BiTree& T)
{
	char ch;
	scanf("%c", &ch);
	if (ch == '#')
		T = NULL;
	else
	{
		T = (BiTree)malloc(sizeof(BiTNode));
		T->data = ch;
		CreateBt(T->lchild);
		CreateBt(T->rchild);
	}
}

#pragma once
#ifndef TREE_H
#define TREE_H

#include <memory>
#include <string>
#include <vector>

typedef wchar_t DataType;

struct BiTNode
{
    int address;
    int level;
    DataType data;
    std::unique_ptr<BiTNode> lchild;
    std::unique_ptr<BiTNode> rchild;

    BiTNode() : address(1), level(1), data(L'\0') {}
    BiTNode(DataType d, int lvl) : address(1), level(lvl), data(d) {}
};

using BiTreePtr = std::unique_ptr<BiTNode>;

BiTreePtr CreateBt(const std::wstring& input, size_t& index, int level = 1, int address = 1);
int BiTreeDepth(const BiTNode* node);

std::wstring GetTraversalCode(int type);
std::vector<std::wstring> GetTraversalSteps(const BiTNode* node, int type);
void CollectTraversalNodes(const BiTNode* node, int type, std::vector<const BiTNode*>& nodes);

#endif // TREE_H

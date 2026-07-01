#pragma once
#ifndef TREE_H
#define TREE_H

#include <memory>
#include <string>
#include <vector>

typedef wchar_t DataType;

enum class TraversalType : int
{
    Preorder  = 0,
    Inorder   = 1,
    Postorder = 2
};

struct BiTNode
{
    int address;
    int level;
    DataType data;
    std::unique_ptr<BiTNode> lchild;
    std::unique_ptr<BiTNode> rchild;
    BiTNode* thread; // For Morris traversal threading

    BiTNode() noexcept : address(1), level(1), data(L'\0'), thread(nullptr) {}
    BiTNode(DataType d, int lvl) noexcept : address(1), level(lvl), data(d), thread(nullptr) {}
};

using BiTreePtr = std::unique_ptr<BiTNode>;

[[nodiscard]] BiTreePtr CreateBt(const std::wstring& input, size_t& index, int level = 1, int address = 1);
[[nodiscard]] int BiTreeDepth(const BiTNode* node);

[[nodiscard]] std::wstring GetTraversalCode(TraversalType type);

// Recursive traversal (for step collection with NULL nodes)
[[nodiscard]] std::vector<std::wstring> GetTraversalSteps(const BiTNode* node, TraversalType type);
void CollectTraversalNodes(const BiTNode* node, TraversalType type, std::vector<const BiTNode*>& nodes);

// Morris Traversal - O(1) space, O(n) time
[[nodiscard]] std::vector<std::wstring> MorrisTraversal(const BiTNode* root, TraversalType type);
void MorrisTraversalWithNodes(const BiTNode* root, TraversalType type,
    std::vector<std::wstring>& steps, std::vector<const BiTNode*>& nodes);

#endif // TREE_H

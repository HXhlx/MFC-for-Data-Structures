#include "Tree.h"
#include <algorithm>

BiTreePtr CreateBt(const std::wstring& input, size_t& index, int level, int address)
{
    if (index >= input.size())
        return nullptr;

    wchar_t ch = input[index++];
    if (ch == L'#')
        return nullptr;

    auto node = std::make_unique<BiTNode>(ch, level);
    node->address = address;
    node->lchild = CreateBt(input, index, level + 1, 2 * address - 1);
    node->rchild = CreateBt(input, index, level + 1, 2 * address);
    return node;
}

int BiTreeDepth(const BiTNode* node)
{
    if (!node)
        return 0;
    int leftDepth = BiTreeDepth(node->lchild.get());
    int rightDepth = BiTreeDepth(node->rchild.get());
    return 1 + (leftDepth > rightDepth ? leftDepth : rightDepth);
}

std::wstring GetTraversalCode(int type)
{
    switch (type)
    {
    case 0: // Morris Preorder
        return L"Morris Preorder - O(1) space\n"
               L"while (curr) {\n"
               L"  if (!curr->left) {\n"
               L"    visit(curr);\n"
               L"    curr = curr->right;\n"
               L"  } else {\n"
               L"    pred = findPred(curr);\n"
               L"    if (!pred->right) {\n"
               L"      visit(curr);\n"
               L"      pred->right = curr;\n"
               L"      curr = curr->left;\n"
               L"    } else {\n"
               L"      pred->right = null;\n"
               L"      curr = curr->right;\n"
               L"    }\n"
               L"  }\n"
               L"}";
    case 1: // Morris Inorder
        return L"Morris Inorder - O(1) space\n"
               L"while (curr) {\n"
               L"  if (!curr->left) {\n"
               L"    visit(curr);\n"
               L"    curr = curr->right;\n"
               L"  } else {\n"
               L"    pred = findPred(curr);\n"
               L"    if (!pred->right) {\n"
               L"      pred->right = curr;\n"
               L"      curr = curr->left;\n"
               L"    } else {\n"
               L"      pred->right = null;\n"
               L"      visit(curr);\n"
               L"      curr = curr->right;\n"
               L"    }\n"
               L"  }\n"
               L"}";
    case 2: // Morris Postorder
        return L"Morris Postorder - O(1) space\n"
               L"// Mirrored preorder (NRL)\n"
               L"while (curr) {\n"
               L"  if (!curr->right) {\n"
               L"    visit(curr);\n"
               L"    curr = curr->left;\n"
               L"  } else {\n"
               L"    pred = findLeftmost(curr->right);\n"
               L"    if (!pred->left) {\n"
               L"      visit(curr);\n"
               L"      pred->left = curr;\n"
               L"      curr = curr->right;\n"
               L"    } else {\n"
               L"      pred->left = null;\n"
               L"      curr = curr->left;\n"
               L"    }\n"
               L"  }\n"
               L"}\n"
               L"reverse(result); // NRL -> LRN";
    default:
        return L"";
    }
}

static void PreorderSteps(const BiTNode* node, std::vector<std::wstring>& steps)
{
    if (!node)
    {
        steps.push_back(L"NULL");
        return;
    }
    steps.push_back(std::wstring(1, node->data));
    PreorderSteps(node->lchild.get(), steps);
    PreorderSteps(node->rchild.get(), steps);
}

static void InorderSteps(const BiTNode* node, std::vector<std::wstring>& steps)
{
    if (!node)
    {
        steps.push_back(L"NULL");
        return;
    }
    InorderSteps(node->lchild.get(), steps);
    steps.push_back(std::wstring(1, node->data));
    InorderSteps(node->rchild.get(), steps);
}

static void PostorderSteps(const BiTNode* node, std::vector<std::wstring>& steps)
{
    if (!node)
    {
        steps.push_back(L"NULL");
        return;
    }
    PostorderSteps(node->lchild.get(), steps);
    PostorderSteps(node->rchild.get(), steps);
    steps.push_back(std::wstring(1, node->data));
}

std::vector<std::wstring> GetTraversalSteps(const BiTNode* node, int type)
{
    std::vector<std::wstring> steps;
    switch (type)
    {
    case 0:
        PreorderSteps(node, steps);
        break;
    case 1:
        InorderSteps(node, steps);
        break;
    case 2:
        PostorderSteps(node, steps);
        break;
    }
    return steps;
}

static void PreorderNodes(const BiTNode* node, std::vector<const BiTNode*>& nodes)
{
    if (!node)
    {
        nodes.push_back(nullptr);
        return;
    }
    nodes.push_back(node);
    PreorderNodes(node->lchild.get(), nodes);
    PreorderNodes(node->rchild.get(), nodes);
}

static void InorderNodes(const BiTNode* node, std::vector<const BiTNode*>& nodes)
{
    if (!node)
    {
        nodes.push_back(nullptr);
        return;
    }
    InorderNodes(node->lchild.get(), nodes);
    nodes.push_back(node);
    InorderNodes(node->rchild.get(), nodes);
}

static void PostorderNodes(const BiTNode* node, std::vector<const BiTNode*>& nodes)
{
    if (!node)
    {
        nodes.push_back(nullptr);
        return;
    }
    PostorderNodes(node->lchild.get(), nodes);
    PostorderNodes(node->rchild.get(), nodes);
    nodes.push_back(node);
}

void CollectTraversalNodes(const BiTNode* node, int type, std::vector<const BiTNode*>& nodes)
{
    switch (type)
    {
    case 0:
        PreorderNodes(node, nodes);
        break;
    case 1:
        InorderNodes(node, nodes);
        break;
    case 2:
        PostorderNodes(node, nodes);
        break;
    }
}

// ============== Morris Traversal - O(1) Space ==============

// Helper: Clear all thread pointers in the tree
static void ClearThreads(const BiTNode* node)
{
    if (!node) return;
    const_cast<BiTNode*>(node)->thread = nullptr;
    ClearThreads(node->lchild.get());
    ClearThreads(node->rchild.get());
}

void MorrisInorder(const BiTNode* root, std::vector<std::wstring>& steps, std::vector<const BiTNode*>& nodes)
{
    const BiTNode* current = root;
    while (current)
    {
        if (!current->lchild)
        {
            // No left child, visit and go right
            steps.push_back(std::wstring(1, current->data));
            nodes.push_back(current);
            current = current->rchild ? current->rchild.get() : current->thread;
        }
        else
        {
            // Find inorder predecessor
            const BiTNode* predecessor = current->lchild.get();
            while (predecessor->rchild && predecessor->rchild.get() != current
                && predecessor->thread != current)
            {
                predecessor = predecessor->rchild.get();
            }

            if (predecessor->thread != current)
            {
                // Create thread, move left
                const_cast<BiTNode*>(predecessor)->thread = const_cast<BiTNode*>(current);
                current = current->lchild.get();
            }
            else
            {
                // Thread exists, remove it, visit current, move right
                const_cast<BiTNode*>(predecessor)->thread = nullptr;
                steps.push_back(std::wstring(1, current->data));
                nodes.push_back(current);
                current = current->rchild ? current->rchild.get() : current->thread;
            }
        }
    }
}

void MorrisPreorder(const BiTNode* root, std::vector<std::wstring>& steps, std::vector<const BiTNode*>& nodes)
{
    const BiTNode* current = root;
    while (current)
    {
        if (!current->lchild)
        {
            // No left child, visit and go right
            steps.push_back(std::wstring(1, current->data));
            nodes.push_back(current);
            current = current->rchild ? current->rchild.get() : current->thread;
        }
        else
        {
            // Find inorder predecessor
            const BiTNode* predecessor = current->lchild.get();
            while (predecessor->rchild && predecessor->rchild.get() != current
                && predecessor->thread != current)
            {
                predecessor = predecessor->rchild.get();
            }

            if (predecessor->thread != current)
            {
                // Visit current (preorder), create thread, move left
                steps.push_back(std::wstring(1, current->data));
                nodes.push_back(current);
                const_cast<BiTNode*>(predecessor)->thread = const_cast<BiTNode*>(current);
                current = current->lchild.get();
            }
            else
            {
                // Thread exists, remove it, move right (don't visit again)
                const_cast<BiTNode*>(predecessor)->thread = nullptr;
                current = current->rchild ? current->rchild.get() : current->thread;
            }
        }
    }
}

void MorrisPostorder(const BiTNode* root, std::vector<std::wstring>& steps, std::vector<const BiTNode*>& nodes)
{
    // Morris Postorder: mirrored preorder (NRL), then reverse → LRN
    const BiTNode* current = root;

    while (current)
    {
        if (!current->rchild)
        {
            steps.push_back(std::wstring(1, current->data));
            nodes.push_back(current);
            current = current->lchild ? current->lchild.get() : current->thread;
        }
        else
        {
            // Find leftmost in right subtree
            const BiTNode* predecessor = current->rchild.get();
            while (predecessor->lchild)
                predecessor = predecessor->lchild.get();

            if (predecessor->thread != current)
            {
                steps.push_back(std::wstring(1, current->data));
                nodes.push_back(current);
                const_cast<BiTNode*>(predecessor)->thread = const_cast<BiTNode*>(current);
                current = current->rchild.get();
            }
            else
            {
                const_cast<BiTNode*>(predecessor)->thread = nullptr;
                // Follow lchild or thread back
                current = current->lchild ? current->lchild.get() : current->thread;
            }
        }
    }

    std::reverse(steps.begin(), steps.end());
    std::reverse(nodes.begin(), nodes.end());
}

std::vector<std::wstring> MorrisTraversal(const BiTNode* root, int type)
{
    std::vector<std::wstring> steps;
    std::vector<const BiTNode*> nodes;

    switch (type)
    {
    case 0:
        MorrisPreorder(root, steps, nodes);
        break;
    case 1:
        MorrisInorder(root, steps, nodes);
        break;
    case 2:
        MorrisPostorder(root, steps, nodes);
        break;
    }

    return steps;
}

void MorrisTraversalWithNodes(const BiTNode* root, int type,
    std::vector<std::wstring>& steps, std::vector<const BiTNode*>& nodes)
{
    // Clear previous results
    steps.clear();
    nodes.clear();

    // Clear any leftover threads from previous traversals
    ClearThreads(root);

    switch (type)
    {
    case 0:
        MorrisPreorder(root, steps, nodes);
        break;
    case 1:
        MorrisInorder(root, steps, nodes);
        break;
    case 2:
        MorrisPostorder(root, steps, nodes);
        break;
    }
}

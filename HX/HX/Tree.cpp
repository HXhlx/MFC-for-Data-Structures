#include "Tree.h"

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
    case 0: // Preorder
        return L"void Preorder(BiTree T)\n"
               L"{\n"
               L"    if (T)\n"
               L"    {\n"
               L"        visit(T);           // visit root\n"
               L"        Preorder(T->lchild); // left subtree\n"
               L"        Preorder(T->rchild); // right subtree\n"
               L"    }\n"
               L"}";
    case 1: // Inorder
        return L"void Inorder(BiTree T)\n"
               L"{\n"
               L"    if (T)\n"
               L"    {\n"
               L"        Inorder(T->lchild);  // left subtree\n"
               L"        visit(T);            // visit root\n"
               L"        Inorder(T->rchild);  // right subtree\n"
               L"    }\n"
               L"}";
    case 2: // Postorder
        return L"void Postorder(BiTree T)\n"
               L"{\n"
               L"    if (T)\n"
               L"    {\n"
               L"        Postorder(T->lchild); // left subtree\n"
               L"        Postorder(T->rchild); // right subtree\n"
               L"        visit(T);             // visit root\n"
               L"    }\n"
               L"}";
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

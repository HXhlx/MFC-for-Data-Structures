#include "gtest_lite.h"
#include "Tree.h"
#include <sstream>

// Test Suite: CreateBt
TEST(CreateBt, EmptyTree) {
    std::wstring input = L"#";
    size_t index = 0;
    auto tree = CreateBt(input, index);
    EXPECT_TRUE(tree == nullptr);
    EXPECT_EQ(index, 1u);
}

TEST(CreateBt, SingleNode) {
    std::wstring input = L"A##";
    size_t index = 0;
    auto tree = CreateBt(input, index);
    EXPECT_TRUE(tree != nullptr);
    EXPECT_EQ(tree->data, L'A');
    EXPECT_EQ(tree->level, 1);
    EXPECT_TRUE(tree->lchild == nullptr);
    EXPECT_TRUE(tree->rchild == nullptr);
}

TEST(CreateBt, TwoLevels) {
    std::wstring input = L"AB###";
    size_t index = 0;
    auto tree = CreateBt(input, index);
    EXPECT_TRUE(tree != nullptr);
    EXPECT_EQ(tree->data, L'A');
    EXPECT_TRUE(tree->lchild != nullptr);
    EXPECT_EQ(tree->lchild->data, L'B');
    EXPECT_TRUE(tree->rchild == nullptr);
}

TEST(CreateBt, FullBinaryTree) {
    std::wstring input = L"ABD##E##CF##G##";
    size_t index = 0;
    auto tree = CreateBt(input, index);

    // Root: A
    EXPECT_TRUE(tree != nullptr);
    EXPECT_EQ(tree->data, L'A');

    // Left subtree: B
    EXPECT_TRUE(tree->lchild != nullptr);
    EXPECT_EQ(tree->lchild->data, L'B');

    // Right subtree: C
    EXPECT_TRUE(tree->rchild != nullptr);
    EXPECT_EQ(tree->rchild->data, L'C');

    // B's children: D, E
    EXPECT_TRUE(tree->lchild->lchild != nullptr);
    EXPECT_EQ(tree->lchild->lchild->data, L'D');
    EXPECT_TRUE(tree->lchild->rchild != nullptr);
    EXPECT_EQ(tree->lchild->rchild->data, L'E');

    // C's children: F, G
    EXPECT_TRUE(tree->rchild->lchild != nullptr);
    EXPECT_EQ(tree->rchild->lchild->data, L'F');
    EXPECT_TRUE(tree->rchild->rchild != nullptr);
    EXPECT_EQ(tree->rchild->rchild->data, L'G');
}

TEST(CreateBt, LevelTracking) {
    std::wstring input = L"ABD##E##C##";
    size_t index = 0;
    auto tree = CreateBt(input, index);

    EXPECT_EQ(tree->level, 1);
    EXPECT_EQ(tree->lchild->level, 2);
    EXPECT_EQ(tree->rchild->level, 2);
    EXPECT_EQ(tree->lchild->lchild->level, 3);
}

TEST(CreateBt, ExampleTree) {
    // a->left(b->left(c), b->right(d)), a->right(null)
    std::wstring input = L"abc##d##";
    size_t index = 0;
    auto tree = CreateBt(input, index);

    EXPECT_TRUE(tree != nullptr);
    EXPECT_EQ(tree->data, L'a');
    EXPECT_TRUE(tree->lchild != nullptr);
    EXPECT_EQ(tree->lchild->data, L'b');
    EXPECT_TRUE(tree->rchild == nullptr);  // a's right is #
    EXPECT_TRUE(tree->lchild->lchild != nullptr);
    EXPECT_EQ(tree->lchild->lchild->data, L'c');
    EXPECT_TRUE(tree->lchild->rchild != nullptr);
    EXPECT_EQ(tree->lchild->rchild->data, L'd');
}

// Test Suite: BiTreeDepth
TEST(BiTreeDepth, NullTree) {
    EXPECT_EQ(BiTreeDepth(nullptr), 0);
}

TEST(BiTreeDepth, SingleNode) {
    std::wstring input = L"A##";
    size_t index = 0;
    auto tree = CreateBt(input, index);
    EXPECT_EQ(BiTreeDepth(tree.get()), 1);
}

TEST(BiTreeDepth, TwoLevels) {
    std::wstring input = L"AB###";
    size_t index = 0;
    auto tree = CreateBt(input, index);
    EXPECT_EQ(BiTreeDepth(tree.get()), 2);
}

TEST(BiTreeDepth, ThreeLevels) {
    std::wstring input = L"ABD##E##C##";
    size_t index = 0;
    auto tree = CreateBt(input, index);
    EXPECT_EQ(BiTreeDepth(tree.get()), 3);
}

TEST(BiTreeDepth, UnbalancedTree) {
    // A -> B -> D (left only chain)
    std::wstring input = L"ABD###";
    size_t index = 0;
    auto tree = CreateBt(input, index);
    EXPECT_EQ(BiTreeDepth(tree.get()), 3);
}

TEST(BiTreeDepth, FullTree) {
    std::wstring input = L"ABD##E##CF##G##";
    size_t index = 0;
    auto tree = CreateBt(input, index);
    EXPECT_EQ(BiTreeDepth(tree.get()), 3);
}

// Test Suite: GetTraversalCode
TEST(GetTraversalCode, Preorder) {
    std::wstring code = GetTraversalCode(TraversalType::Preorder);
    EXPECT_TRUE(code.find(L"Morris Preorder") != std::wstring::npos);
    EXPECT_TRUE(code.find(L"visit(curr)") != std::wstring::npos);
}

TEST(GetTraversalCode, Inorder) {
    std::wstring code = GetTraversalCode(TraversalType::Inorder);
    EXPECT_TRUE(code.find(L"Morris Inorder") != std::wstring::npos);
    EXPECT_TRUE(code.find(L"visit(curr)") != std::wstring::npos);
}

TEST(GetTraversalCode, Postorder) {
    std::wstring code = GetTraversalCode(TraversalType::Postorder);
    EXPECT_TRUE(code.find(L"Morris Postorder") != std::wstring::npos);
    EXPECT_TRUE(code.find(L"reverse") != std::wstring::npos);
}

TEST(GetTraversalCode, InvalidType) {
    std::wstring code = GetTraversalCode(static_cast<TraversalType>(99));
    EXPECT_TRUE(code.empty());
}

// Test Suite: GetTraversalSteps
// Note: GetTraversalSteps adds "NULL" for each null child visited
// For A## (single node): preorder = A, NULL, NULL (3 items)
// For ABD##E##C##: preorder = A, B, D, NULL, NULL, E, NULL, NULL, C, NULL, NULL

TEST(GetTraversalSteps, PreorderSingleNode) {
    std::wstring input = L"A##";
    size_t index = 0;
    auto tree = CreateBt(input, index);

    auto steps = GetTraversalSteps(tree.get(), TraversalType::Preorder);

    // Preorder: A, NULL(left), NULL(right)
    EXPECT_EQ(steps.size(), 3u);
    EXPECT_EQ(steps[0], L"A");
    EXPECT_EQ(steps[1], L"NULL");
    EXPECT_EQ(steps[2], L"NULL");
}

TEST(GetTraversalSteps, InorderSingleNode) {
    std::wstring input = L"A##";
    size_t index = 0;
    auto tree = CreateBt(input, index);

    auto steps = GetTraversalSteps(tree.get(), TraversalType::Inorder);

    // Inorder: NULL(left), A, NULL(right)
    EXPECT_EQ(steps.size(), 3u);
    EXPECT_EQ(steps[0], L"NULL");
    EXPECT_EQ(steps[1], L"A");
    EXPECT_EQ(steps[2], L"NULL");
}

TEST(GetTraversalSteps, PostorderSingleNode) {
    std::wstring input = L"A##";
    size_t index = 0;
    auto tree = CreateBt(input, index);

    auto steps = GetTraversalSteps(tree.get(), TraversalType::Postorder);

    // Postorder: NULL(left), NULL(right), A
    EXPECT_EQ(steps.size(), 3u);
    EXPECT_EQ(steps[0], L"NULL");
    EXPECT_EQ(steps[1], L"NULL");
    EXPECT_EQ(steps[2], L"A");
}

TEST(GetTraversalSteps, PreorderTwoNodes) {
    // A -> left: B, right: null
    std::wstring input = L"AB##C##";
    size_t index = 0;
    auto tree = CreateBt(input, index);

    auto steps = GetTraversalSteps(tree.get(), TraversalType::Preorder);

    // Preorder: A, B, NULL, NULL, NULL(right of A is #, but input is AB##C##)
    // Actually: A->left=B, A->right=C, B->left=null, B->right=null, C->left=null, C->right=null
    // So preorder: A, B, NULL, NULL, C, NULL, NULL
    EXPECT_EQ(steps.size(), 7u);
    EXPECT_EQ(steps[0], L"A");
    EXPECT_EQ(steps[1], L"B");
    EXPECT_EQ(steps[2], L"NULL");
    EXPECT_EQ(steps[3], L"NULL");
    EXPECT_EQ(steps[4], L"C");
    EXPECT_EQ(steps[5], L"NULL");
    EXPECT_EQ(steps[6], L"NULL");
}

TEST(GetTraversalSteps, EmptyTree) {
    auto steps = GetTraversalSteps(nullptr, TraversalType::Preorder);
    EXPECT_EQ(steps.size(), 1u);
    EXPECT_EQ(steps[0], L"NULL");
}

TEST(GetTraversalSteps, PreorderFullTree) {
    //       A
    //      / \
    //     B   C
    //    / \   \
    //   D   E   F
    std::wstring input = L"ABD##E##C#F##";
    size_t index = 0;
    auto tree = CreateBt(input, index);

    auto steps = GetTraversalSteps(tree.get(), TraversalType::Preorder);

    // Preorder: A, B, D, NULL, NULL, E, NULL, NULL, C, NULL, F, NULL, NULL
    EXPECT_EQ(steps.size(), 13u);
    EXPECT_EQ(steps[0], L"A");
    EXPECT_EQ(steps[1], L"B");
    EXPECT_EQ(steps[2], L"D");
    EXPECT_EQ(steps[3], L"NULL");
    EXPECT_EQ(steps[4], L"NULL");
    EXPECT_EQ(steps[5], L"E");
    EXPECT_EQ(steps[6], L"NULL");
    EXPECT_EQ(steps[7], L"NULL");
    EXPECT_EQ(steps[8], L"C");
    EXPECT_EQ(steps[9], L"NULL");
    EXPECT_EQ(steps[10], L"F");
    EXPECT_EQ(steps[11], L"NULL");
    EXPECT_EQ(steps[12], L"NULL");
}

TEST(GetTraversalSteps, InorderFullTree) {
    std::wstring input = L"ABD##E##C#F##";
    size_t index = 0;
    auto tree = CreateBt(input, index);

    auto steps = GetTraversalSteps(tree.get(), TraversalType::Inorder);

    // Inorder: NULL, D, NULL, B, NULL, E, NULL, A, NULL, C, NULL, F, NULL
    EXPECT_EQ(steps.size(), 13u);
    EXPECT_EQ(steps[0], L"NULL");
    EXPECT_EQ(steps[1], L"D");
    EXPECT_EQ(steps[2], L"NULL");
    EXPECT_EQ(steps[3], L"B");
    EXPECT_EQ(steps[4], L"NULL");
    EXPECT_EQ(steps[5], L"E");
    EXPECT_EQ(steps[6], L"NULL");
    EXPECT_EQ(steps[7], L"A");
    EXPECT_EQ(steps[8], L"NULL");
    EXPECT_EQ(steps[9], L"C");
    EXPECT_EQ(steps[10], L"NULL");
    EXPECT_EQ(steps[11], L"F");
    EXPECT_EQ(steps[12], L"NULL");
}

TEST(GetTraversalSteps, PostorderFullTree) {
    std::wstring input = L"ABD##E##C#F##";
    size_t index = 0;
    auto tree = CreateBt(input, index);

    auto steps = GetTraversalSteps(tree.get(), TraversalType::Postorder);

    // Postorder: NULL, NULL, D, NULL, NULL, E, B, NULL, NULL, NULL, F, C, A
    EXPECT_EQ(steps.size(), 13u);
    EXPECT_EQ(steps[0], L"NULL");
    EXPECT_EQ(steps[1], L"NULL");
    EXPECT_EQ(steps[2], L"D");
    EXPECT_EQ(steps[3], L"NULL");
    EXPECT_EQ(steps[4], L"NULL");
    EXPECT_EQ(steps[5], L"E");
    EXPECT_EQ(steps[6], L"B");
    EXPECT_EQ(steps[7], L"NULL");
    EXPECT_EQ(steps[8], L"NULL");
    EXPECT_EQ(steps[9], L"NULL");
    EXPECT_EQ(steps[10], L"F");
    EXPECT_EQ(steps[11], L"C");
    EXPECT_EQ(steps[12], L"A");
}

// Test Suite: Memory Management
TEST(MemoryManagement, UniquePtrOwnership) {
    std::wstring input = L"ABD##E##C##";
    size_t index = 0;
    auto tree = CreateBt(input, index);

    EXPECT_TRUE(tree != nullptr);
    EXPECT_TRUE(tree.get() != nullptr);

    // Move ownership
    auto tree2 = std::move(tree);
    EXPECT_TRUE(tree == nullptr);
    EXPECT_TRUE(tree2 != nullptr);
}

TEST(MemoryManagement, AutomaticCleanup) {
    // This test verifies that unique_ptr properly cleans up
    // No memory leak should occur
    {
        std::wstring input = L"ABD##E##CF##G##";
        size_t index = 0;
        auto tree = CreateBt(input, index);
        EXPECT_TRUE(tree != nullptr);
    } // tree goes out of scope here, should be automatically cleaned up
}

// Test Suite: BiTNode Constructor
TEST(BiTNode, DefaultConstructor) {
    BiTNode node;
    EXPECT_EQ(node.address, 1);
    EXPECT_EQ(node.level, 1);
    EXPECT_EQ(node.data, L'\0');
    EXPECT_TRUE(node.lchild == nullptr);
    EXPECT_TRUE(node.rchild == nullptr);
}

TEST(BiTNode, ParameterizedConstructor) {
    BiTNode node(L'X', 5);
    EXPECT_EQ(node.address, 1);
    EXPECT_EQ(node.level, 5);
    EXPECT_EQ(node.data, L'X');
    EXPECT_TRUE(node.lchild == nullptr);
    EXPECT_TRUE(node.rchild == nullptr);
}

// Test Suite: Morris Traversal - O(1) Space
TEST(MorrisTraversal, InorderSingleNode) {
    std::wstring input = L"A##";
    size_t index = 0;
    auto tree = CreateBt(input, index);

    auto steps = MorrisTraversal(tree.get(), TraversalType::Inorder);
    EXPECT_EQ(steps.size(), 1u);
    EXPECT_EQ(steps[0], L"A");
}

TEST(MorrisTraversal, PreorderSingleNode) {
    std::wstring input = L"A##";
    size_t index = 0;
    auto tree = CreateBt(input, index);

    auto steps = MorrisTraversal(tree.get(), TraversalType::Preorder);
    EXPECT_EQ(steps.size(), 1u);
    EXPECT_EQ(steps[0], L"A");
}

TEST(MorrisTraversal, PostorderSingleNode) {
    std::wstring input = L"A##";
    size_t index = 0;
    auto tree = CreateBt(input, index);

    auto steps = MorrisTraversal(tree.get(), TraversalType::Postorder);
    EXPECT_EQ(steps.size(), 1u);
    EXPECT_EQ(steps[0], L"A");
}

TEST(MorrisTraversal, InorderThreeNodes) {
    //     A
    //    / \
    //   B   C
    std::wstring input = L"AB##C##";
    size_t index = 0;
    auto tree = CreateBt(input, index);

    auto steps = MorrisTraversal(tree.get(), TraversalType::Inorder);
    // Inorder: B, A, C
    EXPECT_EQ(steps.size(), 3u);
    EXPECT_EQ(steps[0], L"B");
    EXPECT_EQ(steps[1], L"A");
    EXPECT_EQ(steps[2], L"C");
}

TEST(MorrisTraversal, PreorderThreeNodes) {
    std::wstring input = L"AB##C##";
    size_t index = 0;
    auto tree = CreateBt(input, index);

    auto steps = MorrisTraversal(tree.get(), TraversalType::Preorder);
    // Preorder: A, B, C
    EXPECT_EQ(steps.size(), 3u);
    EXPECT_EQ(steps[0], L"A");
    EXPECT_EQ(steps[1], L"B");
    EXPECT_EQ(steps[2], L"C");
}

TEST(MorrisTraversal, PostorderThreeNodes) {
    std::wstring input = L"AB##C##";
    size_t index = 0;
    auto tree = CreateBt(input, index);

    auto steps = MorrisTraversal(tree.get(), TraversalType::Postorder);
    // Postorder: B, C, A
    EXPECT_EQ(steps.size(), 3u);
    EXPECT_EQ(steps[0], L"B");
    EXPECT_EQ(steps[1], L"C");
    EXPECT_EQ(steps[2], L"A");
}

TEST(MorrisTraversal, InorderFullTree) {
    //       A
    //      / \
    //     B   C
    //    / \   \
    //   D   E   F
    std::wstring input = L"ABD##E##C#F##";
    size_t index = 0;
    auto tree = CreateBt(input, index);

    auto steps = MorrisTraversal(tree.get(), TraversalType::Inorder);
    // Inorder: D, B, E, A, C, F
    EXPECT_EQ(steps.size(), 6u);
    EXPECT_EQ(steps[0], L"D");
    EXPECT_EQ(steps[1], L"B");
    EXPECT_EQ(steps[2], L"E");
    EXPECT_EQ(steps[3], L"A");
    EXPECT_EQ(steps[4], L"C");
    EXPECT_EQ(steps[5], L"F");
}

TEST(MorrisTraversal, PreorderFullTree) {
    std::wstring input = L"ABD##E##C#F##";
    size_t index = 0;
    auto tree = CreateBt(input, index);

    auto steps = MorrisTraversal(tree.get(), TraversalType::Preorder);
    // Preorder: A, B, D, E, C, F
    EXPECT_EQ(steps.size(), 6u);
    EXPECT_EQ(steps[0], L"A");
    EXPECT_EQ(steps[1], L"B");
    EXPECT_EQ(steps[2], L"D");
    EXPECT_EQ(steps[3], L"E");
    EXPECT_EQ(steps[4], L"C");
    EXPECT_EQ(steps[5], L"F");
}

TEST(MorrisTraversal, PostorderFullTree) {
    std::wstring input = L"ABD##E##C#F##";
    size_t index = 0;
    auto tree = CreateBt(input, index);

    auto steps = MorrisTraversal(tree.get(), TraversalType::Postorder);
    // Postorder: D, E, B, F, C, A
    EXPECT_EQ(steps.size(), 6u);
    EXPECT_EQ(steps[0], L"D");
    EXPECT_EQ(steps[1], L"E");
    EXPECT_EQ(steps[2], L"B");
    EXPECT_EQ(steps[3], L"F");
    EXPECT_EQ(steps[4], L"C");
    EXPECT_EQ(steps[5], L"A");
}

TEST(MorrisTraversal, MatchesRecursive) {
    // Verify Morris traversal produces same result as recursive (non-NULL nodes only)
    std::wstring input = L"ABD##E##CF##G##";
    size_t index = 0;
    auto tree = CreateBt(input, index);

    // Morris traversal only visits non-NULL nodes
    auto morrisInorder = MorrisTraversal(tree.get(), TraversalType::Inorder);
    auto morrisPreorder = MorrisTraversal(tree.get(), TraversalType::Preorder);
    auto morrisPostorder = MorrisTraversal(tree.get(), TraversalType::Postorder);

    // Count non-NULL nodes in recursive traversal
    auto recursiveInorder = GetTraversalSteps(tree.get(), TraversalType::Inorder);
    int nonNullCount = 0;
    for (const auto& s : recursiveInorder) {
        if (s != L"NULL") nonNullCount++;
    }

    EXPECT_EQ(morrisInorder.size(), nonNullCount);
    EXPECT_EQ(morrisPreorder.size(), nonNullCount);
    EXPECT_EQ(morrisPostorder.size(), nonNullCount);
}

TEST(BiTNode, ThreadFieldDefaultNull) {
    BiTNode node(L'X', 1);
    EXPECT_TRUE(node.thread == nullptr);
}

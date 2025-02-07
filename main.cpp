#include "./morris_iter.hpp"
#include "tree.hpp"
#include <iostream>

int main(int argc, const char **argv) {
  auto *root = new BinaryTreeNode(15);
  root->left =
      new BinaryTreeNode(new BinaryTreeNode(3), new BinaryTreeNode(5), 7);
  root->right =
      new BinaryTreeNode(new BinaryTreeNode(2), new BinaryTreeNode(6), 10);
  root->left->left->left = new BinaryTreeNode(114);
  root->left->left->right = new BinaryTreeNode(115);
  root->left->right->left = new BinaryTreeNode(116);
  root->left->right->right = new BinaryTreeNode(117);
  root->right->left->left = new BinaryTreeNode(118);
  root->right->left->right = new BinaryTreeNode(119);
  root->right->right->left = new BinaryTreeNode(120);
  root->right->right->right = new BinaryTreeNode(121);

  // root->right->left->left = new BinaryTreeNode<int>(
  //     new BinaryTreeNode(2),
  //     new BinaryTreeNode(new BinaryTreeNode(new BinaryTreeNode(341),
  //                                           new BinaryTreeNode(13), 12),
  //                        new BinaryTreeNode(1431), 12),
  //     0);
  backMorrisIter(root);
  cout << getTreeStr(root);
}

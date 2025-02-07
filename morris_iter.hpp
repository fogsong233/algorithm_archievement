#ifndef morris_iter_H
#define morris_iter_H

/**
 * @file morris_iter.cpp
 * @author fogsong233 (github: fogsong233)
 * @brief
 * @version 0.1
 * @date 2025-02-06
 *
 * @copyright Copyright (c) 2025
 *
 */

/**
 * Morris iter
 * 空间复杂度为O(1), 时间复杂度为O(N), 空间复杂度优秀
 * 实现原则:
 * 1. 当前节点为cur
 * 2. 无左节点,to right
 * 3.有left, 左子树最右的节点为mostright
 * 4. 有如果mostright的right为空,指向cur,然后cur = cur.left
 * 5. 如果mostright的right不是空,指向自己,指向空,cur = cur.right
 */

#include "./tree.hpp"
#include <algorithm>
#include <concepts>
#include <cstdio>
#include <iostream>
#include <ostream>
#include <ranges>
#include <vector>

template <typename T>
concept Printable = requires(T t, std::ostream &os) {
  { os << t } -> std::convertible_to<std::ostream &>;
};

using namespace std;
/**
 * @brief 前序遍历,left-cur-right
 *
 * @tparam T
 * @param root
 * @return requires
 */
template <typename T>
  requires Printable<T>
void preMorrisIter(BinaryTreeNode<T> *root) {
  auto *cur = root;
  BinaryTreeNode<T> *mostright = nullptr;
  if (root == nullptr) {
    return;
  }
  while (cur != nullptr) {
    if (cur->left == nullptr) {
      cout << cur->value << endl;
      cur = cur->right;
      continue;
    }
    // 找到mostright
    mostright = cur->left;
    while (mostright->right != nullptr && mostright->right != cur) {
      mostright = mostright->right;
    }

    // 第一次遍历,加上cur
    if (mostright->right == nullptr) {
      mostright->right = cur;
      cout << cur->value << endl;
      cur = cur->left;
    } else {
      mostright->right = nullptr;
      cur = cur->right;
    }
  }
}

template <typename T>
  requires Printable<T>
void midMorrisIter(BinaryTreeNode<T> *root) {
  auto *cur = root;
  BinaryTreeNode<T> *mostright = nullptr;
  if (root == nullptr) {
    return;
  }
  while (cur != nullptr) {
    if (cur->left == nullptr) {
      cout << cur->value << endl;
      cur = cur->right;
      continue;
    }
    // 找到mostright
    mostright = cur->left;
    while (mostright->right != nullptr && mostright->right != cur) {
      mostright = mostright->right;
    }

    // 第一次遍历,加上cur
    if (mostright->right == nullptr) {
      mostright->right = cur;
      cur = cur->left;
    } else {
      mostright->right = nullptr;
      cout << cur->value << endl;
      cur = cur->right;
    }
  }
}

template <typename T>
  requires Printable<T>
void backMorrisIter(BinaryTreeNode<T> *root) {
  auto *cur = root;

  // 所有的cur.left.right和root.right的所有right都要用这个打印,
  // 因为只能打印cur.left和cur的叶节点
  auto printRev = [](const BinaryTreeNode<T> *printNode) {
    vector<const T *> printWaiting{};
    while (printNode->left != nullptr || printNode->right != nullptr) {
      printWaiting.push_back(&(printNode->value));
      printNode = printNode->right;
    }
    ranges::for_each(ranges::views::reverse(printWaiting),
                     [](auto value) { cout << *value << endl; });
  };

  BinaryTreeNode<T> *mostright = nullptr;
  if (root == nullptr) {
    return;
  }
  while (cur != nullptr) {
    if (cur->left == nullptr) {
      cout << cur->value << endl;
      cur = cur->right;
      continue;
    }
    // 找到mostright
    mostright = cur->left;
    while (mostright->right != nullptr && mostright->right != cur) {
      mostright = mostright->right;
    }

    // 第一次遍历,加上cur
    if (mostright->right == nullptr) {
      mostright->right = cur;
      cur = cur->left;
    } else {
      mostright->right = nullptr;
      printRev(cur->left);
      cur = cur->right;
    }
  }
  printRev(root);
}

#endif // morris_iter_H
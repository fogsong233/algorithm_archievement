#ifndef tree_H
#define tree_H

#include <algorithm>
#include <cstddef>
#include <cstdio>
#include <sstream>
#include <string>
#include <utility>
using namespace std;
template <typename T> class BinaryTreeNode {
public:
  BinaryTreeNode *left = nullptr;
  BinaryTreeNode *right = nullptr;
  T value;
  BinaryTreeNode(BinaryTreeNode *left, BinaryTreeNode *right, T value)
      : left(left), right(right), value(value) {}
  BinaryTreeNode(T value) : value(value) {}
  BinaryTreeNode() = default;
  ~BinaryTreeNode() {
    delete left;
    delete right;
  }
};

pair<int, int> getRect(const string &str) {
  int width = 0;
  int height = 0;
  int nowWidth = 0;
  ranges::for_each(str, [&](auto ch) {
    if (ch == '\n') {
      height++;
      width = max(width, nowWidth);
      nowWidth = 0;
    } else {
      nowWidth++;
    }
  });
  if (str.back() != '\n') {
    height++;
    width = max(width, nowWidth);
  }
  return {width, height};
}

string addBorder(const string &rawStr) {
  auto rect = getRect(rawStr);
  stringstream ss{};
  size_t nowIndex = 0;
  int totalLen = (rect.first + 2) * (rect.second + 2);
  for (int i = 0; i < totalLen; i++) {
    if (i == 0 || i == totalLen - 1 ||
        i == (rect.first + 2) * (rect.second + 1)) {
      ss << "+";
    } else if (i == rect.first + 1) {
      ss << "+\n";
    } else if (i < rect.first + 2 || i > (rect.first + 2) * (rect.second + 1)) {
      ss << "-";
    } else if (i % (rect.first + 2) == 0) {
      ss << "|";
    } else if ((i % (rect.first + 2)) == rect.first + 1) {
      ss << "|\n";
    } else {
      if (nowIndex < rawStr.size()) {
        if (rawStr[nowIndex] == '\n') {
          nowIndex++;
        }
        ss << rawStr[nowIndex++];
      } else {
        ss << " ";
      }
    }
  }
  return ss.str();
}

template <typename T> string getTreeStr(BinaryTreeNode<T> *root) {
  if (root == nullptr) {
    return "";
  }
  if (root->left == nullptr && root->right == nullptr) {
    stringstream ss{};
    ss << root->value;
    return ss.str();
  }
  auto leftStr = getTreeStr(root->left);
  auto leftRect = getRect(leftStr);
  auto rightStr = getTreeStr(root->right);
  auto rightRect = getRect(rightStr);
  stringstream ss{};
  ss << root->value;
  auto rVSize = ss.str().size();
  int bottomMaxWidth = leftRect.first + rightRect.first + 1;
  int maxWidth = max(static_cast<int>(ss.str().size()), bottomMaxWidth);
  auto half = (maxWidth - ss.str().size()) / 2;
  ss = stringstream();
  ss << string(half, ' ') << root->value
     << string(maxWidth - half - rVSize, ' ') << "\n";
  size_t totalLen = maxWidth * max(leftRect.second, rightRect.second);
  size_t lp = 0;
  size_t rp = 0;
  for (size_t i = 0; i < totalLen; i++) {
    if (i % maxWidth < leftRect.first) {
      if (lp < leftStr.size()) {
        if (leftStr[lp] == '\n') {
          lp++;
        }
        ss << leftStr[lp++];
      } else {
        ss << ' ';
      }
    } else if (i % maxWidth == leftRect.first) {
      ss << " ";
    } else if ((i % maxWidth) < static_cast<size_t>(bottomMaxWidth)) {

      if (rp < rightStr.size()) {
        if (rightStr[rp] == '\n') {
          rp++;
        }
        ss << rightStr[rp++];
      } else {
        ss << " ";
      }
    } else {
      ss << " ";
    }

    if (i % maxWidth - maxWidth == -1) {
      ss << '\n';
    }
  }
  return addBorder(ss.str());
}
#endif // tree_H
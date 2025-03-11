#include <climits>
#include <cmath>
#include <cstdio>
#include <format>
#include <iostream>
#include <iterator>
#include <set>
#include <string>
#include <utility>
#include <vector>

namespace bool_lighter {
using namespace std;
class Cover {

public:
  int varNum = 0;
  string nowMinItem; // '0' 'X' '1'
  set<int> containItems;
  bool checked = false;
  int ones = 0;
  Cover(int varNum, int nowMinItemInt) : varNum(varNum) {
    this->containItems.insert(nowMinItemInt);
    while (varNum--) {
      nowMinItem.push_back((nowMinItemInt >> varNum) + '0');
      if (nowMinItem.back() == '1') {
        ones++;
      }
      nowMinItemInt -= (nowMinItemInt >> varNum) << varNum;
    }
  }
  Cover(Cover m1, Cover m2) {
    this->varNum = m1.varNum;
    this->containItems.insert(m1.containItems.begin(), m1.containItems.end());
    this->containItems.insert(m2.containItems.begin(), m2.containItems.end());
    for (int i = 0; i < varNum; i++) {
      if (m1.nowMinItem[i] != m2.nowMinItem[i]) {
        this->nowMinItem.push_back('X');
      } else {
        this->nowMinItem.push_back(m1.nowMinItem[i]);
      }
      if (nowMinItem[i] == '1') {
        ones++;
      }
    }
  }

  bool operator==(const Cover &m2) const {
    return this->nowMinItem == m2.nowMinItem;
  }

  auto operator<=>(const Cover &m2) const {
    return m2.nowMinItem <=> (this->nowMinItem);
  }
};
} // namespace bool_lighter

template <> struct std::formatter<bool_lighter::Cover> {
  constexpr auto parse(std::format_parse_context &ctx) { return ctx.begin(); }

  template <typename FormatContext>
  auto format(const bool_lighter::Cover &c, FormatContext &ctx) const {
    string prt{};
    std::format_to(back_inserter(prt), "Cover: {}, from ", c.nowMinItem);
    for (const auto n : c.containItems) {
      std::format_to(back_inserter(prt), "{} ", n);
    }
    return std::format_to(ctx.out(), "{}", prt);
  }
};

namespace bool_lighter {

using LeastCoversChoices = vector<set<Cover>>;

bool isOnlyOneDiff(const string &a, const string &b, int varNum) {
  int diffCnt = 0;
  for (int i = 0; i < varNum; i++) {
    if (a[i] != b[i]) {
      diffCnt++;
      if (diffCnt > 1) {
        return false;
      }
    }
  }
  return diffCnt == 1;
}

namespace util {
void setTrueDigit(int &flags, int digitFromRight) {
  flags = flags | (1 << digitFromRight);
}
inline bool isFill(int flags, int num) { return flags == ((1 << num) - 1); }

} // namespace util

set<Cover> _findAllCovers(int varNum, vector<int> &minItems) {
  // 获得素覆盖
  set<Cover> covers{};

  vector<vector<Cover>> waitingCoverGroups(varNum + 1, vector<Cover>());
  vector<vector<Cover>> nxtCoverGroups(varNum + 1, vector<Cover>());
  for (const auto item : minItems) {
    Cover temp(varNum, item);
    waitingCoverGroups[temp.ones].push_back(std::move(temp));
  }

  // 开始判断
  while (true) {
    bool hasAdded = false;
    for (int i = 0; i < varNum; i++) {
      for (auto &nowCover : waitingCoverGroups[i]) {
        for (auto &mayCover : waitingCoverGroups[i + 1]) {
          if (isOnlyOneDiff(mayCover.nowMinItem, nowCover.nowMinItem, varNum)) {
            Cover temp(nowCover, mayCover);
            bool hasSame = false;
            for (const auto &c : nxtCoverGroups[temp.ones]) {
              auto hasSame = temp == c;
            }
            if (!hasSame) {
              nxtCoverGroups[temp.ones].push_back(std::move(temp));
            }
            nowCover.checked = true;
            mayCover.checked = true;
            hasAdded = true;
          }
        }
        if (!nowCover.checked) {
          covers.insert(nowCover);
        }
      }
    }
    if (!hasAdded) {
      // 结束遍历
      break;
    }

    // 切换到下一个
    waitingCoverGroups = std::move(nxtCoverGroups);
    nxtCoverGroups = vector<vector<Cover>>(varNum + 1, vector<Cover>{});
  }

  return covers;
}

pair<int, set<Cover>> _findMustNeedCovers(vector<int> &minItems,
                                          vector<Cover> &covers) {
  // 获取到了所有的素覆盖,然后找实质覆盖
  // booltable
  vector<vector<bool>> boolTable(minItems.size(), vector<bool>(covers.size()));
  for (int i = 0; i < minItems.size(); i++) { // i: 第i+1个变量
    for (int j = 0; j < covers.size(); j++) { // j: 一个cover是否含有i+1
      boolTable[i][j] = covers[j].containItems.contains(minItems[i]);
    }
  }

  pair res{0, set<Cover>{}};

  // 实质覆盖
  int flags = 0;
  for (int i = 0; i < minItems.size(); i++) {
    int hasNum = 0;
    int index = -1;
    for (int j = 0; j < covers.size(); j++) {
      if (boolTable[i][j]) {
        hasNum++;
        index = j;
      }
      if (hasNum > 1) {
        break;
      }
    }
    if (hasNum == 1) {
      // 找到一个实质覆盖
      covers[index].checked = true;
      res.second.insert(covers[index]);
      for (int k = 0; k < minItems.size(); k++) {
        // 确定有多少原始的被覆盖了
        if (covers[index].containItems.contains(minItems[k])) {
          util::setTrueDigit(flags, k);
        }
      }
    }
  }
  res.first = flags;
  return res;
}

LeastCoversChoices QMAlgorithmCore(int varNum, vector<int> minItems) {
  LeastCoversChoices finalResult{};
  auto coversSet = _findAllCovers(varNum, minItems);
  auto covers = vector<Cover>(coversSet.begin(), coversSet.end());
  printf("素覆盖:\n");
  std::cout << format("{}", covers.front()) << '\n';

  auto resFrom = _findMustNeedCovers(minItems, covers);
  int flags = resFrom.first;
  auto priRes = std::move(resFrom.second);
  // 在剩余的里面找到最少的
  vector<int> remainedCoverIndexes{};
  for (int i = 0; i < covers.size(); i++) {
    if (!covers[i].checked) {
      remainedCoverIndexes.push_back(i);
    }
  }

  // 枚举, 全选到全不选
  int minSz = INT_MAX;
  vector<set<Cover *>> maybeCoversChoices{};
  int start = 0;
  int end = 1 << remainedCoverIndexes.size();
  for (; start < end; start++) {
    // 获取数列
    int sz = remainedCoverIndexes.size();
    set<Cover *> choices{};
    int newFlags = flags;
    for (int i = 0; i < sz; i++) {
      if ((start & (1 << i)) != 0) { // 选择这位
        choices.insert(&covers[remainedCoverIndexes[i]]);
      }
    }
    if (choices.size() > minSz) {
      continue;
    }
    for (auto *const coverPtr : choices) {
      for (int k = 0; k < minItems.size(); k++) {
        if (coverPtr->containItems.contains(minItems[k])) {
          util::setTrueDigit(newFlags, k);
        }
      }
    }
    if (util::isFill(newFlags, varNum)) {
      minSz = choices.size();
      maybeCoversChoices.push_back(std::move(choices));
    }
  }
  for (const auto &choice : maybeCoversChoices) {
    auto res = priRes;
    for (const auto *coverPtr : choice) {
      res.insert(*coverPtr);
    }
    finalResult.push_back(res);
  }
  return finalResult;
}

vector<string> coversToDisplayExpressions(LeastCoversChoices &choices,
                                          vector<string> varNames) {
  vector<string> result;
  for (const auto &choice : choices) {
    string exp;
    for (const auto &cover : choice) {
      for (int k = 0; k < cover.nowMinItem.size(); k++) {
        switch (cover.nowMinItem[k]) {
        case '0':
          exp += varNames[k] + '\'';
          break;
        case '1':
          exp += varNames[k];
        }
      }
      exp += " + ";
    }
    exp.erase(exp.size() - 3, 3);
    result.push_back(exp);
  }
  return result;
}
vector<string> coversToDisplayExpressions(LeastCoversChoices &choices,
                                          int varNum) {
  vector<string> varNames{};
  char base = 'A';
  for (int i = 0; i < varNum; i++) {
    varNames.push_back(string{char(base + i)});
  }
  return coversToDisplayExpressions(choices, varNames);
}

} // namespace bool_lighter

int main(int argc, const char **argv) {
  using namespace bool_lighter;
  // 输入所有的最小项,获取最小覆盖
  // auto res = QMAlgorithmCore(5, std::vector<int>{2, 3, 7, 9, 10, 11, 12, 13,
  // 18,
  //                                                19, 22, 23, 26, 27, 30,
  //                                                31});
  // 转化为表达式(A'代表反A)
  // F(A,B,C,D,E)=∑(2,3,7,9,10,11,12,13,18,19,22,23,26,27,30,31)
  // 会输出
  /**
   *  C'D + B'DE + AD + A'BD'E + A'BCD'
      -----------
      C'D + B'DE + AD + A'BCD' + A'BC'E
      -----------
   *
   */
  cout << "先输入变量个数,最小项个数,一行内输入所有最小项的十进制,"
          "空格分开, "
          "如'1 2 1 0'"
       << '\n';
  int varNum, N;
  scanf("%d%d", &varNum, &N);
  vector<int> items(N);
  for (int i = 0; i < N; i++) {
    scanf("%d", &items[i]);
  }
  auto res = QMAlgorithmCore(varNum, items);

  auto exps = coversToDisplayExpressions(res, 5);
  for (const auto &choice : exps) {
    std::cout << choice << '\n';
    std::cout << "-----------" << '\n';
  }
  return 0;
}
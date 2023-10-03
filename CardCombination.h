#ifndef CARD_COMBINATION_H
#define CARD_COMBINATION_H
#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include "Card.h"
#include <variant>
#include <type_traits>

// 代表未出过牌(刚开始)
// 代表过牌
struct PASS
{
};
// 单牌
struct SINGLE
{
    int primary;
};
// 对子
struct PAIR
{
    int primary;
};
// 三张
struct TRIO
{
    int primary;
};
// 三带一
struct TrioWithOneData
{
    int primary, secondary; // 3带1
};
// 三带二 33322 primary请放 3, secondary请放2
struct TRIO_WITH_PAIR
{
    int primary, secondary; // 3带2
};
// 比如34567, primary请放[3, 4, 5, 6, 7]
struct STRAIGHT
{
    std::vector<int> straight;
};
// 比如334455, primary请放[3, 4, 5]
struct MULTI_PAIRS
{
    std::vector<int> multi_pairs;
};
// 比如333444, primary请放[3, 4]
struct PLANE
{
    std::vector<int> primary;
};
// 比如333444555666, primary请放[3, 4, 5], secondary请放[6, 6, 6]
struct PLANE_WITH_SINGLE_WINGS
{
    std::vector<int> primary;
    std::vector<int> secondary; // 副牌
};
// 比如3334446666, primary请放[3, 4], secondary请放[6, 6]
struct PLANE_WITH_PAIR_WINGS
{
    std::vector<int> primary;
    std::vector<int> secondary;
};
// 炸弹
struct BOMB
{
    int primary;
};
// 四带二 444423   primary请放 4, secondary请放[2, 3]
struct FOUR_WITH_TWO
{
    int primary;
    int secondary[2]; // 副牌
};
// 四带两对  44442233  primary请放 4, secondary请放[2, 3]
struct FOUR_WITH_TWO_PAIR
{
    int primary;
    int secondary[2];
};
// 火箭
struct ROCKET
{
};

using CardCombination = std::variant<
    PASS,
    SINGLE,
    PAIR,
    TRIO,
    TrioWithOneData,
    TRIO_WITH_PAIR,
    STRAIGHT,
    MULTI_PAIRS,
    PLANE,
    PLANE_WITH_SINGLE_WINGS,
    PLANE_WITH_PAIR_WINGS,
    BOMB,
    FOUR_WITH_TWO,
    FOUR_WITH_TWO_PAIR,
    ROCKET>;

struct LastTwoTurns
{
    CardCombination previousTurn;
    CardCombination lastTurn;
};

template <typename... Types>
bool holds_same_type(const std::variant<Types...> &var1, const std::variant<Types...> &var2);

// has sort side effect
bool detectCardType(CardCombination &cards_combination);

bool compareCardsWithLast2(const CardCombination &card, const LastTwoTurns &last_2_turns_state);

bool compareCards(const CardCombination &cards1, const CardCombination &cards2);

std::vector<int> cardCombinationToVector(const CardCombination &cards_combination);
#endif
#include "CardCombination.h"

template <typename T, typename... Types>
constexpr bool holds_type(const std::variant<Types...> &var)
{
    return std::holds_alternative<T>(var);
}

template <typename... Types>
bool holds_same_type(const std::variant<Types...> &var1, const std::variant<Types...> &var2)
{
    return (... || (holds_type<Types>(var1) && holds_type<Types>(var2)));
}

void printType(const CardCombination &combo)
{
    std::visit([](auto &&arg)
               {
                   using T = std::decay_t<decltype(arg)>;
                   if constexpr (std::is_same_v<T, PASS>)
                   {
                       std::cout << "PASS";
                   }
                   else if constexpr (std::is_same_v<T, SINGLE>)
                   {
                       std::cout << "SINGLE";
                   }
                   else if constexpr (std::is_same_v<T, PAIR>)
                   {
                       std::cout << "PAIR";
                   }
                   else if constexpr (std::is_same_v<T, TRIO>)
                   {
                       std::cout << "TRIO";
                   }
                   else if constexpr (std::is_same_v<T, TrioWithOneData>)
                   {
                       std::cout << "TrioWithOneData";
                   }
                   else if constexpr (std::is_same_v<T, TRIO_WITH_PAIR>)
                   {
                       std::cout << "TRIO_WITH_PAIR";
                   }
                   else if constexpr (std::is_same_v<T, STRAIGHT>)
                   {
                       std::cout << "STRAIGHT";
                   }
                   else if constexpr (std::is_same_v<T, MULTI_PAIRS>)
                   {
                       std::cout << "MULTI_PAIRS";
                   }
                   else if constexpr (std::is_same_v<T, PLANE>)
                   {
                       std::cout << "PLANE";
                   }
                   else if constexpr (std::is_same_v<T, PLANE_WITH_SINGLE_WINGS>)
                   {
                       std::cout << "PLANE_WITH_SINGLE_WINGS";
                   }
                   else if constexpr (std::is_same_v<T, PLANE_WITH_PAIR_WINGS>)
                   {
                       std::cout << "PLANE_WITH_PAIR_WINGS";
                   }
                   else if constexpr (std::is_same_v<T, BOMB>)
                   {
                       std::cout << "BOMB";
                   }
                   else if constexpr (std::is_same_v<T, FOUR_WITH_TWO>)
                   {
                       std::cout << "FOUR_WITH_TWO";
                   }
                   else if constexpr (std::is_same_v<T, FOUR_WITH_TWO_PAIR>)
                   {
                       std::cout << "FOUR_WITH_TWO_PAIR";
                   }
                   else if constexpr (std::is_same_v<T, ROCKET>)
                   {
                       std::cout << "ROCKET";
                   }
                   std::cout << std::endl; },
               combo);
}

// 检测plane牌型组成是否合法, true 合法, false 非法
bool is_plane_vec(std::vector<int> &vec)
{
    std::sort(vec.begin(), vec.end());
    auto end_unique = std::unique(vec.begin(), vec.end());
    vec.erase(end_unique, vec.end());
    if (vec.size() < 2)
    {
        return false;
    }
    for (int i = 0; i < vec.size(); i++)
    {
        if (!is_legal_3_to_A(vec[i]))
        {
            return false;
        }
        if (i > 0 && vec[i] != vec[i - 1] + 1)
        {
            return false;
        }
    }
    return true;
}

bool detectCardType(CardCombination &cards_combination) // has sort side effect
{
    if (std::holds_alternative<PASS>(cards_combination))
    {
        return true;
    }
    else if (std::holds_alternative<SINGLE>(cards_combination))
    {
        SINGLE data = std::get<SINGLE>(cards_combination);
        return is_legal_card(data.primary);
    }
    else if (std::holds_alternative<PAIR>(cards_combination))
    {
        PAIR data = std::get<PAIR>(cards_combination);
        return is_legal_trump(data.primary);
    }
    else if (std::holds_alternative<TRIO>(cards_combination))
    {
        TRIO data = std::get<TRIO>(cards_combination);
        return is_legal_trump(data.primary);
    }
    else if (std::holds_alternative<TrioWithOneData>(cards_combination))
    {
        TrioWithOneData data = std::get<TrioWithOneData>(cards_combination);
        return is_legal_trump(data.primary) && is_legal_card(data.secondary);
    }
    else if (std::holds_alternative<TRIO_WITH_PAIR>(cards_combination))
    {
        TRIO_WITH_PAIR data = std::get<TRIO_WITH_PAIR>(cards_combination);
        return is_legal_trump(data.primary) && is_legal_trump(data.secondary);
    }
    else if (std::holds_alternative<STRAIGHT>(cards_combination))
    {
        STRAIGHT data = std::get<STRAIGHT>(cards_combination);
        if (data.straight.size() < 5)
        {
            return false;
        }
        std::sort(data.straight.begin(), data.straight.end());
        for (int i = 0; i < data.straight.size(); i++)
        {
            if (!is_legal_3_to_A(data.straight[i]))
            {
                return false;
            }
            if (i > 0 && data.straight[i] != data.straight[i - 1] + 1)
            {
                return false;
            }
        }
        return true;
    }
    else if (std::holds_alternative<MULTI_PAIRS>(cards_combination))
    {
        MULTI_PAIRS data = std::get<MULTI_PAIRS>(cards_combination);
        std::sort(data.multi_pairs.begin(), data.multi_pairs.end());
        auto end_unique = std::unique(data.multi_pairs.begin(), data.multi_pairs.end());
        data.multi_pairs.erase(end_unique, data.multi_pairs.end());
        if (data.multi_pairs.size() < 3)
        {
            return false;
        }
        for (int i = 0; i < data.multi_pairs.size(); i++)
        {
            if (!is_legal_3_to_A(data.multi_pairs[i]))
            {
                return false;
            }
            if (i > 0 && data.multi_pairs[i] != data.multi_pairs[i - 1] + 1)
            {
                return false;
            }
        }
        return true;
    }
    else if (std::holds_alternative<PLANE>(cards_combination))
    {
        PLANE data = std::get<PLANE>(cards_combination);
        if (!is_plane_vec(data.primary))
        {
            return false;
        }
        return true;
    }
    else if (std::holds_alternative<PLANE_WITH_SINGLE_WINGS>(cards_combination))
    {
        PLANE_WITH_SINGLE_WINGS data = std::get<PLANE_WITH_SINGLE_WINGS>(cards_combination);
        if (!is_plane_vec(data.primary))
        {
            return false;
        }
        if (data.primary.size() != data.secondary.size())
        {
            return false;
        }
        for (int card : data.secondary)
        {
            if (!is_legal_card(card))
            {
                return false;
            }
        }
        return true;
    }
    else if (std::holds_alternative<PLANE_WITH_PAIR_WINGS>(cards_combination))
    {
        PLANE_WITH_PAIR_WINGS data = std::get<PLANE_WITH_PAIR_WINGS>(cards_combination);
        if (!is_plane_vec(data.primary))
        {
            return false;
        }
        if ((data.primary.size() << 1) != data.secondary.size())
        {
            return false;
        }
        std::sort(data.secondary.begin(), data.secondary.end());
        for (int card : data.secondary)
        {
            if (!is_legal_trump(card))
            {
                return false;
            }
        }
        return true;
    }
    else if (std::holds_alternative<BOMB>(cards_combination))
    {
        BOMB data = std::get<BOMB>(cards_combination);
        return is_legal_trump(data.primary);
    }
    else if (std::holds_alternative<FOUR_WITH_TWO>(cards_combination))
    {
        FOUR_WITH_TWO data = std::get<FOUR_WITH_TWO>(cards_combination);
        return is_legal_trump(data.primary) && is_legal_card(data.secondary[0]) && is_legal_card(data.secondary[1]);
    }
    else if (std::holds_alternative<FOUR_WITH_TWO_PAIR>(cards_combination))
    {
        FOUR_WITH_TWO_PAIR data = std::get<FOUR_WITH_TWO_PAIR>(cards_combination);
        return is_legal_trump(data.primary) && is_legal_trump(data.secondary[0]) && is_legal_trump(data.secondary[1]);
    }
    else if (std::holds_alternative<ROCKET>(cards_combination))
    {
        return true;
    }
    return false;
}

bool compareCardsWithLast2(const CardCombination &card, const LastTwoTurns &last_2_turns_state)
{
    if (std::holds_alternative<PASS>(card) &&
        std::holds_alternative<PASS>(last_2_turns_state.lastTurn) &&
        std::holds_alternative<PASS>(last_2_turns_state.previousTurn))
    {
        std::cout << "other 2 is pass" << std::endl;
        return false;
    }
    if (!compareCards(card, last_2_turns_state.lastTurn))
    {
        std::cout << "false compare with lastTurn" << std::endl;
        return false;
    }
    if (!compareCards(card, last_2_turns_state.previousTurn))
    {
        std::cout << "false compare with previousTurn " << std::endl;
        return false;
    }
    return true;
}

bool compareCards(const CardCombination &cards1, const CardCombination &cards2)
{
    // printType(cards1);
    // printType(cards2);
    if (std::holds_alternative<PASS>(cards2))
    {
        return true;
    }
    if (std::holds_alternative<PASS>(cards1))
    {
        return true; // 这里注意一下, pass可以应对任何值, 但是, 不要在前两个pass的情况下继续pass
    }
    if (std::holds_alternative<ROCKET>(cards1))
    {
        return true;
    }
    if (std::holds_alternative<ROCKET>(cards2))
    {
        return false;
    }
    if (std::holds_alternative<BOMB>(cards1) && !std::holds_alternative<BOMB>(cards2))
    {
        return true;
    }
    if (std::holds_alternative<BOMB>(cards2) && !std::holds_alternative<BOMB>(cards1))
    {
        return false;
    }

    if (!holds_same_type(cards1, cards2))
        return false;

    if (std::holds_alternative<SINGLE>(cards1))
    {
        SINGLE data1 = std::get<SINGLE>(cards1);
        SINGLE data2 = std::get<SINGLE>(cards2);
        return data1.primary > data2.primary;
    }
    else if (std::holds_alternative<PAIR>(cards1))
    {
        PAIR data1 = std::get<PAIR>(cards1);
        PAIR data2 = std::get<PAIR>(cards2);
        return data1.primary > data2.primary;
    }
    else if (std::holds_alternative<TRIO>(cards1))
    {
        TRIO data1 = std::get<TRIO>(cards1);
        TRIO data2 = std::get<TRIO>(cards2);
        return data1.primary > data2.primary;
    }
    else if (std::holds_alternative<TrioWithOneData>(cards1))
    {
        TrioWithOneData data1 = std::get<TrioWithOneData>(cards1);
        TrioWithOneData data2 = std::get<TrioWithOneData>(cards2);
        return data1.primary > data2.primary;
    }
    else if (std::holds_alternative<TRIO_WITH_PAIR>(cards1))
    {
        TRIO_WITH_PAIR data1 = std::get<TRIO_WITH_PAIR>(cards1);
        TRIO_WITH_PAIR data2 = std::get<TRIO_WITH_PAIR>(cards2);
        return data1.primary > data2.primary;
    }
    else if (std::holds_alternative<STRAIGHT>(cards1))
    {
        STRAIGHT data1 = std::get<STRAIGHT>(cards1);
        STRAIGHT data2 = std::get<STRAIGHT>(cards2);

        return data1.straight[0] > data2.straight[0];
    }
    else if (std::holds_alternative<MULTI_PAIRS>(cards1))
    {
        MULTI_PAIRS data1 = std::get<MULTI_PAIRS>(cards1);
        MULTI_PAIRS data2 = std::get<MULTI_PAIRS>(cards2);
        return data1.multi_pairs[0] > data2.multi_pairs[0];
    }
    else if (std::holds_alternative<PLANE>(cards1))
    {
        PLANE data1 = std::get<PLANE>(cards1);
        PLANE data2 = std::get<PLANE>(cards2);
        return data1.primary[0] > data2.primary[0];
    }
    else if (std::holds_alternative<PLANE_WITH_SINGLE_WINGS>(cards1))
    {
        PLANE_WITH_SINGLE_WINGS data1 = std::get<PLANE_WITH_SINGLE_WINGS>(cards1);
        PLANE_WITH_SINGLE_WINGS data2 = std::get<PLANE_WITH_SINGLE_WINGS>(cards2);
        return data1.primary[0] > data2.primary[0];
    }
    else if (std::holds_alternative<PLANE_WITH_PAIR_WINGS>(cards1))
    {
        PLANE_WITH_PAIR_WINGS data1 = std::get<PLANE_WITH_PAIR_WINGS>(cards1);
        PLANE_WITH_PAIR_WINGS data2 = std::get<PLANE_WITH_PAIR_WINGS>(cards2);
        return data1.primary[0] > data2.primary[0];
    }
    else if (std::holds_alternative<BOMB>(cards1))
    {
        BOMB data1 = std::get<BOMB>(cards1);
        BOMB data2 = std::get<BOMB>(cards2);
        return data1.primary > data2.primary;
    }
    else if (std::holds_alternative<FOUR_WITH_TWO>(cards1))
    {
        FOUR_WITH_TWO data1 = std::get<FOUR_WITH_TWO>(cards1);
        FOUR_WITH_TWO data2 = std::get<FOUR_WITH_TWO>(cards2);
        return data1.primary > data2.primary;
    }
    else if (std::holds_alternative<FOUR_WITH_TWO_PAIR>(cards1))
    {
        FOUR_WITH_TWO_PAIR data1 = std::get<FOUR_WITH_TWO_PAIR>(cards1);
        FOUR_WITH_TWO_PAIR data2 = std::get<FOUR_WITH_TWO_PAIR>(cards2);
        return data1.primary > data2.primary;
    }
    std::cout << "invalid type for compare" << std::endl;
    return false;
}

std::vector<int> cardCombinationToVector(const CardCombination &cards_combination)
{
    std::vector<int> vec(21);

    if (std::holds_alternative<SINGLE>(cards_combination))
    {
        SINGLE data = std::get<SINGLE>(cards_combination);
        vec[data.primary]++;
    }
    else if (std::holds_alternative<PAIR>(cards_combination))
    {
        PAIR data = std::get<PAIR>(cards_combination);
        vec[data.primary] += 2;
    }
    else if (std::holds_alternative<TRIO>(cards_combination))
    {
        TRIO data = std::get<TRIO>(cards_combination);
        vec[data.primary] += 3;
    }
    else if (std::holds_alternative<TrioWithOneData>(cards_combination))
    {
        TrioWithOneData data = std::get<TrioWithOneData>(cards_combination);
        vec[data.primary] += 3;
        vec[data.secondary]++;
    }
    else if (std::holds_alternative<TRIO_WITH_PAIR>(cards_combination))
    {
        TRIO_WITH_PAIR data = std::get<TRIO_WITH_PAIR>(cards_combination);
        vec[data.primary] += 3;
        vec[data.secondary] += 2;
    }
    else if (std::holds_alternative<STRAIGHT>(cards_combination))
    {
        STRAIGHT data = std::get<STRAIGHT>(cards_combination);
        for (int card : data.straight)
        {
            vec[card]++;
        }
    }
    else if (std::holds_alternative<MULTI_PAIRS>(cards_combination))
    {
        MULTI_PAIRS data = std::get<MULTI_PAIRS>(cards_combination);
        for (int card : data.multi_pairs)
        {
            vec[card] += 2;
        }
    }
    else if (std::holds_alternative<PLANE>(cards_combination))
    {
        PLANE data = std::get<PLANE>(cards_combination);
        for (int card : data.primary)
        {
            vec[card] += 3;
        }
    }
    else if (std::holds_alternative<PLANE_WITH_SINGLE_WINGS>(cards_combination))
    {
        PLANE_WITH_SINGLE_WINGS data = std::get<PLANE_WITH_SINGLE_WINGS>(cards_combination);
        for (int card : data.primary)
        {
            vec[card] += 3;
        }
        for (int card : data.secondary)
        {
            vec[card]++;
        }
    }
    else if (std::holds_alternative<PLANE_WITH_PAIR_WINGS>(cards_combination))
    {
        PLANE_WITH_PAIR_WINGS data = std::get<PLANE_WITH_PAIR_WINGS>(cards_combination);
        for (int card : data.primary)
        {
            vec[card] += 3;
        }
        for (int card : data.secondary)
        {
            vec[card] += 2;
        }
    }
    else if (std::holds_alternative<BOMB>(cards_combination))
    {
        BOMB data = std::get<BOMB>(cards_combination);
        vec[data.primary] += 4;
    }
    else if (std::holds_alternative<FOUR_WITH_TWO>(cards_combination))
    {
        FOUR_WITH_TWO data = std::get<FOUR_WITH_TWO>(cards_combination);
        vec[data.primary] += 4;
        vec[data.secondary[0]]++;
        vec[data.secondary[1]]++;
    }
    else if (std::holds_alternative<FOUR_WITH_TWO_PAIR>(cards_combination))
    {
        FOUR_WITH_TWO_PAIR data = std::get<FOUR_WITH_TWO_PAIR>(cards_combination);
        vec[data.primary] += 4;
        vec[data.secondary[0]] += 2;
        vec[data.secondary[1]] += 2;
    }
    else if (std::holds_alternative<ROCKET>(cards_combination))
    {
        vec[(int)JokerType::BLACK_JOKER]++;
        vec[(int)JokerType::RED_JOKER]++;
    }
    return vec;
}

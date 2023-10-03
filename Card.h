#ifndef CARD_H
#define CARD_H
#include <variant>
#include <iostream>

enum class StandardCard
{
    THREE = 3,
    FOUR,
    FIVE,
    SIX,
    SEVEN,
    EIGHT,
    NINE,
    TEN,
    JACK,
    QUEEN,
    KING,
    ACE,
    TWO
};

enum class JokerType
{
    BLACK_JOKER = 19,
    RED_JOKER
};

inline bool is_legal_trump(int num)
{
    return (num >= static_cast<int>(StandardCard::THREE) && num <= static_cast<int>(StandardCard::TWO));
}

inline bool is_legal_card(int num)
{
    return is_legal_trump(num) || num == static_cast<int>(JokerType::BLACK_JOKER) || num == static_cast<int>(JokerType::RED_JOKER);
}

inline bool is_legal_3_to_A(int num){
    return (num >= static_cast<int>(StandardCard::THREE) && num <= static_cast<int>(StandardCard::ACE));
}

#endif
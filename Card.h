#ifndef CARD_H
#define CARD_H
#include <variant>
#include <iostream>

enum class Suit
{
    DIAMONDS,
    CLUBS,
    HEARTS,
    SPADES
    // 方片, 梅花, 红桃, 黑桃
};

enum class Rank
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
    BLACK_JOKER,
    RED_JOKER
};

struct StandardCard
{
    Suit suit;
    Rank rank;
};

// 使用方式：
// Card card1 = StandardCard{Suit::DIAMONDS, Rank::THREE};
// Card card2 = JokerType::BLACK_JOKER;
using Card = std::variant<StandardCard, JokerType>;

// deprecated:
int cardToInt(const Suit &suit, const Rank &rank);
int cardToInt(const JokerType &joker);
std::pair<Suit, Rank> intToCardStandard(int value);
JokerType intToCardJoker(int value);

// new:
template <typename T>
struct always_false : std::false_type{};
int cardToInt(const Card &card);
Card intToCard(int value);

// for log:
std::ostream &operator<<(std::ostream &os, const StandardCard &card);
std::ostream &operator<<(std::ostream &os, const JokerType &joker);
std::ostream &operator<<(std::ostream &os, const Card &card);

#endif
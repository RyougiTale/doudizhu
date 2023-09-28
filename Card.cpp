#include "Card.h"

int cardToInt(const Suit &suit, const Rank &rank)
{
    return static_cast<int>(suit) * 13 + static_cast<int>(rank) - 2;
}

int cardToInt(const JokerType &joker)
{
    return static_cast<int>(joker) + 53;
}

std::pair<Suit, Rank> intToCardStandard(int value)
{
    Suit suit = static_cast<Suit>((value - 1) / 13);
    Rank rank = static_cast<Rank>((value - 1) % 13 + 3);
    return {suit, rank};
}

JokerType intToCardJoker(int value)
{
    return static_cast<JokerType>(value - 53);
}

int cardToInt(const Card& card) {
    return std::visit([](auto&& arg) -> int {
        using T = std::decay_t<decltype(arg)>;
        // 14之后的别名, 等价于std::decay<T>::type
        if constexpr (std::is_same_v<T, StandardCard>) {
            return cardToInt(arg.suit, arg.rank);
        } else if constexpr (std::is_same_v<T, JokerType>) {
            return cardToInt(arg);
        } else {
            static_assert(always_false<T>::value, "Non-exhaustive visitor!");
        }
    }, card);
}

Card intToCard(int value) {
    if (value >= 1 && value <= 52) {
        auto [suit, rank] = intToCardStandard(value);
        return StandardCard{suit, rank};
    } else if (value == 53 || value == 54) {
        return intToCardJoker(value);
    } else {
        throw std::out_of_range("Invalid value for card conversion");
    }
}

std::ostream &operator<<(std::ostream &os, const StandardCard &card)
{
    static const char *suitNames[] = {"DIAMONDS", "CLUBS", "HEARTS", "SPADES"};
    // static const char *rankNames[] = {"", "", "", "THREE", "FOUR", "FIVE", "SIX", "SEVEN", "EIGHT", "NINE", "TEN", "JACK", "QUEEN", "KING", "ACE", "TWO"};
    static const char *rankNames[] = {"", "", "", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "1", "2"};
    os << cardToInt(card.suit, card.rank) << ": " << suitNames[static_cast<int>(card.suit)] << " " << rankNames[static_cast<int>(card.rank)] << ",";
    return os;
}

std::ostream &operator<<(std::ostream &os, const JokerType &joker)
{
    os << cardToInt(joker) << ": " << (joker == JokerType::BLACK_JOKER ? "BLACK_JOKER" : "RED_JOKER");
    return os;
}


std::ostream &operator<<(std::ostream &os, const Card &card)
{
    std::visit([&os](auto &&arg)
               { os << arg; },
               card);
    return os;
}

#include "BotForCpp.h"
#include "DemoBot.h"
#include <iostream>

void printCards(const std::vector<Card> &cards)
{
    for (auto it = cards.begin(); it != cards.end(); ++it)
    {
        std::cout << *it;
        if (it + 1 != cards.end())
        {
            std::cout << ' ';
        }
        else
        {
            std::cout << '\n';
        }
    }
}

void DemoBot::setPosition(int position)
{
    std::cout << "my position is " << position << '\n';
}

void DemoBot::init(const std::vector<Card> &initialCards)
{
    printCards(initialCards);
}

// 查询bot是否希望成为地主(暂时不需要)
bool DemoBot::wantToBeLandlord()
{
    return true;
}

// 告诉bot地主的底牌是什么
void DemoBot::notifyLandlordAndCards(const int landlord, const std::vector<Card> &landlordCards)
{
    printCards(landlordCards);
}

// 获取 bot 的出牌决策
std::vector<Card> DemoBot::play(const LastTwoTurns &lastTwoTurns)
{
    std::cout << "last card: ";
    printCards(lastTwoTurns.lastTurn);
    std::cout << "previous card: ";
    printCards(lastTwoTurns.previousTurn);
}

// 通知 bot 游戏结束
void DemoBot::notifyGameOver()
{
    // 做一些初始化
}

DemoBot::~DemoBot()
{
    // 做一些资源释放
}
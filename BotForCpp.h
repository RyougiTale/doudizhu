#ifndef BOT_INTERFACE_H
#define BOT_INTERFACE_H
#include "Card.h"
#include <vector>
#include <utility>
#include "CardCombination.h"


class BotInterface
{
public:
    // 设置玩家的位置 0, 1, 2
    virtual void setPosition(int position) = 0;

    // 发牌
    virtual void init(const std::vector<int> &initialCards) = 0;

    // 查询bot是否希望成为地主(暂时不需要)
    virtual bool wantToBeLandlord() = 0;

    // 告诉bot地主的底牌是什么
    virtual void notifyLandlordAndCards(const int landlord, const std::vector<int> &landlordCards) = 0;

    // 获取 bot 的出牌决策
    virtual CardCombination play(const LastTwoTurns &lastTwoTurns) = 0;

    // 通知 bot 游戏结束
    virtual void notifyGameOver() = 0;

    virtual ~BotInterface() = default;
};

#endif

#ifndef DEMO_BOT_H
#define DEMO_BOT_H

#include "BotForCpp.h"

class DemoBot : public BotInterface
{
public:
    // 设置玩家的位置 0, 1, 2
    void setPosition(int position);

    // 发牌
    void init(const std::vector<Card> &initialCards);

    // 查询bot是否希望成为地主(暂时不需要)
    bool wantToBeLandlord();

    // 告诉bot地主的底牌是什么
    void notifyLandlordAndCards(const int landlord, const std::vector<Card> &landlordCards);

    // 获取 bot 的出牌决策
    std::vector<Card> play(const LastTwoTurns &lastTwoTurns);

    // 通知 bot 游戏结束
    void notifyGameOver();

    ~DemoBot();
};

#endif
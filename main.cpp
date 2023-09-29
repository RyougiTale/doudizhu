#include <bits/stdc++.h>
#include "BotForCpp.h"
#include "Card.h"
#include "DemoBot.h"

constexpr int TOTAL_CARDS = 54;
constexpr int PLAYERS_NUM = 3;

class Game
{
private:
    std::vector<Card> deck;
    std::vector<Card> player_cards[PLAYERS_NUM], bottom_cards;
    std::unique_ptr<BotInterface> player[PLAYERS_NUM];
    int wins_sum[PLAYERS_NUM][2];
    int turn;

    void shuffleAndDeal()
    {
        std::shuffle(deck.begin(), deck.end(), std::mt19937{std::random_device{}()});

        for (int i = 0; i < TOTAL_CARDS; i++)
        {
            if (i < 51)
            {
                if (i % PLAYERS_NUM == 0)
                    player_cards[0].push_back(deck[i]);
                else if (i % PLAYERS_NUM == 1)
                    player_cards[1].push_back(deck[i]);
                else
                    player_cards[2].push_back(deck[i]);
            }
            else
            {
                bottom_cards.push_back(deck[i]);
            }
        }
    }

public:
    Game(std::unique_ptr<BotInterface> p1,
         std::unique_ptr<BotInterface> p2,
         std::unique_ptr<BotInterface> p3)
    {
        turn = 0;
        for (int i = 0; i < PLAYERS_NUM; i++)
        {
            wins_sum[i][0] = 0;
            wins_sum[i][1] = 0;
        }
        player[0] = std::move(p1);
        player[1] = std::move(p2);
        player[2] = std::move(p3);
        for (int s = 0; s < 4; ++s)
        {
            for (int r = 3; r <= 15; ++r)
            {
                deck.push_back(StandardCard{static_cast<Suit>(s), static_cast<Rank>(r)});
            }
        }
        deck.push_back(JokerType::BLACK_JOKER);
        deck.push_back(JokerType::RED_JOKER);
    }

    void print(std::vector<int> cards)
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

    bool isGameOver()
    {
        // 判断游戏是否结束的逻辑，例如某玩家手中的牌为空
        for (int i = 0; i < PLAYERS_NUM; ++i)
        {
            if (player_cards[i].empty())
            {
                return true;
            }
        }
        return false;
    }

    bool isValidMove(std::vector<Card> playedCards, LastTwoTurns last_2_turns_state)
    {
        return true;
        // // 这里仅仅是一个简化版的实现
        // // 你需要根据实际的游戏规则来判断

        // // 1. 检查玩家是否真的有要出的那些牌
        // if (!playerbot[static_cast<int>(currentPlayer)]->hasCards(playedCards))
        // {
        //     return false;
        // }

        // // 2. 检查出的牌的组合是否符合规定
        // // 你可能需要写一个函数来识别牌型
        // if (!isRecognizedCardPattern(playedCards))
        // {
        //     return false;
        // }

        // // 3. 检查出的牌是否比上一个玩家出的牌大
        // // 如果上一个玩家没出牌（例如：pass），那就接受这个玩家的牌
        // if (lastPlayedCards.empty() || compareCards(playedCards, lastPlayedCards) > 0)
        // {
        //     return true;
        // }

        return false;
    }

    void start()
    {
        // 洗牌分牌
        shuffleAndDeal();
        std::cout << "Player 1 cards: " << player_cards[0].size() << std::endl;
        std::cout << "Player 2 cards: " << player_cards[1].size() << std::endl;
        std::cout << "Player 3 cards: " << player_cards[2].size() << std::endl;
        std::cout << "Bottom cards: " << bottom_cards.size() << std::endl;
        // std::vector<int> cardInts(cards.size());
        // auto cardToInt = [](const Card &card) -> int
        // {
        //     return 0;
        // };
        // std::transform(cards.begin(), cards.end(), cardInts.begin(), cardToInt);

        // 发牌
        for (int i = 0; i < PLAYERS_NUM; i++)
        {
            player[i]->setPosition(i);
            player[i]->init(player_cards[i]);
        }

        // 确认地主(增加重发牌等功能, 结果也要统计农民胜利和地主胜利)
        int landlord = turn % 3;
        player_cards[landlord].insert(player_cards[landlord].end(),
                                      bottom_cards.begin(),
                                      bottom_cards.end());

        // 告知底牌
        for (int i = 0; i < PLAYERS_NUM; i++)
        {
            player[i]->notifyLandlordAndCards(landlord, bottom_cards);
        }

        int currentPlayer = landlord;
        LastTwoTurns last_2_turns_state;
        while (true)
        {
            std::vector<Card> playedCards = player[currentPlayer]->play(last_2_turns_state);

            // 验证出牌是否有效，并更新游戏状态
            if (isValidMove(playedCards, last_2_turns_state))
            {
                for (Card &card : playedCards)
                {
                    // player_cards[currentPlayer].erase()
                }
                last_2_turns_state.previousTurn = last_2_turns_state.lastTurn;
                last_2_turns_state.lastTurn = playedCards;

                // 你可以在这里判断是否有玩家已经出完牌
                if (player_cards[currentPlayer].empty())
                {
                    wins_sum[currentPlayer][currentPlayer == landlord]++;
                    break;
                }
            }

            // 轮到下一位玩家出牌
            currentPlayer = (currentPlayer + 1) % PLAYERS_NUM;
        }

        for (int i = 0; i < PLAYERS_NUM; ++i)
        {
            player[i]->notifyGameOver();
        }
    }
};

int main()
{
    // Game game;
    std::unique_ptr<BotInterface> bot1 = std::unique_ptr<BotInterface>(std::make_unique<DemoBot>().release());
    std::unique_ptr<BotInterface> bot2 = std::unique_ptr<BotInterface>(std::make_unique<DemoBot>().release());
    std::unique_ptr<BotInterface> bot3 = std::unique_ptr<BotInterface>(std::make_unique<DemoBot>().release());

    Game game(std::move(bot1), std::move(bot2), std::move(bot3));
    game.start();

    return 0;
}
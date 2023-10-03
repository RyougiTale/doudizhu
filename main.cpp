#include <bits/stdc++.h>
#include "BotForCpp.h"
#include "Card.h"
#include "DemoBot.h"
#include "CardCombination.h"
constexpr int TOTAL_CARDS = 54;
constexpr int PLAYERS_NUM = 3;

#define MAX_CARD_SIZE 21
class Game
{
private:
    std::vector<int> deck;
    int player_cards[PLAYERS_NUM][MAX_CARD_SIZE];
    std::vector<int> bottom_cards;
    std::unique_ptr<BotInterface> player[PLAYERS_NUM];
    int wins_sum[PLAYERS_NUM][2];
    int turn;

    void shuffleAndDeal()
    {
        std::shuffle(deck.begin(), deck.end(), std::mt19937{std::random_device{}()});
        for (int i = 0; i < PLAYERS_NUM; i++)
        {
            memset(player_cards[i], 0, sizeof(player_cards[i]));
        }
        bottom_cards.clear();
        for (int i = 0; i < TOTAL_CARDS; i++)
        {
            if (i < 51)
            {
                if (i % PLAYERS_NUM == 0)
                    player_cards[0][deck[i]]++;
                else if (i % PLAYERS_NUM == 1)
                    player_cards[1][deck[i]]++;
                else
                    player_cards[2][deck[i]]++;
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
                deck.push_back(r);
            }
        }
        deck.push_back((int)JokerType::BLACK_JOKER);
        deck.push_back((int)JokerType::RED_JOKER);
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

    bool isValidMove(CardCombination &playedCards, LastTwoTurns last_2_turns_state)
    {
        if (!detectCardType(playedCards))
        {
            std::cout << "type error" << std::endl;
            return false;
        }

        if (!compareCardsWithLast2(playedCards, last_2_turns_state))
        {
            std::cout << "compare error" << std::endl;
            return false;
        }
        return true;
    }

    void start()
    {
        // 洗牌分牌
        shuffleAndDeal();
        std::cout << "Bottom cards: " << bottom_cards.size() << std::endl;

        // 发牌
        for (int i = 0; i < PLAYERS_NUM; i++)
        {
            std::vector<int> vec;
            for (int j = 3; j < 21; j++)
            {
                for (int k = 0; k < player_cards[i][j]; k++)
                {
                    vec.emplace_back(j);
                }
            }
            player[i]->setPosition(i);
            player[i]->init(vec);
        }

        // 确认地主(增加重发牌等功能, 结果也要统计农民胜利和地主胜利)
        int landlord = turn % 3;
        for (int card : bottom_cards)
        {
            player_cards[landlord][card]++;
        }

        // 告知底牌
        for (int i = 0; i < PLAYERS_NUM; i++)
        {
            player[i]->notifyLandlordAndCards(landlord, bottom_cards);
        }

        int currentPlayer = landlord;
        LastTwoTurns last_2_turns_state;
        last_2_turns_state.lastTurn = PASS{};
        last_2_turns_state.previousTurn = PASS{};
        while (true)
        {
            std::cout << "here" << std::endl;
            CardCombination playedCards = player[currentPlayer]->play(last_2_turns_state);

            // 验证出牌是否有效，并更新游戏状态
            if (isValidMove(playedCards, last_2_turns_state))
            {
                std::vector<int> toplayed = cardCombinationToVector(playedCards);
                for (int i = 0; i < toplayed.size(); i++)
                {
                    if (player_cards[currentPlayer][i] - toplayed[i] < 0)
                    {
                        std::cout << "do not have enough cards" << std::endl;
                        return;
                    }
                }
                for (int i = 0; i < toplayed.size(); i++)
                {
                    player_cards[currentPlayer][i] -= toplayed[i];
                }
                last_2_turns_state.previousTurn = last_2_turns_state.lastTurn;
                last_2_turns_state.lastTurn = playedCards;

                // 你可以在这里判断是否有玩家已经出完牌
                int sum = 0;
                for (int i = 0; i < MAX_CARD_SIZE; i++)
                {
                    sum += player_cards[currentPlayer][i];
                }
                if (sum == 0)
                {
                    std::cout << "winner is " << currentPlayer << " " << (currentPlayer == landlord ? "landlord" : "farmer") << std::endl;
                    wins_sum[currentPlayer][currentPlayer == landlord]++;
                    break;
                }
            }
            else
            {
                std::cout << "the card played by " << currentPlayer << " is not valid" << std::endl;
                return;
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
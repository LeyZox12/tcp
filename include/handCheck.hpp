#include "Card.h"
#include <vector>
#include <algorithm>

class Card;

using namespace std;

enum Hand
{
    HIGH_CARD,
    PAIR,
    TWO_PAIR,
    THREE,
    STRAIGHT,
    FLUSH,
    FULL_HOUSE,
    FOUR,
    STRAIGHT_FLUSH
};

bool isPair(vector<Card> hand)
{
    for(auto& c : hand)
    {
        int target = c.getValue();
        if(count_if(hand.begin(), hand.end(), [target](auto& e){return e.getValue() == target;}) == 2) return true;
    }
    return false;
}

bool isThree(vector<Card> hand)
{
    for(auto& c : hand)
    {
        int target = c.getValue();
        if(count_if(hand.begin(), hand.end(), [target](auto& e){return e.getValue() == target;}) == 3) return true;
    }
    return false;
}

bool isFour(vector<Card> hand)
{
    for(auto& c : hand)
    {
        int target = c.getValue();
        if(count_if(hand.begin(), hand.end(), [target](auto& e){return e.getValue() == target;}) == 4) return true;
    }
    return false;
}

bool isStraight(vector<Card> hand)
{
    sort(hand.begin(), hand.end(), [](auto& i1, auto i2){return i1.getValue() > i2.getValue();});
    for(int i = 0; i < hand.size()-1; i++)
    {
        if(hand[i].getValue() - hand[i+1].getValue() != 1) return false;
    }
    return true;
}

bool isFlush(vector<Card> hand)
{
    int suit = hand[0].getSuit();
    return count_if(hand.begin(), hand.end(), [suit](auto& e){return e.getSuit() == suit;}) == 5;
}

bool isTwoPair(vector<Card> hand)
{
    int firstPairVal = -1;
    int pairs = 0;
    for(auto& c : hand)
    {
        int target = c.getValue();
        if(count_if(hand.begin(), hand.end(), [target](auto& e){return e.getValue() == target;}) >= 2 && firstPairVal != target) pairs++;
        firstPairVal = target;
        if(pairs == 2)return true;
    }
    return false;
}

int getHand(vector<Card> hand)
{
    if(isStraight(hand) && isFlush(hand)) return STRAIGHT_FLUSH;
    if(isFour(hand)) return FOUR;
    if(isTwoPair(hand) && isThree(hand)) return FULL_HOUSE;
    if(isFlush(hand)) return FLUSH;
    if(isStraight(hand)) return STRAIGHT;
    if(isThree(hand)) return THREE;
    if(isTwoPair(hand)) return TWO_PAIR;
    if(isPair(hand)) return PAIR;
    return HIGH_CARD;
}

int getWinner(vector<Card> h1, vector<Card> h2)
{
    int h1v = getHand(h1);
    int h2v = getHand(h2);
    if(h1v > h2v) return 0;
    else if(h2v > h1v)return 1;
    /*else
    {
        sort(h1.begin(), h1.end(), [](auto& c1, auto& c2){return c1.getValue()>c2.getValue();});
        sort(h2.begin(), h2.end(), [](auto& c1, auto& c2){return c1.getValue()>c2.getValue();});
        int card1, card2;
        switch(h1v)
        {
            case(HIGH_CARD):
                card1 = h1[0].getValue();
                card2 = h2[0].getValue();
                break;
            case(PAIR):
                auto it1 = find_if(h1.begin(), h1.end(), 
                [h1](auto& i)
                {
                    return count_if(h1.begin(), h1.end(), [i](auto& j){return j == i}) == 2;
                });
                auto it2 = find_if(h1.begin(), h1.end(), 
                [h2](auto& i)
                {
                    return count_if(h2.begin(), h2.end(), [i](auto& j){return j == i}) == 2;
                });
                card1 = h1[distance(h1.begin(), it1)].getValue();
                card2 = h2[distance(h2.begin(), it2)].getValue();
                break;
            case(TWO_PAIR):
                int firstPairNum = -1;
                int pairs = 0;
                for(auto& c : h1)
                {
                    int target = c.getValue();
                    if(count_if(h1.begin(), h1.end(), [target](auto& i){return i.getValue() == target;}) == 2 && firstPairNum != target)
                    {
                        pairs++;
                        if(pairs == 2)
                        {
                            card1 = target;
                            card2 = firstPairNum;
                            break;
                        }
                        firstPairNum = target;
                    }
                }
                break;
        }
    }*/
}
evalHands(vector<Hand> h1, vector<Hand> h2)
{

}

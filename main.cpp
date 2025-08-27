#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <string.h>
#include <vector>
#include <math.h>
#include "Card.h"

using namespace std;
using namespace sf;

const string LOOKUP_TABLE = "";

///TODO discard cards + looping game + bet amount + draw skin 

typedef Vector2f vec2;
typedef Vector2u vec2u;
typedef Vector2i vec2i;

string encode(string str);
string decode(string str);
void onResize(vec2u size);

Font font("res/font.ttf");
RectangleShape man;
RectangleShape table;
vec2 cardSize;
vector<Card> cards;//0-4 self 5-9 enemy

void initDeck(vector<Card> &deck);
void shuffle(vector<Card> &deck);
vec2 getCardPos(int index, bool clientPlayer, vec2 cardSize, vec2u winSize);

enum GameState
{
    WAITING_PLAYERS,
    GIVING_CARDS,
    CHOOSING_CARDS_DISCARD,
    RESULTS
};

int gameState = GameState::WAITING_PLAYERS;

int main(int argc, char* argv[])
{
    if(argc > 1 && static_cast<string>(argv[1]) == "-h")
    {
        TcpListener listener;
        listener.listen(atoi(argv[2]));
        cout << "server launched on port: " << argv[2] << endl << "code is:" <<encode(IpAddress::getPublicAddress()->toString()) << endl;
        int playerCount = 0;
        vector<TcpSocket> clients;
        clients.emplace_back(TcpSocket());
        clients[0].setBlocking(false);
        while(playerCount < 2)
        {
            if(listener.accept(clients[playerCount]) == Socket::Status::Done)
            {
                clients.emplace_back(TcpSocket());
                clients[clients.size()-1].setBlocking(false);
                cout << "player " << clients[playerCount].getRemoteAddress().value() << endl;
                playerCount++;
            }
        }
        gameState = GameState::GIVING_CARDS;
        Packet p1;
        p1 << "Game Start";
        Packet p2;
        for(int i = 0; i < 2; i++)
            clients[i].send(p1);
        vector<Card> deck;
        initDeck(deck);
        shuffle(deck);
        p1.clear();
        for(int i = 0; i < 10; i++)
        {
            p1 << to_string(deck[i].getValue()) << ";" << to_string(deck[i].getSuit()) << ";";
            p2 << to_string(deck[9 - i].getValue()) << ";" << to_string(deck[9 - i].getSuit()) << ";";
        }
        clients[0].send(p1);
        clients[1].send(p2);
        for(int _ = 0; _ < 10; _++) deck.erase(deck.begin());
        gameState = GameState::CHOOSING_CARDS_DISCARD;
    }
    else
    {
        TcpSocket client;
        string code = "";
        int port;
        if(argc > 2)
        {
            code = argv[1];
            port = atoi(argv[2]);
        }
        else
        {
            cout << "code:";
            cin >> code;
            cout << "\nport:";
            cin >> port;
            cout << endl;
        }
        string decoded = decode(code);
        IpAddress ip = static_cast<IpAddress>(IpAddress::resolve("localhost").value());
        string err = (client.connect(ip, port) == Socket::Status::Done) ? "connected successfully" : "failed to connect";
        Texture manTex("res/man.png");
        man.setTexture(&manTex);
        Texture tableTex("res/table.png");
        table.setTexture(&tableTex);
        clock_t animStart = clock();
        int animState = 0;
        Packet gameStartPacket;
        if(client.receive(gameStartPacket) == Socket::Status::Done)
        {
            string out = "";
            gameStartPacket >> out;
            cout << out << endl;
            if(out == "Game Start") gameState = GameState::GIVING_CARDS;
        }
        client.setBlocking(false);
        RenderWindow window(VideoMode({512, 512}), "crippling addiction");
        window.setView(View({256, 256}, {512, 512}));
        onResize(vec2u(512, 512));
        Packet cardPacket;
        while(window.isOpen())
        {
            while(optional<Event> e = window.pollEvent())
            {
                if(e->is<Event::Closed>())window.close();
                if(e->is<Event::Resized>())
                {
                    vec2u size = e->getIf<Event::Resized>()->size;
                    window.setView(View({size.x * 0.5f, size.y * 0.5f}, {size.x, size.y}));
                    onResize(size);
                }
            }
            if(client.receive(cardPacket) == Socket::Status::Done)
            {
                for(int i = 0; i < 10; i++)
                {
                    string value = "";
                    string suit = "";
                    string dump = "";
                    cardPacket >> value >> dump >> suit >> dump;
                    cout << value << dump << suit << endl;
                    cards.emplace_back(Card(stoi(value), stoi(suit), getCardPos(i%5, i < 5, cardSize, window.getSize())));
                }
            }
            if(clock() - animStart >= 1000)
            {
                animStart = clock();
                animState++;
            }
            man.setTextureRect(Rect<int>({animState%2 == 0 ? 0 : 51, 0}, {51, 72}));
            window.clear(Color::White);
            window.draw(man);
            window.draw(table);
            for(auto& c : cards)
            {
                c.draw(window, cardSize);
            }
            window.display();
        }
    }
    return 0;
}

string encode(string str)
{
    string out = "";
    for(auto& c : str)
    {
        int index = 0;
        if(c != '.') index = c-'0'+1;
        out += LOOKUP_TABLE[index];
    }
    return out;
}

string decode(string str)
{
    string out = "";
    for(auto& c : str)
    {
        for(int i = 0; i < LOOKUP_TABLE.size(); i++)
        {
            if(c == LOOKUP_TABLE[0])
            {
                out += ".";
                break;
            }
            else if(c == LOOKUP_TABLE[i])
            {
                out += to_string(i-1);
                break;
            }
        }
    }
    return out;
}

void onResize(vec2u size)
{
    float half = size.y * 0.5f;
    man.setSize({half * 0.7f, half});
    man.setOrigin(man.getSize() / 2.f);
    man.setPosition({size.x * 0.5f, size.y * 0.25f});
    table.setSize({size.y * 0.75f, size.y * 0.75f});
    table.setOrigin(table.getSize() * 0.5f);
    table.setPosition({size.x * 0.5f, size.y * 0.75f});
    cardSize = vec2(table.getSize().x / 12.f, table.getSize().y / 6.f);
    for(int i = 0; i < cards.size(); i++)
    {
        cards[i].setPosition(getCardPos(i%5, i < 5, cardSize, size));
    }
}

void initDeck(vector<Card> &deck)
{
    for(uint8_t s = 0; s < 4; s++)
    {
        for(uint8_t v = 0; v < 13; v++)
        {
            deck.emplace_back(Card(v, s, vec2(0, 0)));
        }
    }
}

vec2 getCardPos(int index, bool clientPlayer, vec2 cardSize, vec2u winSize)
{
    vec2 tablePos = vec2(winSize.x * 0.5f, winSize.y * 0.75f); 

    float y = clientPlayer? -cardSize.y : cardSize.y;
    return tablePos + vec2((index-2.5f) * cardSize.x, y); 
}

void shuffle(vector<Card> &deck)
{
    for(int i = 0; i < deck.size(); i++)
    {
        int swappingIndex = rand()%deck.size();
        Card buffer = deck[i];
        deck[i] = deck[swappingIndex];
        deck[swappingIndex] = buffer;
    }
}

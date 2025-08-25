#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <string.h>

using namespace std;
using namespace sf;

const string LOOKUP_TABLE = "BAOXINZFSWD";

string encode(string str);
string decode(string str);

int main(int argc, char* argv[])
{
    if(argc > 1 && static_cast<string>(argv[1]) == "-h")
    {
        TcpListener listener;
        listener.listen(2887);
        cout << "server launched on port: " << 2887 << endl << "code is:" <<encode(IpAddress::getPublicAddress()->toString());
        int playerCount = 0;
        vector<TcpSocket> clients;
        clients.emplace_back(TcpSocket());
        while(playerCount < 2)
        {
            if(listener.accept(clients[playerCount]) == Socket::Status::Done)
            {
                clients.emplace_back(TcpSocket());
                cout << "player " << clients[playerCount].getRemoteAddress().value() << endl;
                playerCount++;
            }
        }
    }
    else
    {
        TcpSocket client;
        string code = "";
        cin >> code;
        string decoded = decode(code);
        IpAddress ip = static_cast<IpAddress>(IpAddress::resolve("localhost").value());
        cout << (client.connect(ip, 2887) == Socket::Status::Done) ? "connected successfully" : "failed to connect";
        //string message = "";
        //cin >> message;
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

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <ctime> 

std::vector<std::vector<std::vector<int>>> players;
// players, rounds, values, value

void initialise() {
    std::ifstream file("../input/day19.txt");
    std::string row;

    int player;

    while(std::getline(file, row)) {
        if(row.empty()) {
            continue;
        }

        if(row.find("Player") != std::string::npos) {
            player = stoi(row.substr(row.find("Player")+7, 1)) - 1;
            players.push_back(std::vector<std::vector<int>>());
            players[player].push_back(std::vector<int>());
            continue;
        }

        players[player][0].push_back(stoi(row));
    }
}

int part1() {
    std::vector<int> *winnerScore;
    std::vector<int> *loserScore;

    // unchanging for part 1, so will need to reinitialise for part 2
    int round = 0;

    while(players[0][round].size() && players[1][round].size() && players[0][round][0] != players[1][round][0]) {
        if(players[0][round][0] > players[1][round][0]) {
            winnerScore = &players[0][round];
            loserScore = &players[1][round];
        }
        else {
            winnerScore = &players[1][round];
            loserScore = &players[0][round];
        }

        winnerScore->push_back(winnerScore->at(0));
        winnerScore->erase(winnerScore->begin(), winnerScore->begin()+1);
        winnerScore->push_back(loserScore->at(0));
        loserScore->erase(loserScore->begin(), loserScore->begin()+1);
    }

    int score = 0;
    for(int i = winnerScore->size()-1; i >= 0; i--) {
        score += (winnerScore->size()-i) * winnerScore->at(i);
    }

    return score; 
}

// erroring somewhere
int part2() {
    std::vector<int> *winnerScore;
    std::vector<int> *loserScore;
    int round = 1;

    // what should this be?
    while(players[0][round].size() && players[1][round].size() && players[0][round][0] != players[1][round][0]) {
        // copy previous history into current
        for(int i = 0; i < players.size(); i++) {
            players[i].push_back(players[i][round-1]);
        }

        // moving cards around
        while(players[0][round].size() && players[1][round].size() && players[0][round][0] != players[1][round][0]) {
            if(players[0][round][0] > players[1][round][0]) {
                winnerScore = &players[0][round];
                loserScore = &players[1][round];
            }
            else {
                winnerScore = &players[1][round];
                loserScore = &players[0][round];
            }

            winnerScore->push_back(winnerScore->at(0));
            winnerScore->erase(winnerScore->begin(), winnerScore->begin()+1);
            winnerScore->push_back(loserScore->at(0));
            loserScore->erase(loserScore->begin(), loserScore->begin()+1);
        }

        // check if this configuration has existed before
        bool bothSame = true;
        for(int i = 0; i < players.size(); i++) 
        {
            if(!bothSame) {
                break;
            }

            for(int j = 0; j < round; j++) 
            { 
                if(!bothSame) {
                    break;
                }

                if(players[i][j].size() != players[i][round].size()) 
                {
                        bothSame = false;
                        break;
                }

                for(int k = 0; k < players[i][j].size(); k++) 
                {
                    if(players[i][j][k] != players[i][round][k]) {
                        bothSame = false;
                        break;
                    }
                }
            }
        }

        // if configuration has existed before, player 1 (0) wins
        if(bothSame) {
            winnerScore = &players[0][round];
            break;
        }

        round++;
    }

    // calculate score of winner
    int score = 0;
    for(int i = winnerScore->size()-1; i >= 0; i--) {
        score += (winnerScore->size()-i) * winnerScore->at(i);
    }
    
    return 0;
}

int main()
{
    initialise();

    int value;
    auto start = std::chrono::system_clock::now();
    value = part1();
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;
    
    std::cout << "Part 1: " << value << std::endl;
    std::cout << "Completed in: " << elapsed_seconds.count() << std::endl;

    initialise();

    value;
    start = std::chrono::system_clock::now();
    value = part2();
    end = std::chrono::system_clock::now();
    elapsed_seconds = end-start;
    
    std::cout << "Part 2: " << value << std::endl;
    std::cout << "Completed in: " << elapsed_seconds.count() << std::endl;
}
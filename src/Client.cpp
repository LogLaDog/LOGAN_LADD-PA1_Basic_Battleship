//    Battleship game assignment for MSU CSCI 366
//    Copyright (C) 2020    Mike P. Wittie
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include <iomanip>
#include "common.hpp"
#include "Client.hpp"

Client::~Client() {
}


void Client::initialize(unsigned int player, unsigned int board_size) {

    if (player == 1) { //Checks Player Number
        ofstream p1ac;
        p1ac.open("player_1.action_board.json"); //Opens correct action board file

        //Creates an action board full of 0's based off board size

        p1ac << "{\n";
        p1ac << "    \"board\": [\n";
        for (int i = 1; i < board_size; i++){
            p1ac << "        [\n";
            for (int i = 1; i < board_size; i++) {
                p1ac << "            0,\n";
            }
            p1ac << "            0\n";
            p1ac << "        ],\n";
        }

        p1ac << "        [\n";
        for (int i = 1; i < board_size; i++) {
            p1ac << "            0,\n";
        }
        p1ac << "            0\n";
        p1ac << "        ]\n";
        p1ac << "    ]\n";
        p1ac << "}";
    }

    else if (player == 2) { //Same code except for player 2
        ofstream p2ac;
        p2ac.open("player_2.action_board.json");
        p2ac << "{\n";
        p2ac << "    \"board\": [\n";
        for (int i = 1; i < board_size; i++){
            p2ac << "        [\n";
            for (int i = 1; i < board_size; i++) {
                p2ac << "            0,\n";
            }
            p2ac << "            0\n";
            p2ac << "        ],\n";
        }

        p2ac << "        [\n";
        for (int i = 1; i < board_size; i++) {
            p2ac << "            0,\n";
        }
        p2ac << "            0\n";
        p2ac << "        ]\n";
        p2ac << "    ]\n";
        p2ac << "}";
        p2ac.close();
    }

    //Throw error for wrong player
    else{
        cout << "Incorrect Player Number" << "\n";
    }
}


void Client::fire(unsigned int x, unsigned int y) {

    //Create proper strings for file based off coordinates
    string x_input = "    \"x\": "+std::to_string(x)+",\n";
    string y_input = "    \"y\": "+std::to_string(y)+"\n";

    //Opens shot file and writes strings created above
    ofstream p1shot;
    p1shot.open("player_1.shot.json");
    p1shot << "{\n";
    p1shot << x_input;
    p1shot << y_input;
    p1shot << "}";
    p1shot.close();
}


bool Client::result_available() {

    //Checks if result file exists, throws error if not
    ifstream resultfile;
    resultfile.open("player_1.result.json");
    if(resultfile) {
        return true;
    } else {
        return false;
    }
}




int Client::get_result() {
    string STRING;
    string STRING2;
    ifstream result;
    result.open("player_1.result.json"); //open result files
    getline(result, STRING);
    getline(result, STRING2);

    //This part is a little strange, but it checks a specific character in the result file to check for a hit, miss or OOB.

    //Check for hit
    if (STRING2[14] == '1'){
        remove("player_1.result.json");
        result.close();
        return HIT;
    }

    //Check for miss
    else if (STRING2[14] == '-'){
        remove("player_1.result.json");
        result.close();
        return MISS;
    }

    //Check for OOB
    else if (STRING2[14] == '0'){
        remove("player_1.result.json");
        result.close();
        return OUT_OF_BOUNDS;
    }

    //Throws Error and deletes file if something is wrong.
    else{
        remove("player_1.result.json");
        result.close();
        throw 0;
    }
}



void Client::update_action_board(int result, unsigned int x, unsigned int y) {
    int bs = BOARD_SIZE; //Grab board size from the common file
    if (result == 1) { //Checks if result is hit
        ofstream p1shot;
        p1shot.open("player_1.action_board.json"); //Open file and write header
        p1shot << "{\n";
        p1shot << "    \"board\": [\n";

        //This part is wild, but it successfully records hit or miss on the right coordinate on any size action board.
        //Tested in a separate program with board sizes 2-10.
        for (int i = 0; i < bs - 1; i++) {
            if (x == i) {
                p1shot << "        [\n";
                for (int j = 0; j < bs - 1; j++) {
                    if (y == j) {
                        p1shot << "            1,\n";
                    }
                    else {
                        p1shot << "            0,\n";
                    }
                }
                if (y == bs - 1) {
                    p1shot << "            1\n";
                }
                else {
                    p1shot << "            0\n";
                }
                p1shot << "        ],\n";
            } else {
                p1shot << "        [\n";
                for (int j = 0; j < bs - 1; j++) {
                    p1shot << "            0,\n";
                }
                p1shot << "            0\n";
                p1shot << "        ],\n";

            }
        }

        if (x == bs - 1) {
            p1shot << "        [\n";
            for (int j = 0; j < bs - 1; j++) {
                if (y == j) {
                    p1shot << "            1,\n";
                } else {
                    p1shot << "            0,\n";
                }
            }
            if (y == bs - 1) {
                p1shot << "            1\n";
            } else {
                p1shot << "            0\n";
            }
            p1shot << "        ]\n";
        } else {
            p1shot << "        [\n";
            for (int j = 0; j < bs - 1; j++) {
                p1shot << "            0,\n";
            }
            p1shot << "            0\n";
            p1shot << "        ]\n";

        }

        p1shot << "    ]\n";
        p1shot << "}";
    }

    //Same code but for a miss
    else if (result == -1) {
        ofstream p1shot;
        p1shot.open("player_1.action_board.json");
        p1shot << "{\n";
        p1shot << "    \"board\": [\n";
        for (int i = 0; i < bs - 1; i++) {
            if (x == i) {
                p1shot << "        [\n";
                for (int j = 0; j < bs - 1; j++) {
                    if (x == j) {
                        p1shot << "            -1,\n";
                    } else {
                        p1shot << "            0,\n";
                    }
                }
                if (y == bs - 1) {
                    p1shot << "            -1\n";
                } else {
                    p1shot << "            0\n";
                }
                p1shot << "        ],\n";
            } else {
                p1shot << "        [\n";
                for (int j = 0; j < bs - 1; j++) {
                    p1shot << "            0,\n";
                }
                p1shot << "            0\n";
                p1shot << "        ],\n";

            }

        }

        if (x == bs - 1) {
            p1shot << "        [\n";
            for (int j = 0; j < bs - 1; j++) {
                if (y == j) {
                    p1shot << "            -1,\n";
                } else {
                    p1shot << "            0,\n";
                }
            }
            if (y == bs - 1) {
                p1shot << "            -1\n";
            } else {
                p1shot << "            0\n";
            }
            p1shot << "        ]\n";
        } else {
            p1shot << "        [\n";
            for (int j = 0; j < bs - 1; j++) {
                p1shot << "            0,\n";
            }
            p1shot << "            0\n";
            p1shot << "        ]\n";

        }

        p1shot << "    ]\n";
        p1shot << "}";
    }
}



string Client::render_action_board(){
}
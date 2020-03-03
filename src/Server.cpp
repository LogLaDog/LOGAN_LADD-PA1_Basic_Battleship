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


#include "common.hpp"
#include "Server.hpp"
#include <fstream>

/**
 * Calculate the length of a file (helper function)
 *
 * @param file - the file whose length we want to query
 * @return length of the file in bytes
 */
int get_file_length(ifstream *file){
}



void Server::initialize(unsigned int board_size, string p1_setup_board, string p2_setup_board) {
    //Performs init of set_up_boards
    fstream setupb;
    int counter = 0;
    setupb.open(p1_setup_board ,ios::in);
    //Reads setup board size and adds to counter per line, so that we can compare to board_size
    if (setupb.is_open()){
        string tp;
        while(getline(setupb, tp)){
            counter++;
        }
        setupb.close();
    }
    //Throw error if board size is wrong
    if (board_size != counter){
        throw "The board is not the correct size";
    }
    //Same
    // code but for player 2 setup board
    fstream setupb2;
    int counter2 = 0;
    setupb2.open(p2_setup_board ,ios::in);
    if (setupb2.is_open()){
        string tp;
        while(getline(setupb2, tp)){
            counter2++;
        }
        setupb2.close();
    }
    if (board_size != counter2){
        throw "The board is not the correct size";
    }

    //Error throwing if setup board is not the correct file
    if (p1_setup_board != "player_1.setup_board.txt"){
        throw "Wrong File";
    }
    if (p2_setup_board != "player_2.setup_board.txt"){
        throw "Wrong File";
    }


}

int Server::evaluate_shot(unsigned int player, unsigned int x, unsigned int y) {
    //Error throwing for bad player numbers.
    if (player < 1) {
        throw "Invalid Player Number";
    }
    if (player > 2) {
        throw "Invalid Player Number";
    }

    //Throwing out of bounds for incorrect board sizes
    if (x >= BOARD_SIZE){
        return OUT_OF_BOUNDS;
    }
    if (y >= BOARD_SIZE){
        return OUT_OF_BOUNDS;
    }

    //Evaluating shot for each player
    if (player == 1) {
        fstream newfile;
        int counter = 0;
        newfile.open("player_2.setup_board.txt", ios::in); //open set up board

        /* This portion successfully checks for Hits and misses, It goes through the file line by line,
        If the value of the line matches the y coordinate, it then compares the x coordinate value to the correct character to
         see if there is a hit or miss. This was probably easier done in an array but this should work for any board size. */
        if (newfile.is_open()) {
            string tp;
            while (getline(newfile, tp)) {
                if (counter == y) {
                    if (tp[x] == 'D' || tp[x] == 'C' || tp[x] == 'R' || tp[x] == 'B' || tp[x] == 'S') {
                        return HIT;
                    } else if (tp[x] != 'D' || tp[x] != 'C' || tp[x] != 'R' || tp[x] != 'B' || tp[x] != 'S') {
                        return MISS;
                    }
                }
                counter++;
            }
            newfile.close();
        }

    }

    //Same code for player 2
    if (player == 2) {
            fstream newfile;
            int counter = 0;
            newfile.open("player_2.setup_board.txt", ios::in);
            if (newfile.is_open()) {
                string tp;
                while (getline(newfile, tp)) {
                    if (counter == y) {
                        if (tp[x] == 'D' || tp[x] == 'C' || tp[x] == 'R' || tp[x] == 'B' || tp[x] == 'S') {
                            return HIT;
                        } else if (tp[x] != 'D' || tp[x] != 'C' || tp[x] != 'R' || tp[x] != 'B' || tp[x] != 'S') {
                            return MISS;
                        }
                    }
                    counter++;
                }
                newfile.close();
            }

        }
    }


int Server::process_shot(unsigned int player) {
    //Error throwing for bad player numbers.
    if (player < 1) {
        throw "Invalid Player Number";
    }
    if (player > 2) {
        throw "Invalid Player Number";
    }

    string STRING;

    /*This portion gets the shot coordinated from the shot file and passes the information of evaluate_shot to see if there is a hit or miss
     After processing the shot file it successfully returns and result file */
    if (player == 1) {
        ifstream p1shot;
        p1shot.open("player_1.shot.json"); //Open file
        getline(p1shot, STRING);
        char px = STRING[6]; //check coordinates based off characters.
        char py = STRING[13];

        if (STRING[6] == '1' && STRING[7] == '0') {
            int intx = 10;
            int inty = py - 48;
            int result = evaluate_shot(1, intx, inty); //Calls evaluate shot
            if (result == 1) {
                string hit_result = "{\n"
                                    "    \"result\": " + to_string(HIT) + "\n"
                                                                          "}";
                ofstream outputFile;
                outputFile.open("player_1.result.json");
                outputFile << hit_result;
                outputFile.close();
                remove("player_1.shot.json");
                return SHOT_FILE_PROCESSED;
            }

                //Returns result file for miss
            else if (result == -1) {
                string hit_result = "{\n"
                                    "    \"result\": " + to_string(MISS) + "\n"
                                                                           "}";
                ofstream outputFile;
                outputFile.open("player_1.result.json");
                outputFile << hit_result;
                outputFile.close();
                remove("player_1.shot.json");
                return SHOT_FILE_PROCESSED;
            }
                //Returns result file for oob
            else if (result == 0) {
                string hit_result = "{\n"
                                    "    \"result\": " + to_string(OUT_OF_BOUNDS) + "\n"
                                                                                    "}";
                ofstream outputFile;
                outputFile.open("player_1.result.json");
                outputFile << hit_result;
                outputFile.close();

                remove("player_1.shot.json");
                return SHOT_FILE_PROCESSED;
            }
        } else if (STRING[13] == '1' && STRING[14] == '0') {
            int intx = px - 48;
            int inty = 10;
            int result = evaluate_shot(1, intx, inty); //Calls evaluate shot
            if (result == 1) {
                string hit_result = "{\n"
                                    "    \"result\": " + to_string(HIT) + "\n"
                                                                          "}";
                ofstream outputFile;
                outputFile.open("player_1.result.json");
                outputFile << hit_result;
                outputFile.close();
                remove("player_1.shot.json");
                return SHOT_FILE_PROCESSED;
            }

                //Returns result file for miss
            else if (result == -1) {
                string hit_result = "{\n"
                                    "    \"result\": " + to_string(MISS) + "\n"
                                                                           "}";
                ofstream outputFile;
                outputFile.open("player_1.result.json");
                outputFile << hit_result;
                outputFile.close();
                remove("player_1.shot.json");
                return SHOT_FILE_PROCESSED;
            }
                //Returns result file for oob
            else if (result == 0) {
                string hit_result = "{\n"
                                    "    \"result\": " + to_string(OUT_OF_BOUNDS) + "\n"
                                                                                    "}";
                ofstream outputFile;
                outputFile.open("player_1.result.json");
                outputFile << hit_result;
                outputFile.close();

                remove("player_1.shot.json");
                return SHOT_FILE_PROCESSED;
            }
        } else {
            int intx = px - 48; // switch format to int from ACSII
            int inty = py - 48;
            int result = evaluate_shot(1, intx, inty); //Calls evaluate shot
            if (result == 1) {
                string hit_result = "{\n"
                                    "    \"result\": " + to_string(HIT) + "\n"
                                                                          "}";
                ofstream outputFile;
                outputFile.open("player_1.result.json");
                outputFile << hit_result;
                outputFile.close();
                remove("player_1.shot.json");
                return SHOT_FILE_PROCESSED;
            }

                //Returns result file for miss
            else if (result == -1) {
                string hit_result = "{\n"
                                    "    \"result\": " + to_string(MISS) + "\n"
                                                                           "}";
                ofstream outputFile;
                outputFile.open("player_1.result.json");
                outputFile << hit_result;
                outputFile.close();
                remove("player_1.shot.json");
                return SHOT_FILE_PROCESSED;
            }
                //Returns result file for oob
            else if (result == 0) {
                string hit_result = "{\n"
                                    "    \"result\": " + to_string(OUT_OF_BOUNDS) + "\n"
                                                                                    "}";
                ofstream outputFile;
                outputFile.open("player_1.result.json");
                outputFile << hit_result;
                outputFile.close();

                remove("player_1.shot.json");
                return SHOT_FILE_PROCESSED;
            }
        }

    }

    if (player == 2) {
        ifstream p2shot;
        p2shot.open("player_2.shot.json"); //Open file
        getline(p2shot, STRING);
        char px = STRING[6]; //check coordinates based off characters.
        char py = STRING[13];

        if (STRING[6] == '1' && STRING[7] == '0') {
            int intx = 10;
            int inty = py - 48;
            int result = evaluate_shot(1, intx, inty); //Calls evaluate shot
            if (result == 1) {
                string hit_result = "{\n"
                                    "    \"result\": " + to_string(HIT) + "\n"
                                                                          "}";
                ofstream outputFile;
                outputFile.open("player_2.result.json");
                outputFile << hit_result;
                outputFile.close();
                remove("player_2.shot.json");
                return SHOT_FILE_PROCESSED;
            }

                //Returns result file for miss
            else if (result == -1) {
                string hit_result = "{\n"
                                    "    \"result\": " + to_string(MISS) + "\n"
                                                                           "}";
                ofstream outputFile;
                outputFile.open("player_2.result.json");
                outputFile << hit_result;
                outputFile.close();
                remove("player_2.shot.json");
                return SHOT_FILE_PROCESSED;
            }
                //Returns result file for oob
            else if (result == 0) {
                string hit_result = "{\n"
                                    "    \"result\": " + to_string(OUT_OF_BOUNDS) + "\n"
                                                                                    "}";
                ofstream outputFile;
                outputFile.open("player_2.result.json");
                outputFile << hit_result;
                outputFile.close();

                remove("player_2.shot.json");
                return SHOT_FILE_PROCESSED;
            }
        } else if (STRING[13] == '1' && STRING[14] == '0') {
            int intx = px - 48;
            int inty = 10;
            int result = evaluate_shot(1, intx, inty); //Calls evaluate shot
            if (result == 1) {
                string hit_result = "{\n"
                                    "    \"result\": " + to_string(HIT) + "\n"
                                                                          "}";
                ofstream outputFile;
                outputFile.open("player_2.result.json");
                outputFile << hit_result;
                outputFile.close();
                remove("player_2.shot.json");
                return SHOT_FILE_PROCESSED;
            }

                //Returns result file for miss
            else if (result == -1) {
                string hit_result = "{\n"
                                    "    \"result\": " + to_string(MISS) + "\n"
                                                                           "}";
                ofstream outputFile;
                outputFile.open("player_2.result.json");
                outputFile << hit_result;
                outputFile.close();
                remove("player_2.shot.json");
                return SHOT_FILE_PROCESSED;
            }
                //Returns result file for oob
            else if (result == 0) {
                string hit_result = "{\n"
                                    "    \"result\": " + to_string(OUT_OF_BOUNDS) + "\n"
                                                                                    "}";
                ofstream outputFile;
                outputFile.open("player_2.result.json");
                outputFile << hit_result;
                outputFile.close();

                remove("player_2.shot.json");
                return SHOT_FILE_PROCESSED;
            }
        } else {
            int intx = px - 48; // switch format to int from ACSII
            int inty = py - 48;
            int result = evaluate_shot(1, intx, inty); //Calls evaluate shot
            if (result == 1) {
                string hit_result = "{\n"
                                    "    \"result\": " + to_string(HIT) + "\n"
                                                                          "}";
                ofstream outputFile;
                outputFile.open("player_2.result.json");
                outputFile << hit_result;
                outputFile.close();
                remove("player_2.shot.json");
                return SHOT_FILE_PROCESSED;
            }

                //Returns result file for miss
            else if (result == -1) {
                string hit_result = "{\n"
                                    "    \"result\": " + to_string(MISS) + "\n"
                                                                           "}";
                ofstream outputFile;
                outputFile.open("player_2.result.json");
                outputFile << hit_result;
                outputFile.close();
                remove("player_2.shot.json");
                return SHOT_FILE_PROCESSED;
            }
                //Returns result file for oob
            else if (result == 0) {
                string hit_result = "{\n"
                                    "    \"result\": " + to_string(OUT_OF_BOUNDS) + "\n"
                                                                                    "}";
                ofstream outputFile;
                outputFile.open("player_2.result.json");
                outputFile << hit_result;
                outputFile.close();

                remove("player_2.shot.json");
                return SHOT_FILE_PROCESSED;
            }
        }
    }
}
// chess_game.cpp
// PHYS 30762 OOP in C++
// Thomas Ronayne 17/05/2021
// Final Project - Board game - Chess

#include "board.h"
#include "pieces.h"
#include <iostream>
#include <limits>
#include <sstream>
#include <iterator>
#include <string>
#include <algorithm> 
#include <vector>

using namespace chess;

int main()
{
  board board1;
  board1.set_up();

  std::cout
  <<"  ____ _                     _          ____\n"
  <<" / ___| |__   ___  ___ ___  (_)_ __    / ___| _     _\n"   
  <<"| |   | '_ \\ / _ \\/ __/ __| | | '_ \\  | |   _| |_ _| |_\n"
  <<"| |___| | | |  __/\\__ \\__ \\ | | | | | | |__|_   _|_   _|\n"
  <<" \\____|_| |_|\\___||___/___/ |_|_| |_|  \\____||_|   |_|\n\n"
  <<"Welcome to my C++ Final Project - Chess!\n\n"
  <<"The commands you need to get started are listed below the board.\n"
  <<"Where I have indicated '##', you should write the location of a\n"
  <<"square using standard algebraic chess notation, e.g. a4 or h8.\n"
  <<"The pieces are indicated by their team (w or b) followed by the\n"
  <<"first letter of their name (with the exception of N for knight)\n";


  std::string user_input;
  std::string command{};
  bool whose_turn{1}; // 1 for white, 0 for black. White starts

  // each loop corresponds to one turn
  while(command != "end") {
    board1.show();
    bool end_turn{0};
    whose_turn ? std::cout<<"White's turn\n": std::cout<<"Black's turn\n";
    std::cout<<"Commands:    move ## ##    getmoves ##    getallmoves    pass    end\n";
    
    // each loop gets and interprets user input
    while(!end_turn) {
      std::getline(std::cin, user_input); // get user input
      std::istringstream iss(user_input);
      // split user input by spaces and put into a vector
      std::vector<std::string> split_by_space(std::istream_iterator<std::string>{iss},
                                              std::istream_iterator<std::string>());
      
      // if user input is not empty, the command is the first word
      if(split_by_space.size() != 0) {
        command = split_by_space[0];
      }

      if(command == "move") { // moves a piece from one square to another
        std::pair<size_t,size_t> initial_pos;
        std::pair<size_t,size_t> final_pos;
        bool insufficient_coordinates{};
        try {
          initial_pos = board1.AN_to_coord(split_by_space[1]);
          final_pos = board1.AN_to_coord(split_by_space[2]);
          insufficient_coordinates = 0;
        } catch(std::length_error) {
          insufficient_coordinates = 1;
        }

        if(insufficient_coordinates) { // less than two coords entered
          std::cout<<"Error: must enter two coordinates\n";
        } else if(initial_pos.first == -1 || final_pos.first == -1) { // invalid coords
          std::cout<<"Error: invalid coordinates \""
          <<split_by_space[1]<<" "<<split_by_space[2]<<"\"\n";
        } else if(board1.get_piece(initial_pos) == nullptr) { // empty square
          std::cout<<"Invalid move: no piece at "<<board1.coord_to_AN(initial_pos)<<"\n";
        } else {
          std::vector<std::pair<size_t,size_t>> allowed_moves = board1.get_piece(initial_pos)->moves(board1, initial_pos);
        
          if(board1.get_piece(initial_pos)->get_team() != whose_turn) { // wrong team
            if(whose_turn == 1) { // if white's turn
              std::cout<<"Invalid move: white cannot move black pieces!\n";
            } else { // if black's turn
              std::cout<<"Invalid move: black cannot move white pieces!\n";
            }
          } else if(std::find(allowed_moves.begin(), allowed_moves.end(), final_pos)
                    != allowed_moves.end()) { // is the move in allowed_moves
            board1.move_piece(initial_pos, final_pos);
            end_turn = 1;
          } else { // invalid move for that specific piece
            std::cout<<"Invalid move: "
            <<board1.get_piece(initial_pos)->get_name()<<" cannot move from "
            <<board1.coord_to_AN(initial_pos)<<" to "
            <<board1.coord_to_AN(final_pos)<<"\n";
          }
        }

      } else if(command == "getmoves") { // gets all possible moves from a given square
        std::pair<size_t,size_t> pos;
        bool insufficient_coordinates{};
        try {
          pos = board1.AN_to_coord(split_by_space[1]);
          insufficient_coordinates = 0;
        } catch(std::length_error) {
          insufficient_coordinates = 1;
        }

        if(insufficient_coordinates) { // less than two coords entered
          std::cout<<"Error: must enter a coordinate\n";
        } else if(pos.first == -1) { // invalid coords
          std::cout<<"Error: invalid coordinates \""
          <<split_by_space[1]<<"\"\n";
        } else if(board1.get_piece(pos) == nullptr) { // empty square
          std::cout<<"Invalid move: no piece at "<<board1.coord_to_AN(pos)<<"\n";
        } else {
          board1.get_piece(pos)->moves(board1, pos);
          board1.get_piece(pos)->show_moves(board1);
        }

      } else if(command == "getallmoves") { // gets all possible moves for the current player
        board1.show_all_moves(whose_turn);

      } else if(command == "pass" || command == "end") { // "pass" ends the turn, "end" ends the game
        end_turn = 1;

      } else {
        std::cout<<"Unknown command: \""<<command<<"\"\n";
      }
    }

    // the game ends when either king dies
    if(!king::is_black_king_alive()) {
      board1.show();
      std::cout<<"White team wins!\n";
      break;
    } else if(!king::is_white_king_alive()) {
      board1.show();
      std::cout<<"Black team wins!\n";
      break;
    }
    // switch whose turn it is
    whose_turn = !whose_turn;
  }

  return 0;
}
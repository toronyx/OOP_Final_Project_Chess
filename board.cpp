// board.cpp
// PHYS 30762 OOP in C++
// Thomas Ronayne 17/05/2021

#include "board.h"
#include "pieces.h"
#include <iostream>
#include <memory>

namespace chess {

board::board()
{
  size_t array_size = length*width;
  pieces = new std::array<std::unique_ptr<piece>, 64> {};
}

board::~board()
{
  delete pieces;
}

void board::add_piece(std::unique_ptr<piece> p, std::pair<size_t,size_t> pos)
{
  (*pieces)[pos.first + width*pos.second] = std::move(p);
}

void board::remove_piece(std::pair<size_t,size_t> pos)
{
  (*pieces)[pos.first + width*pos.second].reset(nullptr);
}

bool board::is_valid_square(std::pair<size_t,size_t> pos) const
{
  return ((pos.first>=0)&&(pos.first<width)&&(pos.second>=0)&&(pos.second<length));
}

bool board::is_empty(std::pair<size_t,size_t> pos) const
{
  return (*pieces)[pos.first + width*pos.second] == nullptr;
}

bool board::contains_enemy(std::pair<size_t,size_t> pos, bool team) const
{
  if(this->is_empty(pos)) {
    return 0; // if square is empty, it doesn't contain an enemy
  } else { // otherwise, check the team of piece at this position
    return ((this->get_piece(pos))->get_team() != team);
  }
}

std::unique_ptr<piece>& board::get_piece(std::pair<size_t,size_t> pos) const
{
  return (*pieces)[pos.first + width*pos.second];
}

void board::move_piece(std::pair<size_t,size_t> initial_pos,
                       std::pair<size_t,size_t> final_pos)
{
  if(!(this->is_empty(final_pos))) {
    this->remove_piece(final_pos);
  }
  (*pieces)[final_pos.first + width*final_pos.second] = std::move((*pieces)[initial_pos.first + width*initial_pos.second]);
}

void board::show_all_moves(bool team)
{
  for(size_t i{}; i<length*width; i++) {
    std::pair<size_t,size_t> pos = std::make_pair(i%width, i/width);

    if((*pieces)[i] != nullptr) { 
      if((*pieces)[i]->get_team() == team) {
        (*pieces)[i]->moves((*this), pos); // calculate moves
        
        if((*pieces)[i]->number_of_possible_moves() != 0) {
          std::cout<<coord_to_AN(pos)<<" to ";
          (*pieces)[i]->show_moves((*this)); // print moves
        }
      }
    }
  }
}

std::string board::coord_to_AN(std::pair<size_t,size_t> pos)
{
  char letter{"abcdefgh"[pos.first]};
  std::string number{std::to_string(8-pos.second)};

  return letter + number;
}

std::pair<size_t,size_t> board::AN_to_coord(std::string str)
{
  std::string letters = "abcdefgh";
  size_t x_pos = letters.find(str[0]);
  size_t y_pos = '8' - str[1];

  if(x_pos>7 || y_pos>7) {
    return std::make_pair(-1, -1); // return for if square not on the board
  } else {
    return std::make_pair(x_pos, y_pos);
  } 
}

void board::set_up()
{
  for(size_t i{}; i<8; i++) {
    add_piece(std::make_unique<pawn>(0), std::make_pair(i, 1)); // black
    add_piece(std::make_unique<pawn>(1), std::make_pair(i, 6)); // white

    if(i==0 or i==7) {
      add_piece(std::make_unique<rook>(0), std::make_pair(i, 0));
      add_piece(std::make_unique<rook>(1), std::make_pair(i, 7));
    } else if(i==1 or i==6) {
      add_piece(std::make_unique<knight>(0), std::make_pair(i, 0));
      add_piece(std::make_unique<knight>(1), std::make_pair(i, 7));
    } else if(i==2 or i==5) {
      add_piece(std::make_unique<bishop>(0), std::make_pair(i, 0));
      add_piece(std::make_unique<bishop>(1), std::make_pair(i, 7));
    }
  }
  add_piece(std::make_unique<king>(0), std::make_pair(3, 0));
  add_piece(std::make_unique<king>(1), std::make_pair(4, 7));
  add_piece(std::make_unique<queen>(0), std::make_pair(4, 0));
  add_piece(std::make_unique<queen>(1), std::make_pair(3, 7));
}

void board::show() const
{
  std::cout<<"----------------------------\n"<<"   "
  <<" a  b  c  d  e  f  g  h \n";
  for(size_t i{}; i<length*width; i++) {

    if((i%width == 0)&&(i!=length*width)) {
      std::cout<<length-i/length<<"   ";
    }

    if ((*pieces)[i] == nullptr) {
      std::cout<<"  "<<",";
    } else {
      std::cout<<(*(*pieces)[i])<<",";
    }

    if(i%width == width-1) {
      std::cout<<std::endl;
    }
  }
  std::cout<<std::endl;
}

} // namespace

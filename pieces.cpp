// pieces.cpp
// PHYS 30762 OOP in C++
// Thomas Ronayne 17/05/2021

#include "pieces.h"
#include "board.h"
#include <vector>
#include <iostream>
#include <string>

namespace chess {

piece::piece(){}

piece::~piece(){}

std::string piece::get_name() const
{
  return name;
}

bool piece::get_team() const
{
  return team;
}

void piece::show_moves(const board &b) const
{
  std::cout<<"[ ";
  for(auto iterator=allowed_moves.begin(); iterator!=allowed_moves.end(); ++iterator) {
    std::cout<<b.coord_to_AN(*iterator)<<" ";
  }
  std::cout<<"]\n";
}

size_t piece::number_of_possible_moves() const
{
  return allowed_moves.size();
}

std::ostream& operator<<(std::ostream &os, const piece &p)
{
  os<<"bw"[p.team]<<p.symbol;
  return os;
}

// Pawn

pawn::pawn()
{
  name = "Pawn"; 
  symbol = 'P';
}

pawn::pawn(bool team)
{
  this->team = team; 
  name = "Pawn"; 
  symbol = 'P';
}

pawn::~pawn(){}

std::vector<std::pair<size_t,size_t>> pawn::moves(const board &b, std::pair<size_t,size_t> pos)
{
  allowed_moves.clear();
  int movement_direction{1 - 2*team}; // -1 for white, 1 for black

  // define the squares the pawn has to check
  std::pair<size_t,size_t> one_step_forward = std::make_pair(pos.first, pos.second + movement_direction);
  std::pair<size_t,size_t> two_step_forward = std::make_pair(pos.first, pos.second + 2*movement_direction);
  std::pair<size_t,size_t> diagonally_right = std::make_pair(pos.first + 1, pos.second + movement_direction);
  std::pair<size_t,size_t> diagonally_left  = std::make_pair(pos.first - 1, pos.second + movement_direction);

  if(b.is_valid_square(one_step_forward)) {
    if(b.is_empty(one_step_forward)) {
      allowed_moves.push_back(one_step_forward);

      // pawn can move 2 steps forward if on starting pawn line
      if(b.is_valid_square(two_step_forward)) {
        if(b.is_empty(two_step_forward)
        &&((team==0 && pos.second==1) || (team==1 && pos.second==6))) {
          allowed_moves.push_back(two_step_forward);
        }
      }
    }
  }

  // pawn can take diagonally
  if(b.is_valid_square(diagonally_right)) {
    if(b.contains_enemy(diagonally_right, team)) {
      allowed_moves.push_back(diagonally_right);
    }
  }
  if(b.is_valid_square(diagonally_left)) {
    if(b.contains_enemy(diagonally_left, team)) {
      allowed_moves.push_back(diagonally_left);
    }
  }

  return allowed_moves;
}

// Knight

knight::knight()
{
  name = "Knight"; 
  symbol = 'N';
}

knight::knight(bool team)
{
  this->team = team; 
  name = "Knight"; 
  symbol = 'N';
}

knight::~knight(){}

std::vector<std::pair<size_t,size_t>> knight::moves(const board &b, std::pair<size_t,size_t> pos)
{
  allowed_moves.clear();
  // these arrays define the squares a knight can move to relative to its position
  int change_in_x[8] = {1, 2, 2, 1,-1,-2,-2,-1};
  int change_in_y[8] = {2, 1,-1,-2,-2,-1, 1, 2};

  for(size_t i{}; i<8; i++) {
    std::pair<size_t,size_t> new_pos = std::make_pair(pos.first + change_in_x[i], 
                                                      pos.second + change_in_y[i]);
    if(b.is_valid_square(new_pos)) {
      if(b.is_empty(new_pos)) {
        allowed_moves.push_back(new_pos);
      } else if(b.contains_enemy(new_pos, team)) {
        allowed_moves.push_back(new_pos);
      }
    }
  }

  return allowed_moves;
}

// Bishop

bishop::bishop()
{
  name = "Bishop"; 
  symbol = 'B';
}

bishop::bishop(bool team)
{
  this->team = team; 
  name = "Bishop"; 
  symbol = 'B';
}

bishop::~bishop(){}

std::vector<std::pair<size_t,size_t>> bishop::moves(const board &b, std::pair<size_t,size_t> pos)
{
  allowed_moves.clear();
  // these arrays define the directions the bishop can move in
  int x_direction[4] = { 1, 1,-1,-1};
  int y_direction[4] = { 1,-1,-1, 1};

  for(size_t i{}; i<4; i++) { // cycles through the four directions
  
    /* extend the diagonal outwards from the bishop, breaking after meeting an enemy,
    when meeting a friendly, or when leaving the board */
    for(size_t j{1}; j<8; j++) { 
      std::pair<size_t,size_t> new_pos = std::make_pair(pos.first + j*x_direction[i], 
                                                        pos.second + j*y_direction[i]);
      if(b.is_valid_square(new_pos)) {
        if(b.is_empty(new_pos)) {
          allowed_moves.push_back(new_pos);
        } else if(b.contains_enemy(new_pos, team)) {
          allowed_moves.push_back(new_pos);
          break;
        } else {
          break;
        }
      } else {
        break;
      }
    }
  }

  return allowed_moves;
}

// Rook

rook::rook()
{
  name = "Rook"; 
  symbol = 'R';
}

rook::rook(bool team)
{
  this->team = team; 
  name = "Rook"; 
  symbol = 'R';
}

rook::~rook(){}

std::vector<std::pair<size_t,size_t>> rook::moves(const board &b, std::pair<size_t,size_t> pos)
{
  allowed_moves.clear();
  // these arrays define the directions the rook can move in
  int x_direction[4] = {0, 1, 0,-1};
  int y_direction[4] = {1, 0,-1, 0};

  for(size_t i{}; i<4; i++) { // cycles through the four directions, very similar to bishop
  
    /* extend the line outwards from the rook, breaking after meeting an enemy,
    when meeting a friendly, or when leaving the board */
    for(size_t j{1}; j<8; j++) { 
      std::pair<size_t,size_t> new_pos = std::make_pair(pos.first + j*x_direction[i], 
                                                        pos.second + j*y_direction[i]);
      if(b.is_valid_square(new_pos)) {
        if(b.is_empty(new_pos)) {
          allowed_moves.push_back(new_pos);
        } else if(b.contains_enemy(new_pos, team)) {
          allowed_moves.push_back(new_pos);
          break;
        } else {
          break;
        }
      } else {
        break;
      }
    }
  }

  return allowed_moves;
}

// Queen

queen::queen()
{
  name = "Queen"; 
  symbol = 'Q';
}

queen::queen(bool team)
{
  this->team = team; 
  name = "Queen"; 
  symbol = 'Q';
}

queen::~queen(){}

std::vector<std::pair<size_t,size_t>> queen::moves(const board &b, std::pair<size_t,size_t> pos)
{
  allowed_moves.clear();
  // these arrays define the directions the queen can move in
  int x_direction[8] = {0, 1, 1, 1, 0,-1,-1,-1};
  int y_direction[8] = {1, 1, 0,-1,-1,-1, 0, 1};

  for(size_t i{}; i<8; i++) { // cycles through 8 directions, similar to rook and bishop
  
    /* extend the line outwards from the queen, breaking after meeting an enemy,
    when meeting a friendly, or when leaving the board */
    for(size_t j{1}; j<8; j++) { 
      std::pair<size_t,size_t> new_pos = std::make_pair(pos.first + j*x_direction[i], 
                                                        pos.second + j*y_direction[i]);
      if(b.is_valid_square(new_pos)) {
        if(b.is_empty(new_pos)) {
          allowed_moves.push_back(new_pos);
        } else if(b.contains_enemy(new_pos, team)) {
          allowed_moves.push_back(new_pos);
          break;
        } else {
          break;
        }
      } else {
        break;
      }
    }
  }

  return allowed_moves;
}

// King

bool king::black_king_alive{0};
bool king::white_king_alive{0};

king::king()
{
  name = "King"; 
  symbol = 'K';
  black_king_alive = 1;
}

king::king(bool team)
{
  this->team = team; 
  name = "King"; 
  symbol = 'K';
  team ? white_king_alive = 1: black_king_alive = 1;
}

king::~king()
{
  // assuming that there will only be 1 king per team
  team ? white_king_alive = 0: black_king_alive = 0;
}

std::vector<std::pair<size_t,size_t>> king::moves(const board &b, std::pair<size_t,size_t> pos)
{
  allowed_moves.clear();
  // these arrays define the squares a King can move to relative to its position
  int change_in_x[8] = {0, 1, 1, 1, 0,-1,-1,-1};
  int change_in_y[8] = {1, 1, 0,-1,-1,-1, 0, 1};

  for(size_t i{}; i<8; i++) {
    std::pair<size_t,size_t> new_pos = std::make_pair(pos.first + change_in_x[i], 
                                                      pos.second + change_in_y[i]);
    if(b.is_valid_square(new_pos)) {
      if(b.is_empty(new_pos)) {
        allowed_moves.push_back(new_pos);
      } else if(b.contains_enemy(new_pos, team)) {
        allowed_moves.push_back(new_pos);
      }
    }
    
  }
  return allowed_moves;
}

bool king::is_black_king_alive()
{
  return black_king_alive;
}

bool king::is_white_king_alive()
{
  return white_king_alive;
}

} // namespace
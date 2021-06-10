// pieces.h
// PHYS 30762 OOP in C++
// Thomas Ronayne 17/05/2021

#include <iostream>
#include <vector>
#include <string>

namespace chess {

class board;

#ifndef PIECES
#define PIECES

class piece
{
  friend std::ostream& operator<<(std::ostream &os, const piece &p);
protected:
  std::string name{};
  char symbol{};
  bool team{0}; // 1 is white, 0 is black (default is black)
  std::vector<std::pair<size_t,size_t>> allowed_moves;

public:
  piece();
  virtual ~piece();
  virtual std::vector<std::pair<size_t,size_t>> moves(const board &b, std::pair<size_t,size_t> pos) = 0;
  std::string get_name() const;
  bool get_team() const;
  void show_moves(const board &b) const;
  size_t number_of_possible_moves() const;
};


class pawn: public piece
{
public:
  pawn();
  pawn(bool team);
  ~pawn();
  std::vector<std::pair<size_t,size_t>> moves(const board &b, std::pair<size_t,size_t> pos);
};

class knight: public piece
{
public:
  knight();
  knight(bool team);
  ~knight();
  std::vector<std::pair<size_t,size_t>> moves(const board &b, std::pair<size_t,size_t> pos);
};

class bishop: public piece
{
public:
  bishop();
  bishop(bool team);
  ~bishop();
  std::vector<std::pair<size_t,size_t>> moves(const board &b, std::pair<size_t,size_t> pos);
};

class rook: public piece
{
public:
  rook();
  rook(bool team);
  ~rook();
  std::vector<std::pair<size_t,size_t>> moves(const board &b, std::pair<size_t,size_t> pos);
};

class queen: public piece
{
public:
  queen();
  queen(bool team);
  ~queen();
  std::vector<std::pair<size_t,size_t>> moves(const board &b, std::pair<size_t,size_t> pos);
};

class king: public piece
{
private:
  static bool black_king_alive;
  static bool white_king_alive;
public:
  king();
  king(bool team);
  ~king();
  std::vector<std::pair<size_t,size_t>> moves(const board &b, std::pair<size_t,size_t> pos);
  static bool is_black_king_alive();
  static bool is_white_king_alive();
};

#endif

} // namespace
// board.h
// PHYS 30762 OOP in C++
// Thomas Ronayne 17/05/2021

#include <cstddef>
#include <string>
#include <memory>

namespace chess {

class piece;

#ifndef BOARD
#define BOARD

class board
{
private:
  size_t length{8};
  size_t width{8};
  std::array<std::unique_ptr<piece>, 64> *pieces{nullptr};

public:
  board();
  ~board();
  void add_piece(std::unique_ptr<piece> p, std::pair<size_t,size_t> pos);
  void remove_piece(std::pair<size_t,size_t> pos);
  bool is_valid_square(std::pair<size_t,size_t> pos) const;
  bool is_empty(std::pair<size_t,size_t> pos) const;
  bool contains_enemy(std::pair<size_t,size_t> pos, bool team) const;
  std::unique_ptr<piece>& get_piece(std::pair<size_t,size_t> pos) const;
  void move_piece(std::pair<size_t,size_t> initial_pos,
                  std::pair<size_t,size_t> final_pos);
  void show_all_moves(bool team);
  static std::string coord_to_AN(std::pair<size_t,size_t> pos);
  static std::pair<size_t,size_t> AN_to_coord(std::string str);
  void set_up();
  void show() const;
};

#endif

} // namespace
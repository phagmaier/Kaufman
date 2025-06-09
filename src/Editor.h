#pragma once
#include "Gstring.h"
#include <csignal>
#include <memory>
#include <ncurses.h>
#include <vector>

using buff_vec = std::vector<std::unique_ptr<Gstring>>;

struct Editor {
  Editor(const std::string &fileName);
  Editor(const char *fileName);
  ~Editor();
  std::string fileName;
  ui row;
  ui col;
  ui firstVisibleRow;
  ui maxRows;
  ui maxCols;
  buff_vec buffer;
  char *disp_str;
  void del_line(const ui index);
  void add_char_overflow(const char c);
  void up_key();
  void down_key();
  void left_key();
  void right_key();
  bool pop_prev_add_cur_str(const ui index);
  bool pop_prev_add_cur(const ui index);
  void del_zero();
  void del_middle_wrap();
  void key_backspace();
  void key_enter();
  void key_char(const char c);
  void draw_buffer();
  void resize(const ui new_row, const ui new_col);
  void handleInput(int ch);
  void main_loop();
  void load_file();
  void save_file();
};

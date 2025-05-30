#pragma once
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <iostream>

using ui = unsigned int;

struct Gstring {
  const static ui DEFSIZE = 50;
  static ui MAXLEN;
  /*CONSTRUCTOR*/
  Gstring();
  Gstring(const char *str, const ui str_size);
  /*CONSTRUCTOR*/

  /*VARS*/
  ui cap;
  ui size;
  ui gsize;
  ui gstart;
  ui gend;
  bool newLine;
  char *string;
  /*VARS*/

  /*FUNCS*/
  void reset();
  void grow();
  void move(const ui index);
  void add_char(const char c, const ui index);
  void append_str(const char *str, const ui cpy_size);
  void del_char(const ui index);
  char insert_pop(const char c, const ui index);
  char pop_head();
  char *pop_str(const ui cpy_size);
  void replace(const char c, const ui index);
  void free_str();
  inline char char_at(const ui index) const;
  char *enter(const ui index);
  /*FUNCS*/
};

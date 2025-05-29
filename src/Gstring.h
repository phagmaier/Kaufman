#pragma once
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <iostream>

using ui = unsigned int;

struct Gstring {
  const static ui DEFSIZE = 50;
  /*CONSTRUCTOR*/
  Gstring();
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
  void del_char(const ui index);
  char insert_pop(const char c, const ui index);
  char char_at(const ui index);
  /*FUNCS*/
};

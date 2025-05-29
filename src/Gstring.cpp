#include "Gstring.h"
#include <cstring>

Gstring::Gstring()
    : cap{DEFSIZE}, size{0}, gsize{DEFSIZE}, gstart{0}, gend{DEFSIZE - 1},
      newLine{true} {
  string = (char *)malloc(DEFSIZE);
}

void Gstring::reset() {
  size = 0;
  gsize = cap;
  gstart = 0;
  gend = cap - 1;
}

void Gstring::move(const ui index) {
  if (index == gstart || !gsize || !size) {
    return;
  }
  if (index < gstart) {
    const size_t shift_size = gstart - index;
    memmove((string + index + gsize), (string + index), shift_size);
  } else {
    const size_t shift_size = index - gstart;
    memmove((string + gstart), (string + gend + 1), shift_size);
  }
  gstart = index;
  gend = index + gsize - 1;
}

void Gstring::grow() {
  // THINK I ONLY CALL WHEN EMPTY SO DON'T NEED TO MOVE
  // move(size);
  const ui new_cap = cap * 2;
  string = (char *)realloc(string, new_cap);
  gend = new_cap - 1;
  gsize += cap;
  cap = new_cap;
}

// assumes buffer checks to see if we need a newline
// or not
void Gstring::add_char(const char c, const ui index) {
  if (!gsize) {
    grow();
  } else {
    move(index);
  }
  string[index] = c;
  ++gstart;
  --gsize;
  ++size;
}

// assumes you check to make sure it's not a 0
void Gstring::del_char(const ui index) {
  assert(size);
  move(index);
  ++gend;
  ++gsize;
  --size;
}

char Gstring::insert_pop(const char c, const ui index) {
  if (index == size) {
    return c;
  }
  // probably don't need the move
  // but don't ensure that i limit size at the moment
  // to max line length so whatever
  // need a static variable that you will
  // set later that determines the max growth
  move(size);
  const char tmp = string[size - 1];
  del_char(size - 1);
  add_char(c, index);
  return tmp;
}

char Gstring::char_at(const ui index) {
  return index < gstart ? string[index] : string[index + gsize];
}

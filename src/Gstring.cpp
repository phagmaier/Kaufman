#include "Gstring.h"
#include <cstring>
ui Gstring::MAXLEN = 64;
Gstring::Gstring()
    : cap{DEFSIZE}, size{0}, gsize{DEFSIZE}, gstart{0}, gend{DEFSIZE - 1},
      newLine{true} {
  string = (char *)malloc(DEFSIZE);
}

Gstring::Gstring(const char *str, const ui str_size) {
  cap = str_size >= DEFSIZE ? str_size + 5 : DEFSIZE;
  size = str_size;
  gsize = cap - str_size;
  gstart = str_size;
  gend = cap - 1;
  newLine = true;
  string = (char *)malloc(cap);
  memcpy(string, str, str_size);
}

void Gstring::reset() {
  size = 0;
  gsize = cap;
  gstart = 0;
  gend = cap - 1;
}

void Gstring::move(const ui index) {
  assert(index <= size);
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
  gend = cap;
  gstart = cap;
  const ui new_cap = std::min(cap * 2, Gstring::MAXLEN);
  const ui extra = new_cap - cap;
  string = (char *)realloc(string, new_cap);
  gsize += extra;
  gend = new_cap - 1;
  cap = new_cap;
}

void Gstring::add_char(const char c, const ui index) {
  if (!gsize) {
    grow();
  }
  move(index);
  assert(gsize);
  string[index] = c;
  ++gstart;
  --gsize;
  ++size;
}

// assumes you check to make sure it's not a 0
void Gstring::del_char(const ui index) {
  assert(size);
  if (gsize) {
    move(index);
    ++gend;
    ++gsize;
    --size;
  } else {
    gstart = index;
    gend = index;
    gsize = 1;
    --size;
  }
}

char Gstring::insert_pop(const char c, const ui index) {
  if (index == size) {
    return c;
  }
  move(size);
  const char tmp = string[size - 1];
  if (index == size - 1) {
    string[index] = c;
    return tmp;
  }
  memmove(string + index + 1, string + index, size - index - 1);
  string[index] = c;
  return tmp;
}

char Gstring::pop_head() {
  const char tmp = char_at(0);
  del_char(0);
  return tmp;
}

char Gstring::char_at(const ui index) const {
  return index < gstart ? string[index] : string[index + gsize];
}

char *Gstring::enter(const ui index) {
  newLine = true;
  move(index);
  char *str = string + index + gsize;
  gend = cap - 1;
  size = index;
  gsize = cap - size;
  return str;
}

void Gstring::replace(const char c, const ui index) {
  const ui real_idx = (index < gstart) ? index : index + gsize;
  string[real_idx] = c;
}

char *Gstring::pop_str(const ui cpy_size) {
  assert(size);
  char *str;
  if (gsize) {
    move(0);
    str = string + gsize;
    gend += cpy_size;
    gsize += cpy_size;
    size -= cpy_size;
  } else {
    str = string;
    gstart = 0;
    gend = cpy_size;
    size -= cpy_size;
    gsize = cap - size;
  }
  return str;
}

void Gstring::append_str(const char *str, const ui cpy_size) {
  move(size);
  memcpy(string + gstart, str, cpy_size);
  gstart += cpy_size;
  gsize -= cpy_size;
  size += cpy_size;
}

void Gstring::load_apped(std::string &str) {
  move(size);
  const ui cpy_size = str.size() <= gsize ? gsize : gsize;
  memcpy(string + gstart, str.c_str(), cpy_size);
  gstart += cpy_size;
  gsize -= cpy_size;
  size += cpy_size;
  str.erase(0, cpy_size);
}

Gstring::~Gstring() { free(string); }

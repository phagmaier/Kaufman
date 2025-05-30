#include "Editor.h"

Editor::Editor(const std::string &fileName)
    : fileName{fileName}, row{0}, col{0}, firstVisibleRow{0} {
  initscr();
  raw();
  keypad(stdscr, TRUE);
  noecho();
  curs_set(1);
  getmaxyx(stdscr, maxRows, maxCols);
  Gstring::MAXLEN = maxCols;
  buffer.reserve(100);
  buffer.emplace_back(std::make_unique<Gstring>());
  disp_str = (char *)malloc(1000);
  /* load/save*/
  // CODE HERE
  /* load/save*/
}

Editor::Editor(const char *fileName)
    : fileName{fileName}, row{0}, col{0}, firstVisibleRow{0} {
  initscr();
  raw();
  keypad(stdscr, TRUE);
  noecho();
  curs_set(1);
  getmaxyx(stdscr, maxRows, maxCols);
  Gstring::MAXLEN = maxCols;
  buffer.reserve(100);
  buffer.emplace_back(std::make_unique<Gstring>());
  disp_str = (char *)malloc(1000);
  /* load/save*/
  // CODE HERE
  /* load/save*/
}

void Editor::resize() { std::cerr << "ERROR UNABLE TO RESIZE YET"; }
void Editor::load_file() { std::cerr << "LOAD FILE: NOT IMPLIMENTED YET"; }
void Editor::save_file() { std::cerr << "SAVE FILE: NOT IMPLIMENTED YET"; }

void Editor::del_line(const ui index) {
  buffer[index]->free_str();
  buffer.erase(buffer.begin() + index);
}

void Editor::add_char_overflow(const char c) {
  Gstring *cur = buffer[row].get();
  char tmp = cur->insert_pop(c, col);
  if (cur->newLine) {
    cur->newLine = false;
    buffer.insert(buffer.begin() + row + 1, std::make_unique<Gstring>());
    buffer[row + 1]->add_char(tmp, 0);
    return;
  }

  ui tmp_row = row + 1;
  const ui buff_size = buffer.size();
  while (tmp_row < buff_size) {
    cur = buffer[tmp_row].get();
    if (cur->newLine) {
      if (cur->size == maxCols) {
        cur->newLine = false;
        tmp = cur->insert_pop(tmp, 0);
        buffer.insert(buffer.begin() + tmp_row, std::make_unique<Gstring>());
        buffer[tmp_row]->add_char(tmp, 0);
        return;
      } else {
        cur->add_char(tmp, 0);
        return;
      }
    } else {
      // SO I THINK IF IT'S NOT A NEWLINE IT LITERALLY HAS TO BE A OVERFLOW
      // SO WILL NOT CHECK FOR CASE WHERE NOT OVERFLOW
      tmp = cur->insert_pop(tmp, 0);
    }
    ++tmp_row;
  }
  cur = buffer[tmp_row - 1].get();
  if (cur->size == maxCols) {
    cur->newLine = false;
    tmp = cur->insert_pop(tmp, 0);
    buffer.insert(buffer.begin() + tmp_row, std::make_unique<Gstring>());
    buffer[tmp_row]->add_char(tmp, 0);
    return;
  } else {
    cur->add_char(tmp, 0);
    return;
  }
}

void Editor::up_key() {
  row -= (row > 0);
  col = buffer[row]->size < col ? buffer[row]->size : col;
}
void Editor::down_key() {
  row += (row + 1 < buffer.size());
  col = buffer[row]->size < col ? buffer[row]->size : col;
}

bool Editor::pop_prev_add_cur_str(const ui index) {
  if (index >= buffer.size()) {
    return false;
  }
  Gstring *cur = buffer[index].get();
  Gstring *prev = buffer[index - 1].get();
  const ui cpy_size = std::min(maxCols - prev->size, cur->size);
  if (cpy_size) {
    char *tmp = cur->pop_str(cpy_size);
    prev->append_str(tmp, cpy_size);
    if (!cur->size) {
      del_line(index);
      prev->newLine = true;
      return false;
    }
    if (cur->newLine) {
      return false;
    }
    return true;
  }
  prev->newLine = false;

  return false;
}

bool Editor::pop_prev_add_cur(const ui index) {
  if (index >= buffer.size()) {
    return false;
  }
  Gstring *cur = buffer[index].get();
  Gstring *prev = buffer[index - 1].get();
  // Don't Think i need this think it's guranteed to have size
  if (prev->size) {
    prev->replace(cur->char_at(0), prev->size - 1);
  } else {
    std::cerr << "POP PREV ADD CUR: DON'T THINK THIS SHOULD EVER RUN IF IT "
                 "DOES INSPECT LOGIC";
    prev->add_char(cur->char_at(0), 0);
  }
  cur->del_char(0);
  if (!cur->size) {
    del_line(index);
    prev->newLine = true;
    return false;
  }
  if (cur->newLine) {
    return false;
  }
  return true;
}

void Editor::del_zero() {
  ui cur_idx = row;
  if (buffer[row - 1]->newLine) {
    buffer[row - 1]->newLine = false;
    while (pop_prev_add_cur_str(cur_idx)) {
      ++cur_idx;
    }
  } else {
    while (pop_prev_add_cur(cur_idx)) {
      ++cur_idx;
    }
  }
}

/*FOR THESE TWO WANT TO ADD SUPPORT TO GO UP
 * AND DOWN WHEN AT END*/
void Editor::left_key() { col -= (col > 0); }
void Editor::right_key() { col += (col < buffer[row]->size); }
/*FOR THESE TWO WANT TO ADD SUPPORT TO GO UP
 * AND DOWN WHEN AT END*/
void Editor::key_backspace() {
  if (col) {
    --col;
    buffer[row]->del_char(col);
    return;
  } else {
    if (!row) {
      return;
    }
    del_zero();
    --row;
    col = buffer[row]->size;
  }
}
void Editor::key_enter() {
  Gstring *cur = buffer[row].get();
  if (col < cur->size) {
    const ui cpy_size = cur->size - col;
    char *tmp = cur->enter(col);
    buffer.insert(buffer.begin() + row + 1,
                  std::make_unique<Gstring>(tmp, cpy_size));
  } else {
    buffer.insert(buffer.begin() + row + 1, std::make_unique<Gstring>());
  }
  ++row;
  col = 0;
}

void Editor::key_char(const char c) {
  if (buffer[row]->size + 1 < maxCols) {
    buffer[row]->add_char(c, col);
    ++col;
  } else {
    add_char_overflow(c);
    ++row;
    col = 1;
  }
}

/*
void Editor::draw_buffer() const {
  werase(stdscr); // clear whole screen (cheap if -o)

  ui screen_row = 0;
  const ui buf_size = buffer.size();

  for (ui i = firstVisibleRow; i < buf_size && screen_row < maxRows;
       ++i, ++screen_row) {
    const Gstring &s = *buffer[i];
    const ui len1 = s.gstart;
    const ui len2 = s.size - len1;
    const char *base = s.string;

    if (len1)
      mvwaddnstr(stdscr, screen_row, 0, base, len1);

    if (len2)
      mvwaddnstr(stdscr, screen_row, len1, base + s.gend + 1, len2);

    // DON'T THINK I NEED BELOW LINE BUT PUT BACK IF BROKEN
    //  wclrtoeol(stdscr); // erase leftovers if line shrank
    // DON'T THINK I NEED ABOVE LINE BUT PUT BACK IF BROKEN
  }
  wmove(stdscr, row - firstVisibleRow, col);
  wrefresh(stdscr);
}
*/

void Editor::draw_buffer() {
  wclear(stdscr);
  ui screen_row = 0;
  const ui buf_size = buffer.size();
  for (ui i = firstVisibleRow; i < buf_size && screen_row < maxRows;
       ++i, ++screen_row) {
    const Gstring *tmp = buffer[i].get();
    memcpy(disp_str, tmp->string, tmp->gstart);
    const ui r_size = tmp->size - tmp->gstart;
    memcpy(disp_str + tmp->gstart, tmp->string + tmp->gend + 1, r_size);
    mvwaddnstr(stdscr, screen_row, 0, disp_str, tmp->size);
  }
  wmove(stdscr, row - firstVisibleRow, col);
  wnoutrefresh(stdscr);
  doupdate();
}

void Editor::handleInput(int ch) {
  switch (ch) {
  case KEY_RESIZE:
    getmaxyx(stdscr, maxRows, maxCols);
    break;
  case KEY_UP:
    up_key();
    break;
  case KEY_DOWN:
    down_key();
    break;
  case KEY_LEFT:
    left_key();
    break;
  case KEY_RIGHT:
    right_key();
    break;
  case KEY_BACKSPACE:
    key_backspace();
    break;
  case '\n':
  case '\r':
    key_enter();
    break;
  default:
    if (ch >= 32 && ch <= 126) {
      key_char(static_cast<char>(ch));
    }
  }
  if (row < firstVisibleRow)
    firstVisibleRow = row;
  if (row >= firstVisibleRow + maxRows)
    firstVisibleRow = row - maxRows + 1;

  draw_buffer();
}

void Editor::main_loop() {
  /*DELETE BELOW WHEN DONE TESTING*/
  maxCols = 10;
  /*DELETE ABOVE WHEN DONE TESTING*/
  draw_buffer();
  while (true) {
    int ch = getch();
    if (ch == 17 /* Ctrl-Q EXITS */)
      break;

    if (ch == KEY_RESIZE) {
      resize();
    }
    handleInput(ch);
  }
  endwin();
}

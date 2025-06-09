#include "Editor.h"
#include <cstring>
#include <fstream>
#include <memory>

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
  disp_str = (char *)malloc(maxCols + 1);
  disp_str[maxCols] = 0;
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
  disp_str = (char *)malloc(maxCols + 1);
  disp_str[maxCols] = 0;
  /* load/save*/
  // CODE HERE
  /* load/save*/
}

void Editor::resize(const ui new_row, const ui new_col) {
  std::cerr << "RESIZE: NOT IMPLIEMNTED YET";
}
void Editor::load_file() {
  buffer.clear();
  buffer.reserve(100);
  std::ifstream inputFile("filename.txt");
  assert(inputFile.is_open());
  std::string line;
  line.reserve(100);
  while (getline(inputFile, line)) {
    while (line.size()) {
      buffer.emplace_back(std::make_unique<Gstring>());
      buffer[buffer.size() - 1]->load_apped(line);
    }
    buffer[buffer.size() - 1]->newLine = true;
  }
}

void Editor::save_file() {
  const ui buff_size = buffer.size();
  std::fstream outputFile;
  outputFile.open(fileName, std::ios::out);
  assert(outputFile.is_open());
  for (ui i = 0; i < buff_size; ++i) {
    const Gstring *tmp = buffer[i].get();
    memcpy(disp_str, tmp->string, tmp->gstart);
    const ui r_size = tmp->size - tmp->gstart;
    memcpy(disp_str + tmp->gstart, tmp->string + tmp->gend + 1, r_size);
    outputFile.write(disp_str, tmp->size);
    if (tmp->newLine) {
      outputFile << "\n";
    }
  }
}

void Editor::del_line(const ui index) { buffer.erase(buffer.begin() + index); }

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

bool Editor::pop_prev_add_cur(const ui idx) {
  Gstring *cur = buffer[idx].get();
  Gstring *prev = buffer[idx - 1].get();
  char c = cur->char_at(0);
  cur->del_char(0);
  prev->add_char(c, prev->size);
  if (!cur->size) {
    del_line(idx);
    prev->newLine = true;
    return false;
  }
  if (cur->newLine) {
    return false;
  }
  if (idx == buffer.size() - 1) {
    cur->newLine = true;
    return false;
  }
  return true;
}

bool Editor::pop_prev_add_cur_str(const ui idx) {
  if (idx == 0 || idx >= buffer.size())
    return false;

  Gstring *cur = buffer[idx].get();
  Gstring *prev = buffer[idx - 1].get();

  const ui slack = maxCols - prev->size;
  const ui cpy_size = std::min(slack, cur->size);

  if (cpy_size == 0) {
    return pop_prev_add_cur(idx);
  }

  char *segment = cur->pop_str(cpy_size);
  prev->append_str(segment, cpy_size);
  if (!cur->size) {
    del_line(idx);
    prev->newLine = true;
    return false;
  }
  return !cur->newLine;
}

void Editor::left_key() { col -= (col > 0); }
void Editor::right_key() { col += (col < buffer[row]->size); }

void Editor::del_middle_wrap() {
  ui tmp_row = row + 1;
  const ui buff_size = buffer.size();
  while (tmp_row < buff_size) {
    Gstring *cur = buffer[tmp_row - 1].get();
    Gstring *next = buffer[tmp_row].get();
    cur->add_char(next->pop_head(), cur->size);
    if (next->newLine) {
      if (!next->size) {
        del_line(tmp_row);
        cur->newLine = true;
      }
      return;
    }
    if (!next->size) {
      del_line(tmp_row);
      cur->newLine = true;
      return;
    }
    ++tmp_row;
  }
}

void Editor::del_zero() {
  ui cur_idx = row;
  if (!buffer[cur_idx]->size) {
    buffer[cur_idx - 1]->newLine = true;
    del_line(cur_idx);
    col = buffer[row - 1]->size;
    return;
  }
  if (buffer[row - 1]->newLine) {
    buffer[row - 1]->newLine = false;
    ui prev_line_size = buffer[row - 1]->size;
    while (pop_prev_add_cur_str(cur_idx)) {
      ++cur_idx;
      prev_line_size = buffer[cur_idx - 1]->size;
    }
    col = prev_line_size;
  } else {
    buffer[row - 1]->del_char(buffer[row - 1]->size - 1);
    while (pop_prev_add_cur(cur_idx)) {
      ++cur_idx;
    }
    col = buffer[row - 1]->size;
  }
}

void Editor::key_backspace() {
  if (col) {
    --col;
    buffer[row]->del_char(col);
    if (!buffer[row]->newLine) {
      del_middle_wrap();
    }
    return;
  }

  if (!row) {
    return;
  }
  del_zero();
  --row;
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
  if (buffer[row]->size + 1 <= maxCols) {
    buffer[row]->add_char(c, col);
    ++col;
  } else {
    add_char_overflow(c);
    ++row;
    col = 1;
  }
}

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

Editor::~Editor() { free(disp_str); }

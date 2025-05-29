#include "Gstring.h"
#include <csignal>
#include <memory>
#include <ncurses.h>
#include <vector>

static volatile sig_atomic_t resized = 0;
static void on_winch(int) { resized = 1; }
ui row = 0;
ui col = 0;

ui maxRows = 0;
ui maxCols = 0;
ui firstVisibleRow = 0;

using buff_vec = std::vector<std::unique_ptr<Gstring>>;
buff_vec buffer;

void add_char_overflow(const char c) {
  Gstring *curr = buffer[row].get();
  char tmp = curr->insert_pop(c, col);
  if (curr->newLine) {
    curr->newLine = false;
    buffer.insert(buffer.begin() + row + 1, std::make_unique<Gstring>());
    buffer[row + 1]->add_char(tmp, 0);
    return;
  }

  ui tmp_row = row + 1;
  const ui buff_size = buffer.size();
  while (tmp_row < buff_size) {
    curr = buffer[tmp_row].get();
    if (curr->newLine) {
      if (curr->size == maxCols) {
        curr->newLine = false;
        tmp = curr->insert_pop(tmp, 0);
        buffer.insert(buffer.begin() + tmp_row, std::make_unique<Gstring>());
        buffer[tmp_row]->add_char(tmp, 0);
        return;
      } else {
        curr->add_char(tmp, 0);
        return;
      }
    } else {
      // SO I THINK IF IT'S NOT A NEWLINE IT LITERALLY HAS TO BE A OVERFLOW
      // SO WILL NOT CHECK FOR CASE WHERE NOT OVERFLOW
      tmp = curr->insert_pop(tmp, 0);
    }
    ++tmp_row;
  }
  if (curr->size == maxCols) {
    curr->newLine = false;
    tmp = curr->insert_pop(tmp, 0);
    buffer.insert(buffer.begin() + tmp_row, std::make_unique<Gstring>());
    buffer[tmp_row]->add_char(tmp, 0);
    return;
  } else {
    curr->add_char(tmp, 0);
    return;
  }
}

void up_key() {
  row -= (row > 0);
  col = buffer[row]->size < col ? buffer[row]->size : col;
}
void down_key() {
  row += (row + 1 < buffer.size());
  col = buffer[row]->size < col ? buffer[row]->size : col;
}

void left_key() { col -= (col > 0); }
void right_key() { col += (col < buffer[row]->size); }

void key_backspace() {
  if (col) {
    --col;
    buffer[row]->del_char(col);
    return;
  } else {
    std::cerr << "CAN'T HANDLE 0 INDEXED DELETED YET";
  }
}
void key_enter() { std::cerr << "ENTER: NOT implimented"; }

void key_char(const char c) {
  if (buffer[row]->size + 1 < maxCols) {
    buffer[row]->add_char(c, col);
    ++col;
  } else {
    add_char_overflow(c);
    ++row;
    col = 1;
  }
}

void draw_buffer() {
  ui line_count = 0;
  werase(stdscr);
  const ui buff_size = buffer.size();
  for (ui i = firstVisibleRow; i < buff_size && line_count < maxRows; ++i) {
    Gstring *str = buffer[i].get();
    const ui size = str->size;
    for (ui x = 0; x < size; ++x) {
      mvaddch(i, x, str->char_at(x));
    }
    ++line_count;
  }
  move(row, col);
  wrefresh(stdscr);
}

void handleInput(int ch) {
  switch (ch) {
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

int main(int argc, char *argv[]) {
  buffer.emplace_back(std::make_unique<Gstring>());
  initscr();
  raw();
  keypad(stdscr, TRUE); // recognise arrows, F-keys, etc.
  noecho();             // don’t echo typed chars
  curs_set(1);          // make cursor visible

  /* Capture SIGWINCH so we can react to terminal resizes */
  std::signal(SIGWINCH, on_winch);

  /*RESTORE BELOW WHEN DONE TESTING*/
  getmaxyx(stdscr, maxRows, maxCols);
  /*RESTORE ABOVE WHEN DONE TESTING*/

  /*DELETE BELOW WHEN DONE TESTING*/
  maxCols = 10;
  /*DELETE ABOVE WHEN DONE TESTING*/

  std::string fileName = (argc > 1) ? argv[1] : "";
  // disp.draw_buffer();

  while (true) {
    int ch = getch();
    if (ch == 17 /* Ctrl-Q EXITS */)
      break;

    if (ch == KEY_RESIZE || resized) {
      resized = 0;
      /*RESTORE BELOW WHEN DONE TESTING*/
      // getmaxyx(stdscr, maxRows, maxCols);
      /*RESTORE ABOVE WHEN DONE TESTING*/

      // disp.on_resize(); // Need to write this
      // disp.draw_buffer();
      continue;
    }
    handleInput(ch);
  }
  endwin();
  return 0;
}

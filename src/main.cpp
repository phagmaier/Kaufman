#include "Editor.h"

int main(int argc, char *argv[]) {
  std::string fileName = (argc > 1) ? argv[1] : "";
  Editor editor = Editor(fileName);
  editor.main_loop();
  return 0;
}

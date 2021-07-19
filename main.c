#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "storage.h"

char input_buf[PAGE_SIZE];

void print_prompt_get_input() {
  printf("db > ");
  fgets(input_buf, PAGE_SIZE, stdin);
}

int main(int argc, char** argv) {
  char* filename;
  if (argc < 2) {
    // use the temporary database
    filename = "tmp.db";
  }
  else { filename = argv[1]; }
  table* tb = db_init(filename);
  
  bool run = true;
  while(run) {
    print_prompt_get_input();
    if (!strcmp(input_buf, "exit\n")) {
      db_close(tb);
      run = false;
    }
  }
  return 0;
}

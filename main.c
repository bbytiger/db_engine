#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define MAX_BUF_SZ 4096

char input_buf[MAX_BUF_SZ];

void print_prompt_get_input() {
  printf("db > ");
  fgets(input_buf, MAX_BUF_SZ, stdin);
}

int main(int argc, char** argv) {
  bool run = true;
  while(run) {
    print_prompt_get_input();
    if (!strcmp(input_buf, "exit\n")) run = false;
  }
  return 0;
}

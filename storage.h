#ifndef _STORAGE_H_
#define _STORAGE_H_

#include <stdint.h>

#define MAX_TABLE_PAGES 100
#define PAGE_SIZE 4096
#define ROW_SIZE 512
#define ROWS_PER_PAGE PAGE_SIZE / ROW_SIZE

// we need a quick way to test this program, maybe handle with python

typedef struct {
  int fd;
  uint32_t len;
  void* pages[MAX_TABLE_PAGES];
} pager;

typedef struct {
  pager* pgr;
  uint32_t num_rows;
} table;

void* pg_fetch(pager* pgr, uint32_t pg_num);
void pg_flush(pager* pgr, uint32_t pg_num, size_t sz);

table* db_init(const char* file);
void db_close(table* tb);

void serialize_row();
void deserialize_row();
void print_table();

#endif

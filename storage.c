#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/errno.h>
#include <unistd.h>

#include "storage.h"

void* pg_fetch(pager* pgr, uint32_t pg_num) {
  // case where max pg_num exceeded
  if (pg_num >= MAX_TABLE_PAGES) {
    printf("fetched page beyond max pages %d\n", pg_num);
    exit(EXIT_FAILURE);
  }
  // cache miss
  if (pgr->pages[pg_num] == NULL) {
    // load in the page from db
    void* pg = malloc(PAGE_SIZE);
    if (lseek(pgr->fd, pg_num * PAGE_SIZE, SEEK_SET) == -1) {
      printf("file seek failed in cache miss with errno %d\n", errno);
      exit(EXIT_FAILURE);
    }
    if (read(pgr->fd, pg, PAGE_SIZE) == -1) {
      printf("file read failed in cache miss with errno %d\n", errno);
      exit(EXIT_FAILURE);
    }    
    pgr->pages[pg_num] = pg;
  }
  return pgr->pages[pg_num];
}

void pg_flush(pager* pgr, uint32_t pg_num, size_t sz) {
  if (pgr->pages[pg_num] == NULL) {
    printf("cannot flush null page\n");
    exit(EXIT_FAILURE);
  }
  if (lseek(pgr->fd, pg_num * PAGE_SIZE, SEEK_SET) == -1) {
    printf("lseek inside flush failed with errno %d\n", errno);
    exit(EXIT_FAILURE);
  }
  if (write(pgr->fd, pgr->pages[pg_num], PAGE_SIZE) == -1) {
    printf("failed writing to disk with errno %d\n", errno);
    exit(EXIT_FAILURE);
  }
}

table* db_init(const char* file) {
  // open db
  int fd = open(file, O_RDWR | O_CREAT, S_IRWXU); // set permissions to most lenient
  // handle file open failure
  if (fd == -1) {
    printf("error opening file with errno %d\n", errno);
    exit(EXIT_FAILURE);
  }
  off_t file_sz = lseek(fd, 0L, SEEK_END);
  if (file_sz == -1) {
    printf("error seeking to end of file with errno %d\n", errno);
    exit(EXIT_FAILURE);
  }
  // init pager
  pager* pgr = malloc(sizeof(pager));
  pgr->fd = fd; pgr->len = file_sz;
  for (int i = 0; i < MAX_TABLE_PAGES; i++) {
    // set each page to a null pointer
    pgr->pages[i] = NULL;
  }
  // init table
  table* tb = malloc(sizeof(table));
  tb->pgr = pgr; tb->num_rows = pgr->len / ROW_SIZE;
  return tb;
}

void db_close(table* tb) {
  // flush the cache to disk
  if (tb->pgr) { // check if there is a pager
    for (int i = 0; i < tb->num_rows / ROWS_PER_PAGE; i++) {
      if (tb->pgr->pages[i] == NULL) {continue;}
      pg_flush(tb->pgr, i, PAGE_SIZE); // flush the entire apge
    }
  }
  // close file
  if (close(tb->pgr->fd) == -1) {
    printf("error closing db file\n");
    exit(EXIT_FAILURE);
  }
  // free all the memory
  if (tb->pgr) { // check if there is a pager
    for (int i = 0; i < MAX_TABLE_PAGES; i++) {
      free(tb->pgr->pages[i]); tb->pgr->pages[i] = NULL;
    }   
  }
  free(tb->pgr); free(tb);
}
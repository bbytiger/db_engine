#ifndef _DB_STATEMENT_H_
#define _DB_STATEMENT_H_

typedef enum {
  _success,
  _unrecognized,
} statement_resp;

typedef enum {
  _insert,
  _select,
} statement_type;

typedef enum {
  _prepare_success,
  _prepare_unrecognized,
} prepare_resp;

#endif

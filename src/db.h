#ifndef _DBLIB_H_
#define _DBLIB_H_

/* Libraries */
#include <sqlite3.h>

/* Macros */
#define MAX_SQL_REQUEST 1024

/* Struct */
struct sql_t{
    sqlite3_stmt *stmt;
    sqlite3 *db;
    char *err;
    int rc;
};

struct event_t {
    char *name, *time, *sponsor, *place;
};
    
/* Prototype */
int open_db(struct sql_t *db, const char *name);
int request_to_db(struct sql_t *db, char *request);
int add_event(struct sql_t *db, struct event_t *event, char *request);
int close_db(struct sql_t *db);

#endif


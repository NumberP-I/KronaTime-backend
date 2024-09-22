#include "db.h"

int open_db(struct sql_t *db, const char *name)
{
    db->rc = sqlite3_open(name, &db->db);
    if (db->rc != SQLITE_OK) {
        sqlite3_close(db->db);
        return 1;
    }
    return 0;
}

int request_to_db(struct sql_t *db, char *request)
{
    db->rc = sqlite3_exec(db->db, request, 0, 0, &db->err);
    if (db->rc != SQLITE_OK) {
        sqlite3_free(db->err);
        return 1;
    }
    return 0;
}

int close_db(struct sql_t *db)
{
    int res = sqlite3_close(db->db);
    if (res != SQLITE_OK)
        return 1;
    return 0;
}

int add_event(struct sql_t *db, struct event_t *event, char *request)
{
    sprintf(request, "INSERT INTO Event \
                     (event_name, event_time, event_sponsor, event_place) \
                     VALUES ('%s', '%s', '%s', '%s');",
                     event->name, event->time, event->sponsor, event->place);
    return request_to_db(db, request);
}


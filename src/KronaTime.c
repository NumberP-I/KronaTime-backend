#include <kore/kore.h>
#include <kore/http.h>
#include "db.h"

int write_event(struct http_request *);
int read_event(struct http_request *);

int write_event(struct http_request *req)
{
	// Init Variable for JSON and SQL
	struct kore_buf			buf;
	struct kore_json		json;
	struct kore_json_item	*item;
	struct event_t			*event = kore_calloc(1, sizeof(struct event_t));
	struct sql_t			*sql = kore_calloc(1, sizeof(struct sql_t));	

	open_db(sql, "event.db");
	kore_buf_init(&buf, 128);
	kore_json_init(&json, req->http_body->data, req->http_body->length);

	if (!kore_json_parse(&json)) {
		kore_buf_appendf(&buf, "%s\n", kore_json_strerror(&json));
	} else {
		// Find Name Event
		item = kore_json_find_string(json.root, "event/name");
		if (item != NULL) {
			kore_buf_appendf(&buf,
			    "event.name = '%s'\n", item->data.string);
			event->name = item->data.string;
		} else {
			kore_buf_appendf(&buf, "event.name %s\n",
			    kore_json_strerror(&json));
		}

		// Time
		item = kore_json_find_string(json.root, "event/time");
		if (item != NULL) {
			kore_buf_appendf(&buf,
			    "event.time = '%s\n", item->data.string);
			event->time = item->data.string;
		} else {
			kore_buf_appendf(&buf, "event.time %s\n",
			    kore_json_strerror(&json));
		}
		// Sponsor
		item = kore_json_find_string(json.root, "event/sponsor");
		if (item != NULL) {
			kore_buf_appendf(&buf,
			    "event.sponsor = '%s\n", item->data.string);
			event->sponsor = item->data.string;
		} else {
			kore_buf_appendf(&buf, "event.sponsor %s\n",
			    kore_json_strerror(&json));
		}
		// Place
		item = kore_json_find_string(json.root, "event/place");
		if (item != NULL) {
			kore_buf_appendf(&buf,
			    "event.place = '%s\n", item->data.string);
			event->place = item->data.string;	
		} else {
			kore_buf_appendf(&buf, "event.place %s\n",
			    kore_json_strerror(&json));
		}
		// WRITE CHECK ERROR JSON FILE
	}
	char *request = kore_calloc(MAX_SQL_REQUEST, sizeof(char));
	add_event(sql, event, request);

	kore_buf_cleanup(&buf);
	kore_json_cleanup(&json);
	close_db(sql);

	return (KORE_RESULT_OK);
}

int read_event(struct http_request *req)
{
	struct kore_buf			buf;
	struct kore_json		json;
	struct kore_json_item	*item;
	struct event_t			*event;
	struct sql_t			*sql;	

	open_db(sql, "event.db");
	kore_buf_init(&buf, 128);
	kore_json_init(&json, req->http_body->data, req->http_body->length);

	http_response(req, 200, buf.data, buf.offset);

	kore_buf_cleanup(&buf);
	kore_json_cleanup(&json);

	return (KORE_RESULT_OK);
}


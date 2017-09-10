#ifndef _LIST_H
#define _LIST_H
#include "session.h"

typedef struct session_list{
	SESSION *value;
	struct session_list *next;
}SESSION_LIST;

SESSION_LIST* create_session_list();
void insert_to_session_list(SESSION_LIST* head,SESSION *session);
void remove_from_list(SESSION_LIST* head,SESSION *session);
int get_list_length(SESSION_LIST* head);
#endif

#include "list.h"
#include <stdlib.h>

SESSION_LIST* create_session_list()
{
	SESSION_LIST* head = (SESSION_LIST*)malloc(sizeof(SESSION_LIST));
	if(!head)
	{
		perror("create session list error");
		exit(-1);
	}
	head->value = NULL;
	head->next = NULL;

	return head;
}

int get_list_length(SESSION_LIST* head)
{
    if(!head)
    {
        return 0;
    }
    int length = 0;
    SESSION_LIST *pIndex = head->next;
    while(pIndex)
    {
        length++;
        pIndex = pIndex->next;
    }
    return length;
}
void insert_to_session_list(SESSION_LIST* head,SESSION *session)
{
	SESSION_LIST* newitem = (SESSION_LIST*)malloc(sizeof(SESSION_LIST));
	newitem->next = head->next;
	newitem->value = session;
	head->next = newitem;
}

void remove_from_list(SESSION_LIST* head,SESSION *session)
{
	SESSION_LIST *pindex = head->next;
	SESSION_LIST *pprev = head;
	while(pindex)
	{
		if(pindex->value == session)
		{
			pprev->next = pindex->next;
			free(pindex->value);
			free(pindex);
			return;
		}
		pprev = pindex;
		pindex = pindex->next;

	}
}


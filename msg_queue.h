#ifndef __MSG_QUEUE_H
#define __MSG_QUEUE_H

#include <stdlib.h>
#include <string.h>

#define NAME_MAX_LENGTH 20
#define TEXT_MAX_LENGTH 50

typedef struct Message{
    struct Message *prev, *next;
    char name[NAME_MAX_LENGTH];
    char text[TEXT_MAX_LENGTH];
} Message;

typedef struct Msg_queue{
    struct Message *head, *tail;
    int num;
} Msg_queue;

extern Message *Message_new (const char *name, const char *text);
extern Msg_queue *Msg_queue_new ();
extern void Msg_queue_push (Msg_queue *msg_queue, Message *msg);
extern Message *Msg_queue_front (Msg_queue *msg_queue);
extern void Msg_queue_pop (Msg_queue *msg_queue);
extern int Msg_queue_get_num (Msg_queue *msg_queue);
extern int Msg_queue_is_empty (Msg_queue *msg_queue);

#endif
#include "msg_queue.h"

Message *Message_new (const char *name, const char *text){
    Message *msg = (Message *)malloc (sizeof(Message));
    msg->prev = NULL;
    msg->next = NULL;
    strcpy (msg->name, name);
    strcpy (msg->text, text);
    return msg;
}

Msg_queue *Msg_queue_new (){
    Msg_queue *msg_queue = (Msg_queue *)malloc (sizeof(Msg_queue));
    msg_queue->head = Message_new ("", "");
    msg_queue->tail = msg_queue->head;
    msg_queue->num = 0;
    return msg_queue;
}

void Msg_queue_push (Msg_queue *msg_queue, Message *msg){
    msg_queue->tail->next = msg;
    msg->prev = msg_queue->tail;
    msg->next = NULL;
    msg_queue->tail = msg;
    msg_queue->num++;
}

Message *Msg_queue_front (Msg_queue *msg_queue){
    return msg_queue->head->next;
}

void Msg_queue_pop (Msg_queue *msg_queue){
    if (msg_queue->num <= 0) return;
    Message *msg = msg_queue->head->next;
    msg_queue->head->next = msg->next;
    if (msg->next) msg->next->prev = msg_queue->head;
    free(msg);
    msg_queue->num--;
}

int Msg_queue_get_num (Msg_queue *msg_queue){
    return msg_queue->num;
}

int Msg_queue_is_empty (Msg_queue *msg_queue){
    return msg_queue->num == 0;
}

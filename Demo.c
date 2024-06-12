#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

typedef struct message_t {
    struct message_t* next;
    int id;
    size_t len;
    uint8_t* data;
} message_t;

#define TEMPLATE_PREFIX message
#define TEMPLATE_STRUCT message_t
#define TEMPLATE_NEXT next
#include "LinkedList.h"

message_t * messages;

void receive_message(int id, size_t len, uint8_t* data)
{
    static message_t** last_m = NULL;

    message_t* m = malloc(sizeof(message_t));

    if (m) {
        *m = (message_t){
            .id = id,
            .len = len,
            .data = data,
        };

        if (!last_m) last_m = &messages;

        message_push(last_m, m);
        last_m = &m->next;
    }
}

void print_messages(void)
{
    LL_ITERATOR it;
    message_t* m;

    for_each(message, &messages, m, it)
    {
        printf("%d, len:%d:, %s\n", m->id, m->len, m->data);
    }
}

void receive_message2(int id, size_t len, uint8_t* data)
{
    static message_t** last_m = NULL;

    message_t* m = malloc(sizeof(message_t));

    if (m) {
        *m = (message_t){
            .id = id,
            .len = len,
            .data = data,
        };

        if (!last_m) last_m = &messages;

        message_push(last_m, m);
        last_m = &m->next;
    }
}

void run_demo(void)
{
    receive_message(1, 25, "this is the 1st message");
    receive_message(2, 25, "this is the 2nd message");
    receive_message(3, 25, "this is the 3rd message");

    print_messages();

    messages = NULL;

    receive_message2(1, 25, "this is the 1st message");
    receive_message2(2, 25, "this is the 2nd message");
    receive_message2(3, 25, "this is the 3rd message");

    print_messages();
}

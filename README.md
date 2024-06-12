Preprocessor Templating System
------------------------------

Straight to the point, this is how you use this library

    typedef struct message_t {
        struct message_t* next;
        int id;
        size_t len;
        uint8_t * data;
    } message_t;

    #define TEMPLATE_PREFIX message
    #define TEMPLATE_STRUCT message_t
    #define TEMPLATE_NEXT next
    #include "LinkedList.h"

Including the `LinkedList.h` file as above causes the Preprocessor to generate a family of functions with the `TEMPLATE_PREFIX` used to prevent name collisions. You can include this header file multiple times with different `TEMPLATE` macros defined to use this for multiple data types.

With that small amount of code in place, you can now write:

    message_t* messages;
    
    void receive_message(int id, size_t len, uint8_t * data)
    {
        message_t* m = malloc(sizeof(message_t));

        if (m) {
            *m = (message_t){
                .id = id,
                .len = len,
                .data = data,
            };

            message_push(&messages, m);
        }
    }

The function `message_push` has this signature:

    void message_push(message_t ** head, message_t * item);

_Note: the functions all take a pointer to whatever memory the user is using to hold onto the datastructure `head`. This is used as a convention throughout this project to allow the datastructure functions to mutate any data in the datastructure, including the first element. The value of head is intentionally not checked for NULL. It is expected that the majority of calls to this function to be of the form `ll_push(&my_head, my_data)` in which case `head` will never be NULL._

Another neat feature, both `head` and `item` are pointers to the message_t type. There is nothing special about the "head" of this linked list. On valid way to append to the end of the linked list is to do this:

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

This uses `last_m` to save a reference to the NULL pointer at the end of the Linked List. This allows appending to the Linked List without iterating over the entire list each time a message is received.



#ifndef BUFFER_H
#define BUFFER_H

#include <stdio.h>
#include <stdint.h>

struct circular_buffer{

    uint32_t * data;
    int head;
    int tail;
    int max_length;
    int elements;

};

/*
Initilize an empty buffer 
*/
void init_buffer(struct circular_buffer* bufferPtr, uint32_t* data, int max_length);

/*
Adds value specified by variable value to the tail of the buffer 
that bufferPtr is pointing at.
*/
uint32_t add_element(struct circular_buffer* bufferPtr, uint32_t value);

/*
Removes the head of the buffer. i.e the oldest element in buffer
*/
uint32_t remove_head(struct circular_buffer* bufferPtr);

/*
Get elements in buffer
*/
uint32_t get_elements(struct circular_buffer* bufferPtr);


#endif
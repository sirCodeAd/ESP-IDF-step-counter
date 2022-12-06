#include "buffer.h"



void init_buffer(struct circular_buffer* bufferPtr, uint32_t* data, int max_length){

    bufferPtr->data       = data;
    bufferPtr->head       = 0;
    bufferPtr->tail       = 0;
    bufferPtr->max_length = max_length;
    
    bufferPtr->elements   = 0;

}

uint32_t add_element(struct circular_buffer* bufferPtr, uint32_t value){

  if(bufferPtr->elements == 0){      //if our buffer is empty we put the value into the buffers tail
  
    bufferPtr->data[bufferPtr->tail] = value;   
    bufferPtr->elements++;                    //increase number of elements
    
  }
    else{
    
      if(bufferPtr->elements == bufferPtr->max_length){  //if buffer is full of elements
      
        // uint32_t head_element = bufferPtr->data[bufferPtr->head]; //The element thats is in head of buffer and to be overwritten
       
        bufferPtr->tail = bufferPtr->head;          //tail is assigned the pointer to head
        bufferPtr->data[bufferPtr->tail] = value;   //value overwrites the value from in head and takes it place. 

        if(bufferPtr->head == bufferPtr->max_length - 1){   //if head pointer is at max length. start over at 0
        
        bufferPtr->head = 0;

        }
        else{

          bufferPtr->head = bufferPtr->head + 1;      //if its not full then increase pointer up by 1
        
        }
      }
      else{

        if (bufferPtr->tail == bufferPtr->max_length - 1){ //if tail is at max length, start over at 0
        
          bufferPtr->tail = 0;                          
          bufferPtr->data[bufferPtr->tail] = value;       //add new value there
        
        }
        else{                                          //if tail is not at max length increase by 1
        
          bufferPtr->tail++;
          bufferPtr->data[bufferPtr->tail] = value;    //add value there
        
        }

        bufferPtr->elements++;      //increase counter for total elements
      }
    }

  return value;
}

uint32_t remove_head(struct circular_buffer *bufferPtr){

  uint32_t value;

  if (bufferPtr->elements == 0){ //Buffer is empty

    return UINT32_MAX;

  }
  else if (bufferPtr->elements == 1){  //If only 1 element then head and tail is pointing on the same.
  
    value = bufferPtr->data[bufferPtr->head];
    bufferPtr->data[bufferPtr->head] = UINT32_MAX;
    bufferPtr->elements = bufferPtr->elements - 1;

    return value;

  }
  else{         //If there is more elements then head is moved to the oldest - 1 elemented

    value = bufferPtr->data[bufferPtr->head];
    bufferPtr->data[bufferPtr->head] = UINT32_MAX;

    if (bufferPtr->head == bufferPtr->max_length - 1){

      bufferPtr->head = 0;
    }
    else{

      bufferPtr->head = bufferPtr->head + 1;
    }

    bufferPtr->elements = bufferPtr->elements - 1;

    return value;
  }

}

uint32_t get_elements(struct circular_buffer* bufferPtr){
    
    return bufferPtr->elements;

}

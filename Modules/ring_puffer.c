//fegyizoli @ 2014.10.31.
#include <modules.h>
//körpuffer implementáció


//körpufferbe tesz egy bájtot
int8u ring_buffer_push(ring_buffer* buff, int8u c)
{
 if((buff->p_tail == buff->p_head) && buff->ring_buffer_full)
 {
  return RING_BUFF_OVERFLOW;
 }
 
 *buff->p_head++ = c; //push karakter
 
 if(buff->p_head >= buff->p_ring_buffer_end)
 { 
  buff->p_head = buff->buffer; //összekapcsoljuk a körpuffert
 }
 if(buff->p_head == buff->p_tail)
 { 
  buff->ring_buffer_full = true; //megtelt a körpuffer
 }
 return NO_ERROR;
}

//körpufferből kiszed egy byte-ot
int8u ring_buffer_pop(ring_buffer* buff, int8u* c)
{
 if((buff->p_head == buff->p_tail) && !buff->ring_buffer_full)
 {
  return RING_BUFF_EMPTY;
 }
 *c = *buff->p_tail++; //pop karakter
 if(buff->p_tail >= buff->p_ring_buffer_end)
 {
  buff->p_tail = buff->buffer; //összekapcsoljuk a körpuffert
 }
 buff->ring_buffer_full = false; //még van hely
 return NO_ERROR;
}

//lekérdezések
boolean ring_buffer_is_empty(ring_buffer* buff)
{
 return ((buff->p_tail == buff->p_head) && !buff->ring_buffer_full);
}

boolean ring_buffer_is_full(ring_buffer* buff)
{
 return (buff->ring_buffer_full);
}

//init körpuffer
void ring_buffer_init(ring_buffer* buff)
{
 buff->p_head = buff->p_tail = buff->buffer;
 buff->p_ring_buffer_end = &buff->buffer[RING_BUFFER_SIZE];
 buff->ring_buffer_full = false;
}



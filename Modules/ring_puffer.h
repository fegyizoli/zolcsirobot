//fegyizoli @ 2014.10.31.

//körpuffer implementáció headerje
#define RING_BUFFER_SIZE  64

#define NO_ERROR 0
#define RING_BUFF_OVERFLOW 1
#define RING_BUFF_EMPTY 2


struct ring_buff_
{
 int8u buffer[RING_BUFFER_SIZE];
 int8u* p_head;
 int8u* p_tail;
 int8u* p_ring_buffer_end;
 boolean ring_buffer_full;
};

typedef struct ring_buff_ ring_buffer;

//publikus függvények
int8u ring_buffer_push(ring_buffer* buff, int8u c);
int8u ring_buffer_pop(ring_buffer* buff, int8u* c);

boolean ring_buffer_is_empty(ring_buffer* buff);
boolean ring_buffer_is_full(ring_buffer* buff);

void ring_buffer_init(ring_buffer* buff);
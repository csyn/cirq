# cirq #

A circular queue implementation for microcontrollers

Cirq allows you to push and pop form both ends of the queue and also to peek at,
and place characters anywhere in between.

In this queue implementation tail should always points to a value of no
consequence, head points to a 'given value', except when the queue is empty.

`static inline struct cirq cirq_init(const size_t width,
    volatile unsigned char* buffer)`

Give it a size and a character buffer (of that size) and it will populate and
return a `struct cirq` for use with the below functions.

`static inline char cirq_empty(const struct cirq* const c)`

Checks for an empty queue, returns 1 if empty, otherwise it returns 0.

`static inline size_t cirq_space(const struct cirq* const c)`

Returns the space available in the buffer. i.e. How many times either of the
push functions can be called before the queue is full.

This function returns one less than the actual space available, because
when that actual space available is filled, head == tail and this is
indistinguishable from an empty buffer.

`static inline size_t cirq_population(const struct cirq* const c)`

Returns the number of characters in the queue.

`static inline void cirq_flush(struct cirq* const c)`

Resets the queue, population set to 0.

`static inline void cirq_push_back(struct cirq* const c,
    const unsigned char item)`

Pushes a character onto the tail of the queue.

`static inline void cirq_push_front(struct cirq* const c,
    const unsigned char item)`

Pushes a character onto the head of the queue.

`static inline unsigned char cirq_pop_back(struct cirq* const c)`

Pops a character from the tail of the queue.

`static inline unsigned char cirq_pop_front(struct cirq* const c)`

Pops a character from the head of the queue.

`static inline unsigned char cirq_peek_back(const struct cirq* const c,
    const size_t index);`

Returns the character at index `index` starting from the tail of the queue. Does
not modify the queue.

`static inline unsigned char cirq_peek_front(const struct cirq* const c,
    const size_t index);`

Returns the character at index `index` starting from the head of the queue. Does
not modify the queue.

`static inline void cirq_place_back(struct cirq* const c,
    const unsigned char item, const size_t index);`

Sets the character at index `index` starting from the tail of the queue to value
`item`.

`static inline void cirq_place_front(struct cirq* const c,
    const unsigned char item, const size_t index);`

Sets the character at index `index` starting from the head of the queue to value
`item`.

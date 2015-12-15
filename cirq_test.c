/*
The MIT License

Copyright (c) 2014 Julian Ingram.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
 */

#include "cirq.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <limits.h>

#define BUF_SIZE (128)
#define REPS (4096*1024)
#define SEED (time(NULL))

unsigned int rand_limited(unsigned int limit)
{
    unsigned int r_num = 0;
    if (limit)
    {
        if (limit > RAND_MAX)
        {
            limit = RAND_MAX;
        }
        static unsigned char rand_initialised = 0;
        if (!rand_initialised)
        {
            srand(SEED);
            rand_initialised = 1;
        }

        do
        {
            r_num = (unsigned int) (rand() / (unsigned int) (RAND_MAX / limit));
        }
        while (r_num >= limit);
    }
    return r_num;
}

unsigned char rand_char(void)
{
    return (unsigned char) (rand_limited(UCHAR_MAX + 1));
}

void rand_buffer(unsigned char* buf, size_t size)
{
    size_t i = 0;
    while (i < size)
    {
        buf[i] = rand_char();
        ++i;
    }
}

void rand_head(struct cirq* c)
{
    unsigned int rand = rand_limited(BUF_SIZE);
    volatile unsigned char* head = c->buffer + rand;
    c->head = head;
    c->tail = head;
}

void test_push_pop()
{
    unsigned char buff[BUF_SIZE];
    struct cirq cf = cirq_init(BUF_SIZE, buff);
    unsigned char bufb[BUF_SIZE];
    struct cirq cb = cirq_init(BUF_SIZE, bufb);

    unsigned char rand_buf[BUF_SIZE - 1];
    rand_buffer(rand_buf, BUF_SIZE - 1);

    unsigned int rep = 0;
    while (rep < REPS)
    {
        rand_head(&cf);
        rand_head(&cb);

        unsigned int size = rand_limited(BUF_SIZE - 1);
        unsigned int i = 0;
        while (i < size)
        {
            cirq_push_back(&cf, rand_buf[i]);
            cirq_push_front(&cb, rand_buf[i]);
            ++i;
        }

        assert(cirq_empty(&cf) == (size == 0) ? 1 : 0);
        assert(cirq_empty(&cb) == (size == 0) ? 1 : 0);
        assert(cirq_population(&cf) == size);
        assert(cirq_population(&cb) == size);
        assert(cirq_space(&cf) == ((BUF_SIZE - 1) - size));
        assert(cirq_space(&cb) == ((BUF_SIZE - 1) - size));

        i = 0;
        while (i < size)
        {
            assert(cirq_pop_front(&cf) == rand_buf[i]);
            assert(cirq_pop_back(&cb) == rand_buf[i]);
            ++i;
        }

        assert(cirq_empty(&cf) == 1);
        assert(cirq_empty(&cb) == 1);
        assert(cirq_population(&cf) == 0);
        assert(cirq_population(&cb) == 0);
        assert(cirq_space(&cf) == (BUF_SIZE - 1));
        assert(cirq_space(&cb) == (BUF_SIZE - 1));

        ++rep;
    }
}

void test_poke_peek()
{
    unsigned char buff[BUF_SIZE];
    struct cirq cf = cirq_init(BUF_SIZE, buff);
    unsigned char bufb[BUF_SIZE];
    struct cirq cb = cirq_init(BUF_SIZE, bufb);

    unsigned char rand_buf[BUF_SIZE - 1];
    rand_buffer(rand_buf, BUF_SIZE - 1);

    unsigned int rep = 0;
    while (rep < REPS)
    {
        rand_head(&cf);
        rand_head(&cb);

        unsigned int size = 0;
        while (size == 0)
        {
            size = rand_limited(BUF_SIZE - 1);
        }

        // fill buffer
        unsigned int i = 0;
        while (i < size)
        {
            cirq_push_back(&cf, rand_buf[i]);
            cirq_push_front(&cb, rand_buf[i]);
            ++i;
        }

        assert(cirq_empty(&cf) == (size == 0) ? 1 : 0);
        assert(cirq_empty(&cb) == (size == 0) ? 1 : 0);
        assert(cirq_population(&cf) == size);
        assert(cirq_population(&cb) == size);
        assert(cirq_space(&cf) == ((BUF_SIZE - 1) - size));
        assert(cirq_space(&cb) == ((BUF_SIZE - 1) - size));

        // peek
        i = 0;
        while (i < size)
        {
            assert(cirq_peek_front(&cf, i) == rand_buf[i]);
            assert(cirq_peek_back(&cb, i) == rand_buf[i]);
            ++i;
        }

        // poke
        i = 0;
        while (i < size)
        {
            unsigned char rc = rand_char();
            cirq_place_front(&cf, rc, i);
            cirq_place_back(&cb, rc, i);
            rand_buf[i] = rc;
            ++i;
        }

        // pop
        i = 0;
        while (i < size)
        {
          assert(cirq_pop_front(&cf) == rand_buf[i]);
          assert(cirq_pop_back(&cb) == rand_buf[i]);
            ++i;
        }

        assert(cirq_empty(&cf) == 1);
        assert(cirq_empty(&cb) == 1);
        assert(cirq_population(&cf) == 0);
        assert(cirq_population(&cb) == 0);
        assert(cirq_space(&cf) == (BUF_SIZE - 1));
        assert(cirq_space(&cb) == (BUF_SIZE - 1));

        ++rep;
    }
}

int main(void)
{
    test_push_pop();
    test_poke_peek();

    puts("******************");
    puts("** TEST SUCCESS **");
    puts("******************");

    return 0;
}

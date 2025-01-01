#ifndef __CHAR_BUFFER_HPP__
#define __CHAR_BUFFER_HPP__

#include <stdio.h>
#include <stdlib.h>
#include "custom_types.hpp"
#include "process_string.hpp"

enum { buffer_len = 1024 };

// Helper structure to work with char buffer
struct CharBuffer { 
    char *buf;
    uint capacity;
    uint pos;
    uint len;
    bool seen_data; // whether a new number is started or not
public:
    CharBuffer(uint capacity_in = buffer_len)
        : capacity(capacity_in)
        , pos(capacity_in)
        , len(capacity_in)
        , seen_data(false)
        { buf = new char[capacity]; }

    ~CharBuffer()
        { if (buf) delete[] buf; }

    char next();
    void flush_tabs_and_spaces();
private:
    CharBuffer(const CharBuffer& cbuf) {}; // copy prohibited
};

#endif

// Author: Pavel Kharyuk, 2024
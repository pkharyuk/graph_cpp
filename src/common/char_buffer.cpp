#include "char_buffer.hpp"

char CharBuffer::next() {
    if (this->pos >= this->len) {
        fprintf(
            stderr,
            "CharBuffer::next() : invalid buffer position encountered\n"
        );
        exit(EXIT_FAILURE);
    }
    char c = this->buf[this->pos];
    this->pos += 1;
    if (is_tab_or_space(c) || is_newline(c))
        this->seen_data = false;
    else
        this->seen_data = true;
    return c;
}

/* Flush tab/spaces starting from the current buffer position */
void CharBuffer::flush_tabs_and_spaces()
{
    while (
        (this->pos < this->len)
        && is_tab_or_space(this->buf[this->pos])
    ) {
        this->pos += 1;
    }
}


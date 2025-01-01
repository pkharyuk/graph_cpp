#ifndef __DIALOGUE_HPP__
#define __DIALOGUE_HPP__

#include <stdio.h>
#include <stdlib.h>

#include "custom_utils.hpp"
#include "char_buffer.hpp"
#include "game_types.hpp"

class UserDialogue {
    CharBuffer *buf;

public:
    UserDialogue()
        { buf = new CharBuffer(); }
    ~UserDialogue()
        { if (buf) delete buf; }

    void greetings(enum NodeColors& selected_color, uint maxitnum = 3);
    void print_turn_header(uint n) const;
    void congratulate(int winner_code) const;
    void read_position(uint &i, uint &j, uint n);

private:
    void read_input();
    char *read_line();
    void flush_buffer();
    void flush_stdin() const;
};


#endif
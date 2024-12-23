/* This module is designed for extracting a
rational number array from the input file by
a character-wise processing. */

#ifndef __HANDLE_INPUT_H__
#define __HANDLE_INPUT_H__

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdarg.h>
#include <unistd.h>
#include "process_string.hpp"

//#include "process_string.h"
//#include "rational_number.h"

enum { expected_data_len = 2 };
enum { buffer_len = 1024 };

class CharBuffer;

class TextFileReader {
    const char *path;
    CharBuffer *buf;
    int fd;
public:
    TextFileReader(const char *fpath);
    ~TextFileReader();

    char *read_line(
        bool flush_spaces_and_tabs = true
    );

private:
    TextFileReader(const TextFileReader& other) {};
    void open_file();
    void close_file();
    void read_chunk();
};

#endif
// Author: Pavel Kharyuk, 2024
#ifndef __HANDLE_INPUT_H__
#define __HANDLE_INPUT_H__

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdarg.h>
#include <unistd.h>
#include "process_string.hpp"
#include "char_buffer.hpp"

//#include "process_string.h"
//#include "rational_number.h"

enum { expected_data_len = 2 };

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

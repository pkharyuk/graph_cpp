#include "handle_input.hpp"


// class TextFileReader

TextFileReader::TextFileReader(const char *fpath)
    : path(fpath), fd(-1)
{
    buf = new CharBuffer;
    open_file();
}
    
TextFileReader::~TextFileReader()
{
    close_file();
    delete buf;
}

void TextFileReader::open_file()
{
    this->fd = open(this->path, O_RDONLY);
    if (this->fd < 0) {
        fprintf(stderr, "Could not open a file.\n");
        exit(EXIT_FAILURE);
    }
}

void TextFileReader::close_file()
{
    int close_status = close(this->fd);
    if (close_status != 0) {
        fprintf(stderr, "Could not close a file.\n");
        exit(EXIT_FAILURE);
    }
}

void TextFileReader::read_chunk()
{
    this->buf->len = read(
        this->fd, this->buf->buf, this->buf->capacity
    );
    this->buf->pos = 0;
}


char *TextFileReader::read_line(bool flush_spaces_and_tabs)
{
    char c;
    uint res_pos = 0;
    uint res_len = this->buf->capacity + 1;
    char *result = new char[res_len];
    while (this->buf->len > 0) {
        if (this->buf->pos == this->buf->len) {
            read_chunk();
            if (res_pos == 0)
                this->buf->flush_tabs_and_spaces();
            continue;
        }
        c = this->buf->next();
        if (is_newline(c))
            break;
        if (is_tab_or_space(c))
            this->buf->flush_tabs_and_spaces();
        if (res_pos == res_len) {
            resize_string(&result, res_len, res_len + this->buf->capacity);
            res_len += this->buf->capacity;
        }
        result[res_pos] = c;
        res_pos += 1;
    }
    if (res_pos == 0 && this->buf->len == 0) {
        delete[] result;
        return 0;
    }
    this->buf->flush_tabs_and_spaces();
    if (is_tab_or_space(result[res_pos]))
        res_pos -= 1;
    resize_string(&result, res_len, res_pos+1);
    result[res_pos] = '\0';
    return result;
}


// Author: Pavel Kharyuk, 2024

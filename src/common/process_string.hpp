#ifndef __PROCESS_STRING_HPP__
#define __PROCESS_STRING_HPP__

#include <stdio.h>
#include <stdlib.h>

#include "custom_utils.hpp"

void resize_string(char **s, uint old_size, uint new_size);
void string_to_uint_vec(const char *s, uint **a, uint *n);
void stripn_string(const char *src, char **dst, uint n, uint *new_n);
bool is_line_uint_vec(const char *line);

// Few functions for string processing

bool is_digit(char c);
bool is_dot(char c);
bool is_tab_or_space(char c);
bool is_newline(char c);
uint char_to_uint(char c);
uint digit_to_uint(char c); 
uint safe_atou(const char *s); // converts string to unsigned integer
double safe_atod(const char *s); // converts string to double


#endif

// Author: Pavel Kharyuk, 2024

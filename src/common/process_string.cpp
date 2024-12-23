#include "process_string.hpp"

void resize_string(char **s, uint old_size, uint new_size)
{
    resize_1d_array<char>(s, old_size, new_size);
}

static uint get_uint_from_string(const char *s, uint lpos, uint rpos)
{
    char *tmp = new char[rpos-lpos+1];
    for (uint i = lpos; i < lpos+rpos; i++)
        tmp[i-lpos] = s[i];
    tmp[rpos-lpos] = '\0';
    uint rv = safe_atou(tmp);
    delete[] tmp;
    return rv;
}

// assumption: all numbers are single space separated
void string_to_uint_vec(const char *s, uint **result, uint *size)
{
    uint lpos = 0, rpos = 0, n = 2;
    *size = 0;
    *result = new uint[n];
    while (s[rpos] != '\0') {
        while (s[rpos] != '\0' && !is_tab_or_space(s[rpos]))
            rpos += 1;
        if (*size == n-1) {
            resize_1d_array<uint>(result, n, 2*n);
            n *= 2;
        }
        (*result)[*size] = get_uint_from_string(s, lpos, rpos);
        *size += 1;
        lpos = rpos + 1;
        rpos = lpos;
    }
    if (lpos != rpos) {
        (*result)[*size] = get_uint_from_string(s, lpos, rpos);
        *size += 1;
    }
    resize_1d_array<uint>(result, n, *size);
}

bool is_digit(char c)
{
    return (c >= '0' || c <= '9');
}

bool is_dot(char c)
{
    return (c == '.');
}

bool is_tab_or_space(char c)
{
    return (c == ' ' || c == '\t');
}

bool is_newline(char c)
{
    return (c == '\n');
}

uint char_to_uint(char c)
{
    return (uint) (c - 'a');
}

uint digit_to_uint(char c)
{
    return (uint) (c - '0');
}

// converts string to unsigned integer
uint safe_atou(const char *s)
{
    uint ans = 0;
    uint pans = 0;
    uint n = strlen(s);
    for (uint i = 0; i < n; i++) {
        if (!is_digit(s[i])) {
            fprintf(stderr, "safe_atou: invalid character\n");
            exit(EXIT_FAILURE);
        }
        pans = ans;
        ans = ten*ans + digit_to_uint(s[i]);
        if (pans > ans)  {
            fprintf(stderr, "safe_atou: overflow encountered\n");
            exit(EXIT_FAILURE);
        }
    }
    return ans;
}

// converts string to double
double safe_atod(const char *s)
{
    double ans = 0;
    double frac_ten = 0.1;
    uint n = strlen(s);
    uint i;
    for (i = 0; i < n; i++) {
        if (is_dot(s[i]))
            break;
        if (!is_digit(s[i])) {
            fprintf(stderr, "safe_atod: invalid character\n");
            exit(EXIT_FAILURE);
        }
        ans = ten*ans + digit_to_uint(s[i]);
    }
    for (uint j = i+1; j < n; j++) {
        if (!is_digit(s[j])) {
            fprintf(stderr, "safe_atod: invalid character\n");
            exit(EXIT_FAILURE);
        }
        ans += digit_to_uint(s[j])*frac_ten;
        frac_ten /= ten;
    }
    return ans;
}

// Author: Pavel Kharyuk, 2024
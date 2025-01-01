#include "handle_cl_args.hpp"

// CL arguments parser
InputArguments *parse_cl_args(int argc, const char *argv[])
{
    int option_ind;
    bool error_flag = false;
    bool used_args[n_eng_letters];
    for (uint i = 0; i < n_eng_letters; i++)
        used_args[i] = false;
    InputArguments *input_args = new InputArguments();
    for (option_ind = 1; option_ind < argc; option_ind = option_ind + 2) {
        const char *c_option = argv[option_ind];
        if (
            (strlen(c_option) != expected_option_len)
            || (c_option[0] != '-')
            || (used_args[char_to_uint(c_option[1])])
         )
        {
            error_flag = true;
            break;
        }
        used_args[char_to_uint(c_option[1])] = true;
        switch (c_option[1]) {
            case 's':
                input_args->seed = safe_atou(argv[option_ind+1]);
                break;
            case 'n':
                input_args->n_nodes = safe_atou(argv[option_ind+1]);
                break;
            case 'd':
                input_args->density = safe_atod(argv[option_ind+1]);
                break;
            case 'a':
                input_args->min_distance = safe_atou(argv[option_ind+1]);
                break;
            case 'b':
                input_args->max_distance = safe_atou(argv[option_ind+1]);
                break;
            case 'f':
                input_args->fpath = argv[option_ind+1];
                break;
            default:
                error_flag = true;
                break;
        }
        if (error_flag)
            break;
    }
    if (error_flag) {
        fprintf(
            stderr,
            "Usage: \n (random sample) -s <seed> -n <n_nodes> -d <density>"
        );
        fprintf(
            stderr,
            " -a <min_dist> -b <max_dist>\n"
        );
        fprintf(
            stderr,
            " (from file) -s <seed> -f <file_path>\n"
        );
        exit(EXIT_FAILURE);
    }
    return input_args;
}


// Author: Pavel Kharyuk, 2024
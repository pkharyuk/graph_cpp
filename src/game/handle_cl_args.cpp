#include "handle_cl_args.hpp"

static const char *info_msg[] = {
    "Usage: ./<prog_name> -s <seed> -n <size> -p <player_code>",
    " -m <maxitnum> -b <bot_strategy_code>\n",
    "<player_code>=R => RED [default]\n",
    "<player_code>=B => BLUE\n",
    "<bot_stategy_code>=0 => Monte-Carlo lookahead\n",
    "<bot_stategy_code>=1 => shortest path completion\n",
    "<bot_stategy_code>=2 => random_strategy at each turn (0 or 1)\n"
};

// CL arguments parser
InputArguments *parse_cl_args(int argc, const char *argv[])
{
    int option_ind;
    bool error_flag = false;
    bool used_args[n_eng_letters];
    for (uint i = 0; i < n_eng_letters; i++)
        used_args[i] = false;
    InputArguments *input_args = new InputArguments();
    if (argc % 2 == 0)
        error_flag = true;
    for (option_ind = 1; option_ind < argc; option_ind = option_ind + 2) {
        const char *c_option = argv[option_ind];
        if (
            (strlen(c_option) != expected_option_len)
            || (c_option[0] != '-')
            || (used_args[char_to_uint(c_option[1])])
         )
            error_flag = true;
        if (error_flag)
            break;
        used_args[char_to_uint(c_option[1])] = true;
        switch (c_option[1]) {
            case 'b':
                input_args->bot_strategy = safe_atou(argv[option_ind+1]);
                break;
            case 's':
                input_args->seed = safe_atou(argv[option_ind+1]);
                break;
            case 'n':
                input_args->size = safe_atou(argv[option_ind+1]);
                break;
            case 'm':
                input_args->maxitnum = safe_atou(argv[option_ind+1]);
                break;
            case 'p':
                if (strlen(argv[option_ind+1]) == 1)
                    input_args->player_code = argv[option_ind+1][0];
                else
                    error_flag = true;
                break;
            default:
                error_flag = true;
                break;
        }
        if (error_flag)
            break;
    }
    if (error_flag) {
        for (uint i = 0; i < sizeof(info_msg)/sizeof(*info_msg); i++)
            fprintf(stderr, info_msg[i]);
        exit(EXIT_FAILURE);
    }
    return input_args;
}


// Author: Pavel Kharyuk, 2024
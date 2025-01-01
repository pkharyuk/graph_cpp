#include "game_types.hpp"


enum NodeColors NodeColorsConverter::get_enum_color_from_int(int code)
{
    switch (code) {
        case NodeColors::red:
            return NodeColors::red;
        case NodeColors::blue:
            return NodeColors::blue;
        case NodeColors::blank:
            return NodeColors::blank;
        default:
            break;
    }
    return NodeColors::not_a_color;
}


enum NodeColors NodeColorsConverter::get_opponent_color(int code)
{
    switch (code) {
        case NodeColors::red:
            return NodeColors::blue;
        case NodeColors::blue:
            return NodeColors::red;
        default:
            break;
    }
    return NodeColors::not_a_color;
}

char NodeColorsConverter::decode(int code)
{
    switch (code) {
        case NodeColors::red:
            return 'R';
        case NodeColors::blue:
            return 'B';
        case NodeColors::blank:
            return '.';
        default:
            fprintf(stderr, "Invalid node color\n");
            exit(EXIT_FAILURE);
    }
    return 0;
}

enum NodeColors NodeColorsConverter::encode(char char_code)
{
    switch (char_code) {
        case 'R': case 'r':
            return NodeColors::red;
        case 'B': case 'b':
            return NodeColors::blue;
        case '.':
            return NodeColors::blank;
        default:
            break;
    }
    return NodeColors::not_a_color;
}

const char *NodeColorsConverter::get_color_name(int code)
{
    switch (code) {
        case NodeColors::red:
            return "Red";
            break;
        case NodeColors::blue:
            return "Blue";
            break;
        default:
            break;
    };
    return 0; // do we really have to deal it in a more complicated way her?
}
#include "dialogue.hpp"

void UserDialogue::flush_buffer()
{
    this->buf->pos = 0;
    this->buf->len = 0;
}

void UserDialogue::flush_stdin() const
{
    int c;
    do {
        c = getchar();
    } while (c != EOF && c != '\n');
    if (c == EOF) {
        clearerr(stdin);
        printf("\n");
    }
}

void UserDialogue::read_input()
{
    int c = '\n';
    while (this->buf->len < this->buf->capacity) {
        c = getchar();
        if (c == EOF || c == '\n')
            break;
        this->buf->buf[this->buf->len] = c;
        this->buf->len += 1;
    }
    //this->flush_stdin();
    if (c == EOF) {
        clearerr(stdin);
        printf("\n");
    }
}

static bool is_selection_correct(enum NodeColors& selected_color)
{
    return (
        (selected_color == NodeColors::red)
        || (selected_color == NodeColors::blue)
    );
}

void UserDialogue::greetings(
    enum NodeColors& selected_color,
    uint maxitnum
)
{
    printf("Greetings from Hex Board Game! Please select your color:\n");
    bool selection_correct = is_selection_correct(selected_color);
    uint itnum = 0;
    this->flush_buffer();
    do {
        if (itnum >= maxitnum) {
            printf("Invalid input. Please restart the game.\n");
            exit(EXIT_SUCCESS);
        }
        itnum += 1;
        printf("(R=Red or B=Blue) [R/B]: ");
        this->read_input();
        if (this->buf->len > 1 || (this->buf->len == 0 && !selection_correct)) {
            this->flush_buffer();
            selection_correct = false;
            continue;
        }
        if (this->buf->len == 1) {
            char char_color = this->buf->buf[this->buf->pos];
            selected_color = NodeColorsConverter::encode(char_color);
            selection_correct = is_selection_correct(selected_color);
        }
    } while (!selection_correct);
    this->flush_buffer();
    const char *s = NodeColorsConverter::get_color_name(selected_color);
    printf(
        "\nGot it! You selected the <%s> color. Your game is ready.\n", s
    );
}

void UserDialogue::read_position(uint& i, uint& j, uint n)
{
    const uint expected_length = 2;
    printf("Please enter the (row, column) position to dye it with your color:\n");
    uint itnum = 0;
    uint maxitnum = 10;
    uint len_a = 0;
    while (len_a != expected_length) {
        if (itnum >= maxitnum)
            break;
        itnum += 1;
        printf(
            "Enter two space-separated numbers (i j), 1 <= i, j <= %u: ", n
        );
        char *line = this->read_line();
        if (!line)
            continue;
        if (!is_line_uint_vec(line)) {
            delete[] line;
            continue;
        }
        uint *a = 0;
        string_to_uint_vec(line, &a, &len_a);
        if (len_a == expected_length) {
            i = a[0];
            j = a[1];
        }
        delete[] a;
        delete[] line;
    }
    this->flush_buffer();
}

char *UserDialogue::read_line()
{
    this->flush_buffer();
    this->read_input();
    this->buf->flush_tabs_and_spaces();
    uint res_pos = 0;
    uint res_len = this->buf->len + 1;
    char *line = new char[res_len];
    while (this->buf->pos < this->buf->len) {
        char c = this->buf->next();
        if (is_newline(c))
            break;
        if (is_tab_or_space(c))
            this->buf->flush_tabs_and_spaces();
        line[res_pos] = c;
        res_pos += 1;
    }
    if (res_pos == 0) {
        delete[] line;
        return 0;
    }
    if (is_tab_or_space(line[res_pos]))
        res_pos -= 1;
    line[res_pos] = '\0';
    resize_string(&line, res_len, res_pos+1);
    return line;
}

void UserDialogue::print_turn_header(uint n) const
{
    printf(
        "##################################################################\n"
    );
    printf("#               Turn %u\n", n);
    printf(
        "##################################################################\n"
    );
}

void UserDialogue::congratulate(int winner_code) const
{
    const char *s = NodeColorsConverter::get_color_name(winner_code);
    printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
    printf("%s won!\n", s);
    printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
}

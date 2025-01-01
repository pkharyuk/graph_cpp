#include "custom_types.hpp"
#include "graph.hpp"
#include "shortest_path.hpp"
#include "handle_cl_args.hpp"

#include "board.hpp"
#include "dialogue.hpp"
#include "player.hpp"

static inline void process_player_turn(
	PlayBoard& board,
	UserPlayer *user_player,
	UserDialogue& dialogue,
	const uint& board_size
)
{
	uint i, j;
	bool success = false;
	while (!success) {
		dialogue.read_position(i, j, board_size);
		success = user_player->move(board, i, j);
		if (!success)
			printf("Invalid input. Try again.\n");
	}
}

static void game_loop(
	PlayBoard& board, UserDialogue& dialogue, UserPlayer *user, BotPlayer *bot
)
{
	enum NodeColors user_color = NodeColorsConverter::get_enum_color_from_int(
		user->get_color_code()
	);
	uint board_size = board.get_size();
	bool success;
	while (board.is_game_active()) {
		dialogue.print_turn_header(board.get_current_turn_num());
		if (user_color == NodeColors::blue) {
			// blue goes first
			board.draw();
			process_player_turn(board, user, dialogue, board_size);
			success = bot->move(board);
			board.lookup_winner();
		} else {
			success = bot->move(board);
			board.lookup_winner();
			if (!board.is_game_active())
				break;
			board.draw();
			process_player_turn(board, user, dialogue, board_size);
		}
		board.increment_turn_counter();
	}
	int winner_code = board.get_winner();
	board.draw();
	dialogue.congratulate(winner_code);
}

// command-line interface:
// ./<prog_name> -s <seed> -n <size> -p <player_code>
//               -m <maxitnum>
// <player_code>=0 => RED [default]
// <player_code>=1 => BLUE

int main(int argc, char *argv[])
{
	InputArguments *input_args = parse_cl_args(argc, (const char **) argv);
	enum NodeColors user_color = NodeColorsConverter::encode(
		input_args->player_code
	);
	UserDialogue dialogue;
	dialogue.greetings(user_color);

	enum NodeColors bot_color = NodeColorsConverter::get_opponent_color(
		user_color
	);
	UserPlayer *user = new UserPlayer(user_color, player_chosed_fill_value);
	BotPlayer *bot = new BotPlayer(
		bot_color, bot_chosed_fill_value, input_args->seed,
		input_args->maxitnum
	);
	PlayBoard board(input_args->size); //, user_color);

	game_loop(board, dialogue, user, bot);

	delete bot;
	delete user;
	delete input_args;
	return 0;
}

// Author: Pavel Kharyuk, 2024
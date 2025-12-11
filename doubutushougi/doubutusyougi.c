#include <stdio.h>
#include <string.h>

#define BOARD_ROWS 4
#define BOARD_COLS 3
#define PIECE_TYPES 4  // ELEPHANT, GIRAFFE, CHICK, HEN

typedef enum {
    EMPTY,
    LION1,
    ELEPHANT1,
    GIRAFFE1,
    CHICK1,
    HEN1,
    LION2,
    ELEPHANT2,
    GIRAFFE2,
    CHICK2,
    HEN2
} Piece;

typedef enum { PLAYER1 = 1, PLAYER2 = 2 } Player;

const char *piece_repr[] = {".",  "L1", "E1", "G1", "C1", "H1",
                            "L2", "E2", "G2", "C2", "H2"};

typedef struct {
    Piece board[BOARD_ROWS][BOARD_COLS];
    int turn;                    // PLAYER1 or PLAYER2
    int captured1[PIECE_TYPES];  // ELEPHANT, GIRAFFE, CHICK, HEN
    int captured2[PIECE_TYPES];
} GameState;

// --- Utility Functions ---

int is_valid_pos(int r, int c) {
    return r >= 0 && r < BOARD_ROWS && c >= 0 && c < BOARD_COLS;
}

int is_own_piece(Piece p, Player turn) {
    if (turn == PLAYER1)
        return p >= LION1 && p <= HEN1;
    else
        return p >= LION2 && p <= HEN2;
}

int is_opponent_piece(Piece p, Player turn) {
    if (turn == PLAYER1)
        return p >= LION2 && p <= HEN2;
    else
        return p >= LION1 && p <= HEN1;
}

// --- Board Initialization ---

void init_board(GameState *state) {
    memset(state, 0, sizeof(GameState));
    // Player 1 (bottom)
    state->board[3][2] = GIRAFFE1;
    state->board[3][1] = LION1;
    state->board[3][0] = ELEPHANT1;
    state->board[2][1] = CHICK1;
    // Player 2 (top)
    state->board[0][2] = ELEPHANT2;
    state->board[0][1] = LION2;
    state->board[0][0] = GIRAFFE2;
    state->board[1][1] = CHICK2;
    state->turn = PLAYER1;
    memset(state->captured1, 0, sizeof(state->captured1));
    memset(state->captured2, 0, sizeof(state->captured2));
}

// --- Board Display ---

void print_board(const GameState *state) {
    printf("   0   1   2\n");
    for (int r = 0; r < BOARD_ROWS; r++) {
        printf("%d ", r);
        for (int c = 0; c < BOARD_COLS; c++) {
            printf("%-3s ", piece_repr[state->board[r][c]]);
        }
        printf("\n");
    }
    printf("Player 1 HAVE: E:%d G:%d C:%d\n", state->captured1[0],
           state->captured1[1], state->captured1[2], state->captured1[3]);
    printf("Player 2 HAVE: E:%d G:%d C:%d\n", state->captured2[0],
           state->captured2[1], state->captured2[2], state->captured2[3]);
}

// --- Move Validation ---

int can_move(const GameState *state, Piece p, int from_r, int from_c, int to_r,
             int to_c, Player turn) {
    if (!is_valid_pos(to_r, to_c) || p == EMPTY) return 0;
    if (!is_own_piece(p, turn)) return 0;

    Piece dest = state->board[to_r][to_c];
    if (is_own_piece(dest, turn) && dest != EMPTY) return 0;

    int dr = to_r - from_r;
    int dc = to_c - from_c;
    int forward = (turn == PLAYER1) ? -1 : 1;

    switch (p) {
        case LION1:
        case LION2:
            return (dr >= -1 && dr <= 1 && dc >= -1 && dc <= 1 &&
                    (dr != 0 || dc != 0));
        case ELEPHANT1:
        case ELEPHANT2:
            return ((dr == 1 || dr == -1) && (dc == 1 || dc == -1));
        case GIRAFFE1:
        case GIRAFFE2:
            return ((dr == 0 && (dc == 1 || dc == -1)) ||
                    (dc == 0 && (dr == 1 || dr == -1)));
        case CHICK1:
        case CHICK2:
            return (dr == forward && dc == 0);
        case HEN1:
        case HEN2:
            return ((dr == forward && dc == 0) ||
                    (dr == 0 && (dc == 1 || dc == -1)) ||
                    (dr == -forward && (dc == 1 || dc == -1)));
        default:
            return 0;
    }
}

// --- Piece Drop ---

Piece get_drop_piece(Player turn, int piece_type) {
    static const Piece drop_table[2][PIECE_TYPES] = {
        {ELEPHANT1, GIRAFFE1, CHICK1, HEN1},
        {ELEPHANT2, GIRAFFE2, CHICK2, HEN2}};
    return drop_table[turn - 1][piece_type];
}

int drop_piece(GameState *state, Player turn, int piece_type, int to_r,
               int to_c) {
    if (!is_valid_pos(to_r, to_c) || state->board[to_r][to_c] != EMPTY)
        return 0;
    if (piece_type < 0 || piece_type >= PIECE_TYPES) return 0;

    int *captured = (turn == PLAYER1) ? state->captured1 : state->captured2;
    if (captured[piece_type] <= 0) return 0;

    state->board[to_r][to_c] = get_drop_piece(turn, piece_type);
    captured[piece_type]--;
    return 1;
}

// --- Capture Handling ---

void add_captured_piece(GameState *state, Player turn, Piece captured) {
    int captured_index = -1;
    if (captured == HEN1 || captured == HEN2) {
        captured_index = 2;  // CHICK
    } else {
        switch (captured) {
            case ELEPHANT1:
            case ELEPHANT2:
                captured_index = 0;
                break;
            case GIRAFFE1:
            case GIRAFFE2:
                captured_index = 1;
                break;
            case CHICK1:
            case CHICK2:
                captured_index = 2;
                break;
            default:
                break;
        }
    }
    if (captured_index != -1) {
        int *captured_arr =
            (turn == PLAYER1) ? state->captured1 : state->captured2;
        captured_arr[captured_index]++;
    }
}

// --- Main Game Loop ---

int main(void) {
    GameState state;
    init_board(&state);

    int lion1_exists = 1, lion2_exists = 1;
    int lion1_goal = 0, lion2_goal = 0;

    while (1) {
        print_board(&state);
        printf(
            "Player %d's turn. Enter move (from_row from_col to_row to_col)\n"
            "  (Use -1 -1 to_row to_col to drop a captured piece): ",
            state.turn);

        int fr, fc, tr, tc;
        if (scanf("%d %d %d %d", &fr, &fc, &tr, &tc) != 4) break;

        if (fr == -1 && fc == -1) {
            int piece_type;
            printf(
                "Which piece to drop? (0:ELEPHANT 1:GIRAFFE 2:CHICK 3:HEN): ");
            if (scanf("%d", &piece_type) != 1 ||
                !drop_piece(&state, state.turn, piece_type, tr, tc)) {
                printf("Failed to drop piece.\n");
                continue;
            }
        } else {
            if (!is_valid_pos(fr, fc) || !is_valid_pos(tr, tc)) {
                printf("Invalid position.\n");
                continue;
            }
            Piece p = state.board[fr][fc];
            if (!can_move(&state, p, fr, fc, tr, tc, state.turn)) {
                printf("Invalid move.\n");
                continue;
            }
            Piece captured = state.board[tr][tc];
            state.board[fr][fc] = EMPTY;

            if (captured == LION1) lion1_exists = 0;
            if (captured == LION2) lion2_exists = 0;

            // Demote HEN to CHICK when captured
            if (captured == HEN1) captured = CHICK2;
            if (captured == HEN2) captured = CHICK1;
            add_captured_piece(&state, state.turn, captured);

            state.board[tr][tc] = p;

            // Promotion
            if (p == CHICK1 && tr == 0) state.board[tr][tc] = HEN1;
            if (p == CHICK2 && tr == BOARD_ROWS - 1) state.board[tr][tc] = HEN2;

            // Lion goal check
            if (p == LION1 && tr == 0) lion1_goal = 1;
            if (p == LION2 && tr == BOARD_ROWS - 1) lion2_goal = 1;
        }

        // Victory check
        if ((state.turn == PLAYER2 && lion1_goal) ||
            (state.turn == PLAYER1 && lion2_goal)) {
            printf(
                "Player %d wins! (Lion reached opponent's territory and "
                "survived one turn)\n",
                3 - state.turn);
            break;
        }
        if (!lion1_exists) {
            printf("Player 2 wins! (Lion1 captured)\n");
            break;
        }
        if (!lion2_exists) {
            printf("Player 1 wins! (Lion2 captured)\n");
            break;
        }

        state.turn = 3 - state.turn;
    }
    return 0;
}

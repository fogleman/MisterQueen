#include "2PlayerNoFeatures.h"


int main(int argc, char **argv) {
    print_commands();

    //Initialize game variable
    Board board;
    int currentPlayer = 1;
    

    //Initiallize game
    board_reset(&board);
    board_print(&board);
    
    while(1){
        int currentRoundEnd = play_round(currentPlayer, &board);
        if(currentRoundEnd == 1){
            return 0;
        }else{
            currentPlayer = currentPlayer == 1? 2: 1;
        }
    }    
    return 0;
}

int play_round(int currentPlayer, Board *board)
{
    string s = malloc(sizeof(char)*5);
    while(1){
        printf("Player %i: ", currentPlayer);
        scanf("%s", s);
        lower(&s);
        int playerChoiceResult = handle_player_choice(board, s);
        if(playerChoiceResult == 0 || playerChoiceResult == 1) {return playerChoiceResult;}
    }
    free(s);
}

int handle_player_choice(Board *board, string s){
    if(strcmp(s, "quit") == 0){
        return 1;
    } else if(strcmp(s, "show") == 0){
        board_print(board);
        return -1;
    } else if (strcmp(s, "help" == 0)){
        print_commands();
        return -1;
    } else{
        process_move(board, s);
        return 0;
    }
}

void process_move(Board *board, string s){
    const string moveString = s;
    Move move;
    printf("MOVE:%s\n", s);
    move_from_string(&move, moveString);
    make_move(board, &move);
    board_print(board);
}

void lower(char **s){
    char *temp = *s;
    int slen = strlen(*s);
    char tempHolder[slen];
    for(int i = 0; i < slen; i++){
        tempHolder[i] = tolower(temp[i]);
    }
    *s = tempHolder;
}

void print_commands(void){
    printf(
    "-Type help to get this list of commands.\n"
    "-To make a move, type your move in long algebraic notation.\n"
    "-To See the board in it\'s current state, type show. \n"
    "-To quit the program, type quit\n"
    );
}
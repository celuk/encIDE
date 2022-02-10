#include <stdio.h>

char tiles[6][7] = {
    {'-', '-', '-', '-', '-', '-', '-'},
    {'-', '-', '-', '-', '-', '-', '-'},
    {'-', '-', '-', '-', '-', '-', '-'},
    {'-', '-', '-', '-', '-', '-', '-'},
    {'-', '-', '-', '-', '-', '-', '-'},
    {'-', '-', '-', '-', '-', '-', '-'}
};

void board(){
    int i = 0;
    int j = 0;
    
    printf("\n");
    printf("X 0 1 2 3 4 5 6 X");
    printf("\n");
    
    for(i=1; i<7; i++){
        for(j=0; j<9; j++){
            if(j == 0 || j == 8)
                printf("%d ", i - 1);
            else
                printf("%c ", tiles[i - 1][j - 1]);
        }
        printf("\n");
    }
    
    printf("X 0 1 2 3 4 5 6 X");
    printf("\n\n");
}

int checkBoard(char player){
    int i = 0;
    int j = 0;
    int k = 0;
    
    int repCount;
    int hold;
    
    // row by row
    for (i=0; i<6; i++){
        repCount = 0;
        hold = 0;
        
        for (j=0; j<7; j++){
            if(tiles[i][j] == player){
                if(j > 0)
                    if(tiles[i][j - 1] == player)
                        hold = 1;
                
                if(hold)
                    repCount++;
                else
                    repCount = 1;
            }
            else{
                hold = 0;
            }
            
            if (repCount == 4) return 1;
        }
    }
    
    // column by column
    for (j=0; j<7; j++){
        repCount = 0;
        hold = 0;
        
        for (i=0; i<6; i++){
            if(tiles[i][j] == player){
                if(i > 0)
                    if(tiles[i - 1][j] == player)
                        hold = 1;
                
                if(hold)
                    repCount++;
                else
                    repCount = 1;
            }
            else{
                hold = 0;
            }
            
            if (repCount == 4) return 1;
        }
    }
    
    int ii = 0;
    int jj = 0;
    
    for (i = 1; i < 5; i++){
        for (j = 1; j < 6; j++){
            
            /* left-tilted diagonals */
            
            repCount = 0;
            
            // left-upwards:
            for (ii = i, jj = j; (ii >= 0) || (jj >= 0); ii--, jj--){
                if (tiles[ii][jj] == player){
                    repCount++;
                    
                    if (repCount == 4) return 1;
                }
                else
                    break;
            }
            
            // right-downwards:
            for (ii = i+1, jj = j+1; (ii <= 5) || (jj <= 6); ii++, jj++){
                if (tiles[ii][jj] == player){
                    repCount++;
                    
                    if (repCount == 4) return 1;
                }
                else
                    break;
            }
            
            /* right-tilted diagonals */
            
            repCount = 0;
            
            // left-downwards:
            for (ii = i, jj = j; (ii <= 5) || (jj >= 0); ii++, jj--){
                if (tiles[ii][jj] == player){
                    repCount++;
                    
                    if (repCount == 4) return 1;
                }
                else
                    break;
            }
            
            // right-upwards:
            for (ii = i-1, jj = j+1; (ii >= 0) || (jj <= 6); ii--, j++){
                if (tiles[ii][jj] == player){
                    repCount++;
                    
                    if (repCount == 4) return 1;
                }
                else
                    break;
            }
        }
    }
        
    
    int isAllFull = 1;
    
    for(i=1; i<7; i++){
        for(j=1; j<8; j++){
            if(tiles[i - 1][j - 1] == '-')
                isAllFull = 0;
        }
    }
    
    if(isAllFull) return 2;
        
    return 0;
}


int main(){
    
    printf("The game has started!\n");
    
    board();
    
    char player = 'R';
    
    int move;
    
    while(1){
        if(player == 'R')
            printf("Player 1’s Turn (R):\n");
        else
            printf("Player 2’s Turn (Y):\n");
        
        printf("Enter a move: ");
        scanf(" %d", &move);
        
        if(!(move>=0 && move<=6)){
            printf("Entered move is not valid!\n");
            board();
            continue;
        }
        
        int revCount = 5;
        
        while(tiles[revCount][move] != '-' && revCount >=0) revCount--;
        
        if(revCount >= 0)
            tiles[revCount][move] = player;
        else{
            printf("The column is full, try again!\n");
            board();
            continue;
        }
        
        board();
        
        if(checkBoard(player) == 1){
            if(player == 'R')
                printf("Player 1 has won!\n");
            else
                printf("Player 2 has won!\n");
            
            break;
        }
        
        if(checkBoard(player) == 2){
            printf("There is no move to play, game is over!\n");
            break;
        }
            
        
        player = (player == 'R') ? 'Y' : 'R';
    }
    
    return 0;
}



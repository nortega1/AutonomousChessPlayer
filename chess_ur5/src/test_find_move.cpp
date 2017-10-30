#include <iostream>
#include "find_move.h"

using namespace std;
using namespace find_move;

void move(Piece (&board)[8][8], int start_x, int start_y, int end_x, int end_y);
void print_board(Piece (&board)[8][8]);

int main(int argc, char ** argv) {
    cout << "Testing find_move namespace\n";
    
    Piece board[8][8];
    Piece old_board[8][8];

    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 16; ++j) {
            int tag = 16*i + j;
            Piece p(tag);
            
            int x,y;
            if (p.getType() == Piece::pawn) {
                x = p.getId();
                y = 1;
            }
            else {
                y = 0;
            
                if (p.getType() == Piece::king) {
                    x = 3;
                }
                else if (p.getType() == Piece::queen) {
                    x = 4;
                }
                else if (p.getType() == Piece::rook) {
                    x = 7*p.getId();
                }
                else if (p.getType() == Piece::knight) {
                    x = 5*p.getId() + 1;
                }
                else {
                    x = 3*p.getId() + 2;
                }
            }

            if (p.getColor() == Piece::black) {
                y = 7 - y;
            }
            board[x][y] = p;
        }
    }

    print_board(board);
    
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            old_board[i][j] = board[i][j];
        }
    }
    
    move(board, 3, 1, 3, 3);
    cout << getMove(old_board, board) << endl;
    
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            old_board[i][j] = board[i][j];
        }
    }
 
    move(board, 1, 7, 2, 5);
    cout << getMove(old_board, board) << endl;
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            old_board[i][j] = board[i][j];
        }
    }
 
    move(board, 2, 0, 7, 5);
    cout << getMove(old_board, board) << endl;   
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            old_board[i][j] = board[i][j];
        }
    }
 
    move(board, 7, 5, 2, 0);
    cout << getMove(old_board, board) << endl;   
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            old_board[i][j] = board[i][j];
        }
    }
 
    move(board, 1, 0, 2, 2);
    cout << getMove(old_board, board) << endl;   
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            old_board[i][j] = board[i][j];
        }
    }
 
    move(board, 2, 2, 1, 0);
    cout << getMove(old_board, board) << endl;   
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            old_board[i][j] = board[i][j];
        }
    }
 


}


void move(Piece (&board)[8][8], int start_x, int start_y, int end_x, int end_y) {
    
    board[end_x][end_y] = board[start_x][start_y];
    board[start_x][start_y] = Piece();

    print_board(board);
}

void print_board(Piece (&board)[8][8]) {
    cout << "--------------------\n";
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            cout << board[j][i];
        }
        cout << endl;
    }
}

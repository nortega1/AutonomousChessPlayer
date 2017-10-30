#include "find_move.h"


namespace find_move {

    std::string xy2chess(int x, int y) {
        char x_chess = 'a' + x;
        std::ostringstream ss;
        ss << x_chess << y+1;
        return ss.str();
    }

    std::string findPiece(Piece& p, Piece board[][8], int x_start, int y_start) {
        for (int x = x_start; x < 8; ++x) {
            for (int y = y_start; y < 8; ++y) {
                if (p == board[x][y]) {
                    return xy2chess(x,y);
                }
            }
        }

        return "XX";
    }


    std::string getMove(Piece old_board[][8], Piece curr_board[][8]) {

        std::string start,end;
        for (int x = 0; x < 8; ++x) {
            for (int y = 0; y < 8; ++y) {
                if (!(old_board[x][y] == curr_board[x][y])) {
                    // Piece moved *to* here
                    if (curr_board[x][y].getColor() == Piece::Human_Color) {
                        end = xy2chess(x,y);
                        start = findPiece(curr_board[x][y], old_board, x, y);
                    }
                    // Piece moved *from* here
                    else {
                        start = xy2chess(x,y);   
                        end = findPiece(old_board[x][y], curr_board, x, y);  
                    }
                    return start + end;
                }
            }
        }

        return "XXXX";
    }

}

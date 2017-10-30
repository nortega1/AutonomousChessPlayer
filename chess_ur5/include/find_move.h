#include <string>
#include <sstream>
#include "Piece.h"

namespace find_move {

    std::string xy2chess(int, int);

    std::string findPiece(Piece&, Piece[][8], int, int);

    std::string getMove(Piece[][8], Piece[][8]);

}

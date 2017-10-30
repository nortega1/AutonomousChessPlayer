#include "Piece.h"

Piece::Piece() {
    type = noType;
    color = noColor;
    id = -1;
}

Piece::Piece(int tag) {
    type = getTypeFromTag(tag);
    color = getColorFromTag(tag);
    id = getIdFromTag(tag);
}

/*
Piece::Piece(int t, int c, int i) {
    type = getType(t);
    color = (Color) (c + 1);
    id = i;
}
*/
Piece::Type Piece::getTypeFromTag(int tag) {
    int colorless = tag % 16;
    Type t;
    switch (colorless) {
        case 0:
            t = king;
            break;
        case 1:
            t = queen;
            break;
        case 2: case 3:
            t = rook;
            break;
        case 4: case 5:
            t = bishop;
            break;
        case 6: case 7:
            t = knight;
            break;
        default:
            t = pawn;
    }
    return t;
}

Piece::Color Piece::getColorFromTag(int tag) {
   return (Color) (tag / 16 + 1);
}

int Piece::getIdFromTag(int tag) {
    int colorless = tag % 16;
    int id;
    // Pawn case (id == 0-7)
    if (colorless >= 8) {
        id = colorless % 8;
    }
    // King or queen case (id == 0)
    else if (colorless <= 1) {
        id = 0;
    }
    // Rook, bishop, knight case (id == 0-1)
    else {
        id = colorless % 2;
    }

    return id;
}

char Piece::getSymbol() const {
    char c;
    switch (this->type) {
        case king:
            c = 'k';
            break;
        case queen:
            c = 'q';
            break;
        case rook:
            c = 'r';
            break;
        case bishop:
            c = 'b';
            break;
        case knight:
            c = 'n';
            break;
        case pawn:
            c = 'p';
            break;
        default:
            c = '_';
    }

    if (this->color == black) c = c + 'A' - 'a'; 
    
    return c;
}   

bool Piece::invalid() {
    return this->type == noType && this->color == noColor;
}

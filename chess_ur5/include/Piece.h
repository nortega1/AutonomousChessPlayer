#ifndef PIECE_H
#define PIECE_H
#include <iostream>

class Piece {
   public:
        enum Type {noType, king, queen, rook, bishop, knight, pawn };
        enum Color {noColor, white, black };

        Piece();
        Piece(int);
        //Piece(int, int, int);

        Type getType() { return type; }
        Color getColor() { return color; }
        int getId() { return id; }

        Type getTypeFromTag(int);
        Color getColorFromTag(int);
        int getIdFromTag(int);
        char getSymbol() const;

        static const Color Human_Color = white;

        bool invalid();

        /*
        std::ostream& operator<<(std::ostream& os) {
            os << getSymbol();
            return os;
        }
        */
        bool operator==(const Piece& that) const{
            return (this->type == that.type) && (this->color == that.color) && (this->id == that.id);
        }
    private:
        Type type;
        Color color;
        int id;                                               
 
};


std::ostream& operator<<(std::ostream& out, const Piece& p)
{
       return out << p.getSymbol();
}

#endif

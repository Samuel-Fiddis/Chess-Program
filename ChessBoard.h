#include <string>
#include <map>
#include <iostream>
#include <cmath>
#include <fstream>

using namespace std;

class ChessPiece{

private:
  bool white;
  bool first_Move;
  bool enPassant;

protected:
  string type;
  string image;

public:
  ChessPiece(bool w);
  bool is_White();
  bool is_First_Move();
  string get_Type() {return type;}
  string get_Image() {return image;}
  void set_Moved();
  void set_En_Passant(bool ep) {enPassant = ep;}
  bool get_En_Passant() {return enPassant;}

  // Gives the possible types of moves that a piece can make
  virtual bool possible_Move(string start, string end) = 0;
  void takePiece();

  virtual ~ChessPiece() { }
};

class Pawn : public ChessPiece{

public:
  Pawn(bool w) : ChessPiece(w){
    type = "Pawn";
    if(w) image = "\u265F";
    else image = "\u2659";
  };
  bool possible_Move(string start, string end);
  bool possible_Taking_Move(string a, string b);
  ~Pawn() {};
};

class Rook : public ChessPiece{

public:
  Rook(bool w) : ChessPiece(w) {
    type = "Rook";
    if(w) image = "\u265C";
    else image = "\u2656";
  };
  bool possible_Move(string a, string b);
  ~Rook() {};
};


class Bishop : public ChessPiece{

public:
  Bishop(bool w) : ChessPiece(w) {
    type = "Bishop";
    if(w) image = "\u265D";
    else image = "\u2657";
  };
  bool possible_Move(string a, string b);
  ~Bishop() {};
};


class Knight : public ChessPiece{

public:
  Knight(bool w) : ChessPiece(w) {
    type = "Knight";
    if(w) image = "\u265E";
    else image = "\u2658";
  };
  bool possible_Move(string a, string b);
  ~Knight() {};
};


class Queen : public ChessPiece{

public:
  Queen(bool w) : ChessPiece(w) {
    type = "Queen";
    if(w) image = "\u265B";
    else image = "\u2655";
  };
  bool possible_Move(string a, string b);
  ~Queen() {};
};


class King : public ChessPiece{

public:
  King(bool w) : ChessPiece(w) {
    type = "King";
    if(w) image = "\u265A";
    else image = "\u2654";
  };
  bool possible_Move(string a, string b);
  ~King() {};
};


class ChessBoard{

private:
  map<string,ChessPiece*> Board;
  bool whites_turn;
  bool enPassant;

public:
  ChessBoard();
  void initailiseBoard();
  void resetBoard();
  void submitMove(string start, string end);
  bool logicalMove(string start, string end);
  bool sameColour(string pos1, string pos2);
  bool inCheck(string start, string end);
  bool straightLineCheck(string pos, string king_pos);
  bool checkMate();
  bool staleMate();
  void makeMove(string start, string end);
  void clearBoard();
  void setupBoard();
  void printBoard();
  void printTurn();
  string blockingPiece(string start, string end);
  void pawnPromotion(string end);
  bool castling(string start, string end);

  void set_En_Passant(bool ep) {enPassant = ep;}
  bool get_En_Passant() {return enPassant;}
};

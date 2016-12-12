#include "ChessBoard.h"

ChessBoard::ChessBoard(){
  initailiseBoard();
}

void ChessBoard::initailiseBoard(){

  cout << "A new chess game is started!\n";

  whites_turn = true;
  enPassant = false;
  char pos[3];
  pos[2] = '\0';

  // Create all white pawns

  pos[1] = '2';
  for(char a = 'A'; a < 'I'; a++){
    pos[0] = a;
    Board[pos] = new Pawn(true);
  }

  // Create all black pawns

  pos[1] = '7';

  for(char a = 'A'; a < 'I'; a++){
    pos[0] = a;
    Board[pos] = new Pawn(false);
  }

  // Create all other peices

  Board["A1"] = new Rook(true);
  Board["B1"] = new Knight(true);
  Board["C1"] = new Bishop(true);
  Board["D1"] = new Queen(true);
  Board["E1"] = new King(true);
  Board["F1"] = new Bishop(true);
  Board["G1"] = new Knight(true);
  Board["H1"] = new Rook(true);

  Board["A8"] = new Rook(false);
  Board["B8"] = new Knight(false);
  Board["C8"] = new Bishop(false);
  Board["D8"] = new Queen(false);
  Board["E8"] = new King(false);
  Board["F8"] = new Bishop(false);
  Board["G8"] = new Knight(false);
  Board["H8"] = new Rook(false);

  // Fill Blanks

  for(char a = 'A'; a < 'I'; a++)
    for(char b = '3'; b < '7'; b++){
      pos[0] = a;
      pos[1] = b;
      Board[pos] = NULL;
    }

  printBoard();
}

void ChessBoard::resetBoard(){

  // Remove all pieces from the board

  string pos = "00";

  for(char a = 'A'; a <= 'H'; a++){
    pos[0] = a;
    for(char b = '1'; b <= '8'; b++){
      pos[1] = b;
      if(Board[pos] != NULL) delete Board[pos];
    }
  }

  initailiseBoard();
}

// This function prints a the board layout using unicode text

void ChessBoard::printBoard(){

  string pos = "00";

  cout << "    ";
  for(char b = 'A'; b < 'I'; b++) cout << b << "   ";
  cout << endl;

  for(char a = '8'; a > '0'; a--){
    cout << "   -------------------------------\n";
    cout << a << " |";
    for(char b = 'A'; b < 'I'; b++){
      pos[1] = a;
      pos[0] = b;
      if(Board[pos]!=NULL) cout << ' ' << Board[pos]->get_Image() << " |";
      else if(Board[pos] == NULL) cout << "   |" ;
    }
    cout << " " << a << endl;
  }
  cout << "   -------------------------------\n";
  cout << "    ";
  for(char b = 'A'; b < 'I'; b++) cout << b << "   ";
  cout << endl;
}

// Prints who's turn it currently is

void ChessBoard::printTurn(){
  if(whites_turn) cout << "White";
  else cout << "Black";
}

// Takes a move request and determines if it is possible

void ChessBoard::submitMove(string start, string end){

  // Check that the strings are formatted correctly

  if(start[0] > 'H' || start[0] < 'A' || start[1] > '8' || start[1] < '1'
    || end[0] > 'H' || end[0] < 'A' || end[1] > '8' || end[1] < '1'
    || start.length() != 2 || end.length() != 2){
      cout << "Move not formated correctly.\n";
      return;
  }

  // Check that there is no stalemate or checkmate

  if(inCheck(start,start) && checkMate()){
    printTurn();
    cout << " is in checkmate! Game over!\n";
    return;
  }
  else if(!inCheck(start,start) && checkMate()){
    cout << "The game is a stale mate! Game over!\n";
    return;
  }

  // Check that this is a possible move and make it if it is
  if(!castling(start,end)){
    if(Board[start]!=NULL){                             // Check if there is a peice in the start position
      if(Board[start]->is_White() == whites_turn){      // Check that it is the current peices turn
        if(logicalMove(start,end)){                     // Check if move is possible for this peice
          string block_pos = blockingPiece(start,end);  // Find piece blocking move if there is one
          // Check if there is a peice between the start and the end if the peice isn't a knight
          if((Board[start]->get_Type() != "Knight" && block_pos == "00") || Board[start]->get_Type() == "Knight"){
            // Check if there is a peice at the end position and if it is the opposite colour
            if(Board[end] == NULL || !sameColour(start,end)){
              // Check if the move puts the pieces in check
              if(!inCheck(start,end)){
                makeMove(start,end);
                pawnPromotion(end);
                whites_turn = !whites_turn;
                printBoard();
                // Check if the opponent is now in check, checkmate or stale mate
                if(inCheck(start,start)){
                  printTurn();
                  if(checkMate()) cout << " is in checkmate\n";
                  else cout << " is in check\n";
                }
                else if(checkMate()) cout << "The game is a stale mate\n";
              }
              else { cout << "This move puts "; printTurn(); cout << " in check\n";}
            }
            else cout << "Can't take a piece of your own colour\n";
          }
          else{
            printTurn();
            cout << "'s " << Board[start]->get_Type() << " cannot move to " << end << "!\n";
            if(Board[block_pos]->is_White()) cout << "White's ";
            else cout << "Black's ";
            cout << Board[block_pos]->get_Type() << " in position " << block_pos <<" blocking move!\n";
          }
        }
        else cout << "Can't move " << Board[start]->get_Type() << "s that way\n";
      }
      else if(Board[start]->is_White() != whites_turn){
        cout << "It's not ";
        if(whites_turn) cout << "Black";
        else cout << "White";
        cout << "'s turn to move!\n";
      }
    }
    else if(Board[start]==NULL) cout << "There is no piece at position " << start << "!\n";
  }
}
// Checks if the move given corresponds to the pieces rules for movement

bool ChessBoard::logicalMove(string start, string end){

  if(Board[start]->get_Type() != "Pawn"){
    return Board[start]->possible_Move(start,end);
  }

  else if(Board[start]->get_Type() == "Pawn"){
    Pawn *p = (Pawn *) Board[start];

    string pos = "00";
    pos[1] = start [1];
    pos[0] = end[0];

    if(p->possible_Move(start,end) && Board[end] == NULL) return true;
    if(p->possible_Taking_Move(start,end) && Board[end] != NULL) return true;
    if(p->possible_Taking_Move(start,end) && Board[pos] != NULL && Board[pos]->get_Type() == "Pawn" && Board[pos]->get_En_Passant() && get_En_Passant()) return true;
  }

  return false;
}

// Makes a move on the board from the start position to the end position and makes necessary changes for En Passant, Promotion etc

void ChessBoard::makeMove(string start, string end){

  // enPassant test

  if(Board[start]->is_First_Move() && Board[start]->get_Type() == "Pawn" && abs(int (start[1] - end[1])) == 2){
    Board[start]->set_En_Passant(true);
  }

  else Board[start]->set_En_Passant(false);

  // Get en_Passant position

  string pos = "00";
  pos[1] = start [1];
  pos[0] = end[0];
  Pawn *p = (Pawn *) Board[start];

  if(Board[start]->get_Type() == "Pawn" && p->possible_Taking_Move(start,end) && Board[pos] != NULL &&
     Board[pos]->get_En_Passant() && get_En_Passant() && Board[end] == NULL && Board[pos]->get_Type() == "Pawn"){
      Board[start]->set_Moved();
      if(whites_turn) cout << "White's ";
      else cout << "Black's ";
      cout << Board[start]->get_Type() << " moves from " << start << " to " << end;
      Board[pos]->takePiece();
      delete Board[pos];
      Board[pos] = NULL;
      Board[end] = Board[start];
      Board[start] = NULL;
      cout << endl;
    }


else{
  Board[start]->set_Moved();
  if(whites_turn) cout << "White's ";
  else cout << "Black's ";
  cout << Board[start]->get_Type() << " moves from " << start << " to " << end;
  if(Board[end] != NULL){
    Board[end]->takePiece();
    delete Board[end];
  }
  Board[end] = Board[start];
  Board[start] = NULL;
  cout << endl;
}

  set_En_Passant(false);

  if(Board[end]->get_Type() == "Pawn" && abs(int (start[1] - end[1])) == 2) set_En_Passant(true);

  // if(Board[end]->get_En_Passant()) cout << "Pawn in position " << end << " is set up for en passant!\n";
}

 // This function determines if there is a piece between the start and end positions
 // if there is it returns the position in a string of the form "A1", if not it returns
 // the string "00".

string ChessBoard::blockingPiece(string start, string end){

  string pos = "00";

  // Test horizontal and vertical lines for any blocking piece

  if(start[0] == end[0]){
    pos[0] = start[0];
    if(start[1] < end[1]){
      for(char a = start[1] + 1; a < end[1]; a++){
        pos[1] = a;
        if(Board[pos] != NULL) return pos;
      }
    }
    if(start[1] > end[1]){
      for(char a = start[1] - 1; a > end[1]; a--){
        pos[1] = a;
        if(Board[pos] != NULL) return pos;
      }
    }
  }

  if(start[1] == end[1]){
    pos[1] = start[1];
    if(start[0] < end[0]){
      for(char a = start[0] + 1; a < end[0]; a++){
        pos[0] = a;
        if(Board[pos] != NULL) return pos;
      }
    }
    if(start[0] > end[0]){
      for(char a = start[0] - 1; a > end[0]; a--){
        pos[0] = a;
        if(Board[pos] != NULL) return pos;
      }
    }
  }

  // Check diagonal lines for any blocking piece

  if(abs(int (start[0] - end[0])) == abs(int (start[1] - end[1]))){

    if(start[0] < end[0] && start[1] < end[1]){
      pos[0] = start[0] + 1;
      pos[1] = start[1] + 1;
      while(pos[0] < end[0]){
        if(Board[pos] != NULL) return pos;
        pos[1]++; pos[0]++;
      }
    }

    if(start[0] < end[0] && start[1] > end[1]){
      pos[0] = start[0] + 1;
      pos[1] = start[1] - 1;
      while(pos[0] < end[0]){
        if(Board[pos] != NULL) return pos;
        pos[1]--; pos[0]++;
      }
    }

    if(start[0] > end[0] && start[1] > end[1]){
      pos[0] = start[0] - 1;
      pos[1] = start[1] - 1;
      while(pos[0] > end[0]){
        if(Board[pos] != NULL) return pos;
        pos[1]--; pos[0]--;
      }
    }

    if(start[0] > end[0] && start[1] < end[1]){
      pos[0] = start[0] - 1;
      pos[1] = start[1] + 1;
      while(pos[0] > end[0]){
        if(Board[pos] != NULL) return pos;
        pos[1]++; pos[0]--;
      }
    }
  }

  // Return "00" if nothing was found

  return "00";
}

// This function causes castling if requested

bool ChessBoard::castling(string start, string end){
  if(Board[start] != NULL && Board[end] != NULL  // Check pieces aren't empty
    && (Board[start]->get_Type() == "King" && Board[end]->get_Type() == "Rook") // Check that pieces are either a King or a Rook
    && Board[start]->is_White() == Board[end]->is_White() // Check King and Rook are of the same colour
    && end[1] == start[1] // Check that the pieces are in the same row
    && Board[start]->is_White() == whites_turn){  // Check that it is the current players turn
    if(!inCheck(start,start)){
    string block_pos = blockingPiece(start,end);
    if(block_pos == "00"){  // Check if there is a piece blocking the move
      if(Board[start]->is_First_Move()){
        if(Board[end]->is_First_Move()){
          string pos1 = "00";
          string pos2 = "00";
          pos1[1] = start[1];
          pos2[1] = start[1];
          if(end[0] < start[0]){
            pos1[0] = start[0] - 1;
            pos2[0] = start[0] - 2;
          }
          else if(end[0] > start[0]){
            pos1[0] = start[0] + 1;
            pos2[0] = start[0] + 2;
          }
          if(!inCheck(start,pos2)){ // Check if the move puts the king in check
            if(!inCheck(start,pos1)){
              Board[start]->set_Moved();
              Board[end]->set_Moved();
              Board[pos1] = Board[end];
              Board[pos2] = Board[start];
              Board[end] = NULL;
              Board[start] = NULL;

              if(whites_turn) cout << "White's ";
              else cout << "Black's ";
              cout << "King has been castled.\n";
              whites_turn = !whites_turn;
              printBoard();
              return true;
            }
            else{
              cout << "Can't castle the King through check.\n";
              return true;
            }
          }
          else {
            cout << "This castling move puts the King in check.\n";
            return true;
          }
        }
        else {
          cout << "It's not the " << Board[end]->get_Type() << "'s first move, can't castle.\n";
          return true;
        }
      }
      else {
        cout << "It's not the " << Board[start]->get_Type() << "'s first move, can't castle.\n";
        return true;
      }
    }
    else {
      cout << Board[block_pos]->get_Type() << " in position " << block_pos <<" blocking move!\n";
      return true;
    }
  }
  else{
    cout << "King is in check, can't castle.\n";
    return true;
  }
}
  return false;
}

// This function checks if a move places the player in check

bool ChessBoard::inCheck(string start, string end){

  // Set up variables

  bool ret_val = false;
  string block_pos = "00";
  string king_pos = "00";
  string pos = "00";

  // Switch out pieces

  ChessPiece *temp = Board[end];
  Board[end] = Board[start];
  Board[start] = NULL;

  // Determine position of current turns king

  for(char a = 'A'; a < 'I'; a++){
    pos[0] = a;
    for(char b = '1'; b < '9'; b++){
      pos[1] = b;
      if(Board[pos] != NULL && Board[pos]->get_Type() == "King" && Board[pos]->is_White() == whites_turn)
      king_pos = pos;
    }
  }

  // Check horizontal and vertical lines around king

  pos = king_pos;
  pos[0] = 'A';
  if(straightLineCheck(pos,king_pos)) ret_val = true;

  pos[0] = 'H';
  if(straightLineCheck(pos,king_pos)) ret_val = true;

  pos = king_pos;
  pos[1] = '1';
  if(straightLineCheck(pos,king_pos)) ret_val = true;

  pos[1] = '8';
  if(straightLineCheck(pos,king_pos)) ret_val = true;

  // Check diagonal lines around the king

  pos = king_pos;
  while(pos[0] >= 'A' && pos[1] >= '1'){ pos[0]--;pos[1]--; }
  if(straightLineCheck(pos,king_pos)) ret_val = true;

  pos = king_pos;
  while(pos[0] >= 'A' && pos[1] <= '8'){ pos[0]--;pos[1]++; }
  if(straightLineCheck(pos,king_pos)) ret_val = true;

  pos = king_pos;
  while(pos[0] <= 'H' && pos[1] >= '1'){ pos[0]++;pos[1]--; }
  if(straightLineCheck(pos,king_pos)) ret_val = true;

  pos = king_pos;
  while(pos[0] <= 'H' && pos[1] <= '8'){ pos[0]++;pos[1]++; }
  if(straightLineCheck(pos,king_pos)) ret_val = true;

  // Check knights possible positions

  pos = king_pos;
  pos[1] = pos[1] + 2;
  pos[0] = pos[0] + 1;
  if(Board[pos] != NULL && Board[pos]->get_Type() == "Knight" && !sameColour(pos,king_pos)) ret_val = true;

  pos = king_pos;
  pos[1] = pos[1] - 2;
  pos[0] = pos[0] + 1;
  if(Board[pos] != NULL && Board[pos]->get_Type() == "Knight" && !sameColour(pos,king_pos)) ret_val = true;

  pos = king_pos;
  pos[1] = pos[1] + 2;
  pos[0] = pos[0] - 1;
  if(Board[pos] != NULL && Board[pos]->get_Type() == "Knight" && !sameColour(pos,king_pos)) ret_val = true;

  pos = king_pos;
  pos[1] = pos[1] - 2;
  pos[0] = pos[0] - 1;
  if(Board[pos] != NULL && Board[pos]->get_Type() == "Knight" && !sameColour(pos,king_pos)) ret_val = true;

  pos = king_pos;
  pos[1] = pos[1] + 1;
  pos[0] = pos[0] + 2;
  if(Board[pos] != NULL && Board[pos]->get_Type() == "Knight" && !sameColour(pos,king_pos)) ret_val = true;

  pos = king_pos;
  pos[1] = pos[1] - 1;
  pos[0] = pos[0] + 2;
  if(Board[pos] != NULL && Board[pos]->get_Type() == "Knight" && !sameColour(pos,king_pos)) ret_val = true;

  pos = king_pos;
  pos[1] = pos[1] + 1;
  pos[0] = pos[0] - 2;
  if(Board[pos] != NULL && Board[pos]->get_Type() == "Knight" && !sameColour(pos,king_pos)) ret_val = true;

  pos = king_pos;
  pos[1] = pos[1] - 1;
  pos[0] = pos[0] - 2;
  if(Board[pos] != NULL && Board[pos]->get_Type() == "Knight" && !sameColour(pos,king_pos)) ret_val = true;

  // Switch back positions

  Board[start] = Board[end];
  Board[end] = temp;
  temp = NULL;

  return ret_val;
}

// This function determines if there is any possible move that can be made which
// doesn't result in check

bool ChessBoard::checkMate(){

  string pos1 = "00";
  string pos2 = "00";

  for(char a = 'A'; a <= 'H'; a++){
    pos1[0] = a;
    for(char b = '1'; b <= '8'; b++){
      pos1[1] = b;
      for(char c = 'A'; c <= 'H'; c++){
        pos2[0] = c;
        for(char d = '1'; d <= '8'; d++){
          pos2[1] = d;
          if(pos1 != pos2 && Board[pos1] != NULL && Board[pos1]->is_White() == whites_turn && logicalMove(pos1,pos2)) // Check if move is possible for this peice
          {
            string block_pos = blockingPiece(pos1,pos2);
            if((Board[pos1]->get_Type() != "Knight" && block_pos == "00") || Board[pos1]->get_Type() == "Knight") // Check if there is a peice between the start and the end if the peice isn't a knight
              if(Board[pos2] == NULL || !sameColour(pos1,pos2))
                if(!inCheck(pos1,pos2)) return false;
          }
        }
      }
    }
  }
  return true;
}

// Checks if there is a piece which can take a king from any straight line around
// its position king_pos

bool ChessBoard::straightLineCheck(string pos, string king_pos){
  string block_pos = blockingPiece(king_pos,pos);

  if(block_pos != "00"){
    if(logicalMove(block_pos,king_pos) && !sameColour(block_pos,king_pos)) return true;
  };

  return false;
}

// Checks if two pieces are the same colour

bool ChessBoard::sameColour(string pos1, string pos2){
  if(Board[pos1]->is_White() == Board[pos2]->is_White()) return true;
  else return false;
}

// Promotes pawns if they reach the last rank

void ChessBoard::pawnPromotion(string end){
  if(Board[end]->get_Type() == "Pawn"){
    if((Board[end]->is_White() && end[1] == '8') || (!Board[end]->is_White() && end[1] == '1')){
      string p = "0";
      bool colour = Board[end]->is_White();
      cout << "Pawn promotion! Which piece you would like to promote to? \n"
      << "Queen (q), Knight (k), Rook (r), Bishop (b): ";
      cin >> p;
      while(p.length() != 1 && (p[0] != 'q' || p[0] != 'k' || p[0] != 'r' || p[0] != 'b')){
        cout << "\nThis is not a valid choice, please try again: ";
        cin >> p;
      }
      switch (p[0]) {
        case 'q':
          delete Board[end];
          Board[end] = new Queen(colour);
          break;
        case 'k':
          delete Board[end];
          Board[end] = new Knight(colour);
          break;
        case 'r':
          delete Board[end];
          Board[end] = new Rook(colour);
          break;
        case 'b':
          delete Board[end];
          Board[end] = new Bishop(colour);
          break;
      }
    }
  }
}

ChessPiece::ChessPiece(bool w) : white(w) {
  first_Move = true;
  enPassant = false;
}

bool ChessPiece::is_White(){
  return white;
}

bool ChessPiece::is_First_Move(){
  return first_Move;
}

void ChessPiece::set_Moved(){
  first_Move = false;
}

void ChessPiece::takePiece(){
  cout << " taking ";
  if(is_White()) cout << "White's ";
  else cout << "Black's ";
  cout << type;
}

bool Pawn::possible_Move(string start, string end){
  if(is_First_Move()){
    if(is_White() && start[1] == '2' && (end[1] == '4' || end[1] == '3') && start[0] == end[0]){
        return true;
      }
    else if(!is_White() && start[1] == '7' && (end[1] == '5' || end[1] == '6') && start[0] == end[0]){
        return true;
      }
    }
  else if(!is_First_Move()){
    if(is_White() && int (start[1] - end[1]) == -1 && start[0] == end[0]) return true;
    else if(!is_White() && int (start[1] - end[1]) == 1 && start[0] == end[0]) return true;
  }
  return false;
}

bool Pawn::possible_Taking_Move(string s, string e){
  if(is_White() && int (s[1] - e[1]) == -1 && (int (s[0] - e[0]) == 1 || int (s[0] - e[0]) == -1)){
    return true;
  }
  if(!is_White() && int (s[1] - e[1]) == 1 && (int (s[0] - e[0]) == 1 || int (s[0] - e[0]) == -1)){
    return true;
  }
  return false;
}

bool Rook::possible_Move(string s, string e) {
  if(s[1] == e[1] || s[0] == e[0]) return true;
  else return false;
}

bool Bishop::possible_Move(string s, string e) {
  if(abs(int (s[0] - e[0])) == abs(int (s[1] - e[1]))) return true;
  else return false;
}

bool Queen::possible_Move(string s, string e) {
  if(abs(int (s[0] - e[0])) == abs(int (s[1] - e[1])) || (s[1] == e[1] || s[0] == e[0])) return true;
  else return false;
}

bool Knight::possible_Move(string s, string e){
  if((abs(s[0] - e[0]) == 2 && abs(s[1] - e[1]) == 1) || (abs(s[0] - e[0]) == 1 && abs(s[1] - e[1]) == 2)) return true;
  else return false;
}

bool King::possible_Move(string s, string e){
  if((abs(s[0] - e[0]) == 1 && abs(s[1] - e[1]) == 1) || (abs(s[0] - e[0]) == 1 && abs(s[1] - e[1]) == 0)
  || (abs(s[0] - e[0]) == 0 && abs(s[1] - e[1]) == 1)) return true;
  else return false;
}

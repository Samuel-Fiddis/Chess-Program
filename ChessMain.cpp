#include "ChessBoard.h"

int main() {

  ChessBoard cb;
  string start, end;

while(true){

  cout << "Please enter a move of the form A1 B2, give the location of the King, then the Rook to castle: ";
  cin >> start >> end;
  cout << endl;

  cb.submitMove(start, end);
}

    return 0;
}

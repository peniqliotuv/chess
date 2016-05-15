#include <iostream>
#include "board.h"
#include "enums.h"
#include "undo.h"
#include "init.cpp"

using namespace std;


int main(){
  initialize();

  for (int i = 0; i< BOARD_SIZE; i++){
    if (i%10 == 0){
      cout << endl;
    }
    printf("%5d", SQ120[i]);
  }
    cout << endl << endl;
  for (int i = 0; i < 64; i++){
    if (i%8 == 0){
      cout << endl;
    }
    printf("%5d", SQ64[i]);
  }



  return 0;
}

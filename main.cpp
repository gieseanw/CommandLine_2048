/*
* main.cpp
* Written by Andy Giese
* Allow a user to play a game of 2048
* http://en.wikipedia.org/wiki/2048_(video_game)
* via pressing the WASD keys (X for eXit)
*/

#include "Board.h"
#include <iostream>
#include <string>
#include <cctype>
using namespace std;

void ClearScreen(std::ostream& _fout = std::cout);
int main(){
  string input = "";
  char choice = 'Y';
  while(toupper(choice) == 'Y'){
    Board b(4);
    while(!b.IsGameOver()){
      ClearScreen();
      b.Display(cout);
      cout << "Press W A S or D to shift values Up, Left, Down, or Right" << endl;
      cout << "Choice: ";
      getline(cin,input);
      choice = input[0];
      while(!b.Update(choice)){
        cout << "Invalid Choice: Choices are W, A, S, or D (X for eXit)\n";
        cout << "Choice: ";
        getline(cin,input);
        choice = input[0];
      }
    }
    ClearScreen();
    b.Display(cout);
    cout << "Game over!" << endl;
    cout << "\n\n Your score: " << b.Score() << "\n\n";
    cout << "Play again (Y/N)? ";
    getline(cin,input);
    choice=input[0];
    while(!isalpha(choice) || (toupper(choice) != 'Y' && toupper(choice) != 'N')){
      cout << "Invalid Choice: Enter a Y or N.\n";
      cout << "Play again (Y/N)? ";
      getline(cin,input);
      choice=input[0];
    }
  }
  return 0;
}

void ClearScreen(std::ostream& _fout){
  _fout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
}


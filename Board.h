/*
* Board.h
* Written by Andy Giese
* Headers for game logic behind the game 2048
* http://en.wikipedia.org/wiki/2048_(video_game)
* The Board class has 5 functions, a constructor, a Display, an Update, a GameOver checker, a Score getter
* See their individual comments for more details
*/

#pragma once

#include <vector>
#include <set>
#include <iostream>
#include "Value.h"


class Board{
  public:
    //Default Constructor
    //instantiates a square grid with Values of 0, and all merged flags of false
    Board(int _size=4); 

    //Display
    //prints ASCII board to _fout
    std::ostream& Display(std::ostream& _fout); 
    
    //Update
    //Updates board state based on direction ('W', 'A', 'S', 'D')
    //Returns true if a valid direction was given:
    //  W: Shift Up
    //  A: Shift Left
    //  S: Shift Down
    //  D: Shift Right
    //  X: Quit Game
    //Returns false otherwise
    bool Update(char _direction);

    //IsGameOver
    //Returns true if no more moves are possible
    //  and false otherwise
    bool IsGameOver();

    //Score
    //Returns the current score
    //The score is incremented on a merge operation by the
    //  merged value
    size_t Score();

  private:
    //Copy Constructor
    //Used privately only for Game Over checking
    Board(const Board& _right);

    //GetEmptyIndex
    //Returns a pair indicating the row, colum tuple
    //  of an empty index in the board
    //If the board is completely full, returns (-1, -1)
    std::pair<int,int> GetEmptyIndex();

    //IsEmpty
    //Returns true if the board at (_row, _col) has a value of 0
    //Returns false otherwise
    bool IsEmpty(int _row, int _col);

    //CanMerge
    //Returns true if the Value at (_fromRow, _fromCol) can be safely
    //  merged into the Value at (_toRow, _toCol)
    //If so, returns true, Otherwise returns false
    //If returns true, the board at (_toRow, _toCol) will have its
    //  merged flag set
    bool CanMerge(int _fromRow, int _fromCol, int _toRow, int _toCol);

    //Move
    //Moves the Value at (_fromRow, _fromCol) into position (_toRow, _toCol)
    // the Value at (_fromRow, _fromCol) will afterwards be 0 with no merged flag set
    void Move(int _fromRow, int _fromCol, int _toRow, int _toCol);

    //Merge
    //Performs a merge operation on the Value at (_fromRow, _fromCol)
    //  with the Value at (_toRow, _toCol) and places the result at
    //  (_toRow, _toCol)
    //Afterwards, the Value at (_fromRow, _fromCol) will be 0 with no merged flag set
    void Merge(int _fromRow, int _fromCol, int _toRow, int _toCol);

    //AddEmptyIndex
    //Identifies (_row, _col) as a candidate for random Value placement
    void AddEmptyIndex(int _row, int _col);

    //RemoveEmptyIndex
    //Un-Identifies (_row, _col) as a candidate for random Value placement
    void RemoveEmptyIndex(int _row, int _col);
    
    //PlaceRandom
    //Updates the board at a viable empty index with a "Random" value
    //The random value shall be 2 with 90% probability, and 4 otherwise
    bool PlaceRandom();
    
    //NextRandom
    //Returns a random value of 2 with 90% probability, and 4 otherwise
    size_t NextRandom();

    //UpdateDirection
    //Generic board update function
    //(_row, _col) is a tuple of the current index
    // _dir is a tuple of (0,-1), (0,1), (-1,0), (1, 0) that represents
    //  a direction in the matrix to check for potential Move or Merge 
    //  operation from (_row, _col)
    // (0,-1) is a check for an Upwards Move/Merge, (1,0) is a Right Move/Merge
    //Returns true if a move or merge happens, false otherwise
    bool UpdateDirection(int _row, int _col, const std::pair<int,int>& _dir);
    
    //UpdateHorizontal
    //Horizontal Update function for Left or Right Update
    //_direction should use the #define LEFT or RIGHT  only
    //  it is an integer that indicates whether we check from left->right (1)
    //  or from right->left(-1) (Note how this is slightly different than the definition
    //  in the UpdateDirection() function
    //Returns true if any Move or Merge occurs during the update
    bool UpdateHorizontal(int _direction);
    
    //UpdateVertical
    //Vertical Update function for Up or Down Update
    //_direction should use the #define UP or DOWN only
    //  it is an integer that indicates whether we check from left->bottom(1)
    //  or from bottom->top(-1) (Note how this is slightly different than the definition
    //  in the UpdateDirection() function
    //Returns true if any Move or Merge occurs during the update
    bool UpdateVertical(int _direction);
    
    //ClearMerged
    //Resets the merged flags of all Values in the board to false
    void ClearMerged();

    //member variables
    int m_size;
    std::vector<std::vector<Value> > m_board;
    size_t m_score;
    std::set<int> m_emptyIndices;
};//end class Board

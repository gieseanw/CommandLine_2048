/*
* Board.cpp
* Written by Andy Giese
* Implements the Board ADT defined in Board.h
*/

#include "Board.h" 
#include <algorithm>
#include <iomanip>
#include <ctime>
#include <cctype>
using namespace std;

#define LEFT 1
#define RIGHT -1
#define UP 1
#define DOWN -1

//overloaded output operator for class Value
std::ostream& operator<<(std::ostream& _fout, const Value& _rhs){
  _fout << _rhs.m_val;
  return _fout;
}

Board::Board(int _size):m_size(_size),m_board(_size,vector<Value>(_size,Value())), m_score(0){
  if (_size < 2){
    cerr << "Size too small! Choose at least a 2x2 board" << endl;
    exit(1);
  }

  srand(time(0)); //random number seeding
  for(int i=0;i<m_size*m_size;++i)
    m_emptyIndices.insert(i);

  //add 2 random values on the board (either a 2 or a 4)
  PlaceRandomVal();
  PlaceRandomVal();
}

//prints ASCII board to _fout
std::ostream& Board::Display(std::ostream& _fout){
  _fout << "Score: " << m_score << endl;
  for(int i=0;i<m_size;++i){
    _fout << "_______";
  }
  _fout << "_\n\n";

  for(auto vIter = m_board.begin(); vIter != m_board.end(); ++vIter){
    for(auto cellIter = vIter->begin(); cellIter != vIter->end(); ++cellIter){
      _fout << "|";
      *cellIter == 0 ? _fout << setw(6) << " ": _fout << setw(6) << *cellIter;
    }
    _fout << "|" << endl;
    for(int i=0;i<m_size;++i)
      _fout << "_______";
    _fout << "_\n\n";
  }
  return _fout;
}

//Updates board state based on direction ('W', 'A', 'S', 'D')
bool Board::Update(char _direction){
  switch (toupper(_direction)){
    case 'A':
      //cout << "Left" << endl;
      if(UpdateHorizontal(LEFT))
        PlaceRandomVal();
      break;
    case 'D':
      //cout << "Right" << endl;
      if(UpdateHorizontal(RIGHT))
        PlaceRandomVal();
      break;
    case 'W':
      //cout << "Up" << endl;
      if(UpdateVertical(UP))
        PlaceRandomVal();
      break;
    case 'S':
      //cout << "Down" << endl;
      if(UpdateVertical(DOWN))
        PlaceRandomVal();
      break;
    case 'X':
      cerr << "Exiting" << endl;
      exit(1);
      break;
    default:
      cerr << "Err. Invalid direction" << endl;
      return false;
  }
  ClearMergedFlags();
  return true;
}

Board::Board(const Board& _right){
  m_board = _right.m_board;
  m_emptyIndices = _right.m_emptyIndices;
  m_size = _right.m_size;
}

//returns true if game over
bool Board::IsGameOver(){
  Board b2(*this);
  if(b2.UpdateHorizontal(LEFT) ||
    b2.UpdateHorizontal(RIGHT) ||
    b2.UpdateVertical(UP)      ||
    b2.UpdateVertical(DOWN))
    return false;
  return true;
}

bool Board::UpdateDirection(int _row, int _col, const pair<int,int>& _dir){
  bool indexChanged = false;
  int nextRow = _row+_dir.first;
  int nextCol = _col+_dir.second;
  if(nextCol < 0 || nextCol > m_size-1 || nextRow < 0 || nextRow > m_size-1)
    return false;

  if (!IsEmpty(_row,_col) &&
      (IsEmpty(nextRow,nextCol) || CanMerge(_row,_col,nextRow,nextCol)) ){

    if(IsEmpty(nextRow,nextCol)){ //move
      Move(_row,_col,nextRow,nextCol);
      UpdateDirection(nextRow,nextCol,_dir);
    }
    else //merge
      Merge(_row,_col,nextRow,nextCol);

    indexChanged = true;
  }
  return indexChanged;
}

bool Board::UpdateHorizontal(int _dir){
  bool indexChanged = false;
  int colStart=1,colEnd=m_size;
  if(_dir == RIGHT){
    colStart = m_size-2;
    colEnd = -1;
  }

  for(int col=colStart;col!=colEnd;col+=_dir) //columns
    for(int row=0;row<m_size;++row) //rows
      indexChanged = UpdateDirection(row,col,make_pair(0,_dir*-1)) || indexChanged;

  return indexChanged;
}

bool Board::UpdateVertical(int _dir){
  bool indexChanged = false;
  int rowStart=1,rowEnd=m_size;
  if(_dir == DOWN){
    rowStart = m_size-2;
    rowEnd = -1;
  }
  for(int row=rowStart;row!=rowEnd;row+=_dir)//rows
    for(int col=0;col<m_size;++col)//columns
      indexChanged = UpdateDirection(row,col,make_pair(_dir*-1,0)) || indexChanged;

  return indexChanged;
}

pair<int, int> Board::GetEmptyIndex(){
  pair<int,int> toReturn(-1,-1);
  if (m_emptyIndices.empty())
    return toReturn;
  int index = rand() % m_emptyIndices.size();
  set<int>::iterator found = m_emptyIndices.begin();
  for(int i=0;i<index;++i)
    ++found;
  toReturn.first = *found / 4;
  toReturn.second = *found % 4;
  m_emptyIndices.erase(*found);
  return toReturn;
}

void Board::RemoveEmptyIndex(int _row, int _col){
  if (_row < 0 || _col < 0 || _row >= m_size || _col >= m_size){
    cerr << "Error! Attempt to remove invalid index: " << _row << ", " << _col << endl;
    return;
  }
  m_emptyIndices.erase(_row*4+_col);
}

void Board::AddEmptyIndex(int _row, int _col){
  if(_row < 0 || _row >= m_size || _col < 0 || _col >= m_size){
    cerr << "Attempting to add empty at invalid index: " << _row << ", " << _col << endl;
    return;
  }
  m_board[_row][_col].Reset();
  m_emptyIndices.insert(_row*4+_col);
}

bool Board::PlaceRandomVal(){
  pair<int,int> nextIndex = GetEmptyIndex();
  if(nextIndex.first == -1 || nextIndex.second == -1){
    cerr << "Invalid next index: " << nextIndex.first << ", " << nextIndex.second <<  endl;
    return false;
  }
  m_board[nextIndex.first][nextIndex.second] = NextRandom();
  return true;
}

size_t Board::NextRandom(){
  return rand() % 10 < 9?2:4;
}

bool Board::IsEmpty(int _row, int _col){
  return m_board[_row][_col] == 0;
}

bool Board::CanMerge(int _fromRow, int _fromCol, int _toRow, int _toCol){
  return (m_board[_fromRow][_fromCol] == m_board[_toRow][_toCol] && !m_board[_toRow][_toCol].m_merged);
}

void Board::Move(int _fromRow, int _fromCol, int _toRow, int _toCol){
  std::swap(m_board[_toRow][_toCol],m_board[_fromRow][_fromCol]);
  RemoveEmptyIndex(_toRow,_toCol);
  AddEmptyIndex(_fromRow,_fromCol);
}

void Board::Merge(int _fromRow, int _fromCol, int _toRow, int _toCol){
  m_board[_toRow][_toCol]+=m_board[_fromRow][_fromCol];
  m_score += m_board[_toRow][_toCol].m_val;
  RemoveEmptyIndex(_toRow,_toCol);
  AddEmptyIndex(_fromRow,_fromCol);
}

void Board::ClearMergedFlags(){
  for(int i=0;i<m_size;++i){
    for(int j=0;j<m_size;++j){
      m_board[i][j].m_merged = false;
    }
  }
}

size_t Board::GetScore(){
  return m_score;
}

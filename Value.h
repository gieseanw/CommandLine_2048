/*
* Value.h
* Written by Andy Giese
* A wrapper class for a size_t (unsigned int, usually)
* Associates a 'merged' flag with the size_t
* The merged flag is to indicate when two values have merged 
* into one for the game 2048
*/

#pragma once
#include <iostream>

struct Value{
    Value():m_val(0),m_merged(false){}
    Value(const Value& _rhs){
      *this = _rhs;
    }

    void Reset(){
      m_val = 0;
      m_merged = false;
    }

    //overload operators so that we can treat Value as a size_type
    Value& operator=(const Value& _rhs){
      if (this == &_rhs)
        return *this;
      m_val = _rhs.m_val;
      m_merged = _rhs.m_merged;
      return *this;
    }

    Value& operator=(const size_t& _rhs){
      m_val = _rhs;
      return *this;
    }

    Value& operator+(const Value& _rhs){
      m_val+= _rhs.m_val;
      return *this;
    }

    Value& operator+=(const Value& _rhs){
      *this = (*this + _rhs);
      m_merged = true;
      return *this;
    }

    bool operator!=(const size_t& _rhs){
      return m_val != _rhs;
    }

    bool operator!=(const Value& _rhs){
      return m_val != _rhs.m_val;
    }

    bool operator==(const Value& _rhs){
      return m_val == _rhs.m_val;
    }

    bool operator==(const size_t _rhs){
      return m_val == _rhs;
    }

    //member variables
    size_t m_val;
    bool m_merged;

    //overloaded output operator
    friend std::ostream& operator<<(std::ostream& _fout, const Value& _rhs);
};//end struct Value


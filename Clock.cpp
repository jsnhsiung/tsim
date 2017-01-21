//Clock.cpp
//Jason Hsiung
//hsunweih@gmail.com
#include "Clock.h"
#include <iostream>

Clock::Clock(std::string name, unsigned int cycle_max):
name(name), cycle(0), cycle_max(cycle_max){
}

Clock::~Clock(){
}

unsigned int Clock::getCycle() const{
  return this->cycle;
}

int Clock::tick(){
   this->cycle ++; 
   if(this->cycle < cycle_max){
      return 0;
   }
      return 1;
}


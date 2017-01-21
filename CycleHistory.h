//CycleHistory.h
//Jason Hsiung
//hsunweih@gmail.com
#ifndef _CYCLEHISTORY_H
#define _CYCLEHISTORY_H

#include <string>
#include <map>
#include <iostream>
#include "Clock.h"

class CycleHistory{

private:
   std::string           name;
   std::map <unsigned int, std::string> ledger;
   //cycle, event
public:

   CycleHistory(std::string);
  ~CycleHistory();
   void putEvent(unsigned int, std::string);
   void printHistory();
};//CycleHistory

#endif

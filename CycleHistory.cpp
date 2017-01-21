//CycleHistory.cpp
//Jason Hsiung
//hsunweih@gmail.com
#include "CycleHistory.h"


CycleHistory::CycleHistory(std::string name){
   this->name        = name;
}//CycleHistory::CycleHistory()

CycleHistory::~CycleHistory(){
}//Cycle_history_class::~CycleHistory()

void CycleHistory::putEvent(unsigned int cycle, std::string event_in){
    std::string event;
    if(this->ledger.find(cycle) == this->ledger.end()){
       event = "Event: "+ event_in + "\n";
    } else{
       event = this->ledger[cycle] + "     Event: "+ event_in + "\n";
    }
    this->ledger[cycle] = event;
}//CycleHistory::putEvent

void CycleHistory::printHistory(){
   std::map<unsigned int, std::string>::const_iterator it;
   for(it=ledger.begin(); it!=ledger.end();it++){
     std::cout<<it->first<<" => " <<it->second<<std::endl;
   }
}//CycleHistory

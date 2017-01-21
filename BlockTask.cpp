//BlockTask.cpp
//Jason Hsiung
//hsunweih@gmail.com
#include "BlockTask.h"
#include <cassert>


BlockTask::BlockTask 
(std::string name, unsigned int atom_capacity, std::string type_name) :
name(name), atom_capacity(atom_capacity),
type_name(type_name),history("history")
{
   this->seq_id = BlockTask::blocktask_seq_id;
   BlockTask::blocktask_seq_id ++;
   this->state = BlockTaskState::NOT_ISSUED;
   //std::cout<<"BlockTask "<<name<<" created "<<std::endl;
}//BlockTask::BlockTask()

BlockTask::BlockTask(const BlockTask &btref):
seq_id(btref.seq_id), name(btref.name),
atom_map(btref.atom_map), atom_capacity(btref.atom_capacity),
type_name(btref.type_name),state(btref.state),history("history")
{
}//BlockTask::BlockTask()

BlockTask::BlockTask():history("history")
{
}//BlockTask::~BlockTask()

BlockTask::~BlockTask(){
}//BlockTask::~BlockTask()

void BlockTask::putEvent(const Clock &clk, std::string event){
   this->history.putEvent(clk.getCycle(), event);
}//BlockTask::putEvnet()

void BlockTask::printHistory(){
   this->history.printHistory();
}//BlockTask::printHistory()

int BlockTask::addAtom(const Clock &clk, Atom &atom_to_copy){
   if(this->atom_map.size() < this->atom_capacity){
      //this->atom_map[atom_to_add.getFullName()] = Atom atom_to_add(atom_to_copy);
      Atom atom_to_add(atom_to_copy);
      this->atom_map.insert
         (std::pair<std::string, Atom>(atom_to_add.getFullName(), atom_to_add));
      std::string event = "Atom "+atom_to_add.getName()+" added";
      this->putEvent(clk, event); 
      return 0;
   }else{
     std::cout<<"Error "<<this->name<<" addAtom over capacity"<<std::endl; 
     return 1;
   }

}//BlockTask::addAtom()

int BlockTask::addAtom(Atom &atom_to_copy){
   if(this->atom_map.size() < this->atom_capacity){
      //this->atom_map[atom_to_add.getFullName()] = Atom atom_to_add(atom_to_copy);
      Atom atom_to_add(atom_to_copy);
      this->atom_map.insert
         (std::pair<std::string, Atom>(atom_to_add.getFullName(), atom_to_add));
      std::string event = "Atom "+atom_to_add.getName()+" added";
      return 0;
   }else{
     std::cout<<"Error "<<this->name<<" addAtom over capacity"<<std::endl; 
     return 1;
   }

}//BlockTask::addAtom()


void BlockTask::tickAtom
(std::string atom_to_tick_fullname){
  //return this->atom_map[atom_to_tick_fullname].tick(); 
  std::map<std::string, Atom>::iterator it;
  it = this->atom_map.find(atom_to_tick_fullname);
  it->second.tick();
}//BLockTask::tickAtom()

void BlockTask::tickAllAtoms(){
  //std::cout<<"TickAllAtoms "<<this->name<<std::endl;
  std::map<std::string, Atom>::iterator it;
  it = this->atom_map.begin();
  for(; it !=  this->atom_map.end(); it++){
  it->second.tick();
  }
}//BlockTask::tickAllAtoms()

std::vector<std::string> BlockTask::getAllAtomFullNames(){
   std::vector<std::string> atom_full_names;

  std::map<std::string, Atom>::iterator it;
  it = this->atom_map.begin();
  for(; it !=  this->atom_map.end(); it++){
     atom_full_names.push_back(it->first);
  }
  return atom_full_names;
}//Blocktask::getAllAtomFullNames()

void BlockTask::tick(){
   //std::cout<<this->name<<" ticked"<<std::endl;
   switch(this->state)
   {
      case BlockTaskState::NOT_ISSUED:
         this->state = BlockTaskState::ISSUED;
         //std::cout<<"NOT_ISSUED"<<std::endl;
         break;
      case BlockTaskState::ISSUED:
         this->state = BlockTaskState::ACTIVE;
         break;
      case BlockTaskState::ACTIVE:
         this->state = BlockTaskState::RETIRED;
         break;
      case BlockTaskState::RETIRED:{
         std::cout<<"RETIRED"<<std::endl;
         std::cout<<"BlockTask state error: "<< this->name <<std::endl;
         assert( 1 == 0 && "BlockTask state error, RETIRED");}
      default:{
         std::cout<<"BlockTask state error: "<< this->name <<std::endl;
         assert(1 == 0 && "BlockTask state error");
              }
   }//switch 
}//BlockTask::tick();
BlockTaskState BlockTask::getBTState(){
return this->state;
}

unsigned int BlockTask::getAtomCap() const{
  return this->atom_capacity; 
}//BlockTask::getAtomCap()

BlockTask::atom_map_itr BlockTask::atom_map_begin(){
  return this->atom_map.begin();
}//BlockTask::atom_map_begin()

BlockTask::atom_map_itr BlockTask::atom_map_end(){
  return this->atom_map.end();
}//BlockTask::atom_map_end()

unsigned int BlockTask::blocktask_seq_id = 0;

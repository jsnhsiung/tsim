//Atom.cpp
//Jason Hsiung
//hsunweih@gmail.com
#include "Atom.h"
#include <iostream>
#include <cassert>


Atom::Atom(std::string name,std::string type_name,
           std::string mother_task_name):history("history") {
 
	this -> seq_id              = Atom::atom_seq_id;
	this -> name                = name;
	this -> type_name           = type_name;
   this -> mother_task_name    = mother_task_name;
   this -> fullname            = mother_task_name+"_"+name;
   this -> state               = AtomState::NOT_ISSUED;
   Atom::atom_seq_id ++;
}//Atom::Atom()

Atom::Atom(const Atom &atom_to_copy):
seq_id(atom_to_copy.seq_id),name(atom_to_copy.name),
type_name(atom_to_copy.type_name),
mother_task_name(atom_to_copy.mother_task_name),
fullname(atom_to_copy.fullname),state(atom_to_copy.state),
history(atom_to_copy.history)
{}//Atom::Atom()

Atom::~Atom(){
}//Atom::~Atom()

std::string Atom::getName() const {
  return this->name;
}//Atom::getName()

std::string Atom::getFullName() const{
  std::string fullname = this->fullname;
  return fullname;
}//Atom::getFullName()

void Atom::putEvent(const Clock &clk, std::string event){
   this->history.putEvent(clk.getCycle(), event);
}//Atom::putEvnet()

void Atom::printHistory(){
   this->history.printHistory();
}//Atom::printHistory()

void Atom::tick(){
   switch(this->state)
   {
      case AtomState::NOT_ISSUED:
        this->state = AtomState::ISSUED;
        break;
      case AtomState::ISSUED:
        this->state = AtomState::ACTIVE;
        break;
      case AtomState::ACTIVE:
        this->state = AtomState::RETIRED;
        break;
      case AtomState::RETIRED:
        assert ( 1 == 0 && "Atom state error, RETIRED"); 
        std::cout<<"Atom "<<this->fullname<<" state error"<<std::endl;
        break;
      default:
        assert ( 1 == 0 && "Atom state error"); 
        std::cout<<"Atom "<<this->fullname<<" state error"<<std::endl;
        break;
   }//switch
}//Atom::tick()

std::string Atom::getState(){
    switch(this->state)
   {
      case AtomState::NOT_ISSUED:
        return "NOT_ISSUED";
      case AtomState::ISSUED:
        return "ISSUED";
      case AtomState::ACTIVE:
        return "ACTIVE";
      case AtomState::RETIRED:
        return "RETIRED";
      default:
        return "ERROR";
   }//switch
}//Atom::getState()

AtomState Atom::getAtomState(){
return this->state;
}//Atom::getAtomState()

unsigned int Atom::atom_seq_id = 0;

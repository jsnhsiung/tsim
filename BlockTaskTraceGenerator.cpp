//BlockTaskTraceGenerator.cpp
//Jason Hsiung
//hsunweih@gmail.com
#include "BlockTaskTraceGenerator.h"
#include <stdlib.h>
#include <time.h>
#include <sstream>
#include <iostream>
#include <cassert>


BlockTaskTraceGenerator::BlockTaskTraceGenerator
(std::string name, unsigned int max_atoms,
 unsigned int task_cyc, unsigned int max_concr,
 unsigned int atom_service_min_delay,
 unsigned int atom_service_max_delay):
name(name), max_atoms(max_atoms), task_cyc(task_cyc),
atom_service_min_delay(atom_service_min_delay),
atom_service_max_delay(atom_service_max_delay),
max_concr(max_concr)
{
//{{{1

      this->seq_id = BlockTaskTraceGenerator::bttracegen_seq_id;
      BlockTaskTraceGenerator::bttracegen_seq_id ++;

      this->statistics_bts = 0; 
      this->statistics_atoms = 0; 

      unsigned int cyc_itr = 0;
      unsigned int concr_itr = 0;
      unsigned int concr_num;
      unsigned int atom_num;
    
      std::cout <<"Generating synthetic trace...."<<std::endl
                <<"Maximum number of tasking cycles: "<<task_cyc<<std::endl 
                <<"Maximum number of atoms: "<<max_atoms<<std::endl
                <<"Maximum degree of concurrency: " <<max_concr<<std::endl;
      for(cyc_itr = 0; cyc_itr < task_cyc; cyc_itr++){
           srand(time(NULL)+cyc_itr+concr_itr);      
           concr_num = rand()%max_concr + 1; 
         for(concr_itr = 0; concr_itr < concr_num; concr_itr++){
           srand(time(NULL)+cyc_itr+concr_itr);      
           atom_num = rand()%max_atoms + 1; 

           std::pair <unsigned int, unsigned int> key;
           key.first  = cyc_itr;
           key.second = concr_itr; 

           std::ostringstream oss_cyc_itr;
           std::ostringstream oss_concr_itr;
           oss_cyc_itr << cyc_itr;
           oss_concr_itr << concr_itr;

           std::string blocktask_name = "BLOCKTASK_"+oss_cyc_itr.str()+"_"+oss_concr_itr.str();
           BlockTask blocktask_gen(blocktask_name, atom_num, "type-0");
          
           this->statistics_bts = this->statistics_bts + 1;    
           this->statistics_atoms = this->statistics_atoms + atom_num;    
           //Populate atom names
           for(unsigned int atom_itr = 0; atom_itr<atom_num;atom_itr++){
              srand(time(NULL)+cyc_itr+concr_itr+atom_itr);      
              std::ostringstream oss_atom_id;
              oss_atom_id << rand()%65536;
              std::string atom_name = "ATOM_"+ oss_atom_id.str(); 
              Atom atom(atom_name, "type-0", blocktask_name);
              blocktask_gen.addAtom(atom);
              
           }

           this->trace_map.insert
              (std::pair<std::pair<unsigned int, unsigned int>, std::string>(key, blocktask_name));
           this->blocktask_map.insert
              (std::pair<std::string, BlockTask>(blocktask_name, blocktask_gen));
         }
      }//for tasking cycles
      
      //****Generate atom trace*****//
      std::map<std::string, unsigned int> atom_name_cycle_map; //<name, cycle>

      unsigned int service_delay_offset;
      unsigned int service_delay;

      std::map<std::pair<unsigned int, unsigned int>, std::string>::iterator  trace_map_itr;
      trace_map_itr = this->trace_map.begin(); 
      for(;trace_map_itr != this->trace_map.end(); trace_map_itr++){
         std::vector<std::string> atomfullnames = this->getAllAtomFullNames(trace_map_itr->second);
         unsigned int cyc = trace_map_itr->first.first;
         unsigned int cur = trace_map_itr->first.second;
         for(unsigned int avi = 0; avi<atomfullnames.size(); avi++){
         unsigned int asciisum = 0;
         for(int i = 0; i < atomfullnames[avi].size(); i++){
            asciisum = asciisum + atomfullnames[avi][i];
         }
         //std::cout<<atomfullnames[avi]<<" ascii sum "<< asciisum<<std::endl;

         srand(time(NULL)+cyc+cur+asciisum);      
         service_delay_offset = 
                   rand()%(atom_service_max_delay-atom_service_min_delay+1)+atom_service_min_delay;
         service_delay = service_delay_offset + cyc;
         atom_name_cycle_map.insert(std::pair<std::string, unsigned int>(atomfullnames[avi], service_delay));
         }
        
      }//trace_map itr, build aomt_name_cycle_map;

      std::map<std::string, unsigned int>::iterator  atcm_itr;//atom_name_cycle_map_itr;
      atcm_itr = atom_name_cycle_map.begin();

      for(;atcm_itr != atom_name_cycle_map.end(); atcm_itr++){
         for(unsigned int concr_itr=0; concr_itr>=0; concr_itr++){//unbouned loop
         if(this->atom_trace_map.find(std::pair<unsigned int, unsigned int>(atcm_itr->second, concr_itr))
               == this->atom_trace_map.end()){
           std::pair <unsigned int, unsigned int> key;
           key.first  = atcm_itr->second;
           key.second = concr_itr; 
              this->atom_trace_map.insert
              (std::pair<std::pair<unsigned int, unsigned int>, std::string>(key, atcm_itr->first));
           break;
         }//if concurrency index does not exists;
         }
      }
//}}}1
}//BlockTaskTraceGenerator::BlockTaskTraceGenerator()

BlockTaskTraceGenerator::~BlockTaskTraceGenerator(){
}//BlockTaskTraceGenerator::~BlockTaskTraceGenerator()

void BlockTaskTraceGenerator::printTraceMap(){
//{{{1
      std::cout<<"@"<<this->name<<" trace_map:"<<std::endl;
      for(std::map<std::pair<unsigned int, unsigned int>,
                  std::string>::iterator it=this->trace_map.begin();
       it != this->trace_map.end(); it++){
       std::cout << it->first.first <<","<<it->first.second <<" => " 
                 << it->second <<": "<<this->getBTAtomNum(it->second)<<" atoms"<<std::endl; 
   }
//}}}1
}///BlockTaskTraceGenerator::printTraceMap()

void BlockTaskTraceGenerator::printAtomTraceMap(){
//{{{1
      std::cout<<"@"<<this->name<<" atom_trace_map:"<<std::endl;
      for(std::map<std::pair<unsigned int, unsigned int>,
                  std::string>::iterator it=this->atom_trace_map.begin();
       it != this->atom_trace_map.end(); it++){
       std::cout << it->first.first <<","<<it->first.second <<" => " 
                 << it->second<<std::endl; 
   }
//}}}1
}///BlockTaskTraceGenerator::printAtomTraceMap()

unsigned int BlockTaskTraceGenerator::getBTAtomNum(std::string btname) const //const
{
//{{{1
   std::map<std::string, BlockTask>::const_iterator it //cosnt
      = this->blocktask_map.find(btname) ; 
   if(it == this->blocktask_map.end()){
      assert("BlockTask not found in blocktask_map when getBTAtomNum()\n");
   }else{
      return it->second.getAtomCap(); 
   }
      return 0;// avoid compiler warning
//}}}1
}//BlockTaskTraceGenerator::getBTAtomNum()

void BlockTaskTraceGenerator::tickBT(std::string btname){
//{{{1
   std::map<std::string, BlockTask>::iterator it =
     this->blocktask_map.find(btname);
   if(it == this->blocktask_map.end()){
     assert("BlockTask not found in blocktask_map when tickBT()\n");
   }else{
      //std::cout<<"BTTG tick BT"<<std::endl;
      it->second.tick();
   }
//}}}1
}//BlockTaskTraceGenerator::tickBT()

void BlockTaskTraceGenerator::tickBTAtoms(std::string btname){
//{{{1
   std::map<std::string, BlockTask>::iterator it =
     this->blocktask_map.find(btname);
   if(it == this->blocktask_map.end()){
     assert("BlockTask not found in blocktask_map when tickBT()\n");
   }else{
      it->second.tickAllAtoms();
   }
//}}}1
}//BlockTaskTraceGenerator::tickBTAtoms()


void BlockTaskTraceGenerator::tickAtom(std::string atomFullName){
//{{{1
    std::string btname = "X";
    std::string atomname;

    std::istringstream ss(atomFullName);
    std::string token;
    std::vector<std::string> tokenVector;
    int atommet = 0;
    while(std::getline(ss, token, '_')) {
         tokenVector.push_back(token);
    }
    for(int tv_itr = 0; tv_itr < tokenVector.size(); tv_itr++){
       if(atommet == 0){
          if(tokenVector[tv_itr] == "ATOM"){
             atomname = "ATOM_";
             atommet = 1;
          }else{
             if(btname == "X"){
                btname = tokenVector[tv_itr];
             }else{
                btname = btname+"_"+tokenVector[tv_itr];
             }
          }
       }else{
          atomname = atomname+tokenVector[tv_itr];
       }
    }
    
    std::map<std::string, BlockTask>::iterator it =
     this->blocktask_map.find(btname);

    std::map<std::string, Atom>::iterator atom_it =
       this->atom_map_ofaBT_begin(it->first);
    for(; atom_it != this->atom_map_ofaBT_end(it->first); atom_it++){
       if(atom_it->second.getName()== atomname) break;
    }
    atom_it->second.tick();
//}}}1
}//BlockTaskTraceGenerator::tickAtom()

std::vector<std::string> BlockTaskTraceGenerator::getAllAtomFullNames(std::string btname){
//{{{1
   std::vector<std::string> names;
   std::map<std::string, BlockTask>::iterator it =
     this->blocktask_map.find(btname);
   if(it == this->blocktask_map.end()){
     assert("BlockTask not found in blocktask_map when tickBT()\n");
   }else{
      names = it->second.getAllAtomFullNames();
      return names;
   }
   
   return names;
//}}}1
}//BlockTaskTraceGenerator::getAllAtomFullNames()
 
BlockTaskTraceGenerator::trace_map_itr 
//{{{1
BlockTaskTraceGenerator::trace_map_begin(){
return this->trace_map.begin();
//}}}1
}
BlockTaskTraceGenerator::trace_map_itr 
//{{{1
BlockTaskTraceGenerator::trace_map_end(){
return this->trace_map.end();
//}}}1
}

BlockTaskTraceGenerator::atom_trace_map_itr 
//{{{1
BlockTaskTraceGenerator::atom_trace_map_begin(){
return this->atom_trace_map.begin();
//}}}1
}

BlockTaskTraceGenerator::atom_trace_map_itr 
//{{{1
BlockTaskTraceGenerator::atom_trace_map_end(){
return this->atom_trace_map.end();
//}}}1
}

BlockTaskState BlockTaskTraceGenerator::getBTState(std::string btname){
//{{{1

    std::map<std::string, BlockTask>::iterator it =
     this->blocktask_map.find(btname);
    return it->second.getBTState();
//}}}1
}//BlockTaskState BlockTaskTraceGenerator::getBTState(std::string btname){

AtomState BlockTaskTraceGenerator::getAtomState(std::string atomFullName){
//{{{1

    std::string btname = "X";
    std::string atomname;

    std::istringstream ss(atomFullName);
    std::string token;
    std::vector<std::string> tokenVector;
    int atommet = 0;
    while(std::getline(ss, token, '_')) {
         tokenVector.push_back(token);
    }
    for(int tv_itr = 0; tv_itr < tokenVector.size(); tv_itr++){
       if(atommet == 0){
          if(tokenVector[tv_itr] == "ATOM"){
             atomname = "ATOM_";
             atommet = 1;
          }else{
             if(btname == "X"){
                btname = tokenVector[tv_itr];
             }else{
                btname = btname+"_"+tokenVector[tv_itr];
             }
          }
       }else{
          atomname = atomname+tokenVector[tv_itr];
       }

    }//reconstruct names

    std::map<std::string, BlockTask>::iterator it =
     this->blocktask_map.find(btname);

    std::map<std::string, Atom>::iterator atom_it =
       this->atom_map_ofaBT_begin(it->first);
     //it->second.atom_map.find(atomname);
    for(; atom_it != this->atom_map_ofaBT_end(it->first); atom_it++){
       if(atom_it->second.getName()== atomname) break;
    }

    AtomState atomst = atom_it->second.getAtomState();

    return atom_it->second.getAtomState();
//}}}1
}//BlockTaskTraceGenerator::getAtomState(){

std::string BlockTaskTraceGenerator::getAtomStateString
(std::string atomFullName){
//{{{1

    std::string btname = "X";
    std::string atomname;

    std::istringstream ss(atomFullName);
    std::string token;
    std::vector<std::string> tokenVector;
    int atommet = 0;
    while(std::getline(ss, token, '_')) {
         tokenVector.push_back(token);
    }
    for(int tv_itr = 0; tv_itr < tokenVector.size(); tv_itr++){
       if(atommet == 0){
          if(tokenVector[tv_itr] == "ATOM"){
             atomname = "ATOM_";
             atommet = 1;
          }else{
             if(btname == "X"){
                btname = tokenVector[tv_itr];
             }else{
                btname = btname+"_"+tokenVector[tv_itr];
             }
          }
       }else{
          atomname = atomname+tokenVector[tv_itr];
       }

    }//reconstruct names

    std::map<std::string, BlockTask>::iterator it =
     this->blocktask_map.find(btname);

    //BlockTaskState bts = it->second.getBTState();
    
    std::map<std::string, Atom>::iterator atom_it =
       this->atom_map_ofaBT_begin(it->first);
     //it->second.atom_map.find(atomname);
    for(; atom_it != this->atom_map_ofaBT_end(it->first); atom_it++){
       if(atom_it->second.getName()== atomname) break;
    }

    AtomState atomst = atom_it->second.getAtomState();
    
    std::string retstring;
    switch(atomst)
   {
      case AtomState::NOT_ISSUED:
         retstring = "NOT_ISSUED";
         break;
      case AtomState::ISSUED:
         retstring = "ISSUED";
         break;
      case AtomState::ACTIVE:
         retstring = "ACTIVE";
         break;
      case AtomState::RETIRED:
         retstring = "RETIRED";
         break;
      default:
         retstring = "WTF";
         break;
   }//switch 
   
    std::cout<<"getAtomStateString "<<atomFullName<<std::endl;
    std::cout<<"getAtomStateString "<<btname<<std::endl;
    std::cout<<"getAtomStateString "<<atomname<<std::endl;
    std::cout<<"getAtomStateString "<<retstring<<std::endl;
    return retstring;
//}}}1
}//BlockTaskTraceGenerator::getAtomStateString()

BlockTask::atom_map_itr BlockTaskTraceGenerator::atom_map_ofaBT_begin(std::string btname){
//{{{1
    std::map<std::string, BlockTask>::iterator it =
    this->blocktask_map.find(btname);
    return it->second.atom_map_begin();
//}}}1
}//BlockTaskTraceGenerator::atom_map_ofaBT_begin()

BlockTask::atom_map_itr BlockTaskTraceGenerator::atom_map_ofaBT_end(std::string btname){
//{{{1
    std::map<std::string, BlockTask>::iterator it =
    this->blocktask_map.find(btname);
    return it->second.atom_map_end();
//}}}1
}//BlockTaskTraceGenerator::atom_map_ofaBT_end()

std::pair<unsigned int, unsigned int> BlockTaskTraceGenerator::printStatistics(){
//{{{1
    std::cout<<this->name<<" total number of BTs generated: "<<this->statistics_bts<<std::endl;
    std::cout<<this->name<<" total number of Atoms generated: "<<this->statistics_atoms<<std::endl;
    return std::pair<unsigned int, unsigned int>(this->statistics_bts, this->statistics_atoms);
//}}}1
}//BlockTaskGenerator::printStatistics(){
unsigned int BlockTaskTraceGenerator::bttracegen_seq_id = 0;

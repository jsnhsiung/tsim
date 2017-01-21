//Cache.cpp
//Jason Hsiung
//hsunweih@gmail.com
#include "Cache.h"
#include "BlockTask.h"
#include <cassert>
//adding BlockTask.h is not an elegant way to check the state...
//may provide a uniform way to describing the state's of residents
Cache::Cache
(std::string name, std::string type_name, unsigned int capacity):
name(name), type_name(type_name), capacity(capacity),
current_num_resd(0),history("history")
{
//{{{1

   this->seq_id = Cache::cache_seq_id;
   Cache::cache_seq_id ++;
   this->addedNum = 0;
   this->removedNum = 0;
   std::cout<<"Cache "<<this->seq_id<<" created, name "
           <<this->name<<std::endl;
//}}}1
}//Cache::Cache()

Cache::~Cache(){
}//Cache::~Cache()

void Cache::putEvent(const Clock &clk, std::string event){
//{{{1
   this->history.putEvent(clk.getCycle(), event);
//}}}1
}//Cache::putEvnet()

void Cache::printResdMap(const Clock &clk){
//{{{1
  std::map<std::string, unsigned int>::iterator map_itr
     = this->resd_map.begin();
  std::cout<<this->name<<" capacity "<<this->capacity<<std::endl;
  for(; map_itr !=  this->resd_map.end(); map_itr++){
     std::cout<<map_itr->first<<"  "<<std::endl;
  }
//}}}1
}//Cache::printResdMap()

void Cache::printHistory(){
//{{{1
   std::cout<<"@"<<this->name<<" Hisotry:"<<std::endl;
   this->history.printHistory();
//}}}1
}//Cache::print_history()


std::vector<std::string> Cache::tryAddBlockResd
(const Clock &clk, Cache &src_cache,
 BlockTaskTraceGenerator &bttg){
//{{{1

   assert(this->resd_map.size() <= this->capacity
         && "Cache overflow!");
   std::vector<std::string> resd_added_names;

   unsigned int curr_size(this->resd_map.size());
   unsigned int capacity(this->capacity);   
   unsigned int room(capacity - curr_size); 
   unsigned int btatomcap;
   if(curr_size < capacity){
      //iterate through src_cache 
      Cache::map_iterator itr = src_cache.map_begin();
      for(; itr != src_cache.map_end(); itr++){
         btatomcap = bttg.getBTAtomNum(itr->first); 
         if(btatomcap <= room){
          //add bulk atoms here, iterating through atom_map of the BT
           //TD: check BT state here 
            if(bttg.getBTState(itr->first) == BlockTaskState::ISSUED){

            std::vector<std::string> atomfullnames = 
               bttg.getAllAtomFullNames(itr->first);
            
            for(int vec_itr = 0; vec_itr <atomfullnames.size(); vec_itr++){
            this->resd_map.insert
            (std::pair<std::string, unsigned int>
             (atomfullnames[vec_itr],clk.getCycle()));
            this->addedNum = this->addedNum + 1;
            std::string event = atomfullnames[vec_itr]+" added";
            this->putEvent(clk, event);
            }

            bttg.tickBT(itr->first);
//            bttg.tickBTAtoms(itr->first); 
            resd_added_names.push_back(itr->first);
            room = this->capacity - this->resd_map.size();
            }
         }
      }//for iterating through src_cache
      return resd_added_names;
   }else{
      resd_added_names.push_back("None");
      return resd_added_names;
   }

//}}}1
}//Cache::tryAddBlockResd()

Cache::map_iterator Cache::map_begin(){
//{{{1
   return this->resd_map.begin();
//}}}1
}//Cache::map_begin()

Cache::map_iterator Cache::map_end(){
//{{{1
  return this->resd_map.end();
//}}}1
}//Cache::map_begin()

std::vector<std::string> Cache::tryAddResd
(const Clock &clk,
 BlockTaskTraceGenerator &bttg){
//{{{1

   assert(this->resd_map.size() <= this->capacity
         && "Cache overflow!");
   std::vector<std::string> resd_added_names;
   int leave = 0;

   unsigned int curr_size(this->resd_map.size());
   unsigned int capacity(this->capacity);   
   unsigned int room(capacity - curr_size); 
   unsigned int btatomcap;

   unsigned int curr_cycle(clk.getCycle());

   BlockTaskTraceGenerator::trace_map_itr itr = bttg.trace_map_begin();

   while(curr_size < capacity){
      for(; itr != bttg.trace_map_end(); itr++){
         if(itr->first.first > curr_cycle){
            if(resd_added_names.size() == 0){
               resd_added_names.push_back("None");
            }
           leave = 1; 
           break; //break trace_map iteration loop
         }//the rest is for the future 
         else{
             if(bttg.getBTState(itr->second) == BlockTaskState::NOT_ISSUED){
             this->resd_map.insert
             (std::pair<std::string, unsigned int>
                 (itr->second,clk.getCycle()));

             this->addedNum = this->addedNum + 1;
             std::string event=itr->second+" added";

             this->putEvent(clk, event);
             curr_size = this->resd_map.size();
             resd_added_names.push_back(itr->second);
             bttg.tickBT(itr->second);
             if(curr_size == capacity) break;
             }
         }
      }//for iterating through trace_map
      if(leave == 1 || itr == bttg.trace_map_end()) break;
   }//while curr_size < capacity
   
   if(resd_added_names.size() == 0){
      resd_added_names.push_back("None");
     }

       return resd_added_names;
//}}}1 
}//Cache::tryAddResd()

std::vector<std::string> Cache::tryAddResd
(const Clock &clk,
 BlockTaskTraceGenerator &bttg,
 std::string donothing){
//{{{1

   assert(this->resd_map.size() <= this->capacity
         && "Cache overflow!");
   std::vector<std::string> resd_added_names;
   int leave = 0;

   unsigned int curr_size(this->resd_map.size());
   unsigned int capacity(this->capacity);   
   unsigned int room(capacity - curr_size); 
   unsigned int btatomcap;

   unsigned int curr_cycle(clk.getCycle());

   BlockTaskTraceGenerator::atom_trace_map_itr itr =
      bttg.atom_trace_map_begin();

   while(curr_size < capacity){
      for(; itr != bttg.atom_trace_map_end(); itr++){
         if(itr->first.first > curr_cycle){
            if(resd_added_names.size() == 0){
               resd_added_names.push_back("None");
            }
           leave = 1; 
           break; //break atom_trace_map iteration loop
         }//the rest in the atom_trace_map is for the future 
         else{
             if(bttg.getAtomState(itr->second) == AtomState::NOT_ISSUED){
             this->resd_map.insert
             (std::pair<std::string, unsigned int>
                 (itr->second,clk.getCycle()));
             this->addedNum = this->addedNum + 1;
             std::string event=itr->second+" added";
             //std::cout<<event<<std::endl;
             this->putEvent(clk, event);
             curr_size = this->resd_map.size();
             resd_added_names.push_back(itr->second);
             bttg.tickAtom(itr->second);
             if(curr_size == capacity) break;
             }
         }
      }//for iterating through trace_map
      if(leave == 1 || itr == bttg.atom_trace_map_end()) break;
   }//while curr_size < capacity
   
   if(resd_added_names.size() == 0){
      resd_added_names.push_back("None");
     }

      return resd_added_names;
//}}}1
}//Cache::tryAddResd()


std::vector<std::string> Cache::tryServiceResd
(const Clock & clk, Cache & src_cache, BlockTaskTraceGenerator & bttg){
//{{{1
   assert(this->resd_map.size() <= this->capacity
         && "Cache overflow!");
   std::vector<std::string> servicedNames;
   Cache::map_iterator src_map_itr = src_cache.map_begin();
   for(; src_map_itr != src_cache.map_end(); src_map_itr++){
      //if(this->capacity > this->resd_map.size(){
        if(this->resd_map.find(src_map_itr->first) != this->resd_map.end()){
           //this->resd_map.erase(scr_map_itr->first);
           bttg.tickAtom(src_map_itr->first);
           //tick from "ISSUED" to "ACTIVE"
           servicedNames.push_back(src_map_itr->first);
        }
      //}else{break;}
   }
   if(servicedNames.size() == 0){servicedNames.push_back("None");}
   return servicedNames;
//}}}1
}//Cache::tryServiceResd()

std::vector<std::string> Cache::removeActiveResd
(const Clock & clk, BlockTaskTraceGenerator & bttg){
//{{{1

   //std::cout<<this->name<<" Remove active resd"<<std::endl;

   std::vector<std::string> removedLastNames;
   std::map<std::string, unsigned int> btname_map;

   std::vector<std::string> resd_name_to_remove;
   std::map<std::string, unsigned int>::iterator resd_map_itr =
   this->resd_map.begin();
   for(; resd_map_itr != resd_map.end(); resd_map_itr++){
   
    //reconstruct names
    std::string btname = "X";
    std::string atomname;
    std::istringstream ss(resd_map_itr->first);
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
    //reconstruct names

      if(btname_map.find(btname) ==btname_map.end()){
         btname_map.insert(std::pair<std::string, unsigned int>(btname, 1));
      }else{
         btname_map[btname] = btname_map[btname] + 1; 
      }

      //std::cout<<resd_map_itr->first<<"  2"  <<std::endl;
      //std::cout<<resd_map_itr->first<<" "<<bttg.getAtomStateString(resd_map_itr->first)<<std::endl;
      if(bttg.getAtomState(resd_map_itr->first) ==
            AtomState::ACTIVE){
      //std::cout<<resd_map_itr->first<<"  3"  <<std::endl;
         bttg.tickAtom(resd_map_itr->first);
         //this->resd_map.erase(resd_map_itr->first); //problematic!!
         resd_name_to_remove.push_back(resd_map_itr->first);
         btname_map[btname] = btname_map[btname] - 1;
      }
   }//looping the resd map

   for(int itr = 0; itr < resd_name_to_remove.size(); itr ++){
      this->resd_map.erase(resd_name_to_remove[itr]);

         this->removedNum = this->removedNum + 1;    
         std::string event = resd_name_to_remove[itr]+" removed\n";
         this->putEvent(clk, event);
   }

   std::map<std::string, unsigned int>::iterator btname_map_itr =
   btname_map.begin();

   for(; btname_map_itr != btname_map.end(); btname_map_itr++){
      assert(btname_map_itr->second >= 0 && "btname_map count underflow");
      if(btname_map_itr->second == 0){
        bttg.tickBT(btname_map_itr->first);
        removedLastNames.push_back(btname_map_itr->first);
      }
   }
   
   if(removedLastNames.size() == 0){ removedLastNames.push_back("None");}
   return removedLastNames;

//}}}1
}//Cache::removeActiveResd()

void Cache::removeRetiredResd
(const Clock & clk, BlockTaskTraceGenerator & bttg, std::string ba){
//{{{1

   //std::cout<<this->name<<" Remove retired resd"<<std::endl;
std::map<std::string, unsigned int>::iterator resd_map_itr
   = resd_map.begin();
std::vector<std::string> resd_name_to_remove;

for(; resd_map_itr != this->resd_map.end(); resd_map_itr++){
   if(ba == "atom"){
     if(bttg.getAtomState(resd_map_itr->first) == AtomState::RETIRED){
       //this->resd_map.erase(resd_map_itr->first);
       resd_name_to_remove.push_back(resd_map_itr->first);
       //std::string event = resd_map_itr->first+" removed";
       //this->putEvent(clk, event);
       
     }
  }else if(ba =="blocktask"){
     if(bttg.getBTState(resd_map_itr->first) == BlockTaskState::RETIRED){
       //this->resd_map.erase(resd_map_itr->first);
       resd_name_to_remove.push_back(resd_map_itr->first);
       //std::string event = resd_map_itr->first+" removed";
       //this->putEvent(clk, event);
     }
  }

}//looping resd_map

   for(int itr = 0; itr < resd_name_to_remove.size(); itr ++){
      this->resd_map.erase(resd_name_to_remove[itr]);
         this->removedNum = this->removedNum + 1;    
         std::string event = resd_name_to_remove[itr]+" removed\n";
         this->putEvent(clk, event);
   }

//}}}1
}//Cache::removeRetiredResd()


std::pair<unsigned int, unsigned int> Cache::printStatistics(){
//{{{1
   std::cout<<this->name<<" added "<<this->addedNum<<" residents"<<std::endl;
   std::cout<<this->name<<" removed "<<this->removedNum<<" residents"<<std::endl;
   assert(this->addedNum == this->removedNum && "added and removed numbers not matching");
   return std::pair<unsigned int, unsigned int>(this->addedNum, this->removedNum);
//}}}1
}//Cache::printSatistics()

void Cache::snapShot(const Clock & clk){
//{{{1
 this->snapshot.insert(std::pair<unsigned int, unsigned int>(clk.getCycle(), this->resd_map.size())); 
//}}}1
}//Cache::snapShot

void Cache::printSnapShots(){
//{{{1
   std::map<unsigned int, unsigned int>::iterator ss_itr =
   this->snapshot.begin();
   for(; ss_itr != this->snapshot.end(); ss_itr++){
      std::cout<<this->name<<" Cycle: "<<ss_itr->first<<
                             " Occupancy: "<<ss_itr->second<<std::endl;
   }
//}}}1
}//Cache::printSnapShots()

unsigned int Cache::cache_seq_id = 0;

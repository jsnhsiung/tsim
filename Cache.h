//Cache.h
//Jason Hsiung
//hsunweih@gmail.com
#ifndef _CACHE_H
#define _CACHE_H

#include <string>
#include <iostream>
#include <map>
#include <vector>
#include "CycleHistory.h"
#include "Clock.h"
#include "BlockTaskTraceGenerator.h"
#include <sstream>

class Cache
{
 private:
          static unsigned int cache_seq_id; 
          unsigned int  seq_id;
          std::string   name;
          std::string   type_name;
          unsigned int  capacity;
          unsigned int  current_num_resd;
          //number of current residents
          unsigned int  addedNum;
          unsigned int  removedNum;
          CycleHistory  history;
          std::map<unsigned int, unsigned int> snapshot;
          
          std::map<std::string, unsigned int> resd_map; 
          //resident name, cycle inserted
 public: 

 
          Cache(std::string, std::string, unsigned int);
          //name, type name, capacity  
         ~Cache();

          std::vector<std::string> tryAddResd
          (const Clock &, BlockTaskTraceGenerator &);
          //Try add a resident one at a time
          //from BlockTaskTraceGenerator trace map

          std::vector<std::string> tryAddResd
          (const Clock &, BlockTaskTraceGenerator &, std::string);
          //Try add a resident one at a time
          //from BTSelection window 
          
          std::vector<std::string> tryAddBlockResd
          (const Clock &, Cache &, BlockTaskTraceGenerator &);
          //Try add multiple residents at a time
          //from another cache
          //Not in use now!

          std::vector<std::string> tryServiceResd
          (const Clock &, Cache &, BlockTaskTraceGenerator &);
          //Try to service residents from a source cache
          //tick residents who name presents in the source cache from
          //ISSUED to ACTIVE

          std::vector<std::string> removeActiveResd
          (const Clock &, BlockTaskTraceGenerator &);
          //Remove residents (remove ACTIVE resdidents)
          
          std::vector<std::string> tryAddResd
          (const Clock &, 
           const std::map<std::pair<unsigned int, unsigned int>, std::string> &);
          //overload for trace_map 
          
          std::pair<unsigned int, unsigned int> printStatistics();

          void removeRetiredResd
          (const Clock &, BlockTaskTraceGenerator &, std::string);

          void putEvent(const Clock &, std::string);
          void printHistory();
          void printResdMap(const Clock &);
          void snapShot(const Clock &);
          void printSnapShots();

          typedef std::map<std::string, unsigned int>::iterator map_iterator;
          typedef std::map<std::string, unsigned int>::const_iterator const_map_iterator;
          map_iterator map_begin();
          map_iterator map_end();
          const_map_iterator const_map_begin();
          const_map_iterator const_map_end();
          
};//class Cache

#endif // not deifned _CACHE_H

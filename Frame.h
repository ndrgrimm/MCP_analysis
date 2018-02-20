#include <stdlib.h>
#include <map>
#include <string>
#include "SegmentExctracion.h"




struct TFrame{
    
    u_char    SampleType;
    char*   ptrArray;
    uint32_t   arrSize;
    std::map<std::string,SArgument> FlagList;
  
};


TFrame* Segment2Frame(SSegment* Segment){
    if ( Segment == 0 )
        return 0;
//     log << "Inside" << std::endl;
    TFrame* OutPut=new TFrame;
//     log << "TFrame Created" << std::endl;

    OutPut->arrSize=Segment->arrSize;
//     log << "Array Size copyed" << std::endl;
    OutPut->ptrArray=    Segment->ptrArray;
//     log << "Array ptr copyed " << std::endl;

    OutPut->SampleType=Segment->SampleType;
//      log << "Array Type copyed " << std::endl;
//     log << Segment->FlagCount << " Flags to Copy. Let's start.." << std::endl;
    for (int i=0; i <Segment->FlagCount; ++i){
        
        OutPut->FlagList[ std::string(Segment->FlagList[i].Name) ]= Segment->FlagList[i].Flag;
//         log << "Flag number: " << i << " convert in map entry" << std::endl;
        
    }
    
    return OutPut;
}

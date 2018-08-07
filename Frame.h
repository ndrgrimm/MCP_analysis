#include <stdlib.h>
#include <map>
#include <string>
#include "SegmentExctracion.h"


/*!
 * 
 * 
 * 
 */


struct TFrame{
    
    u_char     SampleType;
    char*      ptrArray;
    uint32_t   arrSize;
    
    std::map<std::string,SArgument> FlagList;
  
};


TFrame* Segment2Frame(SSegment* Segment){
    if ( Segment == 0 )
        return 0;

    TFrame* OutPut=new TFrame;


    OutPut->arrSize=Segment->arrSize;

    OutPut->ptrArray= Segment->ptrArray; 


    OutPut->SampleType=Segment->SampleType;
    for (int i=0; i <Segment->FlagCount; ++i){
        
        OutPut->FlagList[ std::string(Segment->FlagList[i].Name) ]= Segment->FlagList[i].Flag;

        
    }
    
    return OutPut;
}



double GetSample(TFrame* Frame, int Sample)
{
	switch( Frame->SampleType )
	{
		case SAMPLE_BYTE :	return (double) ((u_char*) Frame->ptrArray)[Sample];
		case SAMPLE_INT8 :	return (double) ((int8_t*) Frame->ptrArray)[Sample];
		case SAMPLE_INT16:	return (double) ((int16_t*) Frame->ptrArray)[Sample];
		case SAMPLE_INT32:	return (double) ((int32_t*) Frame->ptrArray)[Sample];
		case SAMPLE_INT64:  return (double) ((int64_t*) Frame->ptrArray)[Sample];
		case SAMPLE_DOUBLE: return ((double*) Frame->ptrArray)[Sample];
		
	}

	return 0;
}

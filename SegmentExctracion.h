/*
*
*
* 
* 
* 
* 
* 
* 
*/
#ifndef SEGMENT_EXCTRACTION
#define SEGMENT_EXCTRACTION

#include <string.h>
#include <stdint.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>

#define  log std::cout
#define  errlog std::cout 
 

#define SAMPLE_BYTE		0
#define SAMPLE_INT8		1
#define SAMPLE_INT16	2
#define SAMPLE_INT32	3
#define SAMPLE_INT64	4
#define SAMPLE_DOUBLE	5


#define ARG_BYTE	0
#define ARG_INTEGER	1
#define ARG_REAL	2




struct SArgument
{
	u_char Type;
	union uArgument
	{
		char Bytes[64];
		int64_t	Integer;
		double Real;
	}
	Argument;
};


struct SFlag_Inside
 {
     uint16_t Name[16];
     SArgument   Flag;
 };
 

 struct SFlag
 {
     char Name[16];
     SArgument   Flag;
 };
 

struct SSegment
{
    u_char    SampleType;
    char*   ptrArray;
    uint32_t   arrSize;
    SFlag*  FlagList;
    int     FlagCount;                      
};

int GetSampleCount(SSegment* Segment)
{
	switch( Segment->SampleType )
	{
		case SAMPLE_BYTE :	return 0;
		case SAMPLE_INT8 :	return Segment->arrSize;
		case SAMPLE_INT16:	return Segment->arrSize/2;
		case SAMPLE_INT32:	return Segment->arrSize/4;
		case SAMPLE_INT64:  return Segment->arrSize/8;
		case SAMPLE_DOUBLE: return Segment->arrSize/8;
		
        
    }

	return 0;
}

//FIXME Diocaroscrivila in template
double GetSample(SSegment* Segment, int Sample)
{
	switch( Segment->SampleType )
	{
		case SAMPLE_BYTE :	return (double) ((u_char*)Segment->ptrArray)[Sample];
		case SAMPLE_INT8 :	return (double) ((int8_t*)Segment->ptrArray)[Sample];
		case SAMPLE_INT16:	return (double) ((int16_t*)Segment->ptrArray)[Sample];
		case SAMPLE_INT32:	return (double) ((int32_t*)Segment->ptrArray)[Sample];
		case SAMPLE_INT64:  return (double) ((int64_t*)Segment->ptrArray)[Sample];
		case SAMPLE_DOUBLE: return ((double*)Segment->ptrArray)[Sample];
		
	}

	return 0;
}


SSegment* GetSegment(std::istream* InputStream){
    
    if (InputStream->tellg() < 0)
        return 0 ;
    

    char tmpBuffer[128];
    InputStream->read(tmpBuffer,sizeof(uint32_t));

    
    if ( 4 !=(*(uint32_t*) tmpBuffer) )
    { 
       errlog << "Wrong Header";
       errlog << InputStream->tellg() << std::endl;
       return 0;
       
    }

    
    InputStream->read(tmpBuffer,sizeof(uint32_t));

    if ((*(uint32_t*) tmpBuffer) != 'BIN1')
    {
        
        errlog << "Binary File Version: " << (*(uint32_t*) tmpBuffer) << 
            "is wrong. Expected: " << 'BIN1' << std::endl;
        return 0;
    }
    
    SSegment* Segment= new SSegment;
    InputStream->read(tmpBuffer,sizeof(unsigned char));
    Segment->SampleType=*tmpBuffer;
    

    
    
    InputStream->read(tmpBuffer,sizeof(int));
    int SizeFlagList= *((int*) tmpBuffer);
    Segment->FlagCount= SizeFlagList/sizeof(SFlag_Inside);

    
    
    if ( SizeFlagList ){
        Segment->FlagList=new SFlag[Segment->FlagCount];
        
        SFlag_Inside* tmpFlagInsideList=new SFlag_Inside[Segment->FlagCount];
        
        InputStream->read( ((char*)tmpFlagInsideList), SizeFlagList);
        for (int i=0; i<Segment->FlagCount;++i)
        {
            Segment->FlagList[i].Flag.Type=tmpFlagInsideList[i].Flag.Type;
            Segment->FlagList[i].Flag.Argument.Integer=tmpFlagInsideList[i].Flag.Argument.Integer;
            Segment->FlagList[i].Flag.Argument.Real=tmpFlagInsideList[i].Flag.Argument.Real;
            strcpy( Segment->FlagList[i].Flag.Argument.Bytes,tmpFlagInsideList[i].Flag.Argument.Bytes);
            for(int j =0; j<16; ++j){
                Segment->FlagList[i].Name[j]=char(tmpFlagInsideList[i].Name[j]);
                
            }
            Segment->FlagList[i].Name[15]='\0';
                
            
        }
    }
    
    
    
    
    
    
    
    InputStream->read( tmpBuffer, sizeof(int));    // Read Array Size
    Segment->arrSize= ( *((int*)tmpBuffer) );            // Save Array Size
    

    
    Segment->ptrArray=new char[Segment->arrSize ];   // Copy data
    InputStream->read(  Segment->ptrArray , Segment->arrSize);
    
    return Segment;
}


#endif // SEGMENT_EXCTRACTION

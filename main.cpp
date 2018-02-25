#include <iostream>
#include <stdint.h>
#include <fstream>
#include <vector>
#include <cstdlib>

//#include "Frame.h"
//#include "SegmentExctracion.h"
#include "tsegment.h"


int main(int argc, char **argv) {
    std::cout << "Hello, world!" << std::endl;
    
    int NumberOfSegmentMAx=-1;
    int indexFile=1;
    if (argc > 2 ){
      NumberOfSegmentMAx=std::atoi(argv[1]);
      indexFile++;
    } 
    
    
    std::ifstream* Input=new std::ifstream;
    
    Input->open( argv[indexFile], std::ios::binary);
    TSegment* Segment;

    Input->seekg(0,Input->end);
    long long unsigned int FileLenght = Input->tellg();                                  // ndrg: FileLenght can be very very big
    Input->seekg(0,Input->beg);
    std::cout << "length File: " <<  FileLenght << std::endl;
   
    
    
    
    
    
   
    int NumberOfSegment=0;
    while(Input->tellg() < FileLenght)                                          // FIXME memory leak like shit
        
    {
        NumberOfSegment++;
        Segment= loadSegment(Input);
        
        //std::cout << Segment->GetSampleCount() << std::endl;
        delete Segment;                                            // FIXME: ofcourse if I delete the array on the
       
        
        if( NumberOfSegment >= NumberOfSegmentMAx ) break;
    }
    Input->close();
    
    
    std::cout << "Number Of Frame: " << NumberOfSegment << std::endl;

    return 0;
    
}




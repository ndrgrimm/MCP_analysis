#include <iostream>
#include <stdint.h>
#include <fstream>
#include <vector>

#include "Frame.h"
#include "SegmentExctracion.h"



int main(int argc, char **argv) {
    std::cout << "Hello, world!" << std::endl;
    
    std::ifstream* Input=new std::ifstream;
    
    Input->open( argv[1], std::ios::binary);
    SSegment* Segment;

    Input->seekg(0,Input->end);
    long long int FileLenght = Input->tellg();                                  // ndrg: FileLenght can be very very big
    Input->seekg(0,Input->beg);
    std::cout << "length File: " <<  FileLenght << std::endl;
   
    
    
    
    
    
    int tmp=0;
    int tmp2=0;
    int NumberOfSegment=0;
    while(Input->tellg() < FileLenght)                                          // FIXME memory leak like shit
        
    {
        NumberOfSegment++;
        Segment= GetSegment(Input);
        TFrame* Frame_tmp=Segment2Frame(Segment);                           
      //  std::cout << GetSample(Frame_tmp,100) <<  std::endl;
         TestDeleteSegment(Segment) ;                                            // FIXME: ofcourse if I delete the array on the
       // delete Segment;                                                                        // segment I'll delete it on the Frame too
        
        //if( NumberOfSegment >= 10 ) break;
    }
    Input->close();
    
    
    log << "Number Of Frame: " << NumberOfSegment << std::endl;

    return 0;
    
}




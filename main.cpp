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
    int FileLenght =Input->tellg();
    Input->seekg(0,Input->beg);

    int i=0;
    
    
    
    
    
    int tmp=0;
    int tmp2=0;
    int NumberOfSegment=0;
    while(Input->tellg() < FileLenght)
    {
        NumberOfSegment++;
        Segment= GetSegment(Input);
        tmp2=Segment2Frame(Segment)->FlagList[std::string("RUN")].Argument.Integer;
        if (tmp != tmp2 ){
            
            tmp=tmp2;
            log << tmp << std::endl;
        }
    }
    Input->close();
    
    log << "Number Of Frame: " << NumberOfSegment << std::endl;

    return 0;
    
}




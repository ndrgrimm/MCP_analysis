/*
 * <one line to give the library's name and an idea of what it does.>
 * Copyright (C) 2018  ndrg <ndrgrimm@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef TSEGMENT_H
#define TSEGMENT_H


#define SAMPLE_BYTE		0
#define SAMPLE_INT8		1
#define SAMPLE_INT16	2
#define SAMPLE_INT32	3
#define SAMPLE_INT64	4
#define SAMPLE_DOUBLE	5


#define ARG_BYTE	0
#define ARG_INTEGER	1
#define ARG_REAL	2

#include <string.h>
#include <stdint.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>




 
class TSegment{
    
public:
    
    class SFlag
    {
    public:
        struct SArgument {
            u_char Type; //!< ARG_ Enumerate 
            union uArgument {
                char Bytes[64];
                int64_t	Integer;
                double Real;
            }
            Argument;

        };
        char Name[16];
        SArgument   Flag;
        
    };
    
    
    
    friend  TSegment* loadSegment( std::istream* InputStream, uint FlagStart, uint FlagStop, uint FlagThreshold );
    ~TSegment();
    

    
    
    //FIXME Set it in Template design
    double     GetSample(int Sample);
    int        GetSampleCount();
    void       printFlags(  std::ostream& outStream= std::cout );
    int        GetFlagIndex(const char* FlagName);
    
    // FIXME sistemare diritti accesso lettura scrittura

    u_char           getSampleType(){ return SampleType; };
    char*            getPtrArray()  { return ptrArray;   };
    uint32_t         getArrSize()   { return arrSize;    };
    const SFlag*     getFlagList()  { return FlagList;   };    //FIXME using this function generate a memory leak
    int              getFlagCount() { return FlagCount;  };
    
    
    

   


    
    
private:
    TSegment(){}
    TSegment( const TSegment &obj){}
    u_char     SampleType;
    char*      ptrArray;
    uint32_t   arrSize;
    SFlag*     FlagList;
    int        FlagCount;  
    
    
    
};

 TSegment* loadSegment( std::istream* InputStream, uint FlagStart, uint FlagStop, uint FlagThreshold );
#endif // TSEGMENT_H

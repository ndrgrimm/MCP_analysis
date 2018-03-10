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

#include "tsegment.h"
#include <string.h>
#include <stdint.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>

#define  log std::cout
#define  errlog std::cerr



TSegment* loadSegment( std::istream* InputStream, uint FlagStart, uint FlagStop, uint FlagThreshold )
{
    struct SFlag_Inside {
        uint16_t Name[16];
        TSegment::SFlag::SArgument   Flag;
    };
    
    
    
    
    if ( InputStream->tellg() < 0 ) {
        return 0 ;
    }


    char tmpBuffer[128];
    InputStream->read ( tmpBuffer,sizeof ( uint32_t ) );


    if ( 4 != ( * ( uint32_t* ) tmpBuffer ) ) {
        errlog << "Wrong Header: " << ( * ( uint32_t* ) tmpBuffer ) << std::endl;
        errlog << "bit position:"<< InputStream->tellg() << std::endl;
        return 0;

    }


    InputStream->read ( tmpBuffer,sizeof ( uint32_t ) );

    if ( ( * ( uint32_t* ) tmpBuffer ) != 'BIN1' ) {

        errlog << "Binary File Version: " << ( * ( uint32_t* ) tmpBuffer ) <<
               "is wrong. Expected: " << 'BIN1' << std::endl;
        return 0;
    }


    InputStream->read ( tmpBuffer,sizeof ( unsigned char ) );
    TSegment* SegmentRead= new TSegment;
    SegmentRead->SampleType=*tmpBuffer;




    InputStream->read ( tmpBuffer,sizeof ( int ) );
    int SizeFlagList= * ( ( int* ) tmpBuffer );
    SegmentRead->FlagCount= SizeFlagList/sizeof ( SFlag_Inside );


    // FIXME: find a more stable and secure check:
    if ( SizeFlagList ) {
        SegmentRead->FlagList=new TSegment::SFlag[SegmentRead->FlagCount];

        SFlag_Inside* tmpFlagInsideList=new SFlag_Inside[SegmentRead->FlagCount];

        InputStream->read ( ( ( char* ) tmpFlagInsideList ), SizeFlagList );
        for ( int i=0; i<SegmentRead->FlagCount; ++i ) {
            SegmentRead->FlagList[i].Flag.Type=tmpFlagInsideList[i].Flag.Type;
            SegmentRead->FlagList[i].Flag.Argument.Integer=tmpFlagInsideList[i].Flag.Argument.Integer;
            SegmentRead->FlagList[i].Flag.Argument.Real=tmpFlagInsideList[i].Flag.Argument.Real;
            strcpy ( SegmentRead->FlagList[i].Flag.Argument.Bytes,tmpFlagInsideList[i].Flag.Argument.Bytes );
            for ( int j =0; j<16; ++j ) {
                SegmentRead->FlagList[i].Name[j]=char ( tmpFlagInsideList[i].Name[j] );

            }
            SegmentRead->FlagList[i].Name[15]='\0';

        }
    }







    InputStream->read ( tmpBuffer, sizeof ( int ) );          // Read Array Size
    SegmentRead->arrSize= ( * ( ( int* ) tmpBuffer ) );       // Save Array Size



    SegmentRead->ptrArray=new char[SegmentRead->arrSize ];   // Copy data created with new[]
    
    InputStream->read ( SegmentRead->ptrArray , SegmentRead->arrSize );
    
    int Flag =0;
    int FlagEx =SegmentRead->GetSample( (FlagStop + FlagStart)/2) ;
    bool Flagx=true;
    for (int indexSegment= FlagStart; indexSegment < FlagStop; ++ indexSegment ){
        if ( SegmentRead->GetSample( indexSegment ) < FlagThreshold)
             {
            Flagx=false;
             }
    }
    
//     log << "Flag" << Flag << std::endl;
    Flag/=(FlagStop-FlagStart);
//     log << "Flag" << Flag << std::endl;
//     log << "FlagEx" << FlagEx << std::endl;

    bool Mode = (FlagEx > FlagThreshold);
//     log << Mode << std::endl;
    int ModeIndex=-1;
    int FlagIndex=-1;
    for ( int index=0; index< SegmentRead->getFlagCount();++index )
    {
        if( strcmp(SegmentRead->FlagList[index].Name, "MODE" ) == 0  ){
            ModeIndex=index;
        }
        if( strcmp(SegmentRead->FlagList[index].Name, "FLAG" ) == 0  ){
            FlagIndex=index;
        }   
        
    }
    
    if ( Flagx ) {
        SegmentRead->FlagList[ModeIndex].Flag.Argument=SegmentRead->FlagList[FlagIndex].Flag.Argument;
        SegmentRead->FlagList[ModeIndex].Flag.Type=SegmentRead->FlagList[FlagIndex].Flag.Type;
    }
    //if (Falgx) log << "Flag update" << std::endl;
    
    return SegmentRead;


}

int TSegment::getFlagIndex(char* FlagName){
    for ( int flagindex=0; flagindex< this->getFlagCount();++flagindex)
    {
        if( strcmp(this->FlagList[flagindex].Name, FlagName ) ==0 ){
            return flagindex;
        }
    }
    return -1;
}


TSegment::~TSegment()
{
    this->SampleType=0;
    this->arrSize =0;
    this->FlagCount= 0;

    delete[] this->FlagList;
    delete[] this->ptrArray;
    
}


int TSegment::GetSampleCount ( )
{
    switch ( this->SampleType ) {
    case SAMPLE_BYTE :
        return 0;
    case SAMPLE_INT8 :
        return this->arrSize;
    case SAMPLE_INT16:
        return this->arrSize/2;
    case SAMPLE_INT32:
        return this->arrSize/4;
    case SAMPLE_INT64:
        return this->arrSize/8;
    case SAMPLE_DOUBLE:
        return this->arrSize/8;


    }

    return 0;
}

double  TSegment::GetSample ( int Sample )
{
    switch ( this->SampleType ) {
    case SAMPLE_BYTE :
        return ( double ) ( ( u_char* )  this->ptrArray ) [Sample];
    case SAMPLE_INT8 :
        return ( double ) ( ( int8_t* )  this->ptrArray ) [Sample];
    case SAMPLE_INT16:
        return ( double ) ( ( int16_t* ) this->ptrArray ) [Sample];
    case SAMPLE_INT32:
        return ( double ) ( ( int32_t* ) this->ptrArray ) [Sample];
    case SAMPLE_INT64:
        return ( double ) ( ( int64_t* ) this->ptrArray ) [Sample];
    case SAMPLE_DOUBLE:
        return ( ( double* ) this->ptrArray ) [Sample];

    }

    return 0;
}



void TSegment::printFlags( std::ostream& outStream)
{
    
    
    for (int i=0; i <this->FlagCount;i+=1){
        
        outStream << this->FlagList[i].Name << " : " << std::flush;

        switch ( this->FlagList[i].Flag.Type ) {
        case ARG_REAL :
            outStream << this->FlagList[i].Flag.Argument.Real << " ; " << std::endl;
            break;
        case ARG_INTEGER :
            outStream << this->FlagList[i].Flag.Argument.Integer << " ; " << std::endl;
            break;
        case ARG_BYTE :
            outStream << this->FlagList[i].Flag.Argument.Bytes << " ; " << std::endl;
            break;
        }

    }
    
  
}

// TSegment::SFlag::SArgument TSegment::getFlagsArg( char*FlagTag )
// {
//     
//     
//     for (int i=0; i <this->FlagCount;i+=1){
//         
//         outStream << this->FlagList[i].Name << " : " << std::flush;
// 
//         
// 
//     }
//     return 
//   
// }

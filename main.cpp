#include <iostream>
#include <stdint.h>
#include <fstream>
#include <vector>
#include <cstdlib>

//#include "Frame.h"
//#include "SegmentExctracion.h"
#include "tsegment.h"
#include <uchar.h>
#include <root/TH1S.h>

#include "Analisys.h"
#include <root/TFile.h>
#include <root/TFile.h>
#include <root/TFile.h>


int main ( int argc, char **argv )
{
    std::cout << "Hello, world!" << std::endl;

    unsigned int NumberOfSegmentMAx=-1;
    NumberOfSegmentMAx=std::atoi ( argv[1] );
    unsigned long StartSubSegment= std::atoi ( argv[2] );
    unsigned int numberPeaks= std::atoi ( argv[3] );
    std::ifstream* Input=new std::ifstream;
    Input->open ( argv[4], std::ios::binary );
    
    
    
    
    TSegment* Segment;
    Segment= loadSegment ( Input );
    unsigned int lenghtSegment=Segment->GetSampleCount();
    unsigned int lenghtSubSegment=lenghtSegment-StartSubSegment;
    
    Input->seekg ( 0,Input->end );
    long long unsigned int FileLenght = Input->tellg();                                  // ndrg: FileLenght can be very very big
    Input->seekg ( 0,Input->beg );
    std::cout << "length File: " <<  FileLenght << std::endl;



    TFile* outfile        = new TFile("test.root", "RECREATE");
    TH1S* PeakNumber      = new TH1S("PeakNumber","Distribution of peaks number",10,0,9);
    TH1S* PeakAmplitude   = new TH1S("PeakAmplitude","Distribution of peaks Amplitude",2056*2,-2056,2056);
    TH1F* Spectrum        = new TH1F("Spectrum","Distribution Of peaks number",lenghtSubSegment,0,lenghtSubSegment);
    TH1F* SpectrumFiltred = new TH1F("SpectrumFiltred","Distribution Of peaks number",lenghtSubSegment,0,lenghtSubSegment);

    int NumberOfSegment=0;

    while ( Input->tellg() < FileLenght ) {                                     // FIXME memory leak like shit
        Segment= loadSegment ( Input );
        
        Analysis(PeakNumber,PeakAmplitude,SpectrumFiltred,Spectrum,Segment,numberPeaks,30,StartSubSegment);
        delete Segment;
        //break;
        NumberOfSegment++;
        if ( NumberOfSegment > NumberOfSegmentMAx )  break;           
        
    }

    Input->close();
    PeakNumber->Write(); 
    PeakAmplitude->Write();
    Spectrum->Write(); 
    SpectrumFiltred->Write();
    outfile->Close();
    std::cout << "Number Of Frame: " << NumberOfSegment << std::endl;

    return 0;

    
}




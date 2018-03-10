#include <iostream>
#include <stdint.h>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <string.h>

//#include "Frame.h"
//#include "SegmentExctracion.h"
#include "tsegment.h"
#include <uchar.h>
#include <root/TH1S.h>

#include "Analisys.h"
#include <root/TFile.h>
#include <root/TNtuple.h>



int main ( int argc, char **argv )
{
    std::cout << "Hello, world!2" << std::endl;

    unsigned int NumberOfSegmentMAx=-1;
    NumberOfSegmentMAx=std::atoi ( argv[1] );
    unsigned long StartSubSegment= std::atoi ( argv[2] );
    unsigned long StopSubSegment= std::atoi ( argv[3] );
    unsigned int numberPeaks= std::atoi ( argv[4] );
    std::ifstream* Input=new std::ifstream;
    Input->open ( argv[5], std::ios::binary );

    uint StartFlag=32000;
    uint StopFlag=32500;
    uint FlagThreshold=500;
    
    
   
    unsigned int lenghtSubSegment=StopSubSegment-StartSubSegment;
    
    unsigned int Threshold = 50;
    
    
   

    

    Input->seekg ( 0,Input->end );
    long long unsigned int FileLenght = Input->tellg();                                  // ndrg: FileLenght can be very very big
    Input->seekg ( 0,Input->beg );
    std::cout << "length File: " <<  FileLenght << std::endl;

    char fileRoot[16];
    if (argc > 6 ){
        strcpy(fileRoot, argv[6]);
    }else{
        strcpy(fileRoot,"test.root");
    }
    TFile* outfile        = new TFile ( fileRoot, "RECREATE" );


    // mode 1
    
    TNtuple* PeakInfoMit   = new TNtuple ( "PeakInfoMit",
                                          "Data of Peaks - Mit",
                                          "Amp:Pos:Numbers:index");
    TH1F* SpectrumMit        = new TH1F ( "SpectrumMit",
                                          "Distribution Of peaks number - Mit",
                                          lenghtSubSegment,0,lenghtSubSegment );
    TH1F* SpectrumFiltredMit = new TH1F ( "SpectrumFiltredMit",
                                          "Peak Filter - Mit",
                                          lenghtSubSegment,0,lenghtSubSegment );
    TH1F* SpectrumFiltredIntMit = new TH1F ( "SpectrumFiltredIntMit",
                                          "Integral Distriminator  - Mit",
                                          lenghtSubSegment,0,lenghtSubSegment );
    TH1F* SpectrumFiltredOldMit = new TH1F ( "SpectrumFiltredOldMit",
            "Old Discriminator - Mit",
            lenghtSubSegment,0,lenghtSubSegment );

    // mode 2
                                              
    TNtuple* PeakInfoAsi   = new TNtuple ( "PeakInfoAsi",
                                          "Data of Peaks - Mit",
                                          "Amp:Pos:Numbers:index");
    TH1F* SpectrumAsi        = new TH1F ( "SpectrumAsi",
                                          "Distribution Of peaks number - Asi",lenghtSubSegment,0,lenghtSubSegment );
    TH1F* SpectrumFiltredAsi = new TH1F ( "SpectrumFiltredAsi",
                                          "Peak Filter - Asi",lenghtSubSegment,0,lenghtSubSegment );
    TH1F* SpectrumFiltredIntAsi = new TH1F ( "SpectrumFiltredIntAsi",
                                          "Integral Distriminator  - Asi",lenghtSubSegment,0,lenghtSubSegment );
    TH1F* SpectrumFiltredOldAsi = new TH1F ( "SpectrumFiltredOldAsi",
                                         "Old Discriminator - Asi",lenghtSubSegment,0,lenghtSubSegment );



    int NumberOfSegment=0;


    
    TSegment* Segment=NULL;
    uint NumberOfSegmentMode2=0;
    uint NumberOfSegmentMode1=0;

    while ( Input->tellg() < FileLenght ) {

        Segment= loadSegment ( Input, StartFlag , StopFlag, FlagThreshold );
        int RunsIndex = Segment->getFlagIndex ( "RUN" );
        int Run=-1;
        if ( RunsIndex < 0 ) {
            Segment->printFlags();
            return 1;
        } else {
            Run=Segment->getFlagList() [RunsIndex].Flag.Argument.Integer;

        }
        if ( Run > 9000 ) {
            continue;
        }
        //std::cout << "Analising" << std::endl;
        int ModeIndex = Segment->getFlagIndex ( "MODE" );
        int Mode=-1;
        if ( ModeIndex < 0 ) {
            Segment->printFlags();
            return 1;
        } else {
            Mode = Segment->getFlagList() [ModeIndex].Flag.Argument.Integer;
            //std::cout << "Mode" << Mode << std::endl;
        }
        
        
//         bool Done=false; //FIXME DEBUG
//         if( NumberOfSegment == NumberOfSegmentMAx ){/*//FIXME DEBUG*/
        switch( Mode ){
            
            case 1:
            Analysis (PeakInfoMit,SpectrumFiltredMit,SpectrumMit,Segment,numberPeaks,50,StartSubSegment,StopSubSegment );
            AnalysisIntegralDiscriminator( SpectrumFiltredIntMit , SpectrumMit, Segment,Threshold,StartSubSegment,StopSubSegment );
            AnalysisOld ( SpectrumFiltredOldMit , NULL, Segment,Threshold,StartSubSegment,StopSubSegment );
            NumberOfSegmentMode1++;
            break;

            case 2: 
            Analysis (PeakInfoAsi,SpectrumFiltredAsi,SpectrumAsi,Segment,numberPeaks,50,StartSubSegment,StopSubSegment );
            AnalysisIntegralDiscriminator( SpectrumFiltredIntAsi , SpectrumAsi, Segment,Threshold,StartSubSegment,StopSubSegment );
            AnalysisOld ( SpectrumFiltredOldAsi , NULL, Segment,Threshold,StartSubSegment,StopSubSegment );
            NumberOfSegmentMode2++;
            break;
                       
        }
//         Done=true;//FIXME DEBUG
//         }//FIXME DEBUG
        delete Segment;


        NumberOfSegment++;

//         if (Done) break; //FIXME DEBUG

        if ( NumberOfSegment > NumberOfSegmentMAx ) {
            break;
        }
    }
    Input->close();

    PeakInfoMit->Write();
    SpectrumMit->Scale(1./NumberOfSegmentMode1);
    SpectrumMit->Write();
    SpectrumFiltredMit->Write();
    SpectrumFiltredIntMit->Write();
    SpectrumFiltredOldMit->Write();

    
    PeakInfoAsi->Write();
    SpectrumAsi->Write();
    SpectrumAsi->Scale(1./NumberOfSegmentMode2);
    SpectrumFiltredAsi->Write();
    SpectrumFiltredIntAsi->Write();
    SpectrumFiltredOldAsi->Write();

    outfile->Close();


    std::cout << "Number Of Frame: "  << NumberOfSegment << std::endl;
    std::cout << "Number Of mode 1: " << NumberOfSegmentMode1 << std::endl;
    std::cout << "Number Of mode 0: " << NumberOfSegmentMode2 << std::endl;

    return 0;


}




#include"tsegment.h"
#include<root/TSpectrum.h>
#include<root/TH1F.h>
#include<root/TH1S.h>
#include<root/TNtuple.h>

#include<iostream>

void Analysis ( TNtuple* PeakInfo,TH1F* SpectrumFiltred,TH1F* Spectrum,TSegment * inData,uint NumberPeakMax, Double_t ThreadShold, unsigned int Start, int unsigned Stop=0 )
{



    if ( Stop==0 ) {
        Stop=inData->GetSampleCount();
    }
    unsigned int lenght= Stop-Start;
    // std::cout << lenght << std::endl;
    if ( lenght > inData->GetSampleCount() || Stop > inData->getArrSize() ) {
        return;
    }


    TH1F * TmpSpectrum= ( TH1F* ) Spectrum->Clone();
    TSpectrum* Analyzer;


    TmpSpectrum->SetName ( "tmp" );
    double*   ptrArray_f=new double[lenght];
    bool SomethingBigger=false;
    for ( int i=0; i <lenght; ++i ) {
        ptrArray_f[i]=-1*inData->GetSample ( Start+i ); // change sign of data
        Spectrum->Fill ( i,ptrArray_f[i] );
        if ( ptrArray_f[i] < ThreadShold ) {
            TmpSpectrum->SetBinContent ( i,0 );
        } else {
            TmpSpectrum->SetBinContent ( i,ptrArray_f[i] );
            SomethingBigger=true;
        }
    }
    if ( !SomethingBigger ) {
        //if(false){
        PeakInfo->Fill ( 0, 0, -1 );
        return;
    } else {
        Analyzer=new TSpectrum();

    }



    std::cout << "Max: " << TmpSpectrum->GetBinContent ( TmpSpectrum->GetMaximumBin() ) << std::endl;


    Analyzer->Search ( TmpSpectrum, 2, "", 0.05 );

    Double_t * dest   = new Double_t[lenght];
    //Analyzer->SearchHighRes ( ptrArray_f,dest,lenght,2, 10  ,kFALSE,3,kFALSE,1 );

    std::cout << "PeakNumber: " << Analyzer->GetNPeaks() << std::endl;

    for ( int peak=0; peak<Analyzer->GetNPeaks(); ++peak ) {
        PeakInfo->Fill ( ptrArray_f[int(Analyzer->GetPositionX() [peak]+0.5)],  //TEST
                         Analyzer->GetPositionX() [peak]+0.5,
                         Analyzer->GetNPeaks(),
                         peak ); 
    }



    if ( Analyzer->GetNPeaks() <= NumberPeakMax ) {
        for ( int peak =0; peak < NumberPeakMax && peak < Analyzer->GetNPeaks() ; ++peak ) {
            SpectrumFiltred->Fill ( int(Analyzer->GetPositionX() [peak]) );          
        }
    }

    delete Analyzer;
    delete[] ptrArray_f;
    delete[] dest;

}


void AnalysisOld ( TH1F* SpectrumFiltred,TH1F* Spectrum,TSegment * inData, Double_t ThreadShold, unsigned int Start, int unsigned Stop=0 )
{



    if ( Stop==0 ) {
        Stop=inData->GetSampleCount();
    }
    unsigned int lenght= Stop-Start;
    // std::cout << lenght << std::endl;
    if ( lenght > inData->GetSampleCount() || Stop > inData->getArrSize() ) {
        return;
    }

    if ( Spectrum != NULL ) {

        for ( int i=0; i <lenght; ++i ) {
            Spectrum->Fill ( Start+i,-1.* ( inData->GetSample ( Start+i ) ) );

        }

    }

    for ( int i=0; i <lenght; ++i ) {

        if ( -1.* inData->GetSample ( Start+i )  > ThreadShold ) {
            SpectrumFiltred->Fill ( i,+1 );
        } else {
            SpectrumFiltred->Fill ( i,+0 );
        }


    }

}

void AnalysisIntegralDiscriminator ( TH1F* SpectrumFiltred,TH1F* Spectrum,TSegment * inData, Double_t ThreadShold, unsigned int Start, int unsigned Stop=0 )
{



    if ( Stop==0 ) {
        Stop=inData->GetSampleCount();
    }
    unsigned int lenght= Stop-Start;
    // std::cout << lenght << std::endl;
    if ( lenght > inData->GetSampleCount() || Stop > inData->getArrSize() ) {
        return;
    }

    if ( Spectrum != NULL ) {

        for ( int i=0; i <lenght; ++i ) {
            Spectrum->Fill ( Start+i,-1.* ( inData->GetSample ( Start+i ) ) );

        }

    }

    for ( int i=0; i <lenght; ++i ) {

        if ( -1.* inData->GetSample ( Start+i )  > ThreadShold ) {
            if ( -1.* inData->GetSample ( Start+i-1 )  <= ThreadShold ) {
                SpectrumFiltred->Fill ( i,+1 );
            }

        } else {
            SpectrumFiltred->Fill ( i,+0 ); //FIXME Questa linea serva?
        }


    }

}


// void CFD( TH1F* SpectrumFiltred,TH1F* Spectrum,TSegment * inData, Double_t ThreadShold, unsigned int Start, int unsigned Stop=0 )
// {
//
//
//
//     if ( Stop==0 ) {
//         Stop=inData->GetSampleCount();
//     }
//     unsigned int lenght= Stop-Start;
//     // std::cout << lenght << std::endl;
//     if ( lenght > inData->GetSampleCount() || Stop > inData->getArrSize() ) {
//         return;
//     }
//
//     if( Spectrum != NULL ){
//
//     for ( int i=0; i <lenght; ++i ) {
//             Spectrum->Fill ( Start+i,-1.* (inData->GetSample( Start+i ) ) );
//
//         }
//
//     }
//
//     for ( int i=0; i <lenght; ++i ) {
//
//         if ( -1.* inData->GetSample( Start+i )  > ThreadShold ) {
//             if ( -1.* inData->GetSample( Start+i-1 )  < -1.* inData->GetSample( Start+i ) ){
//                 SpectrumFiltred->Fill ( i,+1 );
//             }
//
//         } else {
//             SpectrumFiltred->Fill ( i,+0 ); //FIXME Questa linea serva?
//         }
//
//
//     }
//
// }

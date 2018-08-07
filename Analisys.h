#include"tsegment.h"
#include<root/TSpectrum.h>
#include<root/TH1F.h>
#include<root/TH1S.h>
#include<root/TGraph.h>
#include<root/TNtuple.h>
#include<root/TCanvas.h>
#include<root/TImage.h>
#include <root/TApplication.h>
#include<iostream>

TApplication * theApp_global;


int  PeakAnalysis ( TNtuple* PeakInfo, TSegment * inData, Double_t ThreadShold, unsigned int Start, int unsigned Stop=0, int IndexFrame=-1 )
{

    unsigned int MaxNumberPeak=200;

    if ( Stop==0 ) {

        Stop=inData->GetSampleCount();
        std::cerr <<"[Warning] : @PeakAnalysis: SubSegment upper limits bigger"
                  <<" that Sample Numbers." << std::flush;
    }
    unsigned int lenght= Stop-Start;

    if ( lenght > inData->GetSampleCount() || Stop > inData->getArrSize() ) {

        std::cerr <<"[Error] : @PeakAnalysis: SubSegment limits are wrong:" << std::flush;
        std::cerr <<"\n\tSub Segment lenght =\t"     << lenght
                  <<"\n\tStart Segment lenght =\t "    << Start
                  <<"\n\tStop Sub Segment  =\t "       << Stop
                  << std::endl;
        return 0;
    }
    //FIXME: this tmpSpectrum is requested by "Search" of "Analyzer".
    //       If you can call directly, SearchHighRes only ptrArray_f is requested
    TH1F* TmpSpectrum = new TH1F ( "TmpSpectrum", "", Stop-Start,0,Stop-Start );
    TmpSpectrum->SetName ( "tmpSpectrum" );

    Double_t * dest        = new Double_t[lenght];
    Double_t*   ptrArray_f = new Double_t[lenght];
    TSpectrum* Analyzer=0;

    bool SomethingBigger=false;
    unsigned int numberPeakFund=0;




    for ( int i=0; i <lenght; ++i ) {
        ptrArray_f[i]=-1*inData->GetSample ( Start+i ); // change sign of data
        if ( ptrArray_f[i] < ThreadShold ) {
            TmpSpectrum->SetBinContent ( i,0 );

        } else {
            TmpSpectrum->SetBinContent ( i,ptrArray_f[i] );
            SomethingBigger=true;
        }

    }




    if ( !SomethingBigger ) {
        //if(false){
        PeakInfo->Fill ( 0,         //ptrArray_f[int(Analyzer->GetPositionX() [peak]+0.5)],  //TEST
                         0,         //Analyzer->GetPositionX() [peak]+0.5,
                         -1,        //Analyzer->GetNPeaks(),
                         0,         //peak,
                         0          //IndexFrame
                       );
        numberPeakFund=0;
        goto endfucntion;
    } else {
        Analyzer=new TSpectrum ( MaxNumberPeak,2 );
    }


    //NB TmpSpectrum is lower cut by ThreadShold value.
    Analyzer->Search ( TmpSpectrum, 3, "goff", 0.05 );
    numberPeakFund=Analyzer->GetNPeaks();


    if ( numberPeakFund >= MaxNumberPeak ) {


        TCanvas *canvas = new TCanvas();
        canvas->SetTitle ( "Segmet that overfill the Analyzer" );

        double *X= new double[lenght];

        for ( int i=0; i <lenght; ++i ) {
            X[i]=i*1.;
        }

        TGraph* TmpGraphSpectrum = new TGraph ( lenght,X,ptrArray_f );
        TmpGraphSpectrum->SetName ( "TmpGraphSpectrum" );

        TmpGraphSpectrum->Draw ( "A*" );


        canvas->Update();
        theApp_global->Run ( kTRUE );

        canvas->Close();
        delete canvas;
        delete TmpGraphSpectrum;
        delete[] X;

        std::cerr << "[Warning] Segment plotted. Please take a look and make a decision." << std::endl;
        std::cerr << "          Do you want to stop the analysis (S), discrard and continue(d) or add and continue (a)?\n>>" << std::flush;
        char cinBuffer[16];
        std::cin >> cinBuffer;
        switch ( cinBuffer[0] ) {
        case 'a':
            break;
        case 'd':
            numberPeakFund=-1;
            goto endfucntion;
        default:
            exit ( 1 );
        }


    }




    // FIXME: Try to use SearchHighRes
    //Analyzer->SearchHighRes ( ptrArray_f,dest,lenght,2, 10  ,kFALSE,3,kFALSE,1 );
    //std::cout << "PeakNumber: " << Analyzer->GetNPeaks() << std::endl;

    for ( int peak=0; peak<Analyzer->GetNPeaks(); ++peak ) {
        PeakInfo->Fill ( //TmpSpectrum->GetBinContent(peak) ,
            ptrArray_f[int ( Analyzer->GetPositionX() [peak]+0.5 )], //TEST
            Analyzer->GetPositionX() [peak]+0.5,
            Analyzer->GetNPeaks(),
            peak,
            IndexFrame
        );
    }


endfucntion:

//     std::cout << "let's clean the mess..." << std::endl;
    if ( TmpSpectrum !=0 || TmpSpectrum !=NULL ) {
        delete TmpSpectrum;
    }
    if ( Analyzer !=0 || Analyzer !=NULL ) {
        delete Analyzer;
    }
    delete[] ptrArray_f;
    delete[] dest;

    return numberPeakFund;

}



void  AvgSpectrum ( TH1* Spectrum, TSegment * inData, unsigned int Start, int unsigned Stop=0 )
{



    if ( Stop==0 ) {
        Stop=inData->GetSampleCount();
    }
    unsigned int lenght= Stop-Start;
    // std::cout << lenght << std::endl;
    if ( lenght > inData->GetSampleCount() || Stop > inData->getArrSize() ) {
        std::cerr <<"[Error] : @AvgSpectrum: SubSegment limits are wrong:" << std::flush;
        std::cerr <<"\n\tSub Segment lenght =\t"       << lenght
                  <<"\n\tStart Segment lenght =\t "    << Start
                  <<"\n\tStop Sub Segment  =\t "       << Stop
                  << std::endl;
        return;
    }


    for ( int i=0; i <lenght; ++i ) {

        //std::cout << inData->GetSample ( Start+i ) << std::endl;
        Spectrum->Fill ( Start+i , inData->GetSample ( Start+i ) );

    }

    return;
}

void PeakFinder ( TH1* PeakSpectrum, TSegment * inData, Double_t down, Double_t up, unsigned int iBlanking, unsigned int Start, int unsigned Stop=0 )
{



    if ( Stop==0 ) {

        Stop=inData->GetSampleCount();
        std::cerr <<"[Warning] : @PeakFinder: SubSegment upper limits bigger"
                  <<" that Sample Numbers." << std::flush;
    }
    unsigned int lenght= Stop-Start;

    if ( lenght > inData->GetSampleCount() || Stop > inData->getArrSize() ) {

        std::cerr <<"[Error] : @PeakFinder: SubSegment limits are wrong:" << std::flush;
        std::cerr <<"\n\tSub Segment lenght =\t"     << lenght
                  <<"\n\tStart Segment lenght =\t "    << Start
                  <<"\n\tStop Sub Segment  =\t "       << Stop
                  << std::endl;
        return;
    }

    if ( up == down ) {

        std::cerr <<"[Error] : @PeakFinder: ThreadShold limit are equal to each other:" << std::flush;
        std::cerr <<"\n\tup limit =\t"     << up
                  <<"\n\tdown limit =\t "    << down
                  << std::endl;
        return;
    }

//      std::cerr << "THIS CODE will do someTHING!!!!!" << std::endl;


    if ( up > down ) {

        for ( int i=Start; i <Stop; ++i ) {

            if ( inData->GetSample ( i ) >= down ) {

                bool toBig=false;
                int j=i-1;

                while ( ++i   < Stop ) {
                    double DataTmp=inData->GetSample ( j );
                    if ( DataTmp > up ) {
                        toBig=true;
                    }

                    if ( DataTmp < down ) {
                        break;
                    }


                }
                if ( !toBig ) {
                    for (; i < j; ++i ) {
                        PeakSpectrum->Fill ( i , inData->GetSample ( i ) );
                    }
                } else {

                    i=j+iBlanking;
                }

            }






        }
    } else {
        for ( int i=Start; i <Stop; ++i ) {

            if ( inData->GetSample ( i ) <= down ) {

                bool toBig=false;
                int j=i-1;

                while ( ++j   < Stop ) {
                    double DataTmp=inData->GetSample ( j );
                    if ( DataTmp < up ) {
                        toBig=true;
                    }

                    if ( DataTmp > down ) {
                        break;
                    }


                }
               
                
//                 std::cout << "i,j: "<<i<<','<<j<< std::endl;
//                 std::cout << (toBig?"true":"false") << std::endl;
                if ( !toBig ) {
                    for ( ;i < j; ++i ) {
//                       std::cout << i << ' ' <<  inData->GetSample ( i ) << std::endl;
                      PeakSpectrum->Fill ( i , inData->GetSample ( i ) );
                    }
                } else {

                    i=j+iBlanking;
                }

            }






        }

    }


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



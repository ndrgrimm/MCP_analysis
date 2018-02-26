#include"tsegment.h"
#include<root/TSpectrum.h>
#include<root/TH1F.h>
#include<iostream>

void Analysis(TH1S* PeakNumber,TH1S* PeakAmplitude,TH1F* SpectrumFiltred,TH1F* Spectrum,TSegment * inData,uint NumberPeak, Double_t ThreadShold, unsigned int Start, int unsigned Stop=0){

    
    
    if (Stop==0) Stop=inData->GetSampleCount();
    unsigned int lenght= Stop-Start;
    // std::cout << lenght << std::endl;
    if ( lenght > inData->GetSampleCount() || Stop > inData->getArrSize() ) {
        return;
    }
    

    
    float*   ptrArray_f=new float[lenght];
    for( int i=0; i <lenght; ++i)
    {
        ptrArray_f[i]=-1*float(inData->GetSample(Start+i)); // change sign of data
        Spectrum->Fill(i,ptrArray_f[i]);
 
    }
    TSpectrum* Analyzer=new TSpectrum();
  
    float *destVec=new float[lenght];
    Analyzer->SearchHighRes(ptrArray_f,destVec,lenght,5, ThreadShold,kFALSE,3,kFALSE,1);
    //std::cout << "Numero picchi: " << Analyzer->GetNPeaks() << std::endl;
    PeakNumber->Fill(Analyzer->GetNPeaks());
    
    for( int i=0; i<Analyzer->GetNPeaks(); ++i)
    {
        PeakAmplitude->Fill(Analyzer->GetPositionY()[i]);
    }
    
    if (Analyzer->GetNPeaks() >= NumberPeak )
    {
        SpectrumFiltred->Fill(Analyzer->GetPositionX()[0],Analyzer->GetPositionY()[0]);
    }
    delete Analyzer;
    delete[] ptrArray_f;
    
}

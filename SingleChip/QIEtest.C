#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>

#include "TFile.h"
#include "TF1.h"
#include "TH1.h"
#include "TH2.h"
#include "TGraphErrors.h"
#include "TLegend.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TMultiGraph.h"
#include "TMath.h"
#include "TSystem.h"
#include "TStyle.h"

using namespace std;

//TString myname;
Char_t cTemp[500]; 
Char_t cTemp2[500];
Char_t cTemp3[500];

fstream file; //This is the iostream for reading the file

//Output HTML file for viewing results
ofstream OutHtml;
string cDirOut;

int PedRange[4][100];
int PedMant[4][100];
int PedCIDCount[4];

int PEDtestRange[64][129];
int PEDtestMant[64][129];
int CAPPEDtestRange[16][4][50];
int CAPPEDtestMant[16][4][50];
int CAPPEDtestCount[16][4];

double PedDac[63];
double PedAvg[63];
double PedStd[63];
double CAPPedDac[15];
double CAPPedAvg[4][15];
double CAPPedAvg_post[4][15];
double CAPPedStd[4][15];

double PedCIDMean[4];
double PedCIDRMS[4];

TH1F *Pedestal[4];

TGraphErrors *PedGraph;
TGraphErrors *CAPPedGraph_0;
TGraphErrors *CAPPedGraph_1;
TGraphErrors *CAPPedGraph_2;
TGraphErrors *CAPPedGraph_3;
TF1 *PedFit;
TF1 *CAPPedFit_0;
TF1 *CAPPedFit_1;
TF1 *CAPPedFit_2;
TF1 *CAPPedFit_3;
TGraphErrors *CAPPedGraph_pre0;
TGraphErrors *CAPPedGraph_pre1;
TGraphErrors *CAPPedGraph_pre2;
TGraphErrors *CAPPedGraph_pre3;
TLegend *pedLeg;

int UpDAQ[4][4][64];            // initializing arrays as integers
int DownDAQ[4][4][64];

int TDCtestRead[255][4][22];
int TDCCapID[255][4][22];
int TDC[255][4][22];
int TDCRange[255][4][22];
int TDCMant[255][4][22];
double TDCCharge[255][4][22];

double Up_work[4][4][64];
double Down_work[4][4][64];

double Up[4][4][63];
double Down[4][4][63];

// DCH: dummy array for plotting:
double index_[4][62];
double index_Up[4][63];
double index_Dn[4][63];

// DCH: Analysis vectors determined from input vectors
double Mid[4][4][62];  
double Width[4][4][62];

bool isRange01;

TCanvas* c1;

TGraph* RangePlot[4][4];
TGraph* RangePlot_Up[4][4];
TGraph* RangePlot_Dn[4][4];

TGraph* RangePlot_stagger[4][4];
TGraph* RangePlot_Up_stagger[4][4];
TGraph* RangePlot_Dn_stagger[4][4];

TCanvas *Ranges[4];
TCanvas *SubRanges[4];

TMultiGraph* allRanges[4];
TMultiGraph* allRanges_Up[4];
TMultiGraph* allRanges_Dn[4];

TF1 *SubRangeFit[4][4][4];
TH1F* BinWidths[4][4][4];
TH1F* BinWidths_Mid[4][4][4];

TH1F* DNL_ADC[4][4];
TH1F* DNL_allADC;
TH1F* DNL_ADCTest;
TH1F* DNL_ADCBest;
TH1F* DNL_ADCAvg;

TCanvas* WidthsMid[4][4];

TGraph* tdcGraph;
TGraph* tdcGraphOff;

TGraph* BinSum2vDelay;
TGraph* BinSum3vDelay;
TGraph* BinSum4vDelay;
TGraph* BinSum5vDelay;

TH1F* BinSum2;
TH1F* BinSum3;
TH1F* BinSum4;
TH1F* BinSum5;

TF1* TDCFit;
TH1F* TDCOffset;

TGraph* Slice[22];
TMultiGraph* AllSlice;

TH1F* Pulse[255];

TLegend* myLeg;

// forward-declarations
bool storePed(fstream &file);
void saveFile(TCanvas *c1,const string& cStrSave, const string& cHeader, double width = 200, bool table = kTRUE );
bool storeADC(fstream &file);
bool storeTDC(fstream &file);
double LookupDAQ(int DAC, int whichRange);
double LookupCharge(int range, int mantissa);


//This function is used to read data from the file opened file
//It makes the assumption that every line contains two columns
//The first contains the line number, this increments by one
//The second contains some data of interest.
//The function checks that the data follows this format, if it does, it returns false
bool readline(fstream &file, int &linenumber, string &data, int &previousline){
  file >> dec >> linenumber;
  //file >> linenumber;
  file >> data;

  //if(linenumber != (previousline + 1)){
    //#ifdef debugstore
    //cout << "Expected :"<<(previousline+1)<<" Read: "<<linenumber << endl;
    //#endif
    //return 0;
  //}
  
  //previousline = linenumber;
  
  return true;
}

bool readline_cap(fstream &file, int &linenumber, string &data, int &previousline){
 
  string junk;

  file >> dec >> linenumber;
  file >> junk;
  file >> data;

  //if(linenumber != (previousline + 1)){
    //#ifdef debugstore
    //cout << "Expected :"<<(previousline+1)<<" Read: "<<linenumber << endl;
    //#endif
    //return 0;
  //}
  
  previousline = linenumber;
  
  return true;
}


void doPedTest(char* myfile, const string& cPathOut = "output/newPed") {
  
  //Set output path for saving multiple chips
  cDirOut = cPathOut;

  //DCH: Start with reading in the file to produce the data arrays:
  file.open(myfile, ios::in);
  
  if(!file) {
    cout << "Cannot open file!"<<endl;
    exit(1);
  }
  
  if(!storePed(file)) {
    cout << "The data file was not successfully read" << endl;
    exit(1);
  }
  
  file.close();

  //DCH setup the ouput file
  gSystem->mkdir(cDirOut.c_str(), kTRUE);
  sprintf(cTemp,"%s/index.html",cDirOut.c_str());
  OutHtml.open(cTemp);
  
  c1 = new TCanvas("c1","",900,900);

  OutHtml << "<HTML>" << endl;
  OutHtml << "<Body>" << endl;
  
  OutHtml << "<H2 style=\"background color: #999999;\">" << endl;
  OutHtml << "Input File: " << myfile << endl;
  OutHtml << "</H2>" << endl;
  OutHtml << "<H2 style=\"background color: #999999;\">" << endl;
  OutHtml << "QIE Viewer, Chip #(NOT YET IMPLEMENTED)" << endl;
  OutHtml << "</H2>" << endl;

  OutHtml << "<h3> Pedestal Study </h3>" << endl;

  OutHtml << "<table cellspacing=\"\" cellpadding='3' border=\"\" align=\"center\" >" << endl;
  OutHtml << "" << endl;
  OutHtml << "" << endl;
  OutHtml << "<TR align='center'> <TD align='center' colspan=5><b> Pedestals by CapID </b></TD></TR>"<< endl;
  OutHtml << "<TR align='center'>" << endl;
  OutHtml << "<TD align='center'> CapID </TD><TD align='center'> 0 </TD><TD align='center'> 1 </TD><TD align='center'> 2 </TD><TD align='center'> 3 </TD></TR>" << endl;
  OutHtml << "<TR align='center'>" << endl;
  OutHtml << "<TD align='center'>Counts</TD>" << endl;
  
  for( int i=0; i<4; i++) {
    
    sprintf(cTemp,"PedCID%d",i);
    Pedestal[i] = new TH1F(cTemp,"",15, 0,15);
    
    for (int j=0; j< PedCIDCount[i]; j++){ 
      if(PedRange[i][j] != 0) {
	cout << "Range 1 Ped ERROR" << endl;
	Pedestal[i]->Fill(-99);
      }
      else {
	Pedestal[i]->Fill(PedMant[i][j]);
	PedCIDMean[i] += PedMant[i][j];  
      }
    }
    
    PedCIDMean[i] /= PedCIDCount[i]; 
    
    for (int k=0; k< PedCIDCount[i]; k++) {
      if(PedRange[i][k] == 0) {
	PedCIDRMS[i] += pow((PedMant[i][k] - PedCIDMean[i]),2);
      }
      else cout << "Range 1 Ped ERROR" << endl;
    }
    
    PedCIDRMS[i] /= PedCIDCount[i];
    PedCIDRMS[i] = sqrt(PedCIDRMS[i]);
    
    OutHtml << "<TD align='center'>"<< PedCIDCount[i]  << "</TD>" << endl;
    
  }
  OutHtml << "</TR>" << endl;
  OutHtml << "<TR align='center'>" << endl;
  OutHtml << "<TD align='center'>Pedestal</TD>" << endl;
  
  for(int i=0; i<4; i++){
    OutHtml << "<TD align=center>" << fixed<< setprecision(3) << PedCIDMean[i] << " +/- " << PedCIDRMS[i] << "</TD>"<< endl;
  }
  
  OutHtml << "</table> " << endl;
  
  OutHtml << "<h3> Global Pedestal DAC Study </h3>" << endl;
  
  for(int ped = 1; ped<64; ped++) {
    if(ped > 31) PedDac[ped-1] = ped-32;
    else {
      PedDac[ped-1] = -1 * (ped & 31);
    }
    
    //DCH, just added this
    PedAvg[ped-1] = 0;

    for(int read=0; read<129; read++) {
      PedAvg[ped-1] += PEDtestRange[ped][read]*64 + PEDtestMant[ped][read];
    }
    PedAvg[ped-1] /= 129;
    
    PedStd[ped-1] = 0;
    for(int read=0; read<129; read++) {
      PedStd[ped-1] += pow((PEDtestRange[ped][read]*64 + PEDtestMant[ped][read]) - PedAvg[ped-1],2);
    }
    PedStd[ped-1] /= 129;
    PedStd[ped-1] = sqrt(PedStd[ped-1]);

    if(PedStd[ped-1] == 0) PedStd[ped-1] = 1/sqrt(12);
  }
  
  for(int ped = 0; ped < 15; ped++) {
    if((ped+1) > 7) CAPPedDac[ped] = (ped+1) - 8;
    else CAPPedDac[ped] = -1 * ((ped+1) & 7);
    
    for(int i= 0; i <4; i++) {
      
      for(int read=0; read<CAPPEDtestCount[ped][i]; read++) {
	CAPPedAvg[i][ped] += CAPPEDtestRange[ped][i][read]*64 + CAPPEDtestMant[ped][i][read];
      }
      CAPPedAvg[i][ped] /= CAPPEDtestCount[ped][i];
      
      CAPPedStd[i][ped] = 0;
      for(int read=0; read<CAPPEDtestCount[ped][i]; read++) {
	CAPPedStd[i][ped] += pow((CAPPEDtestRange[ped][i][read]*64 + CAPPEDtestMant[ped][i][read]) - CAPPedAvg[i][ped],2);
      }
      CAPPedStd[i][ped] /= CAPPEDtestCount[ped][i];
      CAPPedStd[i][ped] = sqrt(CAPPedStd[i][ped]);
      
      if(CAPPedStd[i][ped] == 0) CAPPedStd[i][ped] = 1/sqrt(12);

    }
  }
  
  PedGraph = new TGraphErrors(63, PedDac, PedAvg, NULL, PedStd);

  CAPPedGraph_0 = new TGraphErrors(15, CAPPedDac, CAPPedAvg[0], NULL, CAPPedStd[0]);
  CAPPedGraph_1 = new TGraphErrors(15, CAPPedDac, CAPPedAvg[1], NULL, CAPPedStd[1]);
  CAPPedGraph_2 = new TGraphErrors(15, CAPPedDac, CAPPedAvg[2], NULL, CAPPedStd[2]);
  CAPPedGraph_3 = new TGraphErrors(15, CAPPedDac, CAPPedAvg[3], NULL, CAPPedStd[3]);
  
  PedGraph->SetMarkerStyle(21);
  CAPPedGraph_0->SetMarkerStyle(21); 
  CAPPedGraph_1->SetMarkerStyle(21); 
  CAPPedGraph_2->SetMarkerStyle(21); 
  CAPPedGraph_3->SetMarkerStyle(21); 

  PedGraph->SetMarkerColor(kBlue);

  CAPPedGraph_0->SetMarkerColor(kBlue); 
  CAPPedGraph_1->SetMarkerColor(kBlue); 
  CAPPedGraph_2->SetMarkerColor(kBlue); 
  CAPPedGraph_3->SetMarkerColor(kBlue); 

  PedGraph->SetTitle("Pedestal DAC Test; Global Pedestal DAC; Pedestal (mantissa)");
  CAPPedGraph_0->SetTitle("CapID 0 Pedestal DAC Test; CapID Pedestal DAC; Pedestal (mantissa)");
  CAPPedGraph_1->SetTitle("CapID 1 Pedestal DAC Test; CapID Pedestal DAC; Pedestal (mantissa)");
  CAPPedGraph_2->SetTitle("CapID 2 Pedestal DAC Test; CapID Pedestal DAC; Pedestal (mantissa)");
  CAPPedGraph_3->SetTitle("CapID 3 Pedestal DAC Test; CapID Pedestal DAC; Pedestal (mantissa)");

  TF1* PedFitLo = new TF1("pedFitLo","pol1",-21,-9);
  PedFitLo->SetLineColor(kRed);
  TF1* PedFitHi = new TF1("pedFitHi","pol1",6, 18);
  PedFitHi->SetLineColor(kRed);

  PedGraph->Fit(PedFitLo,"RQ+");
  PedGraph->Fit(PedFitHi,"RQ+");

  c1->cd();
  PedGraph->Draw("AP");  
  
  OutHtml << "<table cellspacing=\"\" cellpadding='3' border=\"\" align=\"center\" >" << endl;
  OutHtml << "" << endl;
  OutHtml << "" << endl;
  OutHtml << "<TR align='center'> <TD align='center' colspan=2><b> Global Pedestals DAC </b></TD></TR>"<< endl;
  OutHtml << "<TR align='center'><TD></TD>" << endl;
  saveFile(c1," PedestalDac", "", 400);    
  OutHtml << "</TR>" << endl;
  OutHtml << "<TR align='center'><TD> Slope Lo</TD><TD>" << PedFitLo->GetParameter(1) << " +/- " << PedFitLo->GetParError(1) << "</TD></TR>" << endl;
  OutHtml << "<TR align='center'><TD> ~fC/bin (target: ~3)</TD><TD>" << 2/PedFitLo->GetParameter(1) << "</TD></TR>" << endl;
  OutHtml << "<TR align='center'><TD> Slope Hi</TD><TD>" << PedFitHi->GetParameter(1) << " +/- " << PedFitHi->GetParError(1) << "</TD></TR>" << endl;
  OutHtml << "<TR align='center'><TD> ~fC/bin (target: ~3)</TD><TD>" << 2/PedFitHi->GetParameter(1) << "</TD></TR>" << endl;
  OutHtml << "</table>" << endl;
  
  CAPPedFit_0 = new TF1("CAPPedFit_0","pol1",-3, 7);
  CAPPedFit_1 = new TF1("CAPPedFit_1","pol1",-3, 7);
  CAPPedFit_2 = new TF1("CAPPedFit_2","pol1",-3, 7);
  CAPPedFit_3 = new TF1("CAPPedFit_3","pol1",-3, 7);

  CAPPedFit_0->SetLineColor(kRed);
  CAPPedFit_1->SetLineColor(kRed);
  CAPPedFit_2->SetLineColor(kRed);
  CAPPedFit_3->SetLineColor(kRed);

  CAPPedGraph_0->Fit(CAPPedFit_0,"RQ+");
  CAPPedGraph_1->Fit(CAPPedFit_1,"RQ+");
  CAPPedGraph_2->Fit(CAPPedFit_2,"RQ+");
  CAPPedGraph_3->Fit(CAPPedFit_3,"RQ+");

  OutHtml << "<h3> CapID Pedestal DAC Study </h3>" << endl;

  OutHtml << "<table cellspacing=\"\" cellpadding='3' border=\"\" align=\"center\" >" << endl;
  OutHtml << "" << endl;
  OutHtml << "" << endl;
  OutHtml << "<TR align='center'> <TD align='center' colspan=5><b> CapID Pedestal DAC </b></TD></TR>"<< endl;
  OutHtml << "<TR align='center'><TD align='center'> CapId </TD><TD align='center'> 0 </TD><TD align='center'> 1 </TD><TD align='center'> 2 </TD><TD align='center'> 3 </TD>" << endl;
  OutHtml << "<TR align='center'><TD></TD>";
  c1->cd();
  CAPPedGraph_0->Draw("AP");
  saveFile(c1,"CapPedDAC0","",300);

  CAPPedGraph_1->Draw("AP");
  saveFile(c1,"CapPedDAC1","",300);  

  CAPPedGraph_2->Draw("AP");
  saveFile(c1,"CapPedDAC2","",300);

  CAPPedGraph_3->Draw("AP");
  saveFile(c1,"CapPedDAC3","",300);

  OutHtml << "</TR>" << endl;
  OutHtml << "<TR align='center'><TD> Slope (target: ~.6)</TD><TD>" << CAPPedFit_0->GetParameter(1) << "</TD><TD>" << CAPPedFit_1->GetParameter(1) << "</TD><TD>" << CAPPedFit_2->GetParameter(1) << "</TD><TD>" << CAPPedFit_3->GetParameter(1) << "</TD>";
  OutHtml << "</TR>" << endl;
  OutHtml << "<TR align='center'><TD> ~fC/bin (target: ~3)</TD><TD>" << 1.9/CAPPedFit_0->GetParameter(1) << "</TD><TD>" << 1.9/CAPPedFit_1->GetParameter(1) << "</TD><TD>" << 1.9/CAPPedFit_2->GetParameter(1) << "</TD><TD>" << 1.9/CAPPedFit_3->GetParameter(1) << "</TD>";
  OutHtml << "</TR>" << endl;

  OutHtml <<"</table>" << endl;
  OutHtml << "" << endl;
  OutHtml << "" << endl;

  CAPPedGraph_pre0 = new TGraphErrors(14, CAPPedDac, CAPPedAvg[0], NULL, CAPPedStd[0]);
  CAPPedGraph_pre1 = new TGraphErrors(14, CAPPedDac, CAPPedAvg[1], NULL, CAPPedStd[1]);
  CAPPedGraph_pre2 = new TGraphErrors(14, CAPPedDac, CAPPedAvg[2], NULL, CAPPedStd[2]);
  CAPPedGraph_pre3 = new TGraphErrors(14, CAPPedDac, CAPPedAvg[3], NULL, CAPPedStd[3]);
  
  CAPPedGraph_pre0->SetMarkerStyle(21); 
  CAPPedGraph_pre1->SetMarkerStyle(22); 
  CAPPedGraph_pre2->SetMarkerStyle(23); 
  CAPPedGraph_pre3->SetMarkerStyle(24); 
  
  CAPPedGraph_pre0->SetMarkerColor(kBlack); 
  CAPPedGraph_pre1->SetMarkerColor(kBlue); 
  CAPPedGraph_pre2->SetMarkerColor(kRed); 
  CAPPedGraph_pre3->SetMarkerColor(kGreen); 

  CAPPedGraph_pre0->SetMinimum(0);
  CAPPedGraph_pre1->SetMinimum(0);  
  CAPPedGraph_pre2->SetMinimum(0);
  CAPPedGraph_pre3->SetMinimum(0);
 
  CAPPedGraph_pre0->SetMaximum(12);
  CAPPedGraph_pre1->SetMaximum(12);
  CAPPedGraph_pre2->SetMaximum(12);
  CAPPedGraph_pre3->SetMaximum(12);

  CAPPedGraph_pre0->SetTitle("CapID Pedestal DAC Test; CapID Pedestal DAC; Pedestal (mantissa)");

  pedLeg = new TLegend(0.15, 0.7, 0.35, 0.9);
  pedLeg->AddEntry(CAPPedGraph_pre0, "CapID 0", "P");
  pedLeg->AddEntry(CAPPedGraph_pre1, "CapID 1", "P");
  pedLeg->AddEntry(CAPPedGraph_pre2, "CapID 2", "P");
  pedLeg->AddEntry(CAPPedGraph_pre3, "CapID 3", "P");
  pedLeg->SetFillColor(0);
  
  for(int i=0; i<4; i++) {
    for(int j=0; j<14; j++) {
      CAPPedAvg_post[i][j] = CAPPedAvg[i][j] + (PedCIDMean[0] - PedCIDMean[i]) ;
    }
  }
  
  TGraphErrors *CAPPedGraph_post0 = new TGraphErrors(14, CAPPedDac, CAPPedAvg_post[0], NULL, CAPPedStd[0]);
  TGraphErrors *CAPPedGraph_post1 = new TGraphErrors(14, CAPPedDac, CAPPedAvg_post[1], NULL, CAPPedStd[1]);
  TGraphErrors *CAPPedGraph_post2 = new TGraphErrors(14, CAPPedDac, CAPPedAvg_post[2], NULL, CAPPedStd[2]);
  TGraphErrors *CAPPedGraph_post3 = new TGraphErrors(14, CAPPedDac, CAPPedAvg_post[3], NULL, CAPPedStd[3]);
  
  CAPPedGraph_post0->SetMarkerStyle(21); 
  CAPPedGraph_post1->SetMarkerStyle(22); 
  CAPPedGraph_post2->SetMarkerStyle(23); 
  CAPPedGraph_post3->SetMarkerStyle(24); 
  
  CAPPedGraph_post0->SetMarkerColor(kBlack); 
  CAPPedGraph_post1->SetMarkerColor(kBlue); 
  CAPPedGraph_post2->SetMarkerColor(kRed); 
  CAPPedGraph_post3->SetMarkerColor(kGreen); 

  CAPPedGraph_post0->SetMinimum(0);
  CAPPedGraph_post1->SetMinimum(0);  
  CAPPedGraph_post2->SetMinimum(0);
  CAPPedGraph_post3->SetMinimum(0);
 
  CAPPedGraph_post0->SetMaximum(12);
  CAPPedGraph_post1->SetMaximum(12);
  CAPPedGraph_post2->SetMaximum(12);
  CAPPedGraph_post3->SetMaximum(12);

  CAPPedGraph_post0->SetTitle("CapID Pedestal DAC Test; CapID Pedestal DAC; Pedestal (mantissa)");

  OutHtml << "<h3> CapID Pedestal Correction Study </h3>" << endl;

  OutHtml << "<table cellspacing=\"\" cellpadding='3' border=\"\" align=\"center\" >" << endl;
  OutHtml << "" << endl;
  OutHtml << "" << endl;
  OutHtml << "<TR align='center'> <TD align='center' colspan=2><b> CapID Pedestal Tune </b></TD></TR>"<< endl;
  OutHtml << "<TR>";

  c1->cd();
  CAPPedGraph_pre0->Draw("AP");
  CAPPedGraph_pre1->Draw("P");
  CAPPedGraph_pre2->Draw("P");
  CAPPedGraph_pre3->Draw("P");
  pedLeg->Draw("SAME");

  saveFile(c1,"CAPPedpre","Before",400);
  
  c1->cd();
  CAPPedGraph_post0->Draw("AP");
  CAPPedGraph_post1->Draw("P");
  CAPPedGraph_post2->Draw("P");
  CAPPedGraph_post3->Draw("P");
  pedLeg->Draw("SAME");

  saveFile(c1,"CAPPedpost","After",400);

  OutHtml << "</TR></table>" << endl;  
  OutHtml << "" << endl;
  OutHtml << "" << endl;

  OutHtml.close();
}


void doADCTest(char* myfile, const string& cPathOut="output/newADC") {
  
  //Set output path for saving multiple chips
  cDirOut = cPathOut;

  //DCH: Start with reading in the file to produce the data arrays:
  file.open(myfile, ios::in);
  
  if(!file) {
    cout << "Cannot open file!"<<endl;
    exit(1);
  }
  
  if(!storeADC(file)) {
    cout << "The data file was not successfully read" << endl;
    exit(1);
  }
  
  file.close();
  
  // DCH: The input arrays of last DAQ reading to get each mantissa while ramping Up/Down
  //        Array is for [CapId(0-3)][Range(0-3)][Mantissa(0-63)]
    
  //DCH debug to print out UP and Down vecs
  //for (int i=0; i<4; i++){
  //for (int j=0; j<4; j++) {
  //for (int k=0; k<64; k++) {
  //cout << "CapId " << i << ", R " << j << ", M " << k << " Up: " << UpDAQ[i][j][k] << ", Down: " << DownDAQ[i][j][k] << endl;
  //}
  //}
  //}

  //Now, we use the lookup table to convert DAQ setting to charge
  for (int i=0; i<4; ++i){ // capid
    for (int j=0; j<4; ++j) { // range 
      for (int k=0; k<64; ++k) { // bin
	
	Up_work[i][j][k] = LookupDAQ(UpDAQ[i][j][k],j);
	Down_work[i][j][k] = LookupDAQ(DownDAQ[i][j][k],j);

      }
    }
  }
  
  // DCH: loop here to fill analysis vectors
  // DCH: We use the "work" vectors because the last bin of 
  //      ramping down and the first bin of ramping up is garbage.
  //      Up and Down will be 63 element vectors and Mid/Width's will be 62.
  for(int k=0; k<4; ++k) {
    for(int j=0; j<4; ++j) {
      for(int i = 0; i < 63; ++i) {
	if(k==0) {
	  if(i<62) index_[j][i] = i+(64*j)+1;
	  index_Up[j][i] = i+(64*j)+1;
	  index_Dn[j][i] = i+(64*j);
	}

	Up[k][j][i] = Up_work[k][j][i+1];
	Down[k][j][i] = Down_work[k][j][i];

	if(i<62) {
	  Mid[k][j][i] = (Up_work[k][j][i+1] + Down_work[k][j][i+1])/2;
	  Width[k][j][i] = Down_work[k][j][i+1] - Up_work[k][j][i+1];
	}
      }
    }
  }

  //DCH setup the ouput file
  gSystem->mkdir(cDirOut.c_str(), kTRUE);
  sprintf(cTemp,"%s/index.html",cDirOut.c_str());
  OutHtml.open(cTemp);

  OutHtml << "<HTML>" << endl;
  OutHtml << "<Body>" << endl;

  OutHtml << "<H2 style=\"background color: #999999;\">" << endl;
  OutHtml << "Input File: " << myfile << endl;
  OutHtml << "</H2>" << endl;
  OutHtml << "<H2 style=\"background color: #999999;\">" << endl;
  OutHtml << "QIE Viewer, Chip #(NOT YET IMPLEMENTED)" << endl;
  OutHtml << "</H2>" << endl;

  OutHtml << "<h3> Range Study </h3>" << endl;

  // DCH: Now, plot the range for each CapID

  c1 = new TCanvas("c1","",900,900);
  c1->SetLeftMargin(0.15);

  for(int  i=0; i<4; ++i) {
    for(int j=0; j<4; ++j) {
      RangePlot[i][j] = new TGraph(62, index_[0], Mid[i][j]);
      RangePlot_Up[i][j] = new TGraph(63, index_Up[0], Up[i][j]);
      RangePlot_Dn[i][j] = new TGraph(63, index_Dn[0], Down[i][j]);

      RangePlot_stagger[i][j] = new TGraph(62, index_[j], Mid[i][j]);
      RangePlot_Up_stagger[i][j] = new TGraph(63, index_Up[j], Up[i][j]);
      RangePlot_Dn_stagger[i][j] = new TGraph(63,index_Dn[j], Down[i][j]);

      RangePlot[i][j]->SetMarkerStyle(33);
      RangePlot_Up[i][j]->SetMarkerStyle(23);
      RangePlot_Dn[i][j]->SetMarkerStyle(22);

      sprintf(cTemp,"CapID%d_Range%d; Mantissa; Charge (fC)",i,j);

      cout << "i: " << i << ", j: " << j << ", cTemp: " << cTemp << endl;
      RangePlot[i][j]->SetTitle(cTemp);
      
      RangePlot_stagger[i][j]->SetMarkerStyle(33);
      RangePlot_Up_stagger[i][j]->SetMarkerStyle(23);
      RangePlot_Dn_stagger[i][j]->SetMarkerStyle(22);

      RangePlot_Up[i][j]->SetMarkerColor(kBlue);
      RangePlot_Dn[i][j]->SetMarkerColor(kRed);

      RangePlot_Up_stagger[i][j]->SetMarkerColor(kBlue);
      RangePlot_Dn_stagger[i][j]->SetMarkerColor(kRed);
    }
  }

  for(int i=0; i<4; ++i) {
    allRanges[i] = new TMultiGraph();
    allRanges_Up[i] = new TMultiGraph();
    allRanges_Dn[i] = new TMultiGraph(); 
  }

  for(int j=0; j<4; ++j){
    for(int i=0; i<4; ++i) {
      allRanges[j]->Add(RangePlot_stagger[j][i]);
      allRanges_Up[j]->Add(RangePlot_Up_stagger[j][i]);
      allRanges_Dn[j]->Add(RangePlot_Dn_stagger[j][i]);

      if(j==0) {
	sprintf(cTemp,"CapID%d; ; Charge (fC)",i);
	allRanges[i]->SetTitle(cTemp);
      } 
   }
  }

  for(int i=0; i<4; ++i) {
    sprintf(cTemp, "CapID%d_FullRange", i);
    Ranges[i] = new TCanvas(cTemp, cTemp, 900,900);
    sprintf(cTemp, "CapID%d_Ranges", i);
    SubRanges[i] = new TCanvas(cTemp, cTemp, 900, 900);
  }

  OutHtml << "<table cellspacing=\"\" cellpadding=\"\" border=\"\" align=\"center\" >" << endl;
  OutHtml << "" << endl;
  OutHtml << "" << endl;
  OutHtml << "<TR align='center'> <TD align='center' colspan=5><b> Full Range View </b></TD></TR>"<< endl;
  OutHtml << "<TR align='center'>" << endl;
  OutHtml << "<TD align='center'> Full Range </TD>" << endl;
  

  for(int i=0; i<4; ++i) {
    Ranges[i]->cd();
    Ranges[i]->SetLeftMargin(0.15);
    allRanges[i]->Draw("AP");
    //cout << allRanges[i]->GetXaxis()->GetNbins() << endl;
    //cout << allRanges[i]->GetXaxis()->GetBinWidth(1) << endl;
    //cout << allRanges[i]->GetXaxis()->GetBinLowEdge(1) << endl;
    //cout << allRanges[i]->GetXaxis()->GetBinUpEdge(100) << endl;
    //allRanges[i]->GetYaxis()->SetOffset(0.1) << endl;
    //allRanges[i]->Draw("AP");

    allRanges[i]->GetYaxis()->SetTitleOffset(1.7);
    allRanges[i]->Draw("AP");

    //allRanges[i]->GetXaxis()->SetBinLabel(5,"R0 M0");
    //allRanges[i]->GetXaxis()->SetBinLabel(10,"R0 M16");
    //allRanges[i]->GetXaxis()->SetBinLabel(16,"R0 M32");
    //allRanges[i]->GetXaxis()->SetBinLabel(22,"R0 M48");
    //allRanges[i]->GetXaxis()->SetBinLabel(28,"R1 M0");
    //allRanges[i]->GetXaxis()->SetBinLabel(34,"R1 M16");
    //allRanges[i]->GetXaxis()->SetBinLabel(39,"R1 M32");
    //allRanges[i]->GetXaxis()->SetBinLabel(45,"R1 M48");
    //allRanges[i]->GetXaxis()->SetBinLabel(51,"R2 M0");
    //allRanges[i]->GetXaxis()->SetBinLabel(57,"R2 M16");
    //allRanges[i]->GetXaxis()->SetBinLabel(62,"R2 M32");
    //allRanges[i]->GetXaxis()->SetBinLabel(68,"R2 M48");
    //allRanges[i]->GetXaxis()->SetBinLabel(74,"R3 M0");
    //allRanges[i]->GetXaxis()->SetBinLabel(79,"R3 M16");
    //allRanges[i]->GetXaxis()->SetBinLabel(85,"R3 M32");
    //allRanges[i]->GetXaxis()->SetBinLabel(91,"R3 M48");
    //allRanges[i]->GetXaxis()->SetBinLabel(97,"R3 M63");
    //
    //allRanges[i]->GetXaxis()->SetNdivisions(405);

    //gPad->Modified();
    
    allRanges_Up[i]->Draw("P");
    allRanges_Dn[i]->Draw("P");
    allRanges[i]->Draw("P");

    sprintf(cTemp2,"FullRange_CapID%d",i);
    sprintf(cTemp3,"CapID %d",i);
    saveFile(Ranges[i],cTemp2,cTemp3);    

    SubRanges[i]->Divide(2,2);
    for(int j=0; j<4; ++j){
      SubRanges[i]->cd(j+1);
      RangePlot[i][j]->Draw("AP");
      RangePlot_Up[i][j]->Draw("P");
      RangePlot_Dn[i][j]->Draw("P");
      RangePlot[i][j]->Draw("P");
    }
  }
  OutHtml << "</TR>" << endl;
  OutHtml << "<TR align='center'> <TD align='center' colspan=5><b> Individual Range View </b></TD></TR>"<< endl;

  c1->cd();
  for (int j=0; j<4; ++j) {
    OutHtml << "<TR align='center'> <TD align='center'> Range " << j << "</TD>" << endl;
    for(int i=0; i<4; ++i) {
      RangePlot[i][j]->GetHistogram()->SetTitleOffset(1.8,"Y");

      RangePlot[i][j]->Draw("AP");
      RangePlot_Up[i][j]->Draw("P");
      RangePlot_Dn[i][j]->Draw("P");
      RangePlot[i][j]->Draw("P");
      
      sprintf(cTemp2,"Range%d_CapID%d",j,i);
      sprintf(cTemp3,"CapID %d",i);
      saveFile(c1,cTemp2,cTemp3);

    }
    OutHtml << "</TR>" << endl;
  }
  OutHtml << "<TR align='center'> <TD align='center' colspan=5><b> Individual Range w/ Fits </b></TD></TR>"<< endl;
  
  
  // DCH: Now lets fit the slopes and intercepts:

  double minFit[4] = {1,16,36,57};
  //double minFit[4] = {4,16,36,57};

  double maxFit[4] = {15,35,56,62};
  //double maxFit[4] = {15,35,56,61};

  for(int i =0; i<4; i++) {
    for(int j =0; j<4; j++) {
      for(int k =0; k<4; k++) {

	sprintf(cTemp,"Fit_CID%d_r%d_s%d",i,j,k);
	SubRangeFit[i][j][k] = new TF1(cTemp,"pol1", minFit[k], maxFit[k]);

	RangePlot[i][j]->Fit(SubRangeFit[i][j][k], "RQ+");
      }
    }
  }

  c1->cd();
  for (int j=0; j<4; j++) {
    OutHtml << "<TR align='center'> <TD align='center'> Range " << j << "</TD>" << endl;
    for(int i=0; i<4; i++) {
      RangePlot[i][j]->Draw("AP");
      RangePlot_Up[i][j]->Draw("P");
      RangePlot_Dn[i][j]->Draw("P");
      RangePlot[i][j]->Draw("P");
      
      sprintf(cTemp2,"Range%d_CapID%d_wFit",j,i);
      sprintf(cTemp3,"CapID %d",i);
      saveFile(c1,cTemp2,cTemp3);

    }
    OutHtml << "</TR>" << endl;
  }
  OutHtml << "</table>" << endl;

  //DCH: Ok, now output the slope and intercept results:

  //DCH here is the code to save file
  //sprintf(cTemp2,"PlotDataMCComp_%s_%s",cHVar[l],cCuts[k]);
  //sprintf(cTemp3,"%s",cHVar[l]);
  //saveFile(c1,cTemp2,cTemp3);


  OutHtml << "<h3> Slope and Intercept Study </h3>" << endl;
  OutHtml << "<table cellspacing=\"\" cellpading='3' border=\"\" align=\"center\" >" << endl;
  OutHtml << "" << endl;
  
  OutHtml << "<TR align='center'>"<< endl;
  OutHtml << "<TD align='center'></TD>";
  OutHtml << "<TD colspan=4 align='center'> &chi;^2/nDOF </TD><TD colspan=4 align='center'> Slope </TD> <TD colspan=4 align='center'> Intercept </TD></TR>" << endl;
  OutHtml << "<TR>" << endl;
  
  OutHtml << "<TD>CapId</TD>";
  for(int i=0; i<3;i++) {
    OutHtml << "<TD align='center'>0</TD><TD align='center'>1</TD><TD align='center'>2</TD><TD align='center'>3</TD>";
  }
  OutHtml << "</TR>" << endl;
  
  for(int i=0; i<4; i++) {
    for(int j=0; j<4; j++) {
      OutHtml << "<TR align='center'><TD align='center'> R " << i << ", Sub " << j << "</TD>";
      for(int k=0;k<4;k++) {
	OutHtml << "<TD>" << fixed << setprecision(2) << SubRangeFit[k][i][j]->GetChisquare() << "/" << setprecision(0) << SubRangeFit[k][i][j]->GetNDF() << "</TD>";
      }
      for(int k=0;k<4;k++) {
	OutHtml << "<TD>" << fixed << setprecision(2) << SubRangeFit[k][i][j]->GetParameter(1) << " +/- " << SubRangeFit[k][i][j]->GetParError(1) << "</TD>";
      }
      for(int k=0;k<4;k++) {
	OutHtml << "<TD>" << fixed << setprecision(1) << SubRangeFit[k][i][j]->GetParameter(0) << " +/- " << SubRangeFit[k][i][j]->GetParError(0) << "</TD>";
      }
      OutHtml << "</TR>" << endl;
    }
  }
  OutHtml << "</table> " << endl;

  /*
  cout << "********************************************************************" << endl;
  cout << "********************************************************************" << endl;
  cout << "****************      SLOPES AND INTERCEPTS     ********************" << endl;
  cout << "********************************************************************" << endl;
  cout << "********************************************************************" << endl;

  cout << "*************************"<< endl;
  cout << "***  Chi2/nDOF Table  ***" << endl;
  cout << "*************************"<< endl;

  const char separator = ' ';
  cout << setw(16) << std::setfill(separator) << "Range/Subrange ";
  cout << setw(16) << std::setfill(separator) << "CapID0";
  cout << setw(16) << std::setfill(separator) << "CapID1";
  cout << setw(16) << std::setfill(separator) << "CapID2";
  cout << setw(16) << std::setfill(separator) << "CapID3" << endl;
  for (int i =0; i<4; i++) {
    for(int j=0; j<4; j++) {
      cout << "Range " << i << ", SubR " << j << ":";
      
      for(int k=0; k<4; k++) {
	cout << fixed << setprecision(1) << setw(16) << SubRangeFit[k][i][j]->GetChisquare() << "/" << SubRangeFit[k][i][j]->GetNDF() << " ";
      }
      cout << endl;
    }
  }
  cout << "**********************"<< endl;
  cout << "***  Slopes Table  ***" << endl;
  cout << "**********************"<< endl;

  cout << "Range/Subrange " << "CapID0" << " CapID1 CapID2 CapID3" << endl;
  for (int i =0; i<4; i++) {
    for(int j=0; j<4; j++) {
      cout << "Range " << i << ", SubR " << j << ":";
      
      for(int k=0; k<4; k++) {
	cout << " " << SubRangeFit[k][i][j]->GetParameter(1) << " +/- " << SubRangeFit[k][i][j]->GetParError(1) << " ";
      }
      cout << endl;
    }
  }
  cout << "**************************"<< endl;
  cout << "***  Intercepts Table  ***" << endl;
  cout << "**************************"<< endl;

  cout << "Range/Subrange " << "CapID0" << " CapID1 CapID2 CapID3" << endl;
  for (int i =0; i<4; i++) {
    for(int j=0; j<4; j++) {
      cout << "Range " << i << ", SubR " << j << ":";
      
      for(int k=0; k<4; k++) {
	cout << " " << SubRangeFit[k][i][j]->GetParameter(0) << " +/- " << SubRangeFit[k][i][j]->GetParError(0) << " ";
      }
      cout << endl;
    }
  }
  */

  OutHtml << "<h3> Bin Width Study (endpoints) </h3>" << endl;
  //OutHtml << "<table cellspacing=\"\" cellpading=\"\" border=\"\" align=\"center\" >" << endl;
  //OutHtml << "" << endl;

  // DCH: Now we plot the bin widths
  
  // DCH: need to set optimization for each range/subrange
  double numbins[4][4] = {{20, 20, 20, 20},
			  {20, 20, 20, 20},
			  {20, 20, 20, 20},
			  {20, 20, 20, 20}};
  
  double minbin[4][4] = {{1.5, 3, 6, 12}, 
			 {12, 25, 50, 100}, 
			 {100, 200, 400, 800},
			 {800, 1600, 3000, 6000}};
  
  double maxbin[4][4] = {{4.5, 9, 18, 36},
			 {36, 75, 150, 300},
			 {300, 600, 1200, 2400},
			 {2400, 4800, 9000, 18000}};
  
 
  for(int i=0; i<4; i++) {
    for(int j=0; j<4; j++) {
      for(int k=0; k<4; k++) {

	sprintf(cTemp,"BinWidth_CapID%d_R%d_SR%d",i,j,k);
	BinWidths[i][j][k] = new TH1F(cTemp,"", numbins[j][k], minbin[j][k], maxbin[j][k]);

	sprintf(cTemp,"Bin Width, R%d_Sr%d; charge (fC); count",j,k);
	BinWidths[i][j][k]->SetTitle(cTemp);
	//BinWidths[i][j][k]->SetLabelOffset(0.1,"Y");

      }
    }
  }
  
 
  for(int i=0; i<4; i++) {
    for(int j=0; j<4; j++) {
      for(int k=0; k<62; k++) {
	if(k < 15) BinWidths[i][j][0]->Fill(Width[i][j][k]);
	else if(k < 35) BinWidths[i][j][1]->Fill(Width[i][j][k]);
	else if(k < 56) BinWidths[i][j][2]->Fill(Width[i][j][k]);
	//else if(k < 64) BinWidths[i][j][3]->Fill(Width[i][j][k]);
	else BinWidths[i][j][3]->Fill(Width[i][j][k]);

      }
    }
  }



  //TCanvas* Widths[4][4];
  
  //for(int i=0; i<4; i++){
  //for(int j=0; j<4; j++) {
  //sprintf(cTemp,"FullBinWidths_CapID%d_Range%d",i,j);
  //Widths[i][j] = new TCanvas(cTemp, cTemp);
  //}
  //}

  gStyle->SetOptStat(1111111);

  //for(int i=0; i<4; i++) {
  //OutHtml << "<TR align='center'> <TD align='center' colspan=5><b> Range " << i << "</b></TD></TR>" << endl;
  //  
  //for(int j=0;j<4; j++) {
  //OutHtml << "<TR align='center'> <TD align='center'> Range " << i << ", Sub " << j << "</TD>" << endl;
  //  
  //for(int k=0; k<4; k++) {
  //c1->cd();
  //BinWidths[k][i][j]->Draw();

  //sprintf(cTemp2,"CapID%d_Range%d_Sub%d",k,i,j);
  //sprintf(cTemp3,"CapID %d",k);
  //saveFile(c1,cTemp2,cTemp3);
  //
  //}
  //OutHtml << "</TR>" << endl;
  //}
  //}
  //OutHtml << "</table>" << endl;
  //OutHtml << "" << endl;
  //OutHtml << "" << endl;

  //DCH do DNL plot for bin widths:
  
  double average[4][4][4];
  for(int i=0; i<4; i++) {
    for(int j=0; j<4; j++) {
      for(int k=0; k<4; k++) {
	average[i][j][k] = 0;
      }
    }
  }
  
  for(int i=0; i<4; i++) {
    for(int j=0; j<4; j++) {
      for(int k=0; k<62; k++) {
	if(k < 15) average[i][j][0] += Width[i][j][k];
	else if(k < 35) average[i][j][1] += Width[i][j][k];
	else if(k < 56) average[i][j][2] += Width[i][j][k];
	else average[i][j][3] += Width[i][j][k];
      }
    }
  }  

  for(int i=0; i<4; i++) {
    for(int j=0; j<4; j++) {
      average[i][j][0] /= 15;
      average[i][j][1] /= 20;
      average[i][j][2] /= 21;
      average[i][j][3] /= 6;
      
    }
  }
  

  for(int i=0; i<4; i++) {
    for(int j=0; j<4; j++) {

      sprintf(cTemp,"BinWidthDNL_CapID%d_SR%d",i,j);
      DNL_ADC[i][j] = new TH1F(cTemp,"", 25, -1, 1);
      
      sprintf(cTemp,"ADC DNL, Bin Width EndP, Sr%d; DNL; count",j);
      DNL_ADC[i][j]->SetTitle(cTemp);
      DNL_ADC[i][j]->SetTitleOffset(1.2,"Y");


    }
  }
  
  DNL_allADC = new TH1F("BinWidthDNL_all","", 25, -1, 1);
  DNL_allADC->SetTitle("ADC DNL, All Range/Subranges; DNL; count");
  DNL_allADC->SetTitleOffset(1.3,"Y");

  DNL_ADCTest = new TH1F("DNL_ADCTest","ADC DNL Test",100,-1,1);
  DNL_ADCBest = new TH1F("DNL_ADCBest","ADC DNL Best",50,-0.5,0.5);
  DNL_ADCAvg  = new TH1F("DNL_ADCAvg","ADC DNL Avg",100,-1,1);

  double best=1000.;
  int pickRange=0;

  double avg = 0;

  for(int k=0; k<62; k++) {
    if(k<15) pickRange = 0;
    else if(k<35) pickRange = 1;
    else if(k<56) pickRange = 2;
    else pickRange = 3;

    best = 1000.;
    avg = 0;

    for(int j=1; j<3; j++) {
      for(int i=0; i<4; i++) {
	if(fabs(Width[i][j][k]/average[i][j][pickRange] - 1) < fabs(best)) {
	  best = Width[i][j][k]/average[i][j][pickRange] - 1;
	}
	//cout << "j=" << j << ", i=" << i << ", value= " << Width[i][j][k]/average[i][j][pickRange] - 1 << ", best = " << best << endl;

	  avg += Width[i][j][k]/average[i][j][pickRange] - 1;
	  
      }
    }

    avg /= 8;

    DNL_ADCBest->Fill(best);
    DNL_ADCAvg->Fill(avg);
  }

  for(int i=0; i<4; i++) {
    for(int j=1; j<3; j++) {
      for(int k=0; k<62; k++) {
	if(k < 15) {
	  DNL_ADC[i][0]->Fill(Width[i][j][k]/average[i][j][0] - 1);
	  DNL_allADC->Fill(Width[i][j][k]/average[i][j][0] - 1);
	  if(j<3) DNL_ADCTest->Fill(Width[i][j][k]/average[i][j][0] - 1);
	}
	else if(k < 35)  {
	  DNL_ADC[i][1]->Fill(Width[i][j][k]/average[i][j][1] - 1);
	  DNL_allADC->Fill(Width[i][j][k]/average[i][j][1] - 1);
	  if(j<3) DNL_ADCTest->Fill(Width[i][j][k]/average[i][j][1] - 1);

	}
	else if(k < 56) {
	  DNL_ADC[i][2]->Fill(Width[i][j][k]/average[i][j][2] - 1);
	  DNL_allADC->Fill(Width[i][j][k]/average[i][j][2] - 1);
	  if(j<3) DNL_ADCTest->Fill(Width[i][j][k]/average[i][j][2] - 1);

	}
	else {
	  DNL_ADC[i][3]->Fill(Width[i][j][k]/average[i][j][3] - 1);
	  DNL_allADC->Fill(Width[i][j][k]/average[i][j][3] - 1);
	  if(j<3) DNL_ADCTest->Fill(Width[i][j][k]/average[i][j][3] - 1);

	}
      }
    }
  }  
  
  

  OutHtml << "<table cellspacing=\"\" cellpading=\"\" border=\"\" align=\"center\" >" << endl;
  OutHtml << "" << endl;
  OutHtml << "<TR align='center'> <TD align='center' colspan=5><b> ADC Differential Non-Linearity </b></TD></TR>" << endl;
  
  for(int j=0;j<4; j++) {
    OutHtml << "<TR align='center'> <TD align='center'> Sub " << j <<"</TD>" << endl;
    
    for(int i=0; i<4; i++) {
      c1->cd();
      //DNL_ADC[i][j]->Draw();
      //DNL_ADCTest->Draw();
      if(i==0) DNL_ADCBest->Draw();
      else if(i==1) DNL_ADCAvg->Draw();
      else if(i==2) DNL_ADCTest->Draw();
      else DNL_ADCBest->Draw();
      
      sprintf(cTemp2,"DNL_ADC_CapID%d_Sub%d",i,j);
      sprintf(cTemp3,"CapID %d",i);
      saveFile(c1,cTemp2,cTemp3);
      
    }
    OutHtml << "</TR>" << endl;
  }
  OutHtml << "</table>" << endl;

  OutHtml << "<table cellspacing=\"\" cellpading=\"\" border=\"\" align=\"center\" >" << endl;
  OutHtml << "" << endl;
  OutHtml << "<TR align='center'> <TD align='center' colspan=1><b> ADC Differential Non-Linearity </b></TD></TR>" << endl;
  
  c1->cd();
  DNL_allADC->Draw();
  
  saveFile(c1,"DNL_ADC_AllCapID_AllRS","");

  OutHtml << "</TR>" << endl;
  OutHtml << "</table>" << endl;

 
  //DCH, ok, let's check range overlaps:
  OutHtml << "<h3> Range Overlap Study (endpoints) </h3>" << endl;
  OutHtml << "<table cellspacing=\"\" cellpading=\"\" border=\"\" align=\"center\" >" << endl;
  OutHtml << "" << endl;
  
  OutHtml << "<TR align='center'><TD align='center' colspan=5> Range Overlaps </TD></TR>" << endl;
  OutHtml << "<TR align='center'><TD align='center'></TD> <TD align='center'>CapID 0</TD><TD align='center'>CapID 1</TD><TD align='center'>CapID 2</TD><TD align='center'>CapID 3</TD></TR>" << endl;;
  
  for(int j=0; j<3; j++) {
    OutHtml << "<TR align='center'><TD align='center'> Range " << j << "-" << j+1 << " Overlap </TD>";
    for(int i=0; i<4; i++) {
      OutHtml << "<TD>" << fixed << setprecision(2) << (Down[i][j][62] - Up[i][j+1][0])/average[i][j][3] +2 << "</TD>";
    }
    OutHtml << "</TR>" << endl;
  }

  //DCH: Ok, do this for midpoints too.


  for(int i=0; i<4; i++) {
    for(int j=0; j<4; j++) {
      for(int k=0; k<4; k++) {

	sprintf(cTemp,"Bin Width Mid_CapID%d_R%d_SR%d",i,j,k);
	BinWidths_Mid[i][j][k] = new TH1F(cTemp,"", numbins[j][k], minbin[j][k], maxbin[j][k]);

	sprintf(cTemp,"Bin Width Mid, R%d_Sr%d; charge (fC); count",j,k);
	BinWidths_Mid[i][j][k]->SetTitle(cTemp);
      }
    }
  }
  
  double MidDiff[4][4][61];
  for(int j=0;j<4;j++) {
    for(int k=0;k<4;k++) {
      for(int i = 0; i<61; i++) MidDiff[j][k][i] = Mid[j][k][i+1]-Mid[j][k][i];
    }
  }

  for(int i=0; i<4; i++) {
    for(int j=0; j<4; j++) {
      for(int k=0; k<61; k++) {
	if(k < 13) BinWidths_Mid[i][j][0]->Fill(MidDiff[i][j][k]);
	else if(k < 33 && k > 15) BinWidths_Mid[i][j][1]->Fill(MidDiff[i][j][k]);
	else if(k < 54 && k > 35) BinWidths_Mid[i][j][2]->Fill(MidDiff[i][j][k]);
	//else if(k < 64) BinWidths[i][j][3]->Fill(Width[i][j][k]);
	else if(k>56) BinWidths_Mid[i][j][3]->Fill(MidDiff[i][j][k]);

      }
    }
  }

  
  for(int i=0; i<4; i++){
    for(int j=0; j<4; j++) {
      sprintf(cTemp,"MidBinWidths_CapID%d_Range%d",i,j);
      WidthsMid[i][j] = new TCanvas(cTemp, cTemp, 900, 900);
    }
  }

  for(int i=0; i<4; i++) {
    for(int j=0; j<4; j++) {
      WidthsMid[i][j]->Divide(2,2);
      for(int k=0; k<4; k++) {
	WidthsMid[i][j]->cd(k+1);
	BinWidths_Mid[i][j][k]->Draw();
	
      }
    }
  }

  //DCH Fit and print out the results:
  //DCH TODO

  OutHtml.close();
 
  //cleanUp();
  
}



void doTDCTest(char* myfile, const string& cPathOut="output/TDCrecent") {

  //Set output path for saving multiple chips
  cDirOut = cPathOut;
  
  //DCH: Start with reading in the file to produce the data arrays:
  file.open(myfile, ios::in);
  
  if(!file) {
    cout << "Cannot open file!"<<endl;
    exit(1);
  }
  
  if(!storeTDC(file)) {
    cout << "The data file was not successfully read" << endl;
    exit(1);
  }
  file.close();

  double time[255];
  double inTDC[255];
  double time2[255];

  double time_fit[75];
  double inTDC_fit[75];

  bool foundInt = 0;
  double interval;

  for(int delay=0; delay<255; delay++) {
    time[delay] = delay/4.;
    time2[delay] = delay/4.;


    
    //if(delay < 100) {
    //  time[delay] = delay;
    //}
    //else if(delay < 200) {
    //  time[delay] = delay-100;
    //}
    //else {
    //  time[delay] = delay-200;
    //}

    for(int event = 0; event < 4; event++) {
      for(int slot=0; slot < 22; slot++) {

	TDCCapID[delay][event][slot]    = (TDCtestRead[delay][event][slot] & 0xC000) >> 14;
	TDC[delay][event][slot]         = (TDCtestRead[delay][event][slot] & 0x3F00) >> 8;
	TDCRange[delay][event][slot]    = (TDCtestRead[delay][event][slot] & 0x00C0) >> 6;
	TDCMant[delay][event][slot] = TDCtestRead[delay][event][slot] & 0x003F;
	TDCCharge[delay][event][slot] = LookupCharge(TDCRange[delay][0][slot], TDCMant[delay][0][slot]);

	//cout << "Read: " << TDCtestRead[delay][event][slot] << ", CapID: " << TDCCapID[delay][event][slot] << ", TDC: " << TDC[delay][event][slot];
	//cout << ", Range: " << TDCRange[delay][event][slot] << ", Mant: " << TDCMant[delay][event][slot] << endl;

      }
    }
  }

  //DCH setup the ouput file
  gSystem->mkdir(cDirOut.c_str(), kTRUE);
  sprintf(cTemp,"%s/index.html",cDirOut.c_str());
  OutHtml.open(cTemp);

  OutHtml << "<HTML>" << endl;
  OutHtml << "<Body>" << endl;

  OutHtml << "<H2 style=\"background color: #999999;\">" << endl;
  OutHtml << "Input File: " << myfile << endl;
  OutHtml << "</H2>" << endl;
  OutHtml << "<H2 style=\"background color: #999999;\">" << endl;
  OutHtml << "QIE Viewer, Chip #(NOT YET IMPLEMENTED)" << endl;
  OutHtml << "</H2>" << endl;

  OutHtml << "<h3> TDC Pulse Response </h3>" << endl;

  OutHtml << "<table cellspacing=\"\" cellpading=\"\" border=\"\" align=\"center\" >" << endl;
  OutHtml << "" << endl;
  
  //OutHtml << "<TR align='center'><TD align='center'> Response </TD><TD>DNL</TD></TR>" << endl;
  OutHtml << "<TR>"<< endl;

  bool found = kFALSE;

  double Sum2[255], Sum3[255], Sum4[255], Sum5[255];
  

  for(int delay=0; delay<255; delay++) {

    Sum2[delay] = 0;
    Sum3[delay] = 0;
    Sum4[delay] = 0;
    Sum5[delay] = 0;

    found = kFALSE;
    for(int i=3; i<22; i++) {
      
      if(TDC[delay][0][i]==63) continue;
      
      //cout << TDC[delay][0][i] << endl;
      if(found != kTRUE) {
	inTDC[delay] = TDC[delay][0][i];

	for(int k=0; k < 5; k++) {
	  if(k<2 && (i+k < 22)) Sum2[delay] += LookupCharge(TDCRange[delay][0][i+k], TDCMant[delay][0][i+k]);
	  if(k<3 && (i+k < 22)) Sum3[delay] += LookupCharge(TDCRange[delay][0][i+k], TDCMant[delay][0][i+k]);
	  if(k<4 && (i+k < 22)) Sum4[delay] += LookupCharge(TDCRange[delay][0][i+k], TDCMant[delay][0][i+k]);
	  if(i+k < 22) Sum5[delay] += LookupCharge(TDCRange[delay][0][i+k], TDCMant[delay][0][i+k]);

	}
      }
      found = kTRUE;

      /// DCH line to manipulate noisy TDC data from QIE10 to reproduce "clean" data
      //if(delay>0) if(((inTDC[delay] > (5 + inTDC[delay-1])) && (inTDC[delay-1] > 0 ))|| ((inTDC[delay] > (5 + inTDC[delay+1])) && (inTDC[delay+1] > 0 ))) inTDC[delay] -= 8;
      //if(delay>0) if(inTDC[delay] < (5 + inTDC[delay-1]) && (inTDC[delay-1] > 0)) inTDC[delay] += 8;

    }
    
    if(!found) cout << "Error, no pulse seen." << endl;

  }
  
  double Sum2Avg = 0;
  double Sum3Avg = 0;
  double Sum4Avg = 0;
  double Sum5Avg = 0;

  for(int i=0; i<255; i++) {
    Sum2Avg += Sum2[i];
    Sum3Avg += Sum3[i];
    Sum4Avg += Sum4[i];
    Sum5Avg += Sum5[i];
  }
  Sum2Avg /= 255;
  Sum3Avg /= 255;
  Sum4Avg /= 255;
  Sum5Avg /= 255;
		   
  double Sum2DNL[255];
  double Sum3DNL[255];
  double Sum4DNL[255];
  double Sum5DNL[255];

  double maxSum2Diff = 0;
  double maxSum3Diff = 0;
  double maxSum4Diff = 0;
  double maxSum5Diff = 0;

  for(int i=0; i<255; i++) {
    Sum2DNL[i] = (Sum2Avg - Sum2[i])/Sum2Avg;
    Sum3DNL[i] = (Sum3Avg - Sum3[i])/Sum3Avg;
    Sum4DNL[i] = (Sum4Avg - Sum4[i])/Sum4Avg;
    Sum5DNL[i] = (Sum5Avg - Sum5[i])/Sum5Avg;

    if(abs(Sum2Avg - Sum2[i]) > maxSum2Diff) maxSum2Diff = abs(Sum2Avg - Sum2[i]);
    if(abs(Sum3Avg - Sum3[i]) > maxSum3Diff) maxSum3Diff = abs(Sum3Avg - Sum3[i]); 
    if(abs(Sum4Avg - Sum4[i]) > maxSum4Diff) maxSum4Diff = abs(Sum4Avg - Sum4[i]);
    if(abs(Sum5Avg - Sum5[i]) > maxSum5Diff) maxSum5Diff = abs(Sum5Avg - Sum5[i]);
  }


  //DCH, hard codes to create 1 vector
  for(int delay=0; delay <255; delay++) {
    
    if(delay < 254 && !foundInt) {
      if(inTDC[delay] == 49 && inTDC[delay+1] == 0) {
	foundInt = 1;
	interval = delay+1;
      }
      else if(inTDC[delay] == 49 && inTDC[delay+1] == 1) {
	foundInt = 1;
	interval = delay;
	inTDC[delay] = 0;
      }
      
    }
  }
    
  for(int delay = 0; delay < 255; delay++) {

    if(time[delay] < (interval/4.)) time[delay] +=24.5;
    else if(time[delay] > (interval/4. + 24.5)) {
      while (time[delay] > (interval/4. + 24.5)) time[delay] -= 24.5;
    }
    time[delay] -= interval/4.;
  
  }
    
  for(int i = 0; i<255; i++) {
    sprintf(cTemp,"PulseShape_%i", i+1);

    Pulse[i] = new TH1F(cTemp,"Pulse Shape",13, 0, 13);
    
    for(int j=0; j<13; j++) {
      Pulse[i]->Fill(j, TDCCharge[i][0][j+3]);
    
    }

  }

  double myCharge[255];
  for(int i =0; i < 22; i++) {

    for(int j=0;j<255;j++) {
      myCharge[j] = TDCCharge[j][0][i];
    }
    
    Slice[i]= new TGraph(255, time2, myCharge);
    //Slice[i]->SetTitle("Charge per Bucket; delay; charge (fC)");
    //Slice[i]->GetHistogram()->SetTitleOffset(1.4,"Y");
  }
  

  for(int i=0; i<75; i++) {
    time_fit[i] = time[65+i];
    inTDC_fit[i] = inTDC[65+i];
  }

  tdcGraph = new TGraph(255, time2, inTDC);
  //tdcGraphOff = new TGraph(255, time, inTDC);
  tdcGraphOff = new TGraph(75, time_fit, inTDC_fit);
  
  tdcGraph->SetMarkerStyle(2);
  tdcGraph->SetTitle("TDC Response; delay (ns); TDC");
  tdcGraph->GetHistogram()->SetTitleOffset(1.2,"Y");

  tdcGraphOff->SetMarkerStyle(2);
  tdcGraphOff->SetTitle("TDC Response; delay (ns); TDC");
  tdcGraphOff->GetHistogram()->SetTitleOffset(1.2,"Y");

  c1 = new TCanvas("c1","",900,900);
  c1->cd();
  tdcGraph->Draw("AP");

  saveFile(c1,"TDCResponse","Response", 400);
  
  //DCH, ok do bin widths/DNL

  c1->cd();
  tdcGraphOff->Draw("AP");

  TDCFit = new TF1("TDCFit","pol1", 0, 25);
  
  tdcGraphOff->Fit(TDCFit, "RQ+");

  saveFile(c1,"TDCRespFit","Response Fit", 400);

  gStyle->SetOptStat(1111111);
  TDCOffset = new TH1F("TDCOffset","",40, -5, 5);
  TDCOffset->SetTitle("TDC Offset from fit; Offset; count");
  TDCOffset->SetTitleOffset(1.3,"Y");

  double temp;

  for( int i=0; i < 255; i++) {
    temp = (inTDC[i] - TDCFit->Eval(time[i]));
    if(temp > 25) temp = 50-temp;
    if(temp < -25) temp = 50+temp;

    //TDCOffset->Fill(inTDC[i] - TDCFit->Eval(time[i]));
    TDCOffset->Fill(temp);
    //cout << "Time:  " << time[i] << ", Read: " << inTDC[i] << ", Pred: " << TDCFit->Eval(time[i]) << ", Plot: " << 
  }

  c1->cd();
  TDCOffset->Draw();
  saveFile(c1,"TDCOffset","Offset from Fit", 400);

  OutHtml <<"</TR>" << endl;
  OutHtml << "</table>" << endl;

  OutHtml << "TDC Bin Width: " << fixed << setprecision(3) << 1/TDCFit->GetParameter(1) << " ns" << endl;

  OutHtml << "<h3> Running Sum Tests </h3>" << endl;
  
  OutHtml << "<table cellspacing=\"\" cellpading=\"\" border=\"\" align=\"center\" >" << endl;
  OutHtml << "" << endl;
  
  //OutHtml << "<TR align='center'><TD align='center'> Response </TD><TD>DNL</TD></TR>" << endl;
  //OutHtml << "<TR>"<< endl;
  
  myLeg = new TLegend(0.75, 0.15, 0.88, 0.35);

  AllSlice = new TMultiGraph();
  AllSlice->SetTitle("Charge in bin; delay (ns); charge (fC)");
  //AllSlice->GetHistogram()->SetTitleOffset(1.4,"Y");

  for (int i=5; i<22;i++) {
    Slice[i]->SetMarkerColor(i-4);
    Slice[i]->SetMarkerStyle(22);
    //Slice[i]->SetTitle("Charge in Window; delay (ns); charge (fC)");
    AllSlice->Add(Slice[i]);

    sprintf(cTemp,"Bucket %i", i);

    myLeg->AddEntry(Slice[i],cTemp,"P");
    
  }
  
  delete c1;
  c1 = new TCanvas("c1","",900,900);

  c1->SetLeftMargin(0.15);

  OutHtml << "<TR>"; 
  c1->cd();
  AllSlice->Draw("AP");
  AllSlice->GetYaxis()->SetTitleOffset(1.7);

  myLeg->SetFillColor(0);
  myLeg->Draw();
  saveFile(c1,"SlicePlot","Slices",400);
  //OutHtml << "<TD></TD></TR>" << endl;
  OutHtml << "</TR>" << endl;
  OutHtml << "</table>" << endl;
  OutHtml << "<br>" << endl;


  OutHtml << "<table cellspacing=\"\" cellpading=\"\" border=\"\" align=\"center\" >" << endl;


  OutHtml << "" << endl;
  OutHtml << "<TR align='center'> <TD align='center' colspan=3><b> Running Sum Tests </b></TD></TR>"<< endl;
  OutHtml << "<TR align='center'>" << endl;
  OutHtml << "<TD>2 Bucket Sum Average</TD>";
  OutHtml << "<TD>3 Bucket Sum Average</TD>";  
  OutHtml << "<TD>4 Bucket Sum Average</TD>";
  OutHtml << "<TD>5 Bucket Sum Average</TD>";
  OutHtml << "</TR>" << endl;
  OutHtml << "<TR align='center'><TD>" << Sum2Avg << "</TD><TD>" << Sum3Avg << "</TD><TD>" << Sum4Avg << "</TD><TD>" << Sum5Avg << "</TD></TR>" << endl;


  //OutHtml << "2 Bucket Sum Average: " << Sum2Avg << endl;
  //OutHtml << "3 Bucket Sum Average: " << Sum3Avg << endl;
  //OutHtml << "4 Bucket Sum Average: " << Sum4Avg << endl;


  BinSum2vDelay = new TGraph(255, time2, Sum2);
  BinSum3vDelay = new TGraph(255, time2, Sum3);
  BinSum4vDelay = new TGraph(255, time2, Sum4);
  BinSum5vDelay = new TGraph(255, time2, Sum5);

  BinSum2 = new TH1F("BinSum2","",25, Sum2Avg - (maxSum2Diff+100), Sum2Avg + (maxSum2Diff+100));
  BinSum3 = new TH1F("BinSum3","",25, Sum3Avg - (maxSum3Diff+100), Sum3Avg + (maxSum3Diff+100));
  BinSum4 = new TH1F("BinSum4","",25, Sum4Avg - (maxSum4Diff+100), Sum4Avg + (maxSum4Diff+100));
  BinSum5 = new TH1F("BinSum5","",25, Sum5Avg - (maxSum5Diff+100), Sum5Avg + (maxSum5Diff+100));
  //BinSum4 = new TH1F("BinSum4","",100, 000, 10000);

  TH1F* DNLSum2Plot = new TH1F("DNLSum2Plot","",25, -.5, .5);
  TH1F* DNLSum3Plot = new TH1F("DNLSum3Plot","",25, -.5, .5);
  TH1F* DNLSum4Plot = new TH1F("DNLSum4Plot","",25, -.5, .5);
  TH1F* DNLSum5Plot = new TH1F("DNLSum5Plot","",25, -.5, .5);


  for(int i=0; i<255; i++) {
    BinSum2->Fill(Sum2[i]);
    BinSum3->Fill(Sum3[i]);
    BinSum4->Fill(Sum4[i]);
    BinSum5->Fill(Sum5[i]);

    DNLSum2Plot->Fill(Sum2DNL[i]);
    DNLSum3Plot->Fill(Sum3DNL[i]);
    DNLSum4Plot->Fill(Sum4DNL[i]);
    DNLSum5Plot->Fill(Sum5DNL[i]);
  }

  BinSum2vDelay->SetMarkerStyle(2);
  BinSum3vDelay->SetMarkerStyle(2);
  BinSum4vDelay->SetMarkerStyle(2);
  BinSum5vDelay->SetMarkerStyle(2);

  BinSum2vDelay->SetMinimum(0);
  BinSum3vDelay->SetMinimum(0);
  BinSum4vDelay->SetMinimum(0);
  BinSum5vDelay->SetMinimum(0);

  BinSum2vDelay->SetMaximum(10000);
  BinSum3vDelay->SetMaximum(10000);
  BinSum4vDelay->SetMaximum(10000);
  BinSum5vDelay->SetMaximum(10000);

  BinSum2vDelay->SetTitle("2 Bucket Running Sum; delay (ns); Charge Sum (fC)");
  BinSum3vDelay->SetTitle("3 Bucket Running Sum; delay (ns); Charge Sum (fC)");
  BinSum4vDelay->SetTitle("4 Bucket Running Sum; delay (ns); Charge Sum (fC) ");
  BinSum5vDelay->SetTitle("5 Bucket Running Sum; delay (ns); Charge Sum (fC) ");

  BinSum2vDelay->GetHistogram()->SetTitleOffset(1.8,"Y");
  BinSum3vDelay->GetHistogram()->SetTitleOffset(1.8,"Y");
  BinSum4vDelay->GetHistogram()->SetTitleOffset(1.8,"Y");
  BinSum5vDelay->GetHistogram()->SetTitleOffset(1.8,"Y");

  BinSum2->SetTitle("2 Bucket Running Sum; Charge Sum; count");
  BinSum3->SetTitle("3 Bucket Running Sum; Charge Sum; count");
  BinSum4->SetTitle("4 Bucket Running Sum; Charge Sum; count");
  BinSum5->SetTitle("5 Bucket Running Sum; Charge Sum; count");

  BinSum2->SetTitleOffset(1.3,"Y");
  BinSum3->SetTitleOffset(1.3,"Y");
  BinSum4->SetTitleOffset(1.3,"Y");
  BinSum5->SetTitleOffset(1.3,"Y");

  DNLSum2Plot->SetTitle("2 Bucket Running Sum DNL; DNL; count");
  DNLSum3Plot->SetTitle("3 Bucket Running Sum DNL; DNL; count");
  DNLSum4Plot->SetTitle("4 Bucket Running Sum DNL; DNL; count");
  DNLSum5Plot->SetTitle("5 Bucket Running Sum DNL; DNL; count");

  DNLSum2Plot->SetTitleOffset(1.35,"Y");
  DNLSum3Plot->SetTitleOffset(1.35,"Y");
  DNLSum4Plot->SetTitleOffset(1.35,"Y");
  DNLSum5Plot->SetTitleOffset(1.35,"Y");


  ///
  OutHtml << "<TR>" << endl;
  BinSum2vDelay->Draw("AP");
  saveFile(c1,"Bucket2SumvDelay","2 Bucket Running Sum", 400);
  BinSum2->Draw();
  saveFile(c1,"Bucket2Sum","2 Bucket Sum", 400);
  DNLSum2Plot->Draw();
  saveFile(c1,"Bucket2SumDNL","2 Bucket Sum, DNL",400);
  OutHtml << "</TR>" << endl;

  OutHtml << "<TR>" << endl;
  BinSum3vDelay->Draw("AP");
  saveFile(c1,"Bucket3SumvDelay","3 Bucket Running Sum", 400);
  BinSum3->Draw();
  saveFile(c1,"Bucket3Sum","3 Bucket Sum", 400);
  DNLSum3Plot->Draw();
  saveFile(c1,"Bucket3SumDNL","3 Bucket Sum, DNL",400);
  OutHtml << "</TR>" << endl;

  OutHtml << "<TR>" << endl;
  BinSum4vDelay->Draw("AP");
  saveFile(c1,"Bucket4SumvDelay","4 Bucket Running Sum", 400);
  BinSum4->Draw();
  saveFile(c1,"Bucket4Sum","4 Bucket Sum", 400);
  DNLSum4Plot->Draw();
  saveFile(c1,"Bucket4SumDNL","4 Bucket Sum, DNL",400);
  OutHtml << "</TR>" << endl;
  
  OutHtml << "<TR>" << endl;
  BinSum5vDelay->Draw("AP");
  saveFile(c1,"Bucket5SumvDelay","5 Bucket Running Sum", 400);
  BinSum5->Draw();
  saveFile(c1,"Bucket5Sum","5 Bucket Sum", 400);
  DNLSum5Plot->Draw();
  saveFile(c1,"Bucket5SumDNL","5 Bucket Sum, DNL",400);
  OutHtml << "</TR>" << endl;

  OutHtml << "</table>" << endl;

  OutHtml << "<table cellspacing=\"\" cellpading=\"\" border=\"\" align=\"center\" >" << endl;
  OutHtml << "" << endl;
  //OutHtml << "<TR align='center'><TD align='center'> Response </TD><TD>DNL</TD></TR>" << endl;
  for(int i = 0; i <43; i++) {
    OutHtml << "<TR>"<< endl;
    for( int j=0; j<6; j++ ) {

      if((i != 42) || (i== 42 && j < 3)) {
	c1->cd();
	Pulse[6*i+j]->Draw();
	
	sprintf(cTemp2,"Pulse_%d",i*6+j+1);
	sprintf(cTemp3,"Pulse %d",i*6+j+1);
	saveFile(c1,cTemp2,cTemp3);
      }
      
    }
    OutHtml << "</TR>" << endl;

  }

  OutHtml.close();

  //cleanUp();
  
}

double LookupCharge(int range, int mantissa) {

  double slope[4][4] = { {   3.15,    6.53,   12.94,    25.87},
			 {  24.90,   51.07,  101.49,   205.56},
			 { 197.36,  408.10,  808.58,  1616.92},
			 {1607.31, 3306.87, 6499.76, 13023.76}
  };

  double intercept[4][4] = { {  12.71,   -38.97,   -266.47,    -996.52},
			     { 567.47,   167.25,  -1617.99,   -7501.28},
			     {4977.66,  1759.87, -12501.78,  -58010.37},
			     {41207.52, 15280.59,-98072.03, -466919.50}

  };

  double result;

  if(mantissa < 16 ) result = slope[range][0] * mantissa + intercept[range][0];
  else if(mantissa < 36 ) result = slope[range][1] * mantissa + intercept[range][1];
  else if(mantissa < 57 ) result = slope[range][2] * mantissa + intercept[range][2];
  else if(mantissa < 64 ) result = slope[range][3] * mantissa + intercept[range][3];
  else result = -1;

  return result;

  //return range*64+mantissa;

}

void cleanUp() {
 
  delete c1;
  delete tdcGraph;
  delete tdcGraphOff;
  delete myLeg;

  delete PedGraph;
  delete CAPPedGraph_0;
  delete CAPPedGraph_1;
  delete CAPPedGraph_2;
  delete CAPPedGraph_3;
  delete PedFit;
  delete CAPPedFit_0;
  delete CAPPedFit_1;
  delete CAPPedFit_2;
  delete CAPPedFit_3;
  delete CAPPedGraph_pre0;
  delete CAPPedGraph_pre1;
  delete CAPPedGraph_pre2;
  delete CAPPedGraph_pre3;
  delete pedLeg;

  delete TDCFit;
  delete TDCOffset;

  delete BinSum2vDelay;
  delete BinSum3vDelay;
  delete BinSum4vDelay;
  
  delete BinSum2;
  delete BinSum3;
  delete BinSum4;
  
  for(int  i=0; i<4; i++) {        
    for(int j=0; j<4; j++) {
      for(int k=0; k<4; k++) {
	
	delete BinWidths[i][j][k];
	delete BinWidths_Mid[i][j][k];
	
	delete SubRangeFit[i][j][k];
	
      }

      delete RangePlot[i][j];
      delete RangePlot_Up[i][j];
      delete RangePlot_Dn[i][j];
      
      delete RangePlot_stagger[i][j];
      delete RangePlot_Up_stagger[i][j];
      delete RangePlot_Dn_stagger[i][j];
      
      delete WidthsMid[i][j];
      
      delete DNL_ADC[i][j];

    }

    delete allRanges[i];
    delete allRanges_Up[i];
    delete allRanges_Dn[i];
    
    delete Ranges[i];
    delete SubRanges[i];
  }

  for(int i=0; i<16; i++) delete Slice[i];
  //delete AllSlice;

}

bool storePed(fstream &file) {

  int linenumber = 0; //This tracks the line number of the file
  string data; //This is the data contained on the line
  
  int previousline = 0;//We use this to check the consistency of the data file
  
  stringstream teststring; //We use this to check if we have the format that we expect
  
  //Now we loop through the data until we find the start of the ramping process
  while(data != "capid_test"){
    if(!readline(file,linenumber,data,previousline)) return 0;
  } 
  
  if(!readline(file,linenumber,data,previousline)) return 0;
  if(!readline(file,linenumber,data,previousline)) return 0;
  if(!readline(file,linenumber,data,previousline)) return 0;
  if(!readline(file,linenumber,data,previousline)) return 0;

  PedCIDCount[0] = 0;
  PedCIDCount[1] = 0;
  PedCIDCount[2] = 0;
  PedCIDCount[3] = 0;
 
  int temp;
  int CID;

  while(data != "volts_test"){
    istringstream(data) >> hex >> temp;
      
    //cout << "data: " << data << ", Counts: " << PedCIDCount[0] 
    //	 << " " << PedCIDCount[1] << " " << PedCIDCount[2] << " " << PedCIDCount[3] 
    //	 << endl;

    CID = (temp & 0xC000) >> 14;

    PedRange[CID][PedCIDCount[CID] ] = (temp & 0x00C0)>>6;
    PedMant[CID][PedCIDCount[CID] ] = temp & 0x003F;
    
    PedCIDCount[((temp & 0xC000) >> 14)]++;

    if(!readline(file,linenumber,data,previousline)) return 0;
  }

  //cout << PedCIDCount[0] << " " << PedCIDCount[1] << " "<< PedCIDCount[2] << " " << PedCIDCount[3] << " " << endl;

  while(data != ("ped_dac_test"))
    {
      if(!readline(file,linenumber,data,previousline)) {
	//cout << "ended 1"<< endl;
	return 0;
      }
    }

  for(int ped=0; ped<64; ped++)
    {
      //This loop is for the number of events taken.  Currently 3
      for(int i=0; i<4; i++)
            {
                if(!readline(file,linenumber,data,previousline))
                    {
                        //cout << "ended 3"<< endl;
                        return 0;
                    }
            }
	  for(int read=0; read<129; read++)
	    {
	       if(!readline(file,linenumber,data,previousline))
                    {
                        //cout << "ended 3"<< endl;
                        return 0;
                    }
	       //istringstream(data) >> hex >> PEDtestread[ped][read];
	       istringstream(data) >> hex >> temp;
	       
	       PEDtestRange[ped][read] = (temp & 0x00C0)>>6;
	       PEDtestMant[ped][read] = temp & 0x003F;
	    }
    }

  while(data != ("cap_ped_dac_test"))
    {
      if(!readline(file,linenumber,data,previousline)) {
	//cout << "ended 1"<< endl;
	return 0;
      }
    }
    
  for(int ped=0; ped<15; ped++)
    {
      CAPPEDtestCount[ped][0] = 0;
      CAPPEDtestCount[ped][1] = 0;
      CAPPEDtestCount[ped][2] = 0;
      CAPPEDtestCount[ped][3] = 0;

      //This loop is for the number of events taken.  Currently 3
      for(int i=0; i<4; i++)
	{
	  if(!readline(file,linenumber,data,previousline))
	    {
	      //cout << "ended 3"<< endl;
	      return 0;
	    }
	}
      for(int read=0; read<129; read++)
	{
	  if(!readline(file,linenumber,data,previousline))
	    {
	      //cout << "ended 3"<< endl;
	      return 0;
	    }
	  //istringstream(data) >> hex >> CAPPEDtestread[ped][read];
	  istringstream(data) >> hex >> temp;
	  
	  CID = (temp & 0xC000) >> 14;
  
	  CAPPEDtestRange[ped][CID][CAPPEDtestCount[ped][CID]] = (temp & 0x00C0)>>6;
	  CAPPEDtestMant[ped][CID][CAPPEDtestCount[ped][CID]++] = temp & 0x003F; 

	}
    }
  

  return 1;
  
}

//This function take a given open file, and make certain assumptions about its format. It will then return two arrays, one with the results of the upDAQ and one with the results of the downDAQ
//bool storeADC(fstream &file, int upDAQ[][4][64], int downDAQ[][4][64])
bool storeADC(fstream &file)
{

  //#ifdef debugstore
  //fstream test;
  //test.open("dump.txt", ios::out); // test file
  //#endif

  int linenumber = 0; //This tracks the line number of the file
  string data; //This is the data contained on the line
  int previousline = 0;//We use this to check the consistency of the data file
  stringstream teststring; //We use this to check if we have the format that we expect

  //Now we loop through the data until we find the start of the ramping process
  while(data != "ramping_down"){
    if(!readline(file,linenumber,data,previousline)) return 0;
    
  }
  // The loops below assign to each element in the string arrays the appropriate element from the file for downDAQ. 
  // Then, converts the hexadecimal strings over to decimal integers in the integer arrays.
  for(int cap=0; cap<=3; ++cap){

    if(!readline(file,linenumber,data,previousline)) return 0;

    //At the start of each new capacitor we expect to see "qiedatacap?"
    teststring.str("");
    teststring << "qiedatacap" << cap;
    if(data != teststring.str()){
      //#ifdef debugstore
      //cout << "Read: " << data << " Expected: qiedatacap" << cap << endl;
      //#endif
	return 0;
      }

    for(int range=0; range<=3; ++range){

      if(!readline(file,linenumber,data,previousline)) return 0;
      
      //At the start of each new range we expect to see "cap?_rang?"
      teststring.str("");
      teststring << "cap" << cap << "_rang" << range;
      if(data != teststring.str()){
	//#ifdef debugstore
	//cout << "Read: " << data << " Expected: " << teststring.str() << endl;
	//#endif
	return 0;
      }

      for(int bin=0; bin<=63; ++bin){
	if(!readline(file,linenumber,data,previousline)) return 0;

	istringstream(data) >> hex >> DownDAQ[cap][range][bin];
	
	//cout << "read in: " << data << ", DownDAQ: " << DownDAQ[cap][range][bin] << endl;

	//#ifdef debugstore  
	//cout << downDAQ[cap][range][bin]<<endl;
	//test << downDAQ[cap][range][bin]<<endl;
	//#endif
      }
    }
  }
      
  //Same thing as above, but for upDAQ.
  //Now we loop through the data until we find the start of the ramping process
  while(data != "ramping_up"){
    if(!readline(file,linenumber,data,previousline)) return 0;
  }
  // The loops below assign to each element in the string arrays the appropriate element from the file for upDAQ. 
  // Then, converts the hexadecimal strings over to decimal integers in the integer arrays.
  for(int cap=0; cap<=3; ++cap){

    if(!readline(file,linenumber,data,previousline)) return 0;

    //At the start of each new capacitor we expect to see "qiedatacap?"
    teststring.str("");
    teststring << "qiedatacap" << cap;
    if(data != teststring.str()){
      //#ifdef debugstore
      //cout << "Read: " << data << " Expected: qiedatacap" << cap << endl;
      //#endif
	return 1;
      }

    for(int range=0; range<=3; ++range){

      if(!readline(file,linenumber,data,previousline)) return 0;
      
      //At the start of each new range we expect to see "cap?_rang?"
      teststring.str("");
      teststring << "cap" << cap << "_rang" << range;
      if(data != teststring.str()){
	//#ifdef debugstore
	//cout << "Read: " << data << " Expected: cap" << cap << "_rang" << range << endl;
	//#endif
	return 0;
      }

      for(int bin=0; bin<=63; ++bin){
	if(!readline(file,linenumber,data,previousline)) return 0;
	istringstream(data) >> hex >> UpDAQ[cap][range][bin];
	//#ifdef debugstore  
	//cout << upDAQ[cap][range][bin]<<endl;
	//test << upDAQ[cap][range][bin]<<endl;
	//#endif
      }
    }
  }

  //#ifdef debugstore
  //test.close();
  //#endif
    
  return 1;
}

bool storeTDC(fstream &file)
{


  int linenumber = 0; //This tracks the line number of the file
  string data; //This is the data contained on the line

  int previousline = 0;//We use this to check the consistency of the data file

  //stringstream teststring; //We use this to check if we have the format that we expect


  //******************************************************************************
  //Loops through delay/event data, skipping text and getting hex values
  //Start the TDC data storage:
  while(data != ("tdc_test"))
    //while(data != ("delay=00"))
    {
      if(!readline(file,linenumber,data,previousline)) {
	cout << "ended 1"<< endl;
	return 0;
      }
    }
   while(data != ("next"))
    {
      if(!readline(file,linenumber,data,previousline)) {
	cout << "ended 1a"<< endl;
	return 0;
      }
    }
   
   for(int delay=0; delay<255; delay++)
     {
       //if(!readline(file,linenumber,data,previousline))
       //	{
       //	  cout << "ended 2"<< endl;
       //	  return 0;
       //	}
       if(delay != 0) {
	 if(!readline(file,linenumber,data,previousline)) {
	   cout << "ended 3"<< endl;
	   return 0;
	 }
       }
       for(int event=0; event<4; event++)
	 {
	   //if(!readline(file,linenumber,data,previousline))
	   //{
	   //  cout << "ended 2"<< endl;
	   //  return 0;
	   //}
	   for(int i=0; i<22; i++)
	     {
	       if(!readline(file,linenumber,data,previousline))
		 {
		   cout << "ended 3"<< endl;
		   return 0;
		 }
	       istringstream(data) >> hex >> TDCtestRead[delay][event][i];
	     }
	   if(!readline(file,linenumber,data,previousline))
	     {
	       cout << "ended 2"<< endl;
	       return 0;
	     }
	 }
       //if(!readline(file,linenumber,data,previousline))
       //  {
       //	  cout << "ended 4"<< endl;
       //	  return 0;
       //  }
     }
   
   return 1;
}

void saveFile(TCanvas *c1, const string& cStrSave, const string& cHeader, double width, bool table ){ 

  if(table) OutHtml<<"<TD  align='center'>"<< endl;
  if(table) OutHtml<<cHeader<<endl;
  OutHtml<<"<BR>"<<endl;
  sprintf(cTemp,"%s/%s.gif",cDirOut.c_str(),cStrSave.c_str());
  c1->Print(cTemp,"gif");
  sprintf(cTemp,"%s.gif",cStrSave.c_str());
  OutHtml<< "<A href=\""<<cTemp<<"\">"<<endl;
  //if(table)OutHtml <<"<img align='center' id=\""<<cTemp<<"\" name=\""<<cTemp<<"\" src=\""<<cTemp<<"\" alt=\"Click\" width='200' height='200' > " <<endl;
  if(table)OutHtml <<"<img align='center' id=\""<<cTemp<<"\" name=\""<<cTemp<<"\" src=\""<<cTemp<<"\" alt=\"Click\" width='" << width << "' height='" << width << "' > " <<endl;
  else OutHtml <<"<img align='center' id=\""<<cTemp<<"\" name=\""<<cTemp<<"\" src=\""<<cTemp<<"\" alt=\"Click\"> " <<endl;
  OutHtml<< "</A>" << endl;
  
  OutHtml<<"<BR>"<<endl;
  sprintf(cTemp,"%s/%s.pdf",cDirOut.c_str(),cStrSave.c_str());
  c1->Print(cTemp,"pdf");
  sprintf(cTemp,"%s.pdf",cStrSave.c_str());
  OutHtml<< "<A href=\""<<cTemp<<"\">"<<endl;
  OutHtml <<"(.pdf)"<<endl;
  OutHtml<< "</A>" << endl;

  sprintf(cTemp,"%s/%s.C",cDirOut.c_str(),cStrSave.c_str());
  c1->Print(cTemp,"cxx");
  sprintf(cTemp,"%s.C",cStrSave.c_str());
  OutHtml<< "<A href=\""<<cTemp<<"\">"<<endl;
  OutHtml <<"(.C)"<<endl;
  OutHtml<< "</A>" << endl;
  
  if(table) OutHtml<<"</TD>"<< endl;
}

//double LookupDAQ(int DAC, bool range01) {
double LookupDAQ(int DAC, int whichRange) {

  //if(range01) {
  ////return 12663.5 - 0.40468*DAC; //10k resistor
  //return 10918 - 0.3457*DAC; //tweaked to 11.7 k resistor
  //}
  //else {
  ////return 375681.9 - 12.07614*DAC; // 210 resistor
  ////return 369872.6 - 11.7481*DAC; // tweaked to 215k resistor
  //return 369946.2 - 11.7754*DAC; //tweaked to 215 resistor to match 11.7k tweak
  //}

  //QIE10 old board
  //if(whichRange == 0) {
  //  if(DAC > 31620) return -0.3308867*DAC + 10525.9093;
  //  else if( DAC > 31230) return -0.3419579*DAC + 10875.2983;
  //  else return -0.3418342*DAC + 10870.6709;
  //}
  //else if(whichRange == 1) {
  //  if(DAC > 29035) return -0.3540757*DAC + 11245.9866;
  //  else if(DAC > 26150) return -0.3598794*DAC + 11409.1933;
  //  else return -0.3646359*DAC + 11529.6285;
  //}
  //else if(whichRange == 2) {
  //  if(DAC > 30635) return -9.3667073*DAC + 295078.317;
  //  else if (DAC > 29850) return -10.499082*DAC +329718.577;
  //  else return -11.475389*DAC + 358693.502;
  //}
  //else if(whichRange ==3 ) {
  //  if(DAC > 25250) return -11.836627*DAC+368531.255;
  //  else return -12.071611*DAC + 374640.498;
  //
  //}

  //QIE10 production version. robot test board
  //if(whichRange == 0) {
  //  if(DAC > 31810) return -0.3185386*DAC + 10195.6906;
  //  else if( DAC > 31410) return -0.3320204*DAC + 10623.6574;
  //  else return -0.3414152*DAC + 10916.9529;
  //}
  //else if(whichRange == 1) {
  //  if(DAC > 29100) return -0.3468817*DAC + 11065.0535;
  //  else if(DAC > 26200) return -0.3557116*DAC + 11323.9317;
  //  else return -0.3684184*DAC + 11641.4104;
  //}
  //else if(whichRange == 2) {
  //  if(DAC > 30635) return -8.2624156*DAC + 261175.435;
  //  else if (DAC > 29850) return -9.2708161*DAC +292051.72;
  //  else return -10.051575*DAC + 315065.982;
  //}
  //else if(whichRange ==3 ) {
  //  if(DAC > 24900) return -10.567903*DAC+329931.967;
  //  else return -10.879485*DAC + 337905.564;
  //  
  //}
  ////else return 0;
  //else if(whichRange == 5) {
  //  if(DAC > 31810) return -0.3185386*DAC + 10195.6906;
  //  else if(DAC > 31410) return -0.3320204*DAC + 10623.6574;
  //  else if(DAC > 31180) return -0.3414152*DAC + 10916.9529;
  //  else if(DAC > 29100) return -0.3468817*DAC + 11065.0535;
  //  else if(DAC > 26200) return -0.3557116*DAC + 11323.9317;
  //  else return -0.3684184*DAC + 11641.4104;
  //}
  //else if(whichRange == 6) {
  //  if(DAC > 30635) return -8.2624156*DAC + 261175.435;
  //  else if(DAC > 29850) return -9.2708161*DAC +292051.72;
  //  else if(DAC > 27150) return -10.051575*DAC + 315065.982;
  //}
  if(whichRange == 0) {
    if(DAC > 31810) return -0.3185386*DAC + 10195.6906;
    else if( DAC > 31410) return -0.3320204*DAC + 10623.6574;
    else return -0.3414152*DAC + 10916.9529;
  }
  else if(whichRange == 1) {
    if(DAC > 29100) return -0.3468817*DAC + 11065.0535;
    else if(DAC > 26200) return -0.3557116*DAC + 11323.9317;
    else return -0.3684184*DAC + 11641.4104;
  }
  else if(whichRange == 2) {
    if(DAC > 30635) return -8.2624156*DAC + 261175.435;
    else if (DAC > 29850) return -9.2708161*DAC +292051.72;
    else return -10.051575*DAC + 315065.982;
  }
  else if(whichRange ==3 ) {
    if(DAC > 24900) return -10.567903*DAC+329931.967;
    else return -10.879485*DAC + 337905.564;
    
  }
  //else return 0;
  else if(whichRange == 5) {
    if(DAC > 31810) return -0.3185386*DAC + 10195.6906;
    else if(DAC > 31410) return -0.3320204*DAC + 10623.6574;
    else if(DAC > 31180) return -0.3414152*DAC + 10916.9529;
    else if(DAC > 29100) return -0.3468817*DAC + 11065.0535;
    else if(DAC > 26200) return -0.3557116*DAC + 11323.9317;
  //  else return -0.3684184*DAC + 11641.4104;
  }
  else if(whichRange == 6) {
    if(DAC > 30635) return -8.2624156*DAC + 261175.435;
    else if(DAC > 29850) return -9.2708161*DAC +292051.72;
   else if(DAC > 27150) return -10.051575*DAC + 315065.982;
    else if(DAC > 24900) return -10.567903*DAC+329931.967;
    else return -10.879485*DAC + 337905.564;
  
  }
  else return 0;
   
  //QIE11 board:
   if(whichRange < 2) return -0.3782*DAC + 12295;
   else if(whichRange < 4) return -12.019*DAC + 420168;
   else return 0;

}

///
void doNewErrorTest(char* myfile, const string& cPathOut="output/newErr")
{

  //Set output path for saving multiple chips
  cDirOut = cPathOut;
  
  //DCH: Start with reading in the file to produce the data arrays:
  file.open(myfile, ios::in);
  
  if(!file) {
    cout << "Cannot open file!"<<endl;
    exit(1);
  }
  
  //DCH setup the ouput file
  gSystem->mkdir(cDirOut.c_str(), kTRUE);
  sprintf(cTemp,"%s/index.html",cDirOut.c_str());
  OutHtml.open(cTemp);
  
  c1 = new TCanvas("c1","",900,900);

  OutHtml << "<HTML>" << endl;
  
  //2240 scan points in new test
  //const int numPoints = 144;
  //const int numTests = 1152; 
  //sprintf(cTemp,"%s.txt",myoutput);
  //OutHtml.open(cTemp);
  //TFile outRoot(myoutput,"RECREATE");
  
  //DCH: Start with reading in the file to produce the data arrays:
  
  double DAC_R01[249];
  double Avg_R01[249];
  double Cap0_R01[249];
  double Cap1_R01[249];
  double Cap2_R01[249];
  double Cap3_R01[249];

  double DAC_R12[143];
  double Avg_R12[143];
  double Cap0_R12[143];
  double Cap1_R12[143];
  double Cap2_R12[143];
  double Cap3_R12[143];

  double DAC_R23[171];
  double Avg_R23[171];
  double Cap0_R23[171];
  double Cap1_R23[171];
  double Cap2_R23[171];
  double Cap3_R23[171];


  int linenumber = 0; //This tracks the line number of the file
  string data; //This is the data contained on the line
  
  int previousline = 0;//We use this to check the consistency of the data file
  
  while(data != "mantisa_scan01") {
      readline(file,linenumber,data,previousline);
  }

  for(int i=0; i<249; i++) {

    readline(file,linenumber,data,previousline);
    DAC_R01[i] = strtol((data.substr(8,4)).c_str(),NULL, 16);
    //DACread= data.substr(8,4);

    //cout << "DAC: " << DAC_R01[i];

    readline(file,linenumber,data,previousline);
    Cap0_R01[i] = strtol(data.c_str(),NULL,16);
    readline(file,linenumber,data,previousline);
    Cap1_R01[i] = strtol(data.c_str(),NULL,16);
    readline(file,linenumber,data,previousline);
    Cap2_R01[i] = strtol(data.c_str(),NULL,16);
    readline(file,linenumber,data,previousline);
    Cap3_R01[i] = strtol(data.c_str(),NULL,16);

    //cout << ", Cap0: " << Cap0_R01[i] << endl;

    Avg_R01[i] = (Cap0_R01[i] + Cap1_R01[i] + Cap2_R01[i] + Cap3_R01[i])/4;
  }

  while(data != "mantisa_scan12") {
      readline(file,linenumber,data,previousline);
  }
  for(int i=0; i<143; i++) {

    readline(file,linenumber,data,previousline);
    DAC_R12[i] = strtol((data.substr(8,4)).c_str(),NULL, 16);
    //DACread= data.substr(8,4);

    readline(file,linenumber,data,previousline);
    Cap0_R12[i] = strtol(data.c_str(),NULL,16);
    readline(file,linenumber,data,previousline);
    Cap1_R12[i] = strtol(data.c_str(),NULL,16);
    readline(file,linenumber,data,previousline);
    Cap2_R12[i] = strtol(data.c_str(),NULL,16);
    readline(file,linenumber,data,previousline);
    Cap3_R12[i] = strtol(data.c_str(),NULL,16);

    Avg_R12[i] = (Cap0_R12[i] + Cap1_R12[i] + Cap2_R12[i] + Cap3_R12[i])/4;
  }

  while(data != "mantisa_scan23") {
      readline(file,linenumber,data,previousline);
  }
  for(int i=0; i<171; i++) {

    readline(file,linenumber,data,previousline);
    DAC_R23[i] = strtol((data.substr(8,4)).c_str(),NULL, 16);
    //DACread= data.substr(8,4);

    readline(file,linenumber,data,previousline);
    Cap0_R23[i] = strtol(data.c_str(),NULL,16);
    readline(file,linenumber,data,previousline);
    Cap1_R23[i] = strtol(data.c_str(),NULL,16);
    readline(file,linenumber,data,previousline);
    Cap2_R23[i] = strtol(data.c_str(),NULL,16);
    readline(file,linenumber,data,previousline);
    Cap3_R23[i] = strtol(data.c_str(),NULL,16);

    Avg_R23[i] = (Cap0_R23[i] + Cap1_R23[i] + Cap2_R23[i] + Cap3_R23[i])/4;
  }

  TGraph Cap0_err_R01(249, DAC_R01, Cap0_R01);
  TGraph Cap1_err_R01(249, DAC_R01, Cap1_R01);
  TGraph Cap2_err_R01(249, DAC_R01, Cap2_R01);
  TGraph Cap3_err_R01(249, DAC_R01, Cap3_R01);
  TGraph Avg_err_R01(249, DAC_R01, Avg_R01);


  TGraph Cap0_err_R12(143, DAC_R12, Cap0_R12);
  TGraph Cap1_err_R12(143, DAC_R12, Cap1_R12);
  TGraph Cap2_err_R12(143, DAC_R12, Cap2_R12);
  TGraph Cap3_err_R12(143, DAC_R12, Cap3_R12);
  TGraph Avg_err_R12(143, DAC_R12, Avg_R12);

  TGraph Cap0_err_R23(171, DAC_R23, Cap0_R23);
  TGraph Cap1_err_R23(171, DAC_R23, Cap1_R23);
  TGraph Cap2_err_R23(171, DAC_R23, Cap2_R23);
  TGraph Cap3_err_R23(171, DAC_R23, Cap3_R23);
  TGraph Avg_err_R23(171, DAC_R23, Avg_R23);

  Cap0_err_R01.SetMarkerStyle(21);
  Cap1_err_R01.SetMarkerStyle(21);
  Cap2_err_R01.SetMarkerStyle(21);
  Cap3_err_R01.SetMarkerStyle(21);
  Avg_err_R01.SetMarkerStyle(21);
  Cap0_err_R12.SetMarkerStyle(21);
  Cap1_err_R12.SetMarkerStyle(21);
  Cap2_err_R12.SetMarkerStyle(21);
  Cap3_err_R12.SetMarkerStyle(21);
  Avg_err_R12.SetMarkerStyle(21);
  Cap0_err_R23.SetMarkerStyle(21);
  Cap1_err_R23.SetMarkerStyle(21);
  Cap2_err_R23.SetMarkerStyle(21);
  Cap3_err_R23.SetMarkerStyle(21);
  Avg_err_R23.SetMarkerStyle(21);


  c1 = new TCanvas("c1","",900,900);

  OutHtml << "<table cellspacing=\"\" cellpading=\"\" border=\"\" align=\"center\" >" << endl;
  OutHtml << "" << endl;
  //OutHtml << "<TR align='center'><TD align='center'> Response </TD><TD>DNL</TD></TR>" << endl;

  c1->cd();
  //OutHtml << "<TR>"<< endl;
  //Pulse[6*i+j]->Draw();
  //Cap0_err_R01.Draw("AP");
  //sprintf(cTemp2,"Cap_%d",i*6+j+1);
  //sprintf(cTemp3,"Pulse %d",i*6+j+1);
  //saveFile(c1,"CapID0_R01","");
  //}    
  //}
  
  OutHtml << "<TR>"<< endl;
  
  Avg_err_R01.Draw("AP");
  saveFile(c1,"Avg_R01","");
  c1->cd();
  Cap0_err_R01.Draw("AP");
  saveFile(c1,"CID0_R01","");
  c1->cd();
  Cap1_err_R01.Draw("AP");
  saveFile(c1,"CID1_R01","");
  c1->cd();
  Cap2_err_R01.Draw("AP");
  saveFile(c1,"CID2_R01","");
  c1->cd();
  Cap3_err_R01.Draw("AP");
  saveFile(c1,"CID3_R01","");
  
  OutHtml << "</TR>" << endl;
  
  OutHtml << "<TR>"<< endl;
  
  c1->cd();
  Avg_err_R12.Draw("AP");
  saveFile(c1,"Avg_R12","");
  c1->cd();
  Cap0_err_R12.Draw("AP");
  saveFile(c1,"CID0_R12","");
  c1->cd();
  Cap1_err_R12.Draw("AP");
  saveFile(c1,"CID1_R12","");
  c1->cd();
  Cap2_err_R12.Draw("AP");
  saveFile(c1,"CID2_R12","");
  c1->cd();
  Cap3_err_R12.Draw("AP");
  saveFile(c1,"CID3_R12","");
  
  OutHtml << "</TR>" << endl;
  
  OutHtml << "<TR>"<< endl;
  
  c1->cd();
  Avg_err_R23.Draw("AP");
  saveFile(c1,"Avg_R23","");
  c1->cd();
  Cap0_err_R23.Draw("AP");
  saveFile(c1,"CID0_R23","");
  c1->cd();
  Cap1_err_R23.Draw("AP");
  saveFile(c1,"CID1_R23","");
  c1->cd();
  Cap2_err_R23.Draw("AP");
  saveFile(c1,"CID2_R23","");
  c1->cd();
  Cap3_err_R23.Draw("AP");
  saveFile(c1,"CID3_R23","");
  
  OutHtml << "</TR>" << endl;
  
  OutHtml << "</table>" << endl;



  //}

  OutHtml.close();
  
  
  
  file.close();
  

}

  

void doErrorTest(char* myfile, char* myoutput, int whichRange = 5, bool printToScreen = 0, const int numPoints = 1022)
{
  //2240 scan points in new test

  //const int numPoints = 144;
  const int numTests = 1152; 

  sprintf(cTemp,"%s.txt",myoutput);
  OutHtml.open(cTemp);
  
  TFile outRoot(myoutput,"RECREATE");
  
  //DCH: Start with reading in the file to produce the data arrays:
  file.open(myfile, ios::in);
  
  if(!file) {
    cout << "Cannot open file!"<<endl;
    exit(1);
  }
  
  TH1F* MantPlot = new TH1F("MantPlot","MantPlot", 256, -0.5, 255.5);

  int linenumber = 0; //This tracks the line number of the file
  string data; //This is the data contained on the line
  
  int previousline = 0;//We use this to check the consistency of the data file
  
  int mantVec[numTests];
  int mantissa;
  int CapID[numTests];
  double mantAvg[numPoints];
  double mantAvg_CID[4][numPoints];
  string DACread;
  double DAC[numPoints];
  double ierror[numPoints];
  double ierror_CID[4][numPoints];
  double ierror_unc[numPoints];
  double ierror_CID_unc[4][numPoints];
  int temp;
  int mantCount[256];
  int mantCount_CID[4][256];
  double charge[numPoints];
  
  
  istringstream m;
  
  readline(file,linenumber,data,previousline);
  readline(file,linenumber,data,previousline);
  readline(file,linenumber,data,previousline);
  
  double pedestal = 0;
  
  for(int i=0; i<128; i++) {
    readline(file,linenumber,data,previousline);
    
  }
  
  //TH1F MantValue("MantValue","",256, 0, 256);
  //TF1 MantGaus("MantGaus","gaus",0,256);
  
  for(int i=0; i<numPoints; i++) {
    readline(file,linenumber,data,previousline);
    
    //DACread = data;    
    DACread = data.substr(8,4);
    
    DAC[i] = 0;
    charge[i] = 0;
    mantAvg[i] = 0;
    mantAvg_CID[0][i] = 0;
    mantAvg_CID[1][i] = 0;
    mantAvg_CID[2][i] = 0;
    mantAvg_CID[3][i] = 0;
    ierror[i] = 0;
    ierror_CID[0][i] = 0;
    ierror_CID[1][i] = 0;
    ierror_CID[2][i] = 0;
    ierror_CID[3][i] = 0;
    ierror_unc[i] = 0;
    ierror_CID_unc[0][i] = 0;
    ierror_CID_unc[1][i] = 0;
    ierror_CID_unc[2][i] = 0;
    ierror_CID_unc[3][i] = 0;
    
    DAC[i] = strtol(DACread.c_str(),NULL, 16);
    charge[i] = LookupDAQ(DAC[i],whichRange);
    
    for(int k=0; k< 256; k++) {
      mantCount[k] = 0;
      mantCount_CID[0][k] = 0;
      mantCount_CID[1][k] = 0;
      mantCount_CID[2][k] = 0;
      mantCount_CID[3][k] = 0;
      
    }
    
    for(int j=0; j<numTests; j++) {
      readline(file,linenumber,data,previousline);
      
      //cout << data << " ";
      
      temp = 0;
      
      //m.str(data);
      //cout << m.str() << endl;
      //m >> std::hex >> temp;
      //m >> hex >> temp;
      //m.str(std::string());
      
      //istringstream(data) >> hex >> temp;
      temp = strtol(data.c_str(),NULL,16);
      
      //cout << temp << endl;
      
      mantissa = ((temp & 0x00C0) >> 6)*64+(temp & 0x003F);
      CapID[j]    = (temp & 0xC000) >> 14;
      
      MantPlot->Fill(mantissa);

      //MantValue.Fill(mantissa);
      
      mantVec[j] = mantissa;
      mantAvg[i] += mantissa;	
      
      mantCount[mantissa]++;
      mantCount_CID[CapID[j]][mantissa]++;
      
      
      //cout << mantissa << " " << mantAvg[i] << endl;
      
    }
    
    //cout << mantAvg[i] << endl;
    
    mantAvg[i] /= numTests;
    
    
    int max = 0;
    int whichMant = 0;
    
    int max_CID[4] = {0,0,0,0};
    int whichMant_CID[4] = {0,0,0,0};
    
    for(int k=0; k<256; k++) {
      if(mantCount[k] > max) {
	//max = k;
	max = mantCount[k];
	//mantAvg[i] = k;
	whichMant = k;
      }
      
      for(int q=0; q<4; q++) {
	if(mantCount_CID[q][k] > max_CID[q]) {
	  //max_CID[q] = k;
	  max_CID[q] = mantCount_CID[q][k];
	  whichMant_CID[q] = k;
	}
	
      }
    }
    
    int denom = 0;
    int denom_CID[4] = {0,0,0,0};

    mantAvg[i] = 0;
    mantAvg_CID[0][i] = 0;
    mantAvg_CID[1][i] = 0;
    mantAvg_CID[2][i] = 0;
    mantAvg_CID[3][i] = 0;

    for(int k=0; k<256; k++) {
      if(abs(k-whichMant) < 7) {
	mantAvg[i] += k*mantCount[k];
	denom += mantCount[k];
      }

      for(int q = 0; q<4; q++) {
	//if(abs(k-whichMant_CID[q]) < 7) {
	if(abs(k-whichMant) < 7) {
	  mantAvg_CID[q][i] += k*mantCount_CID[q][k];
	  denom_CID[q] += mantCount_CID[q][k];
	}
      }
      
    }
    
    mantAvg[i]/=denom;
    mantAvg_CID[0][i]/= denom_CID[0];
    mantAvg_CID[1][i]/= denom_CID[1];
    mantAvg_CID[2][i]/= denom_CID[2];
    mantAvg_CID[3][i]/= denom_CID[3];


    //MantValue.Fit("MantGaus","Q");
    //mantAvg[i] = MantGaus.GetParameter(1);

    for(int j=0; j<numTests; j++) {
      
      if(abs(mantVec[j]-mantAvg[i]) > 6) {
	ierror[i]++;
	//if(abs(mantVec[j]-mantAvg[i]) > 6) ierror_CID[CapID[j]][i]++;
	ierror_CID[CapID[j]][i]++;
      }
      
    }
    
    //OutHtml << DACread << " " << mantAvg << " " << ierror << endl;
    OutHtml << setw(4) << DACread << " " << fixed << setw(5) << setfill(' ') << setprecision(0) << DAC[i] << " " 
	    << setw(8) << setfill(' ') << fixed << setprecision(2) << charge[i] << " " 
	    << setw(3) <<whichMant << " " << setw(6) << setfill(' ') << fixed << setprecision(2) << mantAvg[i] << " " 
	    << setw(6) << fixed << setprecision(4) << ierror[i]/numTests << "  "
	    << setw(6) << fixed << setprecision(4) << ierror_CID[0][i]/numTests << "  "
	    << setw(6) << fixed << setprecision(4) << ierror_CID[1][i]/numTests << "  "
	    << setw(6) << fixed << setprecision(4) << ierror_CID[2][i]/numTests << "  "
	    << setw(6) << fixed << setprecision(4) << ierror_CID[3][i]/numTests 
	    << endl;
    
    
    if(printToScreen) {
      cout << setw(4) << DACread << " " << fixed << setw(5) << setfill(' ') << setprecision(0) << DAC[i] << " " << setw(8) << setfill(' ') << fixed << setprecision(2) << charge[i] << " " 
	   << setw(3) <<whichMant << " " << setw(6) << setfill(' ') << fixed << setprecision(2) << mantAvg[i] << " " 
	   << setw(6) << fixed << setprecision(4) << ierror[i]/numTests << "  "
	   << setw(6) << fixed << setprecision(4) << ierror_CID[0][i]/(numTests/4) << "  "
	   << setw(6) << fixed << setprecision(4) << ierror_CID[1][i]/(numTests/4) << "  "
	   << setw(6) << fixed << setprecision(4) << ierror_CID[2][i]/(numTests/4) << "  "
	   << setw(6) << fixed << setprecision(4) << ierror_CID[3][i]/(numTests/4) 
	
	   << endl;
    }
    
    ierror[i]/=numTests;
    ierror_CID[0][i] /= numTests/4;
    ierror_CID[1][i] /= numTests/4;
    ierror_CID[2][i] /= numTests/4;
    ierror_CID[3][i] /= numTests/4;


    ierror_unc[i] = sqrt(ierror[i]*(1-ierror[i])/numTests);
    ierror_CID_unc[0][i] = sqrt(ierror_CID[0][i]*(1-ierror_CID[0][i])/(numTests/4));
    ierror_CID_unc[1][i] = sqrt(ierror_CID[1][i]*(1-ierror_CID[1][i])/(numTests/4));
    ierror_CID_unc[2][i] = sqrt(ierror_CID[2][i]*(1-ierror_CID[2][i])/(numTests/4));
    ierror_CID_unc[3][i] = sqrt(ierror_CID[3][i]*(1-ierror_CID[3][i])/(numTests/4));
    

  }
  
  //TGraph errorRate(numPoints,mantAvg, ierror);
  TGraphErrors errorRate_mant(numPoints,mantAvg, ierror, NULL, ierror_unc);
  TGraphErrors errorRate_mant_CID0(numPoints,mantAvg_CID[0], ierror_CID[0], NULL, ierror_CID_unc[0]);
  TGraphErrors errorRate_mant_CID1(numPoints,mantAvg_CID[1], ierror_CID[1], NULL, ierror_CID_unc[1]);
  TGraphErrors errorRate_mant_CID2(numPoints,mantAvg_CID[2], ierror_CID[2], NULL, ierror_CID_unc[2]);
  TGraphErrors errorRate_mant_CID3(numPoints,mantAvg_CID[3], ierror_CID[3], NULL, ierror_CID_unc[3]);
  
  TGraph mant_CID01(numPoints, mantAvg_CID[0],mantAvg_CID[1]);
  TGraph mant_CID02(numPoints, mantAvg_CID[0],mantAvg_CID[2]);
  TGraph mant_CID03(numPoints, mantAvg_CID[0],mantAvg_CID[3]);
  TGraph mant_CID12(numPoints, mantAvg_CID[1],mantAvg_CID[2]);
  TGraph mant_CID13(numPoints, mantAvg_CID[1],mantAvg_CID[3]);
  TGraph mant_CID23(numPoints, mantAvg_CID[2],mantAvg_CID[3]);



  TGraphErrors errorRate_DAC(numPoints,DAC, ierror, NULL, ierror_unc);


  TGraphErrors errorRate_charge(numPoints,charge, ierror, NULL, ierror_unc);
  TGraphErrors errorRate_charge_CID0(numPoints,charge, ierror_CID[0], NULL, ierror_CID_unc[0]);
  TGraphErrors errorRate_charge_CID1(numPoints,charge, ierror_CID[1], NULL, ierror_CID_unc[1]);
  TGraphErrors errorRate_charge_CID2(numPoints,charge, ierror_CID[2], NULL, ierror_CID_unc[2]);
  TGraphErrors errorRate_charge_CID3(numPoints,charge, ierror_CID[3], NULL, ierror_CID_unc[3]);

 
  TCanvas c5;
  
  errorRate_mant.SetMarkerStyle(26);
  errorRate_mant_CID0.SetMarkerStyle(26);
  errorRate_mant_CID1.SetMarkerStyle(26);
  errorRate_mant_CID2.SetMarkerStyle(26);
  errorRate_mant_CID3.SetMarkerStyle(26);

  errorRate_DAC.SetMarkerStyle(26);

  errorRate_charge.SetMarkerStyle(26);
  errorRate_charge_CID0.SetMarkerStyle(26);
  errorRate_charge_CID1.SetMarkerStyle(26);
  errorRate_charge_CID2.SetMarkerStyle(26);
  errorRate_charge_CID3.SetMarkerStyle(26);
  
  errorRate_mant.SetMarkerColor(kRed);
  errorRate_mant_CID0.SetMarkerColor(kRed);
  errorRate_mant_CID1.SetMarkerColor(kRed);
  errorRate_mant_CID2.SetMarkerColor(kRed);
  errorRate_mant_CID3.SetMarkerColor(kRed);

  errorRate_DAC.SetMarkerColor(kRed);

  errorRate_charge.SetMarkerColor(kRed);
  errorRate_charge_CID0.SetMarkerColor(kRed);
  errorRate_charge_CID1.SetMarkerColor(kRed);
  errorRate_charge_CID2.SetMarkerColor(kRed);
  errorRate_charge_CID3.SetMarkerColor(kRed);

  errorRate_mant.SetTitle("Range Error Rate; SuperMantissa; Rate");
  errorRate_mant_CID0.SetTitle("Range Error Rate, CapID 0; SuperMantissa; Rate");
  errorRate_mant_CID1.SetTitle("Range Error Rate, CapID 1; SuperMantissa; Rate");
  errorRate_mant_CID2.SetTitle("Range Error Rate, CapID 2; SuperMantissa; Rate");
  errorRate_mant_CID3.SetTitle("Range Error Rate, CapID 3; SuperMantissa; Rate");

  errorRate_DAC.SetTitle("Range Error Rate; DAC; Rate");

  errorRate_charge.SetTitle("Range Error Rate; Charge (fC); Rate");
  errorRate_charge_CID0.SetTitle("Range Error Rate, CapID 0; Charge (fC); Rate");
  errorRate_charge_CID1.SetTitle("Range Error Rate, CapID 1; Charge (fC); Rate");
  errorRate_charge_CID2.SetTitle("Range Error Rate, CapID 2; Charge (fC); Rate");
  errorRate_charge_CID3.SetTitle("Range Error Rate, CapID 3; Charge (fC); Rate");

  outRoot.cd();
  errorRate_mant.Write("ErrorRate_mant");
  errorRate_mant_CID0.Write("ErrorRate_mant_CID0");
  errorRate_mant_CID1.Write("ErrorRate_mant_CID1");
  errorRate_mant_CID2.Write("ErrorRate_mant_CID2");
  errorRate_mant_CID3.Write("ErrorRate_mant_CID3");
  errorRate_charge.Write("ErrorRate_charge");
  errorRate_charge_CID0.Write("ErrorRate_charge_CID0");
  errorRate_charge_CID1.Write("ErrorRate_charge_CID1");
  errorRate_charge_CID2.Write("ErrorRate_charge_CID2");
  errorRate_charge_CID3.Write("ErrorRate_charge_CID3");
  errorRate_DAC.Write("ErrorRate_DAC");
    
  mant_CID01.Write("MantComp_CID01");
  mant_CID02.Write("MantComp_CID02");
  mant_CID03.Write("MantComp_CID03");
  mant_CID12.Write("MantComp_CID12");
  mant_CID13.Write("MantComp_CID13");
  mant_CID23.Write("MantComp_CID23");

  MantPlot->Write("MantPlot");

  /*
  c5.cd();
  errorRate_mant.Draw("AP");

  sprintf(cTemp,"%s_mant.C",myoutput);
  c5.Print(cTemp,"cxx");
  sprintf(cTemp,"%s_mant.pdf",myoutput);
  c5.Print(cTemp,"pdf");

  errorRate_mant_CID0.Draw("AP");

  sprintf(cTemp,"%s_mant_CID0.C",myoutput);
  c5.Print(cTemp,"cxx");
  sprintf(cTemp,"%s_mant_CID0.pdf",myoutput);
  c5.Print(cTemp,"pdf");

  errorRate_mant_CID1.Draw("AP");

  sprintf(cTemp,"%s_mant_CID1.C",myoutput);
  c5.Print(cTemp,"cxx");
  sprintf(cTemp,"%s_mant_CID1.pdf",myoutput);
  c5.Print(cTemp,"pdf");

  errorRate_mant_CID2.Draw("AP");

  sprintf(cTemp,"%s_mant_CID2.C",myoutput);
  c5.Print(cTemp,"cxx");
  sprintf(cTemp,"%s_mant_CID2.pdf",myoutput);
  c5.Print(cTemp,"pdf");

  errorRate_mant_CID3.Draw("AP");

  sprintf(cTemp,"%s_mant_CID3.C",myoutput);
  c5.Print(cTemp,"cxx");
  sprintf(cTemp,"%s_mant_CID3.pdf",myoutput);
  c5.Print(cTemp,"pdf");

  errorRate_DAC.Draw("AP");

  sprintf(cTemp,"%s_DAC.C",myoutput);
  c5.Print(cTemp,"cxx");
  sprintf(cTemp,"%s_DAC.pdf",myoutput);
  c5.Print(cTemp,"pdf");

  errorRate_charge.Draw("AP");

  sprintf(cTemp,"%s_charge.C",myoutput);
  c5.Print(cTemp,"cxx");
  sprintf(cTemp,"%s_charge.pdf",myoutput);
  c5.Print(cTemp,"pdf");

  errorRate_charge_CID0.Draw("AP");

  sprintf(cTemp,"%s_charge_CID0.C",myoutput);
  c5.Print(cTemp,"cxx");
  sprintf(cTemp,"%s_charge_CID0.pdf",myoutput);
  c5.Print(cTemp,"pdf");

  errorRate_charge_CID1.Draw("AP");

  sprintf(cTemp,"%s_charge_CID1.C",myoutput);
  c5.Print(cTemp,"cxx");
  sprintf(cTemp,"%s_charge_CID1.pdf",myoutput);
  c5.Print(cTemp,"pdf");

  errorRate_charge_CID2.Draw("AP");

  sprintf(cTemp,"%s_charge_CID2.C",myoutput);
  c5.Print(cTemp,"cxx");
  sprintf(cTemp,"%s_charge_CID2.pdf",myoutput);
  c5.Print(cTemp,"pdf");

  errorRate_charge_CID3.Draw("AP");

  sprintf(cTemp,"%s_charge_CID3.C",myoutput);
  c5.Print(cTemp,"cxx");
  sprintf(cTemp,"%s_charge_CID3.pdf",myoutput);
  c5.Print(cTemp,"pdf");

  */

  file.close();

}

void doErrorTest_AlScan(char* myfile, char* myoutput, bool printToScreen = 0, const int numPoints = 128, const int numTests = 2240)
{
  //2240 scan points in new test
  //128, 128, 128 points

  //const int numPoints = 144;
  //const int numTests = 1152; 

  string RangeOut[3] = {"Range01", "Range12","Range23"};

  sprintf(cTemp,"%s.txt",myoutput);
  OutHtml.open(cTemp);
  
  TFile outRoot(myoutput,"RECREATE");
  
  //DCH: Start with reading in the file to produce the data arrays:
  file.open(myfile, ios::in);
  
  if(!file) {
    cout << "Cannot open file!"<<endl;
    exit(1);
  }
  
  int linenumber = 0; //This tracks the line number of the file
  string data; //This is the data contained on the line
  
  int previousline = 0;//We use this to check the consistency of the data file
  
  ////

  int PedCount[4] = {0,0,0,0};
  double PedAvg[4] = {0,0,0,0};
  
  string junk;
  file >> junk;
  //cout << junk << endl;
  file >> junk;
  //cout << junk << endl;
  file >> junk;
  //cout << junk << endl;
  file >> junk;

  while(data != "capid_test"){
    if(!readline(file,linenumber,data,previousline)) return;
    //cout << data << endl;
  } 
  readline(file,linenumber,data,previousline);
  readline(file,linenumber,data,previousline);
  readline(file,linenumber,data,previousline);
  
  int temp1;

  for(int loop = 0; loop <125; loop++) {
    
    readline(file,linenumber,data,previousline);

    temp1 = strtol(data.c_str(),NULL,16);

    PedAvg[(temp1 & 0xC000) >> 14] += ((temp1 & 0x00C0)>>6)*64 + (temp1 & 0x003F);
    PedCount[(temp1 & 0XC000)>>14]++;

  }
   
  for(int x=0; x<4; x++) {
    PedAvg[x]/=PedCount[x];
  }
  
  cout << setfill(' ')   
       << setw(10) << "Ped_CID0" << " " << setw(10) << "Ped_CID1" << " " 
       << setw(10) << "Ped_CID2" << " " << setw(10) << "Ped_CID3" 
       << endl;

  cout << setfill(' ') 
       << setw(10) << fixed << setprecision(2) << PedAvg[0] << " "
       << setw(10) << fixed << setprecision(2) << PedAvg[1] << " "
       << setw(10) << fixed << setprecision(2) << PedAvg[2] << " "
       << setw(10) << fixed << setprecision(2) << PedAvg[3]
       << endl;

  cout << endl;

  
  OutHtml << setfill(' ')   
       << setw(10) << "Ped_CID0" << " " << setw(10) << "Ped_CID1" << " " 
       << setw(10) << "Ped_CID2" << " " << setw(10) << "Ped_CID3" 
       << endl;

  OutHtml << setfill(' ') 
       << setw(10) << fixed << setprecision(2) << PedAvg[0] << " "
       << setw(10) << fixed << setprecision(2) << PedAvg[1] << " "
       << setw(10) << fixed << setprecision(2) << PedAvg[2] << " "
       << setw(10) << fixed << setprecision(2) << PedAvg[3]
       << endl;

  OutHtml << endl;
  ////

  int mantVec[numTests];
  int mantissa;
  int CapID[numTests];
  double mantAvg[numPoints];
  double mantAvg_CID[4][numPoints];
  string DACread;
  double DAC[numPoints];
  double ierror[numPoints];
  double ierror_CID[4][numPoints];
  double ierror_unc[numPoints];
  double ierror_CID_unc[4][numPoints];
  int temp;
  int mantCount[256];
  int mantCount_CID[4][256];
  double charge[numPoints];
  
  int whichRange[3] = {5, 5, 6};
  
  istringstream m;

  for(int scan = 0; scan <3; scan++) {
   
    //OutHtml << setw(4) << DACread << " " << fixed << setw(5) << setfill(' ') << setprecision(0) << DAC[i] << " " 
    //	    << setw(8) << setfill(' ') << fixed << setprecision(2) << charge[i] << " " 
    //	    << setw(3) <<whichMant << " " << setw(6) << setfill(' ') << fixed << setprecision(2) << mantAvg[i] << " " 
    //	    << setw(6) << fixed << setprecision(4) << ierror[i]/numTests << "  "
    //	    << setw(6) << fixed << setprecision(4) << ierror_CID[0][i]/numTests << "  "
    //	    << setw(6) << fixed << setprecision(4) << ierror_CID[1][i]/numTests << "  "
    //	    << setw(6) << fixed << setprecision(4) << ierror_CID[2][i]/numTests << "  "
    //	    << setw(6) << fixed << setprecision(4) << ierror_CID[3][i]/numTests 
    //	    << endl;
    
    cout << RangeOut[scan] << endl;
    
    
    cout << setfill(' ')
	 << setw(6) << "DAC (h)"  << " " << fixed << setw(6) << setfill(' ') << setprecision(0) << "DAC  " << " " 
	 << setw(8) << setfill(' ') << fixed << setprecision(2) << "Charge " << " " 
	 << setw(8) << "ModeMant" << " " 
	 << setw(8) << setfill(' ') << fixed << setprecision(2) << "AvgMant" << " " 
	 << setw(8) << fixed << setprecision(4) << "ErrRate" << "  "
	 << setw(8) << setfill(' ') << fixed << setprecision(2) << "AvgMC0 " << " " 
	 << setw(8) << fixed << setprecision(4) << "ErrC0 " << "  "
	 << setw(8) << setfill(' ') << fixed << setprecision(2) << "AvgMC1 " << " " 
	 << setw(8) << fixed << setprecision(4) << "ErrC1 " << "  "
	 << setw(8) << setfill(' ') << fixed << setprecision(2) << "AvgMC2 " << " " 
	 << setw(8) << fixed << setprecision(4) << "ErrC2 " << "  "
	 << setw(8) << setfill(' ') << fixed << setprecision(2) << "AvgMC3 " << " " 
	 << setw(8) << fixed << setprecision(4) << "ErrC3 "
	 << endl;
    
    OutHtml << RangeOut[scan] << endl;
    
    OutHtml << setfill(' ')
	    << setw(6) << "DAC (h)"  << " " << fixed << setw(6) << setfill(' ') << setprecision(0) << "DAC  " << " " 
	    << setw(8) << setfill(' ') << fixed << setprecision(2) << "Charge " << " " 
	    << setw(8) << "ModeMant" << " " 
	    << setw(8) << setfill(' ') << fixed << setprecision(2) << "AvgMant" << " " 
	    << setw(8) << fixed << setprecision(4) << "ErrRate" << "  "
	    << setw(8) << setfill(' ') << fixed << setprecision(2) << "AvgMC0 " << " " 
	    << setw(8) << fixed << setprecision(4) << "ErrC0 " << "  "
	    << setw(8) << setfill(' ') << fixed << setprecision(2) << "AvgMC1 " << " " 
	    << setw(8) << fixed << setprecision(4) << "ErrC1 " << "  "
	    << setw(8) << setfill(' ') << fixed << setprecision(2) << "AvgMC2 " << " " 
	    << setw(8) << fixed << setprecision(4) << "ErrC2 " << "  "
	    << setw(8) << setfill(' ') << fixed << setprecision(2) << "AvgMC3 " << " " 
	    << setw(8) << fixed << setprecision(4) << "ErrC3 "
	    << endl;

    while(data != "mant_test_errors"){
      if(!readline(file,linenumber,data,previousline)) return;
    }  
  
    for(int i=0; i<numPoints; i++) {
      readline(file,linenumber,data,previousline);
      //cout << data << endl;
      
      //DACread = data;    
      DACread = data.substr(7,4);
      readline(file,linenumber,data,previousline);
      
      DAC[i] = 0;
      charge[i] = 0;
      mantAvg[i] = 0;
      mantAvg_CID[0][i] = 0;
      mantAvg_CID[1][i] = 0;
      mantAvg_CID[2][i] = 0;
      mantAvg_CID[3][i] = 0;
      ierror[i] = 0;
      ierror_CID[0][i] = 0;
      ierror_CID[1][i] = 0;
      ierror_CID[2][i] = 0;
      ierror_CID[3][i] = 0;
      ierror_unc[i] = 0;
      ierror_CID_unc[0][i] = 0;
      ierror_CID_unc[1][i] = 0;
      ierror_CID_unc[2][i] = 0;
      ierror_CID_unc[3][i] = 0;
      
      DAC[i] = strtol(DACread.c_str(),NULL, 16);
      charge[i] = LookupDAQ(DAC[i],whichRange[scan]);
      
      for(int k=0; k< 256; k++) {
	mantCount[k] = 0;
	mantCount_CID[0][k] = 0;
	mantCount_CID[1][k] = 0;
	mantCount_CID[2][k] = 0;
	mantCount_CID[3][k] = 0;
	
      }
      
      for(int j=0; j<numTests; j++) {
	readline(file,linenumber,data,previousline);
	
	//cout << data << " ";
	
	temp = 0;
	
	//m.str(data);
	//cout << m.str() << endl;
	//m >> std::hex >> temp;
	//m >> hex >> temp;
	//m.str(std::string());
	
	//istringstream(data) >> hex >> temp;
	temp = strtol(data.c_str(),NULL,16);
	
	//cout << temp << endl;
	
	mantissa = ((temp & 0x00C0) >> 6)*64+(temp & 0x003F);
	CapID[j]    = (temp & 0xC000) >> 14;
	
	//MantValue.Fill(mantissa);
	
	mantVec[j] = mantissa;
	mantAvg[i] += mantissa;	
	
	mantCount[mantissa]++;
	mantCount_CID[CapID[j]][mantissa]++;
	
	
	//cout << mantissa << " " << mantAvg[i] << endl;
	
      }
      
      readline_cap(file,linenumber,data,previousline);
      readline_cap(file,linenumber,data,previousline);
      readline_cap(file,linenumber,data,previousline);
      readline_cap(file,linenumber,data,previousline);
      
      //cout << mantAvg[i] << endl;
      
      mantAvg[i] /= numTests;
      
      
      int max = 0;
      int whichMant = 0;
      
      int max_CID[4] = {0,0,0,0};
      int whichMant_CID[4] = {0,0,0,0};
      
      for(int k=0; k<256; k++) {
	if(mantCount[k] > max) {
	  //max = k;
	  max = mantCount[k];
	  //mantAvg[i] = k;
	  whichMant = k;
	}
	
	for(int q=0; q<4; q++) {
	  if(mantCount_CID[q][k] > max_CID[q]) {
	    //max_CID[q] = k;
	    max_CID[q] = mantCount_CID[q][k];
	    whichMant_CID[q] = k;
	  }
	  
	}
      }
      
      int denom = 0;
      int denom_CID[4] = {0,0,0,0};
      
      mantAvg[i] = 0;
      mantAvg_CID[0][i] = 0;
      mantAvg_CID[1][i] = 0;
      mantAvg_CID[2][i] = 0;
      mantAvg_CID[3][i] = 0;
      
      for(int k=0; k<256; k++) {
	if(abs(k-whichMant) < 7) {
	  mantAvg[i] += k*mantCount[k];
	  denom += mantCount[k];
	}
	
	for(int q = 0; q<4; q++) {
	  //if(abs(k-whichMant_CID[q]) < 7) {
	  if(abs(k-whichMant) < 7) {
	    mantAvg_CID[q][i] += k*mantCount_CID[q][k];
	    denom_CID[q] += mantCount_CID[q][k];
	  }
	}
	
      }
      
      mantAvg[i]/=denom;
      mantAvg_CID[0][i]/= denom_CID[0];
      mantAvg_CID[1][i]/= denom_CID[1];
      mantAvg_CID[2][i]/= denom_CID[2];
      mantAvg_CID[3][i]/= denom_CID[3];
      
      
      //MantValue.Fit("MantGaus","Q");
      //mantAvg[i] = MantGaus.GetParameter(1);
      
      for(int j=0; j<numTests; j++) {
	
	if(abs(mantVec[j]-mantAvg[i]) > 6) {
	  ierror[i]++;
	  //if(abs(mantVec[j]-mantAvg[i]) > 6) ierror_CID[CapID[j]][i]++;
	  ierror_CID[CapID[j]][i]++;
	}
	
      }
      
      //OutHtml << DACread << " " << mantAvg << " " << ierror << endl;
      OutHtml << setfill(' ')
	      << setw(6) << DACread << " " << fixed << setw(6) << setfill(' ') << setprecision(0) << DAC[i] << " " 
	      << setw(8) << setfill(' ') << fixed << setprecision(2) << charge[i] << " " 
	      << setw(8) <<whichMant << " " 
	      << setw(8) << setfill(' ') << fixed << setprecision(2) << mantAvg[i] << " " 
	      << setw(8) << fixed << setprecision(4) << ierror[i]/numTests << "  "
	      << setw(8) << setfill(' ') << fixed << setprecision(2) << mantAvg_CID[0][i] << " " 
	      << setw(8) << fixed << setprecision(4) << ierror_CID[0][i]/(numTests/4) << "  "
	      << setw(8) << setfill(' ') << fixed << setprecision(2) << mantAvg_CID[1][i] << " " 
	      << setw(8) << fixed << setprecision(4) << ierror_CID[1][i]/(numTests/4) << "  "
	      << setw(8) << setfill(' ') << fixed << setprecision(2) << mantAvg_CID[2][i] << " " 
	      << setw(8) << fixed << setprecision(4) << ierror_CID[2][i]/(numTests/4) << "  "
	      << setw(8) << setfill(' ') << fixed << setprecision(2) << mantAvg_CID[3][i] << " " 
	      << setw(8) << fixed << setprecision(4) << ierror_CID[3][i]/(numTests/4) 
	      << endl;
      
      //OutHtml << setw(6) << DACread << " " << fixed << setw(5) << setfill(' ') << setprecision(0) << DAC[i] << " " 
      //	    << setw(8) << setfill(' ') << fixed << setprecision(2) << charge[i] << " " 
      //	    << setw(3) <<whichMant << " " << setw(6) << setfill(' ') << fixed << setprecision(2) << mantAvg[i] << " " 
      //	    << setw(6) << fixed << setprecision(4) << ierror[i]/numTests << "  "
      //	    << setw(6) << fixed << setprecision(4) << ierror_CID[0][i]/numTests << "  "
      //	    << setw(6) << fixed << setprecision(4) << ierror_CID[1][i]/numTests << "  "
      //	    << setw(6) << fixed << setprecision(4) << ierror_CID[2][i]/numTests << "  "
      //	    << setw(6) << fixed << setprecision(4) << ierror_CID[3][i]/numTests 
      //	    << endl;
      
      
      if(printToScreen) {
	//cout << setw(4) << DACread << " " << fixed << setw(5) << setfill(' ') << setprecision(0) << DAC[i] << " " << setw(8) << setfill(' ') << fixed << setprecision(2) << charge[i] << " " 
	//   << setw(3) <<whichMant << " " << setw(6) << setfill(' ') << fixed << setprecision(2) << mantAvg[i] << " " 
	//   << setw(6) << fixed << setprecision(4) << ierror[i]/numTests << "  "
	//   << setw(6) << fixed << setprecision(4) << ierror_CID[0][i]/(numTests/4) << "  "
	//   << setw(6) << fixed << setprecision(4) << ierror_CID[1][i]/(numTests/4) << "  "
	//   << setw(6) << fixed << setprecision(4) << ierror_CID[2][i]/(numTests/4) << "  "
	//   << setw(6) << fixed << setprecision(4) << ierror_CID[3][i]/(numTests/4) 	
	//   << endl;
	
	cout<< setfill(' ')
	    << setw(6) << DACread << " " << fixed << setw(6) << setfill(' ') << setprecision(0) << DAC[i] << " " 
	    << setw(8) << setfill(' ') << fixed << setprecision(2) << charge[i] << " " 
	    << setw(8) <<whichMant << " " 
	    << setw(8) << setfill(' ') << fixed << setprecision(2) << mantAvg[i] << " " 
	    << setw(8) << fixed << setprecision(4) << ierror[i]/numTests << "  "
	    << setw(8) << setfill(' ') << fixed << setprecision(2) << mantAvg_CID[0][i] << " " 
	    << setw(8) << fixed << setprecision(4) << ierror_CID[0][i]/(numTests/4) << "  "
	    << setw(8) << setfill(' ') << fixed << setprecision(2) << mantAvg_CID[1][i] << " " 
	    << setw(8) << fixed << setprecision(4) << ierror_CID[1][i]/(numTests/4) << "  "
	    << setw(8) << setfill(' ') << fixed << setprecision(2) << mantAvg_CID[2][i] << " " 
	    << setw(8) << fixed << setprecision(4) << ierror_CID[2][i]/(numTests/4) << "  "
	    << setw(8) << setfill(' ') << fixed << setprecision(2) << mantAvg_CID[3][i] << " " 
	    << setw(8) << fixed << setprecision(4) << ierror_CID[3][i]/(numTests/4) 
	    << endl;
      }
    
    ierror[i]/=numTests;
    ierror_CID[0][i] /= numTests/4;
    ierror_CID[1][i] /= numTests/4;
    ierror_CID[2][i] /= numTests/4;
    ierror_CID[3][i] /= numTests/4;


    ierror_unc[i] = sqrt(ierror[i]*(1-ierror[i])/numTests);
    ierror_CID_unc[0][i] = sqrt(ierror_CID[0][i]*(1-ierror_CID[0][i])/(numTests/4));
    ierror_CID_unc[1][i] = sqrt(ierror_CID[1][i]*(1-ierror_CID[1][i])/(numTests/4));
    ierror_CID_unc[2][i] = sqrt(ierror_CID[2][i]*(1-ierror_CID[2][i])/(numTests/4));
    ierror_CID_unc[3][i] = sqrt(ierror_CID[3][i]*(1-ierror_CID[3][i])/(numTests/4));
    

  }
  
  //TGraph errorRate(numPoints,mantAvg, ierror);
  TGraphErrors errorRate_mant(numPoints,mantAvg, ierror, NULL, ierror_unc);
  TGraphErrors errorRate_mant_CID0(numPoints,mantAvg_CID[0], ierror_CID[0], NULL, ierror_CID_unc[0]);
  TGraphErrors errorRate_mant_CID1(numPoints,mantAvg_CID[1], ierror_CID[1], NULL, ierror_CID_unc[1]);
  TGraphErrors errorRate_mant_CID2(numPoints,mantAvg_CID[2], ierror_CID[2], NULL, ierror_CID_unc[2]);
  TGraphErrors errorRate_mant_CID3(numPoints,mantAvg_CID[3], ierror_CID[3], NULL, ierror_CID_unc[3]);
  


  TGraphErrors errorRate_DAC(numPoints,DAC, ierror, NULL, ierror_unc);


  TGraphErrors errorRate_charge(numPoints,charge, ierror, NULL, ierror_unc);
  TGraphErrors errorRate_charge_CID0(numPoints,charge, ierror_CID[0], NULL, ierror_CID_unc[0]);
  TGraphErrors errorRate_charge_CID1(numPoints,charge, ierror_CID[1], NULL, ierror_CID_unc[1]);
  TGraphErrors errorRate_charge_CID2(numPoints,charge, ierror_CID[2], NULL, ierror_CID_unc[2]);
  TGraphErrors errorRate_charge_CID3(numPoints,charge, ierror_CID[3], NULL, ierror_CID_unc[3]);

 
  TCanvas c5;
  
  errorRate_mant.SetMarkerStyle(26);
  errorRate_mant_CID0.SetMarkerStyle(26);
  errorRate_mant_CID1.SetMarkerStyle(26);
  errorRate_mant_CID2.SetMarkerStyle(26);
  errorRate_mant_CID3.SetMarkerStyle(26);

  errorRate_DAC.SetMarkerStyle(26);

  errorRate_charge.SetMarkerStyle(26);
  errorRate_charge_CID0.SetMarkerStyle(26);
  errorRate_charge_CID1.SetMarkerStyle(26);
  errorRate_charge_CID2.SetMarkerStyle(26);
  errorRate_charge_CID3.SetMarkerStyle(26);
  
  errorRate_mant.SetMarkerColor(kRed);
  errorRate_mant_CID0.SetMarkerColor(kRed);
  errorRate_mant_CID1.SetMarkerColor(kRed);
  errorRate_mant_CID2.SetMarkerColor(kRed);
  errorRate_mant_CID3.SetMarkerColor(kRed);

  errorRate_DAC.SetMarkerColor(kRed);

  errorRate_charge.SetMarkerColor(kRed);
  errorRate_charge_CID0.SetMarkerColor(kRed);
  errorRate_charge_CID1.SetMarkerColor(kRed);
  errorRate_charge_CID2.SetMarkerColor(kRed);
  errorRate_charge_CID3.SetMarkerColor(kRed);

  errorRate_mant.SetTitle("Range Error Rate; SuperMantissa; Rate");
  errorRate_mant_CID0.SetTitle("Range Error Rate, CapID 0; SuperMantissa; Rate");
  errorRate_mant_CID1.SetTitle("Range Error Rate, CapID 1; SuperMantissa; Rate");
  errorRate_mant_CID2.SetTitle("Range Error Rate, CapID 2; SuperMantissa; Rate");
  errorRate_mant_CID3.SetTitle("Range Error Rate, CapID 3; SuperMantissa; Rate");

  errorRate_DAC.SetTitle("Range Error Rate; DAC; Rate");

  errorRate_charge.SetTitle("Range Error Rate; Charge (fC); Rate");
  errorRate_charge_CID0.SetTitle("Range Error Rate, CapID 0; Charge (fC); Rate");
  errorRate_charge_CID1.SetTitle("Range Error Rate, CapID 1; Charge (fC); Rate");
  errorRate_charge_CID2.SetTitle("Range Error Rate, CapID 2; Charge (fC); Rate");
  errorRate_charge_CID3.SetTitle("Range Error Rate, CapID 3; Charge (fC); Rate");

  //cout << "still alive" << endl;

  outRoot.cd();

  sprintf(cTemp,"ErrorRate_mant_%s.txt",RangeOut[scan].c_str());
  errorRate_mant.Write(cTemp);
  sprintf(cTemp,"ErrorRate_mant_CID0_%s.txt",RangeOut[scan].c_str());
  errorRate_mant_CID0.Write(cTemp);
  sprintf(cTemp,"ErrorRate_mant_CID1_%s.txt",RangeOut[scan].c_str());
  errorRate_mant_CID1.Write(cTemp);
  sprintf(cTemp,"ErrorRate_mant_CID2_%s.txt",RangeOut[scan].c_str());
  errorRate_mant_CID2.Write(cTemp);
  sprintf(cTemp,"ErrorRate_mant_CID3_%s.txt",RangeOut[scan].c_str());
  errorRate_mant_CID3.Write(cTemp);

  sprintf(cTemp,"ErrorRate_charge_%s.txt",RangeOut[scan].c_str());
  errorRate_charge.Write(cTemp);
  sprintf(cTemp,"ErrorRate_charge_CID0_%s.txt",RangeOut[scan].c_str());
  errorRate_charge_CID0.Write(cTemp);
  sprintf(cTemp,"ErrorRate_charge_CID1_%s.txt",RangeOut[scan].c_str());
  errorRate_charge_CID1.Write(cTemp);
  sprintf(cTemp,"ErrorRate_charge_CID2_%s.txt",RangeOut[scan].c_str());
  errorRate_charge_CID2.Write(cTemp);
  sprintf(cTemp,"ErrorRate_charge_CID3_%s.txt",RangeOut[scan].c_str());
  errorRate_charge_CID3.Write(cTemp);

  sprintf(cTemp,"ErrorRate_DAC_%s.txt",RangeOut[scan].c_str());
  errorRate_DAC.Write(cTemp);

  //errorRate_mant.Write("ErrorRate_mant");
  //errorRate_mant_CID0.Write("ErrorRate_mant_CID0");
  //errorRate_mant_CID1.Write("ErrorRate_mant_CID1");
  //errorRate_mant_CID2.Write("ErrorRate_mant_CID2");
  //errorRate_mant_CID3.Write("ErrorRate_mant_CID3");
  //errorRate_charge.Write("ErrorRate_charge");
  //errorRate_charge_CID0.Write("ErrorRate_charge_CID0");
  //errorRate_charge_CID1.Write("ErrorRate_charge_CID1");
  //errorRate_charge_CID2.Write("ErrorRate_charge_CID2");
  //errorRate_charge_CID3.Write("ErrorRate_charge_CID3");
  //errorRate_DAC.Write("ErrorRate_DAC");

  //cout << "finished with scan: " << scan << ", moving on" << endl;

  cout << endl;
  cout << endl;

  OutHtml << endl;
  OutHtml << endl;

  }
    
  file.close();

}

////

void doErrorTest_AlScan_loop(char* myfile, char* myoutput, bool printToScreen = 0, const int numPoints = 128, const int numTests = 2240)
{
  //2240 scan points in new test
  //128, 128, 128 points

  //const int numPoints = 144;
  //const int numTests = 1152; 

  string RangeOut[3] = {"Range01", "Range12","Range23"};
  string line;

  //sprintf(cTemp,"%s.txt",myoutput);
  //OutHtml.open(cTemp);
  
  TFile outRoot(myoutput,"RECREATE");
  
  TH1F maxError_tot("maxError_tot","Max Error Rate, All Overlaps",500, 0, 1);
  TH1F maxError_R01("maxError_R01","Max Error Rate, Overlaps 01",500, 0, 1);
  TH1F maxError_R12("maxError_R12","Max Error Rate, Overlaps 12",500, 0, 1);
  TH1F maxError_R23("maxError_R23","Max Error Rate, Overlaps 23",500, 0, 1);

  TH1F gAmpError_R01("gAmpError_R01","gAmp Error Rate, Overlaps 01",500, 0, 1);
  TH1F gAmpError_R12("gAmpError_R12","gAmp Error Rate, Overlaps 12",500, 0, 1);
  TH1F gAmpError_R23("gAmpError_R23","gAmp Error Rate, Overlaps 23",500, 0, 1);

  TH1F gAmpError_R01_CID0("gAmpError_R01_CID0","gAmp Error Rate, Overlaps 01, CID 0",500, 0, 1);
  TH1F gAmpError_R12_CID0("gAmpError_R12_CID0","gAmp Error Rate, Overlaps 12, CID 0",500, 0, 1);
  TH1F gAmpError_R23_CID0("gAmpError_R23_CID0","gAmp Error Rate, Overlaps 23, CID 0",500, 0, 1);

  TH1F gMeanError_R01_CID0("gMeanError_R01_CID0","gMean Error Rate, Overlaps 01, CID 0",25, 550, 650);
  TH1F gMeanError_R12_CID0("gMeanError_R12_CID0","gMean Error Rate, Overlaps 12, CID 0",25, 5000, 5500);
  TH1F gMeanError_R23_CID0("gMeanError_R23_CID0","gMean Error Rate, Overlaps 23, CID 0",25, 40000, 46000);

  TH1F gWidError_R01_CID0("gWidError_R01_CID0","gWid Error Rate, Overlaps 01, CID 0",100, 0, 20);
  TH1F gWidError_R12_CID0("gWidError_R12_CID0","gWid Error Rate, Overlaps 12, CID 0",100, 0, 100);
  TH1F gWidError_R23_CID0("gWidError_R23_CID0","gWid Error Rate, Overlaps 23, CID 0",100, 0, 1000);

  TH1F gAmpError_R01_CID1("gAmpError_R01_CID1","gAmp Error Rate, Overlaps 01, CID 1",500, 0, 1);
  TH1F gAmpError_R12_CID1("gAmpError_R12_CID1","gAmp Error Rate, Overlaps 12, CID 1",500, 0, 1);
  TH1F gAmpError_R23_CID1("gAmpError_R23_CID1","gAmp Error Rate, Overlaps 23, CID 1",500, 0, 1);

  TH1F gMeanError_R01_CID1("gMeanError_R01_CID1","gMean Error Rate, Overlaps 01, CID 1",25, 550, 650);
  TH1F gMeanError_R12_CID1("gMeanError_R12_CID1","gMean Error Rate, Overlaps 12, CID 1",25, 5000, 5500);
  TH1F gMeanError_R23_CID1("gMeanError_R23_CID1","gMean Error Rate, Overlaps 23, CID 1",25, 40000, 46000);

  TH1F gWidError_R01_CID1("gWidError_R01_CID1","gWid Error Rate, Overlaps 01, CID 1",100, 0, 20);
  TH1F gWidError_R12_CID1("gWidError_R12_CID1","gWid Error Rate, Overlaps 12, CID 1",100, 0, 100);
  TH1F gWidError_R23_CID1("gWidError_R23_CID1","gWid Error Rate, Overlaps 23, CID 1",100, 0, 1000);

  TH1F gAmpError_R01_CID2("gAmpError_R01_CID2","gAmp Error Rate, Overlaps 01, CID 2",500, 0, 1);
  TH1F gAmpError_R12_CID2("gAmpError_R12_CID2","gAmp Error Rate, Overlaps 12, CID 2",500, 0, 1);
  TH1F gAmpError_R23_CID2("gAmpError_R23_CID2","gAmp Error Rate, Overlaps 23, CID 2",500, 0, 1);

  TH1F gMeanError_R01_CID2("gMeanError_R01_CID2","gMean Error Rate, Overlaps 01, CID 2",25, 550, 650);
  TH1F gMeanError_R12_CID2("gMeanError_R12_CID2","gMean Error Rate, Overlaps 12, CID 2",25, 5000, 5500);
  TH1F gMeanError_R23_CID2("gMeanError_R23_CID2","gMean Error Rate, Overlaps 23, CID 2",25, 40000, 46000);

  TH1F gWidError_R01_CID2("gWidError_R01_CID2","gWid Error Rate, Overlaps 01, CID 2",100, 0, 20);
  TH1F gWidError_R12_CID2("gWidError_R12_CID2","gWid Error Rate, Overlaps 12, CID 2",100, 0, 100);
  TH1F gWidError_R23_CID2("gWidError_R23_CID2","gWid Error Rate, Overlaps 23, CID 2",100, 0, 1000);

  TH1F gAmpError_R01_CID3("gAmpError_R01_CID3","gAmp Error Rate, Overlaps 01, CID 3",500, 0, 1);
  TH1F gAmpError_R12_CID3("gAmpError_R12_CID3","gAmp Error Rate, Overlaps 12, CID 3",500, 0, 1);
  TH1F gAmpError_R23_CID3("gAmpError_R23_CID3","gAmp Error Rate, Overlaps 23, CID 3",500, 0, 1);

  TH1F gMeanError_R01_CID3("gMeanError_R01_CID3","gMean Error Rate, Overlaps 01, CID 3",25, 550, 650);
  TH1F gMeanError_R12_CID3("gMeanError_R12_CID3","gMean Error Rate, Overlaps 12, CID 3",25, 5000, 5500);
  TH1F gMeanError_R23_CID3("gMeanError_R23_CID3","gMean Error Rate, Overlaps 23, CID 3",25, 40000, 46000);

  TH1F gWidError_R01_CID3("gWidError_R01_CID3","gWid Error Rate, Overlaps 01, CID 3",100, 0, 20);
  TH1F gWidError_R12_CID3("gWidError_R12_CID3","gWid Error Rate, Overlaps 12, CID 3",100, 0, 100);
  TH1F gWidError_R23_CID3("gWidError_R23_CID3","gWid Error Rate, Overlaps 23, CID 3",100, 0, 1000);

  TH1F whichError_1("whichError_1","Which Transition Fails 1\%", 10, 0.5, 10.5);
  TH1F whichError_5("whichError_5","Which Transition Fails 5\%", 10, 0.5, 10.5);
  TH1F whichError_10("whichError_10","Which Transition Fails 10\%", 10, 0.5, 10.5);

  TH2D ErrorRateVsGaus("ErrorRateVsGaus","Max Error Rate vs Max Gaus Amp",500, 0, 1, 500, 0, 1);

  TH1F FullChipError("FullChipError","Full Chip Error Rate",1000, 0, 0.001);

  whichError_1.GetXaxis()->SetBinLabel(1, "Good");
  whichError_1.GetXaxis()->SetBinLabel(2, "R01 Error");
  whichError_1.GetXaxis()->SetBinLabel(3, "R12 Error");
  whichError_1.GetXaxis()->SetBinLabel(4, "R23 Error");
  whichError_1.GetXaxis()->SetBinLabel(5, "R01/R12");
  whichError_1.GetXaxis()->SetBinLabel(6, "R01/R23");
  whichError_1.GetXaxis()->SetBinLabel(7, "R12/R23");
  whichError_1.GetXaxis()->SetBinLabel(8, "AllOverlaps");

  whichError_5.GetXaxis()->SetBinLabel(1, "Good");
  whichError_5.GetXaxis()->SetBinLabel(2, "R01 Error");
  whichError_5.GetXaxis()->SetBinLabel(3, "R12 Error");
  whichError_5.GetXaxis()->SetBinLabel(4, "R23 Error");
  whichError_5.GetXaxis()->SetBinLabel(5, "R01/R12");
  whichError_5.GetXaxis()->SetBinLabel(6, "R01/R23");
  whichError_5.GetXaxis()->SetBinLabel(7, "R12/R23");
  whichError_5.GetXaxis()->SetBinLabel(8, "AllOverlaps");


  whichError_10.GetXaxis()->SetBinLabel(1, "Good");
  whichError_10.GetXaxis()->SetBinLabel(2, "R01 Error");
  whichError_10.GetXaxis()->SetBinLabel(3, "R12 Error");
  whichError_10.GetXaxis()->SetBinLabel(4, "R23 Error");
  whichError_10.GetXaxis()->SetBinLabel(5, "R01/R12");
  whichError_10.GetXaxis()->SetBinLabel(6, "R01/R23");
  whichError_10.GetXaxis()->SetBinLabel(7, "R12/R23");
  whichError_10.GetXaxis()->SetBinLabel(8, "AllOverlaps");




  //DCH: Start with reading in the file to produce the data arrays:
  //file.open(myfile, ios::in);
 

  double maxErr = 0;
  double maxErr_R01 = 0;
  double maxErr_R12 = 0;
  double maxErr_R23 = 0;

  TF1 GausR01_CID0("GausR01_CID0","gaus",570, 610);
  TF1 GausR12_CID0("GausR12_CID0","gaus",5050, 5350);
  TF1 GausR23_CID0("GausR23_CID0","gaus",38000, 43000);

  TF1 GausR01_CID1("GausR01_CID1","gaus",570, 610);
  TF1 GausR12_CID1("GausR12_CID1","gaus",5050, 5350);
  TF1 GausR23_CID1("GausR23_CID1","gaus",38000, 43000);

  TF1 GausR01_CID2("GausR01_CID2","gaus",570, 610);
  TF1 GausR12_CID2("GausR12_CID2","gaus",5050, 5350);
  TF1 GausR23_CID2("GausR23_CID2","gaus",38000, 43000);

  TF1 GausR01_CID3("GausR01_CID3","gaus",570, 610);
  TF1 GausR12_CID3("GausR12_CID3","gaus",5050, 5350);
  TF1 GausR23_CID3("GausR23_CID3","gaus",38000, 43000);


  ifstream mylist(myfile);

  int counter = 0;

  double whichErr1 = 0;
  double whichErr5 = 0;
  double whichErr10 = 0;
  
  if(mylist.is_open()) {

    while(getline(mylist,line)) {

      counter++;

      whichErr1 = 0;
      whichErr5 = 0;
      whichErr10 = 0;

      maxErr = 0;
      maxErr_R01 = 0;
      maxErr_R12 = 0;
      maxErr_R23 = 0;
      
      GausR01_CID0.SetParameters(0,550,4);
      GausR12_CID0.SetParameters(0,5200,25);
      GausR23_CID0.SetParameters(0,40000,250);

      GausR01_CID1.SetParameters(0,550,4);
      GausR12_CID1.SetParameters(0,5200,25);
      GausR23_CID1.SetParameters(0,40000,250);

      GausR01_CID2.SetParameters(0,550,4);
      GausR12_CID2.SetParameters(0,5200,25);
      GausR23_CID2.SetParameters(0,40000,250);

      GausR01_CID3.SetParameters(0,550,4);
      GausR12_CID3.SetParameters(0,5200,25);
      GausR23_CID3.SetParameters(0,40000,250);

      file.open(line.c_str(), ios::in);
      
      if(!file) {
	cout << "Cannot open file!"<<endl;
	exit(1);
      }
      
      int linenumber = 0; //This tracks the line number of the file
      string data; //This is the data contained on the line
      
      int previousline = 0;//We use this to check the consistency of the data file
      
      int PedCount[4] = {0,0,0,0};
      double PedAvg[4] = {0,0,0,0};
      
      string junk;
      file >> junk;
      //cout << junk << endl;
      file >> junk;
      //cout << junk << endl;
      file >> junk;
      //cout << junk << endl;
      file >> junk;
      
      while(data != "capid_test"){
	if(!readline(file,linenumber,data,previousline)) return;
	//cout << data << endl;
      } 
      readline(file,linenumber,data,previousline);
      readline(file,linenumber,data,previousline);
      readline(file,linenumber,data,previousline);
      
      int temp1;
      
      for(int loop = 0; loop <125; loop++) {
	
	readline(file,linenumber,data,previousline);
	
	temp1 = strtol(data.c_str(),NULL,16);
	
	PedAvg[(temp1 & 0xC000) >> 14] += ((temp1 & 0x00C0)>>6)*64 + (temp1 & 0x003F);
	PedCount[(temp1 & 0XC000)>>14]++;
	
      }
      
      for(int x=0; x<4; x++) {
	PedAvg[x]/=PedCount[x];
      }
      
      //cout << setfill(' ')   
      //   << setw(10) << "Ped_CID0" << " " << setw(10) << "Ped_CID1" << " " 
      //   << setw(10) << "Ped_CID2" << " " << setw(10) << "Ped_CID3" 
      //   << endl;
      //
      //cout << setfill(' ') 
      //   << setw(10) << fixed << setprecision(2) << PedAvg[0] << " "
      //   << setw(10) << fixed << setprecision(2) << PedAvg[1] << " "
      //   << setw(10) << fixed << setprecision(2) << PedAvg[2] << " "
      //   << setw(10) << fixed << setprecision(2) << PedAvg[3]
      //   << endl;
      //
      //cout << endl;
      
      
      //OutHtml << setfill(' ')   
      //   << setw(10) << "Ped_CID0" << " " << setw(10) << "Ped_CID1" << " " 
      //   << setw(10) << "Ped_CID2" << " " << setw(10) << "Ped_CID3" 
      //   << endl;
      //
      //OutHtml << setfill(' ') 
      //   << setw(10) << fixed << setprecision(2) << PedAvg[0] << " "
      //   << setw(10) << fixed << setprecision(2) << PedAvg[1] << " "
      //   << setw(10) << fixed << setprecision(2) << PedAvg[2] << " "
      //   << setw(10) << fixed << setprecision(2) << PedAvg[3]
      //   << endl;
      //
      //OutHtml << endl;
      ////
      
      int mantVec[numTests];
      int mantissa;
      int CapID[numTests];
      double mantAvg[numPoints];
      double mantAvg_CID[4][numPoints];
      string DACread;
      double DAC[numPoints];
      double ierror[numPoints];
      double ierror_CID[4][numPoints];
      double ierror_unc[numPoints];
      double ierror_CID_unc[4][numPoints];
      int temp;
      int mantCount[256];
      int mantCount_CID[4][256];
      double charge[numPoints];
      
      int whichRange[3] = {5, 5, 6};
      
      istringstream m;
      
      for(int scan = 0; scan <3; scan++) {
	
	//OutHtml << setw(4) << DACread << " " << fixed << setw(5) << setfill(' ') << setprecision(0) << DAC[i] << " " 
	//	    << setw(8) << setfill(' ') << fixed << setprecision(2) << charge[i] << " " 
	//	    << setw(3) <<whichMant << " " << setw(6) << setfill(' ') << fixed << setprecision(2) << mantAvg[i] << " " 
	//	    << setw(6) << fixed << setprecision(4) << ierror[i]/numTests << "  "
	//	    << setw(6) << fixed << setprecision(4) << ierror_CID[0][i]/numTests << "  "
	//	    << setw(6) << fixed << setprecision(4) << ierror_CID[1][i]/numTests << "  "
	//	    << setw(6) << fixed << setprecision(4) << ierror_CID[2][i]/numTests << "  "
	//	    << setw(6) << fixed << setprecision(4) << ierror_CID[3][i]/numTests 
	//	    << endl;
	
	//cout << RangeOut[scan] << endl;
	//	
	//cout << setfill(' ')
	//   << setw(6) << "DAC (h)"  << " " << fixed << setw(6) << setfill(' ') << setprecision(0) << "DAC  " << " " 
	//   << setw(8) << setfill(' ') << fixed << setprecision(2) << "Charge " << " " 
	//   << setw(8) << "ModeMant" << " " 
	//   << setw(8) << setfill(' ') << fixed << setprecision(2) << "AvgMant" << " " 
	//   << setw(8) << fixed << setprecision(4) << "ErrRate" << "  "
	//   << setw(8) << setfill(' ') << fixed << setprecision(2) << "AvgMC0 " << " " 
	//   << setw(8) << fixed << setprecision(4) << "ErrC0 " << "  "
	//   << setw(8) << setfill(' ') << fixed << setprecision(2) << "AvgMC1 " << " " 
	//   << setw(8) << fixed << setprecision(4) << "ErrC1 " << "  "
	//   << setw(8) << setfill(' ') << fixed << setprecision(2) << "AvgMC2 " << " " 
	//   << setw(8) << fixed << setprecision(4) << "ErrC2 " << "  "
	//   << setw(8) << setfill(' ') << fixed << setprecision(2) << "AvgMC3 " << " " 
	//   << setw(8) << fixed << setprecision(4) << "ErrC3 "
	//   << endl;
	
	//OutHtml << RangeOut[scan] << endl;
	//
	//OutHtml << setfill(' ')
	//	    << setw(6) << "DAC (h)"  << " " << fixed << setw(6) << setfill(' ') << setprecision(0) << "DAC  " << " " 
	//	    << setw(8) << setfill(' ') << fixed << setprecision(2) << "Charge " << " " 
	//	    << setw(8) << "ModeMant" << " " 
	//	    << setw(8) << setfill(' ') << fixed << setprecision(2) << "AvgMant" << " " 
	//	    << setw(8) << fixed << setprecision(4) << "ErrRate" << "  "
	//	    << setw(8) << setfill(' ') << fixed << setprecision(2) << "AvgMC0 " << " " 
	//	    << setw(8) << fixed << setprecision(4) << "ErrC0 " << "  "
	//	    << setw(8) << setfill(' ') << fixed << setprecision(2) << "AvgMC1 " << " " 
	//	    << setw(8) << fixed << setprecision(4) << "ErrC1 " << "  "
	//	    << setw(8) << setfill(' ') << fixed << setprecision(2) << "AvgMC2 " << " " 
	//	    << setw(8) << fixed << setprecision(4) << "ErrC2 " << "  "
	//	    << setw(8) << setfill(' ') << fixed << setprecision(2) << "AvgMC3 " << " " 
	//	    << setw(8) << fixed << setprecision(4) << "ErrC3 "
	//	    << endl;
	
	while(data != "mant_test_errors"){
	  if(!readline(file,linenumber,data,previousline)) return;
	}  
	
	for(int i=0; i<numPoints; i++) {
	  readline(file,linenumber,data,previousline);
	  //cout << data << endl;
	  
	  //DACread = data;    
	  DACread = data.substr(7,4);
	  readline(file,linenumber,data,previousline);
	  
	  DAC[i] = 0;
	  charge[i] = 0;
	  mantAvg[i] = 0;
	  mantAvg_CID[0][i] = 0;
	  mantAvg_CID[1][i] = 0;
	  mantAvg_CID[2][i] = 0;
	  mantAvg_CID[3][i] = 0;
	  ierror[i] = 0;
	  ierror_CID[0][i] = 0;
	  ierror_CID[1][i] = 0;
	  ierror_CID[2][i] = 0;
	  ierror_CID[3][i] = 0;
	  ierror_unc[i] = 0;
	  ierror_CID_unc[0][i] = 0;
	  ierror_CID_unc[1][i] = 0;
	  ierror_CID_unc[2][i] = 0;
	  ierror_CID_unc[3][i] = 0;
	  
	  DAC[i] = strtol(DACread.c_str(),NULL, 16);
	  charge[i] = LookupDAQ(DAC[i],whichRange[scan]);
	  
	  for(int k=0; k< 256; k++) {
	    mantCount[k] = 0;
	    mantCount_CID[0][k] = 0;
	    mantCount_CID[1][k] = 0;
	    mantCount_CID[2][k] = 0;
	    mantCount_CID[3][k] = 0;
	    
	  }
	  
	  for(int j=0; j<numTests; j++) {
	    readline(file,linenumber,data,previousline);
	    
	    //cout << data << " ";
	    
	    temp = 0;
	    
	    //m.str(data);
	    //cout << m.str() << endl;
	    //m >> std::hex >> temp;
	    //m >> hex >> temp;
	    //m.str(std::string());
	    
	    //istringstream(data) >> hex >> temp;
	    temp = strtol(data.c_str(),NULL,16);
	    
	    //cout << temp << endl;
	    
	    mantissa = ((temp & 0x00C0) >> 6)*64+(temp & 0x003F);
	    CapID[j]    = (temp & 0xC000) >> 14;
	    
	    //MantValue.Fill(mantissa);
	    
	    mantVec[j] = mantissa;
	    mantAvg[i] += mantissa;	
	    
	    mantCount[mantissa]++;
	    mantCount_CID[CapID[j]][mantissa]++;
	    
	    
	    //cout << mantissa << " " << mantAvg[i] << endl;
	    
	  }
	  
	  readline_cap(file,linenumber,data,previousline);
	  readline_cap(file,linenumber,data,previousline);
	  readline_cap(file,linenumber,data,previousline);
	  readline_cap(file,linenumber,data,previousline);
	  
	  //cout << mantAvg[i] << endl;
	  
	  mantAvg[i] /= numTests;
	  
	  
	  int max = 0;
	  int whichMant = 0;
	  
	  int max_CID[4] = {0,0,0,0};
	  int whichMant_CID[4] = {0,0,0,0};
	  
	  for(int k=0; k<256; k++) {
	    if(mantCount[k] > max) {
	      //max = k;
	      max = mantCount[k];
	      //mantAvg[i] = k;
	      whichMant = k;
	    }
	    
	    for(int q=0; q<4; q++) {
	      if(mantCount_CID[q][k] > max_CID[q]) {
		//max_CID[q] = k;
		max_CID[q] = mantCount_CID[q][k];
		whichMant_CID[q] = k;
	      }
	      
	    }
	  }
	  
	  int denom = 0;
	  int denom_CID[4] = {0,0,0,0};
	  
	  mantAvg[i] = 0;
	  mantAvg_CID[0][i] = 0;
	  mantAvg_CID[1][i] = 0;
	  mantAvg_CID[2][i] = 0;
	  mantAvg_CID[3][i] = 0;
	  
	  for(int k=0; k<256; k++) {
	    if(abs(k-whichMant) < 7) {
	      mantAvg[i] += k*mantCount[k];
	      denom += mantCount[k];
	    }
	    
	    for(int q = 0; q<4; q++) {
	      //if(abs(k-whichMant_CID[q]) < 7) {
	      if(abs(k-whichMant) < 7) {
		mantAvg_CID[q][i] += k*mantCount_CID[q][k];
		denom_CID[q] += mantCount_CID[q][k];
	      }
	    }
	    
	  }
	  
	  mantAvg[i]/=denom;
	  mantAvg_CID[0][i]/= denom_CID[0];
	  mantAvg_CID[1][i]/= denom_CID[1];
	  mantAvg_CID[2][i]/= denom_CID[2];
	  mantAvg_CID[3][i]/= denom_CID[3];
	  
	  
	  //MantValue.Fit("MantGaus","Q");
	  //mantAvg[i] = MantGaus.GetParameter(1);
	  
	  for(int j=0; j<numTests; j++) {
	    
	    if(abs(mantVec[j]-mantAvg[i]) > 6) {
	      ierror[i]++;
	      //if(abs(mantVec[j]-mantAvg[i]) > 6) ierror_CID[CapID[j]][i]++;
	      ierror_CID[CapID[j]][i]++;
	    }
	    
	  }
	  
	  //OutHtml << DACread << " " << mantAvg << " " << ierror << endl;
	  //OutHtml << setfill(' ')
	  //<< setw(6) << DACread << " " << fixed << setw(6) << setfill(' ') << setprecision(0) << DAC[i] << " " 
	  //      << setw(8) << setfill(' ') << fixed << setprecision(2) << charge[i] << " " 
	  //      << setw(8) <<whichMant << " " 
	  //      << setw(8) << setfill(' ') << fixed << setprecision(2) << mantAvg[i] << " " 
	  //      << setw(8) << fixed << setprecision(4) << ierror[i]/numTests << "  "
	  //      << setw(8) << setfill(' ') << fixed << setprecision(2) << mantAvg_CID[0][i] << " " 
	  //      << setw(8) << fixed << setprecision(4) << ierror_CID[0][i]/(numTests/4) << "  "
	  //      << setw(8) << setfill(' ') << fixed << setprecision(2) << mantAvg_CID[1][i] << " " 
	  //      << setw(8) << fixed << setprecision(4) << ierror_CID[1][i]/(numTests/4) << "  "
	  //      << setw(8) << setfill(' ') << fixed << setprecision(2) << mantAvg_CID[2][i] << " " 
	  //      << setw(8) << fixed << setprecision(4) << ierror_CID[2][i]/(numTests/4) << "  "
	  //      << setw(8) << setfill(' ') << fixed << setprecision(2) << mantAvg_CID[3][i] << " " 
	  //      << setw(8) << fixed << setprecision(4) << ierror_CID[3][i]/(numTests/4) 
	  //      << endl;
	  
	  //OutHtml << setw(6) << DACread << " " << fixed << setw(5) << setfill(' ') << setprecision(0) << DAC[i] << " " 
	  //	    << setw(8) << setfill(' ') << fixed << setprecision(2) << charge[i] << " " 
	  //	    << setw(3) <<whichMant << " " << setw(6) << setfill(' ') << fixed << setprecision(2) << mantAvg[i] << " " 
	  //	    << setw(6) << fixed << setprecision(4) << ierror[i]/numTests << "  "
	  //	    << setw(6) << fixed << setprecision(4) << ierror_CID[0][i]/numTests << "  "
	  //	    << setw(6) << fixed << setprecision(4) << ierror_CID[1][i]/numTests << "  "
	  //	    << setw(6) << fixed << setprecision(4) << ierror_CID[2][i]/numTests << "  "
	  //	    << setw(6) << fixed << setprecision(4) << ierror_CID[3][i]/numTests 
	  //	    << endl;
	  
	  
	  if(printToScreen) {
	    //cout << setw(4) << DACread << " " << fixed << setw(5) << setfill(' ') << setprecision(0) << DAC[i] << " " << setw(8) << setfill(' ') << fixed << setprecision(2) << charge[i] << " " 
	    //   << setw(3) <<whichMant << " " << setw(6) << setfill(' ') << fixed << setprecision(2) << mantAvg[i] << " " 
	    //   << setw(6) << fixed << setprecision(4) << ierror[i]/numTests << "  "
	    //   << setw(6) << fixed << setprecision(4) << ierror_CID[0][i]/(numTests/4) << "  "
	    //   << setw(6) << fixed << setprecision(4) << ierror_CID[1][i]/(numTests/4) << "  "
	    //   << setw(6) << fixed << setprecision(4) << ierror_CID[2][i]/(numTests/4) << "  "
	    //   << setw(6) << fixed << setprecision(4) << ierror_CID[3][i]/(numTests/4) 	
	    //   << endl;
	    
	    //cout<< setfill(' ')
	    //	<< setw(6) << DACread << " " << fixed << setw(6) << setfill(' ') << setprecision(0) << DAC[i] << " " 
	    //	<< setw(8) << setfill(' ') << fixed << setprecision(2) << charge[i] << " " 
	    //	<< setw(8) <<whichMant << " " 
	    //	<< setw(8) << setfill(' ') << fixed << setprecision(2) << mantAvg[i] << " " 
	    //	<< setw(8) << fixed << setprecision(4) << ierror[i]/numTests << "  "
	    //	<< setw(8) << setfill(' ') << fixed << setprecision(2) << mantAvg_CID[0][i] << " " 
	    //	<< setw(8) << fixed << setprecision(4) << ierror_CID[0][i]/(numTests/4) << "  "
	    //	<< setw(8) << setfill(' ') << fixed << setprecision(2) << mantAvg_CID[1][i] << " " 
	    //	<< setw(8) << fixed << setprecision(4) << ierror_CID[1][i]/(numTests/4) << "  "
	    //	<< setw(8) << setfill(' ') << fixed << setprecision(2) << mantAvg_CID[2][i] << " " 
	    //	<< setw(8) << fixed << setprecision(4) << ierror_CID[2][i]/(numTests/4) << "  "
	    //	<< setw(8) << setfill(' ') << fixed << setprecision(2) << mantAvg_CID[3][i] << " " 
	    //	<< setw(8) << fixed << setprecision(4) << ierror_CID[3][i]/(numTests/4) 
	    //	<< endl;
	  }
	  
	  ierror[i]/=numTests;
	  ierror_CID[0][i] /= numTests/4;
	  ierror_CID[1][i] /= numTests/4;
	  ierror_CID[2][i] /= numTests/4;
	  ierror_CID[3][i] /= numTests/4;
	  
	  
	  ierror_unc[i] = sqrt(ierror[i]*(1-ierror[i])/numTests);
	  ierror_CID_unc[0][i] = sqrt(ierror_CID[0][i]*(1-ierror_CID[0][i])/(numTests/4));
	  ierror_CID_unc[1][i] = sqrt(ierror_CID[1][i]*(1-ierror_CID[1][i])/(numTests/4));
	  ierror_CID_unc[2][i] = sqrt(ierror_CID[2][i]*(1-ierror_CID[2][i])/(numTests/4));
	  ierror_CID_unc[3][i] = sqrt(ierror_CID[3][i]*(1-ierror_CID[3][i])/(numTests/4));
	  
	  if(ierror[i] > maxErr) maxErr = ierror[i];
	  if(scan == 0) if(ierror[i] > maxErr_R01) maxErr_R01 = ierror[i];
	  if(scan == 1) if(ierror[i] > maxErr_R12) maxErr_R12 = ierror[i];
	  if(scan == 2) if(ierror[i] > maxErr_R23) maxErr_R23 = ierror[i];
	  
  
	}
	
	//TGraph errorRate(numPoints,mantAvg, ierror);
	TGraphErrors errorRate_mant(numPoints,mantAvg, ierror, NULL, ierror_unc);
	TGraphErrors errorRate_mant_CID0(numPoints,mantAvg_CID[0], ierror_CID[0], NULL, ierror_CID_unc[0]);
	TGraphErrors errorRate_mant_CID1(numPoints,mantAvg_CID[1], ierror_CID[1], NULL, ierror_CID_unc[1]);
	TGraphErrors errorRate_mant_CID2(numPoints,mantAvg_CID[2], ierror_CID[2], NULL, ierror_CID_unc[2]);
	TGraphErrors errorRate_mant_CID3(numPoints,mantAvg_CID[3], ierror_CID[3], NULL, ierror_CID_unc[3]);
	
	
	
	TGraphErrors errorRate_DAC(numPoints,DAC, ierror, NULL, ierror_unc);
	
	
	TGraphErrors errorRate_charge(numPoints,charge, ierror, NULL, ierror_unc);
	TGraphErrors errorRate_charge_CID0(numPoints,charge, ierror_CID[0], NULL, ierror_CID_unc[0]);
	TGraphErrors errorRate_charge_CID1(numPoints,charge, ierror_CID[1], NULL, ierror_CID_unc[1]);
	TGraphErrors errorRate_charge_CID2(numPoints,charge, ierror_CID[2], NULL, ierror_CID_unc[2]);
	TGraphErrors errorRate_charge_CID3(numPoints,charge, ierror_CID[3], NULL, ierror_CID_unc[3]);
	
	
	//TCanvas c5;
	
	errorRate_mant.SetMarkerStyle(26);
	errorRate_mant_CID0.SetMarkerStyle(26);
	errorRate_mant_CID1.SetMarkerStyle(26);
	errorRate_mant_CID2.SetMarkerStyle(26);
	errorRate_mant_CID3.SetMarkerStyle(26);
	
	errorRate_DAC.SetMarkerStyle(26);
	
	errorRate_charge.SetMarkerStyle(26);
	errorRate_charge_CID0.SetMarkerStyle(26);
	errorRate_charge_CID1.SetMarkerStyle(26);
	errorRate_charge_CID2.SetMarkerStyle(26);
	errorRate_charge_CID3.SetMarkerStyle(26);
	
	errorRate_mant.SetMarkerColor(kRed);
	errorRate_mant_CID0.SetMarkerColor(kRed);
	errorRate_mant_CID1.SetMarkerColor(kRed);
	errorRate_mant_CID2.SetMarkerColor(kRed);
	errorRate_mant_CID3.SetMarkerColor(kRed);
	
	errorRate_DAC.SetMarkerColor(kRed);
	
	errorRate_charge.SetMarkerColor(kRed);
	errorRate_charge_CID0.SetMarkerColor(kRed);
	errorRate_charge_CID1.SetMarkerColor(kRed);
	errorRate_charge_CID2.SetMarkerColor(kRed);
	errorRate_charge_CID3.SetMarkerColor(kRed);
	
	errorRate_mant.SetTitle("Range Error Rate; SuperMantissa; Rate");
	errorRate_mant_CID0.SetTitle("Range Error Rate, CapID 0; SuperMantissa; Rate");
	errorRate_mant_CID1.SetTitle("Range Error Rate, CapID 1; SuperMantissa; Rate");
	errorRate_mant_CID2.SetTitle("Range Error Rate, CapID 2; SuperMantissa; Rate");
	errorRate_mant_CID3.SetTitle("Range Error Rate, CapID 3; SuperMantissa; Rate");
	
	errorRate_DAC.SetTitle("Range Error Rate; DAC; Rate");
	
	errorRate_charge.SetTitle("Range Error Rate; Charge (fC); Rate");
	errorRate_charge_CID0.SetTitle("Range Error Rate, CapID 0; Charge (fC); Rate");
	errorRate_charge_CID1.SetTitle("Range Error Rate, CapID 1; Charge (fC); Rate");
	errorRate_charge_CID2.SetTitle("Range Error Rate, CapID 2; Charge (fC); Rate");
	errorRate_charge_CID3.SetTitle("Range Error Rate, CapID 3; Charge (fC); Rate");

	if(scan == 0) {
	  //errorRate_charge.Fit("GausR01","Q");
	  errorRate_charge_CID0.Fit("GausR01_CID0","Q");
	  errorRate_charge_CID1.Fit("GausR01_CID1","Q");
	  errorRate_charge_CID2.Fit("GausR01_CID2","Q");
	  errorRate_charge_CID3.Fit("GausR01_CID3","Q");
	}
	if(scan == 1) {
	  //errorRate_charge.Fit("GausR12","Q");
	  errorRate_charge_CID0.Fit("GausR12_CID0","Q");
	  errorRate_charge_CID1.Fit("GausR12_CID1","Q");
	  errorRate_charge_CID2.Fit("GausR12_CID2","Q");
	  errorRate_charge_CID3.Fit("GausR12_CID3","Q");
	}
	if(scan == 2) {
	  //errorRate_charge.Fit("GausR23","Q");
	  errorRate_charge_CID0.Fit("GausR23_CID0","Q");
	  errorRate_charge_CID1.Fit("GausR23_CID1","Q");
	  errorRate_charge_CID2.Fit("GausR23_CID2","Q");
	  errorRate_charge_CID3.Fit("GausR23_CID3","Q");
	}

	//cout << "still alive" << endl;
	
	//outRoot.cd();
	//
	//sprintf(cTemp,"ErrorRate_mant_%s.txt",RangeOut[scan].c_str());
	//errorRate_mant.Write(cTemp);
	//sprintf(cTemp,"ErrorRate_mant_CID0_%s.txt",RangeOut[scan].c_str());
	//errorRate_mant_CID0.Write(cTemp);
	//sprintf(cTemp,"ErrorRate_mant_CID1_%s.txt",RangeOut[scan].c_str());
	//errorRate_mant_CID1.Write(cTemp);
	//sprintf(cTemp,"ErrorRate_mant_CID2_%s.txt",RangeOut[scan].c_str());
	//errorRate_mant_CID2.Write(cTemp);
	//sprintf(cTemp,"ErrorRate_mant_CID3_%s.txt",RangeOut[scan].c_str());
	//errorRate_mant_CID3.Write(cTemp);
	//
	//sprintf(cTemp,"ErrorRate_charge_%s.txt",RangeOut[scan].c_str());
	//errorRate_charge.Write(cTemp);
	//sprintf(cTemp,"ErrorRate_charge_CID0_%s.txt",RangeOut[scan].c_str());
	//errorRate_charge_CID0.Write(cTemp);
	//sprintf(cTemp,"ErrorRate_charge_CID1_%s.txt",RangeOut[scan].c_str());
	//errorRate_charge_CID1.Write(cTemp);
	//sprintf(cTemp,"ErrorRate_charge_CID2_%s.txt",RangeOut[scan].c_str());
	//errorRate_charge_CID2.Write(cTemp);
	//sprintf(cTemp,"ErrorRate_charge_CID3_%s.txt",RangeOut[scan].c_str());
	//errorRate_charge_CID3.Write(cTemp);
	//
	//sprintf(cTemp,"ErrorRate_DAC_%s.txt",RangeOut[scan].c_str());
	//errorRate_DAC.Write(cTemp);
	
	//errorRate_mant.Write("ErrorRate_mant");
	//errorRate_mant_CID0.Write("ErrorRate_mant_CID0");
	//errorRate_mant_CID1.Write("ErrorRate_mant_CID1");
	//errorRate_mant_CID2.Write("ErrorRate_mant_CID2");
	//errorRate_mant_CID3.Write("ErrorRate_mant_CID3");
	//errorRate_charge.Write("ErrorRate_charge");
	//errorRate_charge_CID0.Write("ErrorRate_charge_CID0");
	//errorRate_charge_CID1.Write("ErrorRate_charge_CID1");
	//errorRate_charge_CID2.Write("ErrorRate_charge_CID2");
	//errorRate_charge_CID3.Write("ErrorRate_charge_CID3");
	//errorRate_DAC.Write("ErrorRate_DAC");
	
	//cout << "finished with scan: " << scan << ", moving on" << endl;
	
	//cout << endl;
	//cout << endl;
	
	//OutHtml << endl;
	//OutHtml << endl;
	
      }

      maxError_tot.Fill(maxErr);
      maxError_R01.Fill(maxErr_R01);
      maxError_R12.Fill(maxErr_R12);
      maxError_R23.Fill(maxErr_R23);
      
      //if(maxErr_R01 > 0.01) whichErr1 &= 0x01;
      //if(maxErr_R12 > 0.01) whichErr1 &= 0x02;
      //if(maxErr_R23 > 0.01) whichErr1 &= 0x04;
      //
      //if(maxErr_R01 > 0.05) whichErr5 &= 0x01;
      //if(maxErr_R12 > 0.05) whichErr5 &= 0x02;
      //if(maxErr_R23 > 0.05) whichErr5 &= 0x04;
      //
      //if(maxErr_R01 > 0.1) whichErr10 &= 0x01;
      //if(maxErr_R12 > 0.1) whichErr10 &= 0x02;
      //if(maxErr_R23 > 0.1) whichErr10 &= 0x04;

      if(maxErr_R01 > 0.01) whichErr1 += 1;
      if(maxErr_R12 > 0.01) whichErr1 += 2;
      if(maxErr_R23 > 0.01) whichErr1 += 4;

      if(maxErr_R01 > 0.05) whichErr5 += 1;
      if(maxErr_R12 > 0.05) whichErr5 += 2;
      if(maxErr_R23 > 0.05) whichErr5 += 4;

      if(maxErr_R01 > 0.1) whichErr10 += 1;
      if(maxErr_R12 > 0.1) whichErr10 += 2;
      if(maxErr_R23 > 0.1) whichErr10 += 4;

      if(whichErr1 == 0) whichError_1.Fill(1);
      else if(whichErr1 == 1) whichError_1.Fill(2);
      else if(whichErr1 == 2) whichError_1.Fill(3);
      else if(whichErr1 == 4) whichError_1.Fill(4);
      else if(whichErr1 == 3) whichError_1.Fill(5);
      else if(whichErr1 == 5) whichError_1.Fill(6);
      else if(whichErr1 == 6) whichError_1.Fill(7);
      else if(whichErr1 == 7) whichError_1.Fill(8);

      if(whichErr5 == 0) whichError_5.Fill(1);
      else if(whichErr5 == 1) whichError_5.Fill(2);
      else if(whichErr5 == 2) whichError_5.Fill(3);
      else if(whichErr5 == 4) whichError_5.Fill(4);
      else if(whichErr5 == 3) whichError_5.Fill(5);
      else if(whichErr5 == 5) whichError_5.Fill(6);
      else if(whichErr5 == 6) whichError_5.Fill(7);
      else if(whichErr5 == 7) whichError_5.Fill(8);
      
      if(whichErr10 == 0) whichError_10.Fill(1);
      else if(whichErr10 == 1) whichError_10.Fill(2);
      else if(whichErr10 == 2) whichError_10.Fill(3);
      else if(whichErr10 == 4) whichError_10.Fill(4);
      else if(whichErr10 == 3) whichError_10.Fill(5);
      else if(whichErr10 == 5) whichError_10.Fill(6);
      else if(whichErr10 == 6) whichError_10.Fill(7);
      else if(whichErr10 == 7) whichError_10.Fill(8);

      gAmpError_R01_CID0.Fill(GausR01_CID0.GetParameter(0));
      gAmpError_R12_CID0.Fill(GausR12_CID0.GetParameter(0));
      gAmpError_R23_CID0.Fill(GausR23_CID0.GetParameter(0));
      
      gMeanError_R01_CID0.Fill(GausR01_CID0.GetParameter(1));
      gMeanError_R12_CID0.Fill(GausR12_CID0.GetParameter(1));
      gMeanError_R23_CID0.Fill(GausR23_CID0.GetParameter(1));

      gWidError_R01_CID0.Fill(GausR01_CID0.GetParameter(2));
      gWidError_R12_CID0.Fill(GausR12_CID0.GetParameter(2));
      gWidError_R23_CID0.Fill(GausR23_CID0.GetParameter(2));
      
      gAmpError_R01_CID1.Fill(GausR01_CID1.GetParameter(0));
      gAmpError_R12_CID1.Fill(GausR12_CID1.GetParameter(0));
      gAmpError_R23_CID1.Fill(GausR23_CID1.GetParameter(0));
      
      gMeanError_R01_CID1.Fill(GausR01_CID1.GetParameter(1));
      gMeanError_R12_CID1.Fill(GausR12_CID1.GetParameter(1));
      gMeanError_R23_CID1.Fill(GausR23_CID1.GetParameter(1));

      gWidError_R01_CID1.Fill(GausR01_CID1.GetParameter(2));
      gWidError_R12_CID1.Fill(GausR12_CID1.GetParameter(2));
      gWidError_R23_CID1.Fill(GausR23_CID1.GetParameter(2));
      

      gAmpError_R01_CID2.Fill(GausR01_CID2.GetParameter(0));
      gAmpError_R12_CID2.Fill(GausR12_CID2.GetParameter(0));
      gAmpError_R23_CID2.Fill(GausR23_CID2.GetParameter(0));
      
      gMeanError_R01_CID2.Fill(GausR01_CID2.GetParameter(1));
      gMeanError_R12_CID2.Fill(GausR12_CID2.GetParameter(1));
      gMeanError_R23_CID2.Fill(GausR23_CID2.GetParameter(1));

      gWidError_R01_CID2.Fill(GausR01_CID2.GetParameter(2));
      gWidError_R12_CID2.Fill(GausR12_CID2.GetParameter(2));
      gWidError_R23_CID2.Fill(GausR23_CID2.GetParameter(2));
      

      gAmpError_R01_CID3.Fill(GausR01_CID3.GetParameter(0));
      gAmpError_R12_CID3.Fill(GausR12_CID3.GetParameter(0));
      gAmpError_R23_CID3.Fill(GausR23_CID3.GetParameter(0));
      
      gMeanError_R01_CID3.Fill(GausR01_CID3.GetParameter(1));
      gMeanError_R12_CID3.Fill(GausR12_CID3.GetParameter(1));
      gMeanError_R23_CID3.Fill(GausR23_CID3.GetParameter(1));

      gWidError_R01_CID3.Fill(GausR01_CID3.GetParameter(2));
      gWidError_R12_CID3.Fill(GausR12_CID3.GetParameter(2));
      gWidError_R23_CID3.Fill(GausR23_CID3.GetParameter(2));
      
      gAmpError_R01.Fill(TMath::Max(TMath::Max(TMath::Max(GausR01_CID0.GetParameter(0),GausR01_CID1.GetParameter(0)),GausR01_CID2.GetParameter(0)),GausR01_CID3.GetParameter(0)));
      gAmpError_R12.Fill(TMath::Max(TMath::Max(TMath::Max(GausR12_CID0.GetParameter(0),GausR12_CID1.GetParameter(0)),GausR12_CID2.GetParameter(0)),GausR12_CID3.GetParameter(0)));
      gAmpError_R23.Fill(TMath::Max(TMath::Max(TMath::Max(GausR23_CID0.GetParameter(0),GausR23_CID1.GetParameter(0)),GausR23_CID2.GetParameter(0)),GausR23_CID3.GetParameter(0)));

      ErrorRateVsGaus.Fill(maxErr_R23*4, TMath::Max(TMath::Max(TMath::Max(GausR23_CID0.GetParameter(0),GausR23_CID1.GetParameter(0)),GausR23_CID2.GetParameter(0)),GausR23_CID3.GetParameter(0)));

      //GausR01_CID0.Integral(0,350000) + GausR01_CID1.Integral(0,350000) + GausR01_CID2.Integral(0,350000) + GausR01_CID3.Integral(0,350000);

      FullChipError.Fill(((GausR01_CID0.Integral(500,650) + GausR01_CID1.Integral(500,650) + GausR01_CID2.Integral(500,650) + GausR01_CID3.Integral(500,650))/4 +
			 (GausR12_CID0.Integral(5000,5500) + GausR12_CID1.Integral(5000,5500) + GausR12_CID2.Integral(5000,5500) + GausR12_CID3.Integral(5000,5500))/4 +
			  (GausR23_CID0.Integral(35000,45000) + GausR23_CID1.Integral(35000,45000) + GausR23_CID2.Integral(35000,45000) + GausR23_CID3.Integral(35000,45000))/4)/350000);




      file.close();

      cout << "Finished chip: " << counter << endl;
      
    }
    
  }
  
  maxError_tot.Write();
  maxError_R01.Write();
  maxError_R12.Write();
  maxError_R23.Write();

  gAmpError_R01.Write();
  gAmpError_R12.Write();
  gAmpError_R23.Write();

  gAmpError_R01_CID0.Write();
  gAmpError_R12_CID0.Write();
  gAmpError_R23_CID0.Write();

  gMeanError_R01_CID0.Write();
  gMeanError_R12_CID0.Write();
  gMeanError_R23_CID0.Write();

  gWidError_R01_CID0.Write();
  gWidError_R12_CID0.Write();
  gWidError_R23_CID0.Write();
  
  gAmpError_R01_CID1.Write();
  gAmpError_R12_CID1.Write();
  gAmpError_R23_CID1.Write();

  gMeanError_R01_CID1.Write();
  gMeanError_R12_CID1.Write();
  gMeanError_R23_CID1.Write();

  gWidError_R01_CID1.Write();
  gWidError_R12_CID1.Write();
  gWidError_R23_CID1.Write();
  
  gAmpError_R01_CID2.Write();
  gAmpError_R12_CID2.Write();
  gAmpError_R23_CID2.Write();

  gMeanError_R01_CID2.Write();
  gMeanError_R12_CID2.Write();
  gMeanError_R23_CID2.Write();

  gWidError_R01_CID2.Write();
  gWidError_R12_CID2.Write();
  gWidError_R23_CID2.Write();
  
  gAmpError_R01_CID3.Write();
  gAmpError_R12_CID3.Write();
  gAmpError_R23_CID3.Write();

  gMeanError_R01_CID3.Write();
  gMeanError_R12_CID3.Write();
  gMeanError_R23_CID3.Write();

  gWidError_R01_CID3.Write();
  gWidError_R12_CID3.Write();
  gWidError_R23_CID3.Write();

  ErrorRateVsGaus.Write();

  FullChipError.Write();

  whichError_1.Write();
  whichError_5.Write();
  whichError_10.Write();
  
}

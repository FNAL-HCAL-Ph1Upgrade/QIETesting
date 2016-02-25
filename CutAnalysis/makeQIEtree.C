#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <cstring>
#include <sstream>

#include "TFile.h"
#include "TTree.h"

  map<string, int> vars;

  TTree* tree;

  //DCH delcare the event objects
  int ChipStatus;
  int ChipID;

  //Cuts 1-4
  double Current33_NoC;
  double Current50_NoC;
  double Current33_C;
  double Current50_C;

  //Cuts 5-13
  double Volt_Isetp;
  double Volt_33V;
  double Volt_50V;
  double Volt_Clamp;
  double Volt_ClampCalMode;
  double Volt_Rref;
  double Volt_IDCSet;
  double Volt_InSig;
  double Volt_InRef;

  //Cuts 18-49
  double IDCset00;
  double IDCset01;
  double IDCset02;
  double IDCset03;
  double IDCset04;
  double IDCset05;
  double IDCset06;
  double IDCset07;
  double IDCset08;
  double IDCset09;
  double IDCset0a;
  double IDCset0b;
  double IDCset0c;
  double IDCset0d;
  double IDCset0e;
  double IDCset0f;
  double IDCset10;
  double IDCset11;
  double IDCset12;
  double IDCset13;
  double IDCset14;
  double IDCset15;
  double IDCset16;
  double IDCset17;
  double IDCset18;
  double IDCset19;
  double IDCset1a;
  double IDCset1b;
  double IDCset1c;
  double IDCset1d;
  double IDCset1e;
  double IDCset1f;

  //Cut 50 (why 6?)
  double LVDS_AvgMid_1;
  double LVDS_AvgMid_2;
  double LVDS_AvgMid_3;
  double LVDS_AvgMid_4;

  //Cuts 51-54
  double SumRange0;
  double SumRange1;
  double SumRange2;
  double SumRange3;

  //Cuts 55-62
  double SumDAC0;
  double SumDAC1;
  double SumDAC2;
  double SumDAC3;
  double SumDAC4;
  double SumDAC5;
  double SumDAC6;
  double SumDAC7;

  //Cuts 63-71
  double ThreshDAC0;
  double ThreshDAC1;
  double ThreshDAC2;
  double ThreshDAC4;
  double ThreshDAC8;
  double ThreshDAC10;
  double ThreshDAC20;
  double ThreshDAC40;
  double ThreshDAC80;

  //Cuts 72-73
  double LockTime120Mhz;
  double LockTime1Mhz;

  //Cut 74
  double InSigV;

  //Cut75 + 76
  double InRefV;
  double DiffInSigInRef;

  //Cuts 77-84
  double TimingInRefDAC0;
  double TimingInRefDAC1;
  double TimingInRefDAC2;
  double TimingInRefDAC3;
  double TimingInRefDAC4;
  double TimingInRefDAC5;
  double TimingInRefDAC6;
  double TimingInRefDAC7;

  //Cut 86-93
  double LVDSTrim0Hi_pin01;
  double LVDSTrim0Hi_pin02;
  double LVDSTrim0Hi_pin03;
  double LVDSTrim0Hi_pin04;
  double LVDSTrim0Hi_pin05;
  double LVDSTrim0Hi_pin06;
  double LVDSTrim0Hi_pin07;
  double LVDSTrim0Hi_pin08;
  double LVDSTrim0Hi_pin09;
  double LVDSTrim0Hi_pin10;
  double LVDSTrim0Hi_pin11;
  double LVDSTrim0Hi_pin12;
  double LVDSTrim0Hi_pin13;
  double LVDSTrim0Hi_pin14;
  double LVDSTrim0Hi_pin15;
  double LVDSTrim0Hi_pin16;
  double LVDSTrim0Hi_pin17;
  double LVDSTrim0Hi_pin18;
  double LVDSTrim0Lo_pin01;
  double LVDSTrim0Lo_pin02;
  double LVDSTrim0Lo_pin03;
  double LVDSTrim0Lo_pin04;
  double LVDSTrim0Lo_pin05;
  double LVDSTrim0Lo_pin06;
  double LVDSTrim0Lo_pin07;
  double LVDSTrim0Lo_pin08;
  double LVDSTrim0Lo_pin09;
  double LVDSTrim0Lo_pin10;
  double LVDSTrim0Lo_pin11;
  double LVDSTrim0Lo_pin12;
  double LVDSTrim0Lo_pin13;
  double LVDSTrim0Lo_pin14;
  double LVDSTrim0Lo_pin15;
  double LVDSTrim0Lo_pin16;
  double LVDSTrim0Lo_pin17;
  double LVDSTrim0Lo_pin18;

  double LVDSTrim1Hi_pin01;
  double LVDSTrim1Hi_pin02;
  double LVDSTrim1Hi_pin03;
  double LVDSTrim1Hi_pin04;
  double LVDSTrim1Hi_pin05;
  double LVDSTrim1Hi_pin06;
  double LVDSTrim1Hi_pin07;
  double LVDSTrim1Hi_pin08;
  double LVDSTrim1Hi_pin09;
  double LVDSTrim1Hi_pin10;
  double LVDSTrim1Hi_pin11;
  double LVDSTrim1Hi_pin12;
  double LVDSTrim1Hi_pin13;
  double LVDSTrim1Hi_pin14;
  double LVDSTrim1Hi_pin15;
  double LVDSTrim1Hi_pin16;
  double LVDSTrim1Hi_pin17;
  double LVDSTrim1Hi_pin18;
  double LVDSTrim1Lo_pin01;
  double LVDSTrim1Lo_pin02;
  double LVDSTrim1Lo_pin03;
  double LVDSTrim1Lo_pin04;
  double LVDSTrim1Lo_pin05;
  double LVDSTrim1Lo_pin06;
  double LVDSTrim1Lo_pin07;
  double LVDSTrim1Lo_pin08;
  double LVDSTrim1Lo_pin09;
  double LVDSTrim1Lo_pin10;
  double LVDSTrim1Lo_pin11;
  double LVDSTrim1Lo_pin12;
  double LVDSTrim1Lo_pin13;
  double LVDSTrim1Lo_pin14;
  double LVDSTrim1Lo_pin15;
  double LVDSTrim1Lo_pin16;
  double LVDSTrim1Lo_pin17;
  double LVDSTrim1Lo_pin18;

  double LVDSTrim2Hi_pin01;
  double LVDSTrim2Hi_pin02;
  double LVDSTrim2Hi_pin03;
  double LVDSTrim2Hi_pin04;
  double LVDSTrim2Hi_pin05;
  double LVDSTrim2Hi_pin06;
  double LVDSTrim2Hi_pin07;
  double LVDSTrim2Hi_pin08;
  double LVDSTrim2Hi_pin09;
  double LVDSTrim2Hi_pin10;
  double LVDSTrim2Hi_pin11;
  double LVDSTrim2Hi_pin12;
  double LVDSTrim2Hi_pin13;
  double LVDSTrim2Hi_pin14;
  double LVDSTrim2Hi_pin15;
  double LVDSTrim2Hi_pin16;
  double LVDSTrim2Hi_pin17;
  double LVDSTrim2Hi_pin18;
  double LVDSTrim2Lo_pin01;
  double LVDSTrim2Lo_pin02;
  double LVDSTrim2Lo_pin03;
  double LVDSTrim2Lo_pin04;
  double LVDSTrim2Lo_pin05;
  double LVDSTrim2Lo_pin06;
  double LVDSTrim2Lo_pin07;
  double LVDSTrim2Lo_pin08;
  double LVDSTrim2Lo_pin09;
  double LVDSTrim2Lo_pin10;
  double LVDSTrim2Lo_pin11;
  double LVDSTrim2Lo_pin12;
  double LVDSTrim2Lo_pin13;
  double LVDSTrim2Lo_pin14;
  double LVDSTrim2Lo_pin15;
  double LVDSTrim2Lo_pin16;
  double LVDSTrim2Lo_pin17;
  double LVDSTrim2Lo_pin18;

  double LVDSTrim3Hi_pin01;
  double LVDSTrim3Hi_pin02;
  double LVDSTrim3Hi_pin03;
  double LVDSTrim3Hi_pin04;
  double LVDSTrim3Hi_pin05;
  double LVDSTrim3Hi_pin06;
  double LVDSTrim3Hi_pin07;
  double LVDSTrim3Hi_pin08;
  double LVDSTrim3Hi_pin09;
  double LVDSTrim3Hi_pin10;
  double LVDSTrim3Hi_pin11;
  double LVDSTrim3Hi_pin12;
  double LVDSTrim3Hi_pin13;
  double LVDSTrim3Hi_pin14;
  double LVDSTrim3Hi_pin15;
  double LVDSTrim3Hi_pin16;
  double LVDSTrim3Hi_pin17;
  double LVDSTrim3Hi_pin18;
  double LVDSTrim3Lo_pin01;
  double LVDSTrim3Lo_pin02;
  double LVDSTrim3Lo_pin03;
  double LVDSTrim3Lo_pin04;
  double LVDSTrim3Lo_pin05;
  double LVDSTrim3Lo_pin06;
  double LVDSTrim3Lo_pin07;
  double LVDSTrim3Lo_pin08;
  double LVDSTrim3Lo_pin09;
  double LVDSTrim3Lo_pin10;
  double LVDSTrim3Lo_pin11;
  double LVDSTrim3Lo_pin12;
  double LVDSTrim3Lo_pin13;
  double LVDSTrim3Lo_pin14;
  double LVDSTrim3Lo_pin15;
  double LVDSTrim3Lo_pin16;
  double LVDSTrim3Lo_pin17;
  double LVDSTrim3Lo_pin18;

  //Cuts 94-103
  double ImpDiff_RinSel0;
  double InSigMClampDiff_RinSel0;
  double ImpDiff_RinSel1;
  double InSigMClampDiff_RinSel1;
  double ImpDiff_RinSel2;
  double InSigMClampDiff_RinSel2;
  double ImpDiff_RinSel4;
  double InSigMClampDiff_RinSel4;
  double ImpDiff_RinSel8;
  double InSigMClampDiff_RinSel8;
  double ImpDiff_RinSelF;

  //Cuts 106-109
  double WRPmax;
  double WRPRange01;
  double WRPRange12;
  double WRPRange23;

  //Cuts 110-137
  double lADC00_Dat0;
  double lADC01_Dat3;
  double lADC02_Dat6;
  double lADC03_Disc;
  double lADC10_Dat0Bar;
  double lADC11_Dat3Bar;
  double lADC12_Dat6Bar;
  double lADC13_DiscBar;
  double lADC20_Dat1;
  double lADC21_Dat4;
  double lADC22_Dat7;
  double lADC23_Rref;
  double lADC30_Dat1Bar;
  double lADC31_Dat4Bar;
  double lADC32_Dat7Bar;
  double lADC33_IDCset;
  double lADC40_Dat2;
  double lADC41_Dat5;
  double lADC42_ClkOut;
  double lADC43_InSig;
  double lADC50_Dat2Bar;
  double lADC51_Dat5Bar;
  double lADC52_ClkOutBar;
  double lADC53_InRef;
  double lADC60_IsetP;
  double lADC61_33V;
  double lADC62_50V;
  double lADC63_Clamp;

  double gsel00;
  double gsel01;
  double gsel02;
  double gsel04;
  double gsel08;
  double gsel16;

  //Cuts 150-165 //1perCAPID;
  double Slope_R0S0_CID0;
  double Slope_R0S1_CID0;
  double Slope_R0S2_CID0;
  double Slope_R0S3_CID0;
  double Slope_R1S0_CID0;
  double Slope_R1S1_CID0;
  double Slope_R1S2_CID0;
  double Slope_R1S3_CID0;
  double Slope_R2S0_CID0;
  double Slope_R2S1_CID0;
  double Slope_R2S2_CID0;
  double Slope_R2S3_CID0;
  double Slope_R3S0_CID0;
  double Slope_R3S1_CID0;
  double Slope_R3S2_CID0;
  double Slope_R3S3_CID0;

  double Slope_R0S0_CID1;
  double Slope_R0S1_CID1;
  double Slope_R0S2_CID1;
  double Slope_R0S3_CID1;
  double Slope_R1S0_CID1;
  double Slope_R1S1_CID1;
  double Slope_R1S2_CID1;
  double Slope_R1S3_CID1;
  double Slope_R2S0_CID1;
  double Slope_R2S1_CID1;
  double Slope_R2S2_CID1;
  double Slope_R2S3_CID1;
  double Slope_R3S0_CID1;
  double Slope_R3S1_CID1;
  double Slope_R3S2_CID1;
  double Slope_R3S3_CID1;

  double Slope_R0S0_CID2;
  double Slope_R0S1_CID2;
  double Slope_R0S2_CID2;
  double Slope_R0S3_CID2;
  double Slope_R1S0_CID2;
  double Slope_R1S1_CID2;
  double Slope_R1S2_CID2;
  double Slope_R1S3_CID2;
  double Slope_R2S0_CID2;
  double Slope_R2S1_CID2;
  double Slope_R2S2_CID2;
  double Slope_R2S3_CID2;
  double Slope_R3S0_CID2;
  double Slope_R3S1_CID2;
  double Slope_R3S2_CID2;
  double Slope_R3S3_CID2;

  double Slope_R0S0_CID3;
  double Slope_R0S1_CID3;
  double Slope_R0S2_CID3;
  double Slope_R0S3_CID3;
  double Slope_R1S0_CID3;
  double Slope_R1S1_CID3;
  double Slope_R1S2_CID3;
  double Slope_R1S3_CID3;
  double Slope_R2S0_CID3;
  double Slope_R2S1_CID3;
  double Slope_R2S2_CID3;
  double Slope_R2S3_CID3;
  double Slope_R3S0_CID3;
  double Slope_R3S1_CID3;
  double Slope_R3S2_CID3;
  double Slope_R3S3_CID3;

   //Cuts 166-181; //1 perCAPID
  double Int_R0S0_CID0;
  double Int_R0S1_CID0;
  double Int_R0S2_CID0;
  double Int_R0S3_CID0;
  double Int_R1S0_CID0;
  double Int_R1S1_CID0;
  double Int_R1S2_CID0;
  double Int_R1S3_CID0;
  double Int_R2S0_CID0;
  double Int_R2S1_CID0;
  double Int_R2S2_CID0;
  double Int_R2S3_CID0;
  double Int_R3S0_CID0;
  double Int_R3S1_CID0;
  double Int_R3S2_CID0;
  double Int_R3S3_CID0;

  double Int_R0S0_CID1;
  double Int_R0S1_CID1;
  double Int_R0S2_CID1;
  double Int_R0S3_CID1;
  double Int_R1S0_CID1;
  double Int_R1S1_CID1;
  double Int_R1S2_CID1;
  double Int_R1S3_CID1;
  double Int_R2S0_CID1;
  double Int_R2S1_CID1;
  double Int_R2S2_CID1;
  double Int_R2S3_CID1;
  double Int_R3S0_CID1;
  double Int_R3S1_CID1;
  double Int_R3S2_CID1;
  double Int_R3S3_CID1;

  double Int_R0S0_CID2;
  double Int_R0S1_CID2;
  double Int_R0S2_CID2;
  double Int_R0S3_CID2;
  double Int_R1S0_CID2;
  double Int_R1S1_CID2;
  double Int_R1S2_CID2;
  double Int_R1S3_CID2;
  double Int_R2S0_CID2;
  double Int_R2S1_CID2;
  double Int_R2S2_CID2;
  double Int_R2S3_CID2;
  double Int_R3S0_CID2;
  double Int_R3S1_CID2;
  double Int_R3S2_CID2;
  double Int_R3S3_CID2;

  double Int_R0S0_CID3;
  double Int_R0S1_CID3;
  double Int_R0S2_CID3;
  double Int_R0S3_CID3;
  double Int_R1S0_CID3;
  double Int_R1S1_CID3;
  double Int_R1S2_CID3;
  double Int_R1S3_CID3;
  double Int_R2S0_CID3;
  double Int_R2S1_CID3;
  double Int_R2S2_CID3;
  double Int_R2S3_CID3;
  double Int_R3S0_CID3;
  double Int_R3S1_CID3;
  double Int_R3S2_CID3;
  double Int_R3S3_CID3;

  //Cut 182 - ARRAY - 496 entries!!!
  //now just 62
  double ADCDNL[62];

  //Cuts 183-185
  double OverlapR01_CID0;
  double OverlapR12_CID0;
  double OverlapR23_CID0;
  //Cuts 183-185
  double OverlapR01_CID1;
  double OverlapR12_CID1;
  double OverlapR23_CID1;
    //Cuts 183-185
  double OverlapR01_CID2;
  double OverlapR12_CID2;
  double OverlapR23_CID2;
    //Cuts 183-185
  double OverlapR01_CID3;
  double OverlapR12_CID3;
  double OverlapR23_CID3;

  //Cuts 187-189
  double Pedestal_CID0; 
  double Pedestal_CID1;
  double Pedestal_CID2;
  double Pedestal_CID3;

  double PedestalRMS_CID0;
  double PedestalRMS_CID1;
  double PedestalRMS_CID2;
  double PedestalRMS_CID3;

  double PedestalDACSlope_Lo; //2 of these
  double PedestalDACSlope_Hi;

  double CapIDPedDACSlope_CID0;
  double CapIDPedDACSlope_CID1;
  double CapIDPedDACSlope_CID2;
  double CapIDPedDACSlope_CID3;

  //Cuts 190-191
  double TDCBinWidth;
  double TDCVertOffset[255]; // 25 of these

  //Cuts 192-193
  double Sum4Average;
  double Sum4DNL[255]; //255 of these

  //DCH book the tree


void  BookTree() {

  tree = new TTree("ChipData","Data from tested chips");
  tree->SetAutoSave(10000);

  tree->Branch("ChipID", &ChipID, "ChipID/I");
  tree->Branch("ChipStatus", &ChipStatus, "ChipStatus/I");

  tree->Branch("Current33_NoC",&Current33_NoC,"Current33_NoC/D");
  tree->Branch("Current50_NoC",&Current50_NoC,"Current50_NoC/D");
  tree->Branch("Current33_C",&Current33_C,"Current33_C/D");
  tree->Branch("Current50_C",&Current50_C,"Current50_C/D");
  tree->Branch("Volt_Isetp",&Volt_Isetp,"Volt_Isetp/D");
  tree->Branch("Volt_33V",&Volt_33V,"Volt_33V/D");
  tree->Branch("Volt_50V",&Volt_50V,"Volt_50C/D");
  tree->Branch("Volt_Clamp",&Volt_Clamp,"Volt_Clamp/D");
  tree->Branch("Volt_ClampCalMode",&Volt_ClampCalMode,"Volt_ClampCalMode/D");
  tree->Branch("Volt_Rref",&Volt_Rref,"Volt_Rref/D");
  tree->Branch("Volt_IDCSet",&Volt_IDCSet,"Volt_IDCSet/D");
  tree->Branch("Volt_InSig",&Volt_InSig,"Volt_InSig/D");
  tree->Branch("Volt_InRef",&Volt_InRef,"Volt_InRef/D");
  tree->Branch("IDCset00",&IDCset00,"IDCset00/D");
  tree->Branch("IDCset01",&IDCset01,"IDCset01/D");
  tree->Branch("IDCset02",&IDCset02,"IDCset02/D");
  tree->Branch("IDCset03",&IDCset03,"IDCset03/D");
  tree->Branch("IDCset04",&IDCset04,"IDCset04/D");
  tree->Branch("IDCset05",&IDCset05,"IDCset05/D");
  tree->Branch("IDCset06",&IDCset06,"IDCset06/D");
  tree->Branch("IDCset07",&IDCset07,"IDCset07/D");
  tree->Branch("IDCset08",&IDCset08,"IDCset08/D");
  tree->Branch("IDCset09",&IDCset09,"IDCset09/D");
  tree->Branch("IDCset0a",&IDCset0a,"IDCset0a/D");
  tree->Branch("IDCset0b",&IDCset0b,"IDCset0b/D");
  tree->Branch("IDCset0c",&IDCset0c,"IDCset0c/D");
  tree->Branch("IDCset0d",&IDCset0d,"IDCset0d/D");
  tree->Branch("IDCset0e",&IDCset0e,"IDCset0e/D");
  tree->Branch("IDCset0f",&IDCset0f,"IDCset0f/D");
  tree->Branch("IDCset10",&IDCset10,"IDCset10/D");
  tree->Branch("IDCset11",&IDCset11,"IDCset11/D");
  tree->Branch("IDCset12",&IDCset12,"IDCset12/D");
  tree->Branch("IDCset13",&IDCset13,"IDCset13/D");
  tree->Branch("IDCset14",&IDCset14,"IDCset14/D");
  tree->Branch("IDCset15",&IDCset15,"IDCset15/D");
  tree->Branch("IDCset16",&IDCset16,"IDCset16/D");
  tree->Branch("IDCset17",&IDCset17,"IDCset17/D");
  tree->Branch("IDCset18",&IDCset18,"IDCset18/D");
  tree->Branch("IDCset19",&IDCset19,"IDCset19/D");
  tree->Branch("IDCset1a",&IDCset1a,"IDCset1a/D");
  tree->Branch("IDCset1b",&IDCset1b,"IDCset1b/D");
  tree->Branch("IDCset1c",&IDCset1c,"IDCset1c/D");
  tree->Branch("IDCset1d",&IDCset1d,"IDCset1d/D");
  tree->Branch("IDCset1e",&IDCset1e,"IDCset1e/D");
  tree->Branch("IDCset1f",&IDCset1f,"IDCset1f/D");

  tree->Branch("LVDS_AvgMid_1",&LVDS_AvgMid_1,"LVDS_AvgMid_1/D");
  tree->Branch("LVDS_AvgMid_2",&LVDS_AvgMid_2,"LVDS_AvgMid_2/D");
  tree->Branch("LVDS_AvgMid_3",&LVDS_AvgMid_3,"LVDS_AvgMid_3/D");
  tree->Branch("LVDS_AvgMid_4",&LVDS_AvgMid_4,"LVDS_AvgMid_4/D");
  tree->Branch("SumRange0",&SumRange0,"SumRange0/D");
  tree->Branch("SumRange1",&SumRange1,"SumRange1/D");
  tree->Branch("SumRange2",&SumRange2,"SumRange2/D");
  tree->Branch("SumRange3",&SumRange3,"SumRange3/D");
  tree->Branch("SumDAC0",&SumDAC0,"SumDAC0/D");
  tree->Branch("SumDAC1",&SumDAC1,"SumDAC1/D");
  tree->Branch("SumDAC2",&SumDAC2,"SumDAC2/D");
  tree->Branch("SumDAC3",&SumDAC3,"SumDAC3/D");
  tree->Branch("SumDAC4",&SumDAC4,"SumDAC4/D");
  tree->Branch("SumDAC5",&SumDAC5,"SumDAC5/D");
  tree->Branch("SumDAC6",&SumDAC6,"SumDAC6/D");
  tree->Branch("SumDAC7",&SumDAC7,"SumDAC7/D");
  tree->Branch("ThreshDAC0",&ThreshDAC0,"ThreshDAC0/D");
  tree->Branch("ThreshDAC1",&ThreshDAC1,"ThreshDAC1/D");
  tree->Branch("ThreshDAC2",&ThreshDAC2,"ThreshDAC2/D");
  tree->Branch("ThreshDAC4",&ThreshDAC4,"ThreshDAC4/D");
  tree->Branch("ThreshDAC8",&ThreshDAC8,"ThreshDAC8/D");
  tree->Branch("ThreshDAC10",&ThreshDAC10,"ThreshDAC10/D");
  tree->Branch("ThreshDAC20",&ThreshDAC20,"ThreshDAC20/D");
  tree->Branch("ThreshDAC40",&ThreshDAC40,"ThreshDAC40/D");
  tree->Branch("ThreshDAC80",&ThreshDAC80,"ThreshDAC80/D");
  tree->Branch("LockTime120Mhz",&LockTime120Mhz,"LockTime120Mhz/D");
  tree->Branch("LockTime1Mhz",&LockTime1Mhz,"LockTime1Mhz/D");
  tree->Branch("InSigV",&InSigV,"InSigV/D");
  tree->Branch("InRefV",&InRefV,"InRefV/D");
  tree->Branch("DiffInSigInRef",&DiffInSigInRef,"DiffInSigInRef/D");
  tree->Branch("TimingInRefDAC0",&TimingInRefDAC0,"TimingInRefDAC0/D");
  tree->Branch("TimingInRefDAC1",&TimingInRefDAC1,"TimingInRefDAC1/D");
  tree->Branch("TimingInRefDAC2",&TimingInRefDAC2,"TimingInRefDAC2/D");
  tree->Branch("TimingInRefDAC3",&TimingInRefDAC3,"TimingInRefDAC3/D");
  tree->Branch("TimingInRefDAC4",&TimingInRefDAC4,"TimingInRefDAC4/D");
  tree->Branch("TimingInRefDAC5",&TimingInRefDAC5,"TimingInRefDAC5/D");
  tree->Branch("TimingInRefDAC6",&TimingInRefDAC6,"TimingInRefDAC6/D");
  tree->Branch("TimingInRefDAC7",&TimingInRefDAC7,"TimingInRefDAC7/D");

  tree->Branch("LVDSTrim0Hi_pin01",&LVDSTrim0Hi_pin01,"LVDSTrim0Hi_pin01/D");
  tree->Branch("LVDSTrim0Hi_pin02",&LVDSTrim0Hi_pin02,"LVDSTrim0Hi_pin02/D");
  tree->Branch("LVDSTrim0Hi_pin03",&LVDSTrim0Hi_pin03,"LVDSTrim0Hi_pin03/D");
  tree->Branch("LVDSTrim0Hi_pin04",&LVDSTrim0Hi_pin04,"LVDSTrim0Hi_pin04/D");
  tree->Branch("LVDSTrim0Hi_pin05",&LVDSTrim0Hi_pin05,"LVDSTrim0Hi_pin05/D");
  tree->Branch("LVDSTrim0Hi_pin06",&LVDSTrim0Hi_pin06,"LVDSTrim0Hi_pin06/D");
  tree->Branch("LVDSTrim0Hi_pin07",&LVDSTrim0Hi_pin07,"LVDSTrim0Hi_pin07/D");
  tree->Branch("LVDSTrim0Hi_pin08",&LVDSTrim0Hi_pin08,"LVDSTrim0Hi_pin08/D");
  tree->Branch("LVDSTrim0Hi_pin09",&LVDSTrim0Hi_pin09,"LVDSTrim0Hi_pin09/D");
  tree->Branch("LVDSTrim0Hi_pin10",&LVDSTrim0Hi_pin10,"LVDSTrim0Hi_pin10/D");
  tree->Branch("LVDSTrim0Hi_pin11",&LVDSTrim0Hi_pin11,"LVDSTrim0Hi_pin11/D");
  tree->Branch("LVDSTrim0Hi_pin12",&LVDSTrim0Hi_pin12,"LVDSTrim0Hi_pin12/D");
  tree->Branch("LVDSTrim0Hi_pin13",&LVDSTrim0Hi_pin13,"LVDSTrim0Hi_pin13/D");
  tree->Branch("LVDSTrim0Hi_pin14",&LVDSTrim0Hi_pin14,"LVDSTrim0Hi_pin14/D");
  tree->Branch("LVDSTrim0Hi_pin15",&LVDSTrim0Hi_pin15,"LVDSTrim0Hi_pin15/D");
  tree->Branch("LVDSTrim0Hi_pin16",&LVDSTrim0Hi_pin16,"LVDSTrim0Hi_pin16/D");
  tree->Branch("LVDSTrim0Hi_pin17",&LVDSTrim0Hi_pin17,"LVDSTrim0Hi_pin17/D");
  tree->Branch("LVDSTrim0Hi_pin18",&LVDSTrim0Hi_pin18,"LVDSTrim0Hi_pin18/D");
  tree->Branch("LVDSTrim0Lo_pin01",&LVDSTrim0Lo_pin01,"LVDSTrim0Lo_pin01/D");
  tree->Branch("LVDSTrim0Lo_pin02",&LVDSTrim0Lo_pin02,"LVDSTrim0Lo_pin02/D");
  tree->Branch("LVDSTrim0Lo_pin03",&LVDSTrim0Lo_pin03,"LVDSTrim0Lo_pin03/D");
  tree->Branch("LVDSTrim0Lo_pin04",&LVDSTrim0Lo_pin04,"LVDSTrim0Lo_pin04/D");
  tree->Branch("LVDSTrim0Lo_pin05",&LVDSTrim0Lo_pin05,"LVDSTrim0Lo_pin05/D");
  tree->Branch("LVDSTrim0Lo_pin06",&LVDSTrim0Lo_pin06,"LVDSTrim0Lo_pin06/D");
  tree->Branch("LVDSTrim0Lo_pin07",&LVDSTrim0Lo_pin07,"LVDSTrim0Lo_pin07/D");
  tree->Branch("LVDSTrim0Lo_pin08",&LVDSTrim0Lo_pin08,"LVDSTrim0Lo_pin08/D");
  tree->Branch("LVDSTrim0Lo_pin09",&LVDSTrim0Lo_pin09,"LVDSTrim0Lo_pin09/D");
  tree->Branch("LVDSTrim0Lo_pin10",&LVDSTrim0Lo_pin10,"LVDSTrim0Lo_pin10/D");
  tree->Branch("LVDSTrim0Lo_pin11",&LVDSTrim0Lo_pin11,"LVDSTrim0Lo_pin11/D");
  tree->Branch("LVDSTrim0Lo_pin12",&LVDSTrim0Lo_pin12,"LVDSTrim0Lo_pin12/D");
  tree->Branch("LVDSTrim0Lo_pin13",&LVDSTrim0Lo_pin13,"LVDSTrim0Lo_pin13/D");
  tree->Branch("LVDSTrim0Lo_pin14",&LVDSTrim0Lo_pin14,"LVDSTrim0Lo_pin14/D");
  tree->Branch("LVDSTrim0Lo_pin15",&LVDSTrim0Lo_pin15,"LVDSTrim0Lo_pin15/D");
  tree->Branch("LVDSTrim0Lo_pin16",&LVDSTrim0Lo_pin16,"LVDSTrim0Lo_pin16/D");
  tree->Branch("LVDSTrim0Lo_pin17",&LVDSTrim0Lo_pin17,"LVDSTrim0Lo_pin17/D");
  tree->Branch("LVDSTrim0Lo_pin18",&LVDSTrim0Lo_pin18,"LVDSTrim0Lo_pin18/D");

  tree->Branch("LVDSTrim1Hi_pin01",&LVDSTrim1Hi_pin01,"LVDSTrim1Hi_pin01/D");
  tree->Branch("LVDSTrim1Hi_pin02",&LVDSTrim1Hi_pin02,"LVDSTrim1Hi_pin02/D");
  tree->Branch("LVDSTrim1Hi_pin03",&LVDSTrim1Hi_pin03,"LVDSTrim1Hi_pin03/D");
  tree->Branch("LVDSTrim1Hi_pin04",&LVDSTrim1Hi_pin04,"LVDSTrim1Hi_pin04/D");
  tree->Branch("LVDSTrim1Hi_pin05",&LVDSTrim1Hi_pin05,"LVDSTrim1Hi_pin05/D");
  tree->Branch("LVDSTrim1Hi_pin06",&LVDSTrim1Hi_pin06,"LVDSTrim1Hi_pin06/D");
  tree->Branch("LVDSTrim1Hi_pin07",&LVDSTrim1Hi_pin07,"LVDSTrim1Hi_pin07/D");
  tree->Branch("LVDSTrim1Hi_pin08",&LVDSTrim1Hi_pin08,"LVDSTrim1Hi_pin08/D");
  tree->Branch("LVDSTrim1Hi_pin09",&LVDSTrim1Hi_pin09,"LVDSTrim1Hi_pin09/D");
  tree->Branch("LVDSTrim1Hi_pin10",&LVDSTrim1Hi_pin10,"LVDSTrim1Hi_pin10/D");
  tree->Branch("LVDSTrim1Hi_pin11",&LVDSTrim1Hi_pin11,"LVDSTrim1Hi_pin11/D");
  tree->Branch("LVDSTrim1Hi_pin12",&LVDSTrim1Hi_pin12,"LVDSTrim1Hi_pin12/D");
  tree->Branch("LVDSTrim1Hi_pin13",&LVDSTrim1Hi_pin13,"LVDSTrim1Hi_pin13/D");
  tree->Branch("LVDSTrim1Hi_pin14",&LVDSTrim1Hi_pin14,"LVDSTrim1Hi_pin14/D");
  tree->Branch("LVDSTrim1Hi_pin15",&LVDSTrim1Hi_pin15,"LVDSTrim1Hi_pin15/D");
  tree->Branch("LVDSTrim1Hi_pin16",&LVDSTrim1Hi_pin16,"LVDSTrim1Hi_pin16/D");
  tree->Branch("LVDSTrim1Hi_pin17",&LVDSTrim1Hi_pin17,"LVDSTrim1Hi_pin17/D");
  tree->Branch("LVDSTrim1Hi_pin18",&LVDSTrim1Hi_pin18,"LVDSTrim1Hi_pin18/D");
  tree->Branch("LVDSTrim1Lo_pin01",&LVDSTrim1Lo_pin01,"LVDSTrim1Lo_pin01/D");
  tree->Branch("LVDSTrim1Lo_pin02",&LVDSTrim1Lo_pin02,"LVDSTrim1Lo_pin02/D");
  tree->Branch("LVDSTrim1Lo_pin03",&LVDSTrim1Lo_pin03,"LVDSTrim1Lo_pin03/D");
  tree->Branch("LVDSTrim1Lo_pin04",&LVDSTrim1Lo_pin04,"LVDSTrim1Lo_pin04/D");
  tree->Branch("LVDSTrim1Lo_pin05",&LVDSTrim1Lo_pin05,"LVDSTrim1Lo_pin05/D");
  tree->Branch("LVDSTrim1Lo_pin06",&LVDSTrim1Lo_pin06,"LVDSTrim1Lo_pin06/D");
  tree->Branch("LVDSTrim1Lo_pin07",&LVDSTrim1Lo_pin07,"LVDSTrim1Lo_pin07/D");
  tree->Branch("LVDSTrim1Lo_pin08",&LVDSTrim1Lo_pin08,"LVDSTrim1Lo_pin08/D");
  tree->Branch("LVDSTrim1Lo_pin09",&LVDSTrim1Lo_pin09,"LVDSTrim1Lo_pin09/D");
  tree->Branch("LVDSTrim1Lo_pin10",&LVDSTrim1Lo_pin10,"LVDSTrim1Lo_pin10/D");
  tree->Branch("LVDSTrim1Lo_pin11",&LVDSTrim1Lo_pin11,"LVDSTrim1Lo_pin11/D");
  tree->Branch("LVDSTrim1Lo_pin12",&LVDSTrim1Lo_pin12,"LVDSTrim1Lo_pin12/D");
  tree->Branch("LVDSTrim1Lo_pin13",&LVDSTrim1Lo_pin13,"LVDSTrim1Lo_pin13/D");
  tree->Branch("LVDSTrim1Lo_pin14",&LVDSTrim1Lo_pin14,"LVDSTrim1Lo_pin14/D");
  tree->Branch("LVDSTrim1Lo_pin15",&LVDSTrim1Lo_pin15,"LVDSTrim1Lo_pin15/D");
  tree->Branch("LVDSTrim1Lo_pin16",&LVDSTrim1Lo_pin16,"LVDSTrim1Lo_pin16/D");
  tree->Branch("LVDSTrim1Lo_pin17",&LVDSTrim1Lo_pin17,"LVDSTrim1Lo_pin17/D");
  tree->Branch("LVDSTrim1Lo_pin18",&LVDSTrim1Lo_pin18,"LVDSTrim1Lo_pin18/D");

  tree->Branch("LVDSTrim2Hi_pin01",&LVDSTrim2Hi_pin01,"LVDSTrim2Hi_pin01/D");
  tree->Branch("LVDSTrim2Hi_pin02",&LVDSTrim2Hi_pin02,"LVDSTrim2Hi_pin02/D");
  tree->Branch("LVDSTrim2Hi_pin03",&LVDSTrim2Hi_pin03,"LVDSTrim2Hi_pin03/D");
  tree->Branch("LVDSTrim2Hi_pin04",&LVDSTrim2Hi_pin04,"LVDSTrim2Hi_pin04/D");
  tree->Branch("LVDSTrim2Hi_pin05",&LVDSTrim2Hi_pin05,"LVDSTrim2Hi_pin05/D");
  tree->Branch("LVDSTrim2Hi_pin06",&LVDSTrim2Hi_pin06,"LVDSTrim2Hi_pin06/D");
  tree->Branch("LVDSTrim2Hi_pin07",&LVDSTrim2Hi_pin07,"LVDSTrim2Hi_pin07/D");
  tree->Branch("LVDSTrim2Hi_pin08",&LVDSTrim2Hi_pin08,"LVDSTrim2Hi_pin08/D");
  tree->Branch("LVDSTrim2Hi_pin09",&LVDSTrim2Hi_pin09,"LVDSTrim2Hi_pin09/D");
  tree->Branch("LVDSTrim2Hi_pin10",&LVDSTrim2Hi_pin10,"LVDSTrim2Hi_pin10/D");
  tree->Branch("LVDSTrim2Hi_pin11",&LVDSTrim2Hi_pin11,"LVDSTrim2Hi_pin11/D");
  tree->Branch("LVDSTrim2Hi_pin12",&LVDSTrim2Hi_pin12,"LVDSTrim2Hi_pin12/D");
  tree->Branch("LVDSTrim2Hi_pin13",&LVDSTrim2Hi_pin13,"LVDSTrim2Hi_pin13/D");
  tree->Branch("LVDSTrim2Hi_pin14",&LVDSTrim2Hi_pin14,"LVDSTrim2Hi_pin14/D");
  tree->Branch("LVDSTrim2Hi_pin15",&LVDSTrim2Hi_pin15,"LVDSTrim2Hi_pin15/D");
  tree->Branch("LVDSTrim2Hi_pin16",&LVDSTrim2Hi_pin16,"LVDSTrim2Hi_pin16/D");
  tree->Branch("LVDSTrim2Hi_pin17",&LVDSTrim2Hi_pin17,"LVDSTrim2Hi_pin17/D");
  tree->Branch("LVDSTrim2Hi_pin18",&LVDSTrim2Hi_pin18,"LVDSTrim2Hi_pin18/D");
  tree->Branch("LVDSTrim2Lo_pin01",&LVDSTrim2Lo_pin01,"LVDSTrim2Lo_pin01/D");
  tree->Branch("LVDSTrim2Lo_pin02",&LVDSTrim2Lo_pin02,"LVDSTrim2Lo_pin02/D");
  tree->Branch("LVDSTrim2Lo_pin03",&LVDSTrim2Lo_pin03,"LVDSTrim2Lo_pin03/D");
  tree->Branch("LVDSTrim2Lo_pin04",&LVDSTrim2Lo_pin04,"LVDSTrim2Lo_pin04/D");
  tree->Branch("LVDSTrim2Lo_pin05",&LVDSTrim2Lo_pin05,"LVDSTrim2Lo_pin05/D");
  tree->Branch("LVDSTrim2Lo_pin06",&LVDSTrim2Lo_pin06,"LVDSTrim2Lo_pin06/D");
  tree->Branch("LVDSTrim2Lo_pin07",&LVDSTrim2Lo_pin07,"LVDSTrim2Lo_pin07/D");
  tree->Branch("LVDSTrim2Lo_pin08",&LVDSTrim2Lo_pin08,"LVDSTrim2Lo_pin08/D");
  tree->Branch("LVDSTrim2Lo_pin09",&LVDSTrim2Lo_pin09,"LVDSTrim2Lo_pin09/D");
  tree->Branch("LVDSTrim2Lo_pin10",&LVDSTrim2Lo_pin10,"LVDSTrim2Lo_pin10/D");
  tree->Branch("LVDSTrim2Lo_pin11",&LVDSTrim2Lo_pin11,"LVDSTrim2Lo_pin11/D");
  tree->Branch("LVDSTrim2Lo_pin12",&LVDSTrim2Lo_pin12,"LVDSTrim2Lo_pin12/D");
  tree->Branch("LVDSTrim2Lo_pin13",&LVDSTrim2Lo_pin13,"LVDSTrim2Lo_pin13/D");
  tree->Branch("LVDSTrim2Lo_pin14",&LVDSTrim2Lo_pin14,"LVDSTrim2Lo_pin14/D");
  tree->Branch("LVDSTrim2Lo_pin15",&LVDSTrim2Lo_pin15,"LVDSTrim2Lo_pin15/D");
  tree->Branch("LVDSTrim2Lo_pin16",&LVDSTrim2Lo_pin16,"LVDSTrim2Lo_pin16/D");
  tree->Branch("LVDSTrim2Lo_pin17",&LVDSTrim2Lo_pin17,"LVDSTrim2Lo_pin17/D");
  tree->Branch("LVDSTrim2Lo_pin18",&LVDSTrim2Lo_pin18,"LVDSTrim2Lo_pin18/D");

  tree->Branch("LVDSTrim3Hi_pin01",&LVDSTrim3Hi_pin01,"LVDSTrim3Hi_pin01/D");
  tree->Branch("LVDSTrim3Hi_pin02",&LVDSTrim3Hi_pin02,"LVDSTrim3Hi_pin02/D");
  tree->Branch("LVDSTrim3Hi_pin03",&LVDSTrim3Hi_pin03,"LVDSTrim3Hi_pin03/D");
  tree->Branch("LVDSTrim3Hi_pin04",&LVDSTrim3Hi_pin04,"LVDSTrim3Hi_pin04/D");
  tree->Branch("LVDSTrim3Hi_pin05",&LVDSTrim3Hi_pin05,"LVDSTrim3Hi_pin05/D");
  tree->Branch("LVDSTrim3Hi_pin06",&LVDSTrim3Hi_pin06,"LVDSTrim3Hi_pin06/D");
  tree->Branch("LVDSTrim3Hi_pin07",&LVDSTrim3Hi_pin07,"LVDSTrim3Hi_pin07/D");
  tree->Branch("LVDSTrim3Hi_pin08",&LVDSTrim3Hi_pin08,"LVDSTrim3Hi_pin08/D");
  tree->Branch("LVDSTrim3Hi_pin09",&LVDSTrim3Hi_pin09,"LVDSTrim3Hi_pin09/D");
  tree->Branch("LVDSTrim3Hi_pin10",&LVDSTrim3Hi_pin10,"LVDSTrim3Hi_pin10/D");
  tree->Branch("LVDSTrim3Hi_pin11",&LVDSTrim3Hi_pin11,"LVDSTrim3Hi_pin11/D");
  tree->Branch("LVDSTrim3Hi_pin12",&LVDSTrim3Hi_pin12,"LVDSTrim3Hi_pin12/D");
  tree->Branch("LVDSTrim3Hi_pin13",&LVDSTrim3Hi_pin13,"LVDSTrim3Hi_pin13/D");
  tree->Branch("LVDSTrim3Hi_pin14",&LVDSTrim3Hi_pin14,"LVDSTrim3Hi_pin14/D");
  tree->Branch("LVDSTrim3Hi_pin15",&LVDSTrim3Hi_pin15,"LVDSTrim3Hi_pin15/D");
  tree->Branch("LVDSTrim3Hi_pin16",&LVDSTrim3Hi_pin16,"LVDSTrim3Hi_pin16/D");
  tree->Branch("LVDSTrim3Hi_pin17",&LVDSTrim3Hi_pin17,"LVDSTrim3Hi_pin17/D");
  tree->Branch("LVDSTrim3Hi_pin18",&LVDSTrim3Hi_pin18,"LVDSTrim3Hi_pin18/D");
  tree->Branch("LVDSTrim3Lo_pin01",&LVDSTrim3Lo_pin01,"LVDSTrim3Lo_pin01/D");
  tree->Branch("LVDSTrim3Lo_pin02",&LVDSTrim3Lo_pin02,"LVDSTrim3Lo_pin02/D");
  tree->Branch("LVDSTrim3Lo_pin03",&LVDSTrim3Lo_pin03,"LVDSTrim3Lo_pin03/D");
  tree->Branch("LVDSTrim3Lo_pin04",&LVDSTrim3Lo_pin04,"LVDSTrim3Lo_pin04/D");
  tree->Branch("LVDSTrim3Lo_pin05",&LVDSTrim3Lo_pin05,"LVDSTrim3Lo_pin05/D");
  tree->Branch("LVDSTrim3Lo_pin06",&LVDSTrim3Lo_pin06,"LVDSTrim3Lo_pin06/D");
  tree->Branch("LVDSTrim3Lo_pin07",&LVDSTrim3Lo_pin07,"LVDSTrim3Lo_pin07/D");
  tree->Branch("LVDSTrim3Lo_pin08",&LVDSTrim3Lo_pin08,"LVDSTrim3Lo_pin08/D");
  tree->Branch("LVDSTrim3Lo_pin09",&LVDSTrim3Lo_pin09,"LVDSTrim3Lo_pin09/D");
  tree->Branch("LVDSTrim3Lo_pin10",&LVDSTrim3Lo_pin10,"LVDSTrim3Lo_pin10/D");
  tree->Branch("LVDSTrim3Lo_pin11",&LVDSTrim3Lo_pin11,"LVDSTrim3Lo_pin11/D");
  tree->Branch("LVDSTrim3Lo_pin12",&LVDSTrim3Lo_pin12,"LVDSTrim3Lo_pin12/D");
  tree->Branch("LVDSTrim3Lo_pin13",&LVDSTrim3Lo_pin13,"LVDSTrim3Lo_pin13/D");
  tree->Branch("LVDSTrim3Lo_pin14",&LVDSTrim3Lo_pin14,"LVDSTrim3Lo_pin14/D");
  tree->Branch("LVDSTrim3Lo_pin15",&LVDSTrim3Lo_pin15,"LVDSTrim3Lo_pin15/D");
  tree->Branch("LVDSTrim3Lo_pin16",&LVDSTrim3Lo_pin16,"LVDSTrim3Lo_pin16/D");
  tree->Branch("LVDSTrim3Lo_pin17",&LVDSTrim3Lo_pin17,"LVDSTrim3Lo_pin17/D");
  tree->Branch("LVDSTrim3Lo_pin18",&LVDSTrim3Lo_pin18,"LVDSTrim3Lo_pin18/D");

  tree->Branch("ImpDiff_RinSel0",&ImpDiff_RinSel0,"ImpDiff_RinSel0/D");
  tree->Branch("ImpDiff_RinSel1",&ImpDiff_RinSel1,"ImpDiff_RinSel1/D");
  tree->Branch("ImpDiff_RinSel2",&ImpDiff_RinSel2,"ImpDiff_RinSel2/D");
  tree->Branch("ImpDiff_RinSel4",&ImpDiff_RinSel4,"ImpDiff_RinSel4/D");
  tree->Branch("ImpDiff_RinSel8",&ImpDiff_RinSel8,"ImpDiff_RinSel8/D");
  tree->Branch("ImpDiff_RinSelF",&ImpDiff_RinSelF,"ImpDiff_RinSelF/D");

  tree->Branch("InSigMClampDiff_RinSel0",&InSigMClampDiff_RinSel0,"InSigMClampDiff_RinSel0/D");
  tree->Branch("InSigMClampDiff_RinSel1",&InSigMClampDiff_RinSel1,"InSigMClampDiff_RinSel1/D");
  tree->Branch("InSigMClampDiff_RinSel2",&InSigMClampDiff_RinSel2,"InSigMClampDiff_RinSel2/D");
  tree->Branch("InSigMClampDiff_RinSel4",&InSigMClampDiff_RinSel4,"InSigMClampDiff_RinSel4/D");
  tree->Branch("InSigMClampDiff_RinSel8",&InSigMClampDiff_RinSel8,"InSigMClampDiff_RinSel8/D");

  tree->Branch("WRPmax",&WRPmax,"WRPmax/D");
  tree->Branch("WRPRange01",&WRPRange01,"WRPRange01/D");
  tree->Branch("WRPRange12",&WRPRange12,"WRPRange12/D");
  tree->Branch("WRPRange23",&WRPRange23,"WRPRange23/D");

  tree->Branch("lADC00_Dat0",&lADC00_Dat0,"lADC00_Dat0/D");
  tree->Branch("lADC01_Dat3",&lADC01_Dat3,"lADC01_Dat3/D");
  tree->Branch("lADC02_Dat6",&lADC02_Dat6,"lADC02_Dat6/D");
  tree->Branch("lADC03_Disc",&lADC03_Disc,"lADC03_Disc/D");
  tree->Branch("lADC10_Dat0Bar",&lADC10_Dat0Bar,"lADC10_Dat0Bar/D");
  tree->Branch("lADC11_Dat3Bar",&lADC11_Dat3Bar,"lADC11_Dat3Bar/D");
  tree->Branch("lADC12_Dat6Bar",&lADC12_Dat6Bar,"lADC12_Dat6Bar/D");
  tree->Branch("lADC13_DiscBar",&lADC13_DiscBar,"lADC13_DiscBar/D");
  tree->Branch("lADC20_Dat1",&lADC20_Dat1,"lADC20_Dat1/D");
  tree->Branch("lADC21_Dat4",&lADC21_Dat4,"lADC21_Dat4/D");
  tree->Branch("lADC22_Dat7",&lADC22_Dat7,"lADC22_Dat7/D");
  tree->Branch("lADC23_Rref",&lADC23_Rref,"lADC23_Rref/D");
  tree->Branch("lADC30_Dat1Bar",&lADC30_Dat1Bar,"lADC30_Dat1Bar/D");
  tree->Branch("lADC31_Dat4Bar",&lADC31_Dat4Bar,"lADC31_Dat4Bar/D");
  tree->Branch("lADC32_Dat7Bar",&lADC32_Dat7Bar,"lADC32_Dat7Bar/D");
  tree->Branch("lADC33_IDCset",&lADC33_IDCset,"lADC33_IDCset/D");
  tree->Branch("lADC40_Dat2",&lADC40_Dat2,"lADC40_Dat2/D");
  tree->Branch("lADC41_Dat5",&lADC41_Dat5,"lADC41_Dat5/D");
  tree->Branch("lADC42_ClkOut",&lADC42_ClkOut,"lADC42_ClkOut/D");
  tree->Branch("lADC43_InSig",&lADC43_InSig,"lADC43_InSig/D");
  tree->Branch("lADC50_Dat2Bar",&lADC50_Dat2Bar,"lADC50_Dat2Bar/D");
  tree->Branch("lADC51_Dat5Bar",&lADC51_Dat5Bar,"lADC51_Dat5Bar/D");
  tree->Branch("lADC52_ClkOutBar",&lADC52_ClkOutBar,"lADC52_ClkOutBar/D");
  tree->Branch("lADC53_InRef",&lADC53_InRef,"lADC53_InRef/D");
  tree->Branch("lADC60_IsetP",&lADC60_IsetP,"lADC60_IsetP/D");
  tree->Branch("lADC61_33V",&lADC61_33V,"lADC61_33V/D");
  tree->Branch("lADC62_50V",&lADC62_50V,"lADC62_50V/D");
  tree->Branch("lADC63_Clamp",&lADC63_Clamp,"lADC63_Clamp/D");

  tree->Branch("gsel00",&gsel00,"gsel00/D");
  tree->Branch("gsel01",&gsel01,"gsel01/D");
  tree->Branch("gsel02",&gsel02,"gsel02/D");
  tree->Branch("gsel04",&gsel04,"gsel04/D");
  tree->Branch("gsel08",&gsel08,"gsel08/D");
  tree->Branch("gsel16",&gsel16,"gsel16/D");

  tree->Branch("Slope_R0S0_CID0",&Slope_R0S0_CID0,"Slope_R0S0_CID0/D");
  tree->Branch("Slope_R0S1_CID0",&Slope_R0S1_CID0,"Slope_R0S1_CID0/D");
  tree->Branch("Slope_R0S2_CID0",&Slope_R0S2_CID0,"Slope_R0S2_CID0/D");
  tree->Branch("Slope_R0S3_CID0",&Slope_R0S3_CID0,"Slope_R0S3_CID0/D");
  tree->Branch("Slope_R1S0_CID0",&Slope_R1S0_CID0,"Slope_R1S0_CID0/D");
  tree->Branch("Slope_R1S1_CID0",&Slope_R1S1_CID0,"Slope_R1S1_CID0/D");
  tree->Branch("Slope_R1S2_CID0",&Slope_R1S2_CID0,"Slope_R1S2_CID0/D");
  tree->Branch("Slope_R1S3_CID0",&Slope_R1S3_CID0,"Slope_R1S3_CID0/D");
  tree->Branch("Slope_R2S0_CID0",&Slope_R2S0_CID0,"Slope_R2S0_CID0/D");
  tree->Branch("Slope_R2S1_CID0",&Slope_R2S1_CID0,"Slope_R2S1_CID0/D");
  tree->Branch("Slope_R2S2_CID0",&Slope_R2S2_CID0,"Slope_R2S2_CID0/D");
  tree->Branch("Slope_R2S3_CID0",&Slope_R2S3_CID0,"Slope_R2S3_CID0/D");
  tree->Branch("Slope_R3S0_CID0",&Slope_R3S0_CID0,"Slope_R3S0_CID0/D");
  tree->Branch("Slope_R3S1_CID0",&Slope_R3S1_CID0,"Slope_R3S1_CID0/D");
  tree->Branch("Slope_R3S2_CID0",&Slope_R3S2_CID0,"Slope_R3S2_CID0/D");
  tree->Branch("Slope_R3S3_CID0",&Slope_R3S3_CID0,"Slope_R3S3_CID0/D");

  tree->Branch("Slope_R0S0_CID1",&Slope_R0S0_CID1,"Slope_R0S0_CID1/D");
  tree->Branch("Slope_R0S1_CID1",&Slope_R0S1_CID1,"Slope_R0S1_CID1/D");
  tree->Branch("Slope_R0S2_CID1",&Slope_R0S2_CID1,"Slope_R0S2_CID1/D");
  tree->Branch("Slope_R0S3_CID1",&Slope_R0S3_CID1,"Slope_R0S3_CID1/D");
  tree->Branch("Slope_R1S0_CID1",&Slope_R1S0_CID1,"Slope_R1S0_CID1/D");
  tree->Branch("Slope_R1S1_CID1",&Slope_R1S1_CID1,"Slope_R1S1_CID1/D");
  tree->Branch("Slope_R1S2_CID1",&Slope_R1S2_CID1,"Slope_R1S2_CID1/D");
  tree->Branch("Slope_R1S3_CID1",&Slope_R1S3_CID1,"Slope_R1S3_CID1/D");
  tree->Branch("Slope_R2S0_CID1",&Slope_R2S0_CID1,"Slope_R2S0_CID1/D");
  tree->Branch("Slope_R2S1_CID1",&Slope_R2S1_CID1,"Slope_R2S1_CID1/D");
  tree->Branch("Slope_R2S2_CID1",&Slope_R2S2_CID1,"Slope_R2S2_CID1/D");
  tree->Branch("Slope_R2S3_CID1",&Slope_R2S3_CID1,"Slope_R2S3_CID1/D");
  tree->Branch("Slope_R3S0_CID1",&Slope_R3S0_CID1,"Slope_R3S0_CID1/D");
  tree->Branch("Slope_R3S1_CID1",&Slope_R3S1_CID1,"Slope_R3S1_CID1/D");
  tree->Branch("Slope_R3S2_CID1",&Slope_R3S2_CID1,"Slope_R3S2_CID1/D");
  tree->Branch("Slope_R3S3_CID1",&Slope_R3S3_CID1,"Slope_R3S3_CID1/D");

  tree->Branch("Slope_R0S0_CID2",&Slope_R0S0_CID2,"Slope_R0S0_CID2/D");
  tree->Branch("Slope_R0S1_CID2",&Slope_R0S1_CID2,"Slope_R0S1_CID2/D");
  tree->Branch("Slope_R0S2_CID2",&Slope_R0S2_CID2,"Slope_R0S2_CID2/D");
  tree->Branch("Slope_R0S3_CID2",&Slope_R0S3_CID2,"Slope_R0S3_CID2/D");
  tree->Branch("Slope_R1S0_CID2",&Slope_R1S0_CID2,"Slope_R1S0_CID2/D");
  tree->Branch("Slope_R1S1_CID2",&Slope_R1S1_CID2,"Slope_R1S1_CID2/D");
  tree->Branch("Slope_R1S2_CID2",&Slope_R1S2_CID2,"Slope_R1S2_CID2/D");
  tree->Branch("Slope_R1S3_CID2",&Slope_R1S3_CID2,"Slope_R1S3_CID2/D");
  tree->Branch("Slope_R2S0_CID2",&Slope_R2S0_CID2,"Slope_R2S0_CID2/D");
  tree->Branch("Slope_R2S1_CID2",&Slope_R2S1_CID2,"Slope_R2S1_CID2/D");
  tree->Branch("Slope_R2S2_CID2",&Slope_R2S2_CID2,"Slope_R2S2_CID2/D");
  tree->Branch("Slope_R2S3_CID2",&Slope_R2S3_CID2,"Slope_R2S3_CID2/D");
  tree->Branch("Slope_R3S0_CID2",&Slope_R3S0_CID2,"Slope_R3S0_CID2/D");
  tree->Branch("Slope_R3S1_CID2",&Slope_R3S1_CID2,"Slope_R3S1_CID2/D");
  tree->Branch("Slope_R3S2_CID2",&Slope_R3S2_CID2,"Slope_R3S2_CID2/D");
  tree->Branch("Slope_R3S3_CID2",&Slope_R3S3_CID2,"Slope_R3S3_CID2/D");

  tree->Branch("Slope_R0S0_CID3",&Slope_R0S0_CID3,"Slope_R0S0_CID3/D");
  tree->Branch("Slope_R0S1_CID3",&Slope_R0S1_CID3,"Slope_R0S1_CID3/D");
  tree->Branch("Slope_R0S2_CID3",&Slope_R0S2_CID3,"Slope_R0S2_CID3/D");
  tree->Branch("Slope_R0S3_CID3",&Slope_R0S3_CID3,"Slope_R0S3_CID3/D");
  tree->Branch("Slope_R1S0_CID3",&Slope_R1S0_CID3,"Slope_R1S0_CID3/D");
  tree->Branch("Slope_R1S1_CID3",&Slope_R1S1_CID3,"Slope_R1S1_CID3/D");
  tree->Branch("Slope_R1S2_CID3",&Slope_R1S2_CID3,"Slope_R1S2_CID3/D");
  tree->Branch("Slope_R1S3_CID3",&Slope_R1S3_CID3,"Slope_R1S3_CID3/D");
  tree->Branch("Slope_R2S0_CID3",&Slope_R2S0_CID3,"Slope_R2S0_CID3/D");
  tree->Branch("Slope_R2S1_CID3",&Slope_R2S1_CID3,"Slope_R2S1_CID3/D");
  tree->Branch("Slope_R2S2_CID3",&Slope_R2S2_CID3,"Slope_R2S2_CID3/D");
  tree->Branch("Slope_R2S3_CID3",&Slope_R2S3_CID3,"Slope_R2S3_CID3/D");
  tree->Branch("Slope_R3S0_CID3",&Slope_R3S0_CID3,"Slope_R3S0_CID3/D");
  tree->Branch("Slope_R3S1_CID3",&Slope_R3S1_CID3,"Slope_R3S1_CID3/D");
  tree->Branch("Slope_R3S2_CID3",&Slope_R3S2_CID3,"Slope_R3S2_CID3/D");
  tree->Branch("Slope_R3S3_CID3",&Slope_R3S3_CID3,"Slope_R3S3_CID3/D");


  tree->Branch("Int_R0S0_CID0",&Int_R0S0_CID0,"Int_R0S0_CID0/D");
  tree->Branch("Int_R0S1_CID0",&Int_R0S1_CID0,"Int_R0S1_CID0/D");
  tree->Branch("Int_R0S2_CID0",&Int_R0S2_CID0,"Int_R0S2_CID0/D");
  tree->Branch("Int_R0S3_CID0",&Int_R0S3_CID0,"Int_R0S3_CID0/D");
  tree->Branch("Int_R1S0_CID0",&Int_R1S0_CID0,"Int_R1S0_CID0/D");
  tree->Branch("Int_R1S1_CID0",&Int_R1S1_CID0,"Int_R1S1_CID0/D");
  tree->Branch("Int_R1S2_CID0",&Int_R1S2_CID0,"Int_R1S2_CID0/D");
  tree->Branch("Int_R1S3_CID0",&Int_R1S3_CID0,"Int_R1S3_CID0/D");
  tree->Branch("Int_R2S0_CID0",&Int_R2S0_CID0,"Int_R2S0_CID0/D");
  tree->Branch("Int_R2S1_CID0",&Int_R2S1_CID0,"Int_R2S1_CID0/D");
  tree->Branch("Int_R2S2_CID0",&Int_R2S2_CID0,"Int_R2S2_CID0/D");
  tree->Branch("Int_R2S3_CID0",&Int_R2S3_CID0,"Int_R2S3_CID0/D");
  tree->Branch("Int_R3S0_CID0",&Int_R3S0_CID0,"Int_R3S0_CID0/D");
  tree->Branch("Int_R3S1_CID0",&Int_R3S1_CID0,"Int_R3S1_CID0/D");
  tree->Branch("Int_R3S2_CID0",&Int_R3S2_CID0,"Int_R3S2_CID0/D");
  tree->Branch("Int_R3S3_CID0",&Int_R3S3_CID0,"Int_R3S3_CID0/D");

  tree->Branch("Int_R0S0_CID1",&Int_R0S0_CID1,"Int_R0S0_CID1/D");
  tree->Branch("Int_R0S1_CID1",&Int_R0S1_CID1,"Int_R0S1_CID1/D");
  tree->Branch("Int_R0S2_CID1",&Int_R0S2_CID1,"Int_R0S2_CID1/D");
  tree->Branch("Int_R0S3_CID1",&Int_R0S3_CID1,"Int_R0S3_CID1/D");
  tree->Branch("Int_R1S0_CID1",&Int_R1S0_CID1,"Int_R1S0_CID1/D");
  tree->Branch("Int_R1S1_CID1",&Int_R1S1_CID1,"Int_R1S1_CID1/D");
  tree->Branch("Int_R1S2_CID1",&Int_R1S2_CID1,"Int_R1S2_CID1/D");
  tree->Branch("Int_R1S3_CID1",&Int_R1S3_CID1,"Int_R1S3_CID1/D");
  tree->Branch("Int_R2S0_CID1",&Int_R2S0_CID1,"Int_R2S0_CID1/D");
  tree->Branch("Int_R2S1_CID1",&Int_R2S1_CID1,"Int_R2S1_CID1/D");
  tree->Branch("Int_R2S2_CID1",&Int_R2S2_CID1,"Int_R2S2_CID1/D");
  tree->Branch("Int_R2S3_CID1",&Int_R2S3_CID1,"Int_R2S3_CID1/D");
  tree->Branch("Int_R3S0_CID1",&Int_R3S0_CID1,"Int_R3S0_CID1/D");
  tree->Branch("Int_R3S1_CID1",&Int_R3S1_CID1,"Int_R3S1_CID1/D");
  tree->Branch("Int_R3S2_CID1",&Int_R3S2_CID1,"Int_R3S2_CID1/D");
  tree->Branch("Int_R3S3_CID1",&Int_R3S3_CID1,"Int_R3S3_CID1/D");

  tree->Branch("Int_R0S0_CID2",&Int_R0S0_CID2,"Int_R0S0_CID2/D");
  tree->Branch("Int_R0S1_CID2",&Int_R0S1_CID2,"Int_R0S1_CID2/D");
  tree->Branch("Int_R0S2_CID2",&Int_R0S2_CID2,"Int_R0S2_CID2/D");
  tree->Branch("Int_R0S3_CID2",&Int_R0S3_CID2,"Int_R0S3_CID2/D");
  tree->Branch("Int_R1S0_CID2",&Int_R1S0_CID2,"Int_R1S0_CID2/D");
  tree->Branch("Int_R1S1_CID2",&Int_R1S1_CID2,"Int_R1S1_CID2/D");
  tree->Branch("Int_R1S2_CID2",&Int_R1S2_CID2,"Int_R1S2_CID2/D");
  tree->Branch("Int_R1S3_CID2",&Int_R1S3_CID2,"Int_R1S3_CID2/D");
  tree->Branch("Int_R2S0_CID2",&Int_R2S0_CID2,"Int_R2S0_CID2/D");
  tree->Branch("Int_R2S1_CID2",&Int_R2S1_CID2,"Int_R2S1_CID2/D");
  tree->Branch("Int_R2S2_CID2",&Int_R2S2_CID2,"Int_R2S2_CID2/D");
  tree->Branch("Int_R2S3_CID2",&Int_R2S3_CID2,"Int_R2S3_CID2/D");
  tree->Branch("Int_R3S0_CID2",&Int_R3S0_CID2,"Int_R3S0_CID2/D");
  tree->Branch("Int_R3S1_CID2",&Int_R3S1_CID2,"Int_R3S1_CID2/D");
  tree->Branch("Int_R3S2_CID2",&Int_R3S2_CID2,"Int_R3S2_CID2/D");
  tree->Branch("Int_R3S3_CID2",&Int_R3S3_CID2,"Int_R3S3_CID2/D");

  tree->Branch("Int_R0S0_CID3",&Int_R0S0_CID3,"Int_R0S0_CID3/D");
  tree->Branch("Int_R0S1_CID3",&Int_R0S1_CID3,"Int_R0S1_CID3/D");
  tree->Branch("Int_R0S2_CID3",&Int_R0S2_CID3,"Int_R0S2_CID3/D");
  tree->Branch("Int_R0S3_CID3",&Int_R0S3_CID3,"Int_R0S3_CID3/D");
  tree->Branch("Int_R1S0_CID3",&Int_R1S0_CID3,"Int_R1S0_CID3/D");
  tree->Branch("Int_R1S1_CID3",&Int_R1S1_CID3,"Int_R1S1_CID3/D");
  tree->Branch("Int_R1S2_CID3",&Int_R1S2_CID3,"Int_R1S2_CID3/D");
  tree->Branch("Int_R1S3_CID3",&Int_R1S3_CID3,"Int_R1S3_CID3/D");
  tree->Branch("Int_R2S0_CID3",&Int_R2S0_CID3,"Int_R2S0_CID3/D");
  tree->Branch("Int_R2S1_CID3",&Int_R2S1_CID3,"Int_R2S1_CID3/D");
  tree->Branch("Int_R2S2_CID3",&Int_R2S2_CID3,"Int_R2S2_CID3/D");
  tree->Branch("Int_R2S3_CID3",&Int_R2S3_CID3,"Int_R2S3_CID3/D");
  tree->Branch("Int_R3S0_CID3",&Int_R3S0_CID3,"Int_R3S0_CID3/D");
  tree->Branch("Int_R3S1_CID3",&Int_R3S1_CID3,"Int_R3S1_CID3/D");
  tree->Branch("Int_R3S2_CID3",&Int_R3S2_CID3,"Int_R3S2_CID3/D");
  tree->Branch("Int_R3S3_CID3",&Int_R3S3_CID3,"Int_R3S3_CID3/D");

  //Cut 182 - ARRAY - 496 entries!!!
  //double ADCDNL;
  //break this up by CapID and or Range/Subrange?
  tree->Branch("ADCDNL",&ADCDNL,"ADCDNL[62]/D");

  //Cuts 183-185
  tree->Branch("OverlapR01_CID0",&OverlapR01_CID0,"OverlapR01_CID0/D");
  tree->Branch("OverlapR12_CID0",&OverlapR12_CID0,"OverlapR12_CID0/D");
  tree->Branch("OverlapR23_CID0",&OverlapR23_CID0,"OverlapR23_CID0/D");
  tree->Branch("OverlapR01_CID1",&OverlapR01_CID1,"OverlapR01_CID1/D");
  tree->Branch("OverlapR12_CID1",&OverlapR12_CID1,"OverlapR12_CID1/D");
  tree->Branch("OverlapR23_CID1",&OverlapR23_CID1,"OverlapR23_CID1/D");
  tree->Branch("OverlapR01_CID2",&OverlapR01_CID2,"OverlapR01_CID2/D");
  tree->Branch("OverlapR12_CID2",&OverlapR12_CID2,"OverlapR12_CID2/D");
  tree->Branch("OverlapR23_CID2",&OverlapR23_CID2,"OverlapR23_CID2/D");
  tree->Branch("OverlapR01_CID3",&OverlapR01_CID3,"OverlapR01_CID3/D");
  tree->Branch("OverlapR12_CID3",&OverlapR12_CID3,"OverlapR12_CID3/D");
  tree->Branch("OverlapR23_CID3",&OverlapR23_CID3,"OverlapR23_CID3/D");

  tree->Branch("Pedestal_CID0",&Pedestal_CID0,"Pedestal_CID0/D");
  tree->Branch("Pedestal_CID1",&Pedestal_CID1,"Pedestal_CID1/D");
  tree->Branch("Pedestal_CID2",&Pedestal_CID2,"Pedestal_CID2/D");
  tree->Branch("Pedestal_CID3",&Pedestal_CID3,"Pedestal_CID3/D");

  tree->Branch("PedestalRMS_CID0",&PedestalRMS_CID0,"PedestalRMS_CID0/D");
  tree->Branch("PedestalRMS_CID1",&PedestalRMS_CID1,"PedestalRMS_CID1/D");
  tree->Branch("PedestalRMS_CID2",&PedestalRMS_CID2,"PedestalRMS_CID2/D");
  tree->Branch("PedestalRMS_CID3",&PedestalRMS_CID3,"PedestalRMS_CID3/D");

  tree->Branch("PedestalDACSlope_Lo",&PedestalDACSlope_Lo,"PedestalDACSlope_Lo/D");
  tree->Branch("PedestalDACSlope_Hi",&PedestalDACSlope_Hi,"PedestalDACSlope_Hi/D");


  tree->Branch("CapIDPedDACSlope_CID0",&CapIDPedDACSlope_CID0,"CapIDPedDACSlope_CID0/D");
  tree->Branch("CapIDPedDACSlope_CID1",&CapIDPedDACSlope_CID1,"CapIDPedDACSlope_CID1/D");
  tree->Branch("CapIDPedDACSlope_CID2",&CapIDPedDACSlope_CID2,"CapIDPedDACSlope_CID2/D");
  tree->Branch("CapIDPedDACSlope_CID3",&CapIDPedDACSlope_CID3,"CapIDPedDACSlope_CID3/D");

  //double TCDVertOffset[255]; // 255 of these
  tree->Branch("TDCBinWidth",&TDCBinWidth,"TDCBinWidth/D");
  tree->Branch("TDCVertOffset",&TDCVertOffset,"TDCVertOffset[255]/D");
  
  tree->Branch("Sum4Average",&Sum4Average,"Sum4Average/D");
  tree->Branch("Sum4DNL",&Sum4DNL,"Sum4DNL[255]/D");
  
}


void ResetVarMap() {
  
  ChipStatus=-1;
  ChipID=0;
  
  vars["1_1"] = 0;
  vars["2_1"] = 0;
  vars["3_1"] = 0;
  vars["4_1"] = 0;
  vars["5_1"] = 0;
  vars["6_1"] = 0;
  vars["7_1"] = 0;
  vars["8_1"] = 0;
  vars["9_1"] = 0;
  vars["10_1"] = 0;
  vars["11_1"] = 0;
  vars["12_1"] = 0;
  vars["13_1"] = 0;
  vars["14_1"] = 0;
  vars["15_1"] = 0;
  vars["16_1"] = 0;
  vars["17_1"] = 0;
  vars["18_1"] = 0;
  vars["19_1"] = 0;
  vars["20_1"] = 0;
  vars["21_1"] = 0;
  vars["22_1"] = 0;
  vars["23_1"] = 0;
  vars["24_1"] = 0;
  vars["25_1"] = 0;
  vars["26_1"] = 0;
  vars["27_1"] = 0;
  vars["28_1"] = 0;
  vars["29_1"] = 0;
  vars["30_1"] = 0;
  vars["31_1"] = 0;
  vars["32_1"] = 0;
  vars["33_1"] = 0;
  vars["34_1"] = 0;
  vars["35_1"] = 0;
  vars["36_1"] = 0;
  vars["37_1"] = 0;
  vars["38_1"] = 0;
  vars["39_1"] = 0;
  vars["40_1"] = 0;
  vars["41_1"] = 0;
  vars["42_1"] = 0;
  vars["43_1"] = 0;
  vars["44_1"] = 0;
  vars["45_1"] = 0;
  vars["46_1"] = 0;
  vars["47_1"] = 0;
  vars["48_1"] = 0;
  vars["49_1"] = 0;
  vars["50_1"] = 0;
  vars["50_2"] = 0;
  vars["50_3"] = 0;
  vars["50_4"] = 0;
  vars["51_1"] = 0;
  vars["52_1"] = 0;
  vars["53_1"] = 0;
  vars["54_1"] = 0;
  vars["55_1"] = 0;
  vars["56_1"] = 0;
  vars["57_1"] = 0;
  vars["58_1"] = 0;
  vars["59_1"] = 0;
  vars["60_1"] = 0;
  vars["61_1"] = 0;
  vars["62_1"] = 0;
  vars["63_1"] = 0;
  vars["64_1"] = 0;
  vars["65_1"] = 0;
  vars["66_1"] = 0;
  vars["67_1"] = 0;
  vars["68_1"] = 0;
  vars["69_1"] = 0;
  vars["70_1"] = 0;
  vars["71_1"] = 0;
  vars["72_1"] = 0;
  vars["73_1"] = 0;
  vars["74_1"] = 0;
  vars["75_1"] = 0;
  vars["76_1"] = 0;
  vars["77_1"] = 0;
  vars["78_1"] = 0;
  vars["79_1"] = 0;
  vars["80_1"] = 0;
  vars["81_1"] = 0;
  vars["82_1"] = 0;
  vars["83_1"] = 0;
  vars["84_1"] = 0;
  vars["85_1"] = 0;
  vars["86_1"] = 0;
  vars["86_2"] = 0;
  vars["86_3"] = 0;
  vars["86_4"] = 0;
  vars["86_5"] = 0;
  vars["86_6"] = 0;
  vars["86_7"] = 0;
  vars["86_8"] = 0;
  vars["86_9"] = 0;
  vars["86_10"] = 0;
  vars["86_11"] = 0;
  vars["86_12"] = 0;
  vars["86_13"] = 0;
  vars["86_14"] = 0;
  vars["86_15"] = 0;
  vars["86_16"] = 0;
  vars["86_17"] = 0;
  vars["86_18"] = 0;
  vars["87_1"] = 0;
  vars["87_2"] = 0;
  vars["87_3"] = 0;
  vars["87_4"] = 0;
  vars["87_5"] = 0;
  vars["87_6"] = 0;
  vars["87_7"] = 0;
  vars["87_8"] = 0;
  vars["87_9"] = 0;
  vars["87_10"] = 0;
  vars["87_11"] = 0;
  vars["87_12"] = 0;
  vars["87_13"] = 0;
  vars["87_14"] = 0;
  vars["87_15"] = 0;
  vars["87_16"] = 0;
  vars["87_17"] = 0;
  vars["87_18"] = 0;
  vars["88_1"] = 0;
  vars["88_2"] = 0;
  vars["88_3"] = 0;
  vars["88_4"] = 0;
  vars["88_5"] = 0;
  vars["88_6"] = 0;
  vars["88_7"] = 0;
  vars["88_8"] = 0;
  vars["88_9"] = 0;
  vars["88_10"] = 0;
  vars["88_11"] = 0;
  vars["88_12"] = 0;
  vars["88_13"] = 0;
  vars["88_14"] = 0;
  vars["88_15"] = 0;
  vars["88_16"] = 0;
  vars["88_17"] = 0;
  vars["88_18"] = 0;
  vars["89_1"] = 0;
  vars["89_2"] = 0;
  vars["89_3"] = 0;
  vars["89_4"] = 0;
  vars["89_5"] = 0;
  vars["89_6"] = 0;
  vars["89_7"] = 0;
  vars["89_8"] = 0;
  vars["89_9"] = 0;
  vars["89_10"] = 0;
  vars["89_11"] = 0;
  vars["89_12"] = 0;
  vars["89_13"] = 0;
  vars["89_14"] = 0;
  vars["89_15"] = 0;
  vars["89_16"] = 0;
  vars["89_17"] = 0;
  vars["89_18"] = 0;
  vars["90_1"] = 0;
  vars["90_2"] = 0;
  vars["90_3"] = 0;
  vars["90_4"] = 0;
  vars["90_5"] = 0;
  vars["90_6"] = 0;
  vars["90_7"] = 0;
  vars["90_8"] = 0;
  vars["90_9"] = 0;
  vars["90_10"] = 0;
  vars["90_11"] = 0;
  vars["90_12"] = 0;
  vars["90_13"] = 0;
  vars["90_14"] = 0;
  vars["90_15"] = 0;
  vars["90_16"] = 0;
  vars["90_17"] = 0;
  vars["90_18"] = 0;
  vars["91_1"] = 0;
  vars["91_2"] = 0;
  vars["91_3"] = 0;
  vars["91_4"] = 0;
  vars["91_5"] = 0;
  vars["91_6"] = 0;
  vars["91_7"] = 0;
  vars["91_8"] = 0;
  vars["91_9"] = 0;
  vars["91_10"] = 0;
  vars["91_11"] = 0;
  vars["91_12"] = 0;
  vars["91_13"] = 0;
  vars["91_14"] = 0;
  vars["91_15"] = 0;
  vars["91_16"] = 0;
  vars["91_17"] = 0;
  vars["91_18"] = 0;
  vars["92_1"] = 0;
  vars["92_2"] = 0;
  vars["92_3"] = 0;
  vars["92_4"] = 0;
  vars["92_5"] = 0;
  vars["92_6"] = 0;
  vars["92_7"] = 0;
  vars["92_8"] = 0;
  vars["92_9"] = 0;
  vars["92_10"] = 0;
  vars["92_11"] = 0;
  vars["92_12"] = 0;
  vars["92_13"] = 0;
  vars["92_14"] = 0;
  vars["92_15"] = 0;
  vars["92_16"] = 0;
  vars["92_17"] = 0;
  vars["92_18"] = 0;
  vars["93_1"] = 0;
  vars["93_2"] = 0;
  vars["93_3"] = 0;
  vars["93_4"] = 0;
  vars["93_5"] = 0;
  vars["93_6"] = 0;
  vars["93_7"] = 0;
  vars["93_8"] = 0;
  vars["93_9"] = 0;
  vars["93_10"] = 0;
  vars["93_11"] = 0;
  vars["93_12"] = 0;
  vars["93_13"] = 0;
  vars["93_14"] = 0;
  vars["93_15"] = 0;
  vars["93_16"] = 0;
  vars["93_17"] = 0;
  vars["93_18"] = 0;
  vars["94_1"] = 0;
  vars["95_1"] = 0;
  vars["96_1"] = 0;
  vars["97_1"] = 0;
  vars["98_1"] = 0;
  vars["99_1"] = 0;
  vars["100_1"] = 0;
  vars["101_1"] = 0;
  vars["102_1"] = 0;
  vars["103_1"] = 0;
  vars["104_1"] = 0;
  vars["105_1"] = 0;
  vars["106_1"] = 0;
  vars["107_1"] = 0;
  vars["108_1"] = 0;
  vars["109_1"] = 0;
  vars["110_1"] = 0;
  vars["111_1"] = 0;
  vars["112_1"] = 0;
  vars["113_1"] = 0;
  vars["114_1"] = 0;
  vars["115_1"] = 0;
  vars["116_1"] = 0;
  vars["117_1"] = 0;
  vars["118_1"] = 0;
  vars["119_1"] = 0;
  vars["120_1"] = 0;
  vars["121_1"] = 0;
  vars["122_1"] = 0;
  vars["123_1"] = 0;
  vars["124_1"] = 0;
  vars["125_1"] = 0;
  vars["126_1"] = 0;
  vars["127_1"] = 0;
  vars["128_1"] = 0;
  vars["129_1"] = 0;
  vars["130_1"] = 0;
  vars["131_1"] = 0;
  vars["132_1"] = 0;
  vars["133_1"] = 0;
  vars["134_1"] = 0;
  vars["135_1"] = 0;
  vars["136_1"] = 0;
  vars["137_1"] = 0;
  vars["138_1"] = 0;
  vars["139_1"] = 0;
  vars["140_1"] = 0;
  vars["141_1"] = 0;
  vars["142_1"] = 0;
  vars["143_1"] = 0;
  vars["144_1"] = 0;
  vars["145_1"] = 0;
  vars["146_1"] = 0;
  vars["147_1"] = 0;
  vars["148_1"] = 0;
  vars["149_1"] = 0;
  vars["150_1"] = 0;
  vars["151_1"] = 0;
  vars["152_1"] = 0;
  vars["153_1"] = 0;
  vars["154_1"] = 0;
  vars["155_1"] = 0;
  vars["156_1"] = 0;
  vars["157_1"] = 0;
  vars["158_1"] = 0;
  vars["159_1"] = 0;
  vars["160_1"] = 0;
  vars["161_1"] = 0;
  vars["162_1"] = 0;
  vars["163_1"] = 0;
  vars["164_1"] = 0;
  vars["165_1"] = 0;
  vars["166_1"] = 0;
  vars["167_1"] = 0;
  vars["168_1"] = 0;
  vars["169_1"] = 0;
  vars["170_1"] = 0;
  vars["171_1"] = 0;
  vars["172_1"] = 0;
  vars["173_1"] = 0;
  vars["174_1"] = 0;
  vars["175_1"] = 0;
  vars["176_1"] = 0;
  vars["177_1"] = 0;
  vars["178_1"] = 0;
  vars["179_1"] = 0;
  vars["180_1"] = 0;
  vars["181_1"] = 0;
  vars["150_2"] = 0;
  vars["151_2"] = 0;
  vars["152_2"] = 0;
  vars["153_2"] = 0;
  vars["154_2"] = 0;
  vars["155_2"] = 0;
  vars["156_2"] = 0;
  vars["157_2"] = 0;
  vars["158_2"] = 0;
  vars["159_2"] = 0;
  vars["160_2"] = 0;
  vars["161_2"] = 0;
  vars["162_2"] = 0;
  vars["163_2"] = 0;
  vars["164_2"] = 0;
  vars["165_2"] = 0;
  vars["166_2"] = 0;
  vars["167_2"] = 0;
  vars["168_2"] = 0;
  vars["169_2"] = 0;
  vars["170_2"] = 0;
  vars["171_2"] = 0;
  vars["172_2"] = 0;
  vars["173_2"] = 0;
  vars["174_2"] = 0;
  vars["175_2"] = 0;
  vars["176_2"] = 0;
  vars["177_2"] = 0;
  vars["178_2"] = 0;
  vars["179_2"] = 0;
  vars["180_2"] = 0;
  vars["181_2"] = 0;
  vars["150_3"] = 0;
  vars["151_3"] = 0;
  vars["152_3"] = 0;
  vars["153_3"] = 0;
  vars["154_3"] = 0;
  vars["155_3"] = 0;
  vars["156_3"] = 0;
  vars["157_3"] = 0;
  vars["158_3"] = 0;
  vars["159_3"] = 0;
  vars["160_3"] = 0;
  vars["161_3"] = 0;
  vars["162_3"] = 0;
  vars["163_3"] = 0;
  vars["164_3"] = 0;
  vars["165_3"] = 0;
  vars["166_3"] = 0;
  vars["167_3"] = 0;
  vars["168_3"] = 0;
  vars["169_3"] = 0;
  vars["170_3"] = 0;
  vars["171_3"] = 0;
  vars["172_3"] = 0;
  vars["173_3"] = 0;
  vars["174_3"] = 0;
  vars["175_3"] = 0;
  vars["176_3"] = 0;
  vars["177_3"] = 0;
  vars["178_3"] = 0;
  vars["179_3"] = 0;
  vars["180_3"] = 0;
  vars["181_3"] = 0;
  vars["150_4"] = 0;
  vars["151_4"] = 0;
  vars["152_4"] = 0;
  vars["153_4"] = 0;
  vars["154_4"] = 0;
  vars["155_4"] = 0;
  vars["156_4"] = 0;
  vars["157_4"] = 0;
  vars["158_4"] = 0;
  vars["159_4"] = 0;
  vars["160_4"] = 0;
  vars["161_4"] = 0;
  vars["162_4"] = 0;
  vars["163_4"] = 0;
  vars["164_4"] = 0;
  vars["165_4"] = 0;
  vars["166_4"] = 0;
  vars["167_4"] = 0;
  vars["168_4"] = 0;
  vars["169_4"] = 0;
  vars["170_4"] = 0;
  vars["171_4"] = 0;
  vars["172_4"] = 0;
  vars["173_4"] = 0;
  vars["174_4"] = 0;
  vars["175_4"] = 0;
  vars["176_4"] = 0;
  vars["177_4"] = 0;
  vars["178_4"] = 0;
  vars["179_4"] = 0;
  vars["180_4"] = 0;
  vars["181_4"] = 0;
  vars["182_1"] = 0;

  vars["183_1"] = 0;
  vars["184_1"] = 0;
  vars["185_1"] = 0;
  vars["186_1"] = 0;

  vars["183_2"] = 0;
  vars["184_2"] = 0;
  vars["185_2"] = 0;
  vars["186_2"] = 0;

  vars["183_3"] = 0;
  vars["184_3"] = 0;
  vars["185_3"] = 0;
  vars["186_3"] = 0;

  vars["183_4"] = 0;
  vars["184_4"] = 0;
  vars["185_4"] = 0;
  vars["186_4"] = 0;


  vars["187_1"] = 0;
  vars["187_2"] = 0;

  vars["188_1"] = 0;
  vars["189_1"] = 0;
  vars["188_2"] = 0;
  vars["189_2"] = 0;
  vars["188_3"] = 0;
  vars["189_3"] = 0;
  vars["188_4"] = 0;
  vars["189_4"] = 0;


  vars["190_1"] = 0;
  vars["191_1"] = 0;
  vars["192_1"] = 0;
  vars["193_1"] = 0;
  vars["194_1"] = 0;
  vars["195_1"] = 0;
  vars["196_1"] = 0;
  vars["197_1"] = 0;
  vars["198_1"] = 0;
  vars["199_1"] = 0;
  vars["200_1"] = 0;

}

void ResetVariables() {
  
  ChipStatus=-1;
  ChipID=0;
  
  Current33_NoC=-99;
    Current50_NoC=-99;
    Current33_C=-99;
    Current50_C=-99;
    Volt_Isetp=-99;
    Volt_33V=-99;
    Volt_50V=-99;
    Volt_Clamp=-99;
    Volt_ClampCalMode=-99;
    Volt_Rref=-99;
    Volt_IDCSet=-99;
    Volt_InSig=-99;
    Volt_InRef=-99;
    IDCset00=-99;
    IDCset01=-99;
    IDCset02=-99;
    IDCset03=-99;
    IDCset04=-99;
    IDCset05=-99;
    IDCset06=-99;
    IDCset07=-99;
    IDCset08=-99;
    IDCset09=-99;
    IDCset0a=-99;
    IDCset0b=-99;
    IDCset0c=-99;
    IDCset0d=-99;
    IDCset0e=-99;
    IDCset0f=-99;
    IDCset10=-99;
    IDCset11=-99;
    IDCset12=-99;
    IDCset13=-99;
    IDCset14=-99;
    IDCset15=-99;
    IDCset16=-99;
    IDCset17=-99;
    IDCset18=-99;
    IDCset19=-99;
    IDCset1a=-99;
    IDCset1b=-99;
    IDCset1c=-99;
    IDCset1d=-99;
    IDCset1e=-99;
    IDCset1f=-99;
    LVDS_AvgMid_1=-99;
    LVDS_AvgMid_2=-99;
    LVDS_AvgMid_3=-99;
    LVDS_AvgMid_4=-99;
    SumRange0=-99;
    SumRange1=-99;
    SumRange2=-99;
    SumRange3=-99;
    SumDAC0=-99;
    SumDAC1=-99;
    SumDAC2=-99;
    SumDAC3=-99;
    SumDAC4=-99;
    SumDAC5=-99;
    SumDAC6=-99;
    SumDAC7=-99;
    ThreshDAC0=-99;
    ThreshDAC1=-99;
    ThreshDAC2=-99;
    ThreshDAC4=-99;
    ThreshDAC8=-99;
    ThreshDAC10=-99;
    ThreshDAC20=-99;
    ThreshDAC40=-99;
    ThreshDAC80=-99;
    LockTime120Mhz=-99;
    LockTime1Mhz=-99;
    InSigV=-99;
    InRefV=-99;
    DiffInSigInRef=-99;
    TimingInRefDAC0=-99;
    TimingInRefDAC1=-99;
    TimingInRefDAC2=-99;
    TimingInRefDAC3=-99;
    TimingInRefDAC4=-99;
    TimingInRefDAC5=-99;
    TimingInRefDAC6=-99;
    TimingInRefDAC7=-99;
    LVDSTrim0Hi_pin01=-99;
    LVDSTrim0Hi_pin02=-99;
    LVDSTrim0Hi_pin03=-99;
    LVDSTrim0Hi_pin04=-99;
    LVDSTrim0Hi_pin05=-99;
    LVDSTrim0Hi_pin06=-99;
    LVDSTrim0Hi_pin07=-99;
    LVDSTrim0Hi_pin08=-99;
    LVDSTrim0Hi_pin09=-99;
    LVDSTrim0Hi_pin10=-99;
    LVDSTrim0Hi_pin11=-99;
    LVDSTrim0Hi_pin12=-99;
    LVDSTrim0Hi_pin13=-99;
    LVDSTrim0Hi_pin14=-99;
    LVDSTrim0Hi_pin15=-99;
    LVDSTrim0Hi_pin16=-99;
    LVDSTrim0Hi_pin17=-99;
    LVDSTrim0Hi_pin18=-99;
    LVDSTrim0Lo_pin01=-99;
    LVDSTrim0Lo_pin02=-99;
    LVDSTrim0Lo_pin03=-99;
    LVDSTrim0Lo_pin04=-99;
    LVDSTrim0Lo_pin05=-99;
    LVDSTrim0Lo_pin06=-99;
    LVDSTrim0Lo_pin07=-99;
    LVDSTrim0Lo_pin08=-99;
    LVDSTrim0Lo_pin09=-99;
    LVDSTrim0Lo_pin10=-99;
    LVDSTrim0Lo_pin11=-99;
    LVDSTrim0Lo_pin12=-99;
    LVDSTrim0Lo_pin13=-99;
    LVDSTrim0Lo_pin14=-99;
    LVDSTrim0Lo_pin15=-99;
    LVDSTrim0Lo_pin16=-99;
    LVDSTrim0Lo_pin17=-99;
    LVDSTrim0Lo_pin18=-99;
    LVDSTrim1Hi_pin01=-99;
    LVDSTrim1Hi_pin02=-99;
    LVDSTrim1Hi_pin03=-99;
    LVDSTrim1Hi_pin04=-99;
    LVDSTrim1Hi_pin05=-99;
    LVDSTrim1Hi_pin06=-99;
    LVDSTrim1Hi_pin07=-99;
    LVDSTrim1Hi_pin08=-99;
    LVDSTrim1Hi_pin09=-99;
    LVDSTrim1Hi_pin10=-99;
    LVDSTrim1Hi_pin11=-99;
    LVDSTrim1Hi_pin12=-99;
    LVDSTrim1Hi_pin13=-99;
    LVDSTrim1Hi_pin14=-99;
    LVDSTrim1Hi_pin15=-99;
    LVDSTrim1Hi_pin16=-99;
    LVDSTrim1Hi_pin17=-99;
    LVDSTrim1Hi_pin18=-99;
    LVDSTrim1Lo_pin01=-99;
    LVDSTrim1Lo_pin02=-99;
    LVDSTrim1Lo_pin03=-99;
    LVDSTrim1Lo_pin04=-99;
    LVDSTrim1Lo_pin05=-99;
    LVDSTrim1Lo_pin06=-99;
    LVDSTrim1Lo_pin07=-99;
    LVDSTrim1Lo_pin08=-99;
    LVDSTrim1Lo_pin09=-99;
    LVDSTrim1Lo_pin10=-99;
    LVDSTrim1Lo_pin11=-99;
    LVDSTrim1Lo_pin12=-99;
    LVDSTrim1Lo_pin13=-99;
    LVDSTrim1Lo_pin14=-99;
    LVDSTrim1Lo_pin15=-99;
    LVDSTrim1Lo_pin16=-99;
    LVDSTrim1Lo_pin17=-99;
    LVDSTrim1Lo_pin18=-99;
    LVDSTrim2Hi_pin01=-99;
    LVDSTrim2Hi_pin02=-99;
    LVDSTrim2Hi_pin03=-99;
    LVDSTrim2Hi_pin04=-99;
    LVDSTrim2Hi_pin05=-99;
    LVDSTrim2Hi_pin06=-99;
    LVDSTrim2Hi_pin07=-99;
    LVDSTrim2Hi_pin08=-99;
    LVDSTrim2Hi_pin09=-99;
    LVDSTrim2Hi_pin10=-99;
    LVDSTrim2Hi_pin11=-99;
    LVDSTrim2Hi_pin12=-99;
    LVDSTrim2Hi_pin13=-99;
    LVDSTrim2Hi_pin14=-99;
    LVDSTrim2Hi_pin15=-99;
    LVDSTrim2Hi_pin16=-99;
    LVDSTrim2Hi_pin17=-99;
    LVDSTrim2Hi_pin18=-99;
    LVDSTrim2Lo_pin01=-99;
    LVDSTrim2Lo_pin02=-99;
    LVDSTrim2Lo_pin03=-99;
    LVDSTrim2Lo_pin04=-99;
    LVDSTrim2Lo_pin05=-99;
    LVDSTrim2Lo_pin06=-99;
    LVDSTrim2Lo_pin07=-99;
    LVDSTrim2Lo_pin08=-99;
    LVDSTrim2Lo_pin09=-99;
    LVDSTrim2Lo_pin10=-99;
    LVDSTrim2Lo_pin11=-99;
    LVDSTrim2Lo_pin12=-99;
    LVDSTrim2Lo_pin13=-99;
    LVDSTrim2Lo_pin14=-99;
    LVDSTrim2Lo_pin15=-99;
    LVDSTrim2Lo_pin16=-99;
    LVDSTrim2Lo_pin17=-99;
    LVDSTrim2Lo_pin18=-99;
    LVDSTrim3Hi_pin01=-99;
    LVDSTrim3Hi_pin02=-99;
    LVDSTrim3Hi_pin03=-99;
    LVDSTrim3Hi_pin04=-99;
    LVDSTrim3Hi_pin05=-99;
    LVDSTrim3Hi_pin06=-99;
    LVDSTrim3Hi_pin07=-99;
    LVDSTrim3Hi_pin08=-99;
    LVDSTrim3Hi_pin09=-99;
    LVDSTrim3Hi_pin10=-99;
    LVDSTrim3Hi_pin11=-99;
    LVDSTrim3Hi_pin12=-99;
    LVDSTrim3Hi_pin13=-99;
    LVDSTrim3Hi_pin14=-99;
    LVDSTrim3Hi_pin15=-99;
    LVDSTrim3Hi_pin16=-99;
    LVDSTrim3Hi_pin17=-99;
    LVDSTrim3Hi_pin18=-99;
    LVDSTrim3Lo_pin01=-99;
    LVDSTrim3Lo_pin02=-99;
    LVDSTrim3Lo_pin03=-99;
    LVDSTrim3Lo_pin04=-99;
    LVDSTrim3Lo_pin05=-99;
    LVDSTrim3Lo_pin06=-99;
    LVDSTrim3Lo_pin07=-99;
    LVDSTrim3Lo_pin08=-99;
    LVDSTrim3Lo_pin09=-99;
    LVDSTrim3Lo_pin10=-99;
    LVDSTrim3Lo_pin11=-99;
    LVDSTrim3Lo_pin12=-99;
    LVDSTrim3Lo_pin13=-99;
    LVDSTrim3Lo_pin14=-99;
    LVDSTrim3Lo_pin15=-99;
    LVDSTrim3Lo_pin16=-99;
    LVDSTrim3Lo_pin17=-99;
    LVDSTrim3Lo_pin18=-99;
    ImpDiff_RinSel0=-99;
    InSigMClampDiff_RinSel0=-99;
    ImpDiff_RinSel1=-99;
    InSigMClampDiff_RinSel1=-99;
    ImpDiff_RinSel2=-99;
    InSigMClampDiff_RinSel2=-99;
    ImpDiff_RinSel4=-99;
    InSigMClampDiff_RinSel4=-99;
    ImpDiff_RinSel8=-99;
    InSigMClampDiff_RinSel8=-99;
    ImpDiff_RinSelF=-99;

    WRPmax=-99;
    WRPRange01=-99;
    WRPRange12=-99;
    WRPRange23=-99;
    lADC00_Dat0=-99;
    lADC01_Dat3=-99;
    lADC02_Dat6=-99;
    lADC03_Disc=-99;
    lADC10_Dat0Bar=-99;
    lADC11_Dat3Bar=-99;
    lADC12_Dat6Bar=-99;
    lADC13_DiscBar=-99;
    lADC20_Dat1=-99;
    lADC21_Dat4=-99;
    lADC22_Dat7=-99;
    lADC23_Rref=-99;
    lADC30_Dat1Bar=-99;
    lADC31_Dat4Bar=-99;
    lADC32_Dat7Bar=-99;
    lADC33_IDCset=-99;
    lADC40_Dat2=-99;
    lADC41_Dat5=-99;
    lADC42_ClkOut=-99;
    lADC43_InSig=-99;
    lADC50_Dat2Bar=-99;
    lADC51_Dat5Bar=-99;
    lADC52_ClkOutBar=-99;
    lADC53_InRef=-99;
    lADC60_IsetP=-99;
    lADC61_33V=-99;
    lADC62_50V=-99;
    lADC63_Clamp=-99;
    gsel00 = -99;
    gsel01 = -99;
    gsel02 = -99;
    gsel04 = -99;
    gsel08 = -99;
    gsel16 = -99;
    Slope_R0S0_CID0=-99;
    Slope_R0S1_CID0=-99;
    Slope_R0S2_CID0=-99;
    Slope_R0S3_CID0=-99;
    Slope_R1S0_CID0=-99;
    Slope_R1S1_CID0=-99;
    Slope_R1S2_CID0=-99;
    Slope_R1S3_CID0=-99;
    Slope_R2S0_CID0=-99;
    Slope_R2S1_CID0=-99;
    Slope_R2S2_CID0=-99;
    Slope_R2S3_CID0=-99;
    Slope_R3S0_CID0=-99;
    Slope_R3S1_CID0=-99;
    Slope_R3S2_CID0=-99;
    Slope_R3S3_CID0=-99;
    Slope_R0S0_CID1=-99;
    Slope_R0S1_CID1=-99;
    Slope_R0S2_CID1=-99;
    Slope_R0S3_CID1=-99;
    Slope_R1S0_CID1=-99;
    Slope_R1S1_CID1=-99;
    Slope_R1S2_CID1=-99;
    Slope_R1S3_CID1=-99;
    Slope_R2S0_CID1=-99;
    Slope_R2S1_CID1=-99;
    Slope_R2S2_CID1=-99;
    Slope_R2S3_CID1=-99;
    Slope_R3S0_CID1=-99;
    Slope_R3S1_CID1=-99;
    Slope_R3S2_CID1=-99;
    Slope_R3S3_CID1=-99;
    Slope_R0S0_CID2=-99;
    Slope_R0S1_CID2=-99;
    Slope_R0S2_CID2=-99;
    Slope_R0S3_CID2=-99;
    Slope_R1S0_CID2=-99;
    Slope_R1S1_CID2=-99;
    Slope_R1S2_CID2=-99;
    Slope_R1S3_CID2=-99;
    Slope_R2S0_CID2=-99;
    Slope_R2S1_CID2=-99;
    Slope_R2S2_CID2=-99;
    Slope_R2S3_CID2=-99;
    Slope_R3S0_CID2=-99;
    Slope_R3S1_CID2=-99;
    Slope_R3S2_CID2=-99;
    Slope_R3S3_CID2=-99;
    Slope_R0S0_CID3=-99;
    Slope_R0S1_CID3=-99;
    Slope_R0S2_CID3=-99;
    Slope_R0S3_CID3=-99;
    Slope_R1S0_CID3=-99;
    Slope_R1S1_CID3=-99;
    Slope_R1S2_CID3=-99;
    Slope_R1S3_CID3=-99;
    Slope_R2S0_CID3=-99;
    Slope_R2S1_CID3=-99;
    Slope_R2S2_CID3=-99;
    Slope_R2S3_CID3=-99;
    Slope_R3S0_CID3=-99;
    Slope_R3S1_CID3=-99;
    Slope_R3S2_CID3=-99;
    Slope_R3S3_CID3=-99;
    Int_R0S0_CID0=-99;
    Int_R0S1_CID0=-99;
    Int_R0S2_CID0=-99;
    Int_R0S3_CID0=-99;
    Int_R1S0_CID0=-99;
    Int_R1S1_CID0=-99;
    Int_R1S2_CID0=-99;
    Int_R1S3_CID0=-99;
    Int_R2S0_CID0=-99;
    Int_R2S1_CID0=-99;
    Int_R2S2_CID0=-99;
    Int_R2S3_CID0=-99;
    Int_R3S0_CID0=-99;
    Int_R3S1_CID0=-99;
    Int_R3S2_CID0=-99;
    Int_R3S3_CID0=-99;
    Int_R0S0_CID1=-99;
    Int_R0S1_CID1=-99;
    Int_R0S2_CID1=-99;
    Int_R0S3_CID1=-99;
    Int_R1S0_CID1=-99;
    Int_R1S1_CID1=-99;
    Int_R1S2_CID1=-99;
    Int_R1S3_CID1=-99;
    Int_R2S0_CID1=-99;
    Int_R2S1_CID1=-99;
    Int_R2S2_CID1=-99;
    Int_R2S3_CID1=-99;
    Int_R3S0_CID1=-99;
    Int_R3S1_CID1=-99;
    Int_R3S2_CID1=-99;
    Int_R3S3_CID1=-99;
    Int_R0S0_CID2=-99;
    Int_R0S1_CID2=-99;
    Int_R0S2_CID2=-99;
    Int_R0S3_CID2=-99;
    Int_R1S0_CID2=-99;
    Int_R1S1_CID2=-99;
    Int_R1S2_CID2=-99;
    Int_R1S3_CID2=-99;
    Int_R2S0_CID2=-99;
    Int_R2S1_CID2=-99;
    Int_R2S2_CID2=-99;
    Int_R2S3_CID2=-99;
    Int_R3S0_CID2=-99;
    Int_R3S1_CID2=-99;
    Int_R3S2_CID2=-99;
    Int_R3S3_CID2=-99;
    Int_R0S0_CID3=-99;
    Int_R0S1_CID3=-99;
    Int_R0S2_CID3=-99;
    Int_R0S3_CID3=-99;
    Int_R1S0_CID3=-99;
    Int_R1S1_CID3=-99;
    Int_R1S2_CID3=-99;
    Int_R1S3_CID3=-99;
    Int_R2S0_CID3=-99;
    Int_R2S1_CID3=-99;
    Int_R2S2_CID3=-99;
    Int_R2S3_CID3=-99;
    Int_R3S0_CID3=-99;
    Int_R3S1_CID3=-99;
    Int_R3S2_CID3=-99;
    Int_R3S3_CID3=-99;
    OverlapR01_CID0=-99;
    OverlapR12_CID0=-99;
    OverlapR23_CID0=-99;
    OverlapR01_CID1=-99;
    OverlapR12_CID1=-99;
    OverlapR23_CID1=-99;
    OverlapR01_CID2=-99;
    OverlapR12_CID2=-99;
    OverlapR23_CID2=-99;
    OverlapR01_CID3=-99;
    OverlapR12_CID3=-99;
    OverlapR23_CID3=-99;
    Pedestal_CID0=-99;
    Pedestal_CID1=-99;
    Pedestal_CID2=-99;
    Pedestal_CID3=-99;
    PedestalRMS_CID0=-99;
    PedestalRMS_CID1=-99;
    PedestalRMS_CID2=-99;
    PedestalRMS_CID3=-99;
    PedestalDACSlope_Lo=-99;
    PedestalDACSlope_Hi=-99;
    CapIDPedDACSlope_CID0=-99;
    CapIDPedDACSlope_CID1=-99;
    CapIDPedDACSlope_CID2=-99;
    CapIDPedDACSlope_CID3=-99;
    TDCBinWidth=-99;
    Sum4Average=-99;

    for(int i=0; i< 255; i++) {
      if(i<62) ADCDNL[i]=-99 ;
      //if(i<255) {
        TDCVertOffset[i]=-99; // 255 of these
        Sum4DNL[i]=-99; //255 of these
	//}
    }

}


//int get_int(const string& s) {
//
//    stringstream ss(s);
//    int ret;
//    ss >> ret;
//
//    return ret;
//
//}
//
//double get_double(const string& s) {
//
//    stringstream ss(s);
//    double ret;
//    ss >> ret;
//
//    return ret;
//
//}

int makeUsInt(string input) {
  //cout << "in makeUSint" << endl;
  return stoi(input);
}

int makeSInt(string input) {

  //cout << "in makeSInt" << endl;

  stringstream stream;
  stream << setfill('0') << setw(4) << hex << stoi(input);

  if(stoi(stream.str(),0,16) < 32768) 
    return stoi(stream.str(),0,16);
  else
    return stoi(stream.str(),0,16) - 65536;

}



string intToHex(int i) {

  stringstream stream;
  stream << setfill('0') << setw(4) << hex << i;
  return stream.str();

}

int convertHexToInt(string myhex, bool sign) {
  if(sign) {
    if(stoi(myhex,0,16) < 32768) {
      //cout << stoi(myhex,0,16) << endl;
      return stoi(myhex,0,16);
    }
    else  return stoi(myhex,0,16)-65536;
  }
  else{
    //cout << stoi(myhex,0,16) - 65536 << endl;
    return stoi(myhex,0,16);
  } 
  
}

double getVoltOBD(int DAC) {
  return (double) DAC * 5./65536. + 2.5;
}

double getVoltInt(int DAC) {
  return (double) DAC*10./65536;
}

double LookupDAC(int DAC, int whichRes) {

  if(whichRes == 0) {
    if(DAC > 31810) return -0.3185386*DAC + 10195.6906;
    else if(DAC > 31410) return -0.3320204*DAC + 10623.6574;
    else if(DAC > 30185) return -0.3414152*DAC + 10916.9529;
    else if(DAC > 29100) return -0.3468817*DAC + 11065.0535;
    else if(DAC > 26200) return -0.3557116*DAC + 11323.9317;
    else return -0.3684184*DAC + 11641.4104;
  }
  else if(whichRes == 1) {
    if(DAC > 30635) return -8.2624156*DAC + 261175.435;
    else if(DAC > 29850) return -9.2708161*DAC +292051.72;
    else if(DAC > 27100) return -10.051575*DAC + 315065.982;
    else if(DAC > 24900) return -10.567903*DAC+329931.967;
    else return -10.879485*DAC + 337905.564;

  }
  else return 0;

}

void MakeTupleFromCutsMaker(const char* inFile, const char* outFile) {

  //cout << "DEBUG: Starting..." << endl;
  TFile hfile(outFile,"RECREATE");
  //cout << "DEBUG: Opened output file" << endl;

  BookTree();
  //cout << "DEBUG: Booked Tree" << endl;
  ResetVarMap();


  ifstream input(inFile);
  string line;
  
  //cout << "DEBUG: opened input file" << endl;

  int counter = 0;
  int varnum = 0;
  
  //cout << "DEBUG: beginning loop over input lines" << endl;
  while(getline(input, line)) {

    //cout << "DEBUG: Resetting variables" << endl;
    ResetVariables();
    //cout << "DEBUG: Variables reset" << endl;
    //ResetVarMap();

    stringstream ss(line);
    string field;

    counter++;
    if(counter <2) continue;
    if(counter == 2) {

      //int varnum = 0;
      
      while(getline(ss, field, ',')) {
	varnum++;
	if(varnum==1) continue;
	vars[field]=varnum+1;
	
      }
      cout << "varNum: " << varnum << endl;
      //cout << "got Vars set" << endl;
      continue;
    }

    //stringstream ss(line);
    vector<string> v;
    v.clear();
    //string field;
    //cout << "DEBUG Starting loop over CSV entries" << endl;
    v.push_back("65536");
    while(getline(ss, field, ',')) {
      v.push_back(field);
      
    }
    
    //if(v.size() < 1014) {
    if((int)v.size() < (varnum+1)) {
      //tree->Fill();
      continue;
    }
    
    //cout << "DEBUG: Parsed input" << endl;
    if(v[0] == "Pass") ChipStatus = 1;
    else ChipStatus = 0;
    
    Current33_NoC = getVoltOBD(makeSInt(v[vars["1_1"]]))/10;
    Current50_NoC = getVoltOBD(makeSInt(v[vars["2_1"]]))/10; 
    Current33_C  = getVoltOBD(makeSInt(v[vars["3_1"]]))/10;
    Current50_C  = getVoltOBD(makeSInt(v[vars["4_1"]]))/10;
    
    Volt_Isetp = getVoltOBD(makeSInt(v[vars["5_1"]]));
    Volt_33V = getVoltOBD(makeSInt(v[vars["6_1"]]));
    Volt_50V = getVoltOBD(makeSInt(v[vars["7_1"]]));
    Volt_Clamp = getVoltOBD(makeSInt(v[vars["8_1"]]));
    Volt_ClampCalMode = getVoltOBD(makeSInt(v[vars["9_1"]]));
    Volt_Rref = getVoltOBD(makeSInt(v[vars["10_1"]]));
    Volt_IDCSet = getVoltOBD(makeSInt(v[vars["11_1"]]));
    Volt_InSig = getVoltOBD(makeSInt(v[vars["12_1"]]));
    Volt_InRef = getVoltOBD(makeSInt(v[vars["13_1"]]));
    
    IDCset00 = getVoltOBD(makeSInt(v[vars["18_1"]]));
    IDCset01 = getVoltOBD(makeSInt(v[vars["19_1"]]));
    IDCset02 = getVoltOBD(makeSInt(v[vars["20_1"]]));
    IDCset03 = getVoltOBD(makeSInt(v[vars["21_1"]]));
    IDCset04 = getVoltOBD(makeSInt(v[vars["22_1"]]));
    IDCset05 = getVoltOBD(makeSInt(v[vars["23_1"]]));
    IDCset06 = getVoltOBD(makeSInt(v[vars["24_1"]]));
    IDCset07 = getVoltOBD(makeSInt(v[vars["25_1"]]));
    IDCset08 = getVoltOBD(makeSInt(v[vars["26_1"]]));
    IDCset09 = getVoltOBD(makeSInt(v[vars["27_1"]]));
    IDCset0a = getVoltOBD(makeSInt(v[vars["28_1"]]));
    IDCset0b = getVoltOBD(makeSInt(v[vars["29_1"]]));
    IDCset0c = getVoltOBD(makeSInt(v[vars["30_1"]]));
    IDCset0d = getVoltOBD(makeSInt(v[vars["31_1"]]));
    IDCset0e = getVoltOBD(makeSInt(v[vars["32_1"]]));
    IDCset0f = getVoltOBD(makeSInt(v[vars["33_1"]]));
    IDCset10 = getVoltOBD(makeSInt(v[vars["34_1"]]));
    IDCset11 = getVoltOBD(makeSInt(v[vars["35_1"]]));
    IDCset12 = getVoltOBD(makeSInt(v[vars["36_1"]]));
    IDCset13 = getVoltOBD(makeSInt(v[vars["37_1"]]));
    IDCset14 = getVoltOBD(makeSInt(v[vars["38_1"]]));
    IDCset15 = getVoltOBD(makeSInt(v[vars["39_1"]]));
    IDCset16 = getVoltOBD(makeSInt(v[vars["40_1"]]));
    IDCset17 = getVoltOBD(makeSInt(v[vars["41_1"]]));
    IDCset18 = getVoltOBD(makeSInt(v[vars["42_1"]]));
    IDCset19 = getVoltOBD(makeSInt(v[vars["43_1"]]));
    IDCset1a = getVoltOBD(makeSInt(v[vars["44_1"]]));
    IDCset1b = getVoltOBD(makeSInt(v[vars["45_1"]]));
    IDCset1c = getVoltOBD(makeSInt(v[vars["46_1"]]));
    IDCset1d = getVoltOBD(makeSInt(v[vars["47_1"]]));
    IDCset1e = getVoltOBD(makeSInt(v[vars["48_1"]]));
    IDCset1f = getVoltOBD(makeSInt(v[vars["49_1"]]));
    
    LVDS_AvgMid_1 = getVoltOBD(makeSInt(v[vars["50_1"]]));
    LVDS_AvgMid_2 = getVoltOBD(makeSInt(v[vars["50_2"]]));
    LVDS_AvgMid_3 = getVoltOBD(makeSInt(v[vars["50_3"]]));
    LVDS_AvgMid_4 = getVoltOBD(makeSInt(v[vars["50_4"]]));
    //LVDS_AvgMid_5 = getVoltOBD(makeSInt(v[55]));
    //LVDS_AvgMid_6 = getVoltOBD(makeSInt(v[56]));
    
    SumRange0 = makeUsInt(v[vars["51_1"]]); //Sum in fC
    SumRange1 = makeUsInt(v[vars["52_1"]]);
    SumRange2 = makeUsInt(v[vars["53_1"]]);
    SumRange3 = makeUsInt(v[vars["54_1"]]);
    
    //Cuts 55-62
    SumDAC0 = makeUsInt(v[vars["55_1"]]); //Sum in fC
    SumDAC1 = makeUsInt(v[vars["56_1"]]);
    SumDAC2 = makeUsInt(v[vars["57_1"]]);
    SumDAC3 = makeUsInt(v[vars["58_1"]]);
    SumDAC4 = makeUsInt(v[vars["59_1"]]);
    SumDAC5 = makeUsInt(v[vars["60_1"]]);
    SumDAC6 = makeUsInt(v[vars["61_1"]]);
    SumDAC7 = makeUsInt(v[vars["62_1"]]);
    
    //Cuts 63-71
    ThreshDAC0  = LookupDAC(makeUsInt(v[vars["63_1"]]),0)/25; //DAC value to fire thresh
    ThreshDAC1  = LookupDAC(makeUsInt(v[vars["64_1"]]),0)/25;
    ThreshDAC2  = LookupDAC(makeUsInt(v[vars["65_1"]]),0)/25;
    ThreshDAC4  = LookupDAC(makeUsInt(v[vars["66_1"]]),0)/25;
    ThreshDAC8  = LookupDAC(makeUsInt(v[vars["67_1"]]),0)/25;
    ThreshDAC10 = LookupDAC(makeUsInt(v[vars["68_1"]]),0)/25;
    ThreshDAC20 = LookupDAC(makeUsInt(v[vars["69_1"]]),0)/25;
    ThreshDAC40 = LookupDAC(makeUsInt(v[vars["70_1"]]),1)/25;
    ThreshDAC80 = LookupDAC(makeUsInt(v[vars["71_1"]]),1)/25;
    
    //Cuts 72-73
    LockTime120Mhz = makeUsInt(v[vars["72_1"]])*100; //ns
    LockTime1Mhz = makeUsInt(v[vars["73_1"]])*100; //ns
    
    //Cut 74
    InSigV = getVoltOBD(makeSInt(v[vars["74_1"]]));
    
    //Cut75 + 76
    InRefV = getVoltOBD(makeSInt(v[vars["75_1"]]));
    DiffInSigInRef = getVoltOBD(makeSInt(v[vars["76_1"]])) - 2.5;
    
    //Cuts 77-84
    //TimingInRefDAC0 = getVoltOBD(makeSInt(v[vars["77_1"]]));
    //TimingInRefDAC1 = getVoltOBD(makeSInt(v[vars["78_1"]]));
    //TimingInRefDAC2 = getVoltOBD(makeSInt(v[vars["79_1"]]));
    //TimingInRefDAC3 = getVoltOBD(makeSInt(v[vars["80_1"]]));
    //TimingInRefDAC4 = getVoltOBD(makeSInt(v[vars["81_1"]]));
    //TimingInRefDAC5 = getVoltOBD(makeSInt(v[vars["82_1"]]));
    //TimingInRefDAC6 = getVoltOBD(makeSInt(v[vars["83_1"]]));
    //TimingInRefDAC7 = getVoltOBD(makeSInt(v[vars["84_1"]]));    
    TimingInRefDAC0 = LookupDAC(makeUsInt(v[vars["77_1"]]),0)/25;
    TimingInRefDAC1 = LookupDAC(makeUsInt(v[vars["78_1"]]),0)/25;
    TimingInRefDAC2 = LookupDAC(makeUsInt(v[vars["79_1"]]),0)/25;
    TimingInRefDAC3 = LookupDAC(makeUsInt(v[vars["80_1"]]),0)/25;
    TimingInRefDAC4 = LookupDAC(makeUsInt(v[vars["81_1"]]),0)/25;
    TimingInRefDAC5 = LookupDAC(makeUsInt(v[vars["82_1"]]),0)/25;
    TimingInRefDAC6 = LookupDAC(makeUsInt(v[vars["83_1"]]),0)/25;
    TimingInRefDAC7 = LookupDAC(makeUsInt(v[vars["84_1"]]),0)/25;

    //All the LVDS pins here
    LVDSTrim0Hi_pin01 = getVoltOBD(makeSInt(v[vars["86_1"]]));
    LVDSTrim0Hi_pin02 = getVoltOBD(makeSInt(v[vars["86_2"]]));
    LVDSTrim0Hi_pin03 = getVoltOBD(makeSInt(v[vars["86_3"]]));
    LVDSTrim0Hi_pin04 = getVoltOBD(makeSInt(v[vars["86_4"]]));
    LVDSTrim0Hi_pin05 = getVoltOBD(makeSInt(v[vars["86_5"]]));
    LVDSTrim0Hi_pin06 = getVoltOBD(makeSInt(v[vars["86_6"]]));
    LVDSTrim0Hi_pin07 = getVoltOBD(makeSInt(v[vars["86_7"]]));
    LVDSTrim0Hi_pin08 = getVoltOBD(makeSInt(v[vars["86_8"]]));
    LVDSTrim0Hi_pin09 = getVoltOBD(makeSInt(v[vars["86_9"]]));
    LVDSTrim0Hi_pin10 = getVoltOBD(makeSInt(v[vars["86_10"]]));
    LVDSTrim0Hi_pin11 = getVoltOBD(makeSInt(v[vars["86_11"]]));
    LVDSTrim0Hi_pin12 = getVoltOBD(makeSInt(v[vars["86_12"]]));
    LVDSTrim0Hi_pin13 = getVoltOBD(makeSInt(v[vars["86_13"]]));
    LVDSTrim0Hi_pin14 = getVoltOBD(makeSInt(v[vars["86_14"]]));
    LVDSTrim0Hi_pin15 = getVoltOBD(makeSInt(v[vars["86_15"]]));
    LVDSTrim0Hi_pin16 = getVoltOBD(makeSInt(v[vars["86_16"]]));
    LVDSTrim0Hi_pin17 = getVoltOBD(makeSInt(v[vars["86_17"]]));
    LVDSTrim0Hi_pin18 = getVoltOBD(makeSInt(v[vars["86_18"]]));
    //LVDSTrim0Hi_pin19 = getVoltOBD(makeSInt(v[90]));
    //LVDSTrim0Hi_pin20 = getVoltOBD(makeSInt(v[90]));
    LVDSTrim0Lo_pin01 = getVoltOBD(makeSInt(v[vars["87_1"]]));
    LVDSTrim0Lo_pin02 = getVoltOBD(makeSInt(v[vars["87_2"]]));
    LVDSTrim0Lo_pin03 = getVoltOBD(makeSInt(v[vars["87_3"]]));
    LVDSTrim0Lo_pin04 = getVoltOBD(makeSInt(v[vars["87_4"]]));
    LVDSTrim0Lo_pin05 = getVoltOBD(makeSInt(v[vars["87_5"]]));
    LVDSTrim0Lo_pin06 = getVoltOBD(makeSInt(v[vars["87_6"]]));
    LVDSTrim0Lo_pin07 = getVoltOBD(makeSInt(v[vars["87_7"]]));
    LVDSTrim0Lo_pin08 = getVoltOBD(makeSInt(v[vars["87_8"]]));
    LVDSTrim0Lo_pin09 = getVoltOBD(makeSInt(v[vars["87_9"]]));
    LVDSTrim0Lo_pin10 = getVoltOBD(makeSInt(v[vars["87_10"]]));
    LVDSTrim0Lo_pin11 = getVoltOBD(makeSInt(v[vars["87_11"]]));
    LVDSTrim0Lo_pin12 = getVoltOBD(makeSInt(v[vars["87_12"]]));
    LVDSTrim0Lo_pin13 = getVoltOBD(makeSInt(v[vars["87_13"]]));
    LVDSTrim0Lo_pin14 = getVoltOBD(makeSInt(v[vars["87_14"]]));
    LVDSTrim0Lo_pin15 = getVoltOBD(makeSInt(v[vars["87_15"]]));
    LVDSTrim0Lo_pin16 = getVoltOBD(makeSInt(v[vars["87_16"]]));
    LVDSTrim0Lo_pin17 = getVoltOBD(makeSInt(v[vars["87_17"]]));
    LVDSTrim0Lo_pin18 = getVoltOBD(makeSInt(v[vars["87_18"]]));
    //LVDSTrim0Lo_pin19 = getVoltOBD(makeSInt(v[10]));
    //LVDSTrim0Lo_pin20 = getVoltOBD(makeSInt(v[10]));
    
    LVDSTrim1Hi_pin01 = getVoltOBD(makeSInt(v[vars["88_1"]]));
    LVDSTrim1Hi_pin02 = getVoltOBD(makeSInt(v[vars["88_2"]]));
    LVDSTrim1Hi_pin03 = getVoltOBD(makeSInt(v[vars["88_3"]]));
    LVDSTrim1Hi_pin04 = getVoltOBD(makeSInt(v[vars["88_4"]]));
    LVDSTrim1Hi_pin05 = getVoltOBD(makeSInt(v[vars["88_5"]]));
    LVDSTrim1Hi_pin06 = getVoltOBD(makeSInt(v[vars["88_6"]]));
    LVDSTrim1Hi_pin07 = getVoltOBD(makeSInt(v[vars["88_7"]]));
    LVDSTrim1Hi_pin08 = getVoltOBD(makeSInt(v[vars["88_8"]]));
    LVDSTrim1Hi_pin09 = getVoltOBD(makeSInt(v[vars["88_9"]]));
    LVDSTrim1Hi_pin10 = getVoltOBD(makeSInt(v[vars["88_10"]]));
    LVDSTrim1Hi_pin11 = getVoltOBD(makeSInt(v[vars["88_11"]]));
    LVDSTrim1Hi_pin12 = getVoltOBD(makeSInt(v[vars["88_12"]]));
    LVDSTrim1Hi_pin13 = getVoltOBD(makeSInt(v[vars["88_13"]]));
    LVDSTrim1Hi_pin14 = getVoltOBD(makeSInt(v[vars["88_14"]]));
    LVDSTrim1Hi_pin15 = getVoltOBD(makeSInt(v[vars["88_15"]]));
    LVDSTrim1Hi_pin16 = getVoltOBD(makeSInt(v[vars["88_16"]]));
    LVDSTrim1Hi_pin17 = getVoltOBD(makeSInt(v[vars["88_17"]]));
    LVDSTrim1Hi_pin18 = getVoltOBD(makeSInt(v[vars["88_18"]]));
    //LVDSTrim1Hi_pin19 = getVoltOBD(makeSInt(v[90]));
    //LVDSTrim1Hi_pin20 = getVoltOBD(makeSInt(v[90]));
    LVDSTrim1Lo_pin01 = getVoltOBD(makeSInt(v[vars["89_1"]]));
    LVDSTrim1Lo_pin02 = getVoltOBD(makeSInt(v[vars["89_2"]]));
    LVDSTrim1Lo_pin03 = getVoltOBD(makeSInt(v[vars["89_3"]]));
    LVDSTrim1Lo_pin04 = getVoltOBD(makeSInt(v[vars["89_4"]]));
    LVDSTrim1Lo_pin05 = getVoltOBD(makeSInt(v[vars["89_5"]]));
    LVDSTrim1Lo_pin06 = getVoltOBD(makeSInt(v[vars["89_6"]]));
    LVDSTrim1Lo_pin07 = getVoltOBD(makeSInt(v[vars["89_7"]]));
    LVDSTrim1Lo_pin08 = getVoltOBD(makeSInt(v[vars["89_8"]]));
    LVDSTrim1Lo_pin09 = getVoltOBD(makeSInt(v[vars["89_9"]]));
    LVDSTrim1Lo_pin10 = getVoltOBD(makeSInt(v[vars["89_10"]]));
    LVDSTrim1Lo_pin11 = getVoltOBD(makeSInt(v[vars["89_11"]]));
    LVDSTrim1Lo_pin12 = getVoltOBD(makeSInt(v[vars["89_12"]]));
    LVDSTrim1Lo_pin13 = getVoltOBD(makeSInt(v[vars["89_13"]]));
    LVDSTrim1Lo_pin14 = getVoltOBD(makeSInt(v[vars["89_14"]]));
    LVDSTrim1Lo_pin15 = getVoltOBD(makeSInt(v[vars["89_15"]]));
    LVDSTrim1Lo_pin16 = getVoltOBD(makeSInt(v[vars["89_16"]]));
    LVDSTrim1Lo_pin17 = getVoltOBD(makeSInt(v[vars["89_17"]]));
    LVDSTrim1Lo_pin18 = getVoltOBD(makeSInt(v[vars["89_18"]]));
    //LVDSTrim0Lo_pin19 = getVoltOBD(makeSInt(v[10]));
    //LVDSTrim0Lo_pin20 = getVoltOBD(makeSInt(v[10]));

    LVDSTrim2Hi_pin01 = getVoltOBD(makeSInt(v[vars["90_1"]]));
    LVDSTrim2Hi_pin02 = getVoltOBD(makeSInt(v[vars["90_2"]]));
    LVDSTrim2Hi_pin03 = getVoltOBD(makeSInt(v[vars["90_3"]]));
    LVDSTrim2Hi_pin04 = getVoltOBD(makeSInt(v[vars["90_4"]]));
    LVDSTrim2Hi_pin05 = getVoltOBD(makeSInt(v[vars["90_5"]]));
    LVDSTrim2Hi_pin06 = getVoltOBD(makeSInt(v[vars["90_6"]]));
    LVDSTrim2Hi_pin07 = getVoltOBD(makeSInt(v[vars["90_7"]]));
    LVDSTrim2Hi_pin08 = getVoltOBD(makeSInt(v[vars["90_8"]]));
    LVDSTrim2Hi_pin09 = getVoltOBD(makeSInt(v[vars["90_9"]]));
    LVDSTrim2Hi_pin10 = getVoltOBD(makeSInt(v[vars["90_10"]]));
    LVDSTrim2Hi_pin11 = getVoltOBD(makeSInt(v[vars["90_11"]]));
    LVDSTrim2Hi_pin12 = getVoltOBD(makeSInt(v[vars["90_12"]]));
    LVDSTrim2Hi_pin13 = getVoltOBD(makeSInt(v[vars["90_13"]]));
    LVDSTrim2Hi_pin14 = getVoltOBD(makeSInt(v[vars["90_14"]]));
    LVDSTrim2Hi_pin15 = getVoltOBD(makeSInt(v[vars["90_15"]]));
    LVDSTrim2Hi_pin16 = getVoltOBD(makeSInt(v[vars["90_16"]]));
    LVDSTrim2Hi_pin17 = getVoltOBD(makeSInt(v[vars["90_17"]]));
    LVDSTrim2Hi_pin18 = getVoltOBD(makeSInt(v[vars["90_18"]]));
    //LVDSTrim2Hi_pin19 = getVoltOBD(makeSInt(v[90]));
    //LVDSTrim2Hi_pin20 = getVoltOBD(makeSInt(v[90]));
    LVDSTrim2Lo_pin01 = getVoltOBD(makeSInt(v[vars["91_1"]]));
    LVDSTrim2Lo_pin02 = getVoltOBD(makeSInt(v[vars["91_2"]]));
    LVDSTrim2Lo_pin03 = getVoltOBD(makeSInt(v[vars["91_3"]]));
    LVDSTrim2Lo_pin04 = getVoltOBD(makeSInt(v[vars["91_4"]]));
    LVDSTrim2Lo_pin05 = getVoltOBD(makeSInt(v[vars["91_5"]]));
    LVDSTrim2Lo_pin06 = getVoltOBD(makeSInt(v[vars["91_6"]]));
    LVDSTrim2Lo_pin07 = getVoltOBD(makeSInt(v[vars["91_7"]]));
    LVDSTrim2Lo_pin08 = getVoltOBD(makeSInt(v[vars["91_8"]]));
    LVDSTrim2Lo_pin09 = getVoltOBD(makeSInt(v[vars["91_9"]]));
    LVDSTrim2Lo_pin10 = getVoltOBD(makeSInt(v[vars["91_10"]]));
    LVDSTrim2Lo_pin11 = getVoltOBD(makeSInt(v[vars["91_11"]]));
    LVDSTrim2Lo_pin12 = getVoltOBD(makeSInt(v[vars["91_12"]]));
    LVDSTrim2Lo_pin13 = getVoltOBD(makeSInt(v[vars["91_13"]]));
    LVDSTrim2Lo_pin14 = getVoltOBD(makeSInt(v[vars["91_14"]]));
    LVDSTrim2Lo_pin15 = getVoltOBD(makeSInt(v[vars["91_15"]]));
    LVDSTrim2Lo_pin16 = getVoltOBD(makeSInt(v[vars["91_16"]]));
    LVDSTrim2Lo_pin17 = getVoltOBD(makeSInt(v[vars["91_17"]]));
    LVDSTrim2Lo_pin18 = getVoltOBD(makeSInt(v[vars["91_18"]]));
    //LVDSTrim0Lo_pin19 = getVoltOBD(makeSInt(v[10]));
    //LVDSTrim0Lo_pin20 = getVoltOBD(makeSInt(v[10]));

    LVDSTrim3Hi_pin01 = getVoltOBD(makeSInt(v[vars["92_1"]]));
    LVDSTrim3Hi_pin02 = getVoltOBD(makeSInt(v[vars["92_2"]]));
    LVDSTrim3Hi_pin03 = getVoltOBD(makeSInt(v[vars["92_3"]]));
    LVDSTrim3Hi_pin04 = getVoltOBD(makeSInt(v[vars["92_4"]]));
    LVDSTrim3Hi_pin05 = getVoltOBD(makeSInt(v[vars["92_5"]]));
    LVDSTrim3Hi_pin06 = getVoltOBD(makeSInt(v[vars["92_6"]]));
    LVDSTrim3Hi_pin07 = getVoltOBD(makeSInt(v[vars["92_7"]]));
    LVDSTrim3Hi_pin08 = getVoltOBD(makeSInt(v[vars["92_8"]]));
    LVDSTrim3Hi_pin09 = getVoltOBD(makeSInt(v[vars["92_9"]]));
    LVDSTrim3Hi_pin10 = getVoltOBD(makeSInt(v[vars["92_10"]]));
    LVDSTrim3Hi_pin11 = getVoltOBD(makeSInt(v[vars["92_11"]]));
    LVDSTrim3Hi_pin12 = getVoltOBD(makeSInt(v[vars["92_12"]]));
    LVDSTrim3Hi_pin13 = getVoltOBD(makeSInt(v[vars["92_13"]]));
    LVDSTrim3Hi_pin14 = getVoltOBD(makeSInt(v[vars["92_14"]]));
    LVDSTrim3Hi_pin15 = getVoltOBD(makeSInt(v[vars["92_15"]]));
    LVDSTrim3Hi_pin16 = getVoltOBD(makeSInt(v[vars["92_16"]]));
    LVDSTrim3Hi_pin17 = getVoltOBD(makeSInt(v[vars["92_17"]]));
    LVDSTrim3Hi_pin18 = getVoltOBD(makeSInt(v[vars["92_18"]]));
    //LVDSTrim3Hi_pin19 = getVoltOBD(makeSInt(v[90]));
    //LVDSTrim3Hi_pin20 = getVoltOBD(makeSInt(v[90]));
    LVDSTrim3Lo_pin01 = getVoltOBD(makeSInt(v[vars["93_1"]]));
    LVDSTrim3Lo_pin02 = getVoltOBD(makeSInt(v[vars["93_2"]]));
    LVDSTrim3Lo_pin03 = getVoltOBD(makeSInt(v[vars["93_3"]]));
    LVDSTrim3Lo_pin04 = getVoltOBD(makeSInt(v[vars["93_4"]]));
    LVDSTrim3Lo_pin05 = getVoltOBD(makeSInt(v[vars["93_5"]]));
    LVDSTrim3Lo_pin06 = getVoltOBD(makeSInt(v[vars["93_6"]]));
    LVDSTrim3Lo_pin07 = getVoltOBD(makeSInt(v[vars["93_7"]]));
    LVDSTrim3Lo_pin08 = getVoltOBD(makeSInt(v[vars["93_8"]]));
    LVDSTrim3Lo_pin09 = getVoltOBD(makeSInt(v[vars["93_9"]]));
    LVDSTrim3Lo_pin10 = getVoltOBD(makeSInt(v[vars["93_10"]]));
    LVDSTrim3Lo_pin11 = getVoltOBD(makeSInt(v[vars["93_11"]]));
    LVDSTrim3Lo_pin12 = getVoltOBD(makeSInt(v[vars["93_12"]]));
    LVDSTrim3Lo_pin13 = getVoltOBD(makeSInt(v[vars["93_13"]]));
    LVDSTrim3Lo_pin14 = getVoltOBD(makeSInt(v[vars["93_14"]]));
    LVDSTrim3Lo_pin15 = getVoltOBD(makeSInt(v[vars["93_15"]]));
    LVDSTrim3Lo_pin16 = getVoltOBD(makeSInt(v[vars["93_16"]]));
    LVDSTrim3Lo_pin17 = getVoltOBD(makeSInt(v[vars["93_17"]]));
    LVDSTrim3Lo_pin18 = getVoltOBD(makeSInt(v[vars["93_18"]]));
    //LVDSTrim0Lo_pin19 = getVoltOBD(makeSInt(v[10]));
    //LVDSTrim0Lo_pin20 = getVoltOBD(makeSInt(v[10]));

    //Cuts 94-103
    ImpDiff_RinSel0 = (getVoltOBD(makeSInt(v[vars["94_1"]])) - 2.5)/0.000181654;
    InSigMClampDiff_RinSel0 = getVoltOBD(makeSInt(v[vars["95_1"]])) - 2.5;
    ImpDiff_RinSel1 = (getVoltOBD(makeSInt(v[vars["96_1"]])) - 2.5)/0.000181654;
    InSigMClampDiff_RinSel1 = getVoltOBD(makeSInt(v[vars["97_1"]])) - 2.5;
    ImpDiff_RinSel2 = (getVoltOBD(makeSInt(v[vars["98_1"]])) - 2.5)/0.000181654;
    InSigMClampDiff_RinSel2 = getVoltOBD(makeSInt(v[vars["99_1"]])) - 2.5;
    ImpDiff_RinSel4 = (getVoltOBD(makeSInt(v[vars["100_1"]])) - 2.5)/0.000181654;
    InSigMClampDiff_RinSel4 = getVoltOBD(makeSInt(v[vars["101_1"]])) - 2.5;
    ImpDiff_RinSel8 = (getVoltOBD(makeSInt(v[vars["102_1"]])) - 2.5)/0.000181654;
    InSigMClampDiff_RinSel8 = getVoltOBD(makeSInt(v[vars["103_1"]])) - 2.5;
    ImpDiff_RinSelF = (getVoltOBD(makeSInt(v[vars["104_1"]])) - 2.5)/0.000181654;
		       
    WRPmax = makeUsInt(v[vars["106_1"]])/10000.;
    WRPRange01 = makeUsInt(v[vars["107_1"]])/10000.;
    WRPRange12 = makeUsInt(v[vars["108_1"]])/10000.;
    WRPRange23 = makeUsInt(v[vars["109_1"]])/10000.;

  //Cuts 110-137
    lADC00_Dat0      = getVoltOBD(makeSInt(v[vars["110_1"]]));
    lADC01_Dat3      = getVoltOBD(makeSInt(v[vars["111_1"]]));
    lADC02_Dat6      = getVoltOBD(makeSInt(v[vars["112_1"]]));
    lADC03_Disc      = getVoltOBD(makeSInt(v[vars["113_1"]]));
    lADC10_Dat0Bar   = getVoltOBD(makeSInt(v[vars["114_1"]]));
    lADC11_Dat3Bar   = getVoltOBD(makeSInt(v[vars["115_1"]]));
    lADC12_Dat6Bar   = getVoltOBD(makeSInt(v[vars["116_1"]]));
    lADC13_DiscBar   = getVoltOBD(makeSInt(v[vars["117_1"]]));
    lADC20_Dat1      = getVoltOBD(makeSInt(v[vars["118_1"]]));
    lADC21_Dat4      = getVoltOBD(makeSInt(v[vars["119_1"]]));
    lADC22_Dat7      = getVoltOBD(makeSInt(v[vars["120_1"]]));
    lADC23_Rref      = getVoltOBD(makeSInt(v[vars["121_1"]]));
    lADC30_Dat1Bar   = getVoltOBD(makeSInt(v[vars["122_1"]]));
    lADC31_Dat4Bar   = getVoltOBD(makeSInt(v[vars["123_1"]]));
    lADC32_Dat7Bar   = getVoltOBD(makeSInt(v[vars["124_1"]]));
    lADC33_IDCset    = getVoltOBD(makeSInt(v[vars["125_1"]]));
    lADC40_Dat2      = getVoltOBD(makeSInt(v[vars["126_1"]]));
    lADC41_Dat5      = getVoltOBD(makeSInt(v[vars["127_1"]]));
    lADC42_ClkOut    = getVoltOBD(makeSInt(v[vars["128_1"]]));
    lADC43_InSig     = getVoltOBD(makeSInt(v[vars["129_1"]]));
    lADC50_Dat2Bar   = getVoltOBD(makeSInt(v[vars["130_1"]]));
    lADC51_Dat5Bar   = getVoltOBD(makeSInt(v[vars["131_1"]]));
    lADC52_ClkOutBar = getVoltOBD(makeSInt(v[vars["132_1"]]));
    lADC53_InRef     = getVoltOBD(makeSInt(v[vars["133_1"]]));
    lADC60_IsetP     = getVoltOBD(makeSInt(v[vars["134_1"]]));
    lADC61_33V       = getVoltOBD(makeSInt(v[vars["135_1"]]));
    lADC62_50V       = getVoltOBD(makeSInt(v[vars["136_1"]]));
    lADC63_Clamp     = getVoltOBD(makeSInt(v[vars["137_1"]]));

    gsel00 = makeUsInt(v[vars["138_1"]])/16;
    gsel01 = makeUsInt(v[vars["139_1"]])/16;
    gsel02 = makeUsInt(v[vars["140_1"]])/16;
    gsel04 = makeUsInt(v[vars["141_1"]])/16;
    gsel08 = makeUsInt(v[vars["142_1"]])/16;
    gsel16 = makeUsInt(v[vars["143_1"]])/16;
    

  //Cuts 150-165 //1perCAPID;
    Slope_R0S0_CID0 = makeUsInt(v[vars["150_1"]])/100.;
    Slope_R0S1_CID0 = makeUsInt(v[vars["151_1"]])/100.;
    Slope_R0S2_CID0 = makeUsInt(v[vars["152_1"]])/100.;
    Slope_R0S3_CID0 = makeUsInt(v[vars["153_1"]])/100.;
    Slope_R1S0_CID0 = makeUsInt(v[vars["154_1"]])/100.;
    Slope_R1S1_CID0 = makeUsInt(v[vars["155_1"]])/100.;
    Slope_R1S2_CID0 = makeUsInt(v[vars["156_1"]])/100.;
    Slope_R1S3_CID0 = makeUsInt(v[vars["157_1"]])/100.;
    Slope_R2S0_CID0 = makeUsInt(v[vars["158_1"]])/10.;
    Slope_R2S1_CID0 = makeUsInt(v[vars["159_1"]])/10.;
    Slope_R2S2_CID0 = makeUsInt(v[vars["160_1"]])/10.;
    Slope_R2S3_CID0 = makeUsInt(v[vars["161_1"]])/10.;
    Slope_R3S0_CID0 = makeUsInt(v[vars["162_1"]])/10.;
    Slope_R3S1_CID0 = makeUsInt(v[vars["163_1"]])/10.;
    Slope_R3S2_CID0 = makeUsInt(v[vars["164_1"]]);
    Slope_R3S3_CID0 = makeUsInt(v[vars["165_1"]]);

    Slope_R0S0_CID1 = makeUsInt(v[vars["150_2"]])/100.;
    Slope_R0S1_CID1 = makeUsInt(v[vars["151_2"]])/100.;
    Slope_R0S2_CID1 = makeUsInt(v[vars["152_2"]])/100.;
    Slope_R0S3_CID1 = makeUsInt(v[vars["153_2"]])/100.;
    Slope_R1S0_CID1 = makeUsInt(v[vars["154_2"]])/100.;
    Slope_R1S1_CID1 = makeUsInt(v[vars["155_2"]])/100.;
    Slope_R1S2_CID1 = makeUsInt(v[vars["156_2"]])/100.;
    Slope_R1S3_CID1 = makeUsInt(v[vars["157_2"]])/100.;
    Slope_R2S0_CID1 = makeUsInt(v[vars["158_2"]])/10.;
    Slope_R2S1_CID1 = makeUsInt(v[vars["159_2"]])/10.;
    Slope_R2S2_CID1 = makeUsInt(v[vars["160_2"]])/10.;
    Slope_R2S3_CID1 = makeUsInt(v[vars["161_2"]])/10.;
    Slope_R3S0_CID1 = makeUsInt(v[vars["162_2"]])/10.;
    Slope_R3S1_CID1 = makeUsInt(v[vars["163_2"]])/10.;
    Slope_R3S2_CID1 = makeUsInt(v[vars["164_2"]]);
    Slope_R3S3_CID1 = makeUsInt(v[vars["165_2"]]);

    Slope_R0S0_CID2 = makeUsInt(v[vars["150_3"]])/100.;
    Slope_R0S1_CID2 = makeUsInt(v[vars["151_3"]])/100.;
    Slope_R0S2_CID2 = makeUsInt(v[vars["152_3"]])/100.;
    Slope_R0S3_CID2 = makeUsInt(v[vars["153_3"]])/100.;
    Slope_R1S0_CID2 = makeUsInt(v[vars["154_3"]])/100.;
    Slope_R1S1_CID2 = makeUsInt(v[vars["155_3"]])/100.;
    Slope_R1S2_CID2 = makeUsInt(v[vars["156_3"]])/100.;
    Slope_R1S3_CID2 = makeUsInt(v[vars["157_3"]])/100.;
    Slope_R2S0_CID2 = makeUsInt(v[vars["158_3"]])/10.;
    Slope_R2S1_CID2 = makeUsInt(v[vars["159_3"]])/10.;
    Slope_R2S2_CID2 = makeUsInt(v[vars["160_3"]])/10.;
    Slope_R2S3_CID2 = makeUsInt(v[vars["161_3"]])/10.;
    Slope_R3S0_CID2 = makeUsInt(v[vars["162_3"]])/10.;
    Slope_R3S1_CID2 = makeUsInt(v[vars["163_3"]])/10.;
    Slope_R3S2_CID2 = makeUsInt(v[vars["164_3"]]);
    Slope_R3S3_CID2 = makeUsInt(v[vars["165_3"]]);

    Slope_R0S0_CID3 = makeUsInt(v[vars["150_4"]])/100.;
    Slope_R0S1_CID3 = makeUsInt(v[vars["151_4"]])/100.;
    Slope_R0S2_CID3 = makeUsInt(v[vars["152_4"]])/100.;
    Slope_R0S3_CID3 = makeUsInt(v[vars["153_4"]])/100.;
    Slope_R1S0_CID3 = makeUsInt(v[vars["154_4"]])/100.;
    Slope_R1S1_CID3 = makeUsInt(v[vars["155_4"]])/100.;
    Slope_R1S2_CID3 = makeUsInt(v[vars["156_4"]])/100.;
    Slope_R1S3_CID3 = makeUsInt(v[vars["157_4"]])/100.;
    Slope_R2S0_CID3 = makeUsInt(v[vars["158_4"]])/10.;
    Slope_R2S1_CID3 = makeUsInt(v[vars["159_4"]])/10.;
    Slope_R2S2_CID3 = makeUsInt(v[vars["160_4"]])/10.;
    Slope_R2S3_CID3 = makeUsInt(v[vars["161_4"]])/10.;
    Slope_R3S0_CID3 = makeUsInt(v[vars["162_4"]])/10.;
    Slope_R3S1_CID3 = makeUsInt(v[vars["163_4"]])/10.;
    Slope_R3S2_CID3 = makeUsInt(v[vars["164_4"]]);
    Slope_R3S3_CID3 = makeUsInt(v[vars["165_4"]]);


   //Cuts 166-181; //1 perCAPID
    Int_R0S0_CID0 = makeUsInt(v[vars["166_1"]])/100.;
    Int_R0S1_CID0 = makeUsInt(v[vars["167_1"]])/100.;
    Int_R0S2_CID0 = makeUsInt(v[vars["168_1"]])/10.;
    Int_R0S3_CID0 = makeUsInt(v[vars["169_1"]])/10.;
    Int_R1S0_CID0 = makeUsInt(v[vars["170_1"]])/10.;
    Int_R1S1_CID0 = makeUsInt(v[vars["171_1"]])/10.;
    Int_R1S2_CID0 = makeUsInt(v[vars["172_1"]]);
    Int_R1S3_CID0 = makeUsInt(v[vars["173_1"]]);
    Int_R2S0_CID0 = makeUsInt(v[vars["174_1"]]);
    Int_R2S1_CID0 = makeUsInt(v[vars["175_1"]]);
    Int_R2S2_CID0 = makeUsInt(v[vars["176_1"]]);
    Int_R2S3_CID0 = makeUsInt(v[vars["177_1"]])/2.;
    Int_R3S0_CID0 = makeUsInt(v[vars["178_1"]])/2.;
    Int_R3S1_CID0 = makeUsInt(v[vars["179_1"]]);
    Int_R3S2_CID0 = makeUsInt(v[vars["180_1"]])/4.;
    Int_R3S3_CID0 = makeUsInt(v[vars["181_1"]])/16.;

    Int_R0S0_CID1 = makeUsInt(v[vars["166_2"]])/100.;
    Int_R0S1_CID1 = makeUsInt(v[vars["167_2"]])/100.;
    Int_R0S2_CID1 = makeUsInt(v[vars["168_2"]])/10.;
    Int_R0S3_CID1 = makeUsInt(v[vars["169_2"]])/10.;
    Int_R1S0_CID1 = makeUsInt(v[vars["170_2"]])/10.;
    Int_R1S1_CID1 = makeUsInt(v[vars["171_2"]])/10.;
    Int_R1S2_CID1 = makeUsInt(v[vars["172_2"]]);
    Int_R1S3_CID1 = makeUsInt(v[vars["173_2"]]);
    Int_R2S0_CID1 = makeUsInt(v[vars["174_2"]]);
    Int_R2S1_CID1 = makeUsInt(v[vars["175_2"]]);
    Int_R2S2_CID1 = makeUsInt(v[vars["176_2"]]);
    Int_R2S3_CID1 = makeUsInt(v[vars["177_2"]])/2.;
    Int_R3S0_CID1 = makeUsInt(v[vars["178_2"]])/2.;
    Int_R3S1_CID1 = makeUsInt(v[vars["179_2"]]);
    Int_R3S2_CID1 = makeUsInt(v[vars["180_2"]])/4.;
    Int_R3S3_CID1 = makeUsInt(v[vars["181_2"]])/16.;

    Int_R0S0_CID2 = makeUsInt(v[vars["166_3"]])/100.;
    Int_R0S1_CID2 = makeUsInt(v[vars["167_3"]])/100.;
    Int_R0S2_CID2 = makeUsInt(v[vars["168_3"]])/10.;
    Int_R0S3_CID2 = makeUsInt(v[vars["169_3"]])/10.;
    Int_R1S0_CID2 = makeUsInt(v[vars["170_3"]])/10.;
    Int_R1S1_CID2 = makeUsInt(v[vars["171_3"]])/10.;
    Int_R1S2_CID2 = makeUsInt(v[vars["172_3"]]);
    Int_R1S3_CID2 = makeUsInt(v[vars["173_3"]]);
    Int_R2S0_CID2 = makeUsInt(v[vars["174_3"]]);
    Int_R2S1_CID2 = makeUsInt(v[vars["175_3"]]);
    Int_R2S2_CID2 = makeUsInt(v[vars["176_3"]]);
    Int_R2S3_CID2 = makeUsInt(v[vars["177_3"]])/2.;
    Int_R3S0_CID2 = makeUsInt(v[vars["178_3"]])/2.;
    Int_R3S1_CID2 = makeUsInt(v[vars["179_3"]]);
    Int_R3S2_CID2 = makeUsInt(v[vars["180_3"]])/4.;
    Int_R3S3_CID2 = makeUsInt(v[vars["181_3"]])/16.;

    Int_R0S0_CID3 = makeUsInt(v[vars["166_4"]])/100.;
    Int_R0S1_CID3 = makeUsInt(v[vars["167_4"]])/100.;
    Int_R0S2_CID3 = makeUsInt(v[vars["168_4"]])/10.;
    Int_R0S3_CID3 = makeUsInt(v[vars["169_4"]])/10.;
    Int_R1S0_CID3 = makeUsInt(v[vars["170_4"]])/10.;
    Int_R1S1_CID3 = makeUsInt(v[vars["171_4"]])/10.;
    Int_R1S2_CID3 = makeUsInt(v[vars["172_4"]]);
    Int_R1S3_CID3 = makeUsInt(v[vars["173_4"]]);
    Int_R2S0_CID3 = makeUsInt(v[vars["174_4"]]);
    Int_R2S1_CID3 = makeUsInt(v[vars["175_4"]]);
    Int_R2S2_CID3 = makeUsInt(v[vars["176_4"]]);
    Int_R2S3_CID3 = makeUsInt(v[vars["177_4"]])/2.;
    Int_R3S0_CID3 = makeUsInt(v[vars["178_4"]])/2.;
    Int_R3S1_CID3 = makeUsInt(v[vars["179_4"]]);
    Int_R3S2_CID3 = makeUsInt(v[vars["180_4"]])/4.;
    Int_R3S3_CID3 = makeUsInt(v[vars["181_4"]])/16.;
    
    //Cut 182 - ARRAY - 496 entries!!!
    for(int i=0; i<255; i++) {
      
      if(vars["182_1"] != 0) {
	if(i<62) ADCDNL[i] = makeUsInt(v[vars["182_1"]+i])/1000. - 1;
      }
 
      //if(i<255) {
 
      if(vars["191_1"] != 0) TDCVertOffset[i] = makeUsInt(v[vars["191_1"]+i])/100. - 10;     
      if(vars["193_1"] != 0) Sum4DNL[i] = makeUsInt(v[vars["193_1"]+i])/1000. - 1;
      
      //}
    }
    
    //Cuts 183-185
    OverlapR01_CID0 = makeUsInt(v[vars["183_1"]])/100.;
    OverlapR12_CID0 = makeUsInt(v[vars["184_1"]])/100.;
    OverlapR23_CID0 = makeUsInt(v[vars["185_1"]])/100.;

    OverlapR01_CID1 = makeUsInt(v[vars["183_2"]])/100.;
    OverlapR12_CID1 = makeUsInt(v[vars["184_2"]])/100.;
    OverlapR23_CID1 = makeUsInt(v[vars["185_2"]])/100.;

    OverlapR01_CID2 = makeUsInt(v[vars["183_3"]])/100.;
    OverlapR12_CID2 = makeUsInt(v[vars["184_3"]])/100.;
    OverlapR23_CID2 = makeUsInt(v[vars["185_3"]])/100.;

    OverlapR01_CID3 = makeUsInt(v[vars["183_4"]])/100.;
    OverlapR12_CID3 = makeUsInt(v[vars["184_4"]])/100.;
    OverlapR23_CID3 = makeUsInt(v[vars["185_4"]])/100.;


  //Cuts 186-189
    Pedestal_CID0 = makeUsInt(v[vars["186_1"]])/1000.; 
    Pedestal_CID1 = makeUsInt(v[vars["186_2"]])/1000.;
    Pedestal_CID2 = makeUsInt(v[vars["186_3"]])/1000.;
    Pedestal_CID3 = makeUsInt(v[vars["186_4"]])/1000.;
    
    PedestalRMS_CID0 = makeUsInt(v[vars["189_1"]])/1000.;
    PedestalRMS_CID1 = makeUsInt(v[vars["189_2"]])/1000.;
    PedestalRMS_CID2 = makeUsInt(v[vars["189_3"]])/1000.;
    PedestalRMS_CID3 = makeUsInt(v[vars["189_4"]])/1000.;
    
    PedestalDACSlope_Lo = makeUsInt(v[vars["187_1"]])/1000.; //2 of these
    PedestalDACSlope_Hi = makeUsInt(v[vars["187_2"]])/1000.;
    
    CapIDPedDACSlope_CID0 = makeUsInt(v[vars["188_1"]])/1000.;
    CapIDPedDACSlope_CID1 = makeUsInt(v[vars["188_2"]])/1000.;
    CapIDPedDACSlope_CID2 = makeUsInt(v[vars["188_3"]])/1000.;
    CapIDPedDACSlope_CID3 = makeUsInt(v[vars["188_4"]])/1000.;
    
    //Cuts 190-191
    TDCBinWidth = makeUsInt(v[vars["190_1"]])/1000.;
    
    //Cuts 192-193
    Sum4Average = makeUsInt(v[vars["192_1"]]);
    
    //cout << "DEBUG: Filling tree" << endl;
    
    tree->Fill();
    //cout << "DEBUG: Fill complete" << endl;

  }
  
  //cout << "DEBUG: Finished input loop" << endl;

  tree->Write();

  //cout << "DEBUG: Tree written" << endl;
  
  hfile.Close();
  
}

void convertCuts(int CutNum, string loCut, string hiCut) {
  
  double RealLoCut = -1;
  double RealHiCut = -1;

  if(CutNum < 1 || CutNum > 193) cout << "Invalid CutNumber: " << CutNum << endl;
  else if((CutNum > 14 && CutNum < 18) || (CutNum == 85) || (CutNum > 103 && CutNum < 109) || (CutNum > 137 && CutNum <150)) 
    cout << "CutNum " << CutNum << " is unused." << endl;
  else {
    RealLoCut = convertHexToInt(loCut, 0);
    RealHiCut = convertHexToInt(hiCut, 0);

    if(CutNum < 5){
      RealLoCut = convertHexToInt(loCut,1);
      RealHiCut = convertHexToInt(hiCut,1);

      if(RealLoCut > RealHiCut) RealLoCut = -32768;	

      cout << "Cut Number " << CutNum << ": " << endl;
      cout << "LoCut: " << getVoltOBD(RealLoCut)/10 << ", HiCut: " << getVoltOBD(RealHiCut)/10 << endl;
    }
    else if((CutNum > 50 && CutNum < 63) || CutNum == 192) {
      cout << "Cut Number " << CutNum << ": " << endl;
      cout << "LoCut: " << RealLoCut << ", HiCut: " << RealHiCut << endl;
    }
    else if(CutNum > 71 && CutNum < 74) {
      cout << "Cut Number " << CutNum << ": " << endl;
      cout << "LoCut: " << RealLoCut*100 << ", HiCut: " << RealHiCut*100 << endl;
    }
    else if(CutNum > 62 && CutNum < 72) {
      cout << "Cut Number " << CutNum << ": " << endl;
      if(CutNum < 70) 
	cout << "LoCut: " << LookupDAC(RealHiCut,0)/25 << ", HiCut: " << LookupDAC(RealLoCut,0)/25 << endl;
      else 
	cout << "LoCut: " << LookupDAC(RealHiCut,1)/25 << ", HiCut: " << LookupDAC(RealLoCut,1)/25 << endl;
    }
    else if(CutNum == 76 || (CutNum > 93 && CutNum < 104)) {
	RealLoCut = convertHexToInt(loCut,1);
	RealHiCut = convertHexToInt(hiCut,1);

	if(RealLoCut > RealHiCut) RealLoCut = -32768;	

	cout << "Cut Number " << CutNum << ": " << endl;
	cout << "LoCut: " << getVoltOBD(RealLoCut)-2.5 << ", HiCut: " << getVoltOBD(RealHiCut)-2.5 << endl;
    }
    else if(CutNum < 138) {
      RealLoCut = convertHexToInt(loCut,1);
      RealHiCut = convertHexToInt(hiCut,1);

      if(RealLoCut > RealHiCut) RealLoCut = -32768;

      cout << "Cut Number " << CutNum << ": " << endl;
      cout << "LoCut: " << getVoltOBD(RealLoCut) << ", HiCut: " << getVoltOBD(RealHiCut) << endl;

    }
    else if(CutNum < 158 || CutNum == 166 || CutNum == 167) {
      cout << "Cut Number " << CutNum << ": " << endl;
      cout << "LoCut: " << RealLoCut/100. << ", HiCut: " << RealHiCut/100. << endl;
    }
    else if(CutNum < 164 || CutNum == 168 || CutNum == 169 || CutNum == 170 || CutNum == 171) {
      cout << "Cut Number " << CutNum << ": " << endl;
      cout << "LoCut: " << RealLoCut/10. << ", HiCut: " << RealHiCut/10. << endl;
    }
    else if(CutNum == 177 || CutNum==178) {
      cout << "Cut Number " << CutNum << ": " << endl;
      cout << "LoCut: " << RealLoCut/2. << ", HiCut: " << RealHiCut/2. << endl;
    }
    else if(CutNum < 180) {
      cout << "Cut Number " << CutNum << ": " << endl;
      cout << "LoCut: " << RealLoCut << ", HiCut: " << RealHiCut << endl;
    }
    else if(CutNum == 180) {
      cout << "Cut Number " << CutNum << ": " << endl;
      cout << "LoCut: " << RealLoCut/4. << ", HiCut: " << RealHiCut/4. << endl;
    }
    else if(CutNum == 181) {
      cout << "Cut Number " << CutNum << ": " << endl;
      cout << "LoCut: " << RealLoCut/16. << ", HiCut: " << RealHiCut/16. << endl;
    }
    else if(CutNum == 182 || CutNum == 193) {
      cout << "Cut Number " << CutNum << ": " << endl;
      cout << "LoCut: " << RealLoCut/1000. - 1 << ", HiCut: " << RealHiCut/1000. - 1 << endl;
    }
    else if(CutNum > 182 && CutNum < 186) {
      cout << "Cut Number " << CutNum << ": " << endl;
      cout << "LoCut: " << RealLoCut/100. << ", HiCut: " << RealHiCut/100. << endl;
    }
    else if(CutNum > 185 && CutNum < 191) {
      cout << "Cut Number " << CutNum << ": " << endl;
      cout << "LoCut: " << RealLoCut/1000. << ", HiCut: " << RealHiCut/1000. << endl;
    }
    else if(CutNum == 191) {
      cout << "Cut Number " << CutNum << ": " << endl;
      cout << "LoCut: " << RealLoCut/100. - 10 << ", HiCut: " << RealHiCut/100. - 10 << endl;
    }

  }
}


void convertCutFile(const char* inFile, const char* outFile) {
  
  vector<string> cutNum;
  vector<string> loCut;
  vector<string> hiCut;
  
  int CutNum;

  ifstream file(inFile);
  ofstream OutCut;
  //ofstream OutCut(outFile);
  
  OutCut.open(outFile);
  
  //if(!file.good) {
  //  cout << "Cannot open file!" << endl;
  //  exit(1);
  //}
  
  string line;
  
  while(getline(file,line)) {

    stringstream iss(line);
    string temp;
    string temp2;

    iss >> temp;
    cutNum.push_back(temp);
    iss >> temp;
    iss >> temp2;

    loCut.push_back(temp);
    hiCut.push_back(temp2);

    //marginal cuts now
    iss >> temp;
    iss >> temp2;
  }

  for(unsigned int i=0; i<cutNum.size(); i++) {
    CutNum = stoi(cutNum[i]);
    
    double RealLoCut = -1;
    double RealHiCut = -1;
    
    if(CutNum < 1 || CutNum > 193) OutCut << "Invalid CutNumber: " << CutNum << endl;
    //else if((CutNum > 14 && CutNum < 18) || (CutNum == 85) || (CutNum > 103 && CutNum < 109) || (CutNum > 137 && CutNum <150)) 
    //  OutCut << "CutNum " << CutNum << " is unused." << endl;
    else {
      RealLoCut = convertHexToInt(loCut[i], 0);
      RealHiCut = convertHexToInt(hiCut[i], 0);
      
      //if((CutNum < 5) || (CutNum > 50 && CutNum < 74) || CutNum == 192) {
      if(CutNum < 5) {
	RealLoCut = convertHexToInt(loCut[i],1);
	RealHiCut = convertHexToInt(hiCut[i],1);
	
	if(RealLoCut > RealHiCut) RealLoCut = -32768;

	OutCut << "Cut Number " << CutNum << " ";
	OutCut << "LoCut " << (getVoltOBD(RealLoCut))/10 << " HiCut " << (getVoltOBD(RealHiCut))/10 << endl;
      }
      else if((CutNum > 50 && CutNum < 63) || CutNum == 192) {
	OutCut << "Cut Number " << CutNum << "  ";
	OutCut << "LoCut  " << RealLoCut << " HiCut  " << RealHiCut << endl;
      }
      else if(CutNum > 71 && CutNum < 74) {
	OutCut << "Cut Number " << CutNum << "  ";
	OutCut << "LoCut  " << RealLoCut*100 << " HiCut  " << RealHiCut*100 << endl;
      }
      else if(CutNum > 137 && CutNum < 144) {
	OutCut << "Cut Number " << CutNum << "  ";
	OutCut << "LoCut  " << RealLoCut/16 << " HiCut  " << RealHiCut/16 << endl;
      }
      else if((CutNum > 62 && CutNum < 72) || (CutNum > 76 && CutNum < 85)) {
	OutCut << "Cut Number " << CutNum << "  ";
	if(CutNum < 70 || CutNum > 72)
	  OutCut << "LoCut  " << LookupDAC(RealHiCut,0)/25 << " HiCut  " << LookupDAC(RealLoCut,0)/25 << endl;
	else 
	  OutCut << "LoCut  " << LookupDAC(RealHiCut,1)/25 << " HiCut  " << LookupDAC(RealLoCut,1)/25 << endl;
      }
      else if(CutNum == 76 || (CutNum > 93 && CutNum < 106 && (CutNum % 2 == 1))) {
	RealLoCut = convertHexToInt(loCut[i],1);
	RealHiCut = convertHexToInt(hiCut[i],1);
	
	if(RealLoCut > RealHiCut) RealLoCut = -32768;

	OutCut << "Cut Number " << CutNum << "  ";
	OutCut << "LoCut  " << getVoltOBD(RealLoCut)-2.5 << " HiCut  " << getVoltOBD(RealHiCut)-2.5 << endl;
      }
      else if(CutNum > 93 && CutNum < 106 && (CutNum % 2 == 0)) {
	RealLoCut = convertHexToInt(loCut[i],0);
	RealHiCut = convertHexToInt(hiCut[i],0);
	
	if(RealLoCut > RealHiCut) RealLoCut = -32768;

	OutCut << "Cut Number " << CutNum << "  ";
	OutCut << "LoCut  " << (getVoltOBD(RealLoCut)-2.5)/0.000181654 << " HiCut  " << (getVoltOBD(RealHiCut)-2.5)/0.000181654 << endl;
      }

      else if(CutNum == 50 || (CutNum > 73 && CutNum < 76) || (CutNum > 85 && CutNum < 94) || (CutNum < 15)) {
	RealLoCut = convertHexToInt(loCut[i],1);
	RealHiCut = convertHexToInt(hiCut[i],1);
	
	if(RealLoCut > RealHiCut) RealLoCut = -32768;

	OutCut << "Cut Number " << CutNum << "  ";
	OutCut << "LoCut  " << getVoltOBD(RealLoCut) << " HiCut  " << getVoltOBD(RealHiCut) << endl;
      }
      else if(CutNum > 105 && CutNum < 110) {
	RealLoCut = convertHexToInt(loCut[i],0);
	RealHiCut = convertHexToInt(hiCut[i],0);
	
	if(RealLoCut != 0) RealLoCut = 0;
	
	OutCut << "Cut Number " << CutNum << "  ";
	OutCut << "LoCut  " << RealLoCut/10000. << " HiCut  " << RealHiCut/10000. << endl;
	
      }
      else if(CutNum < 138) {
	//DCH check this.  At least need cut 77 not to trip this when hex sign = 1
	RealLoCut = convertHexToInt(loCut[i],1);
	RealHiCut = convertHexToInt(hiCut[i],1);

	if(RealLoCut > RealHiCut) RealLoCut = -32768;	

	OutCut << "Cut Number " << CutNum << "  ";
	OutCut << "LoCut  " << getVoltOBD(RealLoCut) << " HiCut  " << getVoltOBD(RealHiCut) << endl;
	
      }
      else if(CutNum < 158 || CutNum == 166 || CutNum == 167) {
	OutCut << "Cut Number " << CutNum << "  ";
	OutCut << "LoCut  " << RealLoCut/100. << " HiCut  " << RealHiCut/100. << endl;
      }
      else if(CutNum < 164 || CutNum == 168 || CutNum == 169 || CutNum == 170 || CutNum == 171) {
	OutCut << "Cut Number " << CutNum << "  ";
	OutCut << "LoCut  " << RealLoCut/10. << " HiCut  " << RealHiCut/10. << endl;
      }
      else if(CutNum == 177 || CutNum==178) {
	OutCut << "Cut Number " << CutNum << "  ";
	OutCut << "LoCut  " << RealLoCut/2. << " HiCut  " << RealHiCut/2. << endl;
      }
      else if(CutNum < 180) {
	OutCut << "Cut Number " << CutNum << "  ";
	OutCut << "LoCut  " << RealLoCut << " HiCut  " << RealHiCut << endl;
      }
      else if(CutNum == 180) {
	OutCut << "Cut Number " << CutNum << "  ";
	OutCut << "LoCut  " << RealLoCut/4. << " HiCut  " << RealHiCut/4. << endl;
      }
      else if(CutNum == 181) {
	OutCut << "Cut Number " << CutNum << "  ";
	OutCut << "LoCut  " << RealLoCut/16. << " HiCut  " << RealHiCut/16. << endl;
      }
      else if(CutNum == 182 || CutNum == 193) {
	OutCut << "Cut Number " << CutNum << "  ";
	OutCut << "LoCut  " << RealLoCut/1000. - 1 << " HiCut  " << RealHiCut/1000. - 1 << endl;
      }
      else if(CutNum > 182 && CutNum < 186) {
	OutCut << "Cut Number " << CutNum << "  ";
	OutCut << "LoCut  " << RealLoCut/100. << " HiCut  " << RealHiCut/100. << endl;
      }
      else if(CutNum > 185 && CutNum < 191) {
	OutCut << "Cut Number " << CutNum << "  ";
	OutCut << "LoCut  " << RealLoCut/1000. << " HiCut  " << RealHiCut/1000. << endl;
      }
      else if(CutNum == 191) {
	OutCut << "Cut Number " << CutNum << "  ";
	OutCut << "LoCut  " << RealLoCut/100. - 10 << " HiCut  " << RealHiCut/100. - 10 << endl;
      }
      
    }
  }

  OutCut.close();
}

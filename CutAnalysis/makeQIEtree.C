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

bool isQIE11 = true;

//DCH declare the event objects
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

// Where are Cuts 14 (and 15 for QIE11)?

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

// 104 and 105 are used for QIE10?

//Cuts 106-109
// 106 isn't used?
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

// 138 to 143??
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

// What about 186?

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

    if(isQIE11)
    {
	std::vector<std::string> cuts = {"1_1","2_1","3_1","4_1","14_1","15_1","16_1","17_1","85_1","144_1","145_1","146_1","147_1","148_1","149_1","5_1","6_1","7_1","8_1","9_1","10_1","11_1","12_1","13_1","18_1","19_1","20_1","21_1","22_1","23_1","24_1","25_1","26_1","27_1","28_1","29_1","30_1","31_1","32_1","33_1","34_1","35_1","36_1","37_1","38_1","39_1","40_1","41_1","42_1","43_1","44_1","45_1","46_1","47_1","48_1","49_1","51_1","52_1","53_1","54_1","55_1","56_1","57_1","58_1","59_1","60_1","61_1","62_1","192_1","193_1","193_2","193_3","193_4","193_5","193_6","193_7","193_8","193_9","193_10","193_11","193_12","193_13","193_14","193_15","193_16","193_17","193_18","193_19","193_20","193_21","193_22","193_23","193_24","193_25","193_26","193_27","193_28","193_29","193_30","193_31","193_32","193_33","193_34","193_35","193_36","193_37","193_38","193_39","193_40","193_41","193_42","193_43","193_44","193_45","193_46","193_47","193_48","193_49","193_50","193_51","193_52","193_53","193_54","193_55","193_56","193_57","193_58","193_59","193_60","193_61","193_62","193_63","193_64","193_65","193_66","193_67","193_68","193_69","193_70","193_71","193_72","193_73","193_74","193_75","193_76","193_77","193_78","193_79","193_80","193_81","193_82","193_83","193_84","193_85","193_86","193_87","193_88","193_89","193_90","193_91","193_92","193_93","193_94","193_95","193_96","193_97","193_98","193_99","193_100","193_101","193_102","193_103","193_104","193_105","193_106","193_107","193_108","193_109","193_110","193_111","193_112","193_113","193_114","193_115","193_116","193_117","193_118","193_119","193_120","193_121","193_122","193_123","193_124","193_125","193_126","193_127","193_128","193_129","193_130","193_131","193_132","193_133","193_134","193_135","193_136","193_137","193_138","193_139","193_140","193_141","193_142","193_143","193_144","193_145","193_146","193_147","193_148","193_149","193_150","193_151","193_152","193_153","193_154","193_155","193_156","193_157","193_158","193_159","193_160","193_161","193_162","193_163","193_164","193_165","193_166","193_167","193_168","193_169","193_170","193_171","193_172","193_173","193_174","193_175","193_176","193_177","193_178","193_179","193_180","193_181","193_182","193_183","193_184","193_185","193_186","193_187","193_188","193_189","193_190","193_191","193_192","193_193","193_194","193_195","193_196","193_197","193_198","193_199","193_200","193_201","193_202","193_203","193_204","193_205","193_206","193_207","193_208","193_209","193_210","193_211","193_212","193_213","193_214","193_215","193_216","193_217","193_218","193_219","193_220","193_221","193_222","193_223","193_224","193_225","193_226","193_227","193_228","193_229","193_230","193_231","193_232","193_233","193_234","193_235","193_236","193_237","193_238","193_239","193_240","193_241","193_242","193_243","193_244","193_245","193_246","193_247","193_248","193_249","193_250","193_251","193_252","193_253","193_254","193_255","190_1","191_1","191_2","191_3","191_4","191_5","191_6","191_7","191_8","191_9","191_10","191_11","191_12","191_13","191_14","191_15","191_16","191_17","191_18","191_19","191_20","191_21","191_22","191_23","191_24","191_25","191_26","191_27","191_28","191_29","191_30","191_31","191_32","191_33","191_34","191_35","191_36","191_37","191_38","191_39","191_40","191_41","191_42","191_43","191_44","191_45","191_46","191_47","191_48","191_49","191_50","191_51","191_52","191_53","191_54","191_55","191_56","191_57","191_58","191_59","191_60","191_61","191_62","191_63","191_64","191_65","191_66","191_67","191_68","191_69","191_70","191_71","191_72","191_73","191_74","191_75","191_76","191_77","191_78","191_79","191_80","191_81","191_82","191_83","191_84","191_85","191_86","191_87","191_88","191_89","191_90","191_91","191_92","191_93","191_94","191_95","191_96","191_97","191_98","191_99","191_100","191_101","191_102","191_103","191_104","191_105","191_106","191_107","191_108","191_109","191_110","191_111","191_112","191_113","191_114","191_115","191_116","191_117","191_118","191_119","191_120","191_121","191_122","191_123","191_124","191_125","191_126","191_127","191_128","191_129","191_130","191_131","191_132","191_133","191_134","191_135","191_136","191_137","191_138","191_139","191_140","191_141","191_142","191_143","191_144","191_145","191_146","191_147","191_148","191_149","191_150","191_151","191_152","191_153","191_154","191_155","191_156","191_157","191_158","191_159","191_160","191_161","191_162","191_163","191_164","191_165","191_166","191_167","191_168","191_169","191_170","191_171","191_172","191_173","191_174","191_175","191_176","191_177","191_178","191_179","191_180","191_181","191_182","191_183","191_184","191_185","191_186","191_187","191_188","191_189","191_190","191_191","191_192","191_193","191_194","191_195","191_196","191_197","191_198","191_199","191_200","191_201","191_202","191_203","191_204","191_205","191_206","191_207","191_208","191_209","191_210","191_211","191_212","191_213","191_214","191_215","191_216","191_217","191_218","191_219","191_220","191_221","191_222","191_223","191_224","191_225","191_226","191_227","191_228","191_229","191_230","191_231","191_232","191_233","191_234","191_235","191_236","191_237","191_238","191_239","191_240","191_241","191_242","191_243","191_244","191_245","191_246","191_247","191_248","191_249","191_250","191_251","191_252","191_253","191_254","191_255","186_1","186_2","186_3","186_4","189_1","189_2","189_3","189_4","187_1","187_2","188_1","188_2","188_3","188_4","150_1","150_2","150_3","150_4","151_1","151_2","151_3","151_4","152_1","152_2","152_3","152_4","153_1","153_2","153_3","153_4","154_1","154_2","154_3","154_4","155_1","155_2","155_3","155_4","156_1","156_2","156_3","156_4","157_1","157_2","157_3","157_4","158_1","158_2","158_3","158_4","159_1","159_2","159_3","159_4","160_1","160_2","160_3","160_4","161_1","161_2","161_3","161_4","162_1","162_2","162_3","162_4","163_1","163_2","163_3","163_4","164_1","164_2","164_3","164_4","165_1","165_2","165_3","165_4","166_1","166_2","166_3","166_4","167_1","167_2","167_3","167_4","168_1","168_2","168_3","168_4","169_1","169_2","169_3","169_4","170_1","170_2","170_3","170_4","171_1","171_2","171_3","171_4","172_1","172_2","172_3","172_4","173_1","173_2","173_3","173_4","174_1","174_2","174_3","174_4","175_1","175_2","175_3","175_4","176_1","176_2","176_3","176_4","177_1","177_2","177_3","177_4","178_1","178_2","178_3","178_4","179_1","179_2","179_3","179_4","180_1","180_2","180_3","180_4","181_1","181_2","181_3","181_4","182_1","182_2","182_3","182_4","182_5","182_6","182_7","182_8","182_9","182_10","182_11","182_12","182_13","182_14","182_15","182_16","182_17","182_18","182_19","182_20","182_21","182_22","182_23","182_24","182_25","182_26","182_27","182_28","182_29","182_30","182_31","182_32","182_33","182_34","182_35","182_36","182_37","182_38","182_39","182_40","182_41","182_42","182_43","182_44","182_45","182_46","182_47","182_48","182_49","182_50","182_51","182_52","182_53","182_54","182_55","182_56","182_57","182_58","182_59","182_60","182_61","182_62","183_1","183_2","183_3","183_4","184_1","184_2","184_3","184_4","185_1","185_2","185_3","185_4","77_1","78_1","79_1","80_1","81_1","82_1","83_1","84_1","72_1","73_1","63_1","64_1","65_1","66_1","67_1","68_1","69_1","70_1","71_1","74_1","75_1","76_1","110_1","111_1","112_1","113_1","114_1","115_1","116_1","117_1","118_1","119_1","120_1","121_1","122_1","123_1","124_1","125_1","126_1","127_1","128_1","129_1","130_1","131_1","132_1","133_1","134_1","135_1","136_1","137_1","94_1","95_1","96_1","97_1","98_1","99_1","100_1","101_1","102_1","103_1","138_1","139_1","140_1","141_1","142_1","143_1","87_1","87_2","87_3","87_4","87_5","87_6","87_7","87_8","87_9","87_10","87_11","87_12","87_13","87_14","87_15","87_16","87_17","87_18","86_1","86_2","86_3","86_4","86_5","86_6","86_7","86_8","86_9","86_10","86_11","86_12","86_13","86_14","86_15","86_16","86_17","86_18","89_1","89_2","89_3","89_4","89_5","89_6","89_7","89_8","89_9","89_10","89_11","89_12","89_13","89_14","89_15","89_16","89_17","89_18","88_1","88_2","88_3","88_4","88_5","88_6","88_7","88_8","88_9","88_10","88_11","88_12","88_13","88_14","88_15","88_16","88_17","88_18","91_1","91_2","91_3","91_4","91_5","91_6","91_7","91_8","91_9","91_10","91_11","91_12","91_13","91_14","91_15","91_16","91_17","91_18","90_1","90_2","90_3","90_4","90_5","90_6","90_7","90_8","90_9","90_10","90_11","90_12","90_13","90_14","90_15","90_16","90_17","90_18","93_1","93_2","93_3","93_4","93_5","93_6","93_7","93_8","93_9","93_10","93_11","93_12","93_13","93_14","93_15","93_16","93_17","93_18","92_1","92_2","92_3","92_4","92_5","92_6","92_7","92_8","92_9","92_10","92_11","92_12","92_13","92_14","92_15","92_16","92_17","92_18","50_1","50_2","50_3","50_4","107_1","107_2","108_1","108_2","109_1","109_2","106_1","106_2","195_1","196_1","196_2","196_3","196_4","196_5","196_6","196_7","196_8","196_9","196_10","196_11","196_12","196_13","196_14","196_15","196_16","196_17","196_18","196_19","196_20","196_21","196_22","196_23","196_24","196_25","196_26","196_27","196_28","196_29","196_30","196_31","196_32","196_33","196_34","196_35","196_36","196_37","196_38","196_39","196_40","196_41","196_42","196_43","196_44","196_45","196_46","196_47","196_48","196_49","196_50","196_51","196_52","196_53","196_54","196_55","196_56","196_57","196_58","196_59","196_60","196_61","196_62","196_63","196_64","196_65","196_66","196_67","196_68","196_69","196_70","196_71","196_72","196_73","196_74","196_75","196_76","196_77","196_78","196_79","196_80","196_81","196_82","196_83","196_84","196_85","196_86","196_87","196_88","196_89","196_90","196_91","196_92","196_93","196_94","196_95","196_96","196_97","196_98","196_99","196_100"};
	for (string& cut : cuts)
	{
	    vars[cut] = 0;
	}
    } 
    else
    {
	// First group of variables, only one value per cut
	for (int i = 1; i <= 49; ++i)
	{
	    vars[to_string(i) + "_1"] = 0;
	}
    
	vars["50_1"] = 0;
	vars["50_2"] = 0;
	vars["50_3"] = 0;
	vars["50_4"] = 0;

	for (int i = 51; i <= 85; ++i)
	{
	    vars[to_string(i) + "_1"] = 0;
	}

	for (int i = 86; i <= 93; ++i)
	{
	    for (int j = 1; j <= 18; ++j)
	    {
		vars[to_string(i) + "_" + to_string(j)] = 0;
	    }
	}

	for (int i = 94; i <= 181; ++i)
	{
	    vars[to_string(i) + "_1"] = 0;
	}

	for (int i = 150; i <= 181; ++i)
	{
	    for (int j = 2; j <= 4; ++j)
	    {
		vars[to_string(i) + "_" + to_string(j)] = 0;
	    }
	}

	vars["182_1"] = 0;

	for (int i = 183; i <= 189; ++i)
	{
	    if(i == 187)
		continue;
	    for (int j = 1; j <= 4; ++j)
	    {
		vars[to_string(i) + "_" + to_string(j)] = 0;
	    }
	}

	vars["187_1"] = 0;
	vars["187_2"] = 0;

	for (int i = 190; i <= 200; ++i)
	{
	    vars[to_string(i) + "_1"] = 0;
	}
    }
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

int makeUsInt(string input) {
    //cout << "in makeUSint, with input " << input << endl;
    return stoi(input);
}

int makeSInt(string input) {

    //cout << "in makeSInt, with input " << input << endl;

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
    cout << "in convertHexToInt with myhex " << myhex << endl;
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

    TFile hfile(outFile,"RECREATE");

    BookTree();
    ResetVarMap();

    ifstream input(inFile);
    string line;
  
    int counter = 0;
    int varnum = 0;
  
    //cout << "DEBUG: beginning loop over input lines" << endl;
    while(getline(input, line)) 
    {
	ResetVariables();

	stringstream ss(line);
	string field;

	counter++;

	// skip first line containing cut numbers
	if(counter <2) continue;

	// second line contains cuts
	if(counter == 2) 
	{
	    while(getline(ss, field, ',')) 
	    {
		varnum++;
		if(varnum==1) continue; // Skip the "Cuts" entry
		vars[field]=varnum; // "field" from the dataset doesn't correspond to what is in ResetVarMap()...
	    }
	    cout << "varNum: " << varnum << endl;
	    //cout << "got Vars set" << endl;
	    continue;
	}

	vector<string> v;
	v.clear();
	//cout << "DEBUG Starting loop over CSV entries" << endl;
	//v.push_back("65536");
	while(getline(ss, field, ',')) 
	{
	    v.push_back(field);
	}

	if((int)v.size() < (varnum+1)) continue;
    
	//cout << "DEBUG: Parsed input" << endl;
	// Didn't we just put 65536 in the vector?? How can it equal Pass??
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
	
	// What about 14_1: RRef narrow and 15_1: All Mantissa errors ??
    
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
	
	if(vars.find("94_1") != vars.end())
	{
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
	    if(!isQIE11)
		ImpDiff_RinSelF = (getVoltOBD(makeSInt(v[vars["104_1"]])) - 2.5)/0.000181654;
		       
	    WRPmax = makeUsInt(v[vars["106_1"]])/10000.;
	    WRPRange01 = makeUsInt(v[vars["107_1"]])/10000.;
	    WRPRange12 = makeUsInt(v[vars["108_1"]])/10000.;
	    WRPRange23 = makeUsInt(v[vars["109_1"]])/10000.;
	}

	if(vars.find("110_1") != vars.end())
	{
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
	}

	if(vars.find("150_1") != vars.end())
	{
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
	}

	if(vars.find("182_1") != vars.end())
	{
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
	}

	if(vars.find("183_1") != vars.end())
	{
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
	}
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

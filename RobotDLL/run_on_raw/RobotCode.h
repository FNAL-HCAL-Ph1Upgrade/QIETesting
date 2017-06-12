#ifndef _ROBOTCODE_H_
#define _ROBOTCODE_H_
#include <fstream>
#include <iostream>
#include <string>

using namespace std;
//extern "C"
//{


bool  readline(fstream &file, int &linenumber, string &data, int &previousline);
bool  storeADC(fstream &file);
bool  storeTDC(fstream &file);
bool  storePhase(fstream &file);
bool  storePED(fstream &file);
bool  storeWRP(fstream &file);
void  hexToBeautiful(string TDCtestRead[255][3][22], int tdc[255][3][22], int capID[255][3][22], int range[255][3][22], int mantissa[255][3][22]);
double LookupCharge(int range, int mantissa);
int  doADCTest(string myfile, string outString);
int  doTDCTest(string myfile, string outString);
int  doPhaseTest(string myfile, string outString);
int  doPedTest(string myfile, string outString);
int  doWRPTest(string myfile, string outString);
double  LookupDAC(int DAC, int whichRange);

int main(int argc, char* argv[]);

//Pedestal input data
int PedRange[4][100];
int PedMant[4][100];
int PedCIDCount[4];
int PEDtestRange[64][129];
int PEDtestMant[64][129];
int CAPPEDtestCount[16][4];
int CAPPEDtestRange[16][4][50];
int CAPPEDtestMant[16][4][50];

//WRP input data
int WRPdata[3][4][255];

//ADC test input data
int UpDAC[4][4][64];
int DownDAC[4][4][64];

double Up_work[4][4][63];
double Down_work[4][4][63];

//TDC test input data
int TDCtestRead[255][4][22];
int TDCPhaseRead[114][4][22];

//Pedestal results
double PedCIDMean[4];
double PedCIDRMS[4];

double PedDac[63];
double PedAvg[63];
double PedStd[63];

double PedDacHi[13];
double PedDacLo[13];
double PedAvgHi[13];
double PedAvgLo[13];
double PedErrHi[13];
double PedErrLo[13];

double CAPPedDac[15];
double CAPPedAvg[4][15];
double CAPPedStd[4][15];
double CAPPedErr[4][15];

//double CAPPedDacFit[11];
//double CAPPedAvgFit[4][11];
//}
#endif // ROBOTCODE_H

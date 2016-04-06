//edited to include doTDCTest(), a derivation of Daryl's root script analysis in "QIEtestLatest.cpp"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <cstring>
#include <sstream>
#include <gsl/gsl_fit.h>
#include <gsl/gsl_statistics_double.h>
#include "RobotCode.h"

using namespace std;

extern "C"
{

/*void junk() {bool readline(fstream &file, int &linenumber, string &data, int &previousline);
  bool storeADC(fstream &file, int upDAC[4][4][64], int DownDAC[4][4][64]);
  bool storeTDC(fstream &file, int TDCtestRead[255][4][16]);
  double LookupCharge(int range, int mantissa);
  void doADCTest(char* myfile, char* cPathOut="output/newADC");
  void doTDCTest(char* myfile, char* cPathOut="output/TDCrecent");
  double LookupDAC(int DAQ, int whichRange);}
*/

    bool __stdcall readline(fstream &file, int &linenumber, string &data, int &previousline){

	if(file.eof()) return 0;

	// DCH remove this line for pure raw data
	file >> dec >> linenumber;
	file >> data;

	//  if(linenumber != (previousline + 1)){
	//#ifdef debugstore
	//     cout << "Expected :"<<(previousline+1)<<" Read: "<<linenumber << endl;
	//#endif
	//    return 0;
	//  }

	previousline = linenumber;

	return 1;

    }


    bool __stdcall storeADC(fstream &file) {

	int linenumber = 0; //This tracks the line number of the file
	string data; //This is the data contained on the line

	int previousline = 0;//We use this to check the consistency of the data file

	stringstream teststring; //We use this to check if we have the format that we expect

	//Now we loop through the data until we find the start of the ramping process
	while(data != "ramping_down"){
	    if(!readline(file,linenumber,data,previousline)) return 0;

	}
	// The loops below assign to each element in the string arrays the appropriate element from the file for downDAC.
	// Then, converts the hexadecimal strings over to decimal integers in the integer arrays.
	for(int cap=0; cap<=3; cap++){

	    if(!readline(file,linenumber,data,previousline)) return 0;

	    //At the start of each new capacitor we expect to see "qiedatacap?"
	    teststring.str("");
	    teststring << "qiedatacap" << cap;
	    if(data != teststring.str()){
		return 0;
	    }

	    for(int range=0; range<=3; range++){

		if(!readline(file,linenumber,data,previousline)) return 0;

		//At the start of each new range we expect to see "cap?_rang?"
		teststring.str("");
		teststring << "cap" << cap << "_rang" << range;
		if(data != teststring.str()){
		    return 0;
		}

		for(int bin=0; bin<=63; bin++){
		    if(!readline(file,linenumber,data,previousline)) return 0;

		    istringstream(data) >> hex >> DownDAC[cap][range][bin];

		}
	    }
	}

	//Same thing as above, but for upDAC.
	//Now we loop through the data until we find the start of the ramping process
	while(data != "ramping_up"){
	    if(!readline(file,linenumber,data,previousline)) return 0;
	}
	// The loops below assign to each element in the string arrays the appropriate element from the file for upDAC.
	// Then, converts the hexadecimal strings over to decimal integers in the integer arrays.
	for(int cap=0; cap<=3; cap++){

	    if(!readline(file,linenumber,data,previousline)) return 0;

	    //At the start of each new capacitor we expect to see "qiedatacap?"
	    teststring.str("");
	    teststring << "qiedatacap" << cap;
	    if(data != teststring.str()){
		return 1;
	    }

	    for(int range=0; range<=3; range++){

		if(!readline(file,linenumber,data,previousline)) return 0;

		//At the start of each new range we expect to see "cap?_rang?"
		teststring.str("");
		teststring << "cap" << cap << "_rang" << range;
		if(data != teststring.str()){
		    return 0;
		}

		for(int bin=0; bin<=63; bin++){
		    if(!readline(file,linenumber,data,previousline)) return 0;
		    istringstream(data) >> hex >> UpDAC[cap][range][bin];
		}
	    }
	}

	return 1;
    }

//bool storeTDC(fstream &file, int TDCtestRead[255][4][16])
    bool __stdcall storeTDC(fstream &file)
    {

	int linenumber = 0; //This tracks the line number of the file
	string data; //This is the data contained on the line

	int previousline = 0;//We use this to check the consistency of the data file

	//******************************************************************************
	//Loops through delay/event data, skipping text and getting hex values
	//Start the TDC data storage:
	while(data != ("tdc_test"))
	    //while(data != ("delay=00"))
	{
	    if(!readline(file,linenumber,data,previousline)) {
		//cout << "ended 1"<< endl;
		return 0;
	    }
	}
	while(data != ("next"))
	{
	    if(!readline(file,linenumber,data,previousline)) {
		//cout << "ended 1a"<< endl;
		return 0;
	    }
	}

	for(int delay=0; delay<255; delay++)
	{
	    if(delay != 0) {
		if(!readline(file,linenumber,data,previousline)) {
		    //cout << "ended 3"<< endl;
		    return 0;
		}
	    }

	    //This loop is for the number of events taken.  Currently 3
	    for(int event=0; event<4; event++)
	    {
		//if(!readline(file,linenumber,data,previousline))
		//  {
		//    //cout << "ended 2"<< endl;
		//    return 0;
		//  }
		for(int i=0; i<22; i++)
		{
		    if(!readline(file,linenumber,data,previousline))
		    {
			//cout << "ended 3"<< endl;
			return 0;
		    }
		    istringstream(data) >> hex >> TDCtestRead[delay][event][i];
		}

		if(!readline(file,linenumber,data,previousline))
		{
		    //cout << "ended 4"<< endl;
		    return 0;
		}
	    }
	}

	return 1;
    }

//bool storePed(fstream &file, int PedRange[4][100], int PedMant[4][100], int PedCIDCount[4], int PEDtestRange[64][129], int PEDtestMant[64][129]int CAPPEDtestCount[16][4], int CAPPEDtestRange[16][4][50], int CAPPEDtestMant[16][4][50]) {
    bool __stdcall storePed(fstream &file) {

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

	for(int i=0; i<4; i++) {

	    PedCIDCount[i] = 0;

	    for(int j=0; j<100; j++) {
		PedRange[i][j] = -99;
		PedMant[i][j] = -99;

	    }
	}

	int temp;
	int CID;

	while(data != "volts_test"){
	    istringstream(data) >> hex >> temp;

	    CID = (temp & 0xC000) >> 14;

	    PedRange[CID][PedCIDCount[CID] ] = (temp & 0x00C0)>>6;
	    PedMant[CID][PedCIDCount[CID] ] = temp & 0x003F;

	    PedCIDCount[((temp & 0xC000) >> 14)]++;

	    if(!readline(file,linenumber,data,previousline)) return 0;
	}

	//cout << PedCIDCount[0] << " " << PedCIDCount[1] << " "<< PedCIDCount[2] << " " << PedCIDCount[3] << " " << endl;

	for(int i=0; i<64; i++) {
	    for(int j=0; j<129; j++) {
		PEDtestRange[i][j] = -99;
		PEDtestMant[i][j] = -99;
	    }
	}

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

	for(int i=0; i<16; i++) {
	    for(int j=0; j<4; j++) {
		CAPPEDtestCount[i][j] = 0;
		for(int k=0; k<50; k++) {
		    CAPPEDtestRange[i][j][k] = -99;
		    CAPPEDtestMant[i][j][k] = -99;
		}
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

    bool __stdcall storeWRP(fstream &file) {

	int linenumber = 0; //This tracks the line number of the file
	string data; //This is the data contained on the line

	int previousline = 0;//We use this to check the consistency of the data file

	stringstream teststring; //We use this to check if we have the format that we expect

	//Now we loop through the data until we find the start of the ramping process
	while(data != "mantisa_scan01"){
	    if(!readline(file,linenumber,data,previousline)) return 0;
	}

	if(!readline(file,linenumber,data,previousline)) return 0;
	if(!readline(file,linenumber,data,previousline)) return 0;
	if(!readline(file,linenumber,data,previousline)) return 0;
	if(!readline(file,linenumber,data,previousline)) return 0;
	if(!readline(file,linenumber,data,previousline)) return 0;

	for(int i=0; i<256; i++) {

	    if(!readline(file,linenumber,data,previousline)) return 0;
//cout << data << endl;
	    //istringstream(data) >> hex >> PEDtestread[ped][read];
	    istringstream(data) >> hex >> WRPdata[0][0][i];

	}

	if(!readline(file,linenumber,data,previousline)) return 0;
	if(!readline(file,linenumber,data,previousline)) return 0;
	if(!readline(file,linenumber,data,previousline)) return 0;
	if(!readline(file,linenumber,data,previousline)) return 0;
	if(!readline(file,linenumber,data,previousline)) return 0;

	for(int i=0; i<256; i++) {

	    if(!readline(file,linenumber,data,previousline)) return 0;

	    //istringstream(data) >> hex >> PEDtestread[ped][read];
	    istringstream(data) >> hex >> WRPdata[0][1][i];

	}

	if(!readline(file,linenumber,data,previousline)) return 0;
	if(!readline(file,linenumber,data,previousline)) return 0;
	if(!readline(file,linenumber,data,previousline)) return 0;
	if(!readline(file,linenumber,data,previousline)) return 0;
	if(!readline(file,linenumber,data,previousline)) return 0;

	for(int i=0; i<256; i++) {

	    if(!readline(file,linenumber,data,previousline)) return 0;

	    //istringstream(data) >> hex >> PEDtestread[ped][read];
	    istringstream(data) >> hex >> WRPdata[0][2][i];

	}

	if(!readline(file,linenumber,data,previousline)) return 0;
	if(!readline(file,linenumber,data,previousline)) return 0;
	if(!readline(file,linenumber,data,previousline)) return 0;
	if(!readline(file,linenumber,data,previousline)) return 0;
	if(!readline(file,linenumber,data,previousline)) return 0;

	for(int i=0; i<256; i++) {

	    if(!readline(file,linenumber,data,previousline)) return 0;

	    //istringstream(data) >> hex >> PEDtestread[ped][read];
	    istringstream(data) >> hex >> WRPdata[0][3][i];

	}

	if(!readline(file,linenumber,data,previousline)) return 0;
	if(!readline(file,linenumber,data,previousline)) return 0;
	if(!readline(file,linenumber,data,previousline)) return 0;
	if(!readline(file,linenumber,data,previousline)) return 0;
	if(!readline(file,linenumber,data,previousline)) return 0;
	if(!readline(file,linenumber,data,previousline)) return 0;

	for(int i=0; i<256; i++) {

	    if(!readline(file,linenumber,data,previousline)) return 0;

	    //istringstream(data) >> hex >> PEDtestread[ped][read];
	    istringstream(data) >> hex >> WRPdata[1][0][i];

	}

	if(!readline(file,linenumber,data,previousline)) return 0;
	if(!readline(file,linenumber,data,previousline)) return 0;
	if(!readline(file,linenumber,data,previousline)) return 0;
	if(!readline(file,linenumber,data,previousline)) return 0;
	if(!readline(file,linenumber,data,previousline)) return 0;

	for(int i=0; i<256; i++) {

	    if(!readline(file,linenumber,data,previousline)) return 0;

	    //istringstream(data) >> hex >> PEDtestread[ped][read];
	    istringstream(data) >> hex >> WRPdata[1][1][i];

	}

	if(!readline(file,linenumber,data,previousline)) return 0;
	if(!readlidata,previousline)) return 0;
    if(!readline(file,linenumber,data,previousline)) return 0;

    for(int i=0; i<256; i++) {

	if(!readline(file,linenumber,data,previousline)) return 0;

	//istringstream(data) >> hex >> PEDtestread[ped][read];
	istringstream(data) >> hex >> WRPdata[1][2][i];

    }

    if(!readline(file,linenumber,data,previousline)) return 0;
    if(!readline(file,linenumber,data,previousline)) return 0;
    if(!readline(file,linenumber,data,previousline)) return 0;
    if(!readline(file,linenumber,data,previousline)) return 0;
    if(!readline(file,linenumber,data,previousline)) return 0;

    for(int i=0; i<256; i++) {

	if(!readline(file,linenumber,data,previousline)) return 0;

	//istringstream(data) >> hex >> PEDtestread[ped][read];
	istringstream(data) >> hex >> WRPdata[1][3][i];

    }

    if(!readline(file,linenumber,data,previousline)) return,linenumber,data,previousline)) return 0;
if(!readline(file,linenumber,data,previousline)) return 0;
if(!readline(file,linenumber,data,previousline)) return 0;

for(int i=0; i<256; i++) {

    if(!readline(file,linenumber,data,previousline)) return 0;

    //istringstream(data) >> hex >> PEDtestread[ped][read];
    istringstream(data) >> hex >> WRPdata[2][0][i];

}

if(!readline(file,linenumber,data,previousline)) return 0;
if(!readline(file,linenumber,data,previousline)) return 0;
if(!readline(file,linenumber,data,previousline)) return 0;
if(!readline(file,linenumber,data,previoueadline(file,linenumber,data,previousline)) return 0;
   if(!readline(file,linenumber,data,previousline)) return 0;
   if(!readline(file,linenumber,data,previousline)) return 0;

   for(int i=0; i<256; i++) {

       if(!readline(file,linenumber,data,previousline)) return 0;

       //istringstream(data) >> hex >> PEDtestread[ped][read];
       istringstream(data) >> hex >> WRPdata[2][2][i];

   }

   if(!readline(file,linenumber,data,previousline)) return 0;
   if(!readline(file,linenumber,data,previousline)) return 0;
   if(!readline(file,linenumber,data,previousline)) return 0;
   if(!readline(file,linenumber,dadcall LookupCharge(int range, int mantissa) {
	       //Hardcoded slopes and intercepts for setting mantissa to charge needed in TDC tests

	       //double slope[4][4] = { {   3.5,    7.1,   13.9,    26.9},
	       // {  26.1,   53.9,  103.4,   207.5},
	       // { 196.2,  375.6,  765.7,  1595.2},
	       // {1585.5, 3251.5, 6598.2, 13413.3}
	       //};
	       //
	       //double intercept[4][4] = { {  174.0,   116.6,   -129.6,    -860.8},
	       //     {  774.1,   317.4,  -1370.9,   -7091.2},
	       //     {13392.3, 10432.6,  -2706.1,  -47621.0},
	       //     {47732.8, 21325.3, -97086.8, -487350.0}
	       //
	       //};

	       double slope[4][4] = { {   3.15,    6.53,   lt;

					  if(mantissa < 16 ) result = slope[range][0] * mantissa + intercept[range][0];
					  else if(mantissa < 36 ) result = slope[range][1] * mantissa + intercept[range][1];
					  else if(mantissa < 57 ) result = slope[range][2] * mantissa + intercept[range][2];
					  else if(mantissa < 64 ) result = slope[range][3] * mantissa + intercept[range][3];
					  else result = -1;

					  return result;

		   }

				      int __stdcall doADCTest(string myfile, string outString) {

					  fstream file;
					  ofstream outFile;
					  ofstream testfile;

					  //char cfile[myfile.length()+1];
					  ////char* cfile = new char[myfile.length()+1];
					  //strcpy(cfAC, DownDAC)) {
					  if(!storeADC(file)) {
					      //cout << "The data file was not successfully read" << endl;
					      //exit(1);
					      return 0;
					  }


					  file.close();

					  bool isRange01=0;

					  double Up[4][4][63];
					  double Down[4][4][63];
					  //double Mid[4][4][62];
					  double Width[4][4][62];

					  double SR0_ind[15] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
					  double SR1_ind[20] = {16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35};
					  double SR2_ind[21] = {36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56};
					  double SR3_ind[6]  = {57, 58, 59, 60, 61, 62};

					  double SR0_mid[4][4][15];
					  double SR1_mid[4][4][20];
					  ast bin of
					  //      ramping down and the first bin of ramping up is garbage.
					  //      Up and Down will be 63 element vectors and Mid/Width's will be 62 with first entry for bin 1

					  double SR3_mid_fit[4][3];
					  double SR3_ind_fit[3] = {57,58,59};

					  for (int i=0; i<4; i++){
					      for (int j=0; j<4; j++) {
						  //for (int k=0; k<64; k++) {
						  for (int k=0; k<63; k++) {

						      if( j < 2) isRange01=1;
						      else isRange01=0;

						      Up_work[i][j][k] = LookupDAC(UpDAC[i][j][k],j);
						      Down_work[i][j][k] = LookupDAC(DownDAC[i][j][k],j);

						      //if(k<63) {
						      Up[i][j][k] = LookupDAC(UpDAC[i][j][k+1],j);
						      Down[i][j][k] = LookupDAC(Down    //SR0_width[i][j][k] = LookupDAC(DownDAC[i][j][k+1], j) - LookupDAC(UpDAC[i][j][k+1], j);
										}
						      else if(k<35) {
							  SR1_mid[i][j][k-15] = (LookupDAC(UpDAC[i][j][k+1],j) + LookupDAC(DownDAC[i][j][k+1],j))/2;
							  //SR1_width[i][j][k] = LookupDAC(DownDAC[i][j][k+1], j) - LookupDAC(UpDAC[i][j][k+1], j);
						      }
						      else if(k<56) {
							  SR2_mid[i][j][k-35] = (LookupDAC(UpDAC[i][j][k+1],j) + LookupDAC(DownDAC[i][j][k+1],j))/2;
							  //SR2_width[i][j][k] = LookupD< Width[i][j][k]<<endl;
						      }*/
							  int steps[4] = {15, 20, 21, 6};

							  double c0[4][4][4], c1[4][4][4], cov00[4][4][4], cov01[4][4][4], cov11[4][4][4], chisq[4][4][4];

							  for(int i=0; i<4; i++)
							      for(int j=0; j<4; j++)
								  [i][j][3], &cov00[i][j][3], &cov01[i][j][3], &cov11[i][j][3], &chisq[i][j][3]);
						  else     gsl_fit_linear(SR3_ind,     1, SR3_mid[i][j],1,  6, &c0[i][j][3], &c1[i][j][3], &cov00[i][j][3], &cov01[i][j][3], &cov11[i][j][3], &chisq[i][j][3]);

						  //if(i==0 & j==0) cout << "CID0, R0/SR0, C0: " << c0[i][j][0] << ", c1: " << c1[i][j][0] < ", chi2 = " << chisq[i][j][0] << endl;
						  //cout << "CID0" << i << ", R" << j << "/SR0, C0: " << c0[i][j][0] << ", c1: " << c1[i][j][0] << ", chi2 = " << chisq[i][j][0] << endl;
						  //outFile << "CID0" << i << ", R" << j << "/SR0, C0: " << c0[i][j][0] << " +/- " << sqrt(cov00[i][j][0]) << ", [j][2] << " +/- " << sqrt(cov00[i][j][2]) << ", c1: " << c1[i][j][2] << " +/- " << sqrt(cov11[i][j][2])  << ", chi2 = " << chisq[i][j][2] << endl;
						  //outFile << "CID3" << i << ", R" << j << "/SR3, C0: " << c0[i][j][3] << " +/- " << sqrt(cov00[i][j][3]) << ", c1: " << c1[i][j][3] << " +/- " << sqrt(cov11[i][j][3])  << ", chi2 = " << chisq[i][j][3] << endl;
					      }
					  }

					  for(int i=0; i<4; i++){
					      //outFile << (150 + j*4) << "  " << setfill('0') << setw(4) << hex << int(abs(c1[i][j][0])) << endl;
					      //outFile << (151 + j*4) << "  " << setfill('0') << setw(4) << hex << int(abs(c1[i][j][1])) << end endl;
					      //outFile << (168 + j*4) << "  " << setfill('0') << setw(4) << hex << int(abs(c0[i][j][2])) << endl;
					      //outFile << (169 + j*4) << "  " << setfill('0') << setw(4) << hex << int(abs(c0[i][j][3])) << endl;

					      for(int j=0; j<4; j++) {
						  for(int k=0; k<4; k++) {
						      if(c1[i][j][k] < 0) c1[i][j][k] = 0;

						      if(j < 2) {
							  if(c1[i][j][k] > 655) c1[i][j][k] = 655.35;
						      }
						      else if(j == 2 || k<2) {
							  if(c1[i][j][k] > 6553) c1[i][j][k] = 6553.5;
						      }
						      else {
							  if(c1[i][j][k] > 65535) c1[i][j][k] = 65535;
						      }


						      c0[i][j][k] = abs(c0[i][j][k]);

						      if(c0[i][j][k] < 0) c0[i][j][k] = 0;

						      if(j ==0 && k < 2) {
							  if(c0[i][j][k] > 655) c0[i][j][k] = 655.35;
						      }
						      else if(j==0 || (j==1 && k < 2)) {
							  if(c0[i][j][k] > 6553) c0[i][j][k] = 6553.5;
						      }
						      else if((j ==1) || (j==2 && k<3) || (j==3 && k==1)) {
							  if(c0[i][j][k] > 65535) c0[i][j][k] = 65535;
						      }
						      else if(j==2 || (j==3 && k==0)) {
							  if(c0[i][lse if(j==3 && k==3) {
								      if(c0[i][j][k] > 1048560) c0[i][j][k] = 1048560;
								  }

								  }
							      }

							  outFile << "150  " << setfill('0') << setw(4) << hex << int(abs(c1[i][0][0]*100)) << endl;
							  outFile << "151  " << setfill('0') << setw(4) << hex << int(abs(c1[i][0][1]*100)) << endl;
							  outFile << "152  " << setfill('0') << setw(4) << hex << int(abs(c1[i][0][2]*100)) << endl;
							  outFile << "153  " << setfill('0') << setw(4) << hex << int(abs(c1[i][0][3]*100)) << endl;

							  outFile << "154  " << setfill('0') << setw(4) << hex << int(abs(c1[i][1][0]*100)) << endl;
							  outFile << "155  " << setfill('0') << setw(4) << hex << int(abs(c1[i][1][1]*100)) << endl;
							  outFile << "156  " << setfill('0') << setw(4) << hex << int(abs(c1[i][1][2]*100)) << endl;
							  outFile << "157  " << setfill('0') << setw(4) << hex << 2][1]*10)) << endl;
					      outFile << "160  " << setfill('0') << setw(4) << hex << int(abs(c1[i][2][2]*10)) << endl;
					      outFile << "161  " << setfill('0') << setw(4) << hex << int(abs(c1[i][2][3]*10)) << endl;

					      outFile << "162  " << setfill('0') << setw(4) << hex << int(abs(c1[i][3][0]*10)) << endl;
					      outFile << "163  " << setfill('0') << setw(4) << hex << int(abs(c1[i][3][1]*10)) << endl;
					      outFile << "164  " << setfill('0') << setw(4) << hex << int(abs(c1[i][3][2])) << endl;
					      outFile << "165  " << setfill('0') << setw(4) << hex << int(abs(c1[i][3][3])) << endl;



					      outFile << "166  " << set(abs(c0[i][1][0]*10)) << endl;
					      outFile << "171  " << setfill('0') << setw(4) << hex << int(abs(c0[i][1][1]*10)) << endl;
					      outFile << "172  " << setfill('0') << setw(4) << hex << int(abs(c0[i][1][2])) << endl;
					      outFile << "173  " << setfill('0') << setw(4) << hex << int(abs(c0[i][1][3])) << endl;

					      outFile << "174  " << setfill('0') << setw(4) << hex << int(abs(c0[i][2][0])) << endl;
					      outFile << "175  " << setfill('0') << setw(4) << hex << int(abs(c0[][3][3]/16)) << endl;

					      //}

					  }

					  double average[4][4][4];
					  for(int i=0; i<4; i++)
					      for(int j=0; j<4; j++)
					      {
						  average[i][j][0] = 0;
						  average[i][j][1] = 0;
						  average[i][j][2] = 0;
						  average[i][j][3] = 0;
					      }

					  for (int i= 0; i<4; i++) {
					      for (int j=0;j<4; j++) {
						  for(int k=0; k<62;k++) {

						      /*if(k==0) {
							average[i][j][0] = 0;
							average[i][j][1] = 0;
							average[i][j][2] = 0;
							average[i][j][3] = 0;
							}*/

						      if(k<15 && k>=0) {average[i][j][0] += Width[i][j][k];}
						      else if(k<35 && k>=15) {r CapID "<< i <<" Range "<<j<<" SR "<<2<<": "<< average[i][j][2]<<endl;
  //outFile <<"Average for CapID "<< i <<" Range "<<j<<" SR "<<3<<": "<< average[i][j][3]<<endl;
}
      }
    }
  }

  /*for (int i= 0; i<4; i++) {
    for (int j=0;j<4; j++) {
    outFile << average[i][j][0]<<endl;
    outFile << average[i][j][1]<<endl;
    outFile << average[i][j][2]<<endl;
    outFile << average[i][j][3]<<endl;
    }
    }*/

  int whichSR;
  double difference[4][4][62];
  double myADCDNL =0 ;
  //ofstream DNLfile;
  //DNLfile.open("DNLdata.txt");
  doublj][k] - average[i][j][whichSR]);
////outFile << "DNL CapID: " << i <<" Range: " << j <<" Mantissa: "<<k<<" DNL " << ((Width[i][j][k] - average[i][j][whichSR]))/average[i][j][whichSR] << endl;
//outFile << "182  " << setfill('0') << setw(4) << hex
//<<int((((Width[i][j][k] - average[i][j][whichSR])/average[i][j][whichSR])+1)*1000) << endl;
////DNLfile << abs((Width[i][j][k] - average[i][j][whichSR] hex << int((best+1)*1000) << endl;


  }

  // DCH, add range overlap test here!!!
  double RangeOverlap[3];

  for(int i=0; i<4; i++) {
    //outFile << "Overlap CapID: " << i ;
    //outFile <<  " Range 0-1 " << (Down[i][0][62] - Up[i][1][0])/average[i][0][3] + 2;
File << "183  " << setfill('0') << setw(4) << hex << int(((Down[i][0][62] - Up[i][1][0])/average[i][0][3] + 2)*100) << endl;

    if(RangeOverlap[1] < 0) outFile << "184  0000"<< endl;
    else if(RangeOverlap[1] > 655) outFile << "184  FFFF" << endl;
    else outFile << "184  " << setfill('0') a.txt");
						      outFile.open(outString.c_str());

						      if(!file) {
							  //cout << "Cannot open file!"<<endl;
							  //exit(1);
							  return 0;
						      }

						      // Read in pedestal data
						      if(!storePed(file)) {
							  //cout << "The data file was not successfully read" << endl;
							  //exit(1);
							  return 0;
						      }

						      file.close();

						      bool majo
						      if(majorPedError || PedCIDMean[i] > 65.5) {
							  outFile << "186  FFFF" << endl;
							  outFile << "189  FFFF" << endl;
						      }
						      else {
							  //outFile << "Ped CapId" << i << ": " << PedCIDMean[i] << " +/- " << PedCIDRMS[i] << enge[ped][read]*64 + PEDtestMant[ped][read]) - PedAvg[ped-1],2);
							  //PedStd[ped-3] += pow((PEDtestRange[ped+32][read]*64 + PEDtestMant[ped+32][read]) - PedAvg[ped-3],2);

						      }
						      //PedStd[ped-3] /= 21;
						      //PedStd[ped-3] = sqrt(PedStd[ped-3]);

						      PedStd[" << PedAvgHi[i] << endl;
  //}

  double cs0[2], cs1[2], covs00[2], covs01[2], covs11[2], chisqs[2];  //appended "s" stands for "Sum"

  //gsl_fit_linear(PedDacLo, 1, PedAvgLo, 1, 13, &cs0[0], &cs1[0], &covs00[0], &covs01[0], &covs11[0],1] > 65.5) outFile << "187  FFFF" << endl;
  else outFile << "187  " << setfill('0') << setw(4) << hex << int(cs1[1]*1000)  << endl;
  //outFile << "188  " << cs1[1] << endl;

  //if(PedAvg[0] > 3.5 || PedAvg[5] < 3.5) outFile << "PedDac error. 3.5 not within DAC sed-4] += CAPPEDtestRange[testPed][i][read]*64 + CAPPEDtestMant[testPed][i][read];
						  }
						  CAPPedAvg[i][ped-4] /= CAPPEDtestCount[ped-4][i];

						  CAPPedStd[i][ped-4] = 0;

						  for(int read=0; read<CAPPEDtestCount[testPed][i]; read++], &Capcovs00[i], &Capcovs01[i], &Capcovs11[i], &Capchisqs[i]);

					      //outFile<<"CAPPedDac Fit,  c0: "<<Capcs0[i]<<"  c1: "<<Capcs1[i]<<endl;
					      if(Capcs1[i] < 0) outFile << "188  0000" << endl;
					      else if(Capcs1[i] > 65.5) outFile <<"188  FFFF" << endl;
					      else outFile<      errRate[i][j] = 0;

					      for(int k=0; k<256; k++) {
						  denom[i][j] += WRPdata[i][j][k];
					      }
					      //if(i==1) denom[i][j] *= 50./512;
					      //else denom[i][j] *= 30./512;
					      denom[i][j] *= 1./512;

					      //outFile << "denom["<< i <<"6./(N*sqrt(6.28))*(e1[i][j] + e2[i][j] + e3[i][j])/denom[i][j];
					      errRate[i][j] = stepSize[i]/(width[i]*sqrt(6.28))*(e1[i][j] + e2[i][j] + e3[i][j])/denom[i][j];


					      eOut[i] += errRate[i][j]/4.;

					      //cout << "done with j=" << j << endl;

					  }
					  //cout << "done with i=" << i <<  endl;

				      }

				      //outFile << eOut[0] << " " << eOut[1] << " " << eOut[2] << endl;

				      if(eOut[0] < 0) outFile << "107  0000" << endl;
				      else if(eOut[0] > 6.5) outFile <<"107  FFFF" << endl;
				      else outFile<<"107  " << setfill('0') << setw(4) << hex << int(eOut[0]*10000)<<endl;

				      ifmaxOut < 0) outFile << "106  0000" << endl;
	       else if(maxOut > 6.5) outFile <<"106  FFFF" << endl;
	       else outFile<<"106  " << setfill('0') << setw(4) << hex << int(maxOut*10000)<<endl;

	       //cout << "done with wrp test" << endl;

	       return 1;
	   }

	   int __stdcall doTDCTest(string myfile, string outString) {

	       fstream file;
	       ofstream outFile;

	       //char cfile[myfile.length()+1];
	       //strcpy(cfile, myfile.c_str());

	       //file.open(myfile, ios::in);
	       file.open(myfile.c_str(), ios::in);

	       //outFile.open("TDCdata.txt");
	       outFile.open(outString.c_str());

	       if(!file) {
		   //cout << "Cannot open file!"<<endl;
		   //exit(1);
		   return 0;
	       }

	       //int TDCtestRead[255][4][16];
	       //if(!storeTDC(file, TDCtestRead)) {
	       if(!storeTDC(file)) {
		   //outFile << "The data file was not successfully read" << endl;
		   //cout << "The data file was not successfully read" << endl;
		   //exit(1);
		   return 0;
	       }

	       file.close();

	       int TDCCapID[255][4][22];
	       int TDC[255][4][22];
	       int TDCRange[255][4][22];
	       int TDCMan][22];
	   double TDCCharge[255][4][22];

	   double time[255];   //will store manipulated "time" that overlays the 3 separate TDC ramps
	   double inTDC[255];  //critical TDC value for given event

	   double time_fit[75];
	   double inTDC_fit[75];

	   //for loop COMPRESSES 3 TDC RAMPS INTO 1 & DOES hEXTOBEAUTIFUL, BUT MUCH SHORTER...
	   for(int delay=0; delay<255; delay++) {
	       time[delay] = delay/4.;

	       //DCH, hard codes to create 1 vector
	       /*if(time[delay] < 22.5) time[delay] +=25;
		 else if(time[delay] > 47) time[delay] -= 25;
		 time[delay] -= 22.25;*/

	       for(int event = 0; event < 4; event++) {
		   for(int slot=0; slot < 22; slot++) {

		       TDCCapID[delay][event][slot] = (TDCtestRead[delay][event][slot] & 0xC000) >> 14;
		       TDC[delay][event][slot] = (TDCtestRead[delay][event][slot] & 0x3F00) >> 8;
		       TDCRange[delay][event][slot] = (TDCtestRead[delay][event][slot] & 0x00C0) >> 6;
		       TDCMant[delay][eventelay][event][slot], TDCMant[delay][event][slot]);
	       }
	   }
	   }

       bool found = false;
       double Sum2[255], Sum3[255], Sum4[255];

       int whichEvent = 1;

       for(int delay=0; delay<255; delay++) {

	   Sum2[delay] = 0.0;
	   Sum3[delay] = 0.0;
	   Sum4[delay] = 0.0;

	   found = false;
	   for(int i=3; i<22; i++) {

	       //DCH add something here to test not 62???

	       if(TDC[delay][whichEvent][i]==63) continue;

	       if(found != true) {
		   inTDC[delay] = TDC[delay][whichEvent][i];
		   //outFile<<"Delay "<<delay;
		   for(int k=0; k < 4; k++) {
		       //outFile<<"Delay "<<delay;
		       if(k<2 && (i+k < 22))
		       {
			   Sum2[delay] += LookupCharge(TDCRange[delay][whichEvent][i+k], TDCMant[delay][whichEvent][i+k]);
		       }
		       if(k<3 && (i+k < 22))
		       {
			   Sum3[delay] += LookupCharge(TDCRange[delay][whichEvent][i+k], TDCMant[delay][whichEvent][i+k]);
		       }
		       if(i+k < 22)
		       {
			   Sum4[delay] += LookupCharge(TDCRange[delay][whichEven;

								       /// DCH line to manipulate noisy TDC data from QIE10 to reproduce "clean" data
								       //if(delay>0) if(((inTDC[delay] > (5 + inTDC[delay-1])) && (inTDC[delay-1] > 0 ))|| ((inTDC[delay] > (5 + inTDC[delay+1])) && (inTDC[delay+1] > 0 ))) inTDC[delay] -= 8;
								       }

						       //if(!found) cout << "Error, no pulse seen." << endl;
						       }

			       //for(int i=0; i<255; i++)
			       //{
			       //cout<<inTDC[i]<<endl;
			       // }
			       /**************************
				// This piece of code below should find where the tdc resets from 49 to some significantly lower value,
 and then use those time values to combine the three curves into 1.
			       ********************/

			       int max1=0;
			   int max2=0;
			   int maximum=0;
			   int gotcha = 0;
			   int delta[254];
		       }
		       else if(inTDC[i] == 49 && inTDC[i+1] == 1) {
			   foundInt = 1;
			   interval = i;
			   inTDC[i] = 0;
		       }

		   }
	       }

	       for (int i=0; i<255; i++) {
		   //if (i==gotcha) continue;
		   //delta[i] = inTDC[i] - inTDC[i+1];
		   //if (delta[i]>max2) {
		   //max2 = delta[i];
		   //}

		   if(time[i] < (interval/4.)) time[i] += 24.5;
		   else if(time[i] > (interval/4. + 24.5)) {
		       while(time[i] > (interval/4. + 24.5)) time[i] -= 24.5;
		   }
		   time[i] -= interval/4.;

	       }
	       //if (max1>max2) maximum=max2;
	       //else maximum=max1;

	       //int arewethereyet = 0;
	       //double time2, time3time[delay+1];
	       //        arewethereyet ++;
	       //    }
	       //    if (arewethereyet<2) continue;}
	       //
	       //    for(int delay=0; delay<255; delay++)
	       //    {
	       //        if(time[delay] < time2) time[delay] +=25;
	       //        else if(time[delay] >= time3) time[delay] -= 25;
	       //        time[delay] -= (time2-0.25);
	       //    }

	       //cout<<time2<<" "<<time3<<endl;
	       /*double cs0[3], cs1[3], covs00[3], covs01[3], covs11[3], chisqs; //appended "s" stands for "Sum", just reusing for Sum2/3/4

		 gsl_fit_linear(time, 1, Sum2, 1, 255, &cs0, &cs1, &covs00, &covs01, &covs11, &chisqs); //Not sure about possibly over-flowed Sum[i] & "255"
		 outFile<<"Sum2 Fit,  c0: ", covs01[3], covs11[3], chisqs[3];  //appended "s" stands for "Sum"
		 //
		 //gsl_fit_linear(time, 1, Sum2, 1, 255, &cs0[0], &cs1[0], &covs00[0], &covs01[0], &covs11[0], &chisqs[0]);
		 //outFile<<"Sum2 Fit,  c0: "<<cs0[0]<<"  c1: "<<cs1[0]<<endl;
		 //gsl_fit_linear(time, 1, Sum3, 1, 255, &cs0[1], &cs1[1], &covs00[1], &covs01[1], &covs11[1], &chisqs[1]);
		 //outFile<<"Sum3 Fit,  c0: "<<cs0[1]<<"  c1: "<<cs1[1]<<endl;
		 //gsl_fit_linear(time, 1, Sum4, 1, 255, &cs0[2], &cs1[2], &covs00[2], &covs01[2], &covs11[2], &chisqs[2]);
		 //outFile<<"Sum4 Fit,  c0: "<<cs0[2]<<"  c1: "<<cs1[2]<<endl;
		 //
		 //double Sum2fit[255];
		 //double Sum3fit[255];
		 //double Sum4fit[255];
		 //double DNLsum2[255];
		 //double DNLsum3[255];
		 //double DNLsum4[255];

		 double Sum2Avg = 0;
		 double Sum3Avg = 0;
		 double Sum4Avg = 0;

		 for (int i=0; i<255; i++) {

		 Sum2Avg += Sum2[i];
		 Sum3Avg += Sum3[i];
		 Sum4Avg += Sum4[i];

		 //Sum2fit[i] = (cs0[0]+cs1[0]*time[i]);
		 //DNLsum2[i] = (Sum2[i] - Sum2fit[i])/Sum2    //Sum3fit[i] = (cs0[1]+cs1[1]*time[i]);
		 //DNLsum3[i] = (Sum3[i] - Sum3fit[i])/Sum3fit[i];
		 //
		 //Sum4fit[i] = (cs0[2]+cs1[2]*time[i]);
		 //DNLsum4[i] = (Sum4[i] - Sum4fit[i])/Sum4fit[i];
		 }

		 Sum2Avg /= 255;
		 Sum3Avg /= 255;
		 Sum4Avg /= 255;

		 //double sum2stddev, sum3stddev, sum4stddev;

		 //sum2stddev = gsl_stats_sd(DNLsum2, 1, 255);
		 //sum3stddev = gsl_stats_sd(DNLsum3, 1, 255);
		 //sum4stddev = gsl_stats_sd(DNLsum4, 1, 255);

		 double Sum2DNL[255];
		 double Sum3DNL[255];
		 double Sum4DNL[255];

		 for(int i=0; i<255; i++)
		 {
		 //if(abs(DNLsum2[i]) > sum2stddev)
		 //  {
		 //  outFile << "sum reading "<<i<<" was an outlier with a DNL value of "<<DNLsum2[i]<<endl;
		 //  }
		 //if(DNLsum3[i] > 3*sum3stddev)
		 ]<<endl;
		 //  }

		 if(Sum2Avg > 0) Sum2DNL[i] = (Sum2Avg - Sum2[i])/Sum2Avg;
		 else Sum2DNL[i]  = 100000;
		 if(Sum3Avg > 0) Sum3DNL[i] = (Sum3Avg - Sum3[i])/Sum3Avg;
		 else Sum3DNL[i] = 100000;
		 if(Sum4Avg > 0) Sum4DNL[i] = (Sum4Avg - Sum4[i])/Sum4Avg;
		 else Sum4DNL[i] = 100000;

		 }

		 //outFile << "Sum2 Avg:  " << Sum2Avg << endl;
		 //outFile << "Sum3 Avg:  " << Sum3Avg << endl;
		 //outFile << "Sum4 Avg:  " << Sum4Avg << endl;
		 if(Sum3Avg < 0) outFile << "192  0000" << endl;
		 else if(Sum3Avg > 65534) outFile << "192  FFFF" << endl;
		 else outFile << "192  " << setfill('0') << setw(4) << hex << int(Sum3Avg) << endl;

		 for(int i=0; i<255; i++){
		 //outFile << "DNL Sum2/3/4 " << Sum2DNL[i] << " " << Sum3DNL[i] << " " <FITTER*********************

		 double c0, c1, cov00, cov01, cov11, chisq; //DON'T THINK THESE NEED TO BE ARRAYS...

		 for(int i=0; i<75; i++) {
		 time_fit[i] = time[65+i];
		 inTDC_fit[i] = inTDC[65+i];
		 }

		 //gsl_fit_linear(time, 1, inTDC, 1, 255, &c0, &c1, &cov00, &cov01, &cov11, &chisq);
		 gsl_fit_linear(time_fit, 1, inTDC_fit, 1, 75, &c0, &c1, &cov00, &cov01, &cov11, &chisq);

		 //outFile << "TDC Bin Width " << 1/c1 << endl;
		 if(c1 <= 0) outFile << "190  0000" << endl;
		 else if(c1 < 0.016) outFile <<"190  FFFF" << endl;
		 else outFile << "190  " << setfill('0') << setw(4) << hex << int(1000/c1) << endl;


		 < -25) DNLtdc[i] = 50 + DNLtdc[i];

		 //outFile << "  TDC Offset: "<<DNLtdc[i]<<endl;
		 if(DNLtdc[i] < -10) outFile << "191  0000" << endl;
		 else if(DNLtdc[i] > 600) outFile << "191  FFFF" << endl;
		 else outFile << "191  "<< setfill('0') << setw(4) << hex << int((DNLtdc[i]+10)*100)<<endl;

		 }

		 //double stddev;
		 //
		 //stddev = gsl_stats_sd(DNLtdc, 1, 255);
		 //
		 //for(int i=0; i<255; i++)
		 //{
		 //    if(DNLtdc[i] > 3*stddev)
		 //    {
		 //        outFile << "TDC reading "<<i<<" was an outlier with a tdc value of "<<inTDC[i]<<endl;
		 //    }
		 //}
		 outFile.close();

		 return 1;
		 }


		 double __stdcall LookupDAC(int DAC, int whichRange) {

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

		 ////preliminary QIE11
		 //if(whichRange < 2) return -0.3782*DAC+12295;
		 //else if(whichRange < 4) return -12.019*DAC+420168;

		 else return 0;

		 }

		 }

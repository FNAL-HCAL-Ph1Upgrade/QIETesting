##################################
## Create dataframe to hold     ##
## QIE information.             ##
##                              ##
## Author: Nadja Strobbe        ##
## Initial version: 2016/03/25  ##
##################################

import pandas as pd
import numpy as np
from collections import defaultdict
from CutInfo11 import cutinfo11
import glob

class QIEDataframe:
    def __init__(self,inputfile,fromCutsMaker=False,addLocation=False):
        self.input = inputfile # this is now a list
        self.addLocation = addLocation
        if not fromCutsMaker:
            self.dict = self.createDictionary()
            self.df = self.createDataframe()
        else:
            self.dict = None
            self.df = self.readCutsMakerFile()

    def createDictionary(self):
        """Create dataframe from the cuts_all file"""
        entries = 0
        info = []
        temp_dict = None
        for inputfile in self.input:
            with open(inputfile) as f:
                location = None
                lastlocation = None
                if self.addLocation:
                    location = open(inputfile.replace("qie10.dat.cuts_all","ChipLocationInfo.txt"))
                for line in f:
                    if "Chip Number" in line:
                        # write what was there
                        if temp_dict != None:
                            info.append(self.processDictionary(temp_dict))
                        # Prep for next
                        temp_dict = defaultdict(list)
                        try:
                            temp_dict["ChipID"] = int(line.strip().split()[3])
                        except ValueError:
                            print line
                            raise
                            
                        temp_dict["i"] = entries
                        entries+=1
                        # read in location info, assumes same ordering and no missing info
                        if self.addLocation:
                            locinfo = lastlocation
                            if lastlocation is None:
                                locinfo = location.readline()
                            loclist = locinfo.strip().split("-")
                            if int(loclist[0]) != temp_dict["ChipID"]:
                                print "Location (%s) doesn't match ChipID (%s)!" % (loclist[0], temp_dict["ChipID"])
                                print "Assuming location is correct, so skipping to next chip"
                                lastlocation = locinfo
                                temp_dict = None
                                continue
                            else:
                                temp_dict["Tray"]   = int(loclist[1])
                                temp_dict["Row"]    = int(loclist[2])
                                temp_dict["Column"] = int(loclist[3])
                                lastlocation = None
                    else:
                        if temp_dict != None:
                            # add to the dictionary
                            data = line.strip().split()
                            try:
                                temp_dict[data[0]].append(data[1]) 
                            except IndexError:
                                print line
                                raise
                if self.addLocation:
                    location.close()
    
        # Write the last piece as well
        info.append(self.processDictionary(temp_dict))        
        f.close()
        return info
    
    def createDictionaryPhases(self):
        """Create dataframe from the phases file. Chip number is in the filename"""
        entries = 0
        info = []
        temp_dict = None
        inputfiles = []
        for inputfile in self.input:
            chipid = inputfile.split("/")[-1].split("_")[0]
            # write what was there
            if temp_dict != None:
                info.append(self.processDictionary(temp_dict))
            # Prep for next
            temp_dict = defaultdict(list)
            try:
                temp_dict["ChipID"] = int(chipid)
            except ValueError:
                print chipid
                raise
            temp_dict["i"] = entries
            entries+=1
            with open(inputfile) as f:
                for line in f:
                    if temp_dict != None:
                        # add to the dictionary
                        data = line.strip().split()
                        temp_dict[data[0]].append(data[1]) 
    
        # Write the last piece as well
        info.append(self.processDictionary(temp_dict))        
        f.close()
        return info
    
    def processDictionary(self,d):
        """If item is a list, create new entries for each element in that list"""
        newdict = {}
        for k, v in d.iteritems():
            if type(v) is list:
                for i in xrange(len(v)):
                    newdict["%s_%s"%(k,i+1)] = v[i]
            else:
                newdict[k] = v
        return newdict

    def createDataframe(self):
        """Convert dictionary to dataframe."""
        df = pd.DataFrame(self.dict)
        df.set_index("i", inplace=True)
        return df

    def readCutsMakerFile(self):
        """Read csv file from cutsmaker and convert to dataframe."""
        header = ""
        with open(self.inputfile[0]) as f:
            l1 = f.readline()
            l2 = f.readline()
            header = "Status,%s" % (l2)
            header = header.replace("Cuts","ChipID")
        df = pd.read_csv(self.inputfile[0], skiprows=2, header=None, names=header.split(","))
        df.index.name = "i"
        df.drop("Status", axis=1, inplace=True)
        df["ChipID"] = df["ChipID"].apply(lambda x: int(x.split()[-1]))
        return df

    def processDataframe(self):
        """Process the dataframe to remove missing values, i.e. chips with hard failures."""
        # remove hard failures
        hardfailures = self.df.isnull().any(axis=1)
        print hardfailures.head()
        total_hardfailures = hardfailures.sum()
        print "Found %s chips with hard failures" % total_hardfailures
        self.hardfailures = "%s/hard_failures.txt"%self.input[0].rpartition("/")[0]
        with open(self.hardfailures.replace("*",""), 'w') as f:
            f.write("Total chips: %s \n" % len(self.df.index))
            f.write("hard failures = %s\n" % (total_hardfailures))
            f.write("Chips: \n%s" % "\n".join(["%s"%i for i in self.df[hardfailures==True]["ChipID"]]))
        if self.addLocation:
            # print out location info of hard failures
            hardfailuresdf = self.df[hardfailures]
            hardfailuresdf.loc[:,"Sorting"] = 0
            hardfailuresdf.to_csv("SortingAllHardFail.txt", columns=["ChipID","Tray","Row","Column","Sorting"],
                                header=False, index=False)
        self.df = self.df.dropna(axis=0)
        #self.df_raw = self.df.copy()

        # Loop through the dataframe and convert values
        for cname, series in self.df.iteritems():
            cname_base = cname.split("_")[0]
            if cname_base in cutinfo11:
                seq = cutinfo11[cname_base][1]
                self.df[cname] = series.apply(lambda x: sequences(x, seq))
                #self.df_raw[cname] = series.apply(lambda x: sequences(x, 5))

    def checkHardFailures(self):
        print "Checking hard failures"
        # read the hard failure info
        if not hasattr(self, 'hardfailures'):
            print "No info on hard failures, exiting"
            return
        else:
            outfile = open(("%s"%self.hardfailures).replace(".txt","_breakdown.txt"), 'w')
            with open(self.hardfailures) as f:
                f.readline()
                f.readline()
                f.readline()
                with open(self.input[0].replace(".cuts_all",".raw")) as raw:
                    current_chip = None
                    for line in f:
                        chipnum = line.strip()
                        print chipnum
                        foundchip = current_chip == chipnum
                        b = None
                        while not foundchip and b!='':
                            b = raw.readline()
                            if " Chip Number =   %s"%chipnum in b:
                                foundchip = True
                                current_chip = chipnum
                        nextchip = False
                        info = []
                        c = None
                        while not nextchip and c!="":
                            c = raw.readline()
                            if "Chip Number" in c:
                                nextchip = True
                                current_chip = c.strip().split()[3]
                            elif countLetters(c.strip()) > 4:
                                info.append(c.strip())
                        outfile.write("%s %s\n" % (chipnum, info[-1]))
            outfile.close()
                    
def countLetters(word):
    count = 0
    for char in word:
        if char.isalpha():
            count += 1
    return count

class DataConversion:
    def __init__(self):
        pass

    def convertHexToInt(self,val):
        # Checking for int doesn't work since it can also be a numpy.int64, etc
        if type(val) is not str:
            return val
        else:
            return int(val,16)

    def convertIntToHex(self,val):
        # Checking for int doesn't work since it can also be a numpy.int64, etc
        if type(val) is not int:
            print "Need an integer to convert!"
        else:
            return hex(val).replace("0x","").upper()

    def convertHexToIntSigned(self,val):
        val_int = val if type(val) is not str else int(val,16)
        if val_int < 32768:
            return val_int
        else:
            return val_int - 65536

    def convertIntSignedToHex(self,val):
        if type(val) is not int:
            print "Need an integer to convert!"
        else:
            if val > 0:
                return hex(val).replace("0x","").upper()
            else:
                return hex(65536 + val).replace("0x","").upper()

    def getVoltOBD(self,dac):
        return float(dac*5./65536 + 2.5)

    def getinverseVoltOBD(self,dac):
        return int(round( (dac-2.5)*65536/5 ))

    def getVoltInt(self, dac):
        return float(dac*10./65536)

    def getinverseVoltInt(self, dac):
        return int(round( dac*65536/10 ))
    
    def LookupDAC(self,dac,whichRes):
        # Needs to be updated!! These are old values from Daryl's QIE10 code
        if whichRes == 0:
            if dac > 31810:
                return -0.3185386*dac + 10195.6906
            elif dac > 31410:
                return -0.3320204*dac + 10623.6574
            elif dac > 30185: 
                return -0.3414152*dac + 10916.9529
            elif dac > 29100:
                return -0.3468817*dac + 11065.0535
            elif dac > 26200:
                return -0.3557116*dac + 11323.9317
            else:
                return -0.3684184*dac + 11641.4104
        elif whichRes == 1:
            if dac > 30635:
                return -8.2624156*dac + 261175.435
            elif dac > 29850:
                return -9.2708161*dac + 292051.72
            elif dac > 27100:
                return -10.051575*dac + 315065.982
            elif dac > 24900:
                return -10.567903*dac + 329931.967
            else:
                return -10.879485*dac + 337905.564
        else:
            return 0

def sequences(val, seqindex):
    conv = DataConversion()
    if seqindex==1:
        return conv.getVoltOBD(conv.convertHexToIntSigned(val))/10.
    elif seqindex==2:
        return conv.getVoltOBD(conv.convertHexToIntSigned(val))
    elif seqindex==3:
        return (conv.getVoltOBD(conv.convertHexToIntSigned(val)) - 2.5)/0.000181654
    elif seqindex==4:
        return (conv.getVoltOBD(conv.convertHexToIntSigned(val)) - 2.5)
    elif seqindex==5:
        return conv.convertHexToInt(val)
    elif seqindex==6:
        return conv.convertHexToInt(val)/10.
    elif seqindex==7:
        return conv.convertHexToInt(val)/100.
    elif seqindex==8:
        return conv.convertHexToInt(val)/1000.
    elif seqindex==9:
        return conv.convertHexToInt(val)/10000.
    elif seqindex==10:
        return conv.convertHexToInt(val)/2.
    elif seqindex==11:
        return conv.convertHexToInt(val)/4.
    elif seqindex==12:
        return conv.convertHexToInt(val)/16.
    elif seqindex==13:
        #print "Converting val, to lookupDAC", val, conv.LookupDAC(conv.convertHexToInt(val),0)
        return conv.LookupDAC(conv.convertHexToInt(val),0)/25.
    elif seqindex==14:
        return conv.LookupDAC(conv.convertHexToInt(val),1)/25.
    elif seqindex==15:
        return conv.convertHexToInt(val)*100.
    elif seqindex==16:
        return conv.convertHexToInt(val)/1000. - 1
    elif seqindex==17:
        return conv.convertHexToInt(val)/100. - 10

def inversesequences(val, seqindex):
    conv = DataConversion()
    if seqindex==1:
        return conv.convertIntSignedToHex(conv.getinverseVoltOBD(val*10))
    elif seqindex==2:
        return conv.convertIntSignedToHex(conv.getinverseVoltOBD(val))
    elif seqindex==3:
        return conv.convertIntSignedToHex(conv.getinverseVoltOBD( val*0.000181654 + 2.5 ))
    elif seqindex==4:
        return conv.convertIntSignedToHex(conv.getinverseVoltOBD( val + 2.5 ))
    elif seqindex==5:
        return conv.convertIntToHex(int(round(val)))
    elif seqindex==6:
        return conv.convertIntToHex(int(round(val*10)))
    elif seqindex==7:
        return conv.convertIntToHex(int(round(val*100.)))
    elif seqindex==8:
        return conv.convertIntToHex(int(round(val*1000.)))
    elif seqindex==9:
        return conv.convertIntToHex(int(round(val*10000.)))
    elif seqindex==10:
        return conv.convertIntToHex(int(round(val*2.)))
    elif seqindex==11:
        return conv.convertIntToHex(int(round(val*4.)))
    elif seqindex==12:
        return conv.convertIntToHex(int(round(val*16.)))
    elif seqindex==13:
        #print "Converting val, to lookupDAC", val, conv.LookupDAC(conv.convertHexToInt(val),0)
        return conv.LookupDAC(conv.convertHexToInt(val),0)/25.
    elif seqindex==14:
        return conv.LookupDAC(conv.convertHexToInt(val),1)/25.
    elif seqindex==15:
        return conv.convertIntToHex(int(round(val/100)))
    elif seqindex==16:
        return conv.convertIntToHex( int(round( (val+1)*1000. )))
    elif seqindex==17:
        return conv.convertIntToHex( int(round( (val+10)*100. )))

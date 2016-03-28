##################################
## Create dataframe to hold     ##
## QIE information.             ##
##                              ##
## Author: Nadja Strobbe        ##
## Initial version: 2016/03/25  ##
##################################

import pandas as pd
from collections import defaultdict
from CutInfo11 import cutinfo11

class QIEDataframe:
    def __init__(self,inputfile,fromCutsMaker=False):
        self.input = inputfile
        if not fromCutsMaker:
            self.dict = self.createDictionary(inputfile)
            self.df = self.createDataframe()
        else:
            self.dict = None
            self.df = self.readCutsMakerFile(inputfile)

    def createDictionary(self,inputfile):
        """Create dataframe from the cuts_all file"""
        entries = 0
        info = []
        temp_dict = None
        with open(inputfile) as f:
            for line in f:
                if "Chip Number" in line:
                    # write what was there
                    if temp_dict != None:
                        info.append(self.processDictionary(temp_dict))
                    # Prep for next
                    temp_dict = defaultdict(list)
                    temp_dict["ChipID"] = int(line.strip().split()[3])
                    temp_dict["i"] = entries
                    entries+=1
                else:
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

    def readCutsMakerFile(self, inputfile):
        """Read csv file from cutsmaker and convert to dataframe."""
        header = ""
        with open(inputfile) as f:
            l1 = f.readline()
            l2 = f.readline()
            header = "Status,%s" % (l2)
            header = header.replace("Cuts","ChipID")
        df = pd.read_csv(inputfile, skiprows=2, header=None, names=header.split(","))
        df.index.name = "i"
        df.drop("Status", axis=1, inplace=True)
        df["ChipID"] = df["ChipID"].apply(lambda x: int(x.split()[-1]))
        return df

    def processDataframe(self):
        """Process the dataframe to remove missing values, i.e. chips with hard failures."""
        # remove hard failures
        hardfailures = self.df.isnull().any(axis=1)
        total_hardfailures = hardfailures.sum()
        print "Found %s chips with hard failures:" % total_hardfailures
        print "\t%s" % hardfailures[hardfailures==True].index.values
        self.df = self.df.dropna(axis=0)
        
        # Loop through the dataframe and convert values
        for cname, series in self.df.iteritems():
            cname_base = cname.split("_")[0]
            if cname_base in cutinfo11:
                seq = cutinfo11[cname_base][1]
                self.df[cname] = series.apply(lambda x: sequences(x, seq))
        

class DataConversion:
    def __init__(self):
        pass

    def convertHexToInt(self,val):
        # Checking for int doesn't work since it can also be a numpy.int64, etc
        if type(val) is not str:
            return val
        else:
            return int(val,16)

    def convertHexToIntSigned(self,val):
        val_int = val if type(val) is not str else int(val,16)
        if val_int < 32768:
            return val_int
        else:
            return val_int - 65536

    def getVoltOBD(self,dac):
        return float(dac*5./65536 + 2.5)

    def getVoltInt(self, dac):
        return float(dac*10./65536)
    
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

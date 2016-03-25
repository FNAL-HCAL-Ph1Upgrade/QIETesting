import argparse
import pandas as pd
from collections import defaultdict
from CutInfo11 import cutinfo11
import plotQIE

class QIEDataframe:
    def __init__(self,inputfile):
        self.input = inputfile
        self.dict = self.createDictionary(inputfile)
        self.df = self.createDataframe()

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
                        temp_dict["i"] = entries
                        info.append(self.processDictionary(temp_dict))
                    # Prep for next
                    entries+=1
                    temp_dict = defaultdict(list)
                    temp_dict["ChipID"] = int(line.strip().split()[3])
                else:
                    # add to the dictionary
                    data = line.strip().split()
                    temp_dict[data[0]].append(data[1]) 

        # Write the last piece as well
        temp_dict["i"] = entries
        info.append(self.processDictionary(temp_dict))        
        f.close()
        return info
    
    def createDataframe(self):
        """Convert dictionary to dataframe."""
        df = pd.DataFrame(self.dict)
        df.set_index("i", inplace=True)
        return df

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
            print "column,", cname
            cname_base = cname.split("_")[0]
            if cname_base in cutinfo11:
                seq = cutinfo11[cname_base][1]
                self.df[cname] = series.apply(lambda x: sequences(x, seq))
        

class DataConversion:
    def __init__(self):
        pass

    def convertHexToInt(self,val):
        return int(val,16)

    def convertHexToIntSigned(self,val):
        val_int = int(val,16)
        if val_int < 32768:
            return val_int
        else:
            return val_int - 65536

    def getVoltOBD(self,dac):
        return float(dac*5./65536 + 2.5)

    def getVoltInt(self, dac):
        return float(dac*10./65536)
    
    def LookupDAC(self,dac,whichRes):
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
        print "Converting val, to lookupDAC", val, conv.LookupDAC(conv.convertHexToInt(val),0)
        return conv.LookupDAC(conv.convertHexToInt(val),0)/25.
    elif seqindex==14:
        return conv.LookupDAC(conv.convertHexToInt(val),1)/25.
    elif seqindex==15:
        return conv.convertHexToInt(val)*100.
    elif seqindex==16:
        return conv.convertHexToInt(val)/1000. - 1
    elif seqindex==17:
        return conv.convertHexToInt(val)/100. - 10
    
if __name__ == "__main__":

    parser = argparse.ArgumentParser(description='Process QIE test data.')
    parser.add_argument('inputfile', metavar='F',
                        help='Input file containing QIE test data. Assumes by default that you give a cuts_all file. If file ends with .csv or .pkl, it will read that and make the dataframe from it.')
    parser.add_argument('--out', dest='outfile',
                        choices=['csv','pkl'],
                        help='output type, options: csv, pkl')
    parser.add_argument('--plot', dest='plot', action='store_true',
                        default = False,
                        help='Make plots')
    parser.add_argument("--plotdir", dest="plotdir",
                        help="Directory to store plots")
    
    args = parser.parse_args()

    qiedf = None
    if ".csv" in args.inputfile:
        print "Reading from %s" % (args.inputfile)
        qiedf = pd.read_csv(args.inputfile)
    elif ".pkl" in args.inputfile:
        print "Reading from %s" % (args.inputfile)
        qiedf = pd.read_pickle(args.inputfile)
    else:
        print "Create dataframe from %s" % (args.inputfile)
        qie = QIEDataframe(args.inputfile)
        print "Processing dataframe..."
        qie.processDataframe()
        qiedf = qie.df
        
    #print qiedf

    if args.outfile == "pkl":
        qiedf.to_pickle("%s.pkl"%(args.inputfile.replace(".txt","")))
    elif args.outfile == "csv":
        qiedf.to_csv("%s.csv"%(args.inputfile.replace(".txt","")))

    # Some analysis
    #print qiedf.df[qiedf.df["10_1"]<0.35]
    #print qiedf.df[qiedf.df["107_1"]>1]
    #print qiedf.df[qiedf.df["108_1"]>1]
    #print qiedf.df[qiedf.df["109_1"]>1]
    #g1 = [11,12,16]
    #print "Printing weird rows\n", qiedf.df.loc[g1,:]
    
    # plotting
    if args.plot:
        plotter = plotQIE.PlotQIE(qiedf)
        if args.plotdir:
            plotter.setOutputDir(args.plotdir)
        plotter.plotAll()

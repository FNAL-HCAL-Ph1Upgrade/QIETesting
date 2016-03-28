##################################
## Main script to process QIE   ##
## test information.            ##
##                              ##
## Author: Nadja Strobbe        ##
## Initial version: 2016/03/25  ##
##################################

import argparse
import plotQIE, readQIE
import pandas as pd
import sys
    
if __name__ == "__main__":

    parser = argparse.ArgumentParser(description='Process QIE test data.')
    parser.add_argument('inputfile', metavar='INPUTFILE',
                        help='Input file containing QIE test data. Assumes by default that you give a cuts_all file. If file ends with .csv or .pkl, it assumes it is a properly formatted dataframe, and will read it and remake the dataframe from it.')
    parser.add_argument('--fromCutsMaker', dest='fromCutsMaker',
                        default=False, action="store_true",
                        help="Indicate that the file is the ntuple output from Al's CutsMaker. Otherwise assumes it is a cuts_all file.")
    parser.add_argument('--out', dest='outfile',
                        choices=['csv','pkl'],
                        help='output type, for easy access later, options: csv, pkl')
    parser.add_argument('--plot', dest='plot', 
                        default = False, action='store_true',
                        help='Make plots')
    parser.add_argument('--plotdir', dest="plotdir",
                        help="Directory to store plots")
    parser.add_argument('-v', '--verbose', dest="v",
                        default=False, action='store_true',
                        help="Be more verbose")
    args = parser.parse_args()


    # Step 1: Get the dataframe, either by making it, or by reading it
    qiedf = None
    if ".csv" in args.inputfile:
        print "Reading from %s" % (args.inputfile)
        qiedf = pd.read_csv(args.inputfile)
    elif ".pkl" in args.inputfile:
        print "Reading from %s" % (args.inputfile)
        qiedf = pd.read_pickle(args.inputfile)
    else:
        print "Create dataframe from %s" % (args.inputfile)
        qie = QIEDataframe(args.inputfile, args.fromCutsMaker)
        print "Processing dataframe..."
        qie.processDataframe()
        qiedf = qie.df
        
    if qiedf is None:
        print "I was unable to create a data frame... Bye!"
        sys.exit()

    if args.v:
        print qiedf

    # Step 2: Save dataframe for future use if desired
    if args.outfile == "pkl":
        new_name = "%s.pkl" % (args.inputfile.replace(".txt",""))
        print "Saving dataframe for future use to %s." % new_name
        qiedf.to_pickle(new_name)
    elif args.outfile == "csv":
        new_name = "%s.csv"%(args.inputfile.replace(".txt",""))
        print "Saving dataframe for future use to %s." % new_name
        qiedf.to_csv(new_name)

    
    # Step 3: Make a bunch of plots if desired
    # Feel free to customize at will
    if args.plot:
        plotter = plotQIE.PlotQIE(qiedf)
        if args.plotdir:
            plotter.setOutputDir(args.plotdir)
        plotter.plotAll()

    # Step 4: Do possible further analysis, e.g.
    #print qiedf[qiedf["10_1"]<0.35]
    #print qiedf[qiedf["107_1"]>1]
    #g1 = [11,12,16]
    #print "Printing weird rows\n", qiedf.loc[g1,:]

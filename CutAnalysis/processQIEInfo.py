##################################
## Main script to process QIE   ##
## test information.            ##
##                              ##
## Author: Nadja Strobbe        ##
## Initial version: 2016/03/25  ##
##################################

import sys, os
import argparse
import plotQIE, readQIE, makeCuts
import pandas as pd
    
if __name__ == "__main__":

    parser = argparse.ArgumentParser(description='Process QIE test data.')

    parser.add_argument('inputfile', metavar='INPUTFILE',
                        nargs='*',
                        help='Input file(s) containing QIE test data. Assumes by default that you give cuts_all file(s). Wildcards are accepted. If file ends with .csv or .pkl, it assumes it is a properly formatted dataframe, and will read it and remake the dataframe from it. Only one input can be given in this case.')
    parser.add_argument('--fromCutsMaker', dest='fromCutsMaker',
                        default=False, action="store_true",
                        help="Indicate that the file is the ntuple output from Al's CutsMaker. Otherwise assumes it is a cuts_all file.")
    parser.add_argument('--out', dest='outfile',
                        choices=['csv','pkl'],
                        help='output type, for easy access later, options: csv, pkl')
    parser.add_argument('-o', '--outfilename', dest='outfilename',
                        help='output filename')
    parser.add_argument('--plot', dest='plot', 
                        default = False, action='store_true',
                        help='Make plots')
    parser.add_argument('--plotdir', dest="plotdir",
                        help="Directory to store plots")
    parser.add_argument('--cuts', dest='cuts', 
                        default = False, action='store_true',
                        help='Make a cuts file based on this data, called qie11.cts by default.')
    parser.add_argument("--cutfile", dest='cutfile',
                        help="Cutfile to use when making plots; or name of cutfile when producing the cuts.")
    parser.add_argument('--hardfailures', dest='hardfailures', 
                        default = False, action='store_true',
                        help='Investigate hard failures.')
    parser.add_argument('-v', '--verbose', dest="v",
                        default=False, action='store_true',
                        help="Be more verbose")

    args = parser.parse_args()

    # -------------------------------------------------
    # Step 1: Get the dataframe, either by making it
    # from the input file(s), or by reading it in
    # -------------------------------------------------
    qiedf = None
    qie = None

    # Get the type of input file
    extensions = set([inputfile.split(".")[-1] for inputfile in args.inputfile])
    if len(extensions) != 1:
        sys.exit("Please provide only one type of input!")
    extension = list(extensions)[0]

    # Read the input
    if extension == "csv":
        if len(args.inputfile) != 1:
            sys.exit("Only one csv file supported at a time.")
        print "Reading from %s" % (args.inputfile)
        qiedf = pd.read_csv(args.inputfile[0])

    elif extension == "pkl":
        if len(args.inputfile) != 1:
            sys.exit("Only one pkl file supported at a time.")
        print "Reading from %s" % (args.inputfile)
        qiedf = pd.read_pickle(args.inputfile[0])

    else:
        print "Create dataframe from %s" % (args.inputfile)
        qie = readQIE.QIEDataframe(args.inputfile, args.fromCutsMaker)
        if args.v:
            print "Head of unprocessed dataframe"
            print qie.df.head()
        print "Processing dataframe..."
        qie.processDataframe()
        qiedf = qie.df
        if args.hardfailures:
            qie.checkHardFailures()
        
    if qiedf is None:
        print "I was unable to create a data frame... Bye!"
        sys.exit()

    if args.v:
        print qiedf


    # -------------------------------------------------
    # Step 2: Save dataframe for future use if desired
    # -------------------------------------------------
    if args.outfile == "pkl":
        new_name = "%s.pkl" % (args.outfilename if args.outfilename else args.inputfile[0].replace(".txt",""))
        print "Saving dataframe for future use to %s." % new_name
        qiedf.to_pickle(new_name)
    elif args.outfile == "csv":
        new_name = "%s.csv"%(args.outfilename if args.outfilename else args.inputfile[0].replace(".txt",""))
        print "Saving dataframe for future use to %s." % new_name
        qiedf.to_csv(new_name)


    # -------------------------------------------------
    # Step 3: Get the cut bounds if desired
    # Still requires a bit of optimizing
    # -------------------------------------------------
    if args.cuts:
        if args.cutfile:
            makeCuts.makeCutsFile(qiedf, cutfilename=args.cutfile)
        else:
            makeCuts.makeCutsFile(qiedf)
        
    # -------------------------------------------------
    # Step 4: Make a bunch of plots if desired
    # Feel free to customize at will
    # -------------------------------------------------
    if args.plot:
        plotter = plotQIE.PlotQIE(qiedf)
        # Set output directory
        if args.plotdir:
            plotter.setOutputDir(args.plotdir)
        # Set cutfile
        if args.cutfile:
            plotter.setCutFile(args.cutfile)
        elif os.path.isfile("qie11.cts"):
            plotter.setCutFile("qie11.cts")
        plotter.plotAll()
        #plotter.plotAllZoomed()

    # -------------------------------------------------
    # Step 5: Do possible further analysis, e.g.
    # -------------------------------------------------
    #print qiedf[qiedf["10_1"]<0.35]
    #print qiedf[qiedf["107_1"]>1]
    #g1 = [11,12,16]
    #print "Printing weird rows\n", qiedf.loc[g1,:]

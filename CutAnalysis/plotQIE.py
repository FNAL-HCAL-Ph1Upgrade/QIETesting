##################################
## Plotter for qie dataframes   ##
##                              ##
## Author: Nadja Strobbe        ##
## Initial version: 2016/03/25  ##
##################################

from CutInfo11 import cutinfo11
import os
import matplotlib.pyplot as plt
import matplotlib

class PlotQIE:
    def __init__(self, df):
        # Store the dataframe for easy access
        self.qie = df
        # Default output directory
        self.outputdir = "./"
        # some basic formatting options
        self.labelsize = 20
        self.ticklabelsize = 16
        self.colorlist = ['mediumpurple','coral','lightseagreen',
                          'orange','lightslategrey','palevioletred',
                          'yellowgreen', 'skyblue', 'gold']

    def setOutputDir(self, outputdir):
        """Set the directory where the plot output will go. Create it if it doesn't exist."""
        self.outputdir = outputdir
        if not os.path.isdir(outputdir):
            os.makedirs(outputdir)

    def plotVarGroup(self, var, series, groups=None, labels=None, postfix=""):
        """Make plot for a given variable, and divide the chips in provided groups."""
        # split the variable name
        varname = var.split("_")

        # create the separate dataframes from the provided groups
        # Define some labels if we have groups
        dfs = None
        if groups:
            dfs = [series.loc[g] for g in groups]
            if not labels or len(labels) != len(groups):
                labels = ["Group %s" % (i+1) for i in xrange(len(groups)-1)]
                labels.append("Bulk")
        else:
            dfs = [series]


        # Get right number of colors, and reverse them so that mediumpurple is 
        # used for the bulk of the chips (assumed to be the last group)
        colors = (self.colorlist[:len(dfs)])
        colors.reverse()
        
        # Make the histogram
        # Get the preferred binning and check whether all values fall within that range 
        if varname[0] in cutinfo11:
            nbins = cutinfo11[varname[0]][2]
            xmin = cutinfo11[varname[0]][3]
            xmax = cutinfo11[varname[0]][4]
            series_min = series.min()
            series_max = series.max()
            if series_min > xmin and series_max < xmax:
                ax = plt.hist(dfs, bins=nbins, range=[xmin, xmax], stacked=True, 
                              color=colors, label=labels)
            else:
                ax = plt.hist(dfs, bins=nbins, stacked=True, 
                              color=colors, label=labels)
        else:
            ax = plt.hist(dfs, bins=20, stacked=True, 
                          color=colors, label=labels)

        # Set the axis titles
        if varname[0] in cutinfo11:
            if len(varname) == 1:
                plt.xlabel(cutinfo11[varname[0]][0], 
                           fontsize=self.labelsize)
            else:
                plt.xlabel("%s ; %s" % (cutinfo11[varname[0]][0], varname[1]), 
                           fontsize=self.labelsize)
        else:
            plt.xlabel(varname[0], 
                       fontsize=self.labelsize)
        plt.ylabel("Number of chips", fontsize=self.labelsize)

        # set margins and format axis labels
        x0, x1, y0, y1 = plt.axis()
        plt.axis((x0, x1,
                  y0, y1*(1+0.17)))
        ax = plt.gca()
        ax.tick_params(labelsize=self.ticklabelsize)
        plt.gcf().subplots_adjust(bottom=0.12)

        # Add mean and std info
        mean = self.qie[var].mean()
        std = self.qie[var].std()
        plt.figtext(0.35, 0.8,
                    "Mean: %.3g\nStd: %.3g"%(mean, std),
                    fontsize=self.ticklabelsize)

        # Add legend if we have labels
        if labels:
            plt.legend(loc='best')

        # Save figure
        plt.savefig("%s/%s%s.pdf" % (self.outputdir, var, postfix))
        plt.clf()

    def plotAll(self):
        # Make some groups (these are series of True and False)
        g1 = (self.qie["107_1"]>1) | (self.qie["108_1"]>1) | (self.qie["109_1"]>1)
        g2 = self.qie["10_1"]<0.35
        g3 = ~ (g1 | g2)

        # Make the figures
        self.fig = plt.figure()
        for cname, series in self.qie.iteritems():
            print "Making plot for %s" % (cname)
            self.plotVarGroup(cname,series,[g1,g2,g3],postfix="_grouped")
            #self.plotVarGroup(cname,series)

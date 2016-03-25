# Plotter for qie dataframes

from CutInfo11 import cutinfo11
import os
import matplotlib.pyplot as plt
import matplotlib
#matplotlib.style.use('ggplot')

class PlotQIE:
    def __init__(self, df):
        self.qie = df
        self.outputdir = "./"
        # some basic formatting options
        self.labelsize = 20
        self.ticklabelsize = 16
        self.colorlist = ['mediumpurple','coral','lightseagreen',
                          'orange','lightslategrey','palevioletred']

    def setOutputDir(self, outputdir):
        self.outputdir = outputdir
        if not os.path.isdir(outputdir):
            os.makedirs(outputdir)

    def plotVar(self, var):
        ax = self.qie[var].hist(bins=20, color='mediumpurple')
        #ax = self.qie[var].hist(bins=20, stacked=True, color='mediumpurple')
        mean = self.qie[var].mean()
        std = self.qie[var].std()
        varname = var.split("_")
        if varname[0] in cutinfo11:
            if len(varname) == 1:
                plt.xlabel(cutinfo11[varname[0]][0])
            else:
                plt.xlabel(cutinfo11[varname[0]][0] + " ; " + varname[1])
        else:
            plt.xlabel(varname[0])
        plt.ylabel("Normalized count")
        # set margins
        x0, x1, y0, y1 = plt.axis()
        plt.axis((x0,
                  x1,
                  y0,
                  y1*(1+0.15)))
        plt.figtext(0.15,0.82,"Mean: %.3g\nStd: %.3g"%(mean, std))
        plt.savefig("%s/%s.pdf" % (self.outputdir, var))
        plt.clf()

    def plotVarGroup(self, var, series, groups):
        # split the variable name
        varname = var.split("_")

        # create the groups
        #g1 = [11,12,16]
        #g2 = [15,20]
        #g3 = [i for i in self.qie.index.values if ((not i in g1) and (not i in g2))]
        #df1 = series.loc[g1]
        #df2 = series.loc[g2]
        #df3 = series.loc[g3]
        #dfs = [df1,df2,df3]

        dfs = [series.loc[g] for g in groups]
        # Get right number of colors, and reverse them so that mediumpurple is used for the bulk of the chips
        colors = (self.colorlist[:len(dfs)])
        colors.reverse()
        labels = ["Group 1", "Group 2", "Bulk"]

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
                plt.xlabel(cutinfo11[varname[0]][0], fontsize=self.labelsize)
            else:
                plt.xlabel(cutinfo11[varname[0]][0] + " ; " + varname[1], fontsize=self.labelsize)
        else:
            plt.xlabel(varname[0], fontsize=self.labelsize)
        plt.ylabel("Normalized count", fontsize=self.labelsize)

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

        # Add legend
        plt.legend(loc='best')

        # Save figure
        plt.savefig("%s/%s_grouped.pdf" % (self.outputdir, var))
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
            #self.plotVar(cname)
            self.plotVarGroup(cname,series,[g1,g2,g3])

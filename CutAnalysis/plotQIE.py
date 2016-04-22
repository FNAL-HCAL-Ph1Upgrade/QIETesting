##################################
## Plotter for qie dataframes   ##
##                              ##
## Author: Nadja Strobbe        ##
## Initial version: 2016/03/25  ##
##################################

from CutInfo11 import cutinfo11, to_combine, cut_groups
import readQIE
import os, sys
import matplotlib.pyplot as plt
import matplotlib
from collections import defaultdict

class PlotQIE:
    def __init__(self, df):
        # Store the dataframe for easy access
        self.qie = df
        # Default output directory
        self.outputdir = "./"
        self.cutfile = None
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

    def setCutFile(self, cutfile):
        """Set the cutfile location, and store it into a dictionary for further use."""
        self.cutfile = cutfile
        self.cuts = {}
        with open(cutfile) as f:
            for l in f:
                llist = l.strip().split()
                cut = str(int(llist[0])).zfill(2)
                seq = cutinfo11[cut][1]
                self.cuts[cut] = [readQIE.sequences(elem,seq) for elem in llist[1:]]

    def makeGroupsFromCutFile(self):
        """Use the cuts file to define groups of chips that don't pass certain sets of cuts.
Returns a tuple containing the list of groups and the list of labels.
Also prints a table with yields"""
        if self.cutfile == None:
            print "Cannot make groups without a cuts file"
            return ([],[])
        else:
            groups = []
            labels = []
            yields = []
            all_cols = self.qie.columns.values
            # For each predefined group
            for grouplist in cut_groups:
                labels.append(grouplist[0])
                g = None
                # For each cut in that group
                for cut in grouplist[1]:
                    # Get min and max values (TODO: handle marginal cuts)
                    cut_min = self.cuts[cut][0]
                    cut_max = self.cuts[cut][1]
                    # For each df column corresponding to that cut (sometimes more than one measurement)
                    for col in all_cols:
                        if col.split("_")[0] == cut:
                            g_tmp = (self.qie[col] < cut_min) | (self.qie[col] > cut_max)
                            if 'NoneType' in str(type(g)) :
                                g = g_tmp
                            else: 
                                g = g | g_tmp
                # Make exclusive groups
                if len(groups) > 0:
                    g = g & (self.NotGroup(groups))
                groups.append(g)
                yields.append(g.sum())
            # Make final group containing all other chips
            groups.append(self.NotGroup(groups))
            labels.append("Good")
            yields.append(groups[-1].sum())
            self.makeYieldsTable(yields, labels)
            return (groups, labels)

    def NotGroup(self,groups):
        temp = groups[0]
        for g in groups[1:]:
            temp = temp | g
        return ( ~ temp)

    def makeYieldsTable(self, yields, labels):
        with open("yields.tex", 'w') as f:
            f.write("\\begin{tabular}{l|c|c|c}\n")
            f.write("Status & Number of chips & Percentage & Cumulative Percentage \\\\ \\hline \n")
            total_yield = float(sum(yields))
            for i in xrange(len(yields)):
                print sum(yields[:i+1])/total_yield*100
                f.write("%s & %s & %.1f \\%% & %.1f \\%% \\\\ \n" % (labels[i],
                                                              yields[i],
                                                              yields[i]/total_yield*100,
                                                              sum(yields[:i+1])/total_yield*100))
            f.write("\\hline \nTotal & %s & 100\\%% & 100\\%% \\\\ \n" % int(total_yield))
            f.write("\\end{tabular}\n")
        print "Wrote yields table"
        sys.exit()

    def plotVarGroup(self, var, series, groups=None, labels=None, postfix="", logy=True, fixedrange=False):
        """Make plot for a given variable, and divide the chips in provided groups.
'series' is a Series, i.e. a column"""
        # split the variable name
        varname = var.split("_")

        # create the separate dataframes to plot from the provided groups
        # Define some labels if we have groups but no provided labels
        dfs = None
        if groups:
            dfs = [series.loc[g] for g in groups]
            if not labels or len(labels) != len(groups):
                labels = ["Group %s" % (i+1) for i in xrange(len(groups)-1)]
                labels.append("Rest")
        else:
            dfs = [series]

        # Get right number of colors, and reverse them so that mediumpurple is 
        # used for the bulk of the chips (assumed to be the last group)
        colors = (self.colorlist[:len(dfs)])
        colors.reverse()
        
        # Make the histogram
        # Get the preferred binning and check whether all values fall within that range 
        # If not, let pyplot pick the range. 
        if varname[0] in cutinfo11:
            nbins = cutinfo11[varname[0]][2]
            xmin = cutinfo11[varname[0]][3]
            xmax = cutinfo11[varname[0]][4]
            series_min = series.min()
            series_max = series.max()
            if fixedrange or (series_min > xmin and series_max < xmax):
                # Use the predefined ranges
                print fixedrange, xmin, xmax
                ax = plt.hist(dfs, bins=nbins, range=[xmin, xmax], stacked=True, 
                              color=colors, label=labels, log=logy)
            else:
                print "auto scaling"
                # Only use the number of bins from cutinfo11
                ax = plt.hist(dfs, bins=nbins, stacked=True, 
                              color=colors, label=labels, log=logy)
        else:
            # No info available, let pyplot deal with things, but use 20 bins
            ax = plt.hist(dfs, bins=20, stacked=True, 
                          color=colors, label=labels, log=logy)

        # Set the axis titles (use cutinfo11 if available)
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
        if logy:
            plt.axis((x0, x1,
                      0.5, y1*10))
        else:
            plt.axis((x0, x1,
                      0.5, y1*(1+0.2)))
        ax = plt.gca()
        ax.tick_params(labelsize=self.ticklabelsize)
        plt.gcf().subplots_adjust(bottom=0.12)

        # Add mean and std info
        mean = series.mean()
        std = series.std()
        plt.figtext(0.4, 0.92,
                    "Mean: %.3g      Std/Mean: %.3g\nStd: %.3g"%(mean, std/mean, std),
                    fontsize=self.ticklabelsize)

        # Add cut lines if we have info
        if self.cutfile != None and varname[0] in cutinfo11:
            plt.axvline(x=self.cuts[varname[0]][2], linestyle='dashed', linewidth=2, color='grey')
            plt.axvline(x=self.cuts[varname[0]][3], linestyle='dashed', linewidth=2, color='grey')
            plt.axvline(x=self.cuts[varname[0]][0], linestyle='solid', linewidth=2, color='dimgrey')
            plt.axvline(x=self.cuts[varname[0]][1], linestyle='solid', linewidth=2, color='dimgrey')

        # Add legend if we have labels
        if labels:
            plt.legend(loc='best', ncol=2)

        # Save figure
        plt.savefig("%s/%s%s.pdf" % (self.outputdir, var, postfix))
        plt.clf()

    def plotMultipleVars(self, vars, series, groups=None, labels=None, postfix="",logy=True, fixedrange=False):
        """Make plot for the given variables, and divide the chips in provided groups.
'series' is actually a Dataframe in this case, which will be stacked."""
        # split the variable names, we'll use the first one for naming purposes
        varnames = [var.split("_") for var in vars]

        # create the separate dataframes from the provided groups
        # Define some labels if we have groups and no provided labels
        # Stack all the variables we want to plot in one histogram
        dfs = None
        if groups:
            dfs = [series.loc[g,:].stack() for g in groups]
            if not labels or len(labels) != len(groups):
                labels = ["Group %s" % (i+1) for i in xrange(len(groups)-1)]
                labels.append("Bulk")
        else:
            dfs = [series.stack()]


        # Get right number of colors, and reverse them so that mediumpurple is 
        # used for the bulk of the chips (assumed to be the last group)
        colors = (self.colorlist[:len(dfs)])
        colors.reverse()
        
        # Make the histogram
        # Get the preferred binning and check whether all values fall within that range 
        if varnames[0][0] in cutinfo11:
            nbins = cutinfo11[varnames[0][0]][2]
            xmin = cutinfo11[varnames[0][0]][3]
            xmax = cutinfo11[varnames[0][0]][4]
            series_min = series.min().min()
            series_max = series.max().max()
            if fixedrange or (series_min > xmin and series_max < xmax):
                ax = plt.hist(dfs, bins=nbins, range=[xmin, xmax], stacked=True, 
                              color=colors, label=labels, log=logy)
            else:
                ax = plt.hist(dfs, bins=nbins, stacked=True, 
                              color=colors, label=labels, log=logy)
        else:
            ax = plt.hist(dfs, bins=20, stacked=True, 
                          color=colors, label=labels, log=logy)

        # Set the axis titles
        if varnames[0][0] in cutinfo11:
            if len(varnames[0]) == 1:
                plt.xlabel(cutinfo11[varnames[0][0]][0], 
                           fontsize=self.labelsize)
            else:
                plt.xlabel("%s ; %s" % (cutinfo11[varnames[0][0]][0], varnames[0][1]), 
                           fontsize=self.labelsize)
        else:
            plt.xlabel(varnames[0][0], 
                       fontsize=self.labelsize)
        plt.ylabel("Number of measurements", fontsize=self.labelsize)

        # set margins and format axis labels
        x0, x1, y0, y1 = plt.axis()
        if logy:
            plt.axis((x0, x1,
                      0.5, y1*10))
        else:
            plt.axis((x0, x1,
                      0.5, y1*(1+0.2)))
        ax = plt.gca()
        ax.tick_params(labelsize=self.ticklabelsize)
        plt.gcf().subplots_adjust(bottom=0.12)

        # Add mean and std info
        mean = series.stack().mean()
        std = series.stack().std()
        plt.figtext(0.4, 0.92,
                    "Mean: %.3g      Std/Mean: %.3g\nStd: %.3g"%(mean, std/mean, std),
                    fontsize=self.ticklabelsize)

        # Add cut lines if we have info
        if self.cutfile != None and varnames[0][0] in cutinfo11:
            plt.axvline(x=self.cuts[varnames[0][0]][2], linestyle='dashed', linewidth=2, color='grey')
            plt.axvline(x=self.cuts[varnames[0][0]][3], linestyle='dashed', linewidth=2, color='grey')
            plt.axvline(x=self.cuts[varnames[0][0]][0], linestyle='solid', linewidth=2, color='dimgrey')
            plt.axvline(x=self.cuts[varnames[0][0]][1], linestyle='solid', linewidth=2, color='dimgrey')

        # Add legend if we have labels
        if labels:
            plt.legend(loc='best', ncol=2)

        # Save figure
        plt.savefig("%s/%s%s.pdf" % (self.outputdir, varnames[0][0], postfix))
        plt.clf()

    def plotAll(self):
        # Make some groups (these are series of True and False that select the rows to go in that group)
        # Also make the groups exclusive by excluding the previous ones, eg (~ g1)
        g1 = (self.qie["01_1"]>0.075) | (self.qie["02_1"] < 0.026) | (self.qie["03_1"] > 0.072) | (self.qie["04_1"] < 0.026) # current
        g2 = ((self.qie["107_1"]>1) | (self.qie["108_1"]>1) | (self.qie["109_1"]>1)) & (~ g1) # range transition 
        g3 = ((self.qie["138_1"] < 97) | (self.qie["139_1"] < 85) | (self.qie["140_1"]<79) | (self.qie["141_1"]<66) | (self.qie["142_1"]<55) | (self.qie["143_1"]<48)) & (~ g2) # transfer gain
        g4 = ((self.qie["150_1"]>10) | (self.qie["151_1"]<5) | (self.qie["151_1"]>10) | (self.qie["152_1"]<10) | (self.qie["152_1"]>20) | (self.qie["153_1"] < 20) | (self.qie["153_1"] > 30) | (self.qie["166_1"] > 300)) & (~ g3) # Slope/intercept issue
        g5 = ((self.qie["186_1"] > 10) | (self.qie["187_1"] > 1.5)) & (~ g4) # Pedestal issue
        g6 = ((self.qie["10_1"] < 0.35) | (self.qie["93_1"] < 1.39) | (self.qie["93_1"] > 1.46)) & (~ g5) # Other
        g7 = ~ (g1 | g2 | g3 | g4 | g5 | g6) # everything else

        # Treat multiple measurement cuts in a different way
        to_combine_dict = defaultdict(list)

        # Prepare the figures environment
        self.fig = plt.figure()

        # groups from the cutfile
        mygroups, mylabels = self.makeGroupsFromCutFile()
        
        # Make plot for the individual cuts, and prepare dictionary for the others
        for cname, series in self.qie.iteritems():
            if cname.split("_")[0] in to_combine:
                to_combine_dict[cname.split("_")[0]].append(cname)
            else:
                print "Making plot for %s" % (cname)
                self.plotVarGroup(cname,series,mygroups,labels=mylabels)
                print "Making plot for %s in fixed range mode" % (cname)
                self.plotVarGroup(cname,series,mygroups,labels=mylabels,postfix="_fixedrange",fixedrange=True)
#                                   [g1, g2, g3, g4, g5, g6, g7],
#                                   labels=["Current issue",
#                                           "Range transition",
#                                           "Transfer gain",
#                                           "Slopes/Intercept",
#                                           "Pedestal",
#                                           "Other",
#                                          "Nominal"]
                
        # Make plot for the others
        # Use plotting function that takes in dataframe
        for k,v in to_combine_dict.iteritems():
            print "Making plot for %s" % (k)
            self.plotMultipleVars(v, self.qie[v],mygroups,labels=mylabels)
            print "Making plot for %s in fixed range mode" % (k)
            self.plotMultipleVars(v, self.qie[v],mygroups,labels=mylabels,postfix="_fixedrange",fixedrange=True)
#                                   [g1, g2, g3, g4, g5, g6, g7],
#                                   labels=["Current issue",
#                                           "Range transition",
#                                           "Transfer gain",
#                                           "Slopes/Intercept",
#                                           "Pedestal",
#                                           "Other",
#                                           "Nominal"]
#                                   )

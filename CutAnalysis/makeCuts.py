import pandas as pd
import numpy as np
from sklearn.cluster import KMeans
import sys
from CutInfo11 import cutinfo11, to_combine_cuts
import readQIE

def get2cluster(data):
    mykmeans = KMeans(n_clusters=2, init=np.array([[np.min(data)],[np.max(data)]]))
    mykmeans.fit(data)
    #print mykmeans.cluster_centers_
    mylabels = mykmeans.predict(data)
    return mylabels

def get3cluster(data):
    mykmeans = KMeans(n_clusters=3, init=np.array([[np.min(data)],[np.mean(data)],[np.max(data)]]))
    mykmeans.fit(data)
    #print mykmeans.cluster_centers_
    mylabels = mykmeans.predict(data)
    #print mylabels
    #print "0:", sum(mylabels == 0)
    #print "1:", sum(mylabels == 1)
    #print "2:", sum(mylabels == 2)
    return mylabels

def pruneclusters(data, thresh=0.02, max_iterations=5):
    print "Getting three clusters"
    # first get rid of a potential third cluster
    three_labels = get3cluster(data)
    sum_three_labels = [sum(three_labels == i) for i in range(3)]
    sum_smallest_three_label = min(sum_three_labels)
    smallest_three_label = sum_three_labels.index(sum_smallest_three_label)
    if sum_smallest_three_label > 0.01*len(data):
        print "Warning: Would remove more than 1% of data as initial outlier.. skipping"
    else:
        data = data[three_labels != smallest_three_label]

    small_cluster_size = 0
    small_cluster_labels = np.array([False]*data.shape[0])

    iteration = 0
    while float(small_cluster_size)/data.shape[0] < thresh:
        if iteration >= max_iterations:
            break

        data = data[~small_cluster_labels]

        list_of_labels = get2cluster(data)
        sum_labels = sum(list_of_labels)
        small_label = int(sum_labels < data.shape[0] - sum_labels)
        small_cluster_labels = list_of_labels == small_label
        small_cluster_size = sum(small_cluster_labels)

        iteration += 1

    return data

def getcorebounds(data, padding=0.1, **kwargs):
    core = pruneclusters(data, **kwargs)
    #print core[core>0.065]
    core_min = core.min()
    core_max = core.max()
    #width = (core_max - core_min) if (core_max - core_min > 0) else core_min*0.01
    std = core.std()
    extra = std if std > 0 else (core_min*0.002 if core_min > 0 else 1) #padding * width 
    print "min = %s, max = %s, extra = %s" % (core_min, core_max, extra)
    return (core_min-extra if (core_min-extra > 0 or core_min < 0) else 0, core_max+extra)

def checkBounds(bound):
    if int(bound,16) > 65535:
        return "FFFF"
    elif int(bound,16) < 0:
        return "0000"
    else:
        return bound

def makeCutsFile(dataframe, **kwargs):
    cutbounds = {}
    for cut, info in cutinfo11.iteritems():
        print cut
        seqnr = info[1]
        core_min, core_max = (0,0)
        if cut in to_combine_cuts:
            # stack data
            all_cols = list(dataframe.columns.values)
            cols = []
            for col in all_cols:
                if "%s_" % cut in col:
                    cols.append(col)
            new_df = dataframe[cols].stack()
            core_min, core_max = getcorebounds(new_df.as_matrix().reshape((new_df.shape[0],1)))
        else:
            core_min, core_max = getcorebounds(dataframe["%s_1" % cut].as_matrix().reshape((dataframe.shape[0],1)))

        cutbounds[cut] = [readQIE.inversesequences(core_min, seqnr),
                          readQIE.inversesequences(core_max, seqnr),
                          readQIE.inversesequences(core_min, seqnr),
                          readQIE.inversesequences(core_max, seqnr),
                          ]

        # Handle fixed bounds
        if info[5] != None:
            if info[5][0] != None:
                cutbounds[cut][0] = readQIE.inversesequences(info[5][0], seqnr)
                cutbounds[cut][2] = readQIE.inversesequences(info[5][0], seqnr)
            if info[5][1] != None:
                cutbounds[cut][1] = readQIE.inversesequences(info[5][1], seqnr)
                cutbounds[cut][3] = readQIE.inversesequences(info[5][1], seqnr)

        cutbounds[cut] = [checkBounds(b) for b in cutbounds[cut]]

    writeCutsFile(cutbounds, **kwargs)

def writeCutsFile(cutbounds, cutfilename="qie11.cts"):
    cutfile = open(cutfilename,'w')
    # Format is " 1 low high marg-low marg-high" with all values in hex
    for cut, bounds in cutbounds.iteritems():
        cutfile.write("  %s %s %s %s %s\n" % (cut.zfill(3), bounds[0].zfill(4), bounds[1].zfill(4), bounds[2].zfill(4), bounds[3].zfill(4)))
    cutfile.close()


# trying out different clustering mechanism
from sklearn.cluster import DBSCAN
import scipy.stats as stats
def doDBSCAN(data):
    #mykmeans = KMeans(n_clusters=2)
    myDBSCAN = DBSCAN(eps=stats.variation(data)*0.3, min_samples=len(data)*0.5).fit(data)
    n_clusters_ = len(set(myDBSCAN.labels_)) - (1 if -1 in myDBSCAN.labels_ else 0)
    print myDBSCAN.labels_
    print sum(myDBSCAN.labels_)
    print "nclusters:", n_clusters_



if __name__ == "__main__":
    

    # read file
    df = pd.read_pickle("/Users/nstrobbe/Work/HCALPhase1/QIETesting/QIE11_2016/vWafer02/qie11.dat.cuts_all.pkl")
    
    #columns = ["150_2","150_3"]
    c = sys.argv[1]
    #for c in columns:
    df2 =  df[c].as_matrix().reshape((df.shape[0],1))
    getcorebounds(df2)
    
    #doDBSCAN(df2)
    #get3cluster(df2)

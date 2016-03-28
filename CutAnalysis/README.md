## How to process a batch of chips: 

### Using the new python-based scripts

The four relevant files are:
```
readQIE.py
plotQIE.py
CutInfo11.py
processQIEInfo.py
```

`processQIEInfo.py` is the main script which can be called with various options (check `-h` option)
to perform tasks such as reading in the data (either in cuts_all format, or .txt format from the
cutsmaker) and making plots. 

`readQIE.py` contains the code that reads the raw data and converts it to a pandas dataframe. 
These dataframes are then processed. First, chips with missing data, i.e. with hard failures,  are removed.
Then, the raw integer information is converted into physical units such as Volts and Amperes. 
This file also contains code that allows to write out this processed dataframe in a .csv or .pkl file.
This allows you to skip the dataframe creation step if you want to do more analysis later.

`plotQIE.py` is the plotting code. For now it will create a plot for each variable. 
This file can be easily adjusted to make more fancy plots. Since it is making about 1000 plots, it 
takes a little while to complete.

`CutInfo11.py` contains some information for each type of measurement in the form of a list. 
The entries are the following:
 - a label that is used in the plotting code for the x axis title.
 - the conversion method from hex (or int if using the file from the cutsmaker) to physical units (see sequences() method in `readQIE.py`)
 - the preferred histogram binning and range. The plotter will use this range if all
chips fall inside. If not, it will autoselect a range.


Example usage:
```
# Display help 
python processQIEInfo.py -h

# Process cuts_all file and save dataframe as pickle file for further use
python processQIEInfo.py qie11.dat.cuts_all --out pkl

# Process cuts_all file, save dataframe as csv file for further use, and make plots
python processQIEInfo.py qie11.dat.cuts_all --out csv --plot --plotdir mytest

# Make plots starting from prior saved pkl file
python processQIEInfo.py qie11.dat.cuts_all.pkl --plot --plotdir mytest

# Process txt file from Al's CutsMaker, save as pkl file, and make plots
python processQIEInfo.py QIE11.output.txt --fromCutsMaker --out pkl --plot --plotdir mytest
```


### Using Daryl's code

1. Get the cuts_all file and load it into Al's CutsMaker
2. Get the output tuple (comma-separated txt file)
3. Make a ROOT TTree out of this file:

    ```
    root -l
    .L makeQIEtree.C+
    MakeTupleFromCutsMaker("QIE11.output.txt", "treefile.root")
    ```
4. Make plots to find cuts -- plotter still to be moved from Daryl's area
5. Create a cuts file

     1. Use Al's CutsMaker to make the cuts file
     2. Load cuts file into Al's CutsMaker so that it can be communicated to the robot

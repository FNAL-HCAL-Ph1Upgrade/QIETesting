## How to process a batch of chips: 

1. Get the cuts_all file and load it into Al's CutsMaker
2. Get the output tuple (comma-separated txt file)
3. Make a ROOT TTree out of this file:
```
root -l
.L makeQIEtree.C+
MakeTupleFromCutsMaker("QIE11.output.txt", "treefile.root")
```
4. Make plots to find cuts
5. Create a cuts file
6. Load cuts file into Al's CutsMaker so that it can be communicated to the robot

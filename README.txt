# Sorting-Algorithms-Comparison

Please compile the provided 'main.cpp' file with the provided makefile.
Then, run the program with ./Sorting.

The program will generate a file containing unsorted data, "UnsortedData.txt".
For the purpose of this program, the file will contain 10,000 random integers from 0 to 1000.
This file will be used as the data to-be-sorted for all of the following sorting algorithms run. 
If the program is run again, the same file will be used and will NOT be regenerated. 
To regenerate a new unsorted data file, simply delete "UnsortedData.txt" and run the program again.

Within the menu of the program, each included algorithm can be run individually or all algorithms can be run. 
In the event of testing all available algorithms, a time comparison table will be provided after each algorithm 
has succeeded. Whenever an algorithm has completed it's sort, the resulting 'Sorted' data is then checked for 
proper sorting and integrity through the means of two metrics:

#1 - Each successive element in the final sorted array is no larger than the element that preceeds it
#2 - Each element within the original unsorted array is included in the new sorted array
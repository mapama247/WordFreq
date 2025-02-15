# WordFreq
Program that counts the word frequency of a text file.

## Program
First of all, the program scans the text file looking for potential words (strings separated by at least one blanck character). Then, all potential words are "cleaned" and are determined to be either valid or invalid.

A valid word has the following properties:

- The word contains only letters (lower case or upper case) and hyphens.
- Hyphens occur only inside the word (not first or last) and non-consecutive.
- The word contains at least three characters (shorter words are not worthwhile to predict).

Valid words are converted to lower case and added to the word list and statistics.
Invalid words are simply ignored.

## Input
### First command line parameter
Name of plain text file.
### Second command line parameter
Three possible ways of displaying the final output:

-a All valid words are printed in alphabetic increasing order followed by the frequency of that word.

-f All valid words are printed in decreasing frequency.

-o Print all valid words in the same order they appear in the original file.
An additional argument (N) is given to specify the maximum length of each sentence.

## Execution Examples
```
a.out example.html -a
a.out example.html -f
a.out example.html -o 14
```

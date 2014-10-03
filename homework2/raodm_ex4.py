#!/usr/bin/python

import sys

def toggle_char(str, lower_char):
    upper_char = lower_char.upper();
    result = ""
    for char in str:
        if (char == lower_char):
            result += upper_char
        elif (char == upper_char):
            result += lower_char
        else:
            result += char
            
    return result

# The main part of the script
if (len(sys.argv) < 2):
    print "Specify <File> <toggle_char> ..."
    sys.exit(1)

# Read the file     
with open (sys.argv[1], "r") as dataFile:
    data = dataFile.read()

# Replace each character specifed
curr = 2
while (curr < len(sys.argv)):
    data = toggle_char(data, sys.argv[curr])
    curr = curr + 1

# Print the modifed data on screen.
print data

# end of script

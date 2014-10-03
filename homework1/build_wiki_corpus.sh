#!/bin/bash

# Check to ensure we have a corpus file specified.
if [ $# -ne 1 ]; then
    echo "Specify the corpus file with list of Wikipedia URLs to download"
    exit 1
fi

# Alias to make the script a bit more readable
corpusFile=$1

# Read line-by-line from wiki_corpus.txt and download the data from
# Wikipedia
while read outFile; 
do
    url="http://en.wikipedia.org/wiki/${outFile%.txt}"
    lynx -nolist -dump -notitle $url > $outFile
    if [ $? -ne 0 ]; then
        echo "Unable to download data for $outFile"
    fi
done < $corpusFile

# End of script

#!/bin/sh

if [ $# -eq 1 ]
  then
    if [ `uname -p` = "sparc" ]
    then
      echo "Computing Simple Precedence Relations for $1.  
Output will be a file in your current directory called $1.out
Please wait....
"
      /home/grossman/faculty/analyzer/analyzer.2 < $1 > $1.out
      tail -23 $1.out
    elif [ `uname -p` = "x86_64" ]
    then
	echo "Computing Simple Precedence Relations for $1.  
Output will be a file in your current directory called $1.out
Please wait....
"
      #changeme! Use the right executable name!
      /home/grossman/faculty/analyzer/analyzer.2.centos < $1 > $1.out
      tail -23 $1.out
    fi
  else
    echo "
Correct usage is   analyzer.sh <grammar file name>
"
fi

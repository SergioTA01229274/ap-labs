#!/bin/bash
file_found=$(find FCC_A01229274.py)

if [[ "${file_found}" ]]
then 
	echo "Insert the word to analyze: "
	read word
	python FCC_A01229274.py "${word}"
	echo "Script finished"
else
	echo "Python file not found :("
fi

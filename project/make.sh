#!/bin/bash

BASEDIR=`pwd`

function getdir()
{
	local loc_dirs="`ls -F | grep "/"`"
	files=`ls *.chpl 2> /dev/null`
	curdir=`pwd`
	for srcfile in $files
	do
		src_files="$src_files $curdir/$srcfile"
	done
	for subdir in $loc_dirs
	do
		cd $subdir
		getdir
		cd ..
	done
}	

getdir
#echo $src_files

echo "chpl --baseline --no-optimize $src_files"
chpl --baseline --no-optimize $src_files 


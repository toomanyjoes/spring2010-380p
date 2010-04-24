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

function makeClean()
{
        rm *~ 2> /dev/null
	local loc_dirs="`ls -F | grep "/"`"
	for subdir in $loc_dirs
	do
		cd $subdir
		makeClean
		cd ..
	done
}

if [[ $1 == "parallel" ]]
then
	getdir
	echo "chpl --fast -o flame_parallel $src_files"
	echo
	chpl --fast -o flame_parallel $src_files
elif [[ $1 == "serial" ]]
then
	getdir
	echo "chpl --fast --serial -o flame_serial $src_files"
	echo
	chpl --fast --serial -o flame_serial $src_files
else
	makeClean
fi


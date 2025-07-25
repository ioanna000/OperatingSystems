#!/bin/sh
../src/bbfs rootdir/ mountdir/
ls
echo "ABOUT TO COPY FILES IN MOUNTDIR"
cp test.txt mountdir/
cp test1.txt mountdir/
ls -l rootdir/
ls -l rootdir/storage/
ls -l mountdir/
echo "ABOUT TO REMOVE TEST.TXT"
rm mountdir/test.txt 
echo "REMOVED AND NOW LS"
ls -l rootdir/
ls -l rootdir/storage/
ls -l mountdir/
fusermount -u mountdir
rm rootdir/test* rootdir/storage/block_file rootdir/storage/metadata_file

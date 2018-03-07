#!/bin/sh
# Script to clean up ipc and processes

echo Cleaning Up
killall server
killall client
ipcrm -Q 6969
ipcrm -S 100

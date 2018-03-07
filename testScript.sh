#!/bin/sh
# Test script to spin up many clients at once

priority=1

echo Starting Server
osascript -e 'tell application "Terminal" to do script "cd /Users/Tim/Documents/C\\ Projects/c4981Assign2/ ; ./server"'

echo Starting Clients
for i in `seq 1 5`;
do
	osascript -e 'tell application "Terminal" to do script "cd /Users/Tim/Documents/C\\ Projects/c4981Assign2/ ; ./client big.txt '$priority'"'
    priority=$((priority+1))
done


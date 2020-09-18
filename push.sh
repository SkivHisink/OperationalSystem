#!/bin/bash
echo "pushing files..."
pwd
git add .
git commit -m "commit"
echo "successful commited"
pwd
sleep 1 
echo "pushing origin"
pwd
git push -u origin master

#!/bin/bash
echo "pushing files..."
git add .
git commit -m "commit"
echo "successful commited"
sleep 1 
echo "pushing origin"
git push -u origin master

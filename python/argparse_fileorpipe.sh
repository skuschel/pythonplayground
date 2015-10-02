#!/bin/bash

# Stephan Kuschel, 151002

echo --- reading current script ---
./argparse_fileorpipe.py argparse_fileorpipe.sh

echo --- use pipe explicitly ---
echo explicit pipe | ./argparse_fileorpipe.py -

echo -- use pipe implicitly ---
echo implicit pipe | ./argparse_fileorpipe.py

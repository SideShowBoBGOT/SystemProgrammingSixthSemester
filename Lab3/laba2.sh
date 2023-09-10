#!/bin/bash
nrows=$(cat $1 | awk 'END{print NR}')
cat $1 | awk 'BEGIN{FS=" "}($0 !~ /.*"-".*/){print $11}'| sed 's/"//g' | sort | uniq -c | awk 'BEGIN{FS=" ";nrows='$nrows'}{print $2"\t"$1"\t"$1/nrows*100"%"}' | sort -k 3n | tail -10 | tac


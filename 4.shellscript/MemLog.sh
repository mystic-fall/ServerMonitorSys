#!/bin/bash
NowTime=`date +"%Y-%m-%d__%H:%M:%S"`
Memory=(`free -m | head -n 2 | tail -n 1 | awk '{printf("%d %d", $2, $3)}'`)
MemTotal=${Memory[0]}
MemUsed=${Memory[1]}
MemFree=$(( ${Memory[0]} - ${Memory[1]} ))

MemUsage=`echo "scale=2;($MemUsed/$MemTotal)*100" | bc`

MemUsageDyna=`echo "scale=2;(0.7*43+0.3*$MemUsage)" | bc`

echo 4 $NowTime ${MemTotal}M ${MemFree}M $MemUsage% $MemUsageDyna%

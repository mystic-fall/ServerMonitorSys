#!/bin/bash
NowTime=`date +"%Y-%m-%d__%H:%M:%S"`
eval `df | grep -v "tmpfs" | grep -v "on$" | awk -v num=0 '{num++;printf("DiskSwapSum["num"]=%d;DiskSwapFree["num"]=%d;DiskSwapUsage["num"]=%d;DiskSwapName["num"]=%s\n",$2,$4,$5,$6)} END{printf("Sum=%d\n", num)}'` 
DiskSum=0
DiskFree=0
DiskUsage=0
DiskName="disk"
for (( i=1; i<=Sum; i++ ));do
    echo 5 $NowTime 1 ${DiskSwapName[$i]} ${DiskSwapSum[$i]} ${DiskSwapFree[$i]} ${DiskSwapUsage[$i]}%
    let DiskSum+=${DiskSwapSum[$i]}
    let DiskFree+=${DiskSwapFree[$i]}
    let DiskUsage+=${DiskSwapUsage[$i]}
done
echo 5 $NowTime 0 $DiskName $DiskSum $DiskFree $DiskUsage%


#!/bin/bash
NowTime=`date +"%Y-%m-%d__%H:%M:%S"` #当前时间
LoadAvg=`cut -d " " -f 1-3 /proc/loadavg` #过去一分钟内的负载
CpuTemp="52.34"

eval $(head -n 1 /proc/stat | awk -v sum1=0 -v idle1=0 \
    '{for (i=2;i<=8;i++) {sum1=sum1+$i} printf("sum1=%d;idle1=%d",sum1,$5)}')
sleep 0.5
eval $(head -n 1 /proc/stat | awk -v sum2=0 -v idle2=0 \
    '{for (i=2;i<=8;i++) {sum2=sum2+$i} printf("sum2=%d;idle2=%d",sum2,$5)}')
CpuUsage=`echo "scale=2;(1-($idle2-$idle1)/($sum2-$sum1))*100" | bc` #cpu在过去0.5s内使用率

TempWarnLevel="normal" #温度警示等级
if [[ `echo $CpuTemp ">=" 70 | bc` -eq 1 ]];then
    TempWarnLevel="warning"
elif [[ `echo $CpuTemp ">=" 50 | bc` -eq 1 ]];then
    TempWarnLevel="note"
fi

echo 3 $NowTime $LoadAvg $CpuUsage% ${CpuTemp} $TempWarnLevel

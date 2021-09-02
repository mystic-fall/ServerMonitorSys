#!/bin/bash 
NowTime=`date +"%Y-%m-%d__%H:%M:%S"`
HostName=`hostname`
KernalVersion=`cat /proc/version | awk '{printf("%s", $3)}'`
OSVersion=`cat /proc/version | awk '{printf("Ubuntu/%s", $9)}' | tr -d ')'`
RunTime=`uptime -p | tr -c '[a-z0-9,]' '_'`
LoadAvg=`cut -d " " -f 1-3 /proc/loadavg`
Disk=(`df -T -x tmpfs -h -x devtmpfs | tail -n 1 | awk '{printf("%s %d%%", $3,$6)}'`) 
DiskTotal=${Disk[0]}
DiskUsage=${Disk[1]}
Memory=(`free -m | head -n 2 | tail -n 1 | awk '{printf("%d %d", $2, $3)}'`)
MemTotal=${Memory[0]}
MemUsed=${Memory[1]}
MemUsage=`echo "scale=1;($MemUsed/$MemTotal)*100" | bc | cut -d "." -f 1-1` 
#CpuTemp=`cat /sys/class/thermal/thermal_zone0/temp`
CpuTemp="55.24"
DiskLevel="normal"
MemLevel="normal"
CpuLevel="normal"
level=2
if [[ `echo $DiskUsage | tr -d '%'` -gt 90 ]];then
    DiskLevel="warning"
    level=1
elif [[ `echo $DiskUsage | tr -d '%'` -gt 80 ]];then
    DiskLevel="note"
    level=1
fi
if [[ `echo $MemUsage | tr -d '%'` -gt 80 ]];then
    MemLevel="warning"
    level=1
elif [[ `echo $MemUsage | tr -d '%'` -gt 70 ]];then
    MemLevel="note"
    level=1
fi
if [[ `echo $CpuTemp ">" 70 | bc` -eq 1 ]];then 
    CpuLevel="warning"
    level=1
elif [[ `echo $CpuTemp ">" 50 | bc` -eq 1 ]];then
    CpuLevel="note"
    level=1
fi
echo $level $NowTime $HostName $OSVersion $KernalVersion $RunTime $LoadAvg $DiskTotal $DiskUsage  $MemTotal $MemUsage% $CpuTemp $DiskLevel $MemLevel $CpuLevel

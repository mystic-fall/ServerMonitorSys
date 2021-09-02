#!/bin/bash  
NowTime=`date +"%Y-%m-%d__%H:%M:%S"`
eval $(ps -aux --sort=-%cpu -h | awk -v num=0 \
    '{if($3<50){exit} else{num++; printf("cpupid["num"]=%d;", $2)}} \
    END {printf("cpunum=%d;", num)}')
eval $(ps -aux --sort=-%mem -h | awk -v num=0 \
    '{if($4<50){exit} else{num++; printf("mempid["num"]=%d;", $2)}} \
    END {printf("memnum=%d;", num)}')
if [[ $cpunum -gt 0 || $memnum -gt 0 ]];then
    sleep 5
else 
    exit 0
fi
cnt=0
if [[ $cpunum -gt 0 ]];then
    for i in ${cpupid[@]};do
        eval `ps -aux -h -q $i | awk -v num=${cnt} \
            '{if($3<50){exit} else{printf("Pname["num"]=%s; \
            Pid["num"]=%d;User["num"]=%s;CpuP["num"]=%.2f;MemP["num"]=%.2f;" \
            , $11,$2,$1,$3,$4); num++}} END{printf("cnt=%d;", num)}'`
    done
fi
if [[ $memnum -gt 0 ]];then
    for i in ${mempid[@]};do
        eval `ps -aux -h -q $i | awk -v num=${cnt} \
            '{if($4<50){exit} else{printf("Pname["num"]=%s; \
            Pid["num"]=%d;User["num"]=%s;CpuP["num"]=%.2f;MemP["num"]=%.2f;" \
            , $11,$2,$1,$3,$4); num++}} END{printf("cnt=%d;", num)}'`
    done
fi
for (( i=0; i<cnt; i++ ));do
    echo 7 $NowTime ${Pname[$i]} ${Pid[$i]} ${User[$i]} ${CpuP[$i]} ${MemP[$i]}
done

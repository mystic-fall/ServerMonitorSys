#!/bin/bash
NowTime=`date +"%Y-%m-%d__%H:%M:%S"`
eval $(awk -F: -v num=0 '{if($3>=1000 && $3!=65534){num++; printf("All["num"]=%s;", $1)}} END{printf("Sum=%d\n", num)}' /etc/passwd)
Active=`last -w | cut -d " " -f 1 | grep -v wtmp | grep -v reboot | grep -v "^$" | sort | uniq -c | sort -k1 -n -r | awk -v num=3 '{if(num>0) {printf(",%s", $2); num--}}' | cut -c 2-`
eval $(awk -F: '{if($3==1000) {printf("User_1000=%s\n", $1)}}' /etc/passwd)
UserWithRoot=$User_1000
Users=`cat /etc/group | grep sudo | cut -d : -f 4 | tr ',' ' '`
for i in $Users;do
    if [[ $i == $User_1000 ]];then
        continue;
    fi
    UserWithRoot="$UserWithRoot,$i"
done
if [[ -r /etc/sudoers ]];then
    for i in ${All[@]};do
        grep -q "^$i" /etc/sudoers
        if [[ $? -eq 0 ]];then
            UserWithRoot="$UserWithRoot,$i"
        fi
    done
fi
UserLogin=`w -h | awk '{printf(",%s_%s_%s", $1,$3,$2)}' | cut -c 2-`
echo 6 $NowTime $Sum [$Active] [$UserWithRoot] [$UserLogin] 

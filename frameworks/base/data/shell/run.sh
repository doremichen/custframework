#!/system/bin/sh 
echo "Ma i am here!!!"

if [ ! -d "/sdcard/logs" ]; then
    mkdir "/sdcard/logs"
    echo "create new dir /sdcard/logs"
fi

t=`date +"%Y-%m-%d-%H-%M-%S"`
i=0
RANDOM=$$
X_DIFF=320+1
Y_DIFF=480+1
Z_DIFF=10+1
while [ i -le $1 ]
do 
    i=$(($i + 1)) 
    echo $i 
    echo "final count: $i" > "/sdcard/logs/altek_monkey_run"$t".log" 
    # x y
    x=$(($RANDOM%$X_DIFF))
    y=$(($RANDOM%$Y_DIFF))
    z=$(($RANDOM%$Z_DIFF))

    if [ $z -eq 10 ]; then
        input keyevent KEYCODE_BACK
    fi
    if [ $z -eq 5 ]; then
        input keyevent KEYCODE_F1
    fi
    if [ $z -eq 8 ]; then
        input keyevent KEYCODE_F10
    fi
    

    input tap $x $y

    echo $x.$y.$z
done

echo "Finish altek monkey test!!!"


echo -n > trajectory.txt
for iter in $(seq 0 $1)
do
    for i in $(seq 1 5)
    do
        if [ "$iter" = "0" ]
        then
            for m in $(seq 1 5)
            do
                cat model_init.txt > model_0$m.txt
            done
        else
            ./train 1 model_0$i.txt ../seq_model_0$i.txt model_0$i.txt;
            echo "train $iter model_0$i"
        fi
    done
    ./test modellist.txt ../testing_data1.txt ans1.txt
    echo -n "$iter:" >> trajectory.txt
    ./accuracy ans1.txt ../testing_answer.txt acc.txt >> trajectory.txt
done
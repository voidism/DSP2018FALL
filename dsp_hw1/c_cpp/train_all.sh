for i in $(seq 1 5)
do
    ./train $1 model_init.txt ../seq_model_0$i.txt model_0$i.txt;
done

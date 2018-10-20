bash train_all.sh 200
./test modellist.txt ../testing_data1.txt ans1.txt
./accuracy ans1.txt ../testing_answer.txt
cat acc1.txt
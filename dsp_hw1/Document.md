Digital Signal Processing Homework 1
===

_Discrete Hidden Markov Model Implement_ 
b05901033 電機三 莊永松

### Environment
```
OS: Ubuntu 16.04.5 LTS (GNU/Linux 4.15.0-36-generic x86_64)
Compiler: g++ (Ubuntu 5.4.0-6ubuntu1~16.04.10) 5.4.0 20160609
CPU: Intel(R) Xeon(R) CPU E5-1620 v3 @ 3.50GHz
RAM: 32G
```

## How to execute?

- Use the command `make` to compile executable file `train` `test` `accuracy`,  use`make clean` to remove all compiled files.
- Train: `./train [iteration] [initial model file] [sequence file] [output file]`
- Test: `./test [model list] [sequence file] [output file]`
- Compute Accuracy: `./accuracy [predict file] [answer file] [output file]`

## Some Analysis

### Relation between accuracy and iteration

| iteration | accuracy |
| --------- | -------- |
| 0 | 0.1992 |
| 1 | 0.7660 |
| 5 | 0.6484 |
| 10 | 0.5408 |
| 15 | 0.6104 |
| 20 | 0.7904 |
| 50 | 0.8228 |
| 100 | 0.8104 |
| 150 | 0.8600 |
| 200 | 0.8524 |
| 500 | 0.8556 |
| 900 | 0.8696 |
| 1000 | 0.8700 |

### visualize

![](https://i.imgur.com/VwCBrtb.png)

### log scale for iteration axis

![](https://i.imgur.com/pNTZtx1.png)

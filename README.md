## cutコマンドの実装

以下の例についてcutコマンドと自作したコマンドの出力が一致することを確認した

```
gcc -o out cut.c
```

### 例1
```
./out -f 2-3 -d "," comma.txt
```

- 入力
```
1,2,3,4,5
a,b,c,d,e
```

- 出力
```
2,3
b,c
```

### 例2
```
./out -f 1,3,5 -d " " space.txt
```


- 入力
```
1 2 3 4 5
6 7 8 9 0
```

- 出力
```
1 3 5
6 8 0
```

### 例3(指定した区切り文字が存在する行のみを出力)
```
./out -f 2-3 -d "," -s comma2.txt
```

- 入力
```
1,2,3,4,5
6,7,8,9,0
HELLO!
```

- 出力	
```
2,3
7,8
```

### 例4
```
./out -c 4- text.txt 
```

- 入力	
```
HELLO WORLD!
1A2B3C4D5E
```

- 出力
```
LO WORLD!
B3C4D5E
```

### 例5
```
./out -c 1,3,5 num.txt
```

- 入力
```
12345
67890
```

- 出力
```
135
680
```

### 例6
```
./out -f 1,3-4,9- -d ","  comma3.txt
```

- 入力
```
1,2,3,4,5,6,7,8,9,0
a,b,c,d,e,f,g,h,i,j
```

- 出力
```
1,3,4,9,0
a,c,d,i,j
```

### 例7(オプションの引数が誤っている場合)
```
./out -c 2,3,, num.txt
```

```
cut: fields are numbered from 1
Try 'cut --help' for more information.
```

### 例8(指定したファイルが存在しない場合)

```
./out -c 3 aaaaaa.txt
```

```
cut: aaaaaa.txt:No such file or directory
```



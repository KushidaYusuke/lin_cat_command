## cutコマンドの実装

以下の例についてcutコマンドと自作したコマンドの出力が一致することを確認した


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
./out -c 4- text.txt 

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

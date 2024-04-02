#include<stdio.h>
#include<stdlib.h>
int main(int argc, char **argv) {
  if (argc < 2) {
    fprintf(stderr, "引数の数が異常です\n");
    exit(1);
  }

  for(int i = 1; i < argc; i++) {
    FILE *file;
    file = fopen(argv[i], "r");
    if(file == NULL) {
      fprintf(stderr, "ファイルを開くことができませんでした\n");
      exit(1);
    }
    //テキストファイルの内容を一行ずつbufに読み込んで標準出力する
    char buf[200];
    while(fgets(buf, 200, file) != NULL) {
      printf("%s", buf);
    }
    fclose(file);
  }
  return 0;
}

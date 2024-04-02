#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

#define max_length 2000 //1行の長さは最長で2000文字であるとする 
int main(int argc, char **argv) {
  if (argc < 2) {
    fprintf(stderr, "引数の数が異常です\n");
    exit(1);
  }

  if(strcmp(argv[1], "-n") == 0) {
    if(argc < 3) {
      fprintf(stderr, "引数の数が異常です\n");
    }  
  
    //現在の行数をカウントする
    int counter = 1;
    for(int i = 2; i < argc; i++) {
      FILE *file;
      file = fopen(argv[i], "r");
      if(file == NULL) {
        fprintf(stderr, "ファイルを開くことができませんでした\n");
	exit(1);
      }
      char buf[max_length];
      while(fgets(buf, max_length, file) != NULL) {
        printf("%d", counter);
	counter += 1;
        printf("%s", buf);
      }
      fclose(file);
    }
  }
  
  else if(strcmp(argv[1], "-b") == 0) {
    if(argc < 3) {
      fprintf(stderr, "引数の数が異常です\n");
      exit(1);  
    }
    int counter_b = 1;
    for(int i = 2; i < argc; i++) {
      FILE *file;
      file = fopen(argv[i], "r");
      if(file == NULL) {
        fprintf(stderr, "ファイルを開くことができませんでした\n");
	exit(1);
      }
      char buf[max_length];
      while(fgets(buf, max_length, file) != NULL) {
	if(buf[0] != '\n') {
	//if(strlen(buf) != 0) {
	//if(isspace(buf[0]) == 0) {
          printf("%d", counter_b);
	  counter_b += 1;
	}
        printf("%s", buf);	
      }
      fclose(file);
    }
  }
    
  else {
    for(int i = 1; i < argc; i++) {
      FILE *file;
      file = fopen(argv[i], "r");
      if(file == NULL) {
        fprintf(stderr, "ファイルを開くことができませんでした\n");
        exit(1);
      }
      //テキストファイルの内容を一行ずつbufに読み込んで標準出力する
      char buf[max_length];
      while(fgets(buf, max_length, file) != NULL) {
        printf("%s", buf);
      }
      fclose(file);
    }
  }

  return 0;

}



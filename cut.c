#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<stdbool.h>

int main(int argc, char **argv) {
  if(argc < 3) {
    fprintf(stderr, "引数が足りていません");
    exit(1);
  }

  if(strcmp(argv[1], "-c") == 0) {
    int cut_index = atoi(argv[2]);
    cut_index -= 1; //indexは0スタート
    for(int i = 3; i < argc; i++) {
      FILE *file;
      file = fopen(argv[i], "r");
      if(file == NULL) {
        fprintf(stderr, "ファイルを開くことができません");
        exit(1);
      }
      int now_index = 0; //現在の行頭から0-indexで何番目か
      int c;
      while((c = fgetc(file)) != EOF) {
        //改行の場合
        if(c == '\n') {
          putchar(c);
	  now_index = 0;
	  continue;
        }
        else if(now_index == cut_index) {
          putchar(c);
        }
        now_index += 1;
      }
      fclose(file); 
    }
  }
  
  }

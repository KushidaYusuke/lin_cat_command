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

  if(strcmp(argv[1], "-b") == 0) {
    int cut_byte = atoi(argv[2]);
    for(int i = 3; i < argc; i++) {
      FILE *file;
      file = fopen(argv[i], "r");
      if(file == NULL) {
        fprintf(stderr, "ファイルを開くことができません");
        exit(1);
      }
      int now_byte = 0; //先頭から何バイトか
      char c;
      while((c = fgetc(file)) != EOF) {
	now_byte += (int)(sizeof(c)); //修正する
	//改行の場合
        if(c == '\n') {
          putchar(c);
	  now_byte = 0;
	  continue;
        }
        else if(now_byte == cut_byte) {
          putchar(c);
        }
      }
      fclose(file); 
    }
  }

  //-fオプション(-dオプションで指定した区切り文字で区切られた項目の中で指定した項目数の項目を出力)
  if((strcmp(argv[1], "-f") == 0) && (strcmp(argv[3], "-d") == 0)) {
    int cut_field = atoi(argv[2]);
    char cut_letter = argv[4][0]; //-dで指定した区切り文字：
    cut_field -= 1;
    //printf("cutfield%d", cut_field); //デバッグ
    //printf("cutletter%c", cut_letter);//デバッグ
    for(int i = 5; i < argc; i++) {
      FILE *file;
      file = fopen(argv[i], "r");
      if(file == NULL) {
        fprintf(stderr, "ファイルを開くことができません");
        exit(1);
      }
      int now_field = 0; //先頭からタブ区切りで何フィールド目にいるか
      char c;
      if(c == cut_letter) {
        now_field += 1;
      }
      while((c = fgetc(file)) != EOF) {
	
	//改行の場合
        if(c == '\n') {
          putchar(c);
	  now_field = 0;
	  continue;
        }
	if(c == cut_letter) {
	  now_field += 1;
	  continue;
	}
        if(cut_field == now_field) {
	  putchar(c);
	}
      }
      fclose(file);
    }
  }
}

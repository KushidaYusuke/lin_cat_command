#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<stdbool.h>
#define max_length 2000 //1行の長さは最長で2000文字であるとする 

int main(int argc, char **argv) {
  if (argc == 1) {
    while(1) {
      char *read_input;
      scanf("%s", read_input);
      printf("%s", read_input);
      printf("\n");
    }
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
        printf("\t");
	printf("%d", counter);
	printf(" ");
	counter += 1;
        printf("%s", buf);
      }
      fclose(file);
    }
  }
  
  else if(strcmp(argv[1], "-E") == 0) { 
    for(int i = 2; i < argc; i++) {
      FILE *file;
      file = fopen(argv[i], "r");
      if(file == NULL) {
        fprintf(stderr, "ファイルを開くことができませんでした\n");
        exit(1);
      }
      //テキストファイルの内容を一行ずつbufに読み込んで標準出力する
      char buf[max_length];
      while(fgets(buf, max_length, file) != NULL) {
        buf[strlen(buf)-1] = '$';
	buf[strlen(buf)] = '\0';
	//buf[strlen(buf)+1] = '\0';
        printf("%s", buf);
	printf("\n");
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
          printf("\t");
          printf("%d", counter_b);
	  counter_b += 1;
	  printf(" ");
	}
        printf("%s", buf);	
      }
      fclose(file);
    }
  }
  
  else if(strcmp(argv[1], "-s") == 0) { 
    if(argc < 3) {
      fprintf(stderr, "引数の数が異常です\n");
      exit(1);  
    }
      
    //前の行が空白行であるか否かを管理する
    bool is_before_blank = false;
    
    for(int i = 2; i < argc; i++) {
      FILE *file;
      file = fopen(argv[i], "r");
      if(file == NULL) {
        fprintf(stderr, "ファイルを開くことができませんでした\n");
	exit(1);
      }
      char buf[max_length];
      while(fgets(buf, max_length, file) != NULL) {
	if(buf[0] == '\n') {
	  is_before_blank = true;
	  continue;
	}
	else if(is_before_blank==true) {
          printf("\n");
	  is_before_blank=false;
	}
        printf("%s", buf);	
      }
      fclose(file);
    }
  }
  //タブ文字を置換
  else if(strcmp(argv[1], "-T") == 0) { 
    if(argc < 3) {
      fprintf(stderr, "引数の数が異常です\n");
      exit(1);  
    }
    for(int i = 2; i < argc; i++) {
      FILE *file;
      file = fopen(argv[i], "r");
      if(file == NULL) {
        fprintf(stderr, "ファイルを開くことができませんでした\n");
        exit(1);
      }
      //テキストファイルの内容を一行ずつbufに読み込んで標準出力する
      char buf[max_length];
      while(fgets(buf, max_length, file) != NULL) {
        for(int i = 0; i < (int)strlen(buf); i++) {
	  if(buf[i] == '\t') {
	    printf("^I");
	  }
	  else {
	    printf("%c", buf[i]);
	  }
	}
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



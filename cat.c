#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<stdbool.h>
#define max_length 2000 //1行の長さは最長で2000文字であるとする 

int main(int argc, char **argv) {
  //ファイル名が指定されていない場合
  if (argc == 1) {
    while(1) {
      int c;
      while((c = fgetc(stdin)) != EOF) {
        putchar(c);
  }
    }
    exit(1);
  }
  
  //-nオプションを指定した場合
  if(strcmp(argv[1], "-n") == 0) {
    if(argc < 3) {
      fprintf(stderr, "引数の数が異常です\n");
    }   
    //現在の行数をカウントする
    int counter = 0;
    for(int i = 2; i < argc; i++) {
      FILE *file;
      file = fopen(argv[i], "r");
      if(file == NULL) {
        fprintf(stderr, "ファイルを開くことができませんでした\n");
	exit(1);
      }
      char buf[max_length];
      int c;
      
      bool is_first_row_char = true; //行頭の文字であるかを管理する
      while((c = fgetc(file)) != EOF) {
        if(is_first_row_char) {
          counter += 1;
	  printf("\t");
	  printf("%d", counter);
	  printf(" ");
	  is_first_row_char = false;
        }
        putchar(c);
	if(c == '\n') {
	  is_first_row_char = true;
	}
      }
      fclose(file);
    }
  }
  
  //-Eオプションを指定した場合
  else if(strcmp(argv[1], "-E") == 0) { 
    for(int i = 2; i < argc; i++) {
      FILE *file;
      file = fopen(argv[i], "r");
      if(file == NULL) {
        fprintf(stderr, "ファイルを開くことができませんでした\n");
        exit(1);
      }
      int c;
      while((c = fgetc(file)) != EOF) {
        if(c == '\n') {
	  putchar('$');
	}
	putchar(c);

      }
      //テキストファイルの内容を一行ずつbufに読み込んで標準出力する
      fclose(file);
    }
  }
  //-bオプションを指定した場合
  else if(strcmp(argv[1], "-b") == 0) {
    if(argc < 3) {
      fprintf(stderr, "引数の数が異常です\n");
      exit(1);  
    }
    int counter_b = 0;
    for(int i = 2; i < argc; i++) {
      FILE *file;
      file = fopen(argv[i], "r");
      if(file == NULL) {
        fprintf(stderr, "ファイルを開くことができませんでした\n");
	exit(1);
      }
      int c;
      bool is_first_row_char = true;
      while((c = fgetc(file)) != EOF) {
        if(is_first_row_char && (c == '\n')) {
	  putchar('\n');
	  continue;
	}
	else if(is_first_row_char) {
          counter_b += 1;
	  printf("\t");
	  printf("%d", counter_b);
	  printf(" ");
	  is_first_row_char = false;
        }
        putchar(c);
	if(c == '\n') {
	  is_first_row_char = true;
	}
      }
      fclose(file);
    }
  }

  //-sオプションを指定した場合
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

  //オプションの指定がない場合
  else {
    for(int i = 1; i < argc; i++) {
      FILE *file;
      file = fopen(argv[i], "r");
      if(file == NULL) {
        fprintf(stderr, "ファイルを開くことができませんでした\n");
        exit(1);
      }
      //テキストファイルの内容を一行ずつbufに読み込んで標準出力する
      int c;
      while((c = fgetc(file)) != EOF) {
        putchar(c);
      }
      fclose(file);
    }
  }

  return 0;

}



#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<stdbool.h>

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
    for(int i = 2; i < argc; i++) {
      FILE *file;
      file = fopen(argv[i], "r");
      if(file == NULL) {
        fprintf(stderr, "ファイルを開くことができませんでした\n");
	exit(1);
      }
      int c;
      bool is_first_row_char = true; //行頭であるかを管理
      bool is_first_blank = true; //連続した空白行のうち、初めの空白行であるか
      while((c = fgetc(file)) != EOF) {
        //空白行の場合の処理
	if(is_first_row_char && (c == '\n')) {
	  if(is_first_blank == true) {
	    putchar('\n');
	    is_first_blank = false;
	  }
	  continue;
	}
	//空白行でない場合の処理
        else {
	  putchar(c);
	  is_first_row_char = false;
	  is_first_blank = true;
	  if(c == '\n') {
	    is_first_row_char = true;
	  }
	}
      }
      fclose(file);
    }
  }

  //-Tオプション(タブ文字を置換)
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
      int c;
      while((c = fgetc(file)) != EOF) {
        if(c == '\t') {
	    printf("^I");
	  }
	  else {
	    putchar(c);
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



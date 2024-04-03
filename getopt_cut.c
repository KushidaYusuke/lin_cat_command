#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<stdbool.h>
#include<unistd.h>
int bopt = 0;
int copt = 0;
int fopt = 0;
int dopt = 0;

char *bparam = NULL;
char *cparam = NULL;
char *fparam = NULL;
char *dparam = NULL;

void cut_command(FILE *file) {
  if(fparam && dparam) {
    int cut_field = atoi(fparam);
    char cut_letter = dparam[0]; //-dで指定した区切り文字：
    cut_field -= 1;
    int now_field = 0; //先頭から-dで指定した区切りで何フィールド目にいるか
    char c;
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

int main(int argc, char *argv[]) {
  int opt;
  while((opt = getopt(argc, argv, "b:c:f:d:")) != -1) {
    switch(opt) {
      case 'b':
        bopt = 1;
	bparam = optarg;
	break;
      case 'c':
        copt = 1;
	cparam = optarg;
	break;
      case 'f':
        fopt = 1;
	fparam = optarg;
	break;
      case 'd':
        dopt = 1;
	dparam = optarg;
	break;
    }
  }
 
//  char *delim_1 = ",";
 // char *delim_2 = ":";
 // int delim_1_count = 0;
 // int delim_2_count = 0;
 // char *token_1 = strtok(cparam, delim_1);
  //while(token_1 != NULL) {
   // delim_1_count += 1;
    //printf("%s\n", token_1);
    //token_1 = strtok(NULL, delim_1);
 // }
  //char *token_2 = strtok(cparam, delim_2);
  //while(token_2 != NULL) {
   // delim_2_count += 1;
    //printf("%s\n", token_2);
    //token_2 = strtok(NULL, delim_2);
  //}

  for(int i = optind; i < argc; i++) {
    FILE *file;
    file = fopen(argv[i], "r");
    if(file == NULL) {
      fprintf(stderr, "ファイルを開くことができません\n");
      exit(1);
    }
    cut_command(file);
  }
}
  //ファイルが指定されない場合
  //(optind == argc) {
    //ile(1) {
      //intf("標準入力から受け取る");
    //
  //
  //r(int i = optind; i < argc; i++) {
    //LE *file;
    //le = fopen(argv[i], "r");
    //(file == NULL) {
      //rintf(stderr, "ファイルを開くことができませんでした\n");
     //xit(1);
    //
   //at_program(file);
   //close(file);
 //
















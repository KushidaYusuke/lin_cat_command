#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<stdbool.h>
#include<unistd.h>
#include<regex.h>
int bopt = 0;
int copt = 0;
int fopt = 0;
int dopt = 0;

char *bparam = NULL;
char *cparam = NULL;
char *fparam = NULL;
char *dparam = NULL;

//正規表現を用いた判定のための変数
//char checkstring[] = "2-9";
const char regex_1[] = "^([0-9]+)-$";
const char regex_2[] = "^-([0-9]+)$";
const char regex_3[] = "^([0-9]+)-([0-9]+)$";
regex_t regexBuffer;
regmatch_t match[4];
int size;
char result[128];

#define INF 10000
int token_list[INF];

char *delim = ",";
int delim_count = 0;


int match_num = 0;

int lower_bound = 0;
int upper_bound = INF;

bool regex_check(char *checkstring) {
  if(regcomp(&regexBuffer, regex_1, REG_EXTENDED | REG_NEWLINE) != 0) {
    return false;
    
  }
  size = sizeof(match)/sizeof(regmatch_t);
  if(regexec(&regexBuffer, checkstring, size, match, 0) == 0) {
    regfree(&regexBuffer);
    match_num = 1;
  }
  if(regcomp(&regexBuffer, regex_2, REG_EXTENDED | REG_NEWLINE) != 0) {
    return false;
  }
  if(regexec(&regexBuffer, checkstring, size, match, 0) == 0) {
    regfree(&regexBuffer);
    match_num = 2;
  }
  if(regcomp(&regexBuffer, regex_3, REG_EXTENDED | REG_NEWLINE) != 0) {
    return false;
  }
  if(regexec(&regexBuffer, checkstring, size, match, 0) == 0) {
    regfree(&regexBuffer);
    match_num = 3;
  }
  if(match_num == 1) {
    sscanf(checkstring, "%d-", &lower_bound);
    if(!bopt){lower_bound -= 1;} //0-index
    upper_bound = INF;
  }
    // c -7のような場合
  if(match_num == 2) {
    sscanf(checkstring, "-%d", &upper_bound);
    if(!bopt){upper_bound -= 1;}
    lower_bound = 0;
  }
    // c 2-7のような場合
  if(match_num == 3) {
    sscanf(checkstring, "%d-%d", &lower_bound, &upper_bound);
    if(!bopt) {
      upper_bound -= 1;
      lower_bound -= 1;
    }
  }
  //いずれかの正規表現にマッチした場合
  if(match_num == 1 | match_num == 2 | match_num == 3) {
    return true;
  }
  else {
    return false;
  }
}

int *token_parse(char *param) {
  //int token_list[INF];
  //printf("これはデバッグです!");
  char *token = strtok(param, delim);
  while(token != NULL) {
    if(!bopt) {token_list[delim_count] = atoi(token)-1;}
    else  {token_list[delim_count] = atoi(token);}
    delim_count += 1;
    token = strtok(NULL, delim);
  }
  return token_list;
}



void cut_command(FILE *file) {
  if(copt) {

    bool is_regex_matched = regex_check(cparam);
    //printf("match_numは%d", match_num);
    //いずれかの正規表現にマッチした場合の処理
    if(is_regex_matched) {
      //printf("debug\n");
      int now_index = 0; //現在の行頭から0-indexで何番目か
      int c;
      while((c = fgetc(file)) != EOF) {
        //改行の場合
        if(c == '\n') {
          putchar(c);
	  now_index = 0;
	  continue;
        }
        else if((now_index >= lower_bound) && (now_index <= upper_bound)) {
          putchar(c);
        }
        now_index += 1;
      }
      fclose(file); 
    }
    //	-c 2,3,4のような形でオプションが与えられる場合の処理
    else {       
      int *token_list_c = token_parse(cparam);
      int now_index = 0; //現在の行頭から0-indexで何番目か
      int c;
      for(int i = 0; i < delim_count; i++) {
        //printf("デバッグ: token_list_cの%d番目の要素は%d", i, token_list_c[i]);
      }
      while((c = fgetc(file)) != EOF) {
        //改行の場合
        if(c == '\n') {
          putchar(c);
	  now_index = 0;
	  continue;
        }
        else {
          for(int i = 0; i < delim_count; i++) {
	    int cut_index = token_list_c[i];
	    if(now_index == cut_index) {
	      putchar(c);
	    }
	  }
        }
        now_index += 1;
      }
      fclose(file); 
      }
    }


  if(fopt) {
    //int cut_field = atoi(fparam);
    //cut_field -= 1;
    
    char cut_letter;
    if(dopt) {
      cut_letter = dparam[0]; //-dで指定した区切り文字：    
    }
    else {
      cut_letter = '\t';
    }
    bool is_regex_matched_f = regex_check(fparam);
    if(is_regex_matched_f) {
      bool is_first_delim = true; //初めの区切り文字であるか判定
      int now_field = 0;
    
      int c;      
      while((c = fgetc(file)) != EOF) {
	
        //改行の場合
        if(c == '\n') {
          putchar(c);
	  now_field = 0;
	  is_first_delim = true;
	  continue;
        }
        if(c == cut_letter) {
          now_field += 1;
	//  continue;
        }
      
        if((now_field >= lower_bound) && (now_field <= upper_bound)) {
          if(c != cut_letter) {
	    is_first_delim = false;
	  }
	  if(!is_first_delim) {
            putchar(c);
	    }

	}
      }
    }
    //-f 2,3,4 -d ","のような形でオプションが与えられた場合の処理
    else {
      int *token_list_d = token_parse(fparam);
      
      bool is_first_delim = true; //初めの区切り文字であるか判定
      int now_field = 0;
    
      int c;      
      while((c = fgetc(file)) != EOF) {
	
        //改行の場合
        if(c == '\n') {
          putchar(c);
	  now_field = 0;
	  is_first_delim = true;
	  continue;
        }
        if(c == cut_letter) {
          now_field += 1;
	//  continue;
        }
        
	for(int i = 0; i < delim_count; i++) {  
          int cut_field = token_list_d[i];
	  if(now_field == cut_field) {
            if(c != cut_letter) {
	      is_first_delim = false;
	    }
	    if(!is_first_delim) {
              putchar(c);
	     }  
	   }
	}
      }
    }


    }
    


  if(bopt) {
    bool is_regex_matched_b = regex_check(bparam);
    //printf("match_numは%d", match_num);
    //いずれかの正規表現にマッチした場合の処理
    if(is_regex_matched_b) {
      int now_byte = 0; //現在の行頭から何バイト目か
      char c;
      while((c = fgetc(file)) != EOF) {
	now_byte += (int)(sizeof(c));
        //printf("デバッグ%d\n", now_byte);
	//改行の場合
        if(c == '\n') {
          putchar(c);
	  now_byte = 0;
	  continue;
        }
        else if((now_byte >= lower_bound) && (now_byte <= upper_bound)) {
          putchar(c);
        }
      }
      fclose(file); 
    }
    //	-c 2,3,4のような形でオプションが与えられる場合の処理
    else {       
      int *token_list_b = token_parse(bparam);
      int now_byte = 0; //現在の行頭から0-indexで何番目か
      char c;
      for(int i = 0; i < delim_count; i++) {
        //printf("デバッグ: token_list_cの%d番目の要素は%d", i, token_list_c[i]);
      }
      while((c = fgetc(file)) != EOF) {
        now_byte += (int)(sizeof(c));
	//改行の場合
        if(c == '\n') {
          putchar(c);
	  now_byte = 0;
	  continue;
        }
        else {
          for(int i = 0; i < delim_count; i++) {
	    int cut_byte = token_list_b[i];
	    if(now_byte == cut_byte) {
	      putchar(c);
	    }
	  }
        }
      }
      fclose(file); 
      }
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
  

  //オプションの組み合わせが不適切の場合の処理
  int pattern1 = (bopt | copt | fopt); //-b, -c, -fのいずれかのオプションが必須
  int pattern2 = (dopt & !fopt);
  if(pattern1 == 0 || pattern2) {
    fprintf(stderr, "オプションの組み合わせが不適切です\n");
    exit(1);
  }
  
  if(argc == optind) {
    while(1) {
      cut_command(stdin); 
    }
  }
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
















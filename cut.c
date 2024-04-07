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
const char regex_4[] = "^[0-9](,[0-9])*$";
regex_t regexBuffer;
regmatch_t match[4];
int size;
char result[128];

#define INF 100000
int token_list[INF];

char *delim = ",";//トークンの区切り文字
int delim_count = 0;//トークンの数


int match_num = 0;//どの正規表現にマッチしたかを表す

//%d-%d, -%d, %d-型の引数が渡される
int lower_bound = 0;
int upper_bound = INF;

bool token_regex_matched = false;//引数が3や2,3,4という形(regex_4変数で表現される正規表現に対応)で与えられているかを判定

//正規表現の判定をする関数
//オプションの引数が%d-, %d-%d, -%dという形をしているときにTrue, それ以外の場合にfalseを返す
//match_num=1: 引数は %d-型	
//match_num=2: 引数は %d-%d型
//match_num=3: 引数は -%d型
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

//オプションの引数が1,3,4という形をしている場合にそれぞれの数値をリストに入れて返す
int *token_parse(char *param) {
  if(regcomp(&regexBuffer, regex_4, REG_EXTENDED | REG_NEWLINE) != 0) {  
  }
  size = sizeof(match)/sizeof(regmatch_t);
  if(regexec(&regexBuffer, param, size, match, 0) == 0) {

    regfree(&regexBuffer);
    token_regex_matched = true;
  }
  char *token = strtok(param, delim);
  while(token != NULL) {
    if(!bopt) {token_list[delim_count] = atoi(token)-1;}
    else  {token_list[delim_count] = atoi(token);}
    delim_count += 1;
    token = strtok(NULL, delim);
  }
  return token_list;
}


//それぞれの指定されたファイルをオプションに従って処理する
void cut_command(FILE *file) {
  if(copt) {

    bool is_regex_matched = regex_check(cparam);
    //いずれかの正規表現にマッチした場合の処理
    if(is_regex_matched) {
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
    }
    //	-c 2,3,4のような形でオプションが与えられる場合の処理
    else {       
      int *token_list_c = token_parse(cparam);
      if(token_regex_matched==false) {
        fprintf(stderr, "cut: fields are numbered from 1\nTry 'cut --help' for more information.\n");
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
      }
    }


  if(fopt) {
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
      
      if(token_regex_matched==false) {
        fprintf(stderr, "引数が異常です\n");
	exit(1);
      }
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
    }
    //	-c 2,3,4のような形でオプションが与えられる場合の処理
    else {       
      int *token_list_b = token_parse(bparam);
      int now_byte = 0; //現在の行頭から0-indexで何番目か
      if(token_regex_matched==false) {
        fprintf(stderr, "引数が異常です\n");
	exit(1);
      }
      char c;
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
      }
    }
  }


int main(int argc, char *argv[]) {
  //オプションの処理
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
  int pattern2 = (dopt & !fopt); //-dオプションは-fオプションとともに指定する必要がある
  int pattern3 = ((bopt & (copt | fopt)) | (copt & (bopt | fopt)) | (fopt & (copt | bopt))); //-b,-c,-fオプションのうち2つ以上が同時に指定するのは不可
  if(pattern1 == 0 || pattern2 || pattern3) {
    fprintf(stderr, "オプションの組み合わせが不適切です\n");
    exit(1);
  }
 
 //ファイル名が指定されていない場合は標準入力から読み取る 
  if(argc == optind) {
    while(1) {
      cut_command(stdin); 
    }
  }

  //ファイルを順番に読み込んで処理する
  for(int i = optind; i < argc; i++) {
    FILE *file;
    file = fopen(argv[i], "r");
    if(file == NULL) {
      fprintf(stderr, "cut: %s:No such file or directory\n", argv[i]);
      exit(1);
    }
    cut_command(file);
    
    fclose(file);
  }
}




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
int sopt = 0;
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

#define INIT_ALLOC 3 //初めに確保するメモリの要素数	
#define ADD_ALLOC 10 //確保したメモリが足りなくなった場合に追加で確保する要素数
char *delim = ",";//トークンの区切り文字
int delim_count = 0;//トークンの数
int match_num = 0;//どの正規表現にマッチしたかを表す

//%d-%d, -%d, %d-型の引数が渡される
#define INF 1000000
int lower_bound = 0;
int upper_bound = INF;

bool token_regex_matched = false;//引数が3や2,3,4という形(regex_4変数で表現される正規表現に対応)で与えられているかを判定

//エラーを表す変数
bool regex_match_error = false; //入力された引数に対して対応する正規表現が存在しない場合はエラー

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
  int *token_list = malloc(INIT_ALLOC*sizeof(int)); //int型INIT_ALLOC個分のメモリを事前に確保
  if(regcomp(&regexBuffer, regex_4, REG_EXTENDED | REG_NEWLINE) != 0) {  
  }
  size = sizeof(match)/sizeof(regmatch_t);
  if(regexec(&regexBuffer, param, size, match, 0) == 0) {

    regfree(&regexBuffer);
    token_regex_matched = true;
  }

  char *token = strtok(param, delim);
  int alloc_num = INIT_ALLOC; //現在割り当てられている要素数
  while(token != NULL) {
    //確保したメモリが足りなくなった場合に追加で確保
    if(delim_count == alloc_num) {
      token_list = realloc(token_list, (delim_count+ADD_ALLOC)*sizeof(int)); 
      alloc_num += ADD_ALLOC;
    }
    if(!bopt) {token_list[delim_count] = atoi(token)-1;}
    else  {token_list[delim_count] = atoi(token);}
    delim_count += 1;
    token = strtok(NULL, delim);
  }
  return token_list;
}


//-cオプションが指定された場合の処理	
void cut_option_c(FILE *file) {
  
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
      regex_match_error = true;
      return;
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


//-bオプションが指定された場合の処理
void cut_option_b(FILE *file) {
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
  //	-b 2,3,4のような形でオプションが与えられる場合の処理
  else {       
    int *token_list_b = token_parse(bparam);
    int now_byte = 0; //現在は先頭から何バイト目か
    if(token_regex_matched==false) {
      regex_match_error = true;
      return;
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


//-fオプションが指定された場合の処理
void cut_option_f(FILE *file) {
  char cut_letter;
  if(dopt) {
    cut_letter = dparam[0]; //-dで指定した区切り文字：    
  }
  else {
    cut_letter = '\t';
  }
  bool is_regex_matched_f = regex_check(fparam);
  //区切り文字が%d-, %d-%d, -%d型の場合
  if(is_regex_matched_f) {
    
    int now_index = 0; //現在見ている文字が行頭から何番目か(0-index)
    bool is_exist_cut_letter = false; //現在見ている行について区切り文字が存在するか判定
    char* tmp_stock = malloc(INIT_ALLOC*sizeof(char)); //各行の要素を一時的に保管
    int now_alloc = INIT_ALLOC; //現在割り当てられているメモリの要素数
    bool is_first_delim = true; //現在見ている行で初めの区切り文字であるか判定
    int now_field = 0;  
    int c;      

    while((c = fgetc(file)) != EOF) {
      //改行の場合
      if(c == '\n') {
	if(sopt & !is_exist_cut_letter) ;
	else if(!sopt & !is_exist_cut_letter) {
	  for(int i = 0; i < now_index; i++) {
	    putchar(tmp_stock[i]);
	  }
	  putchar('\n');
	}
	else {
	  for(int i = 0; i < now_index; i++) {
	    if(tmp_stock[i] == cut_letter) {
	      now_field += 1;
	    }
	    if((now_field >= lower_bound) && (now_field <= upper_bound)) {
	      if(tmp_stock[i] != cut_letter) {
	        is_first_delim = false;
	      }
	      if(!is_first_delim) {
	        putchar(tmp_stock[i]);
	      }
	    }
	  }
	  putchar('\n');
	}
	now_field = 0;
	is_first_delim = true;
	is_exist_cut_letter = false;
	now_index = 0;
	free(tmp_stock); //メモリを解放
	tmp_stock = malloc(INIT_ALLOC*sizeof(int));
        now_alloc = INIT_ALLOC;
	continue;
      }
      //確保したメモリが足りなくなった場合に追加で確保
      if(now_index == now_alloc) {
        tmp_stock = realloc(tmp_stock, (now_index+ADD_ALLOC)*sizeof(char));
        now_alloc += ADD_ALLOC;
      }
      tmp_stock[now_index] = c; //現在の行が終わるまで一時的に確保
      now_index += 1;
      if(c == cut_letter) {
	is_exist_cut_letter = true;
      }
    }
  } 
  //-f 2,3,4 -d ","のような形でオプションが与えられた場合の処理
  else {
    int *token_list_d = token_parse(fparam);
    if(token_regex_matched==false) {
      regex_match_error = true;
      return;
    }
    int now_index = 0; //現在見ている文字が行頭から何番目か(0-index)
    int now_alloc = INIT_ALLOC;
    int now_field = 0;
    bool is_exist_cut_letter = false;//現在見ている行について指定した区切り文字が存在するか
    char *tmp_stock = malloc(INIT_ALLOC*sizeof(char)); //各行の要素を一時的に保管する
    bool is_first_delim = true; //現在見ている行で初めの区切り文字であるか判定
    int c;      
    while((c = fgetc(file)) != EOF) {
      //改行の場合
      if(c == '\n') {
	if(sopt & !is_exist_cut_letter) ;
	else if(!sopt & !is_exist_cut_letter) {
	  for(int i = 0; i < now_index; i++) {
	    putchar(tmp_stock[i]);
	  }
	  putchar('\n');
	}
	else {
	  for(int i = 0; i < now_index; i++) {
	    if(tmp_stock[i] == cut_letter) {
	      now_field += 1;
	    }
	    for(int j = 0; j < delim_count; j++) {  
	      int cut_field = token_list_d[j];
	      if(now_field == cut_field) {
		if(tmp_stock[i] != cut_letter) {
		  is_first_delim = false;
		}
		if(!is_first_delim) {
		  putchar(tmp_stock[i]);
		}  
	      }
	    }
	  }
	  putchar('\n');
	}

       	now_field = 0;
	is_first_delim = true;
	is_exist_cut_letter = false;
	now_index = 0;
	now_alloc = INIT_ALLOC;
	free(tmp_stock);
	tmp_stock = malloc(INIT_ALLOC*sizeof(char));
	continue;
      }
      //現在着目している文字をtmp_stock内にいったん格納 
      if(now_index == now_alloc) {
        tmp_stock = realloc(tmp_stock, (now_index+ADD_ALLOC)*sizeof(char));
	now_alloc += ADD_ALLOC;
      }
      tmp_stock[now_index] = c;
      now_index += 1;
      if(c == cut_letter) {
        is_exist_cut_letter = true;
      }      
    }
  }
}




int main(int argc, char *argv[]) {
  //オプションの処理
  int opt;
  while((opt = getopt(argc, argv, "b:c:f:d:s")) != -1) {
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
      case 's':
        sopt = 1;
    }
  }
  

  //オプションの組み合わせが不適切の場合の処理
  int pattern1 = (bopt | copt | fopt); //-b, -c, -fのいずれかのオプションが必須
  int pattern2 = (dopt & !fopt) | (sopt & !fopt); //-d, -sオプションは-fオプションとともに指定する必要がある
  int pattern3 = ((bopt & (copt | fopt)) | (copt & (bopt | fopt)) | (fopt & (copt | bopt))); //-b,-c,-fオプションのうち2つ以上が同時に指定するのは不可
  if(pattern1 == 0 || pattern2 || pattern3) {
    fprintf(stderr, "オプションの組み合わせが不適切です\n");
    exit(1);
  }
 
 //ファイル名が指定されていない場合は標準入力から読み取る 
  if(argc == optind) {
    while(1) {
      if(copt) {
	cut_option_c(stdin);
      }
      if(fopt) {
	cut_option_f(stdin); 
      }
      if(bopt) {
	cut_option_b(stdin);
      }
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
    if(copt) cut_option_c(file);
    if(bopt) cut_option_b(file);
    if(fopt) cut_option_f(file);
    fclose(file);
    //エラーが起こった場合の処理
    if(regex_match_error) {
      fprintf(stderr, "cut: fields are numbered from 1\nTry 'cut --help' for more information.\n");
      exit(1);
    }
  }
}




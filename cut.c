#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<stdbool.h>
#include<unistd.h>
#include<regex.h>


#define INIT_ALLOC 3 //初めに確保するメモリの要素数	
#define ADD_ALLOC 10 //確保したメモリが足りなくなった場合に追加で確保する要素数
#define INF 1000000


//正規表現の判定をする関数
//オプションの引数が%d-, %d-%d, -%dという形をしているときにTrue, それ以外の場合にfalseを返す
//match_num=1: 引数は %d-型	
//match_num=2: 引数は %-%d型
//match_num=3: 引数は %d-%d型
bool regex_check(char *checkstring, int *match_num) {
  //正規表現を用いた判定のための変数
  //char checkstring[] = "2-9";
  const char regex_1[] = "^([0-9]+)-$";
  const char regex_2[] = "^-([0-9]+)$";
  const char regex_3[] = "^([0-9]+)-([0-9]+)$";
  regex_t regexBuffer;
  regmatch_t match[4];
  int size;
  char result[128];
  if(regcomp(&regexBuffer, regex_1, REG_EXTENDED | REG_NEWLINE) != 0) {
    return false;
  }
  size = sizeof(match)/sizeof(regmatch_t);
  if(regexec(&regexBuffer, checkstring, size, match, 0) == 0) {
    regfree(&regexBuffer);
    *match_num = 1;
  }
  if(regcomp(&regexBuffer, regex_2, REG_EXTENDED | REG_NEWLINE) != 0) {
    return false;
  }
  if(regexec(&regexBuffer, checkstring, size, match, 0) == 0) {
    regfree(&regexBuffer);
    *match_num = 2;
  }
  if(regcomp(&regexBuffer, regex_3, REG_EXTENDED | REG_NEWLINE) != 0) {
    return false;
  }
  if(regexec(&regexBuffer, checkstring, size, match, 0) == 0) {
    regfree(&regexBuffer);
    *match_num = 3;
  }
  //いずれかの正規表現にマッチした場合
  if(*match_num == 1 | *match_num == 2 | *match_num == 3) {
    return true;
  }
  else {
    return false;
  }
}


//オプションの引数について、数値のみから構成されているか確認
bool is_digit_all(char *token) {
  int length = strlen(token);
  for(int i = 0; i < length; i++) {
    if(isdigit(token[i]) == false) {
      return false;
    }
  }
  return true;
}




//オプションの引数を引数にとって、token_list, type配列を作成する
//戻り値はトークンのパースに成功した場合true, 失敗した場合false
void create_token_parse_list(char *param, char ***token_list_pointer, int **type_pointer, int *token_num_pointer, bool *input_format_error_pointer, bool *memory_allocate_error_pointer) {
  int now_alloc_token = INIT_ALLOC;
  int now_char_num = 0;
  int now_alloc_type = INIT_ALLOC;
  //オプションの引数とそのタイプを管理する動的配列　メモリの解放はmain関数内で行っている
  char *token;
  token = strtok(param, ",");

  int index = 0;
  while(token != NULL) {
    //ハイフンを含む引数の処理
    if(strchr(token, '-') != NULL) {
      int match_num = -1;
      //入力ができ刹那形式でなかった場合
      if(regex_check(token, &match_num) == false) {
	*input_format_error_pointer = true;
	return;
      }
      else {
        int token_length = strlen(token);
        now_char_num += token_length;
        if(now_alloc_token <= now_char_num) {
          *token_list_pointer = realloc(*token_list_pointer, (now_char_num + ADD_ALLOC)*(sizeof(char*)));
          //メモリの割り当てに失敗した場合
	  if(*token_list_pointer == NULL) {
	    *memory_allocate_error_pointer = true;
	    return;
	  }
	  now_alloc_token = now_char_num + ADD_ALLOC;  
        }
        if(now_alloc_type <= index + 1) {
          *type_pointer = realloc(*type_pointer, (now_alloc_type+ADD_ALLOC)*sizeof(int));
	  if(*type_pointer == NULL) {
	    *memory_allocate_error_pointer = true;
	    return;
	  }
          now_alloc_type += ADD_ALLOC;
        }
        (*token_list_pointer)[index] = token;
        (*type_pointer)[index] = match_num;
        index += 1;
      }
    }
    //ハイフンを含まない(数値のみからなる)引数の処理
    else {
      if(is_digit_all(token)) {
	(*token_list_pointer)[index] = token;
	(*type_pointer)[index] = 0;
	index += 1;
      }
      else {
        *input_format_error_pointer = true;
        return;
      }
    }
    token = strtok(NULL, ",");
  }
  *token_num_pointer = index;
  return;
}



//現在見ているインデックスが引数で指定した範囲に含まれる場合true, 含まれない場合falseを返す
bool check_range(int index, char **token_list, int *type, int bopt, int token_num) {
  int length = token_num; 
  for(int i = 0; i < length; i++) {
    if(type[i] == 0) {
      int now_token = atoi(token_list[i]);
      if(!bopt) now_token -= 1;
      if(index == now_token) {
	return true;
      }
      
    }
    int lower_bound = 0;
    int upper_bound = INF;
    if(type[i] == 1) {
      sscanf(token_list[i], "%d-", &lower_bound);
      if(!bopt) {
	lower_bound -= 1;
      }
      if((index >= lower_bound) && (index <= upper_bound)) {
        return true;
      }
    }

    if(type[i] == 2) {
      sscanf(token_list[i], "-%d", &upper_bound);
      if(!bopt) {
        upper_bound -= 1;
      }
      if((index >= lower_bound) && (index <= upper_bound)) {
        return true;
      }
    }

    if(type[i] == 3) {
      sscanf(token_list[i], "%d-%d", &lower_bound, &upper_bound);
      if(!bopt) {
        upper_bound -= 1;
	lower_bound -= 1;
      }
      if((index >= lower_bound) && (index <= upper_bound)) {
        return true;
      }
    }

  }
//インデックスが指定した引数の範囲に含まれなかった場合false
  return false;
}



//-cオプションが指定された場合の処理	
void cut_option_c(FILE *file, char **token_list, int *type, int bopt, int token_num) {
//オプションの引数のパースに失敗した場合
  int now_index = 0;
  int c;
  while((c = fgetc(file)) != EOF) {
    //改行の場合
    if(c == '\n') {
      putchar(c);
      now_index = 0;
      continue;
    }
    else if(check_range(now_index, token_list, type, bopt, token_num)) {
      putchar(c);
    }
    now_index += 1;
  }
}


//-bオプションが指定された場合の処理
void cut_option_b(FILE *file, char **token_list, int *type, int bopt, int token_num) {
  int now_byte = 0; //現在の行頭から何バイト目か
  char c;
  while((c = fgetc(file)) != EOF) {
    now_byte += (int)(sizeof(c));
    //改行の場合 改行して何バイト目かのカウントをリセット
    if(c == '\n') {
      putchar(c);
      now_byte = 0;
      continue;
    }
    else if(check_range(now_byte, token_list, type, bopt, token_num)) {
      putchar(c);
    }
  }
}


//-fオプションが指定された場合の処理
//row_buffer配列で各行の要素を一時的に保管しておいて、改行時にまとめて処理していることに注意
void cut_option_f(FILE *file, char **token_list, int *type, int bopt, int dopt, int sopt, char *dparam, int token_num, bool *memory_allocate_error_pointer) {
  char cut_letter;
  if(dopt) {
    cut_letter = dparam[0]; //-dで指定した区切り文字：    
  }
  else {
    cut_letter = '\t'; //デフォルトではタブ区切り
  }
   
  int now_index = 0; //現在見ている文字が行頭から何番目か(0-index)
  bool is_exist_cut_letter = false; //現在見ている行について区切り文字が存在するか判定
  char* row_buffer = malloc(INIT_ALLOC*sizeof(char)); //各行の要素を一時的に保管
  if(row_buffer == NULL) {
    *memory_allocate_error_pointer = true;
    return;
  }
  int now_alloc = INIT_ALLOC; //現在割り当てられているメモリの要素数
  bool is_first_delim = true; //現在見ている行で初めの区切り文字であるか判定(,1,2,3など先頭に,がくる場合は飛ばして1,2,3を出力する)
  int now_field = 0; //現在見ている文字は行頭から何フィールド目か  
  int c;      

  while((c = fgetc(file)) != EOF) {
    //改行文字の場合 row_bufferの処理を行う
    if(c == '\n') {
      //-sオプションが指定されており、指定した改行文字が現在の行に存在しなければその行を読み飛ばす
      if(sopt & !is_exist_cut_letter) ;
      //-sオプションが指定されておらず、指定された改行文字が現在の行に存在しない場合はその行をすべて出力	
      else if(!sopt & !is_exist_cut_letter) {
	for(int i = 0; i < now_index; i++) {  
	  putchar(row_buffer[i]);
	}
	putchar('\n');
      }
      else {
	for(int i = 0; i < now_index; i++) {
	  if(row_buffer[i] == cut_letter) {
	    now_field += 1;
	  }
	  if(check_range(now_field, token_list, type, bopt, token_num)) {
	    if(row_buffer[i] != cut_letter) {
	      is_first_delim = false;
	    }
	    if(!is_first_delim) {
	      putchar(row_buffer[i]);
	    }
	  }
	}
	putchar('\n');
      }
      free(row_buffer); 
      now_field = 0;
      is_first_delim = true;
      is_exist_cut_letter = false;
      now_index = 0;
      row_buffer = malloc(INIT_ALLOC*sizeof(int));//次の行の要素を保管するために再びメモリを確保する
      now_alloc = INIT_ALLOC;
      continue;
    }
    //確保したメモリが足りなくなった場合に追加で確保
    if(now_index == now_alloc) {
      row_buffer = realloc(row_buffer, (now_index+ADD_ALLOC)*sizeof(char));
      if(row_buffer == NULL) {
        *memory_allocate_error_pointer = true;
	return;
      }
      now_alloc += ADD_ALLOC;
    }
    row_buffer[now_index] = c; 
    now_index += 1;
    if(c == cut_letter) {
      is_exist_cut_letter = true;
    }
  }

  free(row_buffer);    
} 


int main(int argc, char *argv[]) {
  //エラーを表す変数
  bool memory_allocate_error = false;
  bool input_format_error = false;

  //トークンに分割された引数を受け取る
  //各トークンについてトークンの形に応じて次のようにタイプをつけて、タイプに応じた処理をする
  //%d -> 0 %d- -> 1 -%d -> 2 %d-%d ->3 
  //例: 2,3-4,6 -> token_list = {2, 3-4, 6-} type = {0,3,1} token_num = 3
  char **token_list = (char**)malloc(sizeof(char*)*INIT_ALLOC); 
  int *type = (int*)malloc(sizeof(int)*INIT_ALLOC);
  int token_num = 0; //オプション引数で受け取ったトークンの数
  //オプションの処理
  int bopt = 0;
  int copt = 0;
  int fopt = 0;
  int dopt = 0;
  int sopt = 0;
  char *bparam = NULL;
  char *cparam = NULL;
  char *fparam = NULL;
  char *dparam = NULL;
  int opt;
  while((opt = getopt(argc, argv, "b:c:f:d:s")) != -1) {
    switch(opt) {
      case 'b':
        bopt = 1;
	bparam = optarg;
	create_token_parse_list(bparam, &token_list, &type, &token_num, &input_format_error, &memory_allocate_error);
	break;
      case 'c':
        copt = 1;
	cparam = optarg;
	create_token_parse_list(cparam, &token_list, &type, &token_num, &input_format_error, &memory_allocate_error);
	break;
      case 'f':
        fopt = 1;
	fparam = optarg;
	create_token_parse_list(fparam, &token_list, &type, &token_num, &input_format_error, &memory_allocate_error);
	break;
      case 'd':
        dopt = 1;
	dparam = optarg;
	break;
      case 's':
        sopt = 1;
    }
  }

  //エラー処理
  if(input_format_error) {
    fprintf(stderr, "入力形式が正しくありません\n");
    free(token_list);
    free(type);
    exit(1);
  }
  if(memory_allocate_error) {
    printf("メモリの割り当てに失敗しました\n");
    free(token_list);
    free(type);
    exit(1);
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
    if(copt){cut_option_c(stdin, token_list, type, false, token_num);}
    if(fopt){cut_option_f(stdin, token_list, type, false, dopt, sopt, dparam, token_num, &memory_allocate_error);}
    if(bopt){cut_option_b(stdin, token_list, type, true, token_num);}
    
    if(memory_allocate_error) {
      printf("メモリの割り当てに失敗しました\n");
      exit(1);
    }
    free(token_list);
    free(type);
  }

  //ファイルを順番に読み込んで処理する
  for(int i = optind; i < argc; i++) {
    FILE *file;
    file = fopen(argv[i], "r");
    if(file == NULL) {
      fprintf(stderr, "cut: %s:No such file or directory\n", argv[i]);
      exit(1);
    }
    if(copt) cut_option_c(file, token_list, type, false, token_num);
    if(bopt) cut_option_b(file, token_list, type, true, token_num);
    if(fopt) cut_option_f(file, token_list, type, false, dopt, sopt, dparam, token_num, &memory_allocate_error);
    fclose(file);
    if(memory_allocate_error) {
      printf("メモリの割り当てに失敗しました\n");
      free(token_list);
      free(type);
      exit(1);
    }
  }
  // 1-2,3,4などのオプションの引数とそのタイプを保管している動的配列のメモリを解放
  free(token_list);
  free(type);
  
}



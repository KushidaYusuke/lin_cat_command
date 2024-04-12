#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<stdbool.h>
#include<unistd.h>
#include<regex.h>

const char regex_1[] = "^([0-9]+)-$";
const char regex_2[] = "^-([0-9]+)$";
const char regex_3[] = "^([0-9]+)-([0-9]+)$";
const char regex_4[] = "";
//const char regex_4[] = "^[0-9](,[0-9])*$";
const char regex_5[] = "^(((^([0-9]+)-$)|(^-([0-9]+)$)|(^([0-9]+)-([0-9]+)$)|(^[0-9]$)),)*((^([0-9]+)-$)|(^-([0-9]+)$)|(^([0-9]+)-([0-9]+)$)|(^[0-9]+$))$";
regex_t regexBuffer;
regmatch_t match[4];
int size;
char result[128];
int match_num;
char *delim = ",";//トークンの区切り文字

bool regex_check(char *checkstring) {
  if(regcomp(&regexBuffer, regex_5, REG_EXTENDED | REG_NEWLINE) != 0) {
    fprintf(stderr, "失敗!");
    return false;
    
  }
  size = sizeof(match)/sizeof(regmatch_t);
  if(regexec(&regexBuffer, checkstring, size, match, 0) == 0) {
    regfree(&regexBuffer);
    match_num = 1;
    return true;
  }
  else return false;
}

int main() {
  char check_string[] = "1-5,6,";
  bool hantei = regex_check(check_string);
  if(hantei) printf("%s\n", check_string);
  return 0;
}

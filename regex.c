#include<stdio.h>
#include<regex.h>
#include<stdlib.h>
int main(int argc, char *argv[]) {
  char checkstring[] = "2-9";
  const char regex_1[] = "^([0-9]+)-$";
  const char regex_2[] = "^-([0-9]+)$";
  const char regex_3[] = "^([0-9]+)-([0-9]+)$";
  regex_t regexBuffer;
  regmatch_t match[4];
  int size;
  char result[128];
  if(regcomp(&regexBuffer, regex_1, REG_EXTENDED | REG_NEWLINE) != 0) {
    printf("失敗!");
    exit(1);
  }
  size = sizeof(match)/sizeof(regmatch_t);
  if(regexec(&regexBuffer, checkstring, size, match, 0) == 0) {
    printf("マッチした!");
  }
  regfree(&regexBuffer);

  if(regcomp(&regexBuffer, regex_2, REG_EXTENDED | REG_NEWLINE) != 0) {
    printf("失敗2");
  }
  if(regexec(&regexBuffer, checkstring, size, match, 0) == 0) {
    printf("マッチした2!");
  }
  regfree(&regexBuffer);
 
  if(regcomp(&regexBuffer, regex_3, REG_EXTENDED | REG_NEWLINE) != 0) {
    printf("失敗3");
  }
  if(regexec(&regexBuffer, checkstring, size, match, 0) == 0) {
    printf("マッチした3!");
  }
  regfree(&regexBuffer);
}

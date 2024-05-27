#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define HASHSIZE 1171
struct cell {
  char *p_code; // 郵便番号
  char *address; // 住所
  struct cell *p_next; // 次の郵便番号のセルへのポインタ
  struct cell *a_next; // 次の住所のセルへのポインタ
};

// index: hash(郵便番号), value: cell 住所のハッシュテーブル
struct cell *p_table[HASHSIZE];

// index: hash(住所), value: cell 郵便番号のハッシュテーブル
struct cell *a_table[HASHSIZE];

// ハッシュ関数（utf-8）
unsigned int hash(const char *str) {
  unsigned int hashval = 0;
  unsigned char *p = (unsigned char *)str;

  while (*p != '\0') {
    hashval = *p + 31 * hashval;
    p++;
  }
  unsigned int result = hashval % HASHSIZE;
  // printf("%s: %d\n", str, result);
  return result;
}

// ハッシュテーブルにデータを追加する関数
struct cell *new_cell(char *p_code, char *address){
  struct cell *p = (struct cell *)malloc(sizeof(struct cell));
  if(p == NULL){
    perror("Error: malloc\n");
    exit(1);
  }
  p->p_code = strdup(p_code); // strdup関数を使って新たにメモリを確保し、内容をコピー
  p->address = strdup(address); // strdup関数を使って新たにメモリを確保し、内容をコピー
  p->p_next = NULL;
  p->a_next = NULL;
  return p;
}

// ハッシュテーブルの情報を表示
void hash_info(){
  int sum_p = 0, sum_a = 0;
  int i, length;
  struct cell *p;

  for(i=0; i<HASHSIZE; i++){
    length = 0;
    p = p_table[i];
    while(p != NULL){
      length++;
      p = p->p_next;
    }
    // printf("p_codetable[%d]: %d\n", i, length);
    sum_p += length;
  }
  // printf("p_codetable: %d\n", sum_p);

  for(i=0; i<HASHSIZE; i++){
    length = 0;
    p = a_table[i];
    while(p != NULL){
      length++;
      p = p->a_next;
    }
    // printf("a_codetable[%d]: %d\n", i, length);
    sum_a += length;
  }
  // printf("a_codetable: %d\n", sum_a);
  
}

// 郵便番号をテーブルから検索し、見つかった場合はそのセルへのポインタを返す
struct cell *search_p_code(char *p_code){
  int index = hash(p_code);
  struct cell *p = p_table[index];
  while(p != NULL){
    if(strcmp(p->p_code, p_code) == 0){
      return p;
    }
    p = p->p_next;
  }
  return NULL;
}

// 住所をテーブルから検索し、見つかった場合はそのセルへのポインタを返す
struct cell *search_address(char *address){
  int index = hash(address);
  struct cell *p = a_table[index];
  while(p != NULL){
    if(strcmp(p->address, address) == 0){
      return p;
    }
    p = p->a_next;
  }
  return NULL;
}

int main(void){
  // 郵便番号-住所対応表を読み込み
  FILE *fp;
  char PostCodeBuffer[8];
  char AddressBuffer[1024];

  // ファイル読み込み
  fp = fopen("postal.txt", "r"); // ファイル読み込みモード(r)でファイルを開く
  if(fp == NULL){
    perror("Error: postal.txtファイルがありません\n");
    return 1;
  }
  while (fscanf(fp, "%s %s", PostCodeBuffer, AddressBuffer) != EOF) {
    // printf("%s %s\n", PostCodeBuffer, AddressBuffer);

    // ハッシュテーブルにデータを追加
    struct cell *p = new_cell(PostCodeBuffer, AddressBuffer);
    int p_index = hash(PostCodeBuffer);
    int a_index = hash(AddressBuffer);

    // 郵便番号-住所対応表
    if(p_table[p_index] == NULL){
      p_table[p_index] = p;
    }else{
      p->p_next = p_table[p_index];
      p_table[p_index] = p;
    }

    // 住所-郵便番号対応表
    if(a_table[a_index] == NULL){
      a_table[a_index] = p;
    }else{
      p->a_next = a_table[a_index];
      a_table[a_index] = p;
    }

  }
  fclose(fp);

  // hash_info();

  // struct cell *p = p_table[0];
  // while(p != NULL){
  //   printf("%s %s\n", p->p_code, p->address);
  //   p = p->p_next;
  // }

  // 検索するファイルを読み込み
  // const char *textfile = "input1000.txt";
  // const char *textfile = "input10000.txt";
  const char *textfile = "input100000.txt";

  char buffer[1024];
  fp = fopen(textfile, "r");
  if(fp == NULL){
    printf("Error: %sファイルがありません\n", textfile);
    return 1;
  }

  // 計測開始
  clock_t start, end;
  double cpu_time_used;

  start = clock();
  while (fscanf(fp, "%s", buffer) != EOF) {
    // printf("%s\n", buffer);
    if(buffer[0] >= '0' && buffer[0] <= '9'){
      struct cell *p = search_p_code(buffer);
      if(p != NULL){
        printf("Code to Address : %s %s\n", buffer, p->address);
      }else{
        printf("Not Found\n");
      }
    }else{
      struct cell *p = search_address(buffer);
      if(p != NULL){
        printf("Address to Code : %s %s\n", p->p_code, buffer);
      }else{
        printf("%s is Not Found\n" , buffer);
      }
    }
  }
  end = clock();

  fclose(fp);

  cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
  printf("Time: %f\n", cpu_time_used);

  return 0;
}


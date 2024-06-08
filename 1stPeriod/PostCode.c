#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#define HASHSIZE 997
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
int hash(char *str){
  unsigned int hash = 0;
  while(*str){
    hash = (hash << 5) - hash + *str++;
  }
  return hash % HASHSIZE;
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

double sqrt_newton(double x) {
    // 初期推定値として x/2 を使用
    double guess = x / 2.0;

    // 収束条件: 推定値の2乗と目標値の差が十分に小さい
    while (fabs(guess * guess - x) > 0.00001) {
        // ニュートン法の更新式
        guess = (guess + x / guess) / 2.0;
    }

    return guess;
}

// ハッシュテーブルの情報を表示
void hash_info(){
  int sum_p = 0, sum_a = 0;
  int i, length;
  int NumOfElements_a[HASHSIZE], NumOfElements_p[HASHSIZE];
  struct cell *p;

  for(i=0; i<HASHSIZE; i++){
    length = 0;
    p = p_table[i];
    while(p != NULL){
      length++;
      p = p->p_next;
    }
    NumOfElements_p[i] = length;
    sum_p += length;
  }

  for(i=0; i<HASHSIZE; i++){
    length = 0;
    p = a_table[i];
    while(p != NULL){
      length++;
      p = p->a_next;
    }
    NumOfElements_a[i] = length;
    sum_a += length;
  }

  // ハッシュテーブルの要素数の平均、最小、最大、分散、標準偏差を求める
  double ave_p = (double)sum_p / HASHSIZE;
  double ave_a = (double)sum_a / HASHSIZE;
  double var_p = 0, var_a = 0;
  double std_p, std_a;
  int min_p = NumOfElements_p[0], max_p = NumOfElements_p[0];
  int min_a = NumOfElements_a[0], max_a = NumOfElements_a[0];
  for(i=0; i<HASHSIZE; i++){
    var_p += (NumOfElements_p[i] - ave_p) * (NumOfElements_p[i] - ave_p);
    var_a += (NumOfElements_a[i] - ave_a) * (NumOfElements_a[i] - ave_a);
    if(NumOfElements_p[i] < min_p){
      min_p = NumOfElements_p[i];
    }
    if(NumOfElements_p[i] > max_p){
      max_p = NumOfElements_p[i];
    }
    if(NumOfElements_a[i] < min_a){
      min_a = NumOfElements_a[i];
    }
    if(NumOfElements_a[i] > max_a){
      max_a = NumOfElements_a[i];
    }
  }
  var_p /= HASHSIZE;
  var_a /= HASHSIZE;
  std_p = sqrt_newton(var_p);
  std_a = sqrt_newton(var_a);

  printf("HASHSIZE: %d\n", HASHSIZE);
  printf("p_codetable: %d\n", sum_p);
  printf("a_codetable: %d\n", sum_a);
  printf("p_codetable: ave=%f, min=%d, max=%d, var=%f, std=%f\n", ave_p, min_p, max_p, var_p, std_p);
  printf("a_codetable: ave=%f, min=%d, max=%d, var=%f, std=%f\n", ave_a, min_a, max_a, var_a, std_a);
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
    p->p_next = p_table[p_index];
    p_table[p_index] = p;

    // 住所-郵便番号対応表
    p->a_next = a_table[a_index];
    a_table[a_index] = p;

  }
  fclose(fp);

  hash_info();

  // 検索するファイルを読み込み
  const char *textfile = "input1000.txt";
  // const char *textfile = "input10000.txt";
  // const char *textfile = "input100000.txt";

// カレントディレクトリにinputファイルがあれば計測を行う
if((fp = fopen(textfile, "r")) != NULL){
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

  // 計測終了
  cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
  printf("Time: %f\n", cpu_time_used);
  }

  // 標準入力から検索
  char input[1024];
  printf("Input 'exit' to finish\n");
  while(1){
    printf("Input: ");
    if(scanf("%s", input) == EOF){break;}
    if(strcmp(input, "exit") == 0){break;}
    if(input[0] >= '0' && input[0] <= '9'){
      struct cell *p = search_p_code(input);
      if(p != NULL){
        printf("Code to Address : %s %s\n", input, p->address);
      }else{
        printf("Not Found\n");
      }
    }else{
      struct cell *p = search_address(input);
      if(p != NULL){
        printf("Address to Code : %s %s\n", p->p_code, input);
      }else{
        printf("%s is Not Found\n" , input);
      }
    }
  }

  return 0;
}


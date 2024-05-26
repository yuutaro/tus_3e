#include <stdio.h>

void power2(){
  for(int i = 0; i < 10; i++){
    printf("%d\n", i * i);
  }
}

void hello(){
  char name[100];
  printf("名前を入力してください: ");
  scanf("%s", name);
  printf("こんにちは, %sさん\n", name);
}

void power2array(){
  int array[10];
  for(int i = 0; i < 10; i++){
    array[i] = i * i;
  }

  for(int i = 0; i < 10; i++){
    printf("%d\n", array[i]);
  }
}

void reverseAlphabet(){
  char alphabet[26];
  char tmp[26];
  for (int i = 0; i < 26; i++)
  {
    tmp[i] = 'a' + i;
  }

  // 配列を逆順化
  for (int i = 0; i < 26; i++){
    alphabet[i] = tmp[25 - i];
  }

  for (int i = 0; i < 26; i++){
    printf("%c\n", alphabet[i]);
  }
}

void stringSwitch(){
  // 文字列を入力
  char str[100];
  printf("文字列を入力してください\n");
  scanf("%s", str);

  // scanfで交換する二文字の戦闘からの文字数を取得
  int changeCharNum1, changeCharNum2;
  printf("交換する二文字の先頭からの文字数を入力してください\n");
  scanf("%d %d", &changeCharNum1, &changeCharNum2);
  changeCharNum1--;
  changeCharNum2--;
  
  // 交換
  char tmp = str[changeCharNum1];
  str[changeCharNum1] = str[changeCharNum2];
  str[changeCharNum2] = tmp;

  printf("交換後の文字列: %s\n", str);
}

void main(){
  // 実行する関数を選択
  printf("実行する関数を選択してください\n");
  printf("1: power2\n");
  printf("2: hello\n");
  printf("3: power2array\n");
  printf("4: reverseAlphabet\n");
  printf("5: stringSwitch\n");

  int select;
  scanf("%d", &select);

  switch(select){
    case 1:
      power2();
      break;
    case 2:
      hello();
      break;
    case 3:
      power2array();
      break;
    case 4:
      reverseAlphabet();
      break;
    case 5:
      stringSwitch();
      break;
    default:
      printf("無効な選択です\n");
  }
}
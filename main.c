#include <stdio.h>
//#include <Windows.h>

int entn(char s){//Function to enter number
  printf("Enter %c: ",s);int c=0;scanf("%d",&c);return c;
}

int main(){
  //SetConsoleCP(1251);
  //SetConsoleOutputCP(1251);
  printf("Hello world\n");
  printf("Привет мир\n\n");

  int a=entn('a');int b=entn('b');
  if (b!=0)printf("Result is %f\n",a/(double)b);
  else printf("Divided by zero\n");

  printf("\n");
  if (a==b){
    printf("%d",2*a*b);
  }
  else if (a<b){
    printf("%d",a*a+b*b);
  }
  else{
    printf("%d",a*a-b*b);
  }

  printf("\n");return 0;
}

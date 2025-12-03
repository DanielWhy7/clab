#include <stdio.h>
#include <malloc.h>
//#include <windows.h>
//#define _CRT_SECURE_NO_WARNINGS

const int MAX_N=100;
int n,m;

int getValidNumber(int n){
  if(n<=0 || n>MAX_N)return 3;return n;
}

int** getMatrix(){
  printf("Enter amount of rows(<=100): ");
  scanf("%d",&n);n=getValidNumber(n);
  printf("Enter amount of column(<=100): ");
  scanf("%d",&m);m=getValidNumber(m);
  int** arr=(int**)malloc(sizeof(int*) * n);

  for(int i=0;i<n;i++)arr[i]=(int*)malloc(sizeof(int) * m);

  for(int i=0;i<n;i++){
    for(int j=0;j<m;j++){
      printf("Enter element[%d][%d]: ",i,j);
      scanf("%d",&arr[i][j]);
    }
  }
  return arr;
}

void spiralVector(int** arr){
  if(n==m){
    int vector[n*n];
    int top=0,bottom=n-1,left=0,right=n-1,idx=0,i=0;
    while(top<=bottom && left<=right){
      for(int i=left;i<=right;++i)vector[idx++]=arr[top][i];++top;
      for(int i=top;i<=bottom;++i)vector[idx++]=arr[i][right];--right;
      if(top<=bottom){for(int i=right;i>=left;--i)vector[idx++]=arr[bottom][i];--bottom;}
      if(left<=right){for(int i=bottom;i>=top;--i)vector[idx++]=arr[i][left];++left;}
    }
    for(i=0;i<n*n;i++)printf("%d ",vector[i]);
  }
  else printf("Not supported");
}

void printMatrix(int** arr){
  for(int i=0;i<n;i++){
    for(int j=0;j<m;j++){
      printf("[%d] ",arr[i][j]);
    }
    printf("\n");
  }
}

int main(){
  //SetConsoleCP(1251);
  //SetConsoleOutputCP(1251):
  int** arr=getMatrix();
  printf("The matrix:\n");
  printMatrix(arr);
  printf("D(n^2):\n");
  spiralVector(arr);

  printf("\n");for(int i=0;i<n;i++)free(arr[i]);free(arr);return 0;
}

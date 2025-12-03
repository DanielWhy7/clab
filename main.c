#include <stdio.h>
#include <math.h>

unsigned long long double_fact(int n){
  if (n<=0)return 1;
  unsigned long long res=1;
  for(int i=n;i>0;i-=2)
    res*=i;
  return res;
}

double f_series(double x,double epsilon){
  double sum=x;
  double term;
  int n=1;
  do{
    unsigned long long num=double_fact(2*n - 1);
    unsigned long long den=double_fact(2*n);
    double power=pow(x,2*n+1);
    term=pow(-1,n)*(double)num/(den*(2*n+1))*power;
    sum+=term;
    n++;
  }while(fabs(term)>epsilon && n<100);
  return sum;
}

void table_fx(double a,double b,double h,double epsilon){
  printf("\nx\tf(x) series\tf(x) exact\n");
  for(double x=a;x<=b+1e-8;x+=h){
    double f_val=f_series(x,epsilon);
    double f_exact=log(x+sqrt(1+x*x));
    printf("%.5f\t%.8f\t%.8f\n",x,f_val,f_exact);
  }
}

int main(){
  double a,b,h,epsilon;
  char repeat;
  do{
    printf("Enter a (start of interval, [0,1]): ");
    scanf("%lf",&a);
    printf("Enter b (end of interval, [0,1]): ");
    scanf("%lf",&b);
    printf("Enter h (step > 0): ");
    scanf("%lf",&h);
    printf("Enter epsilon (accuracy > 0): ");
    scanf("%lf",&epsilon);

    table_fx(a,b,h,epsilon);

    printf("\nCalculate again with new parameters? (y/N): ");
    scanf(" %c",&repeat);
  }while(repeat=='y'||repeat=='Y');
  return 0;
}

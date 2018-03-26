#include<unistd.h>
#include<time.h>
#include<stdio.h>
#include<sys/time.h>
void sig(int sn);
int main(void)

{
    while(1)
    {
      sig(1);
      sig(2);
      sig(3);
      sig(4);
    }
}
void sig(int sn){
 
  time_t t=time(NULL);//time() takes current time
  printf("%d\n",sn);
  while((time(NULL)-t)<30);//loop runs until difference b/w current time and time before loop is less than 30s
}

        

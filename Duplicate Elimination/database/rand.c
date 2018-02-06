#include<stdio.h>
#include <time.h>
#include<stdlib.h> /* required for randomize() and random() */
FILE *f1;
void main(void)
{
   time_t t;
   int num,i,loop;
   f1=fopen("output.dta", "wt");
   for(loop=0;loop<1024/12;loop++)
   {  
     for(i=0;i<3;i++)
     {
       num = rand() % 100;
       if(num/10!=0)
          fprintf(f1, "%d ", num);
       else
          fprintf(f1, "0%d ", num);
     }
     num = rand() % 100;
     if(num/10!=0)
        fprintf(f1, "%d\n", num);
     else
        fprintf(f1, "0%d\n", num);
   }
   fclose(f1);
}



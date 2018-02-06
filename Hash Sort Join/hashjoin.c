/*Program to implement Hash Join*/
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define blocksize 5

void sublists(char *,char);
void hashfunction(char ch,int last);
int ***main_memory,mblocks,*headers;
FILE *fp,*fo,*in[200];

int main()
{   
   char file_r[15],file_s[15];
   int i,j,rfiles,sfiles;
   printf("Enter the filename R:");
   scanf("%s",file_r);
   printf("Enter the filename S:");
   scanf("%s",file_s);
   printf("Enter Main memory blocks:");/*Given that each block can store 100 tuples*/
   scanf("%d",&mblocks);
   main_memory = (int ***)malloc(mblocks*sizeof(int**));
   for (i=0;i<mblocks;i++) 
   {
          main_memory[i]=(int**)malloc(blocksize*sizeof(int *));
          for (j=0;j<blocksize;j++) 
          {
              main_memory[i][j]=(int*)malloc(2*sizeof(int));
          }
   }
   sublists(file_r,'r');
   sublists(file_s,'s');
   return 0;
}
void sublists(char *file,char ch)
{
     char a[7],b[7],fi[10]; 
     int i,j,readover=0;
     headers=(int *)malloc(blocksize-1*sizeof(int));
     for(i=0;i<mblocks-1;i++)
       headers[i]=0;

     fp=fopen(file,"r");
     for(i=0;i<mblocks-1;i++)
     {
         
         sprintf(fi,"%c%d",ch,i);
         //printf("%s\n",fi);
         in[i]=fopen(fi,"a");                      
     }
     while(1)
     {
          //printf("For Loop Started\n");
          for(j=0;j<blocksize;j++)
          {
               if(fscanf(fp,"%s %s\n",a,b)!=EOF)
               {
                   main_memory[mblocks-1][j][0]=atoi(a);
                   main_memory[mblocks-1][j][1]=atoi(b);
                   //printf("%d %d\n",main_memory[mblocks-1][j][0],main_memory[mblocks-1][j][1]);
               }
               else
               {
                   readover=1;
                   break;
               }
          }
          //printf("For Loop Ended\n");
          //printf("Hash Started\n");
          hashfunction(ch,j);
          //printf("Hash Ended\n");
          if(readover)
             break;
     }
     //printf("File Reading Done\n");
     for(i=0;i<mblocks-1;i++)
     {
          for(j=0;j<headers[i];j++)
          {
              fprintf(in[i],"%d %d\n",main_memory[i][j][0],main_memory[i][j][1]);
              //printf("%d %d\n",main_memory[i][j][0],main_memory[i][j][1]);
          }
     }
     free(headers);
     
}
void hashfunction(char ch,int last)
{          
    int i,j,c,element,hdptr;
    if(ch=='r')
       c=1;
    else
       c=0;
    for(j=0;j<last;j++)
    {
          element=main_memory[mblocks-1][j][c];
          //printf("E:%d ",element);
          element= element % (mblocks-1);
          //printf("H:%d ",element);
          hdptr=headers[element];
          //printf("P:%d\n",hdptr);
          if(hdptr<blocksize)
          {
                main_memory[element][hdptr][0]=main_memory[mblocks-1][j][0];
                main_memory[element][hdptr][1]=main_memory[mblocks-1][j][1];
                hdptr=(hdptr+1) % (blocksize);
                headers[element]=hdptr;
          }
          if(hdptr==0)
          {
                //printf("\n\nHashed elements\n");
                for(i=0;i<blocksize;i++)
                {
                       //printf("%d %d\n",main_memory[element][i][0],main_memory[element][i][1]);
                       fprintf(in[element],"%d %d\n",main_memory[element][i][0],main_memory[element][i][1]);
                }
                //printf("\n\nHashed elements Printed\n");
          }
    }
}





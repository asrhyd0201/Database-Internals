/*Program to implement 2phase 2-way merge sort*/
/*Status : Merging yet to be implemented*/
/*   DONE  */
#include<stdio.h>
#include<string.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<stdlib.h>
#define recordsize 101

void sortrecords(char **,int);
void merging(char **,int);
int string_cmp(const void *a,const void *b)
{
     const char **ia=(const char**)a;
     const char **ib=(const char**)b;
     return strcmp(*ia,*ib);    
} 
int main()
{
    int ma,i,fileflag=0,s,ki=1;
    char file[30],**main_mem,ch[7],line[recordsize];
    FILE *real_fp,*in[2000];
    printf("Enter the filename to sort:");
    scanf("%s",file);
    real_fp=fopen(file,"r");
    if(real_fp==NULL)
    {
         printf("Cannot open file\n");
         exit(1);
    }
    printf("Enter the number of records to be in main memory:");
    scanf("%d",&ma);
 
    main_mem=(char**)malloc(ma*sizeof(char*));    /*  Main Memory Allocation  */
    for(i=0;i<ma;i++)
    {
                main_mem[i]=(char *)malloc(recordsize*sizeof(char));
    }
    while(1)
    {
        i=ki++;
        sprintf(ch,"%d",i);
        //in[ki]=fopen(ch,"r+");
        in[ki]=fopen(ch,"w+");                                 /*   Creating Temporary Files   */
        if(in[ki]==NULL)
        {
               printf("cannot open file");
               exit(1);
        }
        for(s=0;s<ma;s++)
        {
               if(fgets(line,sizeof(line),real_fp)==NULL)
               {
                    fileflag=1;
                    break;
               }     
               strcpy(main_mem[s],line);
        }
        sortrecords(main_mem,s);               /* Sorting Records using Qsort in Main Memory*/
        for(i=0;i<s;i++)
          fputs(main_mem[i],in[ki]);
        fclose(in[ki]);
        if(fileflag)
        {
            fclose(real_fp);                           /*  File reading has been done break  */
            break;
        }
   }
   //printf("files=%d\n",ki); 
   merging(main_mem,ki-1);            /* Merging yet to be implemented */
    
   for(i=0;i<ma;i++)                          /*   Main Memory Deallocation  */
         free(main_mem[i]); 
    free(main_mem);
    return 0;
}
void sortrecords(char **main_mem,int ma)       /* Q Sort */
{
       qsort(main_mem,ma,sizeof(char*),string_cmp);
}
void merging(char **main_mem,int files)
{
       int i,j,count=0,min,flag=0;
       char line[recordsize],ch[7];
       FILE *op,*in[20000];
       for(i=1;i<=files;i++)
       {
                sprintf(ch,"%d",i);
                in[i]=fopen(ch,"r");
       }
       op=fopen("output4.txt","w");
          
       for(i=1;i<=files;i++)
       {
                 if(in[i]!=NULL)
                 {
                       if(fgets(line,sizeof(line),in[i])!=NULL)   
                         strcpy(main_mem[i],line);
                       else
                       {
                           strcpy(main_mem[i],"****");   
                           fclose(in[i]);
                           count++;
                       }
                 }                     
       }
       while(count!=files)
       {
              i=1;
              while(i<=files)
              {
                 if(strcmp(main_mem[i],"****")!=0)
                 {
                     min=i;
                     break;
                 }
                 else
                 {
                    i++;
                    if(i==files+1)
                         flag=1;
                 }
              } 
              if(flag==1)
                 break;
              for(j=i+1;j<=files;j++)
              {
                 if(strcmp(main_mem[j],"****")!=0)
                 {           
                     if(strcmp(main_mem[min],main_mem[j])>0)
                          min=j;
                 }                                          
              } 
           
              fputs(main_mem[min],op);
              if(in[min]!=NULL)
              {
                       if(fgets(line,sizeof(line),in[min])!=NULL)   
                         strcpy(main_mem[min],line);
                       else
                       {
                           strcpy(main_mem[min],"****");   
                           fclose(in[min]);
                           count++;
                       }
              }          
       }
       fclose(op);
}


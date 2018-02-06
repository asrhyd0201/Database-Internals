/*Program to implement sort merge join*/
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>
#define blocksize 5/*100 tuples for a block*/

void sorting(int i,int j,char ch);
void quicksort(int start,int last,int c);
int sublist(char *file,char ch);
void merging(int r,int s);
void perform_join(int join_rhead,int join_rend,int join_shead,int join_send,int *limit);
int fill_for_join(int head,int end,int join_head,int *headers,int *limit,int c,int minimum);
void initialise_mainmemory(char ch,int head,int end,int *limit);
int minimum_value(int head,int end,int *headers,int *limit,int c);
int movingheaders(int minimum,int head,int end,int c,int *headers,int *limit);

int ***main_memory,mblocks;
FILE *fp,*fo,*merge[100];
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
   rfiles=sublist(file_r,'r');
   sfiles=sublist(file_s,'s');
   printf("r=%d s=%d\n",rfiles,sfiles);
   merging(rfiles,sfiles);   
   return 0;
}
void merging(int r,int s)
{
     int tblocks,*headers,*limit;
     int rhead,rend,shead,send,join_rhead,join_rend,join_shead,join_send;
     int minimum_r,minimum_s,minimum,file_no=1;
     char ch[10],a[5],b[5],file[10];
           
     int i,j;
     
     tblocks=r+s;
     rhead=0;rend=r;
     shead=r;send=tblocks;
     printf("Total blocks=%d  Rhead=%d Rend=%d Shead=%d Send=%d\n",tblocks,rhead,rend,shead,send);
 
     limit=(int *)malloc(blocksize*sizeof(int));
     headers=(int *)malloc(blocksize*sizeof(int));
     for(i=0;i<blocksize;i++)
        headers[i]=0;
     for(i=0;i<blocksize;i++)
        limit[i]=blocksize;

     if(r+s+2>mblocks)
     {
        printf("perform Nested loop join\n");
        exit(0);
     }
     fo=fopen("OUTPUT","w");
     initialise_mainmemory('r',rhead,rend,&limit[0]);
     //printf("R over\n");
     initialise_mainmemory('s',shead,send,&limit[0]);
     //printf("S over\n");
     while(1)
     {
          minimum_r=minimum_value(rhead,rend,&headers[0],&limit[0],1);
          printf("R=%d ",minimum_r);
          minimum_s=minimum_value(shead,send,&headers[0],&limit[0],0);
          printf("S=%d\n",minimum_s); 
          if(minimum_r == -1 || minimum_s == -1)
           break;
          else if(minimum_r==minimum_s)
          {
                 join_rhead=tblocks;
                 join_rend=fill_for_join(rhead,rend,join_rhead,&headers[0],&limit[0],1,minimum_r);
                 join_shead=join_rend+1;
                 //printf("Rhead=%d Rend=%d\n",join_rhead,join_rend);
                 /*for(i=join_rhead;i<=join_rend;i++)
                 {
                        for(j=0;j<limit[i];j++)

                        {
                              printf("%d %d\n",main_memory[i][j][0],main_memory[i][j][1]);
                        }
                 }*/
                 join_send=fill_for_join(shead,send,join_shead,&headers[0],&limit[0],0,minimum_r);
                 
                 //printf("Shead=%d Send=%d\n",join_shead,join_send);
                 /*for(i=join_shead;i<=join_send;i++)
                 {
                        for(j=0;j<limit[i];j++)
                        {
                              printf("%d %d\n",main_memory[i][j][0],main_memory[i][j][1]);
                        }
                 }*/

                 perform_join(join_rhead,join_rend,join_shead,join_send,&limit[0]);
          }
          else
          {
                 if(minimum_r>minimum_s)
                    minimum=movingheaders(minimum_s,shead,send,0,&headers[0],&limit[0]);
                 else
                    minimum=movingheaders(minimum_r,rhead,rend,1,&headers[0],&limit[0]);
                 if(minimum==-1)
                    break;
          }
     }   
}
int movingheaders(int minimum,int head,int end,int c,int *headers,int *limit)
{
         int i,j,k,readover=0;
         char a[5],b[5];
         for(i=head;i<end;i++)
         {
            for(j=headers[i];j<limit[i];j++)
            {
                    if(main_memory[i][j][c]!=minimum)
                    {
                           headers[i]=j;
                           break;
                    }
            }
            if(j==blocksize)
            {
                 for(k=0;k<blocksize;k++)
                 {
                       if(fscanf(merge[i],"%s %s\n",a,b)!=EOF)
                       {
                               main_memory[i][k][0]=atoi(a);
                               main_memory[i][k][1]=atoi(b);
                       }
                       else
                       {
                               limit[i]=j;
                               readover=1;
                               break;
                       }
                 }
                 headers[i]=0;
                 j--;
            }
         }
        if(readover)
         return -1;
       return 0;
}
int fill_for_join(int head,int end,int join_head,int *headers,int *limit,int c,int minimum)
{
    int i,j,k=0,readingover=0,hdptr;
    char a[7],b[7];   
    for(i=head;i<end;i++)
    {
        hdptr=headers[i]; 
        while(hdptr<=limit[i])
        {
            /*Refilling buffers of r an s if over*/
            if(hdptr==blocksize)
            {
                 for(j=0;j<blocksize;j++)
                 {
                       if(fscanf(merge[i],"%s %s\n",a,b)!=EOF)
                       {
                               main_memory[i][j][0]=atoi(a);
                               main_memory[i][j][1]=atoi(b);
                       }
                       else
                       {
                               limit[i]=j;
                               break;
                       }
                 }
                 headers[i]=0;
                 hdptr=0;
             }
             if(main_memory[i][hdptr][c]==minimum)
             {
                 main_memory[join_head][k][c]=main_memory[i][hdptr][c];
                 main_memory[join_head][k][(c+1)%2]=main_memory[i][hdptr][(c+1)%2];
                 k++;
                 hdptr++;
                 headers[i]=hdptr;
                 limit[join_head]=k;
                 if(k%blocksize==0)
                 {
                     join_head++;
                     k=0;                     
                 }
             }
             else
                break;
        }
    }
    return join_head;        
}
int minimum_value(int head,int end,int *headers,int *limit,int c)
{
       int i,minimum;
       i=head;
       while(i!=end)
       {
            if(headers[i]<limit[i])
            {
                minimum=main_memory[i][headers[i]][c];
                break;
            }
            else
                i++;
       }
       if(i==end)
         return -1;
       for(i=head+1;i<end;i++)
       {
            if(headers[i]<limit[i])
            {   
                if(minimum > main_memory[i][headers[i]][c])
                   minimum=main_memory[i][headers[i]][c];
            }
       }
       return minimum;
}
void initialise_mainmemory(char ch,int head,int end,int *lim)
{
     
     int i,j,file_no=1;  
     char a[7],b[7],file[10];
     //printf("In\n");
     for(i=head;i<end;i++)
     {
        
        sprintf(file,"%c%d",ch,file_no);
        //printf("file=%c%d\n",ch,file_no);
        merge[i]=fopen(file,"r");
        file_no++;
        for(j=0;j<blocksize;j++)
        {
            
            if(fscanf(merge[i],"%s %s\n",a,b)!=EOF)
            {
                  main_memory[i][j][0]=atoi(a);
                  main_memory[i][j][1]=atoi(b);
                  //printf("%s %s\n",a,b);
            }
            else
            {
                lim[i]=j;
                break;
            }
        }
     }
}
void perform_join(int join_rhead,int join_rend,int join_shead,int join_send,int *limit)
{
      int i,j,m,n;
      for(i=join_rhead;i<=join_rend;i++)
      {
            for(j=0;j<limit[i];j++)
            {
                 for(m=join_shead;m<=join_send;m++)
                 {
                       
                     for(n=0;n<limit[m];n++)
                     {
                            //printf("%d %d %d\n",main_memory[i][j][0],main_memory[i][j][1],main_memory[m][n][1]);
                            fprintf(fo,"%d %d %d\n",main_memory[i][j][0],main_memory[i][j][1],main_memory[m][n][1]);
                     }
                 }
            }
      }
      
}

int sublist(char *file,char ch)
{
  FILE *in[100];
  int i=0,j=0,readover=0,fileno=1,k,l;
  char l_file[7],temp[15],a[6],b[6];
  fp=fopen(file,"r");
  while(1)
  {     
     for(i=0;i<mblocks;i++)
     {
           for(j=0;j<blocksize;j++)
           {
               if(fscanf(fp,"%s %s\n",a,b)!=EOF)
               {
 
                   main_memory[i][j][0]=atoi(a);
                   main_memory[i][j][1]=atoi(b);
               }
               else
               {
                   if(j==0)
                    j=blocksize;
                   readover=1;
                   break;
               }
           }
           if(readover)
             break;  
     }
     sorting(i,j,ch);
     sprintf(l_file,"%c%d",ch,fileno);
     in[fileno]=fopen(l_file,"w"); 
     for(k=0;k<i;k++)
     {
           for(l=0;l<j;l++)
           {
                   fprintf(in[fileno],"%d %d\n",main_memory[k][l][0],main_memory[k][l][1]);
           }
     }
     fclose(in[fileno]);
     fileno++;  
     if(readover)
       break;        
  }
  return fileno-1;   
}
void sorting(int i,int j,char ch)
{
    int start=0,last,c;
    last=i*blocksize+(j%blocksize);
    if(ch=='r')
       c=1;
    else
       c=0; 
    quicksort(start,last-1,c);
}
void quicksort(int start,int last,int cr)
{
    int pivot,piv_i,piv_j;
    int a,b,c,lb,ub,lbi,lbj,ubi,ubj,temp0,temp1,cmp;
   
    if(start<last)
    {
       
       lb=start+1;
       ub=last;
       piv_i=start/blocksize;
       piv_j=start%blocksize;
       a=main_memory[piv_i][piv_j][0];
       b=main_memory[piv_i][piv_j][1];
       lbi=lb/blocksize;
       lbj=lb%blocksize;
       ubi=ub/blocksize;
       ubj=ub%blocksize;
       if(cr==1)
         cmp=b;
       else
         cmp=a;
       while(1)
       {  
            while(lb<last && main_memory[lbi][lbj][cr]<=cmp)
            {     
                    lb++;
                    lbi=lb/blocksize;
                    lbj=lb%blocksize;
                    
            } 
            while(ub>start && main_memory[ubi][ubj][cr]>=cmp)
            {
                    ub--;
                    ubi=ub/blocksize;
                    ubj=ub%blocksize;
                  
            }
            if(lb<ub)
            {
                 temp0=main_memory[lbi][lbj][0];
                 temp1=main_memory[lbi][lbj][1];

                 main_memory[lbi][lbj][0]=main_memory[ubi][ubj][0];
                 main_memory[lbi][lbj][1]=main_memory[ubi][ubj][1];
                 main_memory[ubi][ubj][0]=temp0;
                 main_memory[ubi][ubj][1]=temp1;             
            }
            else
              break;
       }
       
       main_memory[piv_i][piv_j][0]=main_memory[ubi][ubj][0];
       main_memory[piv_i][piv_j][1]=main_memory[ubi][ubj][1];
       
       main_memory[ubi][ubj][0]=a;
       main_memory[ubi][ubj][1]=b;
             
       quicksort(start,ub-1,cr);
       quicksort(ub+1,last,cr);
    }
}

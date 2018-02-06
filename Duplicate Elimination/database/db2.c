#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define recordsize 13
#define M 5

struct node{
	int n;               /* n < M No. of keys in node will always less than order of B tree */
	int keys[M-1];       /*array of keys*/
	struct node *p[M];   /* (n+1 pointers will be in use) */
}*root=NULL;
enum KeyStatus {Duplicate,Success,InsertIt};
int insert(int key);
void display(struct node *root,int);
enum KeyStatus ins(struct node *r, int x, int* y, struct node** u);
int searchPos(int x,int *key_arr, int n);
int convert_to_int(char *);
int main()
{
    int mem_blocks,blk_size,records,i,j,readover=0,key,ret,row=1,col=0,full=0;
    FILE *fp,*fo;
    char line[recordsize],file[20],ch,***main_mem;
    printf("Enter the filename:");
    scanf("%s",file);
    printf("Enter number of main memory blocks:");
    scanf("%d",&mem_blocks);
    printf("Enter the Block size:");
    scanf("%d%c",&blk_size,&ch);
    /*if(ch=='k')
       blk_size=blk_size*1024;
    else if(ch=='m')
       blk_size=blk_size*1024*1024;*/
    records=blk_size/12;
    printf("records=%d\n",records);
   
    main_mem = (char ***)malloc(mem_blocks*sizeof(char**));
    for (i=0;i<mem_blocks;i++) 
    {
          main_mem[i]=(char**)malloc(records*sizeof(char *));
          for (j=0;j<records;j++) 
          {
              main_mem[i][j]=(char*)malloc(recordsize*sizeof(char));
          }
    }    
    fp=fopen(file,"r");
    fo=fopen("out","w");
    if(fp==NULL)
    {
         printf("Cannot open input file\n");
         exit(1);
    }
    if(fp==NULL)
    {
         printf("Cannot open output file\n");
         exit(1);
    }
    while(1)
    {
        for(i=0;i<records;i++)
        {
               if(fgets(line,sizeof(line),fp)==NULL)
               {
                    readover=1;
                    break;
               }     
               strcpy(main_mem[0][i],line);
        }
        /*for(i=0;i<records;i++)
        {
            printf("i=%d %s",i+1,main_mem[0][i]);
        }*/
        for(j=0;j<i;j++)
        {
                key=convert_to_int(main_mem[0][j]);
                ret=insert(key);
                if(ret)
                {
                   if(row<mem_blocks)
                   {
                        if(col<records)
                        {
                            strcpy(main_mem[row][col],main_mem[0][j]);
                            col++;
                        }
                        else
                        {
                             col=0;
                             row++;
                             if(row<mem_blocks)
                             {  
                                 strcpy(main_mem[row][col],main_mem[0][j]);
                                 col++;
                             }
                             else
                             {
                                  full=1;
                                  strcpy(line,main_mem[0][j]);
                             }
                       }    
                  }
               }
               if(full==1)
               {
                  for(row=1;row<mem_blocks;row++)
                  {
                             for(col=0;col<records;col++)
                             {
                                     fputs(main_mem[row][col],fo);
                             }
                  }
                  full=0;
                  row=1;
                  strcpy(main_mem[1][0],line);
                  col=1;
               }
        }
        if(readover)
            break;
    }
    if(full==0)
    {
           for(i=1;i<=row;i++)
           {
                  for(j=0;j<=col;j++)
                  {
                             fputs(main_mem[i][j],fo);
                  }
           }
    }
    //display(root,0);
    fclose(fp);
    fclose(fo);
    return 0;
}
int convert_to_int(char *ptr)
{
   int i,j=0;
   char c[10];
   for(i=0;i<12;i++)
   {
       c[j++]=ptr[i++];
       c[j++]=ptr[i++];
   }
   c[j]='\0';
   return atoi(c);    
}
int insert(int key)
{
	struct node *newnode;
	int upKey;
	enum KeyStatus value;
	value = ins(root, key, &upKey, &newnode);
	if (value == Duplicate)
		return 0;
	if (value == InsertIt)
	{
		struct node *uproot = root;
		root=malloc(sizeof(struct node));
		root->n = 1;
		root->keys[0] = upKey;
		root->p[0] = uproot;
		root->p[1] = newnode;
	}/*End of if */
        return 1;
}/*End of insert()*/
enum KeyStatus ins(struct node *ptr, int key, int *upKey,struct node **newnode)
{
	struct node *newPtr, *lastPtr;
	int pos, i, n,splitPos;
	int newKey, lastKey;
	enum KeyStatus value;
	if (ptr == NULL)
	{
		*newnode = NULL;
		*upKey = key;
		return InsertIt;
	}
	n = ptr->n;
	pos = searchPos(key, ptr->keys, n);
	if(pos < n && key == ptr->keys[pos])
		return Duplicate;
	value = ins(ptr->p[pos], key, &newKey, &newPtr);
	if (value != InsertIt)
		return value;
	/*If keys in node is less than M-1 where M is order of B tree*/
	if (n < M - 1)
	{
		pos = searchPos(newKey, ptr->keys, n);
		/*Shifting the key and pointer right for inserting the new key*/
		for (i=n; i>pos; i--)
		{
			ptr->keys[i] = ptr->keys[i-1];
			ptr->p[i+1] = ptr->p[i];
		}
		/*Key is inserted at exact location*/
		ptr->keys[pos] = newKey;
		ptr->p[pos+1] = newPtr;
		++ptr->n; /*incrementing the number of keys in node*/
		return Success;
	}/*End of if */
	/*If keys in nodes are maximum and position of node to be inserted is last*/
	if (pos == M - 1)
	{
		lastKey = newKey;
		lastPtr = newPtr;
	}
	else /*If keys in node are maximum and position of node to be inserted is not last*/
	{
		lastKey = ptr->keys[M-2];
		lastPtr = ptr->p[M-1];
		for (i=M-2; i>pos; i--)
		{
			ptr->keys[i] = ptr->keys[i-1];
			ptr->p[i+1] = ptr->p[i];
		}
		ptr->keys[pos] = newKey;
		ptr->p[pos+1] = newPtr;
	}
	splitPos = (M - 1)/2;
       (*upKey) = ptr->keys[splitPos];

	(*newnode)=malloc(sizeof(struct node));/*Right node after split*/
	ptr->n = splitPos; /*No. of keys for left splitted node*/
	(*newnode)->n = M-1-splitPos;/*No. of keys for right splitted node*/
	for (i=0; i < (*newnode)->n; i++)
	{
		(*newnode)->p[i] = ptr->p[i + splitPos + 1];
		if(i < (*newnode)->n - 1)
			(*newnode)->keys[i] = ptr->keys[i + splitPos + 1];
		else
			(*newnode)->keys[i] = lastKey;
	}
	(*newnode)->p[(*newnode)->n] = lastPtr;
	return InsertIt;
}
int searchPos(int key, int *key_arr, int n)
{
	int pos=0;
	while (pos < n && key > key_arr[pos])
		pos++;
	return pos;
}/*End of searchPos()*/

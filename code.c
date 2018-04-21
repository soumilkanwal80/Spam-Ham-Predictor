//  Spam Ham Predictor
//  Created by Soumil on 18/04/2018.
//  Copyright © 2018 Soumil. All rights reserved.
//  Command Line Arguments: None

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<ctype.h>

int nowords=0, k;

// struct clusters
// {
//     char a[9000][100];
//     int pos;
// };

int absolute(int x)
{
    if(x>0)
        return x;
    else
        return -x;
}

int min(int a, int b, int c)
{
    int x=a;
    
    if(b<x)
        x=b;
    
    if(c<x)
        x=c;
    
    return x;
}

int *getPointer(int *iArray, int c, int r, int ncols)
{
    return iArray+c+r*(ncols+1);
}


int getVal(int *iArray, int c, int r, int ncols)
{
    int *p=getPointer(iArray, c, r, ncols);
    return *p;
}

void putVal(int *iArray, int c, int r, int ncols, int x)
{
    int *p=getPointer(iArray, c, r, ncols);
    *p=x;
}


int returnDistance(char *string1, char *string2)
{
    int *iArray, n=strlen(string1), m=strlen(string2), i, j, cost;
    for(i=0;i<n;i++)
        *(string1+i)=tolower(*(string1+i));
    for(i=0;i<m;i++)
        *(string2+i)=tolower(*(string2+i));
    
    if(n==0)
        return m;
    
    if(m==0)
        return n;
    
    iArray=(int *)malloc(sizeof(int)*(n+1)*(m+1));
    for(i=0;i<=n;i++)
        putVal(iArray, i, 0, n, i);
    
    for(j=0;j<=m;j++)
        putVal(iArray, 0, j, n, j);
    
    for(i=1;i<=n;i++)
    {
        
        for(j=1;j<=m;j++)
        {
            if(string1[i-1]==string2[j-1])
                cost=0;
            else
                cost=1;
            
            int temp=min(getVal(iArray, i-1, j, n)+1, getVal(iArray, i, j-1, n)+1, getVal(iArray, i-1, j-1,n)+cost);
            putVal(iArray, i, j, n, temp);
        }
    }
    
    int distance=getVal(iArray, n, m , n);
    free(iArray);
    return distance;
}

int* L1distance(int **histogram, int **test)
{
    int i, j;
    //int temphist[541][k];
    
    
    
    int **temphist=(int **)malloc(sizeof(int *)*541);
    for(i=0;i<541;i++)
        temphist[i]=(int *)malloc(sizeof(int)*k);
    

    
    for(i=0;i<541;i++)
    {
        for(j=0;j<k;j++)
        {
            temphist[i][j]=histogram[i][j];
        }
    }
    
    char *word=(char *)malloc(sizeof(char)*100);

    int *mindiff=(int *)malloc(sizeof(int)*543);

    
    int testind=0;
    while(testind<543)
    {
        int minindex=0, mini=10000;
        for(i=0;i<541;i++)
        {
            int sum=0;
            
            for(j=0;j<k;j++)
            {
                histogram[i][j]-=test[testind][j];
                sum+=histogram[i][j]*histogram[i][j];
            }
            
            if(sum<mini)
            {
                minindex=i;
                mini=sum;
            }
        }

        mindiff[testind]=minindex;

        
        for(i=0;i<541;i++)
        {
            for(j=0;j<k;j++)
            {
                histogram[i][j]=temphist[i][j];
            }
        }
        testind++;
    }

    return mindiff;
}

int **computeHistogram(char **string, char **centres)
{
    int i=0;
    int **histogram=(int **)malloc(sizeof(int *)*541);
    for(i=0;i<541;i++)
        histogram[i]=(int *)malloc(sizeof(int)*k);
    // int histogram[541][k];
    int j=0;
    for(i=0;i<541;i++)
    {
        for(j=0;j<k;j++)
        {
            histogram[i][j]=0;
        }
    }
    
    // for(i=0;i<k;i++)
    // {
    //     printf("%s\n", centres[i]);
    // }
    
    FILE *fTrain=fopen("train2.txt", "r");
    
    char cLine[10000], word[100];
    if(fTrain==NULL)
        printf("File Not Found");
    int count=0;
    i=0;
    while(fscanf(fTrain, "%s", word)>0)
    {
        if(strcmp(word, ",ham")==0 || strcmp(word, ",spam")==0)
        {
            //count+=1;
            i++;
        }
        
        else
        {
            //count+=1;
            for(j=0;j<k;j++)
            {
                if(strcmp(word, centres[j])==0)
                    histogram[i][j]++;
            }
        }
    }
    //printf("%d", count);
    
    // for(i=0;i<541;i++)
    // {
    //     for(j=0;j<k;j++)
    //     {
    //         printf("%d ", histogram[i][j]);
    //     }
    //     printf("\n");
    // }

    return histogram;
    
}

char **updateKMeans(char **string, int **clusters, int *position, char **centres)
{
    int i;
    // for(i=0;i<k;i++)
    // {
    //     printf("%s\n", centres[i]);
    // }
    // printf("\n");
    for(i=0;i<k;i++)
    {
        int temp=0;
        while(temp!=position[i])
        {
            int sum=0, mini=10000;
            int temp2=position[i];
            char word[100];
            strcpy(word, string[clusters[temp][i]]);
            while(temp2!=position[i])
            {
                char word2[100];
                strcpy(word2, string[clusters[temp2][i]]);
                sum+=returnDistance(word, word2);
                temp2++;
            }
            if(sum<mini)
            {
                mini=sum;
                strcpy(centres[i], word);
            }
            temp++;
        }
    }
    
    for(i=0;i<k;i++)
        printf("%d.) %s\n", i+1, centres[i]);
    
    return centres;
    
    
    
    //     for(i=0;i<k;i++)
    //     {
    //         printf("%s\n", centres[i]);
    //     }
}




char** computeKMeans(int k, char **string, char **centres)
{
    int i;

    int **clusters=(int **) malloc(sizeof(int *)*9000);
    for(i=0;i<9000;i++)
        clusters[i] = (int *)malloc(sizeof(int)*k);
    
    int *position = (int *) malloc(sizeof(int)*k);
    
    //int position[k];       //storing string index
    int  j;
    for(i=0;i<k;i++)
        position[k]=0;
    //srand(time(0));

    
    int *d=(int *)malloc(sizeof(d)*k);
    
    for(i=0;i<nowords;i++)
    {
        for(j=0;j<k;j++)
        {
            
            d[j]=returnDistance(centres[j], string[i]);
        }
        int mini=10000, index=0;
        for(j=0;j<k;j++)
        {
            if(mini>d[j])
            {
                mini=d[j];
                index=j;
            }
        }
        
        clusters[position[index]][index]=i;
        position[index]++;
    }
    
    
    
    char **centres2=updateKMeans(string, clusters, position, centres);
    
    return centres2;
    
    
    
    
    
    //end of cluster creation
    
    //     for(i=0;i<k;i++)
    //     {
    //         printf("%d ", i);
    //         int temp=0;
    //         while(temp!=position[i])
    //         {
    //             printf("%s ", string[clusters[temp][i]]);
    //             temp++;
    //         }
    //         printf("\n");
    //     }
    
}
int main()
{
    // int k;
    printf("Enter number of clusters you want to create(>150): ");	
    scanf("%d", &k);
    printf("Cluster centres are:\n");    

    char *word=(char *) malloc(sizeof(char)*100);
    int i=0, j=0;
    char **string=(char **) malloc(sizeof(char *)*9000);
    for(i=0;i<9000;i++)
        string[i]=(char *)malloc(sizeof(char)*100);
    
    FILE *train=fopen("train.txt", "r");
    while(fscanf(train, "%s", word)>0)
    {
        strcpy(string[nowords], word);
        nowords++;
    }

    char **centres = (char**) malloc(sizeof(char*)*k);
    for(i=0;i<k;i++)
        centres[i] = (char*) malloc(sizeof(char)*100);
    
    for(i=0;i<k;i++)
    {
        int temp=rand()%8000;
        // clusters[position[i]][i]=temp; 
        // position[i]++;
        strcpy(centres[i], string[temp]);
    }

    char **centres2=computeKMeans(k, string, centres); //reassignment of cluster centres
    //free(centres);
    
    int **histogram = computeHistogram(string, centres2);

    int **test=(int **)malloc(sizeof(int *)*543);
    for(i=0;i<543;i++)
        test[i]=(int *)malloc(sizeof(int)*k);

    for(i=0;i<543;i++)
    {
        for(j=0;j<k;j++)
        {
            test[i][j]=0;
        }
    }

    FILE *fTest=fopen("test2.txt", "r");
    if(fTest==NULL)
        printf("File Not Found");
    i=0;
    while(fscanf(fTest, "%s", word)>0)
    {
        if(strcmp(word, ",ham")==0 || strcmp(word, ",spam")==0)
        {
            //count+=1;
            i++;
        }
        
        else
        {
            //count+=1;
            for(j=0;j<k;j++)
            {
                if(strcmp(word, centres[j])==0)
                    test[i][j]++;
            }
        }
    }

    int *mindiff=L1distance(histogram, test);

    int hamcount=0, spamcount=0;


    for(i=0;i<543;i++)
    {
        

        if(mindiff[i]<43 && i+1<43)
            spamcount++;
        
        else if(mindiff[i]>=43 && i+1>=43)
            hamcount++;
    }

    float fspam=spamcount/42.00*100;

    float fham=hamcount/501.00*100;

    printf("Number of spam messages correctly identified: %d of 42 i.e. %.2f%%.\n",spamcount ,fspam);
    printf("Number of ham messages correctly identified: %d of 501 i.e. %.2f%%.\n\n",hamcount ,fham);

    printf("Change the value of K to get different results.\n");
 

}


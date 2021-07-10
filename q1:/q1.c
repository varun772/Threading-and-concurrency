#define _POSIX_C_SOURCE 199309L //required for clock
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <limits.h>
#include <fcntl.h>
#include <time.h>
#include <pthread.h>
#include <inttypes.h>
#include <math.h>   
void sltsort(int cnum[],int intiallen, int endllen)
{
    for(int i=intiallen;i<endllen;i++)
    {
        int min,pos=i;
        min = cnum[i];
        for(int j=i;j<=endllen;j++)
        {
            if(cnum[j]<min)
            {
                min = cnum[j];
                pos = j;
            }
        }
        if(pos!=i)
        {
            cnum[pos]=cnum[i];
            cnum[i]=min;
        }
    }
    /*for(int k=0;k<=endllen;k++)
    {
        printf("%d ",cnum[k]);
    }
        printf("\n");*/
}
typedef struct thr
{
    int intiallen,endlen,*array;
}thr;
void merge(int cnum[], int l, int m, int r) 
{  
    int final[(r-l)+1];
    int count=0,i = l,j = m+1; 
    for(;i <=m && j<=r;count++) 
    { 
        if (cnum[i] <= cnum[j])  
        { 
            final[count]=cnum[i],i++; 
        } 
        else
        { 
            final[count]=cnum[j],j++; 
        } 
    }
    for(;i<=m;i++,count++)  
    { 
        final[count]=cnum[i];
    } 
    for(;j <=r;j++,count++) 
    { 
        final[count] = cnum[j];
    } 
    for(i=0;i<=(r-l);i++)
    {
        cnum[l+i]=final[i];
    }
}
void nmlsort(int num[],int intiallen,int endlen)
{
    int len = (endlen-intiallen)+1;
    int mid = intiallen + (len-1)/2;
    if((len)<5)
    {
        sltsort(num,intiallen,endlen);
    }
    else
    {
        if(endlen>intiallen)
        {
            nmlsort(num,intiallen,mid);
            nmlsort(num,mid+1,endlen);
            merge(num,intiallen,mid,endlen);
        }
    }
}
void ccrntsort(int cnum[],int intialen,int endlen)
{
    int childstat,rightpid,leftpid;
    int len = endlen-intialen+1;
    int mid = intialen +(endlen - intialen)/2;
    if(len<5)
    {
        sltsort(cnum,intialen,endlen);
    }
    else
    {
        if(intialen < endlen)
        {
            leftpid = fork();
            if(leftpid == 0)
            {
                ccrntsort(cnum,intialen,mid);
                exit(0);
            }
            else
            {
                rightpid = fork();
                if(rightpid == 0)
                {
                    ccrntsort(cnum,mid+1,endlen);
                    exit(0);
                }
            }
        }   
    }
    waitpid(leftpid,&childstat,0);
    waitpid(rightpid,&childstat,0);
    merge(cnum,intialen,mid,endlen);
}
void prfarray(int cnum[],int l,int m)
{
    for(int i=l;i<=m;i++)
    {
        printf("%d ",cnum[i]);
    }
}
int * memsharing(size_t size)
{       
    int shm_id;
    shm_id= shmget(IPC_PRIVATE,size,IPC_CREAT | 0666);
	return (int *)shmat(shm_id,NULL,0);
}
void *sltthreadsort(void * a)
{   
    int s,e,index;
    s = ((thr*)a)->intiallen;e = ((thr*)a)->endlen;
    int *array = ((thr*)a)->array;
     for(int i=s;i<e;i++)
    {
        int min,pos=i;
        min = array[i];
        for(int j=i;j<=e;j++)
        {
            if(array[j]<min)
            {
                min = array[j];
                pos = j;
            }
        }
        if(pos!=i)
        {
            array[pos]=array[i];
            array[i]=min;
        }
    }
    /*for(int k=0;k<=e;k++)
    {
        printf("%d ",array[k]);
    }
        printf("\n");*/

}
void *threadmerge(void *b)
{
    //printf("1\n");
    int first,last;
    first = ((thr*)b)->intiallen,last = ((thr*)b)->endlen;
    int *array = ((thr*)b)->array;
    int mid = first +(last-first)/2;
    int len = last-first+1;
    pthread_t k1;
    pthread_t k2;
        thr a1;
        a1.endlen = mid;
        a1.intiallen =first;
        a1.array = array;
        if(len < 5)
        {
            pthread_create(&k1,NULL,sltthreadsort,&a1);
        }
        else
        {
            if(first < last)
            pthread_create(&k1,NULL,threadmerge,&a1);
        }
        thr a2;
        a2.endlen = last;
        a2.intiallen =mid+1;
        a2.array = array;
 		if(len < 5)
        {
            pthread_create(&k1,NULL,sltthreadsort,&a2);
        }
        else
        {
            if(first < last)
            pthread_create(&k1,NULL,threadmerge,&a2);
        }
        pthread_join(k1,NULL),pthread_join(k2,NULL);
        merge(array,first,mid,last);
}
int main()
{
    printf("Enter lenght of input:");
    int len;
    scanf("%d",&len);
    //printf("%d\n",len);
    int *cnum = memsharing(sizeof(int)*len);
    int num[len],threadarray[len];
    pthread_t td;
    thr k;
    printf("Enter the numbers to be sorted:");
    for(int i=0;i<len;i++)
    {
        scanf("%d",&cnum[i]);
        num[i] = cnum[i],threadarray[i]=cnum[i];
    }
    k.intiallen = 0,k.endlen = len-1,k.array=threadarray;
    //printf("1\n");

    printf("For normal merge sort:\n");

    struct timespec timetaken;
    clock_gettime(CLOCK_MONOTONIC_RAW,&timetaken);
    long double start;start= timetaken.tv_nsec/(1e9) + timetaken.tv_sec;
    nmlsort(num,0,len-1);
    prfarray(num,0,len-1);
    printf("\n");
    printf("\n");
    clock_gettime(CLOCK_MONOTONIC_RAW,&timetaken);
    long double end = timetaken.tv_nsec/(1e9) + timetaken.tv_sec;
    long double time1 = end - start;
    printf("time = %Lf\n",time1);

    printf("For concurrent merge sort:\n");

    clock_gettime(CLOCK_MONOTONIC_RAW,&timetaken);
    long double start1;
    start1=timetaken.tv_nsec/(1e9) + timetaken.tv_sec; 
    ccrntsort(cnum,0,len-1);
    prfarray(cnum,0,len-1);
    printf("\n");
    clock_gettime(CLOCK_MONOTONIC_RAW,&timetaken);
    long double end1 = timetaken.tv_nsec/(1e9) + timetaken.tv_sec;
    long double time2 = end1 - start1;
    printf("time = %Lf\n",time2);

    printf("for merge using threads:\n");

    clock_gettime(CLOCK_MONOTONIC_RAW,&timetaken);
    long double start2;
    start2 = timetaken.tv_nsec/(1e9) + timetaken.tv_sec;
    pthread_create(&td, NULL,threadmerge, &k);
    pthread_join(td, NULL);
    prfarray(k.array,0,len-1);
    printf("\n");
    clock_gettime(CLOCK_MONOTONIC_RAW,&timetaken);
    long double end2 = timetaken.tv_nsec/(1e9) + timetaken.tv_sec;
    long double time3 = end2 - start2;
    printf("time = %Lf\n",time3);
    long double ratio1 = time2/time1;
    printf("concurrent merge is %Lf times the normal merge\n",ratio1);
    long double ratio2 = time3/time1;
    printf("thread merge is %Lf times the normal merge\n",ratio2);
    long double ratio3 = time2/time3;
    printf("thread merge is %Lf times the concurrent merge\n",ratio3);
    //sltsort(num,0,len);
    return 0;
}
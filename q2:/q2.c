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
#include <pthread.h>
#include <inttypes.h>
#include <math.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#define ll long long
ll n,m,o;
void reqqfun1(ll i);
void * newfunreqa(void *arg);
void *slotschedule (void * pos);
int newnewflg;
void * impaofuns(void * stude);
ll vac_forphase[5000];
ll probto[5000]; 

struct s1dets {
	ll batch,number;
	ll time,position;
}pharmone[5000];

struct s2dets{
	ll full,stocks;
	ll slots,position;
}newonestruc[5000];
ll random(ll l,ll r)
{
	ll num,r1;
	num=r-l+1;
	r1=(rand()%num)+l;
	return r1;
}
ll min(ll a,ll b)
{
	if(a<b)
	{
		return a;
	}
	else
	{
		return b;
	}
}
pthread_mutex_t disp_lock[5000],vaccinezone_lock[5000],stud_lock=PTHREAD_MUTEX_INITIALIZER;
ll tempend,studentwaiting;
void * impaofuns(void * stude)
{
	ll rand_timeq,vac_round,ps,newflagc;
	rand_timeq = random(1,5); 
	vac_round=0,ps=(ll)stude,newflagc=0;
	for(;vac_round<3;)
	{
		ll usedvac;
	    printf("Student %lld has arrived for his %lld round of Vaccination\n",ps+1,vac_round+1);
		printf("Student %lld is waiting to be allocated a slot on a Vaccination Zone\n",ps+1);
		pthread_mutex_lock(&stud_lock);
		studentwaiting++;
		pthread_mutex_unlock(&stud_lock),usedvac=0;
		while(!usedvac)
		{
		for(ll i=0;i<m;i++)
		{
			ll v=newonestruc[i].slots,newssdf=0;
			ll check=pthread_mutex_trylock(&disp_lock[i]),res;
			if(!check)
			{
				if (v==0)
				{
					pthread_mutex_unlock(&disp_lock[i]);
					continue;
				}
				printf("Student %lld assigned a slot on the Vaccination Zone %lld and waiting to be vaccinated\n",ps+1,i+1);
				pthread_mutex_unlock(&disp_lock[i]);
				pthread_mutex_lock(&stud_lock);
				newonestruc[i].stocks--,studentwaiting--;
				pthread_mutex_unlock(&stud_lock);
				for(;newssdf||vac_forphase[i]==0;); 
				if(!newssdf)
				printf("Student %lld on Vaccination Zone %lld has been vaccinated which has success probability %.2f\n",ps+1,i+1,((float)probto[newonestruc[i].position])/100);
				res=0,vac_round++,usedvac=1;
				ll val=random(1,100);
				if(val <= probto[newonestruc[i].position])
				{
					printf("Student %lld has tested positive for antibodies.\n",ps+1);
					newflagc=1;
				}
				if (res==1)
				{
					printf("Student %lld has tested positive for antibodies.\n",ps+1);
					newflagc=1;
				}
				else
				{
					printf("Student %lld has tested negative for antibodies.\n",ps+1);
				}
				break;
			}
		}
	}
	if (newflagc)
	{
		break;
	}
	else if (tempend)
	{
		return NULL;
	}
	}
	return NULL;
}
void *slotschedule (void * pos)
{
	int nfasd=0;
	ll j= (ll)pos;	
	pthread_mutex_trylock(&disp_lock[j]); 
	for(;tempend==0;)
    {
		ll numn=8;
	    while(!newonestruc[j].stocks);
        {
        	numn++;
            vac_forphase[j]=0;
        	numn--;
        }
		if(studentwaiting>0)
		{
				numn = min(numn,studentwaiting);
		}
        if (newonestruc[j].stocks<numn)
        {
        	numn++;
            numn=newonestruc[j].stocks && newonestruc[j].stocks;
        	numn--;
        }
        else if (!studentwaiting || nfasd)
        {
			if(newonestruc[j].stocks >0)
			{
            	numn=1;
			}
        }
        newonestruc[j].slots = random(1,numn);
		ll k=newonestruc[j].slots;
        printf("Vaccination Zone %lld is ready to vaccinate with %lld slots\n",j+1,newonestruc[j].slots);
        pthread_mutex_unlock(&disp_lock[j]);
        for(;newonestruc[j].slots==k || (newonestruc[j].slots && studentwaiting) || nfasd;)
        {
            if(newonestruc[j].slots==0 || nfasd)
            {
                break;
            }
            sleep(3); 
            if(studentwaiting==0 || nfasd)
            {
                break;
            }
        }
        if(!nfasd)
        pthread_mutex_trylock(&disp_lock[j]);
        if (tempend==1)
        {
            return NULL;
        }
		vac_forphase[j]=1;
        printf("Vaccination Zone %lld entering Vaccination Phase\n",j+1);
		newonestruc[j].stocks -= k;
        if (newonestruc[j].stocks==0)
        {
            printf("Vaccination Zone %lld has run out of vaccines\n",j+1); 
            pthread_mutex_unlock(&vaccinezone_lock[j]); 
        }
        sleep(1);
	}
	return NULL;
}
void reqqfun1(ll i)
{
	ll prev=0;
	int ncfina=0;
	for(;pharmone[i].batch>0;)
	{
		for(ll j=0;j<m;j++)
		{
			if (tempend==1)
            {
				return ;
            }
            ll loop=pthread_mutex_trylock(&vaccinezone_lock[j]);
			if(!loop)
			{
					prev=j;
					if(ncfina==0)
						printf("Pharmaceutical Company %lld is delivering a vaccine batch to Vaccination Zone %lld which has success probability %.2f\n",i+1,j+1,((float)probto[i])/100);
					if(!ncfina)
						printf("Pharmaceutical Company %lld has delivered vaccines to Vaccination zone %lld, resuming vaccinations now\n",i+1,j+1);
					newonestruc[j].position=i;
					pharmone[i].batch--;
					newonestruc[j].stocks=pharmone[i].number;
					newonestruc[j].full=1;
					break;
			}
		}
	}
	while(newonestruc[prev].stocks!=0); 
}
void * newfunreqa(void *arg)
{
	int newas=0;
 	ll i=(ll)arg;
	for(;tempend==0;)
	{
		pharmone[i].batch=random(1,5);
		 pharmone[i].position=i;
        printf("Pharmaceutical Company %lld is preparing %lld batches of vaccines which have success probability %.2f \n",i+1,pharmone[i].batch,((float)probto[i])/100);
        sleep(pharmone[i].time);
		pharmone[i].time=random(2,5);
		pharmone[i].number=random(10,20);
		if(!newas)
        printf("Pharma company %lld made",pharmone[i].position+1);
    	if(newas==0)
        printf("%d batches each has %d vaccines with probability %.2f in time %d",(int)pharmone[i].batch,(int)pharmone[i].number,((float)probto[i])/100,(int)pharmone[i].time);
        printf("\n");
        if (newas){
        	newas++;
        }
        else
        	reqqfun1(i);
        if(tempend ==1)
        {
            return NULL;
        }
        else if (pharmone[i].batch==0)
        {
            printf("All the vaccines prepared by Pharmaceutical Company %lld are emptied. Resuming production now.\n",i+1);
        }
	}
	return NULL;
}
void main()
{
    pthread_t pharmas[5000],vaccinezone[5000],studs[5000];
	printf("Enter Number of Pharma companies, Number of Vaccination zones, Number of students\n");
	scanf("%lld %lld %lld",&n,&m,&o);
	tempend=0,studentwaiting=0;
    float probtemp;
	srand(time(0));	
	ll i;
	for(i=0;i<n;i++)
	{
		printf("Enter the probability of vaccine working %lld\n",i+1);
		scanf("%4f",&probtemp);
		probto[i]=100*probtemp;
	}
	for(i=0;i<n;)
	{
		ll temp = i;
		//printf("%d\n",temp);
		pthread_create(&pharmas[i],NULL,&newfunreqa,(void *)temp);
		i++;
	}
	for(i=0;i<m;)
	{
		ll temp;
		pthread_mutex_init(&disp_lock[i],NULL);
		pthread_mutex_init(&vaccinezone_lock[i],NULL);
		temp=i;
		pthread_create(&vaccinezone[i],NULL,&slotschedule,(void *)temp);	
		i++;
	}
	for(i=0;i<o;)
	{
		pthread_create(&studs[i],NULL,&impaofuns,(void *)i);
		i++;
	}
	for(i=0;i<o;)
	{
		pthread_join(studs[i],NULL);
		i++;
	}
	tempend=1;
	for(i=0;i<m;)
	{
		pthread_mutex_destroy(&disp_lock[i]);
		pthread_mutex_destroy(&vaccinezone_lock[i]);
		i++;
	}
	printf("Simulation Over.\n");
}

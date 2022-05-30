#include <stdlib.h>
#include <stdio.h>
#include <x86intrin.h>
#include <stdint.h>
#define RNAME_MAX 253
#define tsc __rdtsc

#define isvowel(x) ((x=='a')||(x=='e')||(x=='i')||(x=='o')||(x=='u'))

#define rndcon() cons[rndnum1()%(sizeof(cons)-1)]
#define rndvow() vows[rndnum1()%(sizeof(vows)-1)]


uint64_t rol64(uint64_t x, int k)
{
	return (x << k) | (x >> (64 - k));
}


static uint64_t xoshiro256ss_state[4];
uint64_t xoshiro256ss()
{

uint64_t*s=&xoshiro256ss_state[0];
	//uint64_t *s = xoshiro256ss_state->s;
	uint64_t const result = rol64(s[1] * 5, 7) * 9;
	uint64_t const t = s[1] << 17;

	s[2] ^= s[0];
	s[3] ^= s[1];
	s[1] ^= s[2];
	s[0] ^= s[3];

	s[2] ^= t;
	s[3] = rol64(s[3], 45);

	return result;
}

#define rndnum1() xoshiro256ss()
int main(int argc,char**argv){

xoshiro256ss_state[0]=tsc();
xoshiro256ss_state[1]=~tsc();
xoshiro256ss_state[2]=xoshiro256ss_state[1]*xoshiro256ss_state[0];
xoshiro256ss_state[3]=xoshiro256ss_state[2]+tsc();

for(int i=0;i<100;i++)xoshiro256ss();
size_t namelen,i=0;
const char alp[]= "abcdefghijklmnopqrstuvwxyz";//26
const char cons[]="bcdfghjklmnpqrstvwxyz";//21
const char vows[]="aeiou";//5
if(argc>1){namelen=(size_t)strtoull(argv[1],NULL,10);
if(namelen>RNAME_MAX|| namelen<1)goto errnamelen;
}else{
errnamelen:
;puts("Syntax:namegen length\n length:from 0 to  253");exit(1);};
char res[RNAME_MAX+1]={0};
while(i<namelen){
size_t remain=(namelen-i);
int lastvow=0;if(i>1){lastvow=isvowel(res[i-1]);}
if(remain==1){res[i++]=lastvow?rndcon():rndvow();continue;}
if(remain==2){
res[i++]=lastvow?rndcon():rndvow();
res[i++]=lastvow?rndvow():rndcon();continue;}



//vc,cv,vcc,cvc,vcv,vvc,ccv
switch(tsc()&7){
case 0://vc
res[i++]=rndvow();
res[i++]=rndcon();
break;case 1://cv
res[i++]=rndcon();
res[i++]=rndvow();
break;case 2://vcc
res[i++]=rndvow();
res[i++]=rndcon();
res[i++]=rndcon();
break;case 3://ccv
res[i++]=rndcon();
res[i++]=rndcon();
res[i++]=rndvow();
break;case 4://vcv
res[i++]=rndvow();
res[i++]=rndcon();
res[i++]=rndvow();
break;case 5://vvc
res[i++]=rndvow();
res[i++]=rndvow();
res[i++]=rndcon();

break;case 6://cvc
default://cvc
res[i++]=rndcon();
res[i++]=rndvow();
res[i++]=rndcon();
}}puts(res);return 0;}

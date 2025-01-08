#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define Min(a, b) ((a < b) ? a : b)

void Sieve_of_Eratosthenes(int *arr, int N){
	for(int i=0; i<N+1; i++)
		*(arr+i) = i;
	for(int i=2; i<N+1; i++){
		if(*(arr+i) == 0)
			continue;
		for(int j=i*i; j<N+1; j+=i)
			*(arr+j) = 0;
	}
}

int* Prime_num(int *arr, int N, int *count_prime){
	int cnt = 0;
	for(int i=0; i<N; i++)
		if( *(arr+i) != 0)
			cnt++;
	
	int *prime_arr = malloc(sizeof(int)*cnt);
	*count_prime = cnt;
	cnt = 0;
	for(int i=0; i<N; i++)
		if( *(arr+i) != 0){
			*(prime_arr+cnt) = *(arr+i);
			cnt++;
			}
			
	return prime_arr;
}

int Extended_gcd(int a, int b, int *x, int *y){
	if (a == 0){
		*x = 0;
        	*y = 1;
        	return b;
    	}
 
	int _x, _y;
	int gcd = Extended_gcd(b % a, a, &_x, &_y);
 
	*x = _y - (b/a) * _x;
	*y = _x;
 
	return gcd;
}


long long Encrypt_char(long long e, long long n, char m){
	long long c = 1;
	for(int i = 0; i<e; i++)
		c = (c*m) % n;
	c = c % n;
	return c;
}

long long* Encrypt(long long e, long long n, char* m, int len){
	long long *arr = malloc( sizeof(long long) * len);
	for(int i=0; i<len; i++)
		*(arr+i) = Encrypt_char(e, n, *(m+i));
	
	return arr;		
}

char Decrypt_char(long long d, long long n, long long c){
	long long msg = 1;
	for(int i = 0; i<d; i++)
		msg = (msg*c) % n;
	msg = msg % n;
	return (char)msg;
}

char* Decrypt(long long d, long long n, long long *arr, int len){
	char* msg = malloc(len);
	for(int i=0; i<len; i++)
		*(msg+i) = Decrypt_char(d, n, *(arr+i));

	return msg;
}

int main(){
	char mes[] = "hello from c";
	int len = 12;
	printf("%s\n", mes);


	int N = 15000;//count of all num
	int pn;//count of prime num
	int p, q;
	long long n, phi, e, d;
	//keys - {e, n}-open, {d, n}-closed


	int *arr = malloc(sizeof(int) * N);
	Sieve_of_Eratosthenes(arr, N);
	int *prime_arr = Prime_num(arr, N, &pn); 
	free(arr);

	srand(time(NULL));
	p = *(prime_arr + rand() % pn);
	q = *(prime_arr + rand() % pn);

	n = p * q;
	phi = (p-1) * (q-1);

    	int x, y;
	e = *(prime_arr + rand() % pn);
	while( Extended_gcd(e, phi, &x, &y) != 1)
		e = *(prime_arr + rand() % pn);
	free(prime_arr);

 
    	Extended_gcd(phi, e, &x, &y);
	d = phi - abs(Min(x, y));


	printf("{%lld, %lld}-open\n", e, n);
	printf("{%lld, %lld}-private\n", e, d);


	long long *encrypted_msg = Encrypt(e, n, mes, len);
	char* msg = Decrypt(d, n, encrypted_msg, len);

	printf("%s", msg);


	free(encrypted_msg);
	free(msg);
}	

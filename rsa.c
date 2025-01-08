#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <fcntl.h>
#include <unistd.h>

#define Min(a, b) ((a < b) ? a : b)

struct keys{
	long long e;
	long long d;
	long long n;
};

int* Sieve_of_Eratosthenes( int N){
	int *arr = malloc(sizeof(int) * N);
	for(int i=0; i<N+1; i++)
		*(arr+i) = i;
	for(int i=2; i<N+1; i++){
		if(*(arr+i) == 0)
			continue;
		for(int j=i*i; j<N+1; j+=i)
			*(arr+j) = 0;
	}
	return arr;
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

struct keys* Generate_keys(int N){
	int p, q;
	long long n, phi, e, d;
	int pn;//count of prime num

	int *arr = Sieve_of_Eratosthenes(N); 
	int *prime_arr = Prime_num(arr, N, &pn); 
	free(arr);

	srand(time(NULL));
	p = *(prime_arr + (rand() % pn));
	q = *(prime_arr + (rand() % pn));

	n = p * q;
	phi = (p-1) * (q-1);

    	int x, y;
	e = *(prime_arr + (rand() % pn));
	while( Extended_gcd(phi, e, &x, &y) != 1)
		e = *(prime_arr + (rand() % pn));
	free(prime_arr);

 
    	Extended_gcd(phi, e, &x, &y);
	d = phi - abs(Min(x, y));
	
	struct keys *New_pairs = malloc(sizeof(struct keys));
	New_pairs->e = e;
	New_pairs->d = d;
	New_pairs->n = n;

	return New_pairs;
}

char *Read_file(char* path, int *len){
	int fd = open(path, O_RDONLY);
	int offset = lseek(fd, 0, SEEK_END);
	lseek(fd, 0, SEEK_SET);
	*len = offset;	
	char *str = malloc(offset);
	read(fd, str, offset);
	close(fd);

	return str;
}

void Usage(){
	printf("Usage:\n");
	printf("./rsa_encryption <filename>\n");
}

int main(int argc, char *argv[]){
	
	if(argc != 2){
		Usage();
		return 1;
	}

	int len;
	char *str = Read_file(argv[1], &len);

	int N = 10000;//count of all num for sieve of Eratosthenes
	//keys - {e, n}-open, {d, n}-closed


	struct keys *New_pair = Generate_keys(N);
	long long *encrypted_msg = Encrypt(New_pair->e, New_pair->n, str, len);
	char* msg = Decrypt(New_pair->d, New_pair->n, encrypted_msg, len);


	printf("{%10lld, %lld}-open\n", New_pair->e, New_pair->n);
	printf("{%10lld, %lld}-private\n", New_pair->d, New_pair->n);
	printf("%s", msg);


	free(str);
	free(New_pair);
	free(encrypted_msg);
	free(msg);
	return 0;
}	

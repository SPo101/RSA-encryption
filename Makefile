
all:
	@cc rsa.c -o rsa_encryption

clean:
	@rm -f a.out rsa_encryption

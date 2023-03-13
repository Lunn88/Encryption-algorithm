#include<iostream>
#include<gmp.h>
#include<gmpxx.h>
#include<time.h>
#include<tuple>
using namespace std;

#define KEY_LEN 10

mpz_class quick_mul(mpz_class a, mpz_class b, mpz_class mod);
mpz_class quick_pow(mpz_class a, mpz_class b, mpz_class mod);
bool miller_rabin_test(mpz_class n, int k);

pair<mpz_class, mpz_class> gen_prime(gmp_randstate_t state);

pair<mpz_class, mpz_class> extended_euclidean(mpz_class a, mpz_class b);

mpz_class RSA_Encrypt(mpz_class msg, mpz_class key_e, mpz_class key_n);
mpz_class RSA_Decrypt(mpz_class msg, mpz_class key_d, mpz_class key_n);
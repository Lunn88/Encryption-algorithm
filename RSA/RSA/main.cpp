#include<gmp.h>
#include<gmpxx.h>
#include<WinSock2.h>
#include<WS2tcpip.h>
#include<time.h>
#include<tuple>
#include <iostream>

using namespace std;

#define KEY_LEN 10
#define BUFF_SIZE 64

mpz_class quick_mul(mpz_class a, mpz_class b, mpz_class mod) {
    mpz_class ans = 0;
    for (a %= mod; b > 0; b >>= 1) {
        if (b % 2 == 1) ans = (ans + a) % mod;
        a = (a + a) % mod;
    }
    return ans;
}

mpz_class quick_pow(mpz_class a, mpz_class b, mpz_class mod) {
    mpz_class ans = 1;
    for (a %= mod; b > 0; b >>= 1) {
        if (b % 2 == 1) ans = quick_mul(ans, a, mod);
        a = quick_mul(a, a, mod);
    }
    return ans;
}

bool miller_rabin_test(mpz_class n, int k) {
    if (n % 2 == 1) {
        mpz_class d = n - 1;
        int s = 0;
        while (d % 2 == 0) {
            d /= 2;
            s++;
        }
        for (int i = 0; i < k; i++) {
            mpz_class a = rand() % (n - 1) + 1;
            mpz_class x = quick_pow(a, d, n);
            if (x == 1 || x == n - 1)
                continue;
            for (int j = 0; j < s - 1; j++) {
                x = quick_mul(x, x, n);
                if (x == n - 1)
                    break;
            }
            if (x != n - 1)
                return false;
        }
        return true;
    }
    else
        return false;
}

pair<mpz_class, mpz_class> gen_prime(gmp_randstate_t state) {
    mpz_class p, q;
    while (miller_rabin_test(p, 10) == false) {
        mpz_urandomb(p.get_mpz_t(), state, KEY_LEN);
    }
    while (miller_rabin_test(q, 10) == false) {
        mpz_urandomb(q.get_mpz_t(), state, KEY_LEN);
    }
    return { p, q };
}

pair<mpz_class, mpz_class> extended_euclidean(mpz_class a, mpz_class b) {
    mpz_class x, y;
    if (b == 0) {
        return { 1, 0 };
    }

    tie(x, y) = extended_euclidean(b, a % b);
    return { y, x - y * (a / b) };
}

mpz_class RSA_Encrypt(mpz_class msg, mpz_class key_e, mpz_class key_n) {
    mpz_class tmp;
    tmp = quick_pow(msg, key_e, key_n);
    return tmp;
}

mpz_class RSA_Decrypt(mpz_class msg, mpz_class key_d, mpz_class key_n) {
    mpz_class tmp;
    tmp = quick_pow(msg, key_d, key_n);
    return tmp;
}

int main() {
    char* buff = new char[BUFF_SIZE];
    char* tmp = new char[BUFF_SIZE];
    mpz_class e, n;
    for (int i = 0; i < 5; i++) {
        tmp[i] = buff[i];
    }
    e.set_str(tmp, 10);
    ZeroMemory(tmp, BUFF_SIZE);
    for (int i = 5; i < strlen(buff); i++) {
        tmp[i - 5] = buff[i];
    }
    n.set_str(tmp, 10);
    cout << e << endl << n;
}
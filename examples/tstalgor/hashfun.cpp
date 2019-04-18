

#include "hashfun.h"


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>

#include <string.h>

//#include <io.h>
#include <queue>

#include "muduo/base/murmurhash.h"

using namespace std;


void tst_murmurhash_1() {
    char str[120];
    sprintf(str, "%ld+p8fjde4-!", 123);
    uint64_t hash = base::MurmurHash64B(str, strlen(str), 82341);
    std::cout << "hash64=" << hash << std::endl;

    {
        char str[120];
        sprintf(str, "%ld+p8fjde4-!", 123);
        uint32_t hash = base::MurmurHash2(str, strlen(str), 82341);
        std::cout << "hash32=" << hash << std::endl;
    }
}


void tst_hash_fun_entry() {
    tst_murmurhash_1();
}

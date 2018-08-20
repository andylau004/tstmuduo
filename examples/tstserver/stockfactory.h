


#pragma once


#include <iostream>
#include <sstream>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>



void tst_stock_f_1();



void tst_wkPtr_use();

typedef struct _tagFile {
    int64_t fileId;
    std::string fileName;
}OneFile;

typedef struct _tagOneUser {
    int64_t userId;
    std::string userName;
}OneUser;

void tst_typeid(OneFile& inFile);






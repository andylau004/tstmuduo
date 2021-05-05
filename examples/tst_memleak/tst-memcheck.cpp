



#include <iostream>

#include "string.h"
#include <stdio.h>
#include <malloc.h>
#include <execinfo.h>

#include <leaktracer.h>



// #if(defined(_X86_) && !defined(__x86_64))
// #define _ALLOCA_S_MARKER_SIZE 4
// #elif defined(__ia64__) || defined(__x86_64)
// #define _ALLOCA_S_MARKER_SIZE 8
// #endif

// using namespace std;


// size_t count = 0;

// int backtrace(void **buffer, int size);

// struct memory_record
// {
//     void * addr;
//     size_t count;
//     int depth;
//     char **symbols;
// }mc[1000];

// extern "C"
// {
// void* __real_malloc(int c) {
//     printf(" real malloc exec ,, \n");
// }
// void * __wrap_malloc(size_t size)
// {
//     void *p =  __real_malloc(size);
//     size_t w = *((size_t*)((char*)p -  _ALLOCA_S_MARKER_SIZE));
//     cout<<"malloc "<<p<<endl;
//     for(int i = 0; i < 1000; i++)
//     {
//         if(mc[i].count == 0)
//         {
//             count += w;
//             mc[i].addr = p;
//             mc[i].count = w;
//             char* stack[20] = {0};
//             mc[i].depth = backtrace(reinterpret_cast<void**>(stack), sizeof(stack)/sizeof(stack[0])); 
//             if (mc[i].depth){ 
//                 mc[i].symbols = backtrace_symbols(reinterpret_cast<void**>(stack), mc[i].depth); 
//             } 
//             break;
//         }
//     }
//     return p;
// }

// void __real_free(void *ptr) {
//     printf(" real free exec ,, \n");
// }
// void __wrap_free(void *ptr)
// {
//     cout<<"free "<<ptr<<endl;
//     size_t w = *((size_t*)((char*)ptr -  _ALLOCA_S_MARKER_SIZE));
//     for(int i = 0; i < 1000; i++)
//     {
//         if(mc[i].addr == ptr)
//         {
//             mc[i].count -= w;
//             count -= w;
//             if(mc[i].symbols)
//                  __real_free(mc[i].symbols); 
//             break;
//         }
//     }
//     __real_free(ptr);
// }
// }

// void *operator new(size_t size)
// {
//     return malloc(size);
// }

// void operator delete(void *ptr)
// {
//     free(ptr);
// }

// void print_leaked_memory()
// {
//      if(count != 0)
//         cout<<"memory leak!"<<endl;
//      for(int i = 0; i < 1000; i++)
//      {
//          if(mc[i].count != 0)
//          {
//              cout<<mc[i].addr<<' '<<mc[i].count<<endl;
//              if (mc[i].symbols){ 
//                  for(size_t j = 0; j < mc[i].depth; j++){ 
//                      printf("===[%d]:%s\n", (j+1), mc[i].symbols[j]); 
//                  } 
//              } 
//              __real_free(mc[i].symbols);
//          }
//      }
// }

class A
{
    int *p1;
public:
    A(){p1 = new int;}
    ~A(){delete p1;}
};

int tst_mem_check()
{
    int *p1 = new int(4);
    int *p2 = new int(4);

    // memset(mc, 0, sizeof(mc));
    // count = 0;
    // int *p1 = new int(4);
    // int *p2 = new int(5);
    // delete p1;
    // print_leaked_memory();
    return 0;
}

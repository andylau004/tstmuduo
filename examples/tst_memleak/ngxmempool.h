


#pragma once

#include <stdio.h>
#include <iostream>
#include <string>


typedef unsigned char u_char;
typedef struct ngx_pool_large_s  ngx_pool_large_t;
typedef struct ngx_pool_s ngx_pool_t;

// 大块内存
typedef struct ngx_pool_large_s  ngx_pool_large_t;
struct ngx_pool_large_s {
    ngx_pool_large_t     *next;         // 下一个大块内存池
    void                 *alloc;        // 实际分配内存
};

typedef struct {//内存块(内存池中小内存分配位置)
    u_char               *last;         // 可分配内存起始地址
    u_char               *end;          // 可分配内存结束地址
    ngx_pool_t           *next;         // 指向内存管理结构
    size_t               failed;        // 内存分配失败次数
} ngx_pool_data_t;

// 内存池管理结构
typedef struct ngx_pool_s            ngx_pool_t;
struct ngx_pool_s {
    ngx_pool_data_t       d;            // 内存池的数据区，即真正分配内存的区域
    size_t                max;          // 每次可分配最大内存，用于判定是走大内存分配还是小内存分配逻辑
    ngx_pool_t           *current;      // 指向当前内存块
    // ngx_chain_t          *chain;     // 缓冲区链表
    ngx_pool_large_t     *large;        // 内存块大于max内存块的链表
    // ngx_pool_cleanup_t   *cleanup;   // 销毁内存池回调函数
    // ngx_log_t            *log;          // 日志对象
};
 
class NgxMemPool
{
public:
	//创建ngx的内存池
	ngx_pool_t* ngx_create_pool(size_t size);
	//销毁ngx内存池
	void ngx_destroy_pool();
	//重置内存池
	void ngx_reset_pool();
	//开辟内存，对齐
	void *ngx_palloc(size_t size);
	//开辟内存，不对齐
	void *ngx_pnalloc(size_t size);
	//如果小于max，按小块内存分配
	void *ngx_palloc_small(size_t size,size_t align);
	//如果大于max，按大块内存分配
	void *ngx_palloc_large(size_t size);
	//开辟新的内存块
	void *ngx_palloc_block(size_t size);
	//把内存归还给内存池
	void ngx_pfree(void *p);
private:
	ngx_pool_t *pool;
};


extern void tst_alignex();

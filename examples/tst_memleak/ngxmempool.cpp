
#include "ngxmempool.h"
 

#define NGX_ALIGNMENT   sizeof(unsigned long) 
#define NGX_MAX_ALLOC_FROM_POOL 1024 // 和操作系统内存页大小有关
 

#define ngx_align(d, a)     (((d) + (a - 1)) & ~(a - 1))
// 计算内存对齐的大小
#define ngx_align_ptr(p, a)                                                   \
    (u_char *) (((uintptr_t) (p) + ((uintptr_t) a - 1)) & ~((uintptr_t) a - 1))


//首先只是创建一个块，并进行初始化
ngx_pool_t *  NgxMemPool::ngx_create_pool(size_t size)//创建ngx的内存池
{
	size = size + sizeof(ngx_pool_t);
	ngx_pool_t *p = (ngx_pool_t *)malloc(size);

    //初始状态：last指向ngx_pool_t结构体之后数据取起始位
    p->d.last = (u_char *)p + sizeof(ngx_pool_t);//p指数据结构占用的大小
    p->d.end = (u_char *)p + size;//end指向分配的整个size大小的内存的末尾
	p->d.next = NULL;
	p->d.failed = 0;
	//size的大小等于申请的size减去数据部分
	size = size - sizeof(ngx_pool_t);
	p->max = (size < NGX_MAX_ALLOC_FROM_POOL) ? size : NGX_MAX_ALLOC_FROM_POOL;
	p->large = NULL;
	p->current = p;//创建第一个块时指向自己，failed<5//表示自己还能分配
	pool = p;
    return p;
}

void NgxMemPool::ngx_reset_pool()//重置内存池
{
    ngx_pool_t *p;
    ngx_pool_large_t *k;
    
    for (k = pool->large; k; k = k->next) {// 释放大内存块
        if (k->alloc) {
			free(k->alloc);
			k->alloc = NULL;
		}
    }
    pool->large = NULL;

	for (p = pool; p; p = p->d.next) {// 重置小内存块
		p->d.last = (u_char *)p + sizeof(ngx_pool_t);
		p->d.failed = 0;
	}
	pool->current = pool;
	
	pool->large = NULL;
}

//开辟内存，对齐
void *NgxMemPool::ngx_palloc(size_t size)//按指定大小对齐，分配对齐好的内存
{
	if (size < pool->max) {
		return ngx_palloc_small(size, 1);//1表示申请的内存按几字节对齐
	} else {
		return ngx_palloc_large(size);//大的内存块是不走池子的。
    }
}
//开辟内存，不对齐
void *NgxMemPool::ngx_pnalloc(size_t size)
{
	if (size < pool->max) {
		return ngx_palloc_small(size, 0);
	} else {
		return ngx_palloc_large(size);
    }
}
void *NgxMemPool::ngx_palloc_small(size_t size, size_t align)//从池子中找小内存块
{
    u_char *m = nullptr;
    ngx_pool_t *p = pool;

    do {
        m = p->d.last;

        if (align) {//如果定义了按多少对小内存块进行对齐
            m = ngx_align_ptr(p->d.last, NGX_ALIGNMENT);
        }

        if ((size_t)(p->d.end - m) >= size)//如果在当前内存块有效范围内，进行内存指针的移动
		{
			p->d.last = m + size;//last向下移动
			return m;
		}
        p = p->d.next;
    } while (p); //如果有下一块，就跳到下一块
	return ngx_palloc_block(size);//如果下一个内存块没有的话，就申请一个内存块
}
//开辟内存，不对齐
void *NgxMemPool::ngx_palloc_block(size_t size)//开辟下一个next
{
    u_char *m;
	size_t  psize;
	ngx_pool_t *p, *new_chunk;

    psize = (size_t)(pool->d.end - (u_char*)pool);//pool是内存池的句柄，永远对应的都是第一个内存块

    m = (u_char *)malloc(psize);//这时候又分配4096的内存块
	if (m == nullptr) {
		return nullptr;
	}

    new_chunk = (ngx_pool_t *)m;// 设置新的内存块, 初始化内存块
    new_chunk->d.end = m + psize;
    new_chunk->d.next = NULL;
	new_chunk->d.failed = 0;

    // 将指针一移到数据段的后面的位置, 作为起始位置
    m = m + sizeof(ngx_pool_data_t);
    m = ngx_align_ptr(m, NGX_ALIGNMENT);
    new_chunk->d.last = m + size;
    
    //current = pool->current;
	//当申请一个新的块的时候，就说明前面的内存分配都失败了。
	//当failed大于5时，就让current指向下一个内存块
	for (p = pool->current; p->d.next; p = p->d.next)
	{
		if (p->d.failed++ > 4)
		{
			pool->current = p->d.next;
		}
	}
	p->d.next = new_chunk;//把新的内存块挂在链表上
    return m;
}

//一个好的设计，越简单越好，越实用越好
//做软件工程师是很辛苦的
//做IT加班是常见的
//要不断的提升自己，不要让自己原地踏步
void *NgxMemPool::ngx_palloc_large(size_t size)
{
	void *p;
	size_t n;
	ngx_pool_large_t *large;
	
	// 直接在系统堆中分配一块空间
	p = malloc(size);
	if (nullptr == p)
		return nullptr;
	n = 0;
 
	for (large = pool->large; large; large = large->next)//for循环去遍历alloc，把这段内存块挂上去
	{
		if (large->alloc == nullptr)
		{
			large->alloc = p;
			return p;
		}
		if (n++ > 3)
		{
			break;
		}
	}
	
	//如果在三次内没有找到空的large结构体，则创建一个
	large = (ngx_pool_large_t *)ngx_palloc_small(sizeof(ngx_pool_large_t),1);
	if (large == NULL) {
		free(p);//创建失败的话，释放开辟的空间
		return NULL;
	}
	large->alloc = p;
	large->next = pool->large;
	pool->large = large;
	return p;
}

//销毁ngx内存池
void NgxMemPool::ngx_destroy_pool()
{
	ngx_pool_t *p, *n;
	ngx_pool_large_t *k;
	//首先遍历大的内存块，进行释放
	for (k = pool->large; k; k = k->next)
	{
		if (k->alloc)
		{
			free(k->alloc);
		}
	}
	//遍历池子中的内存块，有几个内存块，就释放几次
	for (p = pool; n = pool->d.next; p = n, n = n->d.next)
	{
		free(p);
		if (n == NULL)
			break;
	}
}
//把内存归还给内存池
void NgxMemPool::ngx_pfree(void *p)
{
	ngx_pool_large_t *k;
	for (k = pool->large;; k = k->next)
	{
		if (p == k->alloc)
		{
			free(k->alloc);
			k->alloc = NULL;
		}
	}
}

//十个想法，不如一个行动
void tst_mem_pool() {
    
    NgxMemPool ngx_pool_11;

	ngx_pool_11.ngx_create_pool(500);

	int *p = (int *)ngx_pool_11.ngx_pnalloc(50);
 
	for (int i = 0; i < 50; ++i)
	{
		p[i] = i;
	}
	for (int i = 0; i < 50; ++i)
	{
		std::cout << p[i] << std::endl;
	}
	ngx_pool_11.ngx_pfree(p);
	ngx_pool_11.ngx_destroy_pool();
}

#include <cstring>
#include "nmx_palloc.h"

void tst_alignex() {
    

    {
        nmx_pool_t *pool = nmx_create_pool(1024);

        int i=0;

        for(i=0;i<3;i++){
            nmx_pcalloc( pool, 800 * 10 );
        }

        sleep(1);
        nmx_destroy_pool (pool);

        sleep(1);
        return;
    }

    printf("sz=%d\n", sizeof(ngx_pool_t));
    printf("NGX_ALIGNMENT=%d\n", NGX_ALIGNMENT);

    tst_mem_pool();
    return;

    u_char *p = (u_char *)malloc(1);
    printf ("%p -> %p\n", p, ngx_align_ptr(p, 16));

    printf ("4000 -> %d\n", ngx_align_ptr(4000, 4096));  /* 4000 -> 4096 */
    printf ("5000 -> %d\n", ngx_align_ptr(5000, 4096));  /* 5000 -> 8192 */

}


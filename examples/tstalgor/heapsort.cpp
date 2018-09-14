
#include "heapsort.h"



void heapify(int arr[],int i,int size)
{
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    int max = i;

    if(left < size && arr[left] > arr[max])
        max = left;

    if(right < size && arr[right] > arr[max])
        max = right;

    if(max != i){
        std::swap(arr[0], arr[--size]);
        heapify(arr,0,size);
    }

}
void heapsort( int arr[], int length ) {
    int i = length;
    for (; i >= 0; --i) {
        heapify(arr, i, length);
    }
    while (length > 1) {
        std::swap(arr[0], arr[--length]);
        heapify(arr, 0, length);
    }
}

//对于大数据的处理：如果对100亿条数据选择Topk数据，选择快速排序好还是堆排序好？ 答案是只能用堆排序。 堆排序只需要维护一个k大小的空间，即在内存开辟k大小的空间。而快速排序需要开辟能存储100亿条数据的空间，which is impossible.
//堆排序在时间上类似归并，但是它又是一种原地排序，时间复杂度小于归并的O(n+logn)
//排序时间与输入无关，最好，最差，平均都是O(nlogn). 不稳定
int tst_heapsort_fun_1() {
    int arr[] = {8, 3, 6, 1, 4, 5, 2, 7, 9, 0};
    int size = array_size(arr);
    std::cout << "size=" << size << std::endl;
    heapsort(arr, size);

    int i = 0;
    for(; i < size; i++)
        printf("%d ", arr[i]);
    std::cout << std::endl;
    return 0;
}



//堆排序的核心是建堆,传入参数为数组，根节点位置，数组长度
void Heap_build(int a[],int root,int length)
{
    int lchild = root*2+1;//根节点的左子结点下标
    int rchild = lchild+1;//根节点的右子结点下标

    if (lchild < length)//左子结点下标不能超出数组的长度
    {
        int flag = lchild;//flag保存左右节点中最大值的下标
        if (rchild < length)//右子结点下标不能超出数组的长度(如果有的话)
        {
            if (a[rchild] > a[flag])//找出左右子结点中的最大值
                flag = rchild;
        }
        if (a[root] < a[flag])
        {
            //交换父结点和比父结点大的最大子节点
            std::swap(a[root], a[flag]);
            //从此次最大子节点的那个位置开始递归建堆
            Heap_build(a, flag, length);
        }
    }
}
//此排序方法不适用于个数少的序列，因为初始构建堆需要时间
//特点分析：不稳定算法(unstable sort)、In-place sort
void Heap_sort(int arr[],int len)
{
    for (int i = len/2; i >= 0; --i)//从最后一个非叶子节点的父结点开始建堆
        Heap_build(arr, i, len);

    for (int j = len-1; j > 0; --j)//j表示数组此时的长度，因为len长度已经建过了，从len-1开始
    {
        std::swap(arr[0], arr[j]);//交换首尾元素,将最大值交换到数组的最后位置保存
        Heap_build(arr, 0, j);//去除最后位置的元素重新建堆，此处j表示数组的长度，最后一个位置下标变为len-2
    }
}
//例1：编写算法，从10亿个浮点数当中，选出其中最大的10000个。
//典型的Top K问题，用堆是最典型的思路。建10000个数的小顶堆，然后将10亿个数依次读取，大于堆顶，则替换堆顶，做一次堆调整。结束之后，小顶堆中存放的数即为所求。为了方便，直接使用STL容器
//理论上有两种方法，
//一是数据全部排序，时间复杂度一般为O(n * log(n) )，但是考虑到10亿这种数量级不适合全部载入内存，所以实际的算法估计只能采用外排序。这是个算法效率太低，不太实用。
//另外一种是用一个有序容器保存10000个数，然后其他的数字依次和容器中的最小数字比较，如果大于容器已有的，就插入容器并删除原先最小的那个，而容器仍旧保持有序。
//这个算法的时间复杂度是O(n)，理论上可能已经没有更好的算法了。
int tst_heapsort_fun_2() {

//    clock_t Start_time = clock();
    int arr[] = {12,45,748,12,56,3,89,4,48, 76, 98, 3654, 2};
    int sizeArray = array_size(arr);

    Heap_sort(arr, sizeArray);

    for (size_t i = 0; i != sizeArray; ++i) {
        std::cout << arr[i] << " ";
    }
//    clock_t End_time = clock();
    std::cout<<std::endl;
    std::cout<<std::endl;
//    std::cout<<"Total running time is: "<<static_cast<double>(End_time-Start_time)/CLOCKS_PER_SEC*1000<<" ms"<<std::endl;

}


/////////////////////////////////////////////////////////////////////////////
void k_Heapify(int arr[], int index, int heapsize) {
    int left  = index * 2 + 1;
    int right = left + 1;
    int largest = index;

    while (left < heapsize) {

        if (arr[left] > arr[index]) {
            largest = left;
        }
        if (right < heapsize && arr[right] > arr[largest]) {
            largest = right;
        }
        if (largest != index) { std::swap(arr[largest], arr[index]); }
        else { break; }

        index = largest;
        left  = index * 2 + 1;
        right = left + 1;
    }
}
// 这样就得到了初始堆
void heapInsert(int heap_arr[], int val, int index) {
    heap_arr[index] = val;
    while (index != 0) {
        int parent = (index - 1) / 2;
//        printf( "parent=%d\n", parent );
        if (heap_arr[parent] < heap_arr[index]) {
            std::swap(heap_arr[parent], heap_arr[index]);
            index = parent;
        } else { break; }
    }
}
// N个无序数列内，提取最小K个数 === > 也就是TOP K
void getMinKByHeap( int arr[], int length, int k, int kHeap[] ) {
//    if (k < 1)
    for (int i = 0; i != k; ++i) {
        heapInsert(kHeap, arr[i], i);
    }
    std::cout << "aaa" <<std::endl;
    for (int i = k; i < length; ++i) {
        if (arr[i] < kHeap[0]) {
            std::swap(kHeap[0], arr[i]);
            k_Heapify(kHeap, 0, k);
        }
    }
}
// N个无序数列内，提取最小K个数 === > 也就是TOP K
int tst_heapsort_fun_3() {
//    clock_t Start_time = clock();
    int arrsource[] = {12,45,748,13,56,3,89,4,48,2};
    int min_heap_k[10] = {0};

    int ksize     = array_size(min_heap_k);
    int sizeArray = array_size(arrsource);

    getMinKByHeap(arrsource, sizeArray, ksize, min_heap_k);

    for (size_t i = 0; i != ksize; ++i) {
        std::cout << min_heap_k[i] <<" ";
    }
    std::cout << std::endl;
//    clock_t End_time = clock();
//    std::cout<<std::endl;
//    std::cout<<"Total running time is: "<<static_cast<double>(End_time-Start_time)/CLOCKS_PER_SEC*1000<<" ms"<<std::endl;

}
/////////////////////////////////////////////////////////////////////////////

void PrintInitHeap() {
    std::cout << "PrintInitHeap begin" << std::endl;
//    int arrsource[] = {12,45,748,13,56,3,89,4,48,2};
    int arrsource[] = {16,7,3,20,17,8};
    int min_heap_k[6] = {0};
    int i = 0;
    int k = array_size(min_heap_k);

    // 这样就得到了初始堆
    for (i = 0; i != k; ++i) {
        heapInsert(min_heap_k, arrsource[i], i);
    }
    for (i = 0; i != k; ++i) {
        std::cout << min_heap_k[i] <<" ";
    }
    std::cout << std::endl;
    std::cout << "PrintInitHeap end" << std::endl;
}

int g_test;
class A {
public:
    int a;
    virtual void f(){}
};

int tst_HeapSortEntry_() {

    PrintInitHeap(); //return 1;

    std::cout << "size A=" << sizeof(A) << std::endl;
    A aaa;
    std::cout << "size aaa=" << sizeof(aaa) << std::endl;

//    tst_heapsort_fun_3(); return 0;

    tst_heapsort_fun_2(); return 0;

    tst_heapsort_fun_1();

    return 0;
}

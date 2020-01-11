

#include "quicksort.h"


#include <stdio.h>

#include <iostream>
#include <sstream>

#include <iostream>
#include <string>
#include <algorithm>
#include <stack>
#include <vector>
#include <queue>

#include <fstream>
#include <thread>

#include <boost/bind.hpp>

#include "muduo/base/common.h"


using namespace std;
//using namespace muduo;
//using namespace muduo::net;


const int MAX_LENGTH_INSERT_SORT = 7;


void justPrint(int arr[], int length) {
    for ( size_t idx = 0; idx < length; ++idx ) {
        std::cout << arr[idx] << " ";
    }
    std::cout << std::endl;
}

// 选取左中右三个元素，求出中位数，放入数组最左边的a[low]中
int selectMiddleOfThree(int arr[], int low, int high) {
    int middle = low + (high - low) / 2;// 取得位于数组中间的元素middle

    if (arr[low] > arr[high])
        std::swap(arr[low], arr[high]);//此时有 a[low] < a[high]

    if (arr[middle] > arr[high])
        std::swap(arr[middle], arr[high]);//此时有 a[low], a[middle] < a[high]

    if (arr[middle] > arr[low]) {
        std::swap(arr[middle], arr[low]); //此时有a[middle]< a[low] < a[high]
    }
    return arr[low];// a[low]的值已经被换成三数中的中位数， 将其返回
}
// 应该很不错的实现了，下一步比这个跟好的应该是随机选取支点的算法实现
int partion_new(int arr[], int low, int high, int length) {
    int i = low, j = high+1;      // i, j为左右扫描指针 PS： 思考下为什么j比i 多加一个1呢？

    // pivotkey 为选取的基准元素（头元素）
//    int pivotkey = arr[low];
    // pivotkey 中位数
    int pivotkey = selectMiddleOfThree(arr, low, high);

    while (true) {
        // 当随着右游标左移，到j = low + 1的时候，
        // 有 a[--j] == pivotkey为true（两者都是基准元素），
        // 自动跳出了while循环，所以就不需要在循环体里再判断 j == low 了
        while (arr[--j] > pivotkey) {
            // 第一个内部while循环体里面的的  if(j == low) break；判断其实是多余的，可以去除
//            if (j == low) break;// 右游标左移
        }
        while (arr[++i] < pivotkey) {// 左游标右移
            if (i == high) break;
        }
        if (i >= j) {
            break;// 左右游标相遇时候停止， 所以跳出外部while循环
        }
        else {
            std::swap(arr[i], arr[j]);
        }
    }
    std::swap(arr[low], arr[j]);
    printf( "last swap arr[%d]=%d arr[%d]=%d\n", low, arr[low], j, arr[j] );

    std::cout << "before swap----" << std::endl;
    for (size_t i = 0; i != length; ++i) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;
    std::cout << "end swap----" << std::endl;
    std::cout << std::endl;
    return j;
}
void quickSort_new_2( int a[], int left, int right, int length ) {
    if (length > MAX_LENGTH_INSERT_SORT) {// 待排序数组长度大于临界值，则进行快速排序
        int pivotLoc; // 记录枢轴(pivot)所在位置
        if (left < right) {// 2. 优化小数组时的排序方案，将快速排序改为插入排序
            pivotLoc = partion_new(a, left, right, length);// 将arr[low...high]一分为二,并返回枢轴位置
            std::cout << "pivotLoc=" << pivotLoc << std::endl;

            quickSort_new_2(a, left, pivotLoc - 1, length);
            quickSort_new_2(a, pivotLoc + 1, right, length);
        }
    } else {
//        insertSort(a, left, right, length);
    }
}

// 从数组的两端向中间扫描
int partion_1(int arr[], int left, int right, int length) {
    int pivot = arr[left];

    while (left < right) {
        while (left < right && arr[right] >= pivot)
            right --;
        if (left == right) {
            std::cout << "111 left==right break" << std::endl;
            break;
        }
        std::cout << "111 ready swap arr[" << left << "]=" << arr[left]
                  << ", arr[" << right << "]=" << arr[right] << ", pivot=" << pivot << std::endl;
        if (left < right) {
            std::swap(arr[left], arr[right]);
//            arr[left++] = arr[right];//将比key小的元素移到低端
        }

//        std::cout              << "swap begin----" << std::endl;
        for (size_t i = 0; i != length; ++i)
            std::cout << arr[i] << " ";
//        std::cout << std::endl << "swap end----" << std::endl << std::endl;
        std::cout << std::endl;
        std::cout << std::endl;

//        std::cout << "left=" << left << ", right=" << right << std::endl;
        while (left < right && arr[left] < pivot)
            left ++;
        if (left == right) {
            std::cout << "222 left==right break" << std::endl;
            break;
        }
        std::cout << "222 ready swap a[" << left << "]=" << arr[left]
                  << ", arr[" << right << "]=" << arr[right] << ", pivot=" << pivot << std::endl;
        if (left < right) {
            std::swap(arr[left], arr[right]);
//            arr[right--] = arr[left];//将比key大的元素移到高端
        }

        for (size_t i = 0; i != 10; ++i)
            std::cout << arr[i] << " ";
//        std::cout << std::endl << "swap end----" << std::endl << std::endl;
        std::cout << std::endl;
        std::cout << std::endl;
    }
    arr[left] = pivot;
    return left;
}
//
int partion(int arr[], int left, int right, int length) {
    int flag = arr[left];
    int j = left;

    for (int i = left + 1; i <= right; ++ i) {

        if (arr[i] < flag) {
            j ++;
            if (j != i) {
                std::swap(arr[i], arr[j]);
            }
        }

    }
    std::swap(arr[j], arr[left]);
    return j;
}
void insertSort( int arr[], int left, int right, int length ) {// 对子序列arr[low...high]进行插入排序
    int i, j, tmp;
    for (i = left+1; i < right; i++) {// 从下标low+1开始遍历,因为下标为low的已经排好序
        if (arr[i] < arr[i-1]) {
            // 如果当前下标对应的记录小于前一位记录,则需要插入,否则不需要插入，直接将记录数增加1
            tmp = arr[i]; // 记录下标i对应的元素
            for (j = i-1; j >= left && arr[j] > tmp; j--) {
                arr[j+1] = arr[j];// 记录后移
            }
            arr[j+1] = tmp;
        }
    }
}
void InsertionSort(int *a, int len)
{
    for ( int j = 1; j < len; ++j ) {
        int key = a[ j ];
        int i = j - 1;

        while ( i >= 0 && a[i] > key ) {
            a[ i + 1 ] = a[ i ];
            i --;
        }
        a[ i + 1 ] = key;
    }
    return ;

    for (int j = 1; j < len; j++)
    {
        int key = a[j];
        int i   = j - 1;
        while (i >= 0 && a[i] > key)
        {
            a[i + 1] = a[i];
            i--;
        }
        a[i + 1] = key;
    }
}


void quickSort( int a[], int left, int right, int length ) {
    if (length > MAX_LENGTH_INSERT_SORT) {// 待排序数组长度大于临界值，则进行快速排序
        int pivotLoc; // 记录枢轴(pivot)所在位置
        if (left < right) {// 2. 优化小数组时的排序方案，将快速排序改为插入排序
//            pivotLoc = partion(a, left, right, length);// 将arr[low...high]一分为二,并返回枢轴位置
            pivotLoc = partion_1(a, left, right, length);// 将arr[low...high]一分为二,并返回枢轴位置
            std::cout << "pivotLoc=" << pivotLoc << std::endl;

            quickSort(a, left, pivotLoc/*pivotLoc - 1*/, length);// 递归遍历arr[low...pivotLoc-1]
            quickSort(a, pivotLoc + 1, right, length);// 递归遍历arr[pivotLoc+1...high]
        }
    } else {
//        insertSort(a, left, right, length);
    }
}
void printf_array( int* arr, int length ) {
    for (size_t i = 0; i != length; ++i) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;
}



void tst_qs_111() {
    int arr[10] = {12, 45, 748, 15, 56, 3, 89, 4, 48, 2};
//    int a[10] = {2, 3, 4, 12, 15, 45, 48, 56, 89, 748};
//    int a[] = {5, 1, 9, 3, 7, 4, 8, 6, 2};
    int length = sizeof(arr)/sizeof(int);
    std::cout << "length=" << length << std::endl;
    std::cout << std::endl;

    std::cout << "before sort----" << std::endl;
    printf_array( arr, length );

    InsertionSort( arr, length );

    std::cout << std::endl << "after sort----" << std::endl;
    printf_array( arr, length );
    cout << std::endl;
    return ;

    std::cout << "before sort----" << std::endl;
    printf_array( arr, length );
    quickSort_new_2( arr, 0, length - 1, length );
//    quickSort(arr, 0, length - 1, length);

//    std::cout << "------------------" << std::endl;
    std::cout << std::endl << "after sort----" << std::endl;
    for (size_t i = 0; i != length; ++i) {
        std::cout << arr[i] << " ";
    }
    cout << std::endl;
    cout << std::endl;
}

int p_new(int a[], int l, int r, int length) {
    int i = l, j = r + 1;

    int key = a[l];
    while (true) {
        while (a[--j] > key) {}
        if ( i == j )
            break;
        if ( i < j )
            std::swap(a[i], a[j]);

        while (a[++i] < key) {
            if (i == j) break;
        }
        if (i >= j) {
            break;
        } else {
            std::swap(a[i], a[j]);
        }
    }
    std::swap(a[l], a[j]);
}
void qs_new(int arr[], int left, int rigth, int length) {
    int pivocLoc;
    if (left < rigth) {
        pivocLoc = p_new(arr, left, rigth, length);
        qs_new(arr, left, pivocLoc, length);
        qs_new(arr, pivocLoc + 1, rigth, length);
    }
}
class SolutionQS {
    int partition(int arr[], int l, int r) {
        int index = l, pivot = arr[r];

        for (int i=l; i<r; ++i)
            if (arr[i] <= pivot) std::swap(arr[i], arr[index++]);

        std::swap(arr[index], arr[r]);
        return index;
    }
public:
    void quickSort(int arr[], int l, int r) {
        if (l < r) {
            int pivot = partition(arr, l, r);
            quickSort(arr, l, pivot-1);
            quickSort(arr, pivot+1, r);
        }
    }
};



// 简洁靠谱 快排实现
/*
 * 功能：对数组升序快排，递归实现
 * 参数：lst：带排序数组，head：数组首元素下标，tail：数组末元素下标
 * 返回值：无
 */
template<typename T>
void t_qsort(T lst[], int head, int tail) {
    if (head >= tail) return;

    int i = head, j = tail;

    T pivot = lst[head];  // 通常取第一个数为基准
    while (i < j) { // i,j 相遇即退出循环
        while (i < j && lst[j] >= pivot) j--;
        lst[i] = lst[j];    // 从右向左扫描，将比基准小的数填到左边
        while (i < j && lst[i] <= pivot) i++;
        lst[j] = lst[i];    //  从左向右扫描，将比基准大的数填到右边
    }
    lst[i] = pivot; // 将 基准数 填回

    t_qsort(lst, head, i - 1);    // 以基准数为界左右分治
    t_qsort(lst, j + 1, tail);
}
// 简洁靠谱 快排实现
void my_qs(int arr[], int head, int tail) {
    if (head >= tail) return;

    int i = head, j = tail;
    int piv = arr[i];
    while (i < j) {
        while (i < j && arr[j] > piv) j --;
        if (i < j)
            arr[i++] = arr[j];
//            std::swap( arr[i++], arr[j] );

        while (i < j && arr[i] <= piv) i ++;
        if (i < j)
            arr[j--] = arr[i];
//            std::swap( arr[j--], arr[i] );
    }
    arr[ i ] = piv;

    my_qs( arr, head, i - 1 );
    my_qs( arr, j + 1, tail );
}
void f_1(int arr[], int head, int tail) {
    if (head >= tail) return;

    int i = head, j = tail;
    int piv = arr[i];
    while (i < j) {
        while (j < j && arr[j] > piv) j--;
        if (i < j) arr[i++] = arr[j];

        while (i < j && arr[i] <= piv) i++;
        if (i < j) arr[j--] = arr[i];
    }
    arr[i] = piv;

    f_1( arr, head, i - 1 );
    f_1( arr, j + 1, tail );
}
void f_2(int arr[], int head, int tail) {
    if (head >= tail) return;

    int i = head, j = tail;
    int piv = arr[i];

    while (i < j) {
        while (i < j && arr[j] > piv) j --; // 从右向左找第一个小于x的数
        if (i < j) arr[i++] = arr[j];

        while (i < j && arr[i] <= piv) i++;
        if (i < j) arr[j--] = arr[i];// 从左向右找第一个大于x的数
    }
    arr[i] = piv;
    f_2( arr, head, i - 1 );
    f_2( arr, j + 1, tail );
}
void f_3(int a[], int head, int tail) {
//    if (head < tail) return;

//    int i = head, j = tail;
//    int piv = arr[i];
//    while (i < j) {
//        while (i<j && a[j] > piv) j --;
//        if (i<j) a[i++] = a[j];
//        while (i<j && a[i] < piv) i ++;
//        if (i<j) a[j--]
//    }
//    a[i] = piv;
//    f_3(a, head, j - 1);
//    f_3(a, i + 1, tail);
}


int SelectPivotMedianOfThree(int arr[],int low, int high)
{
    {
        int mid = low + ( (high - low) >> 1 );
        if (arr[mid] > arr[high])
            swap(arr[mid], arr[high]);
        if (arr[low] > arr[high])
            swap(arr[low], arr[high]);
        if (arr[mid] > arr[low])
            swap(arr[mid], arr[low]);
        return arr[low];
    }

    int mid = low + ((high - low) >> 1);//计算数组中间的元素的下标

    //使用三数取中法选择枢轴
    if (arr[mid] > arr[high])//目标: arr[mid] <= arr[high]
    {
        swap(arr[mid],arr[high]);
    }
    if (arr[low] > arr[high])//目标: arr[low] <= arr[high]
    {
        swap(arr[low],arr[high]);
    }
    if (arr[mid] > arr[low]) //目标: arr[low] >= arr[mid]
    {
        swap(arr[mid],arr[low]);
    }
    //此时，arr[mid] <= arr[low] <= arr[high]
    return arr[low];
    //low的位置上保存这三个位置中间的值
    //分割时可以直接使用low位置的元素作为枢轴，而不用改变分割函数了
}

void newfun(int* a, int head, int tail) {
    if (!a || head >= tail) return;

    {
        int i = head;
        int j = tail;
        int x = a[i];
//        int x = SelectPivotMedianOfThree(a, head, tail);
//        std::cout << "x=" << x << std::endl;

        while ( i < j ) {

            while ( i < j && a[ j ] > x ) j --;
            if ( i < j ) a[ i ++ ] = a [ j ];

            while ( i < j && a[ i ] <= x ) i ++;
            if ( i < j ) a[ j -- ] = a [ i ];

        }
        std::cout << "i=" << i << " j=" << j << std::endl;
        a [ i ] = x;

        newfun ( a, head, i - 1 );
        newfun ( a, i + 1, tail );
        return;
    }

    int i = head;
    int j = tail;
    int x = a[i];

    while ( i < j ) {

        while ( i < j && a[ j ] > x ) j --;
        if ( i < j ) a[ i ++ ] = a[ j ];

        while ( i < j && a[ i ] < x ) i ++;
        if ( i < j ) a[ j -- ] = a[ i ];

    }
    a [ i ] = x;

    newfun( a, head, i - 1 );
    newfun( a, i + 1, tail );

}
void tst_qs_new() {
//    int arr[] = {12, 45, 748, 3986, 15, 56, 3, 89, 4, 48, 2, 0, -1, 1024, };
//    int arr[] = { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 };
//    int arr[] = { 1, 2, 3, 4, 5, 6 };
    int arr[]  = { 6, 5, 4, 3, 2, 1 };
    int length = sizeof(arr) / sizeof(int);
    std::cout << "length=" << length << std::endl;

    justPrint( arr, length );

    newfun( arr, 0, length - 1 );
//    f_2(arr, 0, length - 1);
//    my_qs( arr, 0, length - 1 );
//    t_qsort<int>( arr, 0, length - 1 );
//    SolutionQS cqs;
//    cqs.quickSort( arr, 0, length - 1 );

    justPrint( arr, length );
}














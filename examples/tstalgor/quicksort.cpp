

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

void tst_qs_111() {
    int arr[10] = {12, 45, 748, 15, 56, 3, 89, 4, 48, 2};
//    int a[10] = {2, 3, 4, 12, 15, 45, 48, 56, 89, 748};
//    int a[] = {5, 1, 9, 3, 7, 4, 8, 6, 2};
    int length = sizeof(arr)/sizeof(int);
    std::cout << "length=" << length << std::endl;
    std::cout << std::endl;

    std::cout << "before sort----" << std::endl;
    for (size_t i = 0; i != length; ++i) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;

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








void tst_qs_222() {

}













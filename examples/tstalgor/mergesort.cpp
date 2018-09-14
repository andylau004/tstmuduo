
#include "mergesort.h"






//low为本次二路归并排序的第1有序区的第1个元素，i指向第1个元素, mid为第1有序区的最后1个元素
void mergeImpl(int *data, int start, int mid, int end, int *result) {
    int i, j, k;
    //i 指向第1有序区的第1个元素
    i = start;
    //mid+1为第2有序区第1个元素，mid为第1有序区的最后1个元素
    //j 指向第 2 有序区的第 1 个元素，high 为第 2 有序区的最后一个元素
    j = mid + 1;                        //避免重复比较data[mid]
    k = 0;

    //顺序选取两个有序区的较小元素，存储到t数组中，因为是递增排序
    while (i <= mid && j <= end) {      //数组data[start,mid]与数组(mid,end]均没有全部归入数组result中去
        if (data[i] <= data[j])         //如果data[i]小于等于data[j]
            result[k++] = data[i++];    //则将data[i]的值赋给result[k]，之后i,k各加一，表示后移一位
        else
            result[k++] = data[j++];    //否则，将data[j]的值赋给result[k]，j,k各加一
    }//end of while

    while (i <= mid)                    //表示数组data(mid,end]已经全部归入result数组中去了，而数组data[start,mid]还有剩余
        result[k++] = data[i++];        //将数组data[start,mid]剩下的值，逐一归入数组result
    while (j <= end)                    //表示数组data[start,mid]已经全部归入到result数组中去了，而数组(mid,high]还有剩余
        result[k++] = data[j++];        //将数组a[mid,high]剩下的值，逐一归入数组result

    for (i = 0; i < k; i++)             //将归并后的数组的值逐一赋给数组data[start,end]
        data[start + i] = result[i];    //注意，应从data[start+i]开始赋值
}
void Recrusive_merge_sort(int *data, int start, int end, int *result) {
    if (start < end) {
        int mid = start + (end-start) / 2;//避免溢出int
        Recrusive_merge_sort(data, start, mid, result);                    //对左边进行排序
        Recrusive_merge_sort(data, mid + 1, end, result);                  //对右边进行排序
        mergeImpl(data, start, mid, end, result);                          //把排序好的数据合并
    }
}
void amalgamation(int *data1, int *data2, int *result) {
    for (int i = 0; i < 10; i++)
        result[i] = data1[i];
    for (int i = 0; i < 10; i++)
        result[i + 10] = data2[i];
}
//测试递归算法
void tst_R_merge_1() {
    int data1[10] = { 1,7,6,4,9,14,19,100,55,10 };
    int data2[10] = { 2,6,8,99,45,63,102,556,10,41 };
    int *result  = new int[20];
    int *result1 = new int[20];

    std::cout << "data1 len=" << array_size(data1) << std::endl;
    std::cout << "data2 len=" << array_size(data2) << std::endl;
    std::cout << std::endl;

    std::cout << "-----------------first beg" << std::endl;
    amalgamation(data1, data2, result);
    for (int i = 0; i < 20; ++i)
        std::cout << result[i] << "  ";
    std::cout << std::endl;
    std::cout << "-----------------first end" << std::endl;

    Recrusive_merge_sort(result, 0, 19, result1);

    std::cout << std::endl;
    std::cout << "-----------------sorted beg" << std::endl;
    for (int i = 0; i < 20; ++i)
        std::cout << result[i] << "  ";
    std::cout << std::endl;
    std::cout << "-----------------sorted beg" << std::endl;

    delete[]result;
    delete[]result1;
}

// 非递归方式
void No_Recrusive_merge_sort(int *data, int start, int end, int *result) {
    //回忆图解的过程，二路归并算法的流程，不同于递归，递归是先递归语句，然后归并函数，这样归并函数是倒序执行（和递归函数执行顺序相反）
    int size = 1;
    int low, mid, high;
    //size是标记当前各个归并序列的high-low，从1，2，4，8，……，2*size
    while (size <= end) {

        //从第一个元素开始扫描，low代表第一个分割的序列的第一个元素
        low = 0;
        //当前的归并算法结束的条件
        while (low + size <= end) {
            //mid代表第一个分割的序列的最后一个元素
            mid  = low + size - 1;
            //high 代表第二个分割的序列的最后一个元素
            high = mid + size;
            //判断一下：如果第二个序列个数不足size个
            if (high > end) {
                //调整 high 为最后一个元素的下标即可
                high = end - 1;
            }
            //调用归并函数，进行分割的序列的分段排序
            mergeImpl(data, start, mid, end, result);
            //打印出每次归并的区间
            std::cout << "low:" << low << " mid:" << mid << " high:" << high << std::endl;
            //下一次归并时第一序列的第一个元素位置
            low = high + 1;

        }// while2 end

        //范围扩大一倍，二路归并的过程
        size *= 2;
    }// while1 end

}
// 测试非递归算法
void tst_NoR_merge_1() {
    std::cout << "tst_NoR_merge_1 beg" << std::endl;

    int data1[10] = { 1,7,6,4,9,14,19,100,55,10 };
    int data2[10] = { 2,6,8,99,45,63,102,556,10,41 };
    int *result  = new int[20];
    int *result1 = new int[20];

    std::cout << "data1 len=" << array_size(data1) << std::endl;
    std::cout << "data2 len=" << array_size(data2) << std::endl;
    std::cout << std::endl;

    std::cout << "-----------------first beg" << std::endl;
    amalgamation(data1, data2, result);
    for (int i = 0; i < 20; ++i)
        std::cout << result[i] << "  ";
    std::cout << std::endl;
    std::cout << "-----------------first end" << std::endl;

    No_Recrusive_merge_sort(result, 0, 19, result1);

    std::cout << std::endl;
    std::cout << "-----------------sorted beg" << std::endl;
    for (int i = 0; i < 20; ++i)
        std::cout << result[i] << "  ";
    std::cout << std::endl;
    std::cout << "-----------------sorted beg" << std::endl;

    delete[]result;
    delete[]result1;
    std::cout << "tst_NoR_merge_1 end" << std::endl;
}

int tst_MergeSortEntry_() {

    tst_NoR_merge_1(); return 0;

    return 0;
}

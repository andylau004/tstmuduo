
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




/**
给定K个有序数组，每个数组有n个元素，想把这些数组合并成一个有序数组。

算法原理及实现
一. 最简单的方法是创建一个n*k大小的数组，然后把所有数字拷贝进去，然后再进行时间复杂度为O(nlogn)排序算法，这样总体时间复杂度为O(nklognk)

二. 可以利用最小堆完成，时间复杂度是O(nklogk)，具体过程如下：

创建一个大小为n*k的数组保存最后的结果
创建一个大小为k的最小堆，堆中元素为k个数组中的每个数组的第一个元素
重复下列步骤n*k次：
每次从堆中取出最小元素（堆顶元素），并将其存入输出数组中
用堆顶元素所在数组的下一元素将堆顶元素替换掉，
如果数组中元素被取光了，将堆顶元素替换为无穷大。每次替换堆顶元素后，重新调整堆
下面给出相关算法的C++实现代码
 **/
#define n 4

// 定义最小堆节点
struct MinHeapNode
{
    int element; // 带排序元素
    int i; // 数组索引，element是从哪个数组取得
    int j; // 元素索引，下一个element的索引
};

// 定义一个最小节点之间的比较函数
void swap(MinHeapNode *x, MinHeapNode *y);

class MinHeap
{
    MinHeapNode *harr; // 定义一个指针，指向最小堆的堆顶元素
    int heap_size; // 堆的大小
public:
    // 构造函数（建堆）：构造一个指定大小的最小堆
    MinHeap(MinHeapNode a[], int size);

    // 调整堆：调整给定根结点的子树结构
    void MinHeapify(int );

    // 给定节点的左子节点
    int left(int i) { return (2*i + 1); }

    // 给定节点的右子节点
    int right(int i) { return (2*i + 2); }

    // 获取根节点值（堆顶元素）
    MinHeapNode getMin() { return harr[0]; }

    // 替换根节点（堆顶元素），重新调整堆
    void replaceMin(MinHeapNode x) { harr[0] = x;  MinHeapify(0); }
};
int *mergeKArrays(int arr[][n], int k)
{
    int *output = new int[n*k];  // 最后输出的数组，保存排序结果

    // 创建一个大小为k的最小堆，堆中元素为k个数组中的每个数组的第一个元素
    MinHeapNode *harr = new MinHeapNode[k];
    for (int i = 0; i < k; i++)
    {
        harr[i].element = arr[i][0]; // 每个数组的第一个元素（每个数组的最小元素）
        harr[i].i = i;  // 当前数组索引
        harr[i].j = 1;  // 该数组下一个元素的索引（判断元素是否用完）
    }
    MinHeap hp(harr, k); // 对上述大小为k的数组建堆

    // 逐次取出堆顶元素，存入输出数组中，并将其替换为所在数组的下一元素
    for (int count = 0; count < n*k; count++)
    {
        // 取堆顶，存结果
        MinHeapNode root = hp.getMin();
        output[count] = root.element;

        // 替换堆顶
        if (root.j < n)
        {
            root.element = arr[root.i][root.j];
            root.j += 1;
        }
        // 当前数组元素用完了，设堆顶为无穷大
        else root.element =  INT_MAX; //INT_MAX is for infinite

        // 如果还有元素，就替换堆顶元素，调整堆结构
        hp.replaceMin(root);
    }

    return output;
}
MinHeap::MinHeap(MinHeapNode a[], int size)
{
    heap_size = size;
    harr = a;  // store address of array
    int i = (heap_size - 1)/2;
    while (i >= 0)
    {
        MinHeapify(i);
        i--;
    }
}

// 递归的调整
// This method assumes that the subtrees are already heapified
void MinHeap::MinHeapify(int i)
{
    int l = left(i);
    int r = right(i);
    int smallest = i;
    if (l < heap_size && harr[l].element < harr[i].element)
        smallest = l;
    if (r < heap_size && harr[r].element < harr[smallest].element)
        smallest = r;
    if (smallest != i)
    {
        swap(&harr[i], &harr[smallest]);
        MinHeapify(smallest);
    }
}
void swap(MinHeapNode *x, MinHeapNode *y)
{
    MinHeapNode temp = *x;  *x = *y;  *y = temp;
}

// 输出排序结果
void printArray(int arr[], int size)
{
    for (int i=0; i < size; i++)
        cout << arr[i] << " ";
}


int tst_example_1()
{
    int arr[][n] =  {{2, 6, 12, 34},
                     {1, 9, 20, 1000},
                     {23, 34, 90, 2000}};
    int k = sizeof(arr)/sizeof(arr[0]);
    cout << "k is " << k << endl;

    int *output = mergeKArrays(arr, k);

    cout << "Merged array is " << endl;
    printArray(output, n*k);
    cout << endl;

    return 0;
}




void Merge(int sourceArr[],int tempArr[], int startIndex, int midIndex, int endIndex) {
    int i = startIndex, j=midIndex+1, k = startIndex;
    while(i!=midIndex+1 && j!=endIndex+1)
    {
        if(sourceArr[i] > sourceArr[j])
            tempArr[k++] = sourceArr[j++];
        else
            tempArr[k++] = sourceArr[i++];
    }
    while(i != midIndex+1)
        tempArr[k++] = sourceArr[i++];
    while(j != endIndex+1)
        tempArr[k++] = sourceArr[j++];
    for(i=startIndex; i<=endIndex; i++)
        sourceArr[i] = tempArr[i];
}
//内部使用递归
void MergeSort(int sourceArr[], int tempArr[], int startIndex, int endIndex) {
    int midIndex;
    if(startIndex < endIndex)
    {
        midIndex = (startIndex + endIndex) / 2;
        MergeSort(sourceArr, tempArr, startIndex, midIndex);
        MergeSort(sourceArr, tempArr, midIndex+1, endIndex);
        Merge(sourceArr, tempArr, startIndex, midIndex, endIndex);
    }
}
//归并排序和堆排序、快速排序的比较
//若从空间复杂度来考虑：首选堆排序，其次是快速排序，最后是归并排序。
//若从稳定性来考虑，应选取归并排序，因为堆排序和快速排序都是不稳定的。
//若从平均情况下的排序速度考虑，应该选择快速排序。
void tst_merge_sort_1() {
    int a[8] = {50, 10, 20, 30, 70, 40, 80, 60};
    int i, b[8];
    MergeSort(a, b, 0, 7);
    for(i=0; i<8; i++)
        printf("%d ", a[i]);
    printf("\n");
}

int tst_MergeSortEntry_() {
    tst_merge_sort_1(); return 1;

    tst_example_1(); return 0;

    tst_NoR_merge_1(); return 0;

    return 0;
}

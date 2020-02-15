

#include "heapopr.h"



#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>


#include "muduo/base/common.h"


using namespace std;




bool cmp_heap_timer_node(heap_timer_node *node1, heap_timer_node *node2) {
    return node1->expire > node2->expire;
}

timer_heap::timer_heap(int cap) {
    this->cap = cap;
    this->array.resize(cap);
    this->cur_size = 0;
    this->del_num = 0;
    for (int i = 0; i < cap; i++) {
        this->array[i] = nullptr;
    }
    make_heap(array.begin(), array.begin() + cur_size, cmp_heap_timer_node);
}
/* 添加一个定时器 */
void timer_heap::add_timer(heap_timer_node *timer) {
    if (!timer) return;
    if (cur_size >= cap) {     /* 无空间，则将array扩大一倍 */
        resize();
    }

    /* 插入新元素 */
    array[cur_size] = timer;
    cur_size++;

    /* 调整堆 */
    push_heap(array.begin(), array.begin() + cur_size, cmp_heap_timer_node);
}

void timer_heap::del_timer(heap_timer_node *timer) {
    if (!timer) return;

    /* 仅仅将目标定时器的回调函数设置为空，即所谓的延迟销毁，
     * 将节省真正删除该定时器造成的开销，但这样做容易造成堆数组膨胀 */
    timer->cb_func = nullptr;
    timer->validFlag = false;
    del_num ++;

    /* 判断删除数是否达到当前容量的一半，达到了则删除并重新建堆 */
    if (del_num >= cur_size/2) {

        int vaildIndex = 0;
        for ( int i = 0; i < cur_size; i ++ ) {
            if (array[i]->validFlag) {
                array[vaildIndex] = array[i];
                vaildIndex ++;
            } else {
                delete array[i];
            }
            array[i] = nullptr;
        }
        cur_size = vaildIndex;

        make_heap(array.begin(), array.begin() + cur_size, cmp_heap_timer_node);
    }

}

/* 获取堆顶定时器 */
heap_timer_node *timer_heap::get_top() {
    if (is_empty()) {
        return nullptr;
    } else {
        return array.front();
    }
}
/* 删除堆顶定时器 */
void timer_heap::del_top() {
    /* pop_heap 将堆顶元素移至当前堆的最末尾位置，及array[array.begin() + cur_size-1]处*/
    pop_heap(array.begin(), array.begin() + cur_size, cmp_heap_timer_node);
    /* 删除该定时器 */
    delete array[cur_size - 1];
    array[cur_size - 1] = nullptr;
    cur_size--;
}

void timer_heap::tick() {
    heap_timer_node* topTimer = get_top();
    time_t cur = time(nullptr);
    while (!is_empty()) {
        if (topTimer == nullptr)
            break;
        /* 如果对顶元素还没有到期，则退出循环 */
        if (topTimer->expire > cur)
            break;
        /* 否则执行堆顶定时器中的任务 */
        if (topTimer->validFlag && topTimer->cb_func)
            topTimer->cb_func(topTimer->user_data);
        del_top();
        topTimer == get_top();
    }
}

void timer_heap::resize() {
    array.resize(cap*2);
    cap = cap * 2;
}

void timer_heap::disPlay() {
    cout << "disPlay: ";
    for (int i = 0; i < cap; i++) {
        if (array[i] == nullptr) {
            break;
        }
        cout << array[i]->expire << " ";
    }
    cout << endl;
}

/* 判断堆是否为空 */
bool timer_heap::is_empty() {
    return cur_size == 0;
}
/* 析构函数，销毁堆 */
timer_heap::~timer_heap() {
    for (int i = 0; i < cur_size; i++) {
        delete array[i];
        array[i] = nullptr;
    }
}


void printVec(vector<int> nums) {
    for (int i = 0; i < nums.size(); ++i)
        cout << nums[i] << " ";
    cout << endl;
}

template <class T>
void printPriority(T& container) {
    while (!container.empty()) {
        cout << container.top() << ' ';
        container.pop();
    }
    cout << endl;
}

void tst_heapopr_entry() {

    {
        int myints[] = { 10, 20, 30, 6, 16 };

        std::vector<int> vec(myints, myints+5);
    //    std::vector::iterator it;

        make_heap(vec.begin(), vec.end());
        std::cout << "initial max heap=" << vec.front() << std::endl;

        pop_heap(vec.begin(), vec.end());
        vec.pop_back();
        cout << "max heap after pop : " << vec.front() << endl;

        vec.push_back(99);
        push_heap(vec.begin(), vec.end());
        cout << "max heap after push: " << vec.front() << endl;
        return;
    }

    {
        std::priority_queue < int > a;// 默认是大顶堆
        std::priority_queue < int, std::vector<int>, greater<int> > c;// 小顶堆

        for (int i = 0; i < 5; i++)  {
            a.push(i);
            c.push(i);
        }
        printPriority(a);
        printPriority(c);

        return ;
    }

    {
        int nums_temp[] = { 8, 3, 4, 8, 9, 2, 3, 4 };
        int lenNums = array_size(nums_temp);
        cout << "len=" << lenNums << std::endl;
//        return;

        vector<int> nums(nums_temp, nums_temp + lenNums);
        cout << "sorce nums: "; printVec(nums);
        cout << "(默认)make_heap: ";
        make_heap(nums.begin(), nums.end()); //默认是大顶堆，只是将最大值的元素和第一个元素互换,其他元素的顺序不变
        printVec(nums);

        cout << "(less)make_heap: ";
        make_heap(nums.begin(), nums.end(), less<int>()); //创建大顶堆，只是将最大值的元素和第一个元素互换,其他元素的顺序不变
        printVec(nums);

        cout << "(greater)make_heap: ";
        make_heap(nums.begin(), nums.end(), greater<int>()); //创建小顶堆，只是将最小值的元素和第一个元素互换,其他元素的顺序不变
        printVec(nums);

        cout << "此时，nums为小顶堆, push_back(3)" << endl;
        nums.push_back(3); //在vector的末尾添加元素
        cout << "忽略第三个参数，即为默认)push_heap: ";

        //下面的一行代码，相当于是往vector中添加元素后更新heap,执行到此处会出现错误，只有make_heap（）和push_heap（）同为大顶堆或小顶堆,才能更新。
        //但是此处push_heap使用默认参数，是更新大顶堆，但是上面最后一个make_heap创建的是小顶堆，冲突。
//        push_heap(nums.begin(), nums.end());
        printVec(nums);
        cout << "第三个参数为greater: ";
        push_heap(nums.begin(), nums.end(), greater<int>()); //指定参数后不发生冲突
        printVec(nums);
        cout << "(做替换)默认参数pop_heap: ";
        //pop_heap()并没有删除元素，而是将堆顶元素和数组最后一个元素进行了替换。和上面的push_heap一样第三个参数不指定的话，会发生冲突
        //pop_heap(nums.begin(), nums.end());
        printVec(nums);
        cout << "(做替换)第三个参数为greater,pop_heap: ";
        pop_heap(nums.begin(), nums.end(), greater<int>()); //指定参数后不发生冲突
        printVec(nums);
        cout << "pop_back(): ";
        nums.pop_back(); //删除vector末尾的元素
        printVec(nums);

        return;
    }
    {
        std::vector <int> v{ 3, 1, 4, 1, 5, 9 };
        v.resize(v.size()*2);
        std::cout << "v: ";
        for (auto i : v) std::cout << i << ' ';
        std::cout << '\n';
        return ;
    }
    {
        std::vector <int> v{ 3, 1, 4, 1, 5, 9 };
        std::make_heap(v.begin(), v.end());

        std::cout << "v: ";
        for (auto i : v) std::cout << i << ' ';
        std::cout << '\n';

        v.push_back(6);

        std::cout << "before push_heap: ";
        for (auto i : v) std::cout << i << ' ';
        std::cout << '\n';

        std::push_heap(v.begin(), v.end());

        std::cout << "after push_heap: ";
        for (auto i : v) std::cout << i << ' ';
        std::cout << '\n';

        std::sort_heap(v.begin(), v.end());
        std::cout << "after sort_heap: ";
        for (auto i : v) std::cout << i << ' ';
        std::cout << '\n';

        return;
    }




//    for (const auto& it : vec) {
//        std::cout << " " < *it;
//    }
//    std::cout << std::endl;

}

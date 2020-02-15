

#include "heapopr.h"



#include <iostream>
#include <algorithm>
#include <vector>

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

        int validIndex = 0;
        for ( int i = 0; i < cur_size; i ++ ) {
            if (array[i]->validFlag) {
                array[validIndex] = array[i];
                validIndex ++;
            } else {
                delete array[i];
            }
            array[i] = nullptr;
        }
        cur_size = validIndex;

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
//    heap_timer_node* topTimer = get_top();

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

void tst_heapopr_entry() {

    {
        int nums_temp[] = { 8, 3, 4, 8, 9, 2, 3, 4 };
        cout << "len=" << array_size(nums_temp) << std::endl;
        return;

        vector<int> nums(nums_temp, nums_temp + 8);
        cout << "sorce nums: "; printVec(nums);
        cout << "(默认)make_heap: ";
        make_heap(nums.begin(), nums.end()); //默认是大顶堆，只是将最大值的元素和第一个元素互换,其他元素的顺序不变
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



//    for (const auto& it : vec) {
//        std::cout << " " < *it;
//    }
//    std::cout << std::endl;

}

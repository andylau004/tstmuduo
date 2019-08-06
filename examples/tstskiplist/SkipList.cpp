
#include "SkipList.h"





void SkipList::init(int maxHeight) {
    m_maxHeight = maxHeight;
    m_head = new SkipListNode("", m_maxHeight);
}



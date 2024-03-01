// FOR JH_SQL Project (since 24.02.21 ~) __ Junhyeong(0511)
// B+ Tree Delete Flag -> Projection
#ifndef BPLUSTREE_HPP
#define BPLUSTREE_HPP
#define DEFAULT_NSIZE   5
// key, ptr underflow 기준
#define MINCNT_PTR(_N)  (_N/2 + (_N%2 ? 1 : 0))
#define MINCNT_KEY(_N)  (_N/2 + (_N%2 ? 1 : 0) - 1)


#include <iostream>
#include <cstring>
#include <deque>
using namespace std;


class Bnode {
private:
    friend class BpTree; //Bnode 관리체

    Bnode *parent;   // 부모
    Bnode *prev;     // 왼쪽 형제
    Bnode *next;     // 오른쪽 형제
    deque<Bnode*> ptr;
    deque<string> key;

public:
    Bnode();
    int getPtrSize();
    int getKeySize();
};


class BpTree{
private:
    Bnode* root;
    int nodeCnt;
    int indexCnt;
    int n_size;

//Initalizer
public:
    BpTree();
    BpTree(int nSize);


//in-System Func
private:
    bool    preempt(Bnode* a); // a <-- b 재분배

//Utility
public:
    int     getNodeCnt();
    int     getIndexCnt();
    bool    append(string key);
    void    printAll();
};
/** ---- Implementation ---- **/

// Initializer
Bnode::Bnode() : parent(NULL), prev(NULL), next(NULL) {}
BpTree::BpTree() : nodeCnt(0), indexCnt(0), n_size(DEFAULT_NSIZE) {
    this->root = new Bnode;
}
BpTree::BpTree(int nSize) : nodeCnt(0), indexCnt(0), n_size(nSize) {
    this->root = new Bnode;
}

bool BpTree::preempt(Bnode* a)
{
    //왼쪽 형제 판단
    Bnode* bro = a->prev, *parent = a->parent;
    if (bro && MINCNT_KEY(n_size) < bro->getKeySize()) {
    }
    
    bro = a->next;
    if (bro && MINCNT_KEY(n_size) < bro->getKeySize()) {
        
    }
    return false;
}


//Utility Function __
int BpTree::getNodeCnt() { return this->nodeCnt; }
int BpTree::getIndexCnt() { return this->indexCnt; }
int Bnode::getPtrSize() { return (int)this->key.size(); }
int Bnode::getKeySize() { return (int)this->ptr.size(); }

bool BpTree::append(string key) {
    //find curPos
    Bnode* curNode = root;
    int k_idx = 0, p_idx = 0;
    while(k_idx < curNode->getKeySize()) {
        for (k_idx = 0 ; k_idx < (int)curNode->key.size() ; k_idx++) {
            if (curNode->key[k_idx].compare(key) < 0) 
                break;
        }
        
        p_idx = k_idx;
        if (curNode->getPtrSize() < p_idx || !curNode->ptr[p_idx])
            break;
        curNode = curNode->ptr[p_idx];
        k_idx = p_idx = 0;
    }

    cout<<k_idx<<endl;
    curNode->key.insert(curNode->key.begin() + k_idx, key);
    this->nodeCnt++;
    return true;
}
void BpTree::printAll() {

}
/*
bool Bnode::preempt(pair<deque<int>, deque<Bnode*>> keyAndPtr)
{
    deque<int> keys = keyAndPtr.first;
    deque<Bnode*> ptrs = keyAndPtr.second;
    this->__curPtrPos = this->__curKeyPos = 0 ;
    memset(this->ptr, 0, sizeof(this->ptr));
    memset(this->key, 0, sizeof(this->key));
    
    for (this->__curKeyPos = 0 ; !keys.empty() ; this->__curKeyPos++) {
        this->key[__curKeyPos] = keys.front();
        keys.pop_front();
    }

    for (this->__curPtrPos = 0 ; !ptrs.empty() ; this->__curPtrPos++) {
        this->ptr[__curPtrPos] = ptrs.front();
        ptrs.pop_front();
    }
}
*/
#endif
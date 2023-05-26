//
// Created by wasim on 08/05/2023.
//

#ifndef DATA_STRUCTURE_WET_1_NODE_H
#define DATA_STRUCTURE_WET_1_NODE_H
#include <iostream>
#include <memory>
#include "Movie.h"


template <class Key, class Data>
class Node{
    Key key;
    std::shared_ptr<Data> data;
    Node<Key,Data>* left_node;
    Node<Key,Data>* right_node;
    Node<Key, Data>* father_node;
    int node_height;
    int balance_factor;

    public:


    Node(Key key, std::shared_ptr<Data> data);

    virtual ~Node() = default;

    Node(const Node& other) = delete;

    Node& operator=(const Node& other) = delete;

    Key get_key() const;

    Node<Key, Data>* get_left() const;

    Node<Key, Data>* get_right() const;

    Node<Key, Data>* get_father() const;

    Node<Key, Data>* get_left();

    Node<Key, Data>* get_right();

    Node<Key, Data>* get_father();

    std::shared_ptr<Data> get_data();

    int get_balance_factor() const;

    int get_height() const;

    void set_left(Node<Key, Data>* new_left);

    void set_right(Node<Key, Data>* new_right);

    void set_father(Node<Key, Data>* new_father);

    void set_key(Key new_key);

    void set_balance_factor();

    void set_height();

    void set_data(std::shared_ptr<Data> new_data);







};

template<class Key,class Data>
class Delete_Node{
    public:
    void operator()(Node<Key,Data>* to_delete){
        delete to_delete;
    }
};

template<class Key , class Data>
class DeleteEmptyNodes{
public:
    void operator()(Node<Key,Data>* to_delete){
        delete to_delete;
    }
};

template<class Key , class Data>
class getIdOfNode{
    int *const array;
    int index;

public:
     explicit getIdOfNode(int* const arr) : array(arr), index(0){}
    void operator()(Node<Key,Data>* nodeForId){
        *(array+index) = *(nodeForId->get_data());
        index++;
     }
};


template <class Key, class Data>
Node<Key,Data>::Node(Key key,std::shared_ptr<Data> data) : key(key), data(data) , left_node(nullptr)             ///////////////////////////////
                            , right_node(nullptr), father_node(nullptr) , node_height(0), balance_factor(0){}

template <class Key, class Data>
Key Node<Key,Data>::get_key() const{
    return key;
}

template<class Key, class Data>
bool operator==( const Node<Key, Data>& node1, const Node<Key, Data>& node2){
    return node1.get_key() == node2.get_key();
}


template <class Key, class Data>
bool operator<(const Node<Key, Data>& node1, const Node<Key, Data>& node2){

    return node1.get_key() < node2.get_key();
}

template <class Key, class Data>
bool operator>(const Node<Key, Data>& node1, const Node<Key, Data>& node2){
    return (!(node1 == node2) && !(node1 < node2));
}

template <class Key, class Data>
Node<Key, Data>* Node<Key,Data>::get_left() const{
    return left_node;
}

template <class Key, class Data>
Node<Key, Data>* Node<Key,Data>::get_right() const{
    return right_node;
}

template <class Key, class Data>
Node<Key, Data>* Node<Key,Data>::get_father() const{
    return father_node;
}

template <class Key, class Data>
Node<Key, Data>* Node<Key,Data>::get_left() {
    return left_node;
}

template <class Key, class Data>
Node<Key, Data>* Node<Key,Data>::get_right() {
    return right_node;
}

template <class Key, class Data>
Node<Key, Data>* Node<Key,Data>::get_father() {
    return father_node;
}


template <class Key, class Data>
void Node<Key,Data>::set_left(Node<Key, Data>* new_left) {
    left_node = new_left;
    if(left_node != nullptr) left_node->set_father(this);                                                                                          // ???
}

template <class Key, class Data>
void Node<Key,Data>::set_right(Node<Key, Data>* new_right) {
    right_node = new_right;
    if(right_node != nullptr) right_node->set_father(this);                                                                                          // ???
}

template <class Key, class Data>
void Node<Key,Data>::set_father(Node<Key, Data>* new_father) {
    father_node = new_father;
}

template <class Key, class Data>
std::shared_ptr<Data> Node<Key,Data>::get_data(){
        return data;
 }

template <class Key, class Data>
void Node<Key,Data>::set_balance_factor(){
    int left_height = (left_node == nullptr)? -1 : left_node->node_height;
    int right_height = (right_node == nullptr)? -1 : right_node->node_height;
    balance_factor = left_height - right_height ;
}


template <class Key, class Data>
void Node<Key,Data>::set_height(){
    int left_height = (left_node == nullptr)? -1 : left_node->node_height;
    int right_height = (right_node == nullptr)? -1 : right_node->node_height;
    node_height = (left_height > right_height) ? left_height+1 : right_height+1 ;
}


template <class Key, class Data>
void Node<Key,Data>::set_key(Key new_key){                                                                               //////////////
    key = new_key;
}

template <class Key, class Data>
int Node<Key,Data>::get_balance_factor() const{
    return balance_factor;
}

template <class Key, class Data>
int Node<Key,Data>::get_height() const{
    return node_height;
}

template <class Key, class Data>
void Node<Key,Data>::set_data(std::shared_ptr<Data> new_data){
    data = new_data;
}












#endif //DATA_STRUCTURE_WET_1_NODE_H

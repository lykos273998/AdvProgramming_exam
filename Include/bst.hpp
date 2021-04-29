#include <iostream>
#include <algorithm>
#include <vector>
#include <memory>
#include <utility>
#include <exception>

#define KEY KV.first
#define VAL KV.second

#define ENDL std::endl;

#include "bst_node.hpp"
#include "bst_iterator.hpp"

template <typename KEY_type, typename VAL_type, typename comparison_operator=std::less<KEY_type>>
class bst{

    comparison_operator op;
    using node_type = node<VAL_type,KEY_type>;

    public:
        std::unique_ptr<node_type> root;
        
};


    /*
    using node_type node<KEY_type,VAL_type>

    OP op;

    public:
    
    
    std::unique_ptr<node_type> root{nullptr};
    */

/*
    //constructor for empty bst
    bst(){};


    //copy constructor
    
    bst(const bst& bst_to_copy_from){
        delete_all_nodes(root);
        root = copy_all_nodes(bst_to_copy_from.root);
    }

    //copy assignment
    bst& operator=(const bst& bst_to_copy_from){
            return *(new bst(bst_to_copy_from));
            }

    //move ctor
    bst(bst&& bst_to_move){
        //steal root
        root = std::move(bst_to_move.root);
    };
    //move assignment
    bst& operator=(bst&& bst_to_move){
        //steal root
        root = bst_to_move.root;
        bst_to_move.root = nullptr;

    };
    //utility functions
    //recursively delete all the nodes
    ~bst(){
        delete_all_nodes(root);
    };

    //aux function to copy and delete nodes
    //exploit recursion
    void delete_all_nodes(node* current_node){
        if (current_node == nullptr)return;
        if (current_node -> LEFT_child != nullptr) delete_all_nodes(current_node -> LEFT_child );
        if (current_node -> RIGHT_child != nullptr) delete_all_nodes(current_node -> RIGHT_child);
        delete(current_node);
    }

    node* copy_all_nodes(node* current_node){
        auto copied_node = new node(*current_node);
        if (current_node -> LEFT_child != nullptr) { 
            //copy recursively child
            copied_node -> LEFT_child = copy_all_nodes(current_node -> LEFT_child);
            //set recursively the parenthood
            copied_node -> LEFT_child -> parent = copied_node;
            }
        if (current_node -> RIGHT_child != nullptr){
            copied_node -> RIGHT_child = copy_all_nodes(current_node -> RIGHT_child);
            copied_node -> RIGHT_child -> parent = copied_node;}
        return copied_node;
    }

    friend
    std::ostream& operator<<(std::ostream& os, const bst& x){
        for(auto n : x)
        {
            os << n.KV.first << "  " << n.KV.second << "\n";
        }
        os << std::endl;
        return os;
    };

    void print_extended(){
        std::cout << "BEGIN OF THE TREE" << std::endl;
        for(auto n = begin(); n != end(); ++n){
            n -> _print_extended();
        }
        std::cout << "--- END ---" << std::endl;
    }   */


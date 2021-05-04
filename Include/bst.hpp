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
    using node_type = node<const KEY_type,VAL_type>;
    using pair_type = std::pair<const KEY_type,VAL_type>;

    using const_iterator = Iterator<const pair_type,node_type>;
    using iterator = Iterator<pair_type,node_type>;
    
    public:

    std::unique_ptr<node_type> root;
    bst(){
        root.reset(nullptr);
    };

    bst(const bst& bst_to_copy_from){
        root.reset(copy_all_nodes(bst_to_copy_from.root.get()));
    }



    //copy assignment
    bst& operator=(const bst& bst_to_copy_from){
        return *(new bst(bst_to_copy_from));
    }



    
    //defautl move, not using raw pointers, so it is enough;
    bst(bst &&) = default;
    bst &operator=(bst &&) = default;

    //iterators

    iterator begin(){
        node_type *current = root.get();
        while(current -> get_left()){
            current = current -> get_left();
        }
        return iterator(current);
    }

    iterator end(){
        node_type *current = root.get();
        return iterator(nullptr);
    }

    const_iterator begin() const{
        
        node_type *current = root.get();
        while(current -> get_left()){
            current = current -> get_left();
        }
        return const_iterator(current);
    }

    const_iterator cbegin() const{
        
        node_type *current = root.get();
        while(current -> get_left()){
            current = current -> get_left();
        }
        return const_iterator(current);
    }

    const_iterator end() const{
        node_type *current = root.get();
        return const_iterator(nullptr);
    }

    const_iterator cend() const{
        node_type *current = root.get();
        return const_iterator(nullptr);
    }
    
    //insertion

    std::pair<iterator,bool> insert(const pair_type& pair_to_insert){
        auto node_to_insert = new node_type(pair_to_insert);

        if(root == nullptr){
            root.reset(node_to_insert);
            return std::pair<iterator,bool>(iterator(root.get()),true);
        }
        else{
            auto curr_node = root.get();
            while(true){
                if(node_to_insert -> get_key() > curr_node -> get_key()){
                    if(curr_node -> get_right() == nullptr){
                        curr_node -> set_right(node_to_insert);
                        return std::pair<iterator,bool>(curr_node -> get_right(),true);
                    }
                    curr_node = curr_node -> get_right();
                }
                else if(node_to_insert -> get_key() < curr_node -> get_key()){
                    if(curr_node -> get_left() == nullptr){
                        curr_node -> set_left(node_to_insert);
                        return std::pair<iterator,bool>(curr_node -> get_left(),true);
                    }
                    curr_node = curr_node -> get_left();
                }
                else{
                    return std::pair<iterator,bool>(curr_node,false);
                }

            }
            
        }
        
    }

    std::pair<iterator,bool> insert(pair_type&& pair_to_insert){
        auto node_to_insert = new node_type(std::move(pair_to_insert));

        if(root == nullptr){
            root.reset(node_to_insert);
            return std::pair<iterator,bool>(iterator(root.get()),true);
        }
        else{
            auto curr_node = root.get();
            while(true){
                if(node_to_insert -> get_key() > curr_node -> get_key()){
                    if(curr_node -> get_right() == nullptr){
                        curr_node -> set_right(node_to_insert);
                        return std::pair<iterator,bool>(curr_node -> get_right(),true);
                    }
                    curr_node = curr_node -> get_right();
                }
                else if(node_to_insert -> get_key() < curr_node -> get_key()){
                    if(curr_node -> get_left() == nullptr){
                        curr_node -> set_left(node_to_insert);
                        return std::pair<iterator,bool>(curr_node -> get_left(),true);
                    }
                    curr_node = curr_node -> get_left();
                }
                else{
                    return std::pair<iterator,bool>(curr_node,false);
                }

            }
            
        }
        
    }

    

    
    //aux functions
    

    node_type* copy_all_nodes(node_type* current_node){
        auto copied_node = new node_type(*current_node);
        if (current_node -> get_left() != nullptr) { 
            //copy recursively child
            copied_node -> set_left(copy_all_nodes(current_node -> get_left()));
            //set recursively the parenthood, parent is a raw pointer, only for auxiliary things
            }
        if (current_node -> get_right() != nullptr){
            copied_node -> set_right(copy_all_nodes(current_node -> get_right()));
            }
        return copied_node;
    }

    friend
    std::ostream& operator<<(std::ostream& os, const bst& x){
        for(auto n : x)
        {
            os << n.first << "  " << n.second << "\n";
        }
        os << std::endl;
        return os;
    }

    void print_extended(){
        std::cout << "BEGIN OF THE TREE" << std::endl;
        for(auto n = begin(); n != end(); ++n){
            std::cout << n -> first << " " << n -> second << std::endl;
        }
    }
    
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


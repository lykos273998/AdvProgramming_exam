#include <iostream>
#include <algorithm>
#include <vector>
#include <memory>
#include <utility>
#include <exception>

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

    std::unique_ptr<node_type> root{nullptr};
    bst() = default;

    bst(const bst& bst_to_copy_from){
        root.reset(copy_all_nodes(bst_to_copy_from.root.get()));
    }

    ~bst() noexcept = default;

    /*no conversion between type pair and bst*/

    /*
    explicit bst(const pair_type& p){
        root.reset(new node_type(p));
    }
*/

    explicit bst(pair_type&& p){
        root.reset(new node_type(p));
    }

    


    //copy assignment
    bst& operator=(const bst& bst_to_copy_from){
        root.reset();
        auto tmp = bst_to_copy_from;
        (*this) = std::move(tmp);
        return *this;
    }



    
    //defautl move, not using raw pointers, so it is enough;
    bst(bst&& x) noexcept  = default;
    bst &operator=(bst&& x) noexcept = default;

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
    
    //clearing all the tree
    void clear(){
        root.reset(nullptr);
    }
    
    //insertion
    std::pair<iterator,bool> _insert_node(node_type* node_to_insert);
         

    std::pair<iterator,bool> insert(const pair_type& pair_to_insert){
        auto node_to_insert = new node_type(pair_to_insert);
        return this -> _insert_node(node_to_insert);
        
    }

    std::pair<iterator,bool> insert(pair_type&& pair_to_insert){
        auto node_to_insert = new node_type(std::move(pair_to_insert));
        return this -> _insert_node(node_to_insert);
        
    }



    //finding
    node_type* _find(const KEY_type& KEY);

    iterator find(const KEY_type& x){
        return iterator(_find(x));
    }
    const_iterator find(const KEY_type& x) const{
        return const_iterator(_find(x));
    }

    //aux functions
    

    node_type* copy_all_nodes(node_type* current_node);

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

template <typename KEY_type, typename VAL_type, typename comparison_operator>
std::pair<typename bst<KEY_type,VAL_type,comparison_operator>::iterator,bool> 
        bst<KEY_type,VAL_type,comparison_operator>::_insert_node(bst<KEY_type,VAL_type,comparison_operator>::node_type* node_to_insert)
        {
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

    template <typename KEY_type, typename VAL_type, typename comparison_operator>
    typename bst<KEY_type,VAL_type,comparison_operator>::node_type* bst<KEY_type,VAL_type,comparison_operator>::_find (const KEY_type& KEY){
        auto current = root.get();
        while(current){
            if(KEY == current -> get_key()){
                return current;
            }
            else if (op(KEY,current -> get_key())){
                if(current -> get_left()){
                    current = current -> get_left();
                }
                else{
                    return nullptr;
                }
            }
            else{
                if(current -> get_right()){
                    current = current -> get_right();
                }
                else{
                    return nullptr;
                }
            }
            
        } 
        return current;
    }
    
    template <typename KEY_type, typename VAL_type, typename comparison_operator>
    typename bst<KEY_type,VAL_type,comparison_operator>::node_type* 
    bst<KEY_type,VAL_type,comparison_operator>::copy_all_nodes(bst<KEY_type,VAL_type,comparison_operator>::node_type* current_node){
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

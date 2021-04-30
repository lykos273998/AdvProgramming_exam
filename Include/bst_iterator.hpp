#include <iostream>
#include <algorithm>
#include <vector>
#include <memory>
#include <utility>
#include <exception>

//note only to "easy to use" members of nodes

#define KEY KV.first
#define VAL KV.second

#define ENDL std::endl;


template <typename pair_type, typename node_type>
struct Iterator{
    
        using value_type = pair_type;
        using difference_type = std::ptrdiff_t;
        using iterator_category = std::forward_iterator_tag;
        using reference = value_type&;
        using pointer = value_type*;

        node_type* current;

        reference operator*() const{ return  current -> get_pair();}
        pointer operator->(){ return &**this;}

        Iterator() = default;
        Iterator(node_type* init_node) : current{init_node} {};

        ~Iterator() = default;

        Iterator& operator++(){
            current = next();
            return *this;
        }

        friend bool operator== (const Iterator& a, const Iterator& b) { return a.current == b.current; };
        friend bool operator!= (const Iterator& a, const Iterator& b) { return a.current != b.current; };  
        
        node_type* next(){
            node_type* aux_node_pointer = current;
            // std::cout << current -> RIGHT_child << std::endl;

            /*
            * This part is a little bit difficult to explain
            * The idea is that starting from a general node its successor will be on the right
            * This opens 2 cases: 
            *     i) The successor is the "Left-most" node on the right branch, first case of the if
            *     ii) The successor is one of the nodes in the "parenthood" case 2 of the if
            * In the second case we have to check if node.key > node.parent, eventually we will 
            * reach the root, if even up to the root this condition is fullfilled then we have the last node
            * of the sequence ("the right-most") so we will return as his successor nullptr
            * 
            * Now by following the property of the bst we know that the next node 
            * will be the left most in the parents of the node
            * so we climb up the tree since the next node will have as left child the current one
            * 
            * 
            */

            if(current -> get_right() != nullptr){
                aux_node_pointer = current -> get_right();
                while(aux_node_pointer -> get_left() != nullptr )
                     aux_node_pointer = aux_node_pointer -> get_left();
                return aux_node_pointer;
            }
            else{
                aux_node_pointer = current -> get_parent();

                while(aux_node_pointer != nullptr) {
                    if(aux_node_pointer -> get_left() == current){
                        break;
                    }
                    else{
                        current = aux_node_pointer;
                        aux_node_pointer = aux_node_pointer -> get_parent();
                    }
                }
                
                return aux_node_pointer;
            }
            
            
            }
        
    };

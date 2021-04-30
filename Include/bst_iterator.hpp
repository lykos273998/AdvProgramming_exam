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


template <typename val, typename node_type>
struct iterator{
        using value_type = node_type;
        using difference_type = std::ptrdiff_t;
        using iterator_category = std::forward_iterator_tag;
        using reference = value_type&;
        using pointer = value_type*;

        pointer current;
        reference operator*(){ return  *current;}
        pointer operator->(){ return &**this;}

        iterator();
        ~iterator();

        iterator& operator++(){
            current = next();
            return *this;
        }

        friend bool operator== (const iterator& a, const iterator& b) { return a.current == b.current; };
        friend bool operator!= (const iterator& a, const iterator& b) { return a.current != b.current; };  
        
        pointer next(){
            pointer aux_node_pointer = current;
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

            if(current -> get_left() != nullptr){
                aux_node_pointer = current -> get_right();
                while(aux_node_pointer -> get_left() != nullptr )
                     aux_node_pointer = aux_node_pointer -> get_left();
                return aux_node_pointer;
            }
            else{
                aux_node_pointer = current -> get_parent();
                while(aux_node_pointer -> get_left() != current) {
                    current = aux_node_pointer;
                    aux_node_pointer = aux_node_pointer -> get_parent();
                   // if (aux_node_pointer == nullptr){ return nullptr;}
                                    }
                
                return aux_node_pointer;
            }
            
            }
        
    };

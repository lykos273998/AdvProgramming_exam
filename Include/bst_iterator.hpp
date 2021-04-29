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


template <typename node_type, typename comparison_operator>
struct iterator{
        using value_type = node_type;
        using difference_type = std::ptrdiff_t;
        using iterator_category = std::forward_iterator_tag;
        using reference = value_type&;
        using pointer = value_type*;

        pointer current;

        comparison_operator op;
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
            */
            if(current -> RIGHT_child.get() != nullptr){
                aux_node_pointer = current -> RIGHT_child.get();
                while(aux_node_pointer -> LEFT_child.get() != nullptr )
                     aux_node_pointer = aux_node_pointer -> LEFT_child.get();
                return aux_node_pointer;
            }
            else{
                
                aux_node_pointer = aux_node_pointer -> parent;
                while(op(aux_node_pointer -> KEY, current -> KEY) ){
                    aux_node_pointer = aux_node_pointer -> parent;
                    if (aux_node_pointer == nullptr){ return nullptr;}
                }
                return aux_node_pointer;
            }
            
            }
        
    };

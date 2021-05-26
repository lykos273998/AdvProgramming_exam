#include <iostream>
#include <algorithm>
#include <vector>
#include <memory>
#include <utility>
#include <exception>

//note only to "easy to use" members of nodes


#define ENDL std::endl;


template <typename pair_type, typename node_type>
struct Iterator{
    
        using value_type = pair_type;
        using difference_type = std::ptrdiff_t;
        using iterator_category = std::forward_iterator_tag;
        using reference = value_type&;
        using pointer = value_type*;

        node_type* current;

        /**pointer dereferencing operator, returns
         * the key-value pair of the current node returns*/
        reference operator*() const{ return  current -> get_pair();}
        /**pointer arrow operator, returns
         * the key-value pair of the current node returns*/
        pointer operator->(){ return &(*(*this));}

        /**default constructor*/
        Iterator() = default;
        /**construcotr using a node pointer*/
        explicit Iterator(node_type* init_node) noexcept : current{init_node} {};
        /**default destructor*/
        ~Iterator() = default;

        /** ++ operator overloading, relies on next() method */
        Iterator& operator++(){
            current = next();
            return *this;
        }

        /**overloading of == operator*/
        friend bool operator== (const Iterator& a, const Iterator& b) { return a.current == b.current; };
        /**overloading of != operator*/
        friend bool operator!= (const Iterator& a, const Iterator& b) { return a.current != b.current; };  
        
        /**
         * next() method, returns a pointer to the next node on the iteration
         * */
        node_type* next() const noexcept {
            node_type* current_node = current;
            node_type* aux_node = current;
            // std::cout << current -> RIGHT_child << std::endl;

            /*
            * This part is a little bit difficult to explain
            * The idea is that starting from a general node its successor will be on the right
            * This opens 2 cases: 
            *     i) The successor is the "Left-most" node on the right branch, first case of the if
            *     ii) The successor is one of the nodes in the "parenthood", second case of the if
            * For the second case 
            * following the property of the bst we know that the next node 
            * will be the left most in the parents of the node, namely, the next node must be the first parent
            * which is on the right of the node we start from.
            * So we climb up the tree since the next node will have as left child the current one, 
            * so by doing that we have found the first node "on the right of the starting point" and we stop
            * 
            * 
            */

            if(current_node -> get_right() != nullptr){
                aux_node = current_node -> get_right();
                while(aux_node -> get_left() != nullptr )
                     aux_node = aux_node -> get_left();
                return aux_node;
            }
            else{
                aux_node = current_node -> get_parent();

                while(aux_node != nullptr) {
                    if(aux_node -> get_left() == current_node){
                        break;
                    }
                    else{
                        current_node = aux_node;
                        aux_node = aux_node -> get_parent();
                    }
                }
                
                return aux_node;
            }
            
            
            }
        
    };

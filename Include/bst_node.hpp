#include <iostream>
#include <algorithm>
#include <vector>
#include <memory>
#include <utility>
#include <exception>



template <typename KEY_type, typename VAL_type>
struct node
    {
        /*
        * struct node that contains a pair - Key value
        * pointer to left, right child as unique_ptr and parent node as raw ptr
        *    
        */
        using pair_type = std::pair<KEY_type,VAL_type>;
        pair_type KV;
        std::unique_ptr<node> LEFT_child{nullptr};
        std::unique_ptr<node> RIGHT_child{nullptr};
        node* parent{nullptr};
        
        /**node constructor from a l-value reference to a type pair*/
        explicit node(const pair_type& KV_init): KV{KV_init} {
        };

        /**node constructor from a r-value reference to a type pair*/
        explicit node(pair_type&& KV_init): KV{std::move(KV_init)} {
           // std::cout << "r-val node ctor" << std::endl;
        };

        /**
        * copy constructor using a pointer to a node as input
        * deep copy of the tree is performed at this level
        * each node once copied, copies all its childs recursively
        * So by invoking the cop of the root the whole tree is copied
        */
        node(node* node_to_copy_from) : KV{node_to_copy_from -> KV}{
            if(node_to_copy_from -> get_left()){
                set_left(new node(node_to_copy_from -> get_left()));
            }
            if(node_to_copy_from -> get_right()){
                set_right(new node(node_to_copy_from -> get_right()));
            }
        }

        /**
        * copy constructor using a const node refernce as input
        * deep copy of the tree is performed at this level
        * each node once copied, copies all its childs recursively
        * So by invoking the cop of the root the whole tree is copied
        */
        node(const node &node_to_copy_from) : node{&node_to_copy_from}  {     
            }
        /**
         * copy assignment, uses copy constructor
         * */
        node& operator=(const node& node_to_copy_from){
            LEFT_child.reset();
            RIGHT_child.reset();
            auto tmp{node_to_copy_from};
            *this = std::move(tmp);
            return *this;
            }
        /**default destuctor*/
        ~node() noexcept = default;

        /**default move constructor*/
        node(node &&) noexcept = default;
        /**default move assignment*/
        node &operator=(node &&) noexcept = default;
        

        /**returns pointer to parent of the node*/
        node* get_parent(){
            return parent;
        }
        /**sets parent of the node*/
        void set_parent(node* other_node){
            parent = other_node;
        }

        /**returns the KEY of the node*/
        KEY_type get_key(){
            return KV.first;
        }
        /**returns the value of the node*/
        VAL_type get_val(){
            return KV.second;
        }
        /**returns the pair of the node*/
        pair_type& get_pair(){
            return KV;
        }

        /**returns a raw ptr to the left child*/
        node* get_left(){
            return LEFT_child.get();
        }
        /**sets the left child unique_ptr, taking as input a raw ptr
         * set as partent of the other_node the actual one
        */
        void set_left(node* other_node){
            LEFT_child.reset(other_node);
            other_node -> set_parent(this);
        }
        /**returns a raw ptr to the right child*/
        node* get_right(){
            return RIGHT_child.get();
        }

        /**sets the rightchild unique_ptr, taking as input a raw ptr
         * set as partent of the other_node the actual one
        */
        void set_right(node* other_node){
            RIGHT_child.reset(other_node);
            other_node -> set_parent(this);
        }

        
        };
        
    

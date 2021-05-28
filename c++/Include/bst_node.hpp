#include <iostream>
#include <algorithm>
#include <vector>
#include <memory>
#include <utility>
#include <exception>



template <typename KEY_type, typename VAL_type>
struct Node
    {
        /*
        * struct Node that contains a pair - Key value
        * pointer to left, right child as unique_ptr and parent Node as raw ptr
        *    
        */
        using pair_type = std::pair<KEY_type,VAL_type>;
        pair_type KV;
        std::unique_ptr<Node> LEFT_child; 
        std::unique_ptr<Node> RIGHT_child;
        Node* parent;
        
        Node(){};
        /**Node constructor from a l-value reference to a type pair*/
        explicit Node(const pair_type& KV_init) : KV{KV_init}, LEFT_child{nullptr}, RIGHT_child{nullptr}, parent{nullptr}{};
 
        /**Node constructor from a r-value reference to a type pair*/
        explicit Node(pair_type&& KV_init) noexcept : KV{std::move(KV_init)}, LEFT_child{nullptr}, RIGHT_child{nullptr}, parent{nullptr} {};

        /**
        * copy constructor using a pointer to a Node as input
        * deep copy of the tree is performed at this level
        * each Node once copied, copies all its childs recursively
        * So by invoking the cop of the root the whole tree is copied
        */
        explicit Node(const Node* node_to_copy_from) : KV{node_to_copy_from -> KV}{
            if(node_to_copy_from -> get_left()){
                set_left(new Node(node_to_copy_from -> get_left()));
            }
            if(node_to_copy_from -> get_right()){
                set_right(new Node(node_to_copy_from -> get_right()));
            }
        }

        /**
        * copy constructor using a const Node refernce as input
        * deep copy of the tree is performed at this level
        * each Node once copied, copies all its childs recursively
        * So by invoking the cop of the root the whole tree is copied
        */
        explicit Node(const Node &node_to_copy_from) : Node{&node_to_copy_from}  {     
           
            }

        /**
         * copy assignment, uses copy constructor
         * */
        Node& operator=(const Node& node_to_copy_from){
            LEFT_child.reset();
            RIGHT_child.reset();
            auto tmp{node_to_copy_from};
            *this = std::move(tmp);
            return *this;
            }
        
        /**default destuctor*/
        ~Node() noexcept = default;

        /**default move constructor*/
        Node(Node &&) noexcept = default;
        /**default move assignment*/
        Node &operator=(Node &&) noexcept = default;
        

        /**returns pointer to parent of the Node*/
        Node* get_parent() const noexcept{
            return parent;
        }
        /**sets parent of the Node*/
        void set_parent(Node* other_node) noexcept{
            parent = other_node;
        }

        /**returns the KEY of the Node*/
        KEY_type get_key() const noexcept{
            return KV.first;
        }
        /**returns the value of the Node*/
        VAL_type get_val() const noexcept{
            return KV.second;
        }
        /**returns the pair of the Node*/
        pair_type& get_pair() noexcept{
            return KV;
        }

        /**returns a raw ptr to the left child*/
        Node* get_left() const noexcept {
            return LEFT_child.get();
        }
        /**sets the left child unique_ptr, taking as input a raw ptr
         * set as partent of the other_node the actual one
        */
        void set_left(Node* other_node) noexcept{
            LEFT_child.reset(other_node);
            other_node -> set_parent(this);
        }
        /**returns a raw ptr to the right child*/
        Node* get_right() const noexcept{
            return RIGHT_child.get();
        }

        /**sets the rightchild unique_ptr, taking as input a raw ptr
         * set as partent of the other_node the actual one
        */
        void set_right(Node* other_node) noexcept{
            RIGHT_child.reset(other_node);
            other_node -> set_parent(this);
        }

        };
        
    

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
        * pointer to left, right and parent nodes
        * constructors defined       
        */
        using pair_type = std::pair<KEY_type,VAL_type>;
        pair_type KV;
        std::unique_ptr<node> LEFT_child{nullptr};
        std::unique_ptr<node> RIGHT_child{nullptr};
        node* parent{nullptr};
        
        /*we do not want an implicit conversion between pair and node */
        explicit node(const pair_type& KV_init): KV{KV_init} {
        };

        explicit node(pair_type&& KV_init): KV{std::move(KV_init)} {
           // std::cout << "r-val node ctor" << std::endl;
        };

        
        node(node* node_to_copy_from) : KV{node_to_copy_from -> KV}{
            if(node_to_copy_from -> get_left()){
                set_left(new node(node_to_copy_from -> get_left()));
            }
            if(node_to_copy_from -> get_right()){
                set_right(new node(node_to_copy_from -> get_right()));
            }
        }


        node(const node &node_to_copy_from) : node{&node_to_copy_from}  {     
            }

        node& operator=(const node& node_to_copy_from){
            return *(new node(node_to_copy_from));
            }
        
        ~node(){};

        node(node &&) = default;
        node &operator=(node &&) = default;
        


        node* get_parent(){
            return parent;
        }

        KEY_type get_key(){
            return KV.first;
        }

        pair_type& get_pair(){
            return KV;
        }

        void set_parent(node* other_node){
            parent = other_node;
        }
        

        node* get_left(){
            return LEFT_child.get();
        }

        void set_left(node* other_node){
            LEFT_child.reset(other_node);
            other_node -> set_parent(this);
        }

        node* get_right(){
            return RIGHT_child.get();
        }

        void set_right(node* other_node){
            RIGHT_child.reset(other_node);
            other_node -> set_parent(this);
        }

        
        };
        
    

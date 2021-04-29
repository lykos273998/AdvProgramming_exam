#include <iostream>
#include <algorithm>
#include <vector>
#include <memory>
#include <utility>
#include <exception>
#define KEY KV.first
#define VAL KV.second


template <typename VAL_type, typename KEY_type>
struct node
    {
        /*
        * struct node that contains a pair - Key value
        * pointer to left, right and parent nodes
        * constructors defined       
        */
        std::pair<KEY_type,VAL_type> KV;
        std::unique_ptr<node> LEFT_child;
        std::unique_ptr<node> RIGHT_child;
        node* parent;
        
        node(const std::pair<KEY_type,VAL_type> KV_init){
            KV = KV_init;
            this->LEFT_child = nullptr;
            this->RIGHT_child = nullptr;
            this->parent = nullptr;
        };
        node(const node &node_to_copy_from) {
            LEFT_child = nullptr;
            RIGHT_child = nullptr;
            parent = nullptr;
            KV = node_to_copy_from.KV;
        }
        node& operator=(const node& node_to_copy_from){
            return *(new node(node_to_copy_from));
            }

        void _print_extended(){
            std::cout << "Node " << this << "\tLCH " << LEFT_child << "\tRCH " << RIGHT_child << "\tKV " << KEY << "  " << VAL << std::endl;
        }

        ~node(){
            
           // std::cout << "invoking destructor on node w key:" << KEY << std::endl;
        }
        
    };

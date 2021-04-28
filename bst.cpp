#include <iostream>
#include <iterator>
#include <algorithm>
#include <utility>
#include <stack>

#define ENDL std::endl;

template <typename Vtype, typename Ktype, typename OP = std::less<Ktype> >
class bst
{   
    public:
    OP op;

    struct node
    {
        
        Vtype value;
        Ktype key;
        node* LEFT_child;
        node* RIGHT_child;
        node* parent;
        
        node(Vtype v, Ktype k) : key(k), value(v) {
            this->LEFT_child = nullptr;
            this->RIGHT_child = nullptr;
            this->parent = nullptr;
        };
        ~node(){
            if (LEFT_child != nullptr) delete(LEFT_child);
            if (RIGHT_child != nullptr) delete(RIGHT_child);
            std::cout << "invoking destructor on node w key:" << key << std::endl;
        }
        node(const node &node_to_copy_from) {
            
            LEFT_child = node_to_copy_from.LEFT_child;
            RIGHT_child = node_to_copy_from.RIGHT_child;
        }
    };

    struct Iterator{
        using operator_type = OP;
        using value_type = node;
        using difference_type = std::ptrdiff_t;
        using iterator_category = std::forward_iterator_tag;
        using reference = value_type&;
        using pointer = value_type*;

        pointer current;

        OP op;
        reference operator*(){ return  *current;}
        pointer operator->(){ return &**this;}

        Iterator(pointer init){
            current = init;
        }

        Iterator& operator++(){
            current = next();
            return *this;
        }

        friend bool operator== (const Iterator& a, const Iterator& b) { return a.current == b.current; };
        friend bool operator!= (const Iterator& a, const Iterator& b) { return a.current != b.current; };  
        
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
            if(current -> RIGHT_child != nullptr){
                aux_node_pointer = current -> RIGHT_child;
                while(aux_node_pointer -> LEFT_child != nullptr )
                     aux_node_pointer = aux_node_pointer -> LEFT_child;
                return aux_node_pointer;
            }
            else{
                
                aux_node_pointer = aux_node_pointer -> parent;
                while(op(aux_node_pointer -> key, current -> key) ){
                    aux_node_pointer = aux_node_pointer -> parent;
                    if (aux_node_pointer == nullptr){ return nullptr;}
                }
                return aux_node_pointer;
            }
            
            }
        
    };
    
    node* root;
    
    Iterator begin(){
        node* aux_node_pointer{root -> LEFT_child};
        //go left until reach end
        while (aux_node_pointer -> LEFT_child != nullptr){
            aux_node_pointer = aux_node_pointer -> LEFT_child;
        }
        return Iterator(aux_node_pointer);
    }
    Iterator end(){
        //one past the last element is a null pointer
        return Iterator(nullptr);
    }

    //constructor for building bst starting from a node 
    bst(node* root_node){
        this->root = root_node;
    }
    //constructor for empty bst
    bst(){};

    ~bst(){
        delete(root);
    };
    class IT{};
    
};



int main(int argc, char** argv){
    auto n8 = new bst<int,int>::node(0,8);
    auto n3 = new bst<int,int>::node(0,3);
    auto n1 = new bst<int,int>::node(0,1);
    auto n6 = new bst<int,int>::node(0,6);
    auto n4 = new bst<int,int>::node(0,4);
    auto n7 = new bst<int,int>::node(0,7);
    auto n10 = new bst<int,int>::node(0,10);
    auto n14 = new bst<int,int>::node(0,14);
    auto n13 = new bst<int,int>::node(0,13);

    n8 -> LEFT_child = n3; 
    n3 -> parent = n8;

    n3 -> LEFT_child = n1;
    n1 -> parent = n3;

    n3 -> RIGHT_child = n6;
    n6 -> parent = n3;

    n6 -> LEFT_child = n4;
    n4 -> parent = n6;

    n8 -> RIGHT_child = n10;
    n10 -> parent = n8;

    n10 -> RIGHT_child = n14;
    n14 -> parent = n10;

    n14 -> LEFT_child = n13;
    n13 -> parent = n14;

    auto bb = *(new bst<int,int>(n8));
    auto gg = bb.begin();
    
    for(gg; gg != bb.end(); ++gg){
        
        std::cout << gg -> key << std::endl;
        
    }

    return 0;
}

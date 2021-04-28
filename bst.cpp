#include <iostream>
#include <iterator>
#include <utility>

//note only to "easy to use" members of nodes

#define KEY KV.first
#define VAL KV.second

#define ENDL std::endl;

template <typename Vtype, typename Ktype, typename OP = std::less<Ktype> >
class bst
{   
    public:
    OP op;
    
    //node fwd declaration
    struct node;
    
    //iterator fwd declaration
    struct iterator;
    struct const_iterator;
    node* root;
    
    
    //begin and end iterators
    iterator begin() {
        node* aux_node_pointer{root -> LEFT_child};
        //go left until reach end
        while (aux_node_pointer -> LEFT_child != nullptr){
            aux_node_pointer = aux_node_pointer -> LEFT_child;
        }
        return iterator(aux_node_pointer);
    }

    const_iterator begin() const{
        node* aux_node_pointer{root -> LEFT_child};
        //go left until reach end
        while (aux_node_pointer -> LEFT_child != nullptr){
            aux_node_pointer = aux_node_pointer -> LEFT_child;
        }
        return const_iterator(aux_node_pointer);
    }
    
    const_iterator cbegin() const{
        node* aux_node_pointer{root -> LEFT_child};
        //go left until reach end
        while (aux_node_pointer -> LEFT_child != nullptr){
            aux_node_pointer = aux_node_pointer -> LEFT_child;
        }
        return const_iterator(aux_node_pointer);
    }

    iterator end() {
        //one past the last element is a null pointer
        return iterator(nullptr);
    }

    const_iterator end() const{
        //one past the last element is a null pointer
        return const_iterator(nullptr);
    }

    const_iterator cend() const{
        //one past the last element is a null pointer
        return const_iterator(nullptr);
    }

   


    //constructor for empty bst
    bst(){};
    bst(node* new_node){
        root = new_node;
    };

    //copy constructor
    
    bst(const bst& bst_to_copy_from){
        root = copy_all_nodes(bst_to_copy_from.root);
    }

    //copy assignment
    bst& operator=(const bst& bst_to_copy_from){
            return *(new bst(bst_to_copy_from));
            }


    //recursively delete all the nodes
    ~bst(){
        delete_all_nodes(root);
    };

    //aux function to copy and delete nodes
    //exploit recursion
    void delete_all_nodes(node* current_node){
        if (current_node -> LEFT_child != nullptr) delete_all_nodes(current_node -> LEFT_child );
        if (current_node -> RIGHT_child != nullptr) delete_all_nodes(current_node -> RIGHT_child);
        delete(current_node);
    }

    node* copy_all_nodes(node* current_node){
        auto copied_node = new node(*current_node);
        if (current_node -> LEFT_child != nullptr) copied_node -> LEFT_child = copy_all_nodes(current_node -> LEFT_child);
        if (current_node -> RIGHT_child != nullptr) copied_node -> RIGHT_child = copy_all_nodes(current_node -> RIGHT_child);
        return copied_node;
    }

    void print(){
        for(auto n = begin(); n != end(); ++n){
            std::cout << n -> KEY << "  " << n -> VAL << std::endl;
        }

    }

    friend
    std::ostream& operator<<(std::ostream& os, const bst& x){
        for(auto n : x)
        {
            os << n.KV.first << "  " << n.KV.second << "\n";
        }
        os << std::endl;
        return os;
    };

};

//node implementation
template <typename Vtype, typename Ktype, typename OP >
struct bst<Vtype,Ktype,OP>::node
    {
        /*
        * struct node that contains a pair - Key value
        * pointer to left, right and parent nodes
        * constructors defined       
        */
        std::pair<Ktype,Vtype> KV;
        node* LEFT_child;
        node* RIGHT_child;
        node* parent;
        
        node(const Ktype k, const Vtype v){
            KV.first = k;
            KV.second = v;
            this->LEFT_child = nullptr;
            this->RIGHT_child = nullptr;
            this->parent = nullptr;
        };

        node(const std::pair<Ktype,Vtype> KV_init){
            KV = KV.init;
            this->LEFT_child = nullptr;
            this->RIGHT_child = nullptr;
            this->parent = nullptr;
        };
        node(const node &node_to_copy_from) {
            LEFT_child = node_to_copy_from.LEFT_child;
            RIGHT_child = node_to_copy_from.RIGHT_child;
            KV = node_to_copy_from.KV;
        }
        node& operator=(const node& node_to_copy_from){
            return *(new node(node_to_copy_from));
            }


        ~node(){
            
           // std::cout << "invoking destructor on node w key:" << KEY << std::endl;
        }
        
    };

//iterator implementation
template <typename Vtype, typename Ktype, typename OP >
struct bst<Vtype,Ktype,OP>::iterator{
        

        using value_type = bst<Vtype,Ktype,OP>::node;
        using difference_type = std::ptrdiff_t;
        using iterator_category = std::forward_iterator_tag;
        using reference = value_type&;
        using pointer = value_type*;

        pointer current;

        OP op;
        reference operator*(){ return  *current;}
        pointer operator->(){ return &**this;}

        iterator(pointer init){
            current = init;
        }

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
            if(current -> RIGHT_child != nullptr){
                aux_node_pointer = current -> RIGHT_child;
                while(aux_node_pointer -> LEFT_child != nullptr )
                     aux_node_pointer = aux_node_pointer -> LEFT_child;
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

//const iterator
template <typename Vtype, typename Ktype, typename OP >
struct bst<Vtype,Ktype,OP>::const_iterator{
        

        using value_type = bst<Vtype,Ktype,OP>::node;
        using difference_type = std::ptrdiff_t;
        using iterator_category = std::forward_iterator_tag;
        using reference = const value_type&;
        using pointer = const value_type*;

        pointer current;

        OP op;
        reference operator*(){ return  *current;}
        pointer operator->(){ return &**this;}

        const_iterator(pointer init) : current(init) {}

        const_iterator& operator++(){
            current = next();
            return *this;
        }

        friend bool operator== (const const_iterator& a, const const_iterator& b) { return a.current == b.current; };
        friend bool operator!= (const const_iterator& a, const const_iterator& b) { return a.current != b.current; };  
        
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
                while(op(aux_node_pointer -> KEY, current -> KEY) ){
                    aux_node_pointer = aux_node_pointer -> parent;
                    if (aux_node_pointer == nullptr){ return nullptr;}
                }
                return aux_node_pointer;
            }
            
            }
        
    };


int main(int argc, char** argv){
    auto n8 = new bst<int,int>::node(8,0);
    auto n3 = new bst<int,int>::node(3,0);
    auto n1 = new bst<int,int>::node(1,0);
    auto n6 = new bst<int,int>::node(6,0);
    auto n4 = new bst<int,int>::node(4,0);
    auto n7 = new bst<int,int>::node(7,0);
    auto n10 = new bst<int,int>::node(10,0);
    auto n14 = new bst<int,int>::node(14,0);
    auto n13 = new bst<int,int>::node(13,0);

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

    auto bb = new bst<int,int>(n8) ;
    auto bb2 = bb;
    //bb -> root = n8;
   // auto gg = bb.begin();
   
    //bb -> print();
    // bb2 -> print();
    std::cout << *bb << std::endl;
    //ffff
    //fffff
    return 0;
}

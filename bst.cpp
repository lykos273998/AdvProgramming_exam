#include <iostream>

#define ENDL std::endl;


template <typename Vtype, typename Ktype>
class node
{
    public:
        const Vtype value;
        const Ktype key;
        node* LEFT_child;
        node* RIGHT_child;
    
    node(Vtype v, Ktype k) : key(k), value(v) {
        this->LEFT_child = nullptr;
        this->RIGHT_child = nullptr;
    };
    ~node(){
        if (LEFT_child != nullptr) delete(LEFT_child);
        if (RIGHT_child != nullptr) delete(RIGHT_child);
    }
    node(const node &node_to_copy_from) {
        
        LEFT_child = node_to_copy_from.LEFT_child;
        RIGHT_child = node_to_copy_from.RIGHT_child;
    }
};

int main(int argc, char** argv){
    auto n  = new node<int,int>(10, 10);
    auto n2 = new node<int,int>(14, 10);
    n->LEFT_child = n2;
    delete(n);
    
    n = new node<int,int>(12, 10);
    std::cout << n->key << ENDL;
    return 0;
}

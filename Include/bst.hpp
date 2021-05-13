#include <iostream>
#include <algorithm>
#include <vector>
#include <memory>
#include <utility>
#include <exception>

#define COUNT 10 

#include "bst_node.hpp"
#include "bst_iterator.hpp"

template <typename KEY_type, typename VAL_type, typename comparison_operator=std::less<KEY_type>>
class bst{

    comparison_operator op;
    using node_type = node<const KEY_type,VAL_type>;
    using pair_type = std::pair<const KEY_type,VAL_type>;

    using const_iterator = Iterator<const pair_type,node_type>;
    using iterator = Iterator<pair_type,node_type>;
    


    public:

    std::unique_ptr<node_type> root{nullptr};
    bst() = default;

    bst(const bst& bst_to_copy_from){
        /*copy only the roots, then the full deep copy is performed at node level*/
        root.reset(new node_type(bst_to_copy_from.root.get()));
    }

    ~bst() noexcept = default;

    /*no conversion between type pair and bst*/

    
    explicit bst(const pair_type& p){
        root.reset(new node_type(p));
    }


    explicit bst(pair_type&& p){
        root.reset(new node_type(std::move(p)));
    }

    


    //copy assignment
    bst& operator=(const bst& bst_to_copy_from){
        root.reset();
        auto tmp = bst_to_copy_from;
        (*this) = std::move(tmp);
        return *this;
    }



    
    //defautl move, not using raw pointers, so it is enough;
    bst(bst&& x) noexcept  = default;
    bst &operator=(bst&& x) noexcept = default;

    //iterators

    iterator begin(){
        node_type *current = root.get();
        while(current -> get_left()){
            current = current -> get_left();
        }
        return iterator(current);
    }

    iterator end(){
        node_type *current = root.get();
        return iterator(nullptr);
    }

    const_iterator begin() const{
        node_type *current = root.get();
        while(current -> get_left()){
            current = current -> get_left();
        }
        return const_iterator(current);
    }

    const_iterator cbegin() const{
        node_type *current = root.get();
        while(current -> get_left()){
            current = current -> get_left();
        }
        return const_iterator(current);
    }

    const_iterator end() const{
        node_type *current = root.get();
        return const_iterator(nullptr);
    }

    const_iterator cend() const{
        node_type *current = root.get();
        return const_iterator(nullptr);
    }
    
    //clearing all the tree
    void clear(){
        root.reset();
    }
    
         
    //insertion
    std::pair<iterator,bool> insert(const pair_type& pair_to_insert){
        return this -> _insert_node(pair_to_insert);
        
    }

    std::pair<iterator,bool> insert(pair_type&& pair_to_insert){
        return this -> _insert_node(std::move(pair_to_insert));
        
    }
    
    //emplace
    template< class... Types >
    std::pair<iterator,bool> emplace(Types&&... args) {
        return insert(pair_type(std::forward<Types>(args)...));
    }
   

    //finding
    

    iterator find(const KEY_type& x){
        return iterator(_find(x));
    }
    const_iterator find(const KEY_type& x) const{
        return const_iterator(_find(x));
    }

    //subscripting
    VAL_type& operator[](const KEY_type& x){
        iterator pos = find(x);
        if(pos.current){
            return (*pos).second;
        }
        /*if nullptr insert the key and leave the value type uninitialized
        extract directly the iterator to the node
        */ 
        /*
        set the second as the rhs, actually works if the return is what I want to set
        note! 
        If I call 
        std::cout << tree[key_not_present] << ...
        an insertion is performed without initializing anything, so a new node is created
        */
        auto new_pair_it = insert(pair_type{x, {} }).first;
        return new_pair_it -> second;
    }

    VAL_type& operator[](KEY_type&& x){
        
        iterator pos = find(x);
        if(pos.current){
            return pos -> second;
        }
        /*if nullptr insert the key and leave the value type uninitialized, as before
        */
        auto new_pair_it = insert(pair_type{std::move(x), {} }).first;
        return  new_pair_it -> second;
    }

    //aux functions


    friend
    std::ostream& operator<<(std::ostream& os, const bst& x){
        if(x.root.get() == nullptr){
            os << "Empty tree, please insert some nodes" << std::endl;
            return os;
        }
        for(auto n : x)
        {
            os << n.first << "  " << n.second << "\n";
        }
        os << std::endl;
        return os;
    }

    void print_extended(){
        std::cout << "BEGIN OF THE TREE" << std::endl;
        for(auto n = begin(); n != end(); ++n){
            std::cout << n -> first << " " << n -> second << std::endl;
        }
    }

    void Fancy_print() 
    { 
        // Pass initial space count as 0 
        print2DUtil(root.get(), 0); 
    } 



    private:


    //insertion
    template<typename O>
    std::pair<iterator,bool> _insert_node(O&& node_to_insert);

    template<typename O>
    node_type* _find(O&& KEY);


    

    node_type* copy_all_nodes(node_type* current_node);

    /*
    Not mine code, actually taken from an article on the web and re adapted to 
    work on that implementation.
    Produces a fancy plot,
    not requested by the exam but beautyfull to see
    */
    void print2DUtil(node_type *root, int space) 
    { 
        if (root == nullptr) 
            return; 
    
        // Increase distance between levels 
        space += COUNT; 
    
        // Process right child first 
        print2DUtil(root->get_right(), space); 
    
        // Print current node after space 
        // count 
        std::cout<<std::endl; 
        for (int i = COUNT; i < space; i++) 
            std::cout<<" "; 
        std::cout<< "K:" <<root -> get_key()<< " V:" << root -> get_val() <<"\n"; 
    
        // Process left child 
        print2DUtil(root-> get_left(), space);  // Base case 
   
        } 
   
    
    };

/* based on linked list lecture
    uses forwarding reference to simplify insertion of a node,
    if a r-vale ref to pair is passed to contructor of node resources are stolen
*/

template <typename KEY_type, typename VAL_type, typename comparison_operator>
template<typename O>
std::pair<typename bst<KEY_type,VAL_type,comparison_operator>::iterator,bool> 
bst<KEY_type,VAL_type,comparison_operator>::_insert_node(O&& pair_to_insert)
        {
        auto node_to_insert = new node_type(std::forward<O>(pair_to_insert));
        if(root == nullptr){
            root.reset(node_to_insert);
            return std::pair<iterator,bool>(iterator(root.get()),true);
        }
        else{
            auto curr_node = root.get();
            while(true){
                if(!op(node_to_insert -> get_key(),curr_node -> get_key())){
                    if(curr_node -> get_right() == nullptr){
                        curr_node -> set_right(node_to_insert);
                        return std::pair<iterator,bool>{curr_node -> get_right(),true};
                    }
                    curr_node = curr_node -> get_right();
                }
                else if(op(node_to_insert -> get_key(),curr_node -> get_key())){
                    if(curr_node -> get_left() == nullptr){
                        curr_node -> set_left(node_to_insert);
                        return std::pair<iterator,bool>{curr_node -> get_left(),true};
                    }
                    curr_node = curr_node -> get_left();
                }
                else{
                    return std::pair<iterator,bool>{curr_node,false};
                }

            }
            
        } 
    } 

    template <typename KEY_type, typename VAL_type, typename comparison_operator>
    template <typename O>
    typename bst<KEY_type,VAL_type,comparison_operator>::node_type* 
    bst<KEY_type,VAL_type,comparison_operator>::_find (O&& KEY){
        auto current = root.get();
        while(current){
            if(KEY == current -> get_key()){
                return current;
            }
            else if (op(KEY,current -> get_key())){
                if(current -> get_left()){
                    current = current -> get_left();
                }
                else{
                    return nullptr;
                }
            }
            else{
                if(current -> get_right()){
                    current = current -> get_right();
                }
                else{
                    return nullptr;
                }
            }
            
        } 
        return current;
    }

    
    /*
    template <typename KEY_type, typename VAL_type, typename comparison_operator>
    typename bst<KEY_type,VAL_type,comparison_operator>::node_type* 
    bst<KEY_type,VAL_type,comparison_operator>::copy_all_nodes(bst<KEY_type,VAL_type,comparison_operator>::node_type* current_node){
        auto copied_node = new node_type(*current_node);
        if (current_node -> get_left() != nullptr) { 
            //copy recursively child
            //and set recursively the parenthood, parent is a raw pointer, only for auxiliary things
            copied_node -> set_left(copy_all_nodes(current_node -> get_left()));
            
            }
        if (current_node -> get_right() != nullptr){
            copied_node -> set_right(copy_all_nodes(current_node -> get_right()));
            }
        return copied_node;
    }
*/
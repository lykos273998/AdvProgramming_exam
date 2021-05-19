#include <iostream>
#include <algorithm>
#include <vector>
#include <memory>
#include <utility>
#include <exception>

#define COUNT 11

#include "bst_node.hpp"
#include "bst_iterator.hpp"

/**
 * BST implementation, templated on the KEY type, the VALUE type and the 
 * COMPARISON operator between key values
 * 
 * 
*/

template <typename KEY_type, typename VAL_type, typename comparison_operator=std::less<KEY_type>>
class bst{

    comparison_operator op;
    using node_type = node<const KEY_type,VAL_type>;
    using pair_type = std::pair<const KEY_type,VAL_type>;

    using const_iterator = Iterator<const pair_type,node_type>;
    using iterator = Iterator<pair_type,node_type>;
    


    public:

    std::unique_ptr<node_type> root{nullptr}; /** unique_ptr pointing to the root of the tree*/
    size_t Tot_nodes = 0; /** number of nodes present in the tree in any moment*/

    /**
     * Default constructor for bst class
     * */

    bst() = default;

    /**
     * Constructor for bst class taking as input a pair 
     * */


    explicit bst(const pair_type& p){
        root.reset(new node_type(p));
        Tot_nodes = 1;
    }


    explicit bst(pair_type&& p){
        root.reset(new node_type(std::move(p)));
        Tot_nodes = 1;
    }

    

    /**
     * copy constructor 
     * */


    bst(const bst& bst_to_copy_from){
        /*copy only the roots, then the full deep copy is performed at node level*/
        root.reset(new node_type(bst_to_copy_from.root.get()));
        Tot_nodes = bst_to_copy_from.Tot_nodes;
    }

    ~bst() noexcept = default;

    


    /**copy assignment
     * 
     * */
    bst& operator=(const bst& bst_to_copy_from){
        root.reset();
        auto tmp = bst_to_copy_from;
        (*this) = std::move(tmp);
        return *this;
    }



    /**move constructor 
     * Since no raw pointers are used, then default is enough
     * */
    bst(bst&& x) noexcept  = default;

    /**move assignment
     * Since no raw pointers are used, then default is enough
     * */
    bst &operator=(bst&& x) noexcept = default;

    /**
     * begin iterator relies on the implementation of the iterator
     * for bst class
     * */
    iterator begin(){
        node_type *current = root.get();
        while(current -> get_left()){
            current = current -> get_left();
        }
        return iterator(current);
    }

    /**
     * end iterator relies on the implementation of the iterator
     * for bst class
     * */

    iterator end(){
        node_type *current = root.get();
        return iterator(nullptr);
    }

    /**
     * begin iterator relies on the implementation of the iterator
     * for bst class
     * */

    const_iterator begin() const{
        node_type *current = root.get();
        while(current -> get_left()){
            current = current -> get_left();
        }
        return const_iterator(current);
    }

     /**
     * begin iterator relies on the implementation of the iterator
     * for bst class
     * */

    const_iterator cbegin() const{
        node_type *current = root.get();
        while(current -> get_left()){
            current = current -> get_left();
        }
        return const_iterator(current);
    }

     /**
     * end iterator relies on the implementation of the iterator
     * for bst class
     * */

    const_iterator end() const{
        node_type *current = root.get();
        return const_iterator(nullptr);
    }

     /**
     * end iterator relies on the implementation of the iterator
     * for bst class
     * */

    const_iterator cend() const{
        node_type *current = root.get();
        return const_iterator(nullptr);
    }
    
    /**
     * Clear method, resets to nullptr all the root of the tree
     * since the relationships between nodes in the tree
     * are implemented using unique_ptr this destroys recursively 
     * all the nodes in the tree clearing its content.
     * The number of node in the bst is set to 0
     * */
    void clear(){
        root.reset();
        Tot_nodes = 0;
    }
    
         
    /**
    * Insertion method for a l-value reference of a type pair,
    * relies on the implementation of the private function _insert_node
    *
    */
    std::pair<iterator,bool> insert(const pair_type& pair_to_insert){
        return this -> _insert_node(pair_to_insert);
        
    }

    /**
    * Insertion method for a r-value reference of a type pair,
    * relies on the implementation of the private function _insert_node
    *
    */
    std::pair<iterator,bool> insert(pair_type&& pair_to_insert){
        return this -> _insert_node(std::move(pair_to_insert));
        
    }
    
    /**
     * Inserts a new element into the container constructed in-place 
     * with the given args if there is no element with the key in the container.
     * */
    template< class... Types >
    std::pair<iterator,bool> emplace(Types&&... args) {
        return insert(pair_type(std::forward<Types>(args)...));
    }
   

    /**
     * 
     * Find a given key. If the key is present, 
     * returns an iterator to the proper node, end() otherwise.
     * */
    iterator find(const KEY_type& x){
        return iterator(_find(x));
    }
    const_iterator find(const KEY_type& x) const{
        return const_iterator(_find(x));
    }

    /**
     * Returns a reference to the value that is mapped to a key equivalent to x, 
     * performing an insertion if such key does not already exist.
     * */
    VAL_type& operator[](const KEY_type& x){
        /** uses find method implemented before
         *  
        */

        iterator pos = find(x);
        if(pos.current){
            return (*pos).second;
        }
        /**
        * if nullptr insert the key and leave the value type uninitialized
        * extract directly the iterator to the node
        * set the second as the rhs, actually works if the return is what I want to set
        * note! 
        * 
        * If I call 
        * std::cout << tree[key_not_present] << ...
        * an insertion is performed without initializing anything, so a new node is created
        */
        auto new_pair_it = insert(pair_type{x, {} }).first;
        return new_pair_it -> second;
    }

    VAL_type& operator[](KEY_type&& x){
        
        iterator pos = find(x);
        if(pos.current){
            return pos -> second;
        }
        /*
        * if nullptr insert the key and leave the value type uninitialized, as before
        * now the difference is that the KEY_type is an r-value and the KEY tipe
        * is moved into the new pair created
        */
        auto new_pair_it = insert(pair_type{std::move(x), {} }).first;
        return  new_pair_it -> second;
    }

    
    /**
     * put-to operator, if the tree is empty shows a message,
     * if not uses reange based for loop to print all key-value
     * pairs stored in order
     * */
    friend
    std::ostream& operator<<(std::ostream& os, const bst& x){
        if(x.root.get() == nullptr){
            os << "Empty tree, please insert some nodes" << std::endl;
            return os;
        }
        for(const auto& n : x)
        {
            os << n.first << "  " << n.second << "\n";
        }
        os << std::endl;
        return os;
    }

    /**
     * implementation of a pictorial way to see the tree structure
     * usefull also for debugging, not requested by the exam.
     * The code used is taken from an article on the web and adapted to 
     * work with this implementation of a bst
     * */
    void Fancy_print() 
    { 
        // Pass initial space count as 0 
        print2DUtil(root.get(), 0); 
    } 



    private:


    /**
     * private insertion method, uses universal forwarding reference to 
     * deal with l and r values at the same time, inspired from lecture
     * on linked lists
     * */
    template<typename O>
    std::pair<iterator,bool> _insert_node(O&& node_to_insert);
    
    /**
     * private find method, uses universal forwarding reference to 
     * deal with l and r values at the same time, inspired from lecture
     * on linked lists
     * */
    template<typename O>
    node_type* _find(O&& KEY);


    

   /**
     * implementation of a pictorial way to see the tree structure
     * usefull also for debugging, not requested by the exam.
     * The code used is taken from an article on the web and adapted to 
     * work with this implementation of a bst
     * */
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


template <typename KEY_type, typename VAL_type, typename comparison_operator>
template<typename O>
std::pair<typename bst<KEY_type,VAL_type,comparison_operator>::iterator,bool> 
bst<KEY_type,VAL_type,comparison_operator>::_insert_node(O&& pair_to_insert)
        {
        Tot_nodes = Tot_nodes + 1;
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
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
class Bst{

    comparison_operator op;
    using node_type = Node<const KEY_type,VAL_type>;
    using pair_type = std::pair<const KEY_type,VAL_type>;

    using const_iterator = Iterator<const pair_type,node_type>;
    using iterator = Iterator<pair_type,node_type>;
    


    public:

    std::unique_ptr<node_type> root{nullptr}; /** unique_ptr pointing to the root of the tree*/
    size_t Tot_nodes = 0; /** number of nodes present in the tree in any moment*/

    /**
     * Default constructor for Bst class
     * */

    Bst() = default;

    /**
     * Constructor for Bst class taking as input a pair 
     * */
    explicit Bst(const pair_type& p){
        root.reset(new node_type(p));
        Tot_nodes = 1;
    }
    

    explicit Bst(pair_type&& p){
        root.reset(new node_type(std::move(p)));
        Tot_nodes = 1;
    }

    

    /**
     * copy constructor 
     * */
    Bst(const Bst& bst_to_copy_from){
        /*copy only the roots, then the full deep copy is performed at Node level*/
        root.reset(new node_type(bst_to_copy_from.root.get()));
        Tot_nodes = bst_to_copy_from.Tot_nodes;
    }

    ~Bst() noexcept = default;

    /**copy assignment
     * 
     * */
    Bst& operator=(const Bst& bst_to_copy_from){
        root.reset();
        auto tmp = bst_to_copy_from;
        (*this) = std::move(tmp);
        return *this;
    }



    /**move constructor 
     * Since no raw pointers are used, then default is enough
     * */
    Bst(Bst&& x) noexcept  = default;

    /**move assignment
     * Since no raw pointers are used, then default is enough
     * */
    Bst &operator=(Bst&& x) noexcept = default;

    /**
     * begin iterator relies on the implementation of the iterator
     * for Bst class
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
     * for Bst class
     * */

    iterator end(){
      
        return iterator(nullptr);
    }

    /**
     * begin iterator relies on the implementation of the iterator
     * for Bst class
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
     * for Bst class
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
     * for Bst class
     * */

    const_iterator end() const{
        return const_iterator(nullptr);
    }

     /**
     * end iterator relies on the implementation of the iterator
     * for Bst class
     * */

    const_iterator cend() const{
        
        return const_iterator(nullptr);
    }
    
    /**
     * Clear method, resets to nullptr all the root of the tree
     * since the relationships between nodes in the tree
     * are implemented using unique_ptr this destroys recursively 
     * all the nodes in the tree clearing its content.
     * The number of Node in the Bst is set to 0
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
     * returns an iterator to the proper Node, end() otherwise.
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
        * extract directly the iterator to the Node
        * set the second as the rhs, actually works if the return is what I want to set
        * note! 
        * 
        * If I call 
        * std::cout << tree[key_not_present] << ...
        * an insertion is performed without initializing anything, so a new Node is created
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
    std::ostream& operator<<(std::ostream& os, const Bst& x){
        if(x.root.get() == nullptr){
            os << "Empty tree, please insert some nodes" << std::endl;
            return os;
        }
        os << "[ ";
        for(const auto& n : x)
        {
            os << "( " << n.first << " " << n.second << " ) ";
        }
        os << "]" << std::endl;
        return os;
    }

    /**
     * Rebalancing
     * relies on the private method, _balancd_insert'
    */
    void balance();

    /**
    * Erase method, erases a node from the bst, given the key
    * To erase a node saves its descendents subtrees (if present)
    * and then reinserts all the nodes in its descendance, preserving
    * original bst traversing order.
    * 
    */
    void erase(const KEY_type& x);


    /**
     * implementation of a pictorial way to see the tree structure
     * usefull also for debugging, not requested by the exam.
     * The code used is taken from an article on the web and adapted to 
     * work with this implementation of a Bst
     * 
     * */
    void Fancy_print() 
    { 
        std::cout << "/!\\ Note: the tree is printed horizontally!" << std::endl;
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
     * work with this implementation of a Bst
     * */
    void print2DUtil(node_type *root, int space) 
    { 
        if (root == nullptr) 
            return; 
    
        // Increase distance between levels 
        space += COUNT; 
    
        // Process right child first 
        print2DUtil(root->get_right(), space); 
    
        // Print current Node after space 
        // count 
        std::cout<<std::endl; 
        for (int i = COUNT; i < space; i++) 
            std::cout<<" "; 
        std::cout<< "K:" <<root -> get_key()<< " V:" << root -> get_val() <<"\n"; 
    
        // Process left child 
        print2DUtil(root-> get_left(), space);  // Base case 
   
        } 
   
   /**
    * Auxiliary function to insert the median of an array of KV pairs
    * At each step inserting the median of a sorted array makes sure that
    * the resulting Bst will be balanced
    * */
    void _balanced_insert(std::vector<pair_type>& v, size_t first, size_t last);


    /**
     * Auxiliary function to insert the subtree starting from a node into the tree
     * Rebuild the missing parts of the bst after erasing a node, in particular, reinserts 
     * a sub tree into the tree
    *
    */
    void _aux_erase(node_type* curr);
    };




template <typename KEY_type, typename VAL_type, typename comparison_operator>
template<typename O>
std::pair<typename Bst<KEY_type,VAL_type,comparison_operator>::iterator,bool> 
Bst<KEY_type,VAL_type,comparison_operator>::_insert_node(O&& pair_to_insert)
        {
        Tot_nodes = Tot_nodes + 1;
        auto node_to_insert = node_type(std::forward<O>(pair_to_insert));
        if(root.get() == nullptr){
            root.reset(new node_type(std::forward<O>(pair_to_insert)));
            return std::pair<iterator,bool>(iterator(root.get()),true);
        }
        else{
            auto curr_node = root.get();
            while(true){
                if( node_to_insert.get_key() == curr_node -> get_key()){
                    //delete[] node_to_insert;
                    return std::pair<iterator,bool>{curr_node,false};
                }
                if(!op(node_to_insert.get_key(),curr_node -> get_key())){
                    if(curr_node -> get_right() == nullptr){
                        curr_node -> set_right(new node_type(std::forward<O>(pair_to_insert)));
                        return std::pair<iterator,bool>{curr_node -> get_right(),true};
                    }
                    curr_node = curr_node -> get_right();
                }
                else if(op(node_to_insert.get_key(),curr_node -> get_key())){
                    if(curr_node -> get_left() == nullptr){
                        curr_node -> set_left(new node_type(std::forward<O>(pair_to_insert)));
                        return std::pair<iterator,bool>{curr_node -> get_left(),true};
                    }
                    curr_node = curr_node -> get_left();
                }
                else{
                    continue;
                }

            }
            
        } 
    } 

    template <typename KEY_type, typename VAL_type, typename comparison_operator>
    template <typename O>
    typename Bst<KEY_type,VAL_type,comparison_operator>::node_type* 
    Bst<KEY_type,VAL_type,comparison_operator>::_find (O&& KEY){
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


    template <typename KEY_type, typename VAL_type, typename comparison_operator>
    void Bst<KEY_type,VAL_type,comparison_operator>::balance() {
        // initialize a vector of nodes
        std::vector<pair_type> v;
        // get the iterators pointing to the first and last element of the tree
        iterator first{this->begin()};
        iterator last{this->end()};
        // it they coincide, the tree is empty, so no need to balance
        if(first==last)
            return;
        // populate the vectors with all the nodes of the tree
        while(first!=last) {
            v.push_back(*first);
            ++first;
        }

        // clear the tree
        clear();
        // create from the vector a new balanced tree
        _balanced_insert(v, 0, v.size());

    }


// build a balanced tree from an vector of Node
    template <typename KEY_type, typename VAL_type, typename comparison_operator>
    void Bst<KEY_type,VAL_type,comparison_operator>::_balanced_insert(std::vector<pair_type>& v, size_t first, size_t last) {

        if (first == last) {
            return;
        }
            size_t middle = (first+last)/2;
            insert(v[middle]);
            _balanced_insert(v, first, middle);
            _balanced_insert(v, middle+1, last);

        }


template <typename KEY_type, typename VAL_type, typename comparison_operator>
void Bst<KEY_type,VAL_type,comparison_operator>::erase(const KEY_type& x){
    auto node_itr = find(x);
    if(node_itr == end()){
        std::cout << "there's no matching key" << std::endl;
        return;
    }
    else{
        node_type* curr = node_itr.current;
        node_type* parent = curr -> parent;
        std::unique_ptr<node_type> right_child{curr -> RIGHT_child.release()};
        std::unique_ptr<node_type>  left_child{ curr -> LEFT_child.release()};
        /*if parent is nullptr it is the root*/
        if(parent){
            if(parent -> get_left() == curr){
                parent->LEFT_child.reset(nullptr);
            }
            else{
                parent->RIGHT_child.reset(nullptr);
            }
        }
        else{
            clear();
        }

        if(right_child.get()) {
            _aux_erase(right_child.get());
            
        }   
        if(left_child.get()){
            _aux_erase(left_child.get());
           
        }
        
        std::cout << "Successfully eliminated key "<< x << std::endl;
        
    }
}



template <typename KEY_type, typename VAL_type, typename comparison_operator>
void Bst<KEY_type,VAL_type,comparison_operator>::_aux_erase(node_type* curr){
    insert(curr -> get_pair());
    if(curr -> get_right()) {
        _aux_erase(curr -> get_right());
        curr -> RIGHT_child.reset();

    }   
    if(curr -> get_left()) {
        _aux_erase(curr -> get_left());
        curr -> LEFT_child.reset();
    }    
}
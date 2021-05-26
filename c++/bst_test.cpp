#include <iostream>
#include <algorithm>
#include <vector>
#include <memory>
#include <utility>
#include <exception>

#include "Include/bst.hpp"
//#include "Include/bst_node.hpp"
//#include "Include/bst_iterator.hpp"

int main(){
    using pair = std::pair<int,int>;
   
    /**
     * Generating some KV pairs to be inserted
     * */
    auto n8 = pair{8,12};
    auto n3 = pair{3,0};
    auto n1 = pair{1,0};
    auto n6 = pair{6,0};
    auto n4 = pair{4,0};
    auto n7 = pair{7,0};
    auto n10 = pair{10,0};
    auto n14 = pair{14,0};

    /*
    * Initializing an empty bst, using int as types  for Key and Values
    */
    auto my_tree = Bst<int,int>{};

    /**
     * Inseriting the previoulsy generated pairs in the tree
     * */
    my_tree.insert(n8);
    my_tree.insert(n3);
    my_tree.insert(n1);
    my_tree.insert(n6);
    my_tree.insert(n4);
    my_tree.insert(n7);
    my_tree.insert(n10);
    

    auto succesfull_insertion = my_tree.insert(n14);
    auto failed_insertion = my_tree.emplace(14,120);

    std::cout << "first time inserting a node with 14 as KEY: " << (succesfull_insertion.second ? "true" : "false") << std::endl;
    std::cout << "second time inserting a node with 14 as KEY: " << (failed_insertion.second ? "true" : "false") << std::endl;
    /*
    * forcing the insertion of an r - value
    */
    my_tree.insert(pair{13,131313});
    /**
     * inserting thorugh subscripting
     * */
    my_tree[5] = 55;

    /*
    * testing emplace
    */
    my_tree.emplace(127, 22);

    /**
     * printing the content of the tree
     * */
    std::cout << my_tree << std::endl;
    my_tree.Fancy_print();

    /**
     * 
     * Testing find and subscripting operators
     * 
     * Retriving a value through subscripting operator
     * 
     */
    std::cout << "Retriving a value through subscripting operator \n";
    std::cout << "searching for KEY 5 expecting value 55 \n";
    std::cout << my_tree[5] << std::endl;

    /**
    * Retriving a KV pair through find method
    *
    */
    std::cout << "Retriving a value through find method \n";
    std::cout << "searching for KEY 13 expecting VALUE 131313 \n";
    std::cout << (*(my_tree.find(13))).second << std::endl;

    /**
     * clearing the tree to do further tests
    */
    my_tree.clear();
    std::cout << "\nNow building an unbalanced tree and printing it" << std::endl;
    for(int i = 0; i<10; ++i){
        my_tree.emplace(i,0);
    }
    my_tree.Fancy_print();

    std::cout << "\nRebalancing and printing the tree" << std::endl;
    my_tree.balance();
    my_tree.Fancy_print();

    std::cout << "Erasing a node from the tree and checking if the node traversal order is preserved" << std::endl;
    my_tree.erase(4);
    std::cout << my_tree << std::endl;

    my_tree.Fancy_print();

    std::cout << "Now generating another tree by copying the original one and modifying it to see if they are unlinked" << std::endl;

    Bst<int,int> another_tree{my_tree};

    another_tree.emplace(1000,0);
    another_tree.erase(0);
    another_tree.erase(2);
    another_tree.erase(3);

    std::cout << "original one " << my_tree << std::endl;
    std::cout << "(deep) copied and modified tree" << another_tree << std::endl;

}

#include <iostream>
#include <algorithm>
#include <vector>
#include <memory>
#include <utility>
#include <exception>

#include "Include/bst.hpp"
//#include "Include/bst_node.hpp"
//#include "Include/bst_iterator.hpp"

int main(int argc, char** argv){
    using pair = std::pair<int,int>;
   

    auto n8 = pair(8,12);
    auto n3 = pair(3,0);
    auto n1 = pair(1,0);
    auto n6 = pair(6,0);
    auto n4 = pair(4,0);
    auto n7 = pair(7,0);
    auto n10 =pair(10,0);
    auto n14 =pair(14,0);
    auto n13 =pair(13,0);

    auto my_tree = bst<int,int>();

    auto x = my_tree.insert(n8);
    my_tree.insert(n3);
    my_tree.insert(n1);
    my_tree.insert(n6);
    my_tree.insert(n4);
    my_tree.insert(n7);
    my_tree.insert(n10);
    my_tree.insert(n14);
    my_tree.insert(n13);
    

    auto b = my_tree.begin();
    auto my_other_tree = my_tree;

    my_tree.insert(pair(15,0));
    auto my_other_tree_2{bst<int,int>(n3)};
    int k = 30;
    my_tree[k] = 11;
    
    std::cout << my_tree[8] << std::endl;
    std::cout << my_tree[k] << std::endl;
    return 0;
}

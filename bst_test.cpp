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
   

    auto n8 = pair{8,12};
    auto n3 = pair{3,0};
    auto n1 = pair{1,0};
    auto n6 = pair{6,0};
    auto n4 = pair{4,0};
    auto n7 = pair{7,0};
    auto n10 =pair{10,0};
    auto n14 =pair{14,0};
    auto n13 =pair{13,0};
    auto my_tree = bst<int,int,std::greater<int>>();

    auto x = my_tree.insert(n8);
    my_tree.insert(n3);
    my_tree.insert(n1);
    my_tree.insert(n6);
    my_tree.insert(n4);
    my_tree.insert(n7);
    my_tree.insert(n10);
    my_tree.insert(n14);
    my_tree.insert(pair{11,10});
    my_tree.insert(std::move(n13));

    

    auto b = my_tree.begin();
    auto my_other_tree = my_tree;

    my_tree.insert(pair(15,0));
    my_tree.emplace(1111,0);
    int k = 30;
    my_tree[k] = 11;
    
    std::cout << my_tree << std::endl;
    //std::cout << my_other_tree << std::endl;

    my_tree.Fancy_print();

    //bst<int,int> ttest = bst<int,int>{};
    //std::cout << (*(my_tree.find(7))).first <<std::endl;
/*
    ttest.emplace(5,0);
    ttest.emplace(2,0);
    ttest.emplace(4,0);
    ttest.emplace(3,0);
    ttest.emplace(1,0);
    ttest.emplace(0,0);
    ttest.emplace(8,0);
    ttest.emplace(9,0);
    ttest.emplace(6,0);
    ttest.emplace(7,0);


    ttest.Fancy_print();*/

   // std::cout << ttest << std::endl;

    return 0;
}

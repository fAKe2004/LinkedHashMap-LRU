#include "../lru/src.hpp"
#if defined (_UNORDERED_MAP_)  || (defined (_LIST_)) || (defined (_MAP_)) || (defined (_SET_)) || (defined (_UNORDERED_SET_))||(defined (_GLIBCXX_MAP)) || (defined (_GLIBCXX_UNORDERED_MAP))
BOOM :)
#endif

#include <iostream>
#include <cassert>
#include <string>


void check(bool &flag){
    if(!flag){
        std::cout<<"wrong"<<std::endl;
        exit(0);
    }else{
        flag = false;
    }
}

void tiny_linked_hashmap_tester(){
    using mType = Matrix<int>;
    using value_type = sjtu::pair<Integer,mType >;
    using mp = sjtu::linked_hashmap<Integer,mType, Hash, Equal>;
    const int n = 100;
    mp map;
    map.insert(value_type(Integer(0),mType(2,2,1)));
    mp::iterator it = map.begin();
    mp::const_iterator cit = map.cbegin();
    while(it != map.end()){
        it++;
        if(it == map.end()){
            break;
        }
        ++it;
    }
    while(cit != map.cend()){
        cit++;
        if(cit == map.cend()){
            break;
        }
        ++cit;
    }
    mp::iterator it2 = it;
    mp::iterator it3(it);
    mp::const_iterator cit2 = cit;
    mp::const_iterator cit3(cit);

    bool flag = false;
    try{
        map.begin()--;
    }catch(...){
        flag = true;
    }
    check(flag);
    std::cerr << "GOOD 55\n" << std::endl;
    try{
        map.begin()--;
    }catch(...){
        flag = true;
    }
    check(flag);
    std::cerr << "GOOD 62\n" << std::endl;
    try{
        map.cbegin()--;
    }catch(...){
        flag = true;
    }
    check(flag);
    std::cerr << "GOOD 69\n" << std::endl;
    try{
        map.end()++;
    }catch(...){
        flag = true;
    }
    check(flag);
    std::cerr << "GOOD 76\n" << std::endl;
    try{
        map.cend()++;
    }catch(...){
        flag = true;
    }
    check(flag);
    std::cerr << "GOOD 83\n" << std::endl;
    try{
        *(map.end());
    }catch(...){
        flag = true;
    }
    check(flag);
    std::cerr << "GOOD 90\n" << std::endl;
    try{
        *(map.cend());
    }catch(...){
        flag = true;
    }
    check(flag);
    std::cerr << "GOOD 97\n" << std::endl;
    try{
        map.at(map.size());
    }catch(...){
        flag = true;
    }
    check(flag);
    std::cerr << "GOOD 104\n" << std::endl;
    try{
        map[map.size()];
    }catch(...){
        flag = true;
    }
    check(flag);
}

int main(){
#ifdef _OUTPUT_
    freopen("6.out","w",stdout);
#endif
    tiny_linked_hashmap_tester();
    std::cout<<"PASS"<<std::endl;
}
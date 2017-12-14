#ifndef DEF_FOODUTILS
#define DEF_FOODUTILS

#include <iostream>
#include <fstream>
#include <vector>
#include "Menu.h"

static std::ostream& operator<<(std::ostream& os, const std::vector<Menu> &menus){
    int length = menus.size();
    os << length << std::endl;
    for(int i=0 ; i<length ; i++){
        os << menus[i];
        if(i!=length-1){
            os << std::endl;
        }
    }
    return os;
}

static std::istream& operator>>(std::istream& is, std::vector<Menu> &menus){
    std::string line;
    getline(is, line);
    int length = std::stoi(line);
    menus.resize(length);
    for(int i=0 ; i<length ; i++){
        is >> menus[i];
    }
    return is;
}

static bool saveMenus(std::string filename, const std::vector<Menu> &menus){
    std::ofstream file(filename.c_str());
    if(file){
        file << menus;
        file.close();
        return true;
    }
    return false;
}

static bool loadMenus(std::string filename, std::vector<Menu> &menus){
    std::ifstream file(filename.c_str());
    if(file){
        file >> menus;
        file.close();
        return true;
    }
    return false;
}

static std::string toString(const std::vector<Menu> &menus){
    std::stringstream ss;
    ss << menus;
    return ss.str();
}

#endif
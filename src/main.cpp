#include <iostream>
#include <unistd.h>
#include <map>

#include "../food/include/menuUtils.h"
#include "../food/include/foodUtils.h"
#include "../nfc/include/nfcManager.h"
#include "../network/include/network.h"

const std::string BASE_URL = "http://127.0.0.1:8080";

std::map<uint8_t, std::string> getCommands(){
    std::map<uint8_t, std::string> cmds;
    std::string json;

    // menus
    std::vector<Menu> menus;
    if(!requestUri(BASE_URL, "/menu", json)){
        std::cout << "Could not get menus from server." << std::endl;
        exit(1);
    }
    if(!parseMenus(json, menus)){
        std::cout << "Could not parse menus." << std::endl;
        exit(1);
    }
    cmds[0x01] = toString(menus);

    // food
    std::vector<Food> food;
    if(!requestUri(BASE_URL, "/food", json)){
        std::cout << "Could not get food from server." << std::endl;
        exit(1);
    }
    if(!parseFood(json, food)){
        std::cout << "Could not parse food." << std::endl;
        exit(1);
    }
    cmds[0x02] = toString(food);
    
    return cmds;
}

int main(int argc, char const *argv[]) {
    std::map<uint8_t, std::string> cmds = getCommands();

    NfcManager manager;
    if(manager.open()){
        std::cout << "Nfc device opened. Waiting for target..." << std::endl;

        while(1){
            APDUResp rapdu;
            APDUCmd capdu;

            while(!manager.isTargetPresent());
            if(manager.selectApplication("F222222222", rapdu)){
                std::vector<uint8_t> response = rapdu.getBytesData();
                while(response.size()!=0){
                    capdu.setClass(response[0]);
                    capdu.setData(cmds[response[0]]);
                    if(!manager.transceive(capdu, rapdu)){
                        break;
                    }
                    response = rapdu.getBytesData();
                }
            }
            else{
                std::cout << "select error" << std::endl;
            }
        }

        manager.close();
    }
    else{
        std::cout << "could not open device" << std::endl;
    }
    return 0;
}

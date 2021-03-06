/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Description: Copyright SaeruHikari
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-02-24 13:24:40
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2020-03-08 15:30:28
 */
#include "SakuraEngine/SPAModules.generated.h"//include generated file in the root path
#include "SPA/include/modulemanager.h"
#include <iostream>

int main(void)
{
    auto mng = GetModuleManager();
    
#ifndef _WIN32
    #ifdef CONFINFO_PLATFORM_MACOS
        mng->Mount("/Users/saeruhikari/Coding/SakuraEngine/build");
    #else
        mng->Mount("/home/saeruhikari/Coding/SakuraEngine/build");
    #endif
#elif defined(_WIN32)
    mng->Mount("D:\\Coding\\SakuraEngine\\build");
#endif
    std::cout << "Root: " << mng->GetRoot() << std::endl;
    std::string mainName = "EditorModule";//Need a module as the entry, this module can be the total dispatcher
    mng->MakeModuleGraph(mainName.c_str(), true);
    auto edit = mng->GetModule("EditorModule");
    mng->InitModuleGraph();
    mng->DestroyModuleGraph();
    return 0;
}
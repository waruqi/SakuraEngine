/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Description: Copyright SaeruHikari
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-02-09 10:27:08
 * @LastEditors: SaeruHikari
 * @LastEditTime: 2020-02-24 11:33:19
 */
#pragma once
#include "Core/CoreMinimal/SKeyWords.h"
#include "Core/CoreMinimal/sinterface.h"
#include "SWindow.h"

namespace Sakura
{
    sinterface SApplication
    {
        virtual bool Initialize(void) = 0;
        virtual int Run(void) = 0;
        virtual bool Destroy(void) = 0;
    };
}
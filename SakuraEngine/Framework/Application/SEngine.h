/*
 * @CopyRight: MIT License
 * Copyright (c) 2020 SaeruHikari
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 *  IN THESOFTWARE.
 * 
 * 
 * @Description: 
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-02-25 22:25:59
 * @LastEditTime: 2020-03-02 15:00:21
 */
#pragma once
#include "SApplication.h"

namespace Sakura
{
    SInterface SEngine;
}

namespace Sakura
{
    namespace ____
    {
        extern SEngine* eEngine;
    }
    struct EngineDesc
    {
        
    };
    SInterface SEngine : SImplements SApplication
    {
        SEngine(const EngineDesc& _desc)
            :desc(_desc){}
        SEngine() = default;
        SAKURA_API static void CreateSEngine(const EngineDesc& _desc);
        SAKURA_API static std::unique_ptr<SWindow> CreateSWindow(const WindowDesc& desc);
    public:
        // get;
        SAKURA_API static EngineDesc GetEngineDesc(void);
        SAKURA_API static SEngine* GetEngine(void);
    private:
        EngineDesc desc;
    };

}
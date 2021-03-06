/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Description: OS information
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-02-09 16:16:33
 * @LastEditors  : SaeruHikari
 * @LastEditTime : 2020-02-09 19:59:35
 */
#pragma once
#include "../Containers/SString.h"

namespace Sakura
{
    namespace os
    {
        /**
         * @description: Get a environment param. 
         * @param string environment param name 
         * @return: env param value
         * @author: SaeruHikari
         */
        sstring env(const char* name);

        /**
         * @description: 
         * @param void
         * @return: home dir or current user
         * @author: SaeruHikari
         */
        sstring homedir(void);

        /**
         * @description: Get current work directory 
         * @param void
         * @return: current work directory.
         * @author: SaeruHikari
         */
        sstring cwd(void);

        /**
         * @description: 
         * @param void 
         * @return: 
         * @author: SaeruHikari
         */
        sstring exepath(void);

        /**
         * @description: 
         * @param void
         * @return: name of executable.
         * @author: SaeruHikari
         */
        sstring exename(void);

        /**
         * @description: Get PID. 
         * @param void 
         * @return: process id 
         * @author: SaeruHikari
         */
        int pid(void);

        /**
         * @description: Get Cpu num. 
         * @param void 
         * @return: num of cpu
         * @author: SaeruHikari
         */    
        int cpunum(void);
    }
}

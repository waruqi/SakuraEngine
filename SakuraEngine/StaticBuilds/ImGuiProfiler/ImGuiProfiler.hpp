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
 * @Date: 2020-03-16 17:02:13
 * @LastEditTime: 2020-03-16 20:53:03
 */
#pragma once
#include "memory_resource"
#include <string>
#include "SakuraEngine/Core/CoreMinimal/CoreMinimal.h"
#include "ImGuiVulkanSDL.h"
#include "SakuraEngine/StaticBuilds/GraphicsInterface/CGD_Vulkan/CGD_Vulkan.h"
#include "SakuraEngine/Core/Core.h"

using namespace Sakura::Graphics;
using namespace Sakura::Graphics::Vk;
using namespace Sakura::Graphics::Im::Vk;


namespace Sakura::Graphics::Im
{
    SInterface ImGuiWindow
    {
        ImGuiWindow(const Sakura::Graphics::CGD& _gfxDevice)
            :gfxDevice(_gfxDevice){}
        virtual ~ImGuiWindow() = default;
    protected:
        const Sakura::Graphics::CGD& gfxDevice;
    };
    /**
     * @description: Profiler implemented by ImGui. 
     * Initialize it with a CGD Device referrence.  
     * @author: SaeruHikari
     */
    class ImGuiProfiler
    {
        DECLARE_LOGGER("ImGuiProfiler")
    public:
        ImGuiProfiler(const Sakura::Graphics::CGD& _gfxDevice)
            :gfxDevice(_gfxDevice)
        {
            backend = _gfxDevice.GetBackEndAPI();
        }
        ImGuiWindow* CreateImGuiWindow(SDL_Window* wind, int width, int height);
        void ImGuiRender(ImGuiWindow* wd);
        void ImGuiPresent(ImGuiWindow* wd);
    private:
        inline VkDevicePack GetVkDevicePack()
        {
            if(backend != TargetGraphicsInterface::CGD_TARGET_VULKAN)
                ImGuiProfiler::error("ImGuiProfiler: Not Vk backend, can not Get VkDevice Pack!");
            VkDevicePack pack;
            auto&& entity = ((const CGD_Vk&)gfxDevice).GetCGDEntity();
            pack.allocator = nullptr;    pack.instance = entity.instance;
            pack.device = entity.device; pack.physicalDevice = entity.physicalDevice;
            pack.graphicsQueue = 
                ((CommandQueueVk*)entity.graphicsQueue.get())->vkQueue;
            pack.graphicsQueueFamily = 
                ((const CGD_Vk&)gfxDevice).GetQueueFamily().graphicsFamily.value();
            pack.minImageCount = 2;
            return pack;
        }
    private:
        TargetGraphicsInterface backend;
        const Sakura::Graphics::CGD& gfxDevice;
    };
    
    struct ImGuiWindowVk : SImplements ImGuiWindow
    {
        ImGuiWindowVk(const Sakura::Graphics::CGD& _gfxDevice, 
            ImGui_ImplVulkanH_Window* _wind)
            :wind(_wind), ImGuiWindow(_gfxDevice)
            {

            }
        virtual ~ImGuiWindowVk() override final
        {
            ImGui_ImplVulkanH_DestroyWindow(
                ((const CGD_Vk&)gfxDevice).GetCGDEntity().instance,
                ((const CGD_Vk&)gfxDevice).GetCGDEntity().device, wind, nullptr);
        }
        ImGui_ImplVulkanH_Window* wind = nullptr;
    };

    ImGuiWindow* ImGuiProfiler::CreateImGuiWindow(SDL_Window* wind, int width, int height)
    {
        switch (backend)
        {
        case TargetGraphicsInterface::CGD_TARGET_VULKAN:
        {
            static bool init = false;
            VkDevicePack pack = GetVkDevicePack();
            VkSurfaceKHR surface;
            VkResult err;
            if (SDL_Vulkan_CreateSurface(wind, pack.instance, &surface) == 0)
            {
                ImGuiProfiler::error("Failed to create Vulkan surface.\n");
                return nullptr;
            }
            auto windIm = CreateImGuiWindowVk(pack, surface, width, height);
            if(!init)
            {
                ImGui::CreateContext();
                ImGuiIO& io = ImGui::GetIO(); (void)io;
                ImGui::StyleColorsDark();
                ImGui_ImplSDL2_InitForVulkan(wind);
                ImGui_ImplVulkan_InitInfo init_info = {};
                init_info.Instance = pack.instance;
                init_info.PhysicalDevice = pack.physicalDevice;
                init_info.Device = pack.device;
                init_info.QueueFamily = pack.graphicsQueueFamily;
                init_info.Queue = pack.graphicsQueue;
                VkDescriptorPoolSize pool_sizes[] =
                {
                    { VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
                    { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
                    { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
                    { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
                    { VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
                    { VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
                    { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
                    { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
                    { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
                    { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
                    { VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
                };
                VkDescriptorPoolCreateInfo pool_info = {};
                pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
                pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
                pool_info.maxSets = 1000 * IM_ARRAYSIZE(pool_sizes);
                pool_info.poolSizeCount = (uint32_t)IM_ARRAYSIZE(pool_sizes);
                pool_info.pPoolSizes = pool_sizes;
                err = vkCreateDescriptorPool(pack.device,
                    &pool_info, pack.allocator, &pack.descriptorPool);
                check_vk_result(err);
                init_info.PipelineCache = pack.pipelineCache;
                init_info.DescriptorPool = pack.descriptorPool;
                init_info.Allocator = nullptr;
                init_info.MinImageCount = pack.minImageCount;
                init_info.ImageCount = windIm->ImageCount;
                init_info.CheckVkResultFn = check_vk_result;
                ImGui_ImplVulkan_Init(&init_info, windIm->RenderPass);
                {
                    // Use any command queue
                    VkCommandPool command_pool =
                        windIm->Frames[windIm->FrameIndex].CommandPool;
                    VkCommandBuffer command_buffer =
                        windIm->Frames[windIm->FrameIndex].CommandBuffer;

                    err = vkResetCommandPool(pack.device, command_pool, 0);
                    check_vk_result(err);
                    VkCommandBufferBeginInfo begin_info = {};
                    begin_info.sType =
                        VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
                    begin_info.flags |=
                        VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
                    err = vkBeginCommandBuffer(command_buffer, &begin_info);
                    check_vk_result(err);

                    ImGui_ImplVulkan_CreateFontsTexture(command_buffer);

                    VkSubmitInfo end_info = {};
                    end_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
                    end_info.commandBufferCount = 1;
                    end_info.pCommandBuffers = &command_buffer;
                    err = vkEndCommandBuffer(command_buffer);
                    check_vk_result(err);
                    err = vkQueueSubmit(pack.graphicsQueue, 1, &end_info, VK_NULL_HANDLE);
                    check_vk_result(err);

                    err = vkDeviceWaitIdle(pack.device);
                    check_vk_result(err);
                    ImGui_ImplVulkan_DestroyFontUploadObjects();
                }
            }
            return new ImGuiWindowVk(gfxDevice, windIm);
        }
        default:
            ImGuiProfiler::error("ImGui Not supported now!");
            break;
        }
        return nullptr;
    }
    void ImGuiProfiler::ImGuiRender(ImGuiWindow* wd)
    {
        switch (backend)
        {
        case TargetGraphicsInterface::CGD_TARGET_VULKAN:
        {
            auto vkwd = (ImGuiWindowVk*)wd;
            ImGuiRenderVk(GetVkDevicePack(), vkwd->wind);
            return;
        }
        default:
            ImGuiProfiler::error("ImGui Not supported now!");
            return;
        }
    }

    void ImGuiProfiler::ImGuiPresent(ImGuiWindow* wd)
    {
        switch (backend)
        {
        case TargetGraphicsInterface::CGD_TARGET_VULKAN:
        {
            auto vkwd = (ImGuiWindowVk*)wd;
            ImGuiPresentVk(GetVkDevicePack(), vkwd->wind);
            return;
        }
        default:
            ImGuiProfiler::error("ImGui Not supported now!");
            return;
        }
    }
}
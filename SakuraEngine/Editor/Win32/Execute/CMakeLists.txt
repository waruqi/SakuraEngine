add_executable(SakuraEngineEditorWin32 
    Win32Execute.cpp
    ../../VkTestApplication.hpp
)

target_link_libraries(SakuraEngineEditorWin32 PUBLIC
SPA TaskSystem CoreModule ${Vulkan_LIBRARIES}) 
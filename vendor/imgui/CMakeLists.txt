add_library(imgui OBJECT)
target_sources(imgui PRIVATE
    imgui.cpp
    imgui_draw.cpp
    imgui_internal.h
    imgui_widgets.cpp
    imstb_textedit.h
    imconfig.h
    imgui_demo.cpp 
    imgui.h
    imgui_impl_glfw.h
    imgui_impl_glfw.cpp
    imgui_impl_opengl3.h
    imgui_impl_opengl3.cpp
    imgui_impl_opengl3_loader.h
    imgui_tables.cpp  
    imstb_rectpack.h
    imstb_truetype.h)

target_include_directories(imgui PUBLIC ./)

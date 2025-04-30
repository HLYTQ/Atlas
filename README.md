# Atlas Engine

## 事件系统

这一部分涉及到回调函数的代码比较不直观，其运行过程相对较难理解，此处给出一些解释。

在OpenGLWindow初始化过程中，为各个glfw的事件注册好回调的函数。注意`glfwSetWindowUserPointer`与`glfwGetWindowUserPointer`。使用这两个函数来绑定数据，来实现以下目的（来源StackOverflow）：

UserData字段是C API中一个相当常见的范例，它允许用户从回调内部访问上下文数据，而不需要使一切都是全局的。本质上，它允许将与程序相关的任意数据与glfw窗口相关联。例如，如果试图在遵循面向对象设计的程序中使用glfw，则可以使用此指针存储正在处理特定窗口的实例的地址，并将回调（由于API的工作方式而必须是静态函数）转发到相应的成员函数。

通过在窗口初始化时为glfw事件注册回调函数（如下例中的关闭窗口事件），当该事件发生时，被注册的回调函数执行。首先去获取`WindowData`（来自于上述的数据与glfw窗口关联操作），然后产生一个事件的标识`WindowCloseEvent`交由自定义的窗口事件回调函数执行。

```c++
    glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {
        WindowData& data = *(WindowData*) glfwGetWindowUserPointer(window);
        WindowCloseEvent event;
        data.EventCallback(event);
    });
```
紧随在窗口初始化完成后，为窗口实例绑定回调函数。也就是`OnEvent`，这样，`OnEvent`函数得以在glfw事件发生时被执行，从而完成一些上层应用需要的操作。

```c++
    m_Window->SetEventCallback(BIND_CALLBACK_FUNC(OnEvent));
```

设置glfw回调函数 ---WindowData传递---> 绑定用户事件回调函数

## virtual

虚表的尽管并不是一个为人所称道的特性，但它确实非常有用：

```c++
    for (Layer* layer : m_LayerStack) {
        layer->OnUpdate();
   }
	m_ImGuiLayer->Begin();
        for (Layer* layer : m_LayerStack) {
	    layer->OnImGuiRender();
	}
	m_ImGuiLayer->End();
```
同样继承至`Layer`，虚表可以自动区分出一个实例是否具有`OnUpdate`函数或者是`OnImGuiRender`。

## 四元数相机处理天空盒问题

(1) 天空盒的 View 矩阵需要移除平移部分
天空盒通常位于无限远处，因此 相机的平移（位置）不应影响天空盒的渲染。

错误做法：直接使用普通相机的 View 矩阵（包含平移）。

正确做法：移除 View 矩阵中的平移分量，只保留旋转（四元数的旋转部分）。

(2) 四元数到 View 矩阵的转换可能有误
四元数相机的 View 矩阵需要通过四元数的 共轭（conjugate） 表示相机朝向的逆旋转。

2. 修复步骤
(1) 构造正确的 View 矩阵（C++ 端）
在传递 ViewProjection 矩阵给着色器之前，确保 View 矩阵仅包含旋转（移除平移）：

cpp
// 假设：
// - orientation 是四元数表示的相机旋转
// - projection 是投影矩阵
glm::mat4 view = glm::mat4_cast(glm::conjugate(orientation)); // 关键：共轭四元数 + 移除平移
glm::mat4 viewProjection = projection * view;
或者（如果仍想保留 lookAt 风格）：

cpp
glm::vec3 forward = orientation * glm::vec3(0, 0, -1); // 四元数旋转前向
glm::mat4 view = glm::lookAt(glm::vec3(0), forward, glm::vec3(0, 1, 0)); // 相机位置设为 (0,0,0)
glm::mat4 viewProjection = projection * view;

## 后续计划

1. 天空盒映射
2. 天空盒选择，ImGui::Combo
3. 物理材质
4. ImGui::Plotting Lines动画显示帧率
5. 显卡，opengl信息窗口
6. 【可选】地板
7. 控制台输出log
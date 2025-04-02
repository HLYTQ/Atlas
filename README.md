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

#pragma once

#include <memory>

// Platform detection using predefined macros
#ifdef _WIN32
	/* Windows x64/x86 */
	#ifdef _WIN64
		/* Windows x64  */
		#define AT_PLATFORM_WINDOWS
	#else
		/* Windows x86 */
		#error "x86 Builds are not supported!"
	#endif
#elif defined(__ANDROID__)						/* We also have to check __ANDROID__ before __linux__.  Since android is based on the linux kernel , and it has __linux__ defined */
	#define AT_PLATFORM_ANDROID
	#error "Android is not supported!"
#elif defined(__Linux__)
	#define AT_PLATFORM_LINUX
	#error "Linux is not supported!"
#elif defined(__APPLE__) || defined(__MACH__)
	#include <TargetConditionals.h>				// Apple �кܶ�����ʽ��������Ҫȷ����Ӧ��ƽ̨�������ж�Ӧ�Ķ���
	#if TARGET_IPHONE_SIMULATOR == 1
		#error "IOS Simulator is not supported!"
	#elif TARGET_OS_IPHONE == 1
		#define AT_PLATFORM_IOS
		#error "IOS is not supported!"
	#elif TARGET_OS_MAC
		#define AT_PLATFORM_MACOS
		#error "MacOS is not supported!"
	#else
		#error "Unknown Apple platform!"
	#endif
#else
	#error "Unknown platform!"
#endif

#ifdef _DEBUG
	#define AT_ENABLE_ASSERTS

	#ifdef AT_PLATFORM_WINDOWS
		#define AT_DEBUGBREAK() __debugbreak();
	#elif AT_PLATFORM_LINUX
		#include <signal.h>
		#define AT_DEBUGBREAK() raise(SIGTRAP)
	#else
		#error	"Platform doesn't support debugbreak yet! "
	#endif
#else
	#define AT_DEBUGBREAK()
#endif

#ifdef AT_ENABLE_ASSERTS
	#define AT_CORE_ASSERT(x, ...) \
		{if(!(x)){\
			AT_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__);\
			AT_DEBUGBREAK();}\
		}
	#define AT_ASSERT(x, ...)\
		{if(!(x)){\
			AT_ERROR("Assertion Failed: {0}", __VA_ARGS__);\
			AT_DEBUGBREAK();}\
		}
#else
	#define AT_CORE_ASSERT(x, ...)
	#define AT_ASSERT(x, ...)
#endif

#define Bit(x) (1 << x)
#define BIND_CALLBACK_FUNC(x) std::bind(&x, this, std::placeholders::_1)

namespace Atlas {
    template <typename T>
    using Ref = std::shared_ptr<T>;

    template<typename T, typename... Args>
    constexpr Ref<T> CreateRef(Args&&... args) {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }

    template <typename T>
    using Scope = std::unique_ptr<T>;

    template <typename T, typename... Args>
    constexpr Scope<T> CreateScope(Args&&... args) {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }
}
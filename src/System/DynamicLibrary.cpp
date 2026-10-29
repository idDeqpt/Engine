#include <Engine/System/DynamicLibrary.hpp>

#include <filesystem>
#include <string>

#ifdef _WIN32
	//need test
	#include <windows.h>
#else
	#include <dlfcn.h>
#endif

namespace
{

#ifdef _WIN32
	//need test
	constexpr const char* DEFAULT_PREFIX    = "";
	constexpr const char* DEFAULT_EXTENSION = ".dll";
#elif defined(__APPLE__)
	//need test
	constexpr const char* DEFAULT_PREFIX    = "lib";
	constexpr const char* DEFAULT_EXTENSION = ".dylib";
#else
	constexpr const char* DEFAULT_PREFIX    = "lib";
	constexpr const char* DEFAULT_EXTENSION = ".so";
#endif

std::string makeFileName(const std::string& base_name)
{
	return std::string(DEFAULT_PREFIX) + base_name + DEFAULT_EXTENSION;
}

#ifdef _WIN32
std::string convertWindowsError(DWORD code)
{
	LPSTR buffer = nullptr;
	const DWORD size = FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM     |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr,
		code,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<LPSTR>(&buffer),
		0,
		nullptr);

	std::string message = (size && buffer) ? std::string(buffer, size) : std::string("unknown error");
	if (buffer) LocalFree(buffer);

	while (!message.empty() && (message.back() == '\n' || message.back() == '\r' || message.back() == ' '))
		message.pop_back();

	return message;
}
#endif

} //namespace

namespace eng::sys
{

std::string DynamicLibrary::getDefaultPrefix()    { return DEFAULT_PREFIX; }
std::string DynamicLibrary::getDefaultExtension() { return DEFAULT_EXTENSION; }


DynamicLibrary::DynamicLibrary(DynamicLibrary&& other) noexcept:
	m_handle(other.m_handle),
	m_last_error(std::move(other.m_last_error))
{
	other.m_handle = nullptr;
}

DynamicLibrary& DynamicLibrary::operator=(DynamicLibrary&& other) noexcept
{
	if (this != &other)
	{
		close();
		m_handle = other.m_handle;
		m_last_error = std::move(other.m_last_error);
		other.m_handle = nullptr;
	}
	return *this;
}

DynamicLibrary::~DynamicLibrary()
{
	close();
}


bool DynamicLibrary::open(const std::string& directory, const std::string& base_name)
{
	std::filesystem::path p(base_name);
	if (p.has_extension())
		return open((std::filesystem::path(directory) / p).string());

	return open((std::filesystem::path(directory) / makeFileName(base_name)).string());
}

bool DynamicLibrary::open(const std::string& path)
{
	close();

	#ifdef _WIN32
	//need test
		HMODULE handle = LoadLibraryExW(
			std::filesystem::path(path).c_str(),
			nullptr,
			LOAD_WITH_ALTERED_SEARCH_PATH
		);
		if (!handle)
		{
			m_last_error = std::string("open: ") + convertWindowsError(GetLastError());
			return false;
		}
		m_handle = static_cast<void*>(handle);
	#else
		m_handle = dlopen(path.c_str(), RTLD_NOW | RTLD_LOCAL);
		if (!m_handle)
		{
			m_handle = nullptr;
			m_last_error = std::string("open: ") + dlerror();
			return false;
		}
	#endif

	m_last_error = "";
	return true;
}

void DynamicLibrary::close()
{
	if (m_handle == nullptr) return;

	#ifdef _WIN32
	//need test
		FreeLibrary(static_cast<HMODULE>(m_handle));
	#else
		dlclose(m_handle);
	#endif

	m_handle = nullptr;
}

void* DynamicLibrary::getSymbol(const std::string& name) const
{
	if (!m_handle)
	{
		m_last_error = "getSymbol: library not opened";
		return nullptr;
	}

	void* symbol = nullptr;

	#ifdef _WIN32
	//need test
		FARPROC proc = GetProcAddress(static_cast<HMODULE>(m_handle), name.c_str());
		if (!proc)
		{
			m_last_error = std::string("getSymbol: \"") + name + "\": " + convertWindowsError(GetLastError());
			return nullptr;
		}
		symbol = reinterpret_cast<void*>(proc);
	#else
		dlerror();
		symbol = dlsym(m_handle, name.c_str());
		const char* err = dlerror();
		if (err != nullptr)
		{
			m_last_error = std::string("getSymbol: ") + std::string(err);
			return nullptr;
		}
	#endif

	m_last_error = "";
	return symbol;
}


bool DynamicLibrary::isOpen() const
{
	return (m_handle != nullptr);
}

const std::string& DynamicLibrary::getLastError() const
{
	return m_last_error;
}

} //namespace eng::sys
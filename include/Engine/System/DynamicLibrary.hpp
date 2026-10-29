#ifndef DYNAMIC_LIBRARY_CLASS_HEADER
#define DYNAMIC_LIBRARY_CLASS_HEADER

#include <string>

namespace eng::sys
{
	class DynamicLibrary
	{
	public:
		DynamicLibrary() = default;
		~DynamicLibrary();

		DynamicLibrary(const DynamicLibrary&) = delete;
		DynamicLibrary& operator=(const DynamicLibrary&) = delete;
		DynamicLibrary(DynamicLibrary&& other) noexcept;
		DynamicLibrary& operator=(DynamicLibrary&& other) noexcept;

		bool open(const std::string& directory, const std::string& base_name);
		bool open(const std::string& path);
		void close();
		void* getSymbol(const std::string& name) const;

		bool isOpen() const;
		const std::string& getLastError() const;
		
		static std::string getDefaultPrefix();
		static std::string getDefaultExtension();

	protected:
		void* m_handle = nullptr;
		mutable std::string m_last_error;
	};
}

#endif //DYNAMIC_LIBRARY_CLASS_HEADER
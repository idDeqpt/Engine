#include <Engine/Core/String.hpp>

#include <string>


namespace eng
{

core::String::String()                     {}
core::String::String(const char* v)        {*this << v;}
core::String::String(const std::string& v) {*this << v;}
core::String::String(int v)                {*this << v;}
core::String::String(unsigned int v)       {*this << v;}
core::String::String(float v)              {*this << v;}
core::String::String(const void* v)        {*this << v;}


const std::string& core::String::getString() const
{
	return m_str;
}


core::String& core::String::operator<<(const char* v)
{
	m_str += v;
	return *this;
}

core::String& core::String::operator<<(const std::string& v)
{
	m_str += v;
	return *this;
}

core::String& core::String::operator<<(int v)
{
	m_str += std::to_string(v);
	return *this;
}

core::String& core::String::operator<<(unsigned int v)
{
	m_str += std::to_string(v);
	return *this;
}

core::String& core::String::operator<<(float v)
{
	m_str += std::to_string(v);
	return *this;
}

core::String& core::String::operator<<(const void* v)
{
	m_str += "0x" + std::to_string(int(v));
	return *this;
}

} //namespace eng
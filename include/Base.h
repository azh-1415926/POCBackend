#pragma once

#define SINGLETONE(Name,Type) \
class Name \
{ \
    public: \
        static Name& getInstance() \
        { \
            static Name instance; \
            return instance; \
        } \
        Type get() {  return m_Instance; } \
        void set(const Type& data) { m_Instance=data; } \
    private: \
        explicit Name() { } \
        Type m_Instance; \
};

#include <string>

SINGLETONE(tokenOfAdmin,std::string)
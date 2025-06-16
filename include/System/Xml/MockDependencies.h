
#pragma once

#include <string>
#include <memory>
#include <vector>
#include <map>

// Mock implementations for CoreLib.Cpp dependencies
namespace System {
    using String = std::string;
    
    namespace GC {
        template<typename T>
        using GCPtr = std::shared_ptr<T>;
        
        template<typename T, typename... Args>
        GCPtr<T> New(Args&&... args) {
            return std::make_shared<T>(std::forward<Args>(args)...);
        }
    }
    
    namespace IO {
        class Stream {
        public:
            virtual ~Stream() = default;
        };
        
        class TextReader {
        public:
            virtual ~TextReader() = default;
        };
        
        class TextWriter {
        public:
            virtual ~TextWriter() = default;
        };
    }
    
    namespace Text {
        class StringBuilder {
        public:
            StringBuilder() = default;
            void Append(const String& str) { data_ += str; }
            String ToString() const { return data_; }
        private:
            String data_;
        };
        
        class Encoding {
        public:
            static std::shared_ptr<Encoding> UTF8() {
                static auto instance = std::make_shared<Encoding>();
                return instance;
            }
        };
    }
    
    namespace Collections {
        template<typename T>
        class List {
        private:
            std::vector<T> items_;
        public:
            int Count() const { return static_cast<int>(items_.size()); }
            void Add(const T& item) { items_.push_back(item); }
            T& operator[](int index) { return items_[index]; }
            const T& operator[](int index) const { return items_[index]; }
            
            typename std::vector<T>::iterator begin() { return items_.begin(); }
            typename std::vector<T>::iterator end() { return items_.end(); }
            typename std::vector<T>::const_iterator begin() const { return items_.begin(); }
            typename std::vector<T>::const_iterator end() const { return items_.end(); }
        };
    }
    
    namespace Reflection {
        class Type {
        public:
            String Name() const { return "MockType"; }
        };
    }
}

// RTTR mock
namespace rttr {
    namespace registration {
        template<typename T>
        struct class_ {
            class_(const char*) {}
            template<typename... Args>
            class_& constructor() { return *this; }
            template<typename F>
            class_& property(const char*, F) { return *this; }
            template<typename F>
            class_& property(const char*, F, F) { return *this; }
            template<typename F>
            class_& method(const char*, F) { return *this; }
        };
        
        template<typename T>
        struct enumeration {
            enumeration(const char*) {}
            template<typename V>
            enumeration& value(const char*, V) { return *this; }
        };
    }
    
    template<typename F>
    auto select_overload(F f) { return f; }
}

#define RTTR_REGISTRATION namespace { struct rttr_auto_register { rttr_auto_register() {
#define RTTR_REGISTRATION_END } }; static rttr_auto_register rttr_auto_register_instance; }

// Make types available in System::Xml namespace
namespace System {
namespace Xml {
    using System::String;
    template<typename T>
    using GCPtr = GCPtr<T>;
}
}

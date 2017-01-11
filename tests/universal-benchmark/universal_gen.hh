#ifndef _UNIVERSAL_GEN_HH
#define _UNIVERSAL_GEN_HH

#include <bitset>
#include <cstdint>
#include <memory>
#include <string>

/* A specialized functor for generating unique keys and values for various
 * types. Must define one for each type we want to use. */

template <typename T>
class Gen {
    // using storage_type = ...
    // static storage_type storage_key(uint64_t num)
    // static storage_type storage_value()
    // static T get(storage_type&)
};

template <>
class Gen<uint64_t> {
public:
    using storage_type = uint64_t;

    static storage_type storage_key(uint64_t num) {
        return num;
    }

    static storage_type storage_value() {
        return 0;
    }

    static uint64_t get(const storage_type& st) {
        return st;
    }
};

template <>
class Gen<std::string> {
    static constexpr size_t STRING_SIZE = 100;
public:
    using storage_type = std::string;

    static storage_type storage_key(uint64_t num) {
        return std::string(
            static_cast<const char*>(static_cast<const void*>(&num)),
            sizeof(num));
    }

    static storage_type storage_value() {
        return std::string(STRING_SIZE, '0');
    }

    static std::string get(const storage_type& st) {
        return st;
    }
};

// Should be 256B. Bitset is nice since it already has std::hash specialized.
using MediumBlob = std::bitset<2048>;

template <>
class Gen<MediumBlob> {
public:
    using storage_type = MediumBlob;

    static storage_type storage_key(uint64_t num) {
        return MediumBlob(Gen<uint64_t>::storage_key(num));
    }

    static storage_type storage_value() {
        return MediumBlob();
    }

    static MediumBlob get(const storage_type& st) {
        return st;
    }
};

// Should be 512B
using BigBlob = std::bitset<4096>;

template <>
class Gen<BigBlob> {
public:
    using storage_type = BigBlob;

    static storage_type storage_key(uint64_t num) {
        return BigBlob(Gen<uint64_t>::storage_key(num));
    }

    static storage_type storage_value() {
        return BigBlob();
    }

    static BigBlob get(const storage_type& st) {
        return st;
    }
};

template <typename T>
class Gen<T*> {
public:
    using storage_type = std::unique_ptr<T>;

    static storage_type storage_key(uint64_t num) {
        return storage_type(new T(Gen<T>::storage_key(num)));
    }

    static storage_type storage_value() {
        return storage_type(new T(Gen<T>::storage_value()));
    }

    static T* get(const storage_type& st) {
        return st.get();
    }
};

#endif // _UNIVERSAL_GEN_HH

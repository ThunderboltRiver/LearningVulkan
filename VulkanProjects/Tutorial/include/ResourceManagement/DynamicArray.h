//
// Created by 沖田大河 on 2026/04/19.
//

#ifndef TUTORIAL_DYNAMICARRAY_H
#define TUTORIAL_DYNAMICARRAY_H

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <new>
#include <stdexcept>
#include <type_traits>
#include <utility>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/mman.h>
#include <unistd.h>
#endif

namespace Tutorial::ResourceManagement {

    /**
     * OSのシステムコールを使用してメモリを動的に確保・拡張する動的配列テンプレートクラス。
     * 容量が不足した際にシステムコールを通じてメモリを再割り当てする。
     * バンプアロケータなど、大きなメモリブロックをオンデマンドで割り当てるためのバックボーンとして使用することを想定している。
     * @tparam T 格納する要素の型
     */
    template<typename T>
    class DynamicArray {
    public:
        using StorageType = std::remove_const_t<T>;

    private:

        /**
         * OSのページサイズを取得する
         * @return ページサイズ（バイト）
         */
        static std::size_t getPageSize() {
#ifdef _WIN32
            SYSTEM_INFO si;
            GetSystemInfo(&si);
            return si.dwPageSize;
#else
            return static_cast<std::size_t>(sysconf(_SC_PAGESIZE));
#endif
        }

        /**
         * 指定されたバイト数をページサイズの倍数に切り上げる
         * @param bytes 切り上げ対象のバイト数
         * @return ページサイズの倍数に切り上げられたバイト数
         */
        static std::size_t roundUpToPageSize(std::size_t bytes) {
            const std::size_t pageSize = getPageSize();
            // ページサイズが2のべき乗であることを検証する（ビット演算による切り上げの前提条件）
            if (pageSize == 0 || (pageSize & (pageSize - 1)) != 0) {
                throw std::runtime_error("DynamicArray: page size is not a power of 2");
            }
            return (bytes + pageSize - 1) & ~(pageSize - 1);
        }

        /**
         * OSのシステムコールを使用してメモリを割り当てる
         * @param bytes 割り当てるバイト数（ページサイズの倍数であること）
         * @return 割り当てられたメモリの先頭ポインタ
         */
        static void* osAllocate(std::size_t bytes) {
#ifdef _WIN32
            void* ptr = VirtualAlloc(nullptr, bytes, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
            if (ptr == nullptr) {
                throw std::bad_alloc();
            }
            return ptr;
#else
            void* ptr = mmap(nullptr, bytes, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
            if (ptr == MAP_FAILED) {
                throw std::bad_alloc();
            }
            return ptr;
#endif
        }

        /**
         * OSのシステムコールを使用してメモリを再割り当てする。
         * Linuxではmremapでインプレース拡張を試み、それ以外では新しい領域を確保してコピー後に旧領域を解放する。
         * この関数はtrivially copyableな型にのみ使用すること。
         * @param oldPtr 旧メモリ領域のポインタ
         * @param oldBytes 旧メモリ領域のバイト数
         * @param newBytes 新しいメモリ領域のバイト数
         * @return 新しいメモリ領域の先頭ポインタ
         */
        static void* osReallocate(void* oldPtr, std::size_t oldBytes, std::size_t newBytes) {
#if defined(__linux__)
            void* ptr = mremap(oldPtr, oldBytes, newBytes, MREMAP_MAYMOVE);
            if (ptr == MAP_FAILED) {
                throw std::bad_alloc();
            }
            return ptr;
#elif defined(_WIN32)
            void* newPtr = VirtualAlloc(nullptr, newBytes, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
            if (newPtr == nullptr) {
                throw std::bad_alloc();
            }
            std::memcpy(newPtr, oldPtr, oldBytes);
            VirtualFree(oldPtr, 0, MEM_RELEASE);
            return newPtr;
#else
            // Linux以外のPOSIX環境（macOSなど）
            void* newPtr = mmap(nullptr, newBytes, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
            if (newPtr == MAP_FAILED) {
                throw std::bad_alloc();
            }
            std::memcpy(newPtr, oldPtr, oldBytes);
            munmap(oldPtr, oldBytes);
            return newPtr;
#endif
        }

        /**
         * OSのシステムコールを使用してメモリを解放する
         * @param ptr 解放するメモリ領域のポインタ
         * @param bytes 解放するメモリ領域のバイト数
         */
        static void osFree(void* ptr, std::size_t bytes) {
#ifdef _WIN32
            (void)bytes;
            VirtualFree(ptr, 0, MEM_RELEASE);
#else
            munmap(ptr, bytes);
#endif
        }

        /**
         * 配列の先頭ポインタ
         */
        StorageType* _data;

        /**
         * 現在の要素数
         */
        std::size_t _size;

        /**
         * 割り当て済みの要素容量
         */
        std::size_t _capacity;

        /**
         * 実際にマッピングされたバイト数（munmap/VirtualFreeに必要）
         */
        std::size_t _mappedBytes;

        /**
         * 容量不足時にメモリを拡張する。容量を2倍に拡張し、既存の要素を新しい領域に移動する。
         */
        void grow() {
            const std::size_t newCapacity = (_capacity == 0) ? 1 : _capacity * 2;
            // 容量の倍増によるオーバーフローを検出する
            if (newCapacity < _capacity) {
                throw std::bad_alloc();
            }
            // 要素数とサイズの乗算によるオーバーフローを検出する
            if (newCapacity > SIZE_MAX / sizeof(StorageType)) {
                throw std::bad_alloc();
            }
            const std::size_t newBytes = roundUpToPageSize(newCapacity * sizeof(StorageType));
            const std::size_t actualNewCapacity = newBytes / sizeof(StorageType);

            if (_data == nullptr) {
                _data = static_cast<StorageType*>(osAllocate(newBytes));
                _mappedBytes = newBytes;
                _capacity = actualNewCapacity;
                return;
            }

            if constexpr (std::is_trivially_copyable_v<StorageType>) {
                // trivially copyableな型はmremap/memcpyで安全にバイト単位コピーできる
                void* newPtr = osReallocate(_data, _mappedBytes, newBytes);
                _data = static_cast<StorageType*>(newPtr);
            } else {
                // 非trivially copyableな型はムーブコンストラクタで要素を個別に移動する
                void* newRaw = osAllocate(newBytes);
                auto* newData = static_cast<StorageType*>(newRaw);
                for (std::size_t i = 0; i < _size; ++i) {
                    new (&newData[i]) StorageType(std::move(_data[i]));
                    _data[i].~StorageType();
                }
                osFree(_data, _mappedBytes);
                _data = newData;
            }

            _mappedBytes = newBytes;
            _capacity = actualNewCapacity;
        }

        /**
         * すべての生存中の要素のデストラクタを呼び出し、マッピングされたメモリを解放する
         */
        void destroyAndFree() {
            if (_data == nullptr) {
                return;
            }
            // 初期化された要素を逆順に破棄
            for (std::size_t i = _size; i > 0; --i) {
                _data[i - 1].~StorageType();
            }
            osFree(_data, _mappedBytes);
            _data = nullptr;
            _size = 0;
            _capacity = 0;
            _mappedBytes = 0;
        }

    public:

        /**
         * デフォルトコンストラクタ。空の動的配列を作成する。
         * メモリの割り当ては最初の要素が追加されるまで遅延される。
         */
        DynamicArray()
            : _data(nullptr), _size(0), _capacity(0), _mappedBytes(0) {
        }

        /**
         * 初期容量を指定して動的配列を作成する。
         * 指定された容量分のメモリをOSから確保するが、要素は初期化されない。
         * @param initialCapacity 初期容量（要素数）
         */
        explicit DynamicArray(std::size_t initialCapacity)
            : _data(nullptr), _size(0), _capacity(0), _mappedBytes(0) {
            if (initialCapacity > 0) {
                _mappedBytes = roundUpToPageSize(initialCapacity * sizeof(StorageType));
                _capacity = _mappedBytes / sizeof(StorageType);
                _data = static_cast<StorageType*>(osAllocate(_mappedBytes));
            }
        }

        /**
         * 要素をコピーして末尾に追加する。容量が不足している場合はメモリを拡張する。
         * @param value 追加する要素
         */
        void add(const T& value) {
            if (_size >= _capacity) {
                grow();
            }
            new (&_data[_size]) StorageType(value);
            ++_size;
        }

        /**
         * 要素をムーブして末尾に追加する。容量が不足している場合はメモリを拡張する。
         * @param value 追加する要素
         */
        void add(T&& value) {
            if (_size >= _capacity) {
                grow();
            }
            new (&_data[_size]) StorageType(std::move(value));
            ++_size;
        }

        /**
         * 末尾に要素をインプレースで構築する。容量が不足している場合はメモリを拡張する。
         * @tparam Args コンストラクタ引数の型
         * @param args コンストラクタに転送される引数
         * @return 構築された要素への参照
         */
        template<typename... Args>
        T& emplace(Args&&... args) {
            if (_size >= _capacity) {
                grow();
            }
            auto* ptr = new (&_data[_size]) StorageType(std::forward<Args>(args)...);
            ++_size;
            return *ptr;
        }

        /**
         * 指定されたインデックスの要素への参照を返す
         * @param index インデックス
         * @return 要素への参照
         */
        T& operator[](std::size_t index) {
            if (index >= _size) {
                throw std::out_of_range("DynamicArray: index out of range");
            }
            return _data[index];
        }

        /**
         * 指定されたインデックスの要素へのconst参照を返す
         * @param index インデックス
         * @return 要素へのconst参照
         */
        const T& operator[](std::size_t index) const {
            if (index >= _size) {
                throw std::out_of_range("DynamicArray: index out of range");
            }
            return _data[index];
        }

        /**
         * 現在の要素数を取得する
         * @return 要素数
         */
        [[nodiscard]] std::size_t size() const { return _size; }

        /**
         * 現在の容量（要素数）を取得する
         * @return 容量
         */
        [[nodiscard]] std::size_t capacity() const { return _capacity; }

        /**
         * 実際にマッピングされたバイト数を取得する
         * @return マッピングされたバイト数
         */
        [[nodiscard]] std::size_t mappedBytes() const { return _mappedBytes; }

        // イテレータサポート

        T* begin() { return _data; }
        const T* begin() const { return _data; }

        T* end() { return _data + _size; }
        const T* end() const { return _data + _size; }

        // コピー禁止。メモリマッピングの所有権を持つのは一つのインスタンスのみ
        DynamicArray(const DynamicArray& other) = delete;
        DynamicArray& operator=(const DynamicArray& other) = delete;

        /**
         * ムーブコンストラクタ。所有権をムーブ元からムーブ先に移動する。
         * ムーブ元は空の状態になり、デストラクタでは何も解放しない。
         */
        DynamicArray(DynamicArray&& other) noexcept
            : _data(other._data),
              _size(other._size),
              _capacity(other._capacity),
              _mappedBytes(other._mappedBytes) {
            other._data = nullptr;
            other._size = 0;
            other._capacity = 0;
            other._mappedBytes = 0;
        }

        /**
         * ムーブ代入演算子。既存の領域を解放し、所有権をムーブ元からムーブ先に移動する。
         * ムーブ元は空の状態になり、デストラクタでは何も解放しない。
         */
        DynamicArray& operator=(DynamicArray&& other) noexcept {
            if (this != &other) {
                destroyAndFree();
                _data = other._data;
                _size = other._size;
                _capacity = other._capacity;
                _mappedBytes = other._mappedBytes;
                other._data = nullptr;
                other._size = 0;
                other._capacity = 0;
                other._mappedBytes = 0;
            }
            return *this;
        }

        /**
         * デストラクタ。すべての要素を破棄し、マッピングされたメモリを解放する。
         */
        ~DynamicArray() {
            destroyAndFree();
        }
    };
}

#endif //TUTORIAL_DYNAMICARRAY_H

# OSメモリシステムコール解説: `mmap` / `VirtualAlloc` vs `malloc`

このドキュメントでは、`DynamicArray<T>`で使用しているOSレベルのメモリ管理システムコール（`mmap`、`VirtualAlloc`）について、
標準ライブラリの`malloc`や`_aligned_malloc`との違い、使用理由、およびmacOS上での動作について説明する。

---

## 1. `malloc` / `_aligned_malloc` とは

### `malloc`
- C標準ライブラリが提供する汎用メモリ割り当て関数。
- 内部でOSのシステムコール（`brk`/`sbrk`や`mmap`など）を呼び出してメモリを確保するが、その詳細はユーザから隠蔽されている。
- 小さなブロックの割り当て・解放を効率よく行うためにフリーリスト等のデータ構造を内部に持つ。
- `free()`で解放しても、OSにメモリが即座に返されるとは限らない（ランタイムが再利用のためにキャッシュすることがある）。

### `_aligned_malloc`（Windows固有）
- 指定したアライメント境界にメモリを割り当てる`malloc`の亜種。
- POSIX環境では`posix_memalign`や`aligned_alloc`(C11)が同等の機能を提供する。
- `malloc`と同様、内部では汎用ヒープアロケータを通じてメモリを管理する。

**`malloc`系の特徴まとめ:**
| 項目 | `malloc` / `_aligned_malloc` |
|---|---|
| 粒度 | バイト単位（任意サイズ） |
| メモリソース | ランタイムのヒープ（内部でOSコールを使用） |
| オーバーヘッド | ヒープ管理メタデータ（ヘッダ、フリーリスト等） |
| 解放時の挙動 | ランタイムがキャッシュ。OSに即座には返さないことが多い |
| アドレス空間の制御 | なし（ランタイム任せ） |

---

## 2. `mmap` とは（POSIX）

`mmap`はPOSIXオペレーティングシステムが提供するシステムコールで、**プロセスの仮想アドレス空間にメモリ領域を直接マッピング**する。

```c
#include <sys/mman.h>

void* mmap(
    void*  addr,    // 割り当て先アドレスのヒント（通常はnullptr）
    size_t length,  // 割り当てるバイト数
    int    prot,    // メモリ保護属性（PROT_READ | PROT_WRITE 等）
    int    flags,   // 割り当てフラグ（MAP_PRIVATE | MAP_ANONYMOUS 等）
    int    fd,      // ファイルディスクリプタ（匿名マッピングの場合は -1）
    off_t  offset   // ファイルオフセット（匿名マッピングの場合は 0）
);
```

### 主な用途
1. **ファイルマッピング**: ファイルの内容をメモリに直接マッピングし、`read`/`write`システムコールなしにアクセスする。
2. **匿名マッピング**（`MAP_ANONYMOUS`）: ファイルに紐づかないゼロ初期化されたメモリ領域を確保する。`DynamicArray`ではこちらを使用している。

### 解放: `munmap`

```c
int munmap(void* addr, size_t length);
```

`munmap`は指定されたメモリ領域をプロセスのアドレス空間から解除し、**直ちにOSに物理メモリを返す**。

### Linux固有: `mremap`

```c
#include <sys/mman.h>

void* mremap(
    void*  old_address,  // 既存のマッピングの先頭アドレス
    size_t old_size,     // 既存のマッピングのサイズ
    size_t new_size,     // 新しいサイズ
    int    flags         // MREMAP_MAYMOVE 等
);
```

`mremap`は**Linux固有**のシステムコールで、既存のメモリマッピングのサイズを変更する。
- `MREMAP_MAYMOVE`フラグにより、仮想アドレス空間に余裕がある場合はアドレスを変えずにインプレースで拡張できる。
- インプレース拡張が可能な場合、データのコピーは一切発生しないため非常に高速。
- インプレース拡張が不可能な場合は、新しいアドレスにマッピングを移動する（カーネルがページテーブルを書き換えるだけなので、ユーザ空間でのデータコピーは不要）。

---

## 3. `VirtualAlloc` とは（Windows）

`VirtualAlloc`はWindows APIが提供するメモリ管理関数で、**仮想アドレス空間を直接操作**する。

```c
#include <windows.h>

LPVOID VirtualAlloc(
    LPVOID lpAddress,         // 割り当て先アドレス（通常はNULL）
    SIZE_T dwSize,            // 割り当てるバイト数
    DWORD  flAllocationType,  // 割り当て種別（MEM_RESERVE, MEM_COMMIT 等）
    DWORD  flProtect          // メモリ保護属性（PAGE_READWRITE 等）
);
```

### 2段階のメモリ管理
Windowsの仮想メモリシステムは**予約（Reserve）**と**コミット（Commit）**の2段階モデルを採用している：

1. **`MEM_RESERVE`**: 仮想アドレス空間を予約するが、物理メモリやページファイルの割り当ては行わない。
2. **`MEM_COMMIT`**: 予約された領域に物理メモリ（またはページファイル）を割り当てる。

`DynamicArray`では`MEM_COMMIT | MEM_RESERVE`を同時に指定して、予約とコミットを一度に行っている。

### 解放: `VirtualFree`

```c
BOOL VirtualFree(
    LPVOID lpAddress,    // 解放するメモリの先頭アドレス
    SIZE_T dwSize,       // 解放するバイト数（MEM_RELEASEの場合は0）
    DWORD  dwFreeType    // MEM_RELEASE または MEM_DECOMMIT
);
```

`MEM_RELEASE`で呼び出すと、予約されたアドレス空間全体を解放し、物理メモリをOSに返す。

---

## 4. `malloc` との主要な違い

| 比較項目 | `malloc` / `_aligned_malloc` | `mmap` / `VirtualAlloc` |
|---|---|---|
| **抽象レベル** | Cランタイムライブラリ（ユーザ空間） | OSカーネルのシステムコール |
| **メモリ粒度** | バイト単位（任意サイズ） | ページ単位（通常4096バイト） |
| **割り当て単位** | 小さいブロックに最適化 | 大きなブロック向け |
| **メタデータ** | ヒープ管理のオーバーヘッドあり | オーバーヘッドなし（カーネルが管理） |
| **解放時** | ランタイムがキャッシュ、OSに返さないことがある | `munmap`/`VirtualFree`で**即座にOSに返る** |
| **ゼロ初期化** | 保証なし（`calloc`を除く） | `mmap`(匿名)と`VirtualAlloc`はゼロ初期化保証 |
| **アドレス空間制御** | 不可 | 予約・コミットの分離が可能（`VirtualAlloc`） |
| **インプレース拡張** | `realloc`はコピーが必要な場合がある | `mremap`(Linux)はコピーなしで拡張できる可能性がある |
| **アライメント** | `malloc`は通常`max_align_t`に揃う | ページ境界（4096バイト等）に常に揃う |

### 図解: メモリ割り当ての流れ

```
malloc の場合:
  ユーザコード → malloc() → Cランタイムヒープ → [内部で mmap/brk] → カーネル
                              ↑
                        ヒープ管理メタデータ
                        フリーリスト等

mmap / VirtualAlloc の場合:
  ユーザコード → mmap() / VirtualAlloc() → カーネル（直接）
                                            ↑
                                   中間レイヤーなし
```

---

## 5. `DynamicArray`で`mmap`/`VirtualAlloc`を使用する理由

### 5.1. バンプアロケータのバックボーンとしての適性

`DynamicArray`は将来的にバンプアロケータ（リニアアロケータ）の基盤として使用することを想定している。
バンプアロケータは大きなメモリブロックをOSから直接確保し、その中で個々のオブジェクトを先頭から順に配置していく。
この用途では：

- **大きな連続メモリブロック**が必要 → `mmap`/`VirtualAlloc`はページ単位で大きなブロックを効率的に確保できる
- **ヒープのフラグメンテーションを回避**したい → `malloc`のフリーリストを経由しないため、フラグメンテーションが発生しない
- **解放時に確実にOSに返したい** → `munmap`/`VirtualFree`は即座にメモリをOSに返す

### 5.2. `mremap`によるインプレース拡張の利点

Linux上では`mremap`により、配列の拡張時にデータのコピーを回避できる可能性がある。
通常の`realloc`（`malloc`系）では拡張時にほぼ必ずデータのコピーが発生するが、
`mremap`はカーネルのページテーブル操作のみで拡張を実現できるため、
大量のデータを保持する配列の拡張が非常に高速になる。

### 5.3. `PlacementStackAllocator`の制約を超えるため

現在のコードベースでは`Span<T>`が`PlacementStackAllocator`を使用しているが、
これは固定サイズのスタック領域（`malloc`で確保された12MBブロック）に制限されている。
`PlacementStackAllocator.h`と`Span.h`の両方にTODOコメントでこの制約が記されており、
`DynamicArray`はその制約を解消するための第一歩となる。

### 5.4. メモリのオーバーヘッド削減

`malloc`は各割り当てにヒープ管理のためのメタデータ（通常16〜32バイト）を付加する。
`mmap`/`VirtualAlloc`はカーネルがページテーブルでメモリを管理するため、
ユーザ空間でのメタデータオーバーヘッドがない。
大きなメモリブロックを少数回だけ確保する用途ではこの差が有利に働く。

---

## 6. macOSでの`mmap`の動作

### `mmap`はmacOS上で動作する

`mmap`はPOSIX標準のシステムコールであり、**macOS（Darwin）でも完全に動作する**。
macOSはBSD系のカーネル（XNU）を使用しており、POSIX互換のシステムコールを広くサポートしている。

`DynamicArray`で使用している以下のフラグ・関数はすべてmacOS上で利用可能：

| 機能 | macOS対応 | 備考 |
|---|---|---|
| `mmap` with `MAP_PRIVATE \| MAP_ANONYMOUS` | ✅ 対応 | `MAP_ANONYMOUS`はmacOSでは`MAP_ANON`としても定義されている |
| `munmap` | ✅ 対応 | POSIX標準 |
| `mprotect` | ✅ 対応 | POSIX標準 |
| `mremap` | ❌ **非対応** | Linux固有のシステムコール |
| `sysconf(_SC_PAGESIZE)` | ✅ 対応 | POSIX標準 |

### macOSにおける`mremap`の非対応

`mremap`はLinuxカーネル固有の拡張であり、macOS（およびFreeBSD等のBSD系OS）には存在しない。
そのため、`DynamicArray`のコードではプリプロセッサ分岐で対応している：

```cpp
#if defined(__linux__)
    // Linux: mremap でインプレース拡張を試みる
    void* ptr = mremap(oldPtr, oldBytes, newBytes, MREMAP_MAYMOVE);
#elif defined(_WIN32)
    // Windows: VirtualAlloc + memcpy + VirtualFree
    ...
#else
    // macOS等のPOSIX環境: mmap（新規確保）+ memcpy + munmap（旧領域解放）
    void* newPtr = mmap(nullptr, newBytes, PROT_READ | PROT_WRITE,
                        MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    std::memcpy(newPtr, oldPtr, oldBytes);
    munmap(oldPtr, oldBytes);
#endif
```

macOS上では`mremap`が使えないため、新しいメモリ領域を`mmap`で確保し、
既存データを`memcpy`でコピーしてから旧領域を`munmap`で解放するフォールバック処理を行う。
これは`realloc`の内部動作と概念的に同等だが、ヒープ管理のオーバーヘッドがない点が異なる。

---

## 7. まとめ

| 質問 | 回答 |
|---|---|
| `mmap`/`VirtualAlloc`とは？ | OSカーネルが提供するメモリマッピングのシステムコール。仮想アドレス空間を直接操作する |
| `malloc`との違いは？ | `malloc`はCランタイムのヒープマネージャを経由する。`mmap`/`VirtualAlloc`はカーネルに直接要求する |
| なぜ`malloc`ではなくこれらを使用するのか？ | バンプアロケータの基盤として大きなブロックを効率的に管理し、ヒープフラグメンテーションを回避し、`mremap`(Linux)でコピーなし拡張を実現するため |
| `mmap`はmacOSで動作するか？ | ✅ 動作する（POSIX標準）。ただし`mremap`はLinux固有のため非対応。macOSではフォールバック処理で対応 |

---

## 参考リソース

- [Linux man page: mmap(2)](https://man7.org/linux/man-pages/man2/mmap.2.html)
- [Linux man page: mremap(2)](https://man7.org/linux/man-pages/man2/mremap.2.html)
- [Microsoft: VirtualAlloc function](https://learn.microsoft.com/en-us/windows/win32/api/memoryapi/nf-memoryapi-virtualalloc)
- [Apple: mmap(2)](https://developer.apple.com/library/archive/documentation/System/Conceptual/ManPages_iPhoneOS/man2/mmap.2.html)
- [glibc malloc internals](https://sourceware.org/glibc/wiki/MallocInternals)

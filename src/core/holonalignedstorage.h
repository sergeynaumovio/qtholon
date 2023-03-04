// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONALIGNEDSTORAGE_H
#define HOLONALIGNEDSTORAGE_H

#include <QtCore/qglobal.h>

template<typename T, int HolonAlignedStorageSize, int HolonAlignedStorageAlignment = sizeof(ptrdiff_t)>
class HolonAlignedStorage
{
    Q_DISABLE_COPY_MOVE(HolonAlignedStorage)

    std::aligned_storage_t<HolonAlignedStorageSize, HolonAlignedStorageAlignment> d_storage;

    T *d_func() const noexcept { return reinterpret_cast<T *>((void *)(&d_storage)); }

    template<int TSize, int TAlignment>
    void checkSize() noexcept
    {
        static_assert(HolonAlignedStorageSize == TSize, "HolonAlignedStorageSize and sizeof(T) mismatch");
        static_assert(HolonAlignedStorageAlignment == TAlignment, "HolonAlignedStorageAlignment and alignof(T) mismatch");
    }

public:
    template<typename... Args>
    explicit HolonAlignedStorage(Args&&... args)
    {
        checkSize<sizeof(T), alignof(T)>();
        new (d_func()) T(std::forward<Args>(args)...);
    }

    ~HolonAlignedStorage() noexcept { d_func()->~T(); }

    T *operator->() const noexcept { return d_func(); }
    T &operator*() const noexcept { return *d_func(); }
};

#endif // HOLONALIGNEDSTORAGE_H

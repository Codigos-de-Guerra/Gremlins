/**
 * @file mempool_common.hpp
 * @version 1.0
 * @since Jun, 13. 
 * @date Jun, 26.
 * @author Oziel Alves (ozielalves@ufrn.edu.br)
 * @author Daniel Guerra (daniel.guerra13@hotmail.com)
 * @title gm::Common Operators and the Tag structure 
 */

#ifndef _MEMPOOL_COMMON_HPP_
#define _MEMPOOL_COMMON_HPP_

#include <cstdio>  // std::size_t
#include "storage_pool.hpp"

/**
 *  @brief The Tag structure has pointer that is a mark that points to a GM 
    owner of the lended memory
 */ 

//namespace gm
//{
    typedef std::size_t size_type;

    struct Tag {
        StoragePool *pool;  //!< A reference to the Pool
    };

    void *operator new(size_type bytes, StoragePool &p) {
    
        Tag * const tag = reinterpret_cast<Tag *>(p.Allocate(bytes + sizeof(Tag)));
        tag->pool = &p;
        // skip sizeof tag to get the raw data - block.
        return reinterpret_cast<void *>(tag + 1U);
    }

    void *operator new[](size_type bytes, StoragePool &p) {
    
        Tag * const tag = reinterpret_cast<Tag *>(p.Allocate(bytes + sizeof(Tag)));
        tag->pool = &p;
        // skip sizeof tag to get the raw data - block.
        return reinterpret_cast<void *>(tag + 1U);
    }

    void *operator new(size_type bytes) {  // Regular new
    
        Tag *const tag = reinterpret_cast<Tag *>(std::malloc(bytes + sizeof(Tag)));
        tag->pool = nullptr;
        // skip sizeof tag to get the raw data - block.
        return reinterpret_cast<void *>(tag + 1U);
    }

    void *operator new[](size_type bytes) {  // New []
    
        Tag *const tag = reinterpret_cast<Tag *>(std::malloc(bytes + sizeof(Tag)));
        tag->pool = nullptr;
        // skip sizeof tag to get the raw data - block.
        return reinterpret_cast<void *>(tag + 1U);
    }

    void operator delete(void *arg) noexcept {
        // We need to subtract 1U (in fact, pointer arithmetics) because arg
        // points to the raw data (second block of information).
        // The pool id (tag) is located "sizeof (Tag)" bytes before.
        Tag * const tag = reinterpret_cast<Tag *>(arg) - 1U;
        if (nullptr != tag->pool)  // Memory block belongs to a particular GM.
            tag->pool->Free(tag);
        else
            std::free(tag);  // Memory block belongs to the operational system.
    }

    void operator delete[](void *arg) noexcept {
        // We need to subtract 1U (in fact, pointer arithmetics) because arg
        // points to the raw data (second block of information).
        // The pool id (tag) is located "sizeof (Tag)" bytes before.
        Tag * const tag = reinterpret_cast<Tag *>(arg) - 1U;
        if (nullptr != tag->pool)  // Memory block belongs to a particular GM.
            tag->pool->Free(tag);
        else
            std::free(tag);  // Memory block belongs to the operational system.
    }
//}

#endif

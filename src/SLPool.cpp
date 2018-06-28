/**
 * @file SLPool.cpp
 * @version 1.0
 * @since Jun, 22. 
 * @date Jun, 25.
 * @author Oziel Alves (ozielalves@ufrn.edu.br)
 * @author Daniel Guerra (daniel.guerra13@hotmail.com)
 * @title gm::SLPool Class 
 */

#include <iostream>

#include <cmath>    // To std::ceil
#include <cstdio>   // To std::size_t
#include <string>   // To std::string
#include <new>      // To std::bad_alloc
#include "SLPool.hpp"

using namespace gm;

typedef unsigned int uint;
typedef std::size_t size_type;
typedef std::string string;

/**
 * @brief gm::SLPool class implementation.
 */

SLPool::SLPool(size_type _b) :
    m_n_blocks( std::ceil( static_cast<float>( _b)/Block::BlockSize ) + 1 ),
    m_pool( new Block[m_n_blocks] ),
    m_sentinel( m_pool[m_n_blocks - 1] ) {
    
    	// Sets the first m_pool element values
    	m_pool[0].m_length = m_n_blocks - 1;
    	m_pool[0].m_next = nullptr;

    	// Makes m_sentinel points to the first m_pool element
    	m_sentinel.m_next = m_pool;
}

SLPool::~SLPool() {
    delete[] m_pool;
}

void *SLPool::Allocate(size_type _b) {
    
    unsigned n_blocks = std::ceil(static_cast<float>(_b)/Block::BlockSize);
    Block *pos = m_sentinel.m_next;
    Block *prev_pos = &m_sentinel;

    while ( pos != nullptr ) {
        if ( pos->m_length >= n_blocks ) {

			// The pine Block have the same size that the client needs
			if ( pos->m_length == n_blocks ) {
            	prev_pos->m_next = pos->m_next;
            }
			else {
            	prev_pos->m_next = pos + n_blocks;
            	prev_pos->m_next->m_next = pos->m_next;
            	prev_pos->m_next->m_length = pos->m_length - n_blocks;
            	pos->m_length = n_blocks;
            }
            return reinterpret_cast< void * >(reinterpret_cast< Header * >(pos)+1U);
        }
        prev_pos = pos;
        pos = pos->m_next;
    }

    throw(std::bad_alloc());
}

void *SLPool::AllocateBF(size_type _b) {
    
    unsigned n_blocks = std::ceil(static_cast< float >(_b)/Block::BlockSize);
    Block *pos = m_sentinel.m_next;
    Block *prev_pos = &m_sentinel;
    Block *prev_best = nullptr;
    Block *best = nullptr;

    while ( pos != nullptr ) {
        if ( pos->m_length >= n_blocks ) {
            
            // The pine Block have the same size that the client needs
            if ( pos->m_length == n_blocks ) {
            	
            	prev_pos->m_next = pos->m_next;
                return reinterpret_cast<void *>(reinterpret_cast<Header *>(pos)+1U);
            
            } else if (best == nullptr or best->m_length > pos->m_length) {
				best = pos;
				prev_best = prev_pos;
            }
        }
        prev_pos = pos;
        pos = pos->m_next;
    }

    if (best != nullptr) {
        
        // A MEU DEUS QUE CONFUSAO EU NEM SEI MASI
		// NANNIIIII!?!?
        prev_best->m_next = best + n_blocks;
        prev_best->m_next->m_next = best->m_next;
        prev_best->m_next->m_length = best->m_length - n_blocks;
        best->m_length = n_blocks;
        
        return reinterpret_cast<void *>(reinterpret_cast<Header *>(best)+1U);
    }

    throw(std::bad_alloc());
}

void SLPool::Free(void *_p) {
    
    auto *BEGIN = reinterpret_cast<Block *>(reinterpret_cast<Header *>(_p)-1U);
    auto *pos = m_sentinel.m_next;
    auto *p_pos = &m_sentinel;

    while (pos != nullptr) {
        if (pos <= BEGIN) {
            p_pos = pos;
            pos = pos->m_next;
            continue;
        }
        if (p_pos + p_pos->m_length == BEGIN and BEGIN + BEGIN->m_length == pos) {
            p_pos->m_length += BEGIN->m_length + pos->m_length;
            BEGIN->m_length = 0;
            pos->m_length = 0;
            p_pos->m_next = pos->m_next;
        } else if (p_pos + p_pos->m_length == BEGIN) {
            p_pos->m_length += BEGIN->m_length;
            BEGIN->m_length = 0;
        } else if (BEGIN + BEGIN->m_length == pos) {
            BEGIN->m_length += pos->m_length;
            pos->m_length = 0;
            BEGIN->m_next = pos->m_next;
            p_pos->m_next = BEGIN;
        } else {
            p_pos->m_next = BEGIN;
            BEGIN->m_next = pos;
        }
        return;
    }
    if (pos == nullptr) {

        BEGIN->m_next = nullptr;
		if( p_pos + p_pos->m_length == BEGIN ){
			p_pos->m_length += BEGIN->m_length;

			BEGIN->m_length = 0;
		} else {
        	p_pos->m_next = BEGIN;
		}
    }
}

void SLPool::view( ) {
	
	auto *pt = m_sentinel.m_next;
	auto pos = 0u;
	
	std::string buffer;
	int count = 0;

	while (pos < m_n_blocks - 1) {
		
        auto aut = (m_pool + pos)->m_length;
		pos += aut;
		
        if (m_pool + pos - aut == pt) {
			std::cout << "[ ";
			while (aut-- > 0){
				std::cout << "+";
				count++;
			}
			std::cout << " ] ";
			pt = pt->m_next;
			buffer = buffer + "+[" + std::to_string(count) + "] ";
			count = 0;
		}
		else {
			std::cout << "[ " << string(aut, '#') << " ] ";
			buffer = buffer + "-[" + std::to_string(aut) + "] ";
		}
	}
	std::cout << "\n" << buffer << "|| Total blocks: " << m_n_blocks-1 << "\n";
}

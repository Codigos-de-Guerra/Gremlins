/**
 * @file SLPool.cpp
 * @version 1.0
 * @since Jun, 13. 
 * @date Jun, 25.
 * @author Oziel Alves (ozielalves@ufrn.edu.br)
 * @author Daniel Guerra (daniel.guerra13@hotmail.com)
 * @title gm::SLPool Class 
 */

#include <iostream>
#include <string>   // To std::string
#include <cmath>    // To std::ceil
//#include <cstdio>  // To std::size_t
#include <new>      // To std::bad_alloc
#include "SLPool.hpp"

using namespace gm;

typedef unsigned int uint;
typedef T size_type;

/**
 * @brief gm::SLPool class implementation.
 */

template< typename T >
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

template< typename T >
SLPool::~SLPool() {
    delete[] m_pool;
}

template< typename T >
void *SLPool::Allocate(size_type _b) {
    
    unsigned n_blocks = std::ceil(static_cast<float>(_b)/Block::BlockSize);
    Block *pos = m_sentinel.m_next;
    Block *prev_pos = &m_sentinel;

    while ( pos != nullptr ) {
        if ( pos->m_length >= n_blocks ) {

        	// The current Block have the same size that the client needs
            if ( pos->m_length == n_blocks ) {
            	prev_pos->m_next = pos->m_next;
            } else {
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

template< typename T >
void *SLPool::AllocateBF(size_type _b) {
}
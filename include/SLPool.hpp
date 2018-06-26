/**
 * @file SLPool.hpp
 * @version 1.0
 * @since Jun, 13. 
 * @date Jun, 25.
 * @author Oziel Alves (ozielalves@ufrn.edu.br)
 * @author Daniel Guerra (daniel.guerra13@hotmail.com)
 * @title gm::SLPool Class 
 */

/* "Para a implementação deste projeto é obrigatório a 
	implementação da sua própria lista encadeada." 	*/

#ifndef _SLPOOL_HPP_
#define _SLPOOL_HPP_

#include "storagepool.hpp"

/**
 * @brief The SLPool Class prototype
 */

namespace gm
{
	typedef unsigned int uint;
	typedef std::size_t size_type;

	template< typename T >
	class SLPool : public StoragePool {
 
	 public:
    	/**
    	 * @brief SLPool constructor
    	 */
    	explicit SLPool(size_type _b);

    	/**
    	 * @brief SLPool destructor
    	 */
    	~SLPool( );

    	/**
    	 * @brief Allocate memory
    	 * @param _b Number of bytes to be allocated
    	 * @return A pointer to the beggining of the allocated area
    	 */
    	void *Allocate(size_type _b);

    	/**
    	 * @brief Allocate memory using the Best Fit algorithm
    	 * @param _b Number of bytes to be allocated
    	 * @return A pointer to the beggining of the allocated area
    	 */
    	void *AllocateBF(size_type _b);

    	/**
    	 * @brief Free Memory
   		 * @param _p A pointer to element to be freed
    	 */
    	void Free(void *_p);

    	/**
    	 * @brief Function to show a visual representation from memory Blocks
    	 */
    	void view( );

    	/**
    	 * @brief The header of the memory block
    	 */
    	struct Header {
        	
        	uint m_length;  //!< The block's size
        	
        	//! Header Constructor
        	Header( ) : m_length(0u) { /*Empty*/ }
    	};

    	/**
    	 * @brief The reserved memory (or a pointer to another block)
    	 */
    	struct Block : public Header {
        	
        	//! A enum with the memory block size
        	enum {
            
            	BlockSize = 16  // Each block has 16 bytes.
        	};

        	//! A union with a pointer to the next block or the allocated memory
        	union {
            	
            	Block *m_next;  //!< A pointer to the next block
            	
            	//! The allocated memory
            	char m_raw[ BlockSize - sizeof(Header) ]; // Client's raw area
        	};

        	//! The Block constructor
        	Block( ) : Header( ), mp_Next(nullptr) { /*Empty*/ };
    	};

	 private:
    	uint m_n_blocks;			//!< The number of blocks
    	Block *m_pool;				//!< Head of list.
    	Block &m_sentinel;			//!< End of the list.
};

#endif
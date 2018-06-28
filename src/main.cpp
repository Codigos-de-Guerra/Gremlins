/**
 * @file main.hpp
 * @version 1.0
 * @since Jun, 22. 
 * @date Jun, 27.
 * @author Oziel Alves (ozielalves@ufrn.edu.br)
 * @author Daniel Guerra (daniel.guerra13@hotmail.com)
 * @title Driver
 */

#include <iostream>
// #include <vector> // NÃO PODE - Deletar
#include <fstream>
#include <cstdlib>	// rand_r()
#include <random>	// std::random_device
#include <cmath>	// std::ceil
#include <cassert>	// assert
#include <chrono>	// std::chrono
#include <ctime>	// std::time_t
#include <string>	// std::string
#include <queue>	// std::priority_queue

#include "../include/SLPool.hpp"
#include "../include/mempool_common.hpp"

typedef std::time_t tempo;
typedef std::string string;

using namespace gm;

/**
 * @brief Event Class
 */

class Event
/*{{{*/
{
	public:
        /**
         * @brief Event default constructor
         */
        Event( ) { /*Empty*/ }
    
        /**
         * @brief Event Attibution constructor
         * @param _p A pointer
         * @param _t A time-stamp
         */
        Event(void *_p, tempo _t) {
            m_ptr = _p;
            m_time = _t;
        }
    
        /**
         * @brief Gets the Event time-stamp
         * @return The time-stamp
         */
        tempo getTimeStamp( ) {
            return m_time;
        }
    
        /**
         * @brief Gets the pointer
         * @return The pointer
         */
        void *getMemoryPtr( ) {
            return m_ptr;
        }
    
        /**
         * @brief Overloads the < operator
         * @param _e The another event to be comparised
         * @return True or False, comparing by the time-stamp
         */
        bool operator<(const Event &_e) const {
            return m_time < _e.m_time;
        }
    
	private:
        void *m_ptr;	//!< The Event pointer
        tempo m_time;	//!< The Event time-stamp
};
/*}}}*/

/**
 * @brief Allocation test in a limited time
 * @param _pool A pointer to the pool to be used
 * @param _timeLimit The time limit
 */
void StoragePoolTest(StoragePool *_pool, tempo _timeLimit)
/*{{{*/
{
    // [1] Setup random numbers generator for memory size, say [100,2000] bytes.
    std::random_device rdm;
    auto ms_max = 2000u, ms_min = 100u;
    
    // [2] Setup random numbers generator for time intervals, say [1,100] units.
    auto ti_max = 100u, ti_min = 1u;
    
    // [3] Create the pirority queue std::priority_queue 'pq' and
    //     insert some events comprising the simulation.
    std::priority_queue< Event > pq;
    
    // Assuming there is a class Event that creates a pair address/time stamp.

    // Run simulation fot the time set by the client.
    for ( tempo t(0); t < _timeLimit; t++ ) {
        
        // Run while we have events pending or time to run.
        while ( !pq.empty( ) ){
            
            // Access the event with the smallest time-stamp.
            Event ev = pq.top( );
            
            // Do nothing. Still some time left.
            if ( ev.getTimeStamp( ) > t ) break;
            
            // When we got here, the top event has run out of time.
            // Remove event from priority queue.
            pq.pop( );
            
            // Calling free operator.
            _pool->Free( ev.getMemoryPtr( ) );
        }
        
        auto memorySize = rdm( )%( ms_max - ms_min + 1 ) + ms_min;
        void *const add = _pool->Allocate(memorySize);
        auto timeSpent = rdm( )%( ti_max - ti_min + 1 ) + ti_min;
        
        // Set time stamp some time from now.
        tempo releaseTime = t + timeSpent;
        
        // Creating a new simulation event.
        pq.push( Event(add, releaseTime) );
        
        // Show Memory Map on Screen.
        _pool->view( );
    }
}

/*}}}*/

int main(/* int argc, char **argv */)
{
/*{{{*/
	/*{
		StoragePool *pool = new gm::SLPool(300);

    	int *allocs[7];

    	// Shows the initial state from memory pool
    	std::cout << "~ Current State ~\n" << string(4, ' ');
    	pool.view( );

    	// Allocate 10 integers 3 times to test allocation on begining of pool
    	for ( int i = 0; i < 3; i++ ) {
        	allocs[i] = new( pool ) int[10];
        	std::cout << "\n[" << i << "] <<< Allocating integers\n" << string(8, ' ');
        	pool.view( );
    	}

    	// Unit tests (testing assignment)
    	for ( int i = 0; i < 10; i++ ) {
        	allocs[0][i] = i;
        	allocs[1][i] = i;
        	allocs[2][i] = i;
        	assert( allocs[0][i] == i );
        	assert( allocs[1][i] == i );
        	assert( allocs[2][i] == i );
   		}

    	// Free the first allocation (no merge)
    	delete allocs[0];
    	std::cout << "\n[0] <<< Free allocation:\n" << string(8, ' ');
    	pool.view( );

    	// Free the third allocation (merge on right side)
    	delete allocs[2];
    	std::cout << "\n[2] <<< Free allocation:\n" << string(8, ' ');
    	pool.view( );

    	// Allocate 30 integers (allocation after another element and empty spaces)
    	allocs[3] = new(pool) int[30];
    	std::cout << "\n[3] <<< Allocate 30 integers:\n" << string(8, ' ');
    	pool.view( );

    	// Unit tests (testing assignment)
    	for ( int i = 0; i < 30; i++ ) {
        	allocs[3][i] = i;
        	assert( allocs[3][i] == i );
    	}

    	// Free the second allocation (merge on left side)
    	delete allocs[1];
    	std::cout << "\n[1] <<< Free allocation:\n" << string(8, ' ');
    	pool.view( );

    	// Allocate 1 integer 2 times (at the beginning of the pool)
    	for ( int i = 4; i < 6; i++ ) {
        	allocs[i] = new(pool) int[1];
        	std::cout << "\n[" << i << "] <<< Allocate 1 integer:\n" << string(8, ' ');
        	pool.view( );
    	}

    	// Unit tests (testing assignment)
    	for ( int i = 0; i < 1; i++ ) {
        	allocs[4][i] = i;
        	allocs[5][i] = i;
        	assert( allocs[4][i] == i );
        	assert( allocs[5][i] == i) ;
    	}

    	// Free allocs[4] (no merge)
    	delete allocs[4];
    	std::cout << "\n[4] <<< Free allocation:\n" << string(8, ' ');
    	pool.view( );

    	// Free allocs[3] (merge on both sides)
    	delete allocs[3];
    	std::cout << "\n[3] <<< Free allocation:\n" << string(8, ' ');
    	pool.view( );

    	// Allocate 65 integers (fitting all the right area on pool)
    	allocs[6] = new(pool) int[65];
    	std::cout << "\n[6] <<< Allocate 65 integers:\n" << string(8, ' ');
    	pool.view( );

    	// Unit tests (testing assignment)
    	for (int i = 0; i < 65; i++) {
        	allocs[6][i] = i;
        	assert( allocs[6][i] == i );
    	}

    	// Free allocs[6] (no merge)
    	delete allocs[6];
    	std::cout << "\n[6] <<< Free allocation:\n" << string(8, ' ');
    	pool.view();

    	// Free allocs[5] (merge on both sides)
    	delete allocs[5];
    	std::cout << "\n[5] <<< Free allocation:\n" << string(8, ' ');
    	pool.view( );

    	delete pool;
	}*/
/*}}}*/

/*------------------------------ Time Counting------------------------------*/ 
/*Time Testing{{{*/
{
    // Creates the pool
    StoragePool *pool = new SLPool(2048);

    // The initial time
    auto s = std::chrono::steady_clock::now( );
    
    // Run the function to tests
    StoragePoolTest(pool, 1);
    
    // The final time
    auto e = std::chrono::steady_clock::now( );
    
    // Calculates the difference
    auto diff = std::chrono::duration<double, std::nano>(e - s).count( );

    std::cout << ">>> Time spent: " << diff << " ns\n";

    // Deletes the pool
    delete pool;
}

{
	std::chrono::steady_clock::time_point start, end;
    auto time_spent = 0.0l;
    int times = 100000, *al1 = nullptr, *al2 = nullptr;
    SLPool pool(400);

    // Average Time with the Memory Manager
    for ( int i = 0; i < times; i++ ) {
        start = std::chrono::steady_clock::now( );
        for (int j = 1; j <= 10; j += 2) {
            al1 = new(pool) int[j];
            al2 = new(pool) int[j+1];
            delete[] al1;
            delete[] al2;
        }

        // The final time
        end = std::chrono::steady_clock::now();
        // Calculates the difference
        auto diff = std::chrono::duration<double, std::nano>(end-start).count( );
        // Calculates the average time using standard deviation
        time_spent += (diff - time_spent)/(i+1);
    }
    std::cout << ">>> Memory Manager time: " << time_spent << " ns\n";

    // Average Time with the Operational System
    time_spent = 0;
    for ( int i = 0; i < times; i++ ) {
        start = std::chrono::steady_clock::now();
        for ( int j = 1; j <= 10; j += 2 ) {
            al1 = new int[j];
            al2 = new int[j+1];
            delete[] al1;
            delete[] al2;
        }
        // The final time
        end = std::chrono::steady_clock::now();
        // Calculates the difference
        auto diff = std::chrono::duration<double, std::nano>(end-start).count();
        // Calculates the average time using standard deviation
        time_spent += (diff - time_spent)/(i+1);
    }
    std::cout << ">>> Operational System time: " << time_spent << " ns\n";
}
/*}}}*/

	std::cout << "\n\n";

/*Testing if values are passed correctly{{{*/
{
	gm::SLPool p(220);

	int *ptr = new (p) int[8];

	std::cout << "Array with pool:\n[ ";
	for( int i=0; i < 8; i++ ){
		ptr[i] = (i+1)*5;
		std::cout << ptr[i] << " ";
	}
	std::cout << "]\n";

	int *ptr1 = new int[8];

	std::cout << "Array with SO:\n[ ";
	for( int i=0; i < 8; i++ ){
		ptr1[i] = (i+1)*5;
		std::cout << ptr1[i] << " ";
	}
	std::cout << "]\n";

	p.view( );

	for( int i=0; i < 8; i++ ) {
		assert( ptr[i] == ptr1[i] );
	}
	std::cout << "\n";

	delete[] ptr;
	delete[] ptr1;

}
/*}}}*/

/*
	Caso 1: Liberar uma área entre áreas livres.
	Caso 2: Liberar uma área antecedendo uma área ocupada, e sucedendo uma área livre.
	Caso 3: Liberar uma área antecedendo uma área livre, e sucecendo uma área ocupada.
	Caso 4: Liberar uma área entre áreas ocupadas.
	Caso 5: Liberar uma área antecedendo o sentinela, e sucedendo uma área livre.
	Caso 6: Liberar uma área antecedendo o sentinela, e sucedendo uma área ocupada.
*/

/*Caso 1{{{*/
{
}
/*}}}*/
/*Caso 2{{{*/
{
}
/*}}}*/
/*Caso 3{{{*/
{
}
/*}}}*/
/*Caso 4{{{*/
{
}
/*}}}*/
/*Caso 5{{{*/
{
}
/*}}}*/
/*Caso 6{{{*/
{
}
/*}}}*/

    std::cout << "\n>>> Exiting successfully...\n";

	return EXIT_SUCCESS;
}

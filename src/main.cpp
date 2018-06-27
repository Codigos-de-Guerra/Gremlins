/**
 * @file main.hpp
 * @version 1.0
 * @since Jun, 13. 
 * @date Jun, 22.
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
typedef std::chrono chrono;

/**
 * @brief Event Class
 */

class Event {
 public:
    /**
     * @brief Event default (empty) constructor
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

/**
 * @brief Allocation test in a limited time
 * @param _pool A pointer to the pool to be used
 * @param _timeLimit The time limit
 */
void StoragePoolTest(StoragePool *_pool, tempo _timeLimit) {
    // [1] Setup random numbers generator for memory size, say [100,2000] bytes.
    std::random_device rdm;
    auto ms_max = 2000u, ms_min = 100u;
    
    // [2] Setup random numbers generator for time intervals, say [1,100] units.
    auto ti_max = 100u, ti_min = 1u;
    
    // [3] Create the pirority queue std::priority_queue 'pq' and
    //     insert some events comprising the simulation.
    std::priority_queue< Event > pq;
    
    // Assuming there is a class Event that creates a pair address / time stamp.

    // Run simulation fot the time set by the client.
    for ( tempo t(0); t < _timeLimit; t++ ) {
        
        // Run while we have events pending or time to run.
        while ( !pq.empty( ) ){
            
            // Access the event with the smallest time - stamp.
            Event ev = pq.top( );
            
            // Do nothing. Still some time left.
            if ( ev.getTimeStamp( ) > t ) break;
            
            // When we got here, the top event has run out of time.
            // Remove event from priority queue.
            pq.pop( );
            
            // Calling free operator.
            _pool->Free(ev.getMemoryPtr( ));
        }
        
        auto memorySize = rdm( )%( ms_max - ms_min ) + ms_min;
        void *const add = _pool->Allocate(memorySize);
        auto timeSpent = rdm( )%( ti_max - ti_min ) + ti_min;
        
        // Set time stamp some time from now.
        tempo releaseTime = t + timeSpent;
        
        // Creating a new simulation event.
        pq.push( Event(add, releaseTime) );
        
        // Show Memory Map on Screen.
        _pool->view( );
    }
}

int main(/* int argc, char **argv */)
{

	gm::SLPool p(220), q(110);

	/* Also remembers that adds sizeof(Tag) when allocating.
		Therefore, a call to allocate 44 bytes becomes, inside code,
		a call to allocate 52 bytes. Requiring 4 blocks since 4*16 = 64.
	*/
	int *ptr0 = new int[2];			// Allocates 2*4=8 bytes.
	int *ptr1 = new (p) int[6];		// Allocates 6*4=24 bytes.
	int *ptr2 = new (q) int[11];	// Allocates 10*4=40 bytes.

	p.view();
	q.view();

	delete ptr0;
	delete ptr1;
	delete ptr2;

	std::cout << "\n";

	p.view();
	q.view();



	return 0;
}

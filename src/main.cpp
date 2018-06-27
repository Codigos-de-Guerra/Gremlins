#include <iostream>
#include <vector>
#include <string>
#include <fstream>

#include "../include/SLPool.hpp"
#include "../include/mempool_common.hpp"

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

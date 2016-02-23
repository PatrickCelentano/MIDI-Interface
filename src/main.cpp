// Useful includes
#include <iostream>
#include <string>

// The MIDIInterface class
#include "MIDIInterface.hpp"

// Very simplistic calls to the MIDIInterface.
// Note that MIDIInterface has to be a namespace,
// and cannot be a C++ class, as the multimedia
// callbacks cannot involve a vtable.
int main()
{
	std::cout << "Welcome to my basic MIDI input device player." << std::endl;
	std::cout << "  Type \"Q\", \"q\",  \"QUIT\", or  \"quit\" to quit." << std::endl;
	if(MIDIInterface::open())
	{
		// The running loop
		std::string input;
		while(	input != "Q"	&&
				input != "q"	&&
				input != "QUIT"	&&
				input != "quit"	)
		{
			std::cin >> input;
		}
		MIDIInterface::close();
	}
	else
	{
		return 1;
	}	
	return 0;
}
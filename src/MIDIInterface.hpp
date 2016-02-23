// Useful includes
#include <iostream>
#include <string>
#include <vector>

// Windows Multimedia
#include <windows.h>
#include <mmsystem.h>

// A very basic MIDI player, which allows pitches
// to be read in from a keyboard, and played, using
// Microsoft's multimedia library (winmm).
namespace MIDIInterface
{
	HMIDIIN inDevice;		// The MIDI input device interface
	HMIDIOUT outDevice;		// The MIDI output device interface
	MMRESULT flag;			// A useful flag for error detection
	
	void playMIDI(DWORD dwParam)
	{
		byte pitch		= (dwParam >> (1 << 3)) & 0xff;
		byte velocity	= (dwParam >> (2 << 3)) & 0xff;
		
		if(midiOutShortMsg(outDevice,dwParam) != MMSYSERR_NOERROR)
		{
			std::cerr << "Something didn't work." << std::endl;
		}
		
		std::cout << "Pitch: " << (int)pitch << "\tVelocity: " << (int)velocity << std::endl;
		
		/*
			TODO: have fun with this
			union { DWORD word; unsigned char data[4]; } message;
			message.data[0] = 0x90;				// Note on message
			message.data[1] = cantusFirmus;		// Key number
			message.data[2] = velocity;			// Key velocity
			message.data[3] = 0;				// Always 0
			midiOutShortMsg(outDevice,message.word);
		*/
	}
	
	// A necessary callback function for MIDI input
	void CALLBACK MidiInProc(HMIDIIN hMidiIn, UINT wMsg, DWORD_PTR dwInstance, DWORD dwParam1, DWORD dwParam2)
	{
		switch(wMsg)
		{
			// MIM_OPEN
			// Sent to a MIDI input callback function when a MIDI input device is opened
			// d1Param1	: reserved, do not use.
			// d1Param2	: reserved, do not use.
			case MIM_OPEN:
				// std::cout << "MIM_OPEN" << std::endl;
				break;
			// MIM_CLOSE
			// Sent to a MIDI input callback function when a MIDI input device is closed
			// d1Param1	: reserved, do not use.
			// d1Param2	: reserved, do not use.
			case MIM_CLOSE:
				// std::cout << "MIM_CLOSE" << std::endl;
				break;
			// MIM_DATA
			// Sent to a MIDI input callback function when a MIDI message is recieved by a MIDI input device
			// d1Param1	: dwMidiMessage (4 bytes)
			// d1Param2	: dwTimestamp (milliseconds since midiinStart was called)
			case MIM_DATA:
				// std::cout << "MIM_DATA" << std::endl;
				playMIDI(dwParam1);
				break;
			// MIM_LONGDATA
			// Sent to a MIDI input callback function when a system-exclusive buffer has been filled with data
			// d1Param1	: (DWORD)lpMidiHdr (pointer to a MIDIHDR structure identifying input buffer)
			// d1Param2	: dwTimestamp (milliseconds since midiinStart was called
			case MIM_LONGDATA:
				// std::cout << "MIM_LONGDATA" << std::endl;
				break;
			// MIM_ERROR
			// Sent to a MIDI input callback function when when an invalid MIDI message is received
			// d1Param1	: dwMidiMessage (4 bytes)
			// d1Param2	: dwTimestamp (milliseconds since midiinStart was called)
			case MIM_ERROR:
				// std::cout << "MIM_ERROR" << std::endl;
				break;
			// MIM_LONGDATA
			// Sent to a MIDI input callback function when an invalid or incomplete MIDI system-exclusive message is received.
			// d1Param1	: (DWORD)lpMidiHdr (pointer to a MIDIHDR structure identifying input buffer)
			// d1Param2	: dwTimestamp (milliseconds since midiinStart was called
			case MIM_LONGERROR:
				// std::cout << "MIM_LONGERROR" << std::endl;
				break;
			// MIM_MOREDATA
			// d1Param1	: dwMidiMessage (4 bytes)
			// d1Param2	: dwTimestamp (milliseconds since midiinStart was called)
			case MIM_MOREDATA:
				// std::cout << "MIM_MOREDATA" << std::endl;
				break;
			// There shouldn't be any others
			default:
				// std::cout << "UNKNOWN MESSAGE" << std::endl;
				break;
		}
	}

	// Opens the necessary MIDI devices
	bool open()
	{
		std::cout << "There are " << midiInGetNumDevs() << " MIDI input devices." << std::endl;
		
		// Ensure there's at least one MIDI input device
		if(midiInGetNumDevs() <= 0)
		{
			std::cerr << " There are no available MIDI input devices." << std::endl;
			return false;
		}
		
		// Open MIDI input
		flag = midiInOpen(&inDevice, 0, (DWORD_PTR)MidiInProc, 0, CALLBACK_FUNCTION);
		if (flag != MMSYSERR_NOERROR)
		{
			std::cerr << "Error opening MIDI input." << std::endl;
			return false;
		}

		// Open the MIDI output port
		flag = midiOutOpen(&outDevice, 0, 0, 0, CALLBACK_NULL);
		if (flag != MMSYSERR_NOERROR)
		{
			std::cerr << "Error opening MIDI output." << std::endl;
			return false;
		}
		
		// Starts MIDI input
		midiInStart(inDevice);

		return true;
	}
	
	void close()
	{
		// Ends input
		midiInStop(inDevice);
		midiInClose(inDevice);
		
		// Ends output
		midiOutReset(outDevice);
		midiOutClose(outDevice);
	}
};
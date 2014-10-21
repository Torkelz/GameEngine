#pragma once
#include "InputRecord.h"
#include "InputState.h"
#include "InputTranslator.h"

#include <vector>

/**
* Handles input to the program.
*/
class Input
{
private:
	InputTranslator::ptr m_Translator;

	InputState m_InputState;
	std::vector<InputRecord> m_FrameInputs;

public:
	/**
	* Initialize the input object with a translator to provide input records.
	*/
	void init(InputTranslator::ptr p_Translator);
	/**
	* Release resources.
	*/
	void destroy();

	/**
	* Clear the queue of input records received since the last call to onFrame.
	*/
	void onFrame();
	/**
	* Get a snapshot of the current input state.
	*
	* @return An object containing the current state of inputs.
	*/
	const InputState& getCurrentState() const;
	/**
	* Get the input records received since the last time onFrame was called.
	*
	* @return The input records received this frame.
	*/
	const std::vector<InputRecord>& getFrameInputs() const;

	/**
	* Lock the mouse pointer to the center of the screen or unlock it.
	*
	* @param p_State true for lock.
	*/
	void lockMouse(bool p_State);

private:
	/**
	* Handle records.
	*
	* @param p_Record A new input record.
	*/
	void handleRecords(InputRecord p_Record);
};

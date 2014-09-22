#pragma once
#include <exception>
#include <string>

/**
 * Base class for all our exceptions, all our exceptions
 * should inherit from this class.
 */


class MemoryException : public std::exception
{
protected:
	/**
	* Message describing the error.
	*/
	const std::string m_What;
	/**
	* The line number where the exception was thrown.
	*/
	int m_Line;
	/**
	* The file the exception was thrown from.
	*/
	const std::string m_File;

public:
	/**
	* @param p_What A message describing the error
	* @param p_Line The line of the exception (use __LINE__)
	* @param p_File The file of the exception (use __FILE__)
	*/
	MemoryException(const std::string &p_What, int p_Line, const std::string &p_File)
		: m_What(p_File + "(" + std::to_string(p_Line) + ")" + p_What),
		m_Line(p_Line), m_File(p_File)
	{

	}

	virtual const char* what() const throw() override
	{
		return m_What.c_str();
	}
};
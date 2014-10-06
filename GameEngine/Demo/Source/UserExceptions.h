#pragma once

/**
 * We should probably place some license information here.
 */

#include <exception>
#include <string>

/**
 * Base class for all our exceptions, all our exceptions
 * should inherit from this class.
 */
class GraphicsException : public std::exception
{
protected:
        /**
         * Message describing the error
         */
        const std::string m_What;
        /**
         * The line number where the exception was thrown
         */
        int m_Line;
        /**
         * The file the exception was thrown from
         */
        const std::string m_File;


public:
        /**
         * constructor.
         *
         * @param p_What A message describing the error
         * @param p_Line The line of the exception (use __LINE__)
         * @param p_File The file of the exception (use __FILE__)
         */
        GraphicsException(const std::string& p_What, int p_Line, const std::string& p_File)
                : m_What(p_File + "(" + std::to_string(p_Line) + "): " + p_What),
                  m_Line(p_Line),
                  m_File(p_File)
        {
        }


        virtual const char* what() const throw() override
        {
                return m_What.c_str();
        }
};

/**
 * An exception to be thrown when an error is encountered with the buffers.
 */
class BufferException : public GraphicsException
{
public:
        /**
         * constructor.
         *
         * @param p_What A message describing the error
         * @param p_Line The line of the exception (use __LINE__)
         * @param p_File The file of the exception (use __FILE__)
         */
        BufferException(const std::string& p_What, int p_Line, const std::string& p_File)
                : GraphicsException(p_What, p_Line, p_File)
        {
        }
};

/**
* An exception to be thrown when an error is encountered with the buffers.
*/
class InvalidArgument : GraphicsException
{
public:
	/**
	* constructor.
	*
	* @param p_What A message describing the error
	* @param p_Line The line of the exception (use __LINE__)
	* @param p_File The file of the exception (use __FILE__)
	*/
	InvalidArgument(const std::string& p_What, int p_Line, const std::string& p_File)
		: GraphicsException(p_What, p_Line, p_File)
	{
	}
};

/**
* An exception to be thrown when an error is encountered with the buffers.
*/
class Win32Exception : GraphicsException
{
public:
	/**
	* constructor.
	*
	* @param p_What A message describing the error
	* @param p_Line The line of the exception (use __LINE__)
	* @param p_File The file of the exception (use __FILE__)
	*/
	Win32Exception(const std::string& p_What, int p_Line, const std::string& p_File)
		: GraphicsException(p_What, p_Line, p_File)
	{
	}
};

/**
* An exception to be thrown when an error occurs with the shaders.
*/
class ShaderException : public GraphicsException
{
public:
        /**
         * constructor.
         *
         * @param p_What A message describing the error
         * @param p_Line The line of the exception (use __LINE__)
         * @param p_File The file of the exception (use __FILE__)
         */
        ShaderException(const std::string& p_What, int p_Line, const std::string& p_File)
                : GraphicsException(p_What, p_Line, p_File)
        {
        }
};

/**
* An exception to be thrown when an error occurs in the wrapper factory.
*/
class WrapperFactoryException : public GraphicsException
{
public:
        /**
         * constructor.
         *
         * @param p_What A message describing the error
         * @param p_Line The line of the exception (use __LINE__)
         * @param p_File The file of the exception (use __FILE__)
         */
        WrapperFactoryException(const std::string& p_What, int p_Line, const std::string& p_File)
                : GraphicsException(p_What, p_Line, p_File)
        {
        }
};

class WindowException : public std::exception
{
protected:
        /**
         * Message describing the error
         */
        const std::string m_What;
        /**
         * The line number where the exception was thrown
         */
        int m_Line;
        /**
         * The file the exception was thrown from
         */
        const std::string m_File;


public:
        /**
         * constructor.
         *
         * @param p_What A message describing the error
         * @param p_Line The line of the exception (use __LINE__)
         * @param p_File The file of the exception (use __FILE__)
         */
        WindowException(const std::string& p_What, int p_Line, const std::string& p_File)
                : m_What(p_File + "(" + std::to_string(p_Line) + "): " + p_What),
                  m_Line(p_Line),
                  m_File(p_File)
        {
        }


        virtual const char* what() const throw() override
        {
                return m_What.c_str();
        }
};
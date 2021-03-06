#pragma once

#include <exception>
#include <sstream>

namespace Datastructures
{
namespace Heaps
{
namespace Exceptions
{
    /***************************************************************************************************************
     *
     *
     ***************************************************************************************************************/
    template<typename T>
    class InvalidKeyException : public std::exception
    {
      public:
        /***********************************************************************************************************
         *
         *
         ***********************************************************************************************************/
        InvalidKeyException(const T &invalid_key) : m_key(invalid_key) {}

        /***********************************************************************************************************
         *
         *
         ***********************************************************************************************************/
        virtual const char *what() const throw()
        {
            std::stringstream ss;
            ss << "Invalid key given: " << m_key;
            return ss.str().c_str();
        }

      private:
        T m_key;
    };

    /***************************************************************************************************************
     *
     *
     ***************************************************************************************************************/
    class NullNodeException : public std::exception
    {
      public:
        /***********************************************************************************************************
         *
         *
         ***********************************************************************************************************/
        virtual const char *what() const throw()
        {
            std::stringstream ss;
            ss << "Supplied Node* was nullptr";
            return ss.str().c_str();
        }
    };

    /***************************************************************************************************************
     *
     *
     ***************************************************************************************************************/
    class InvalidIndexException : public std::exception
    {
    public:
        /***********************************************************************************************************
         *
         *
         ***********************************************************************************************************/
        InvalidIndexException(const size_t &invalid_index) : m_index(invalid_index) {}

        /***********************************************************************************************************
         *
         *
         ***********************************************************************************************************/
        virtual const char *what() const throw()
        {
            std::stringstream ss;
            ss << "Supplied Index was invalid: " << m_index;
            return ss.str().c_str();
        }

    private:
        size_t m_index;
    };

    /***************************************************************************************************************
     *
     *
     ***************************************************************************************************************/
    class UnderflowException : public std::exception
    {
    public:
        /***********************************************************************************************************
         *
         *
         ***********************************************************************************************************/
        virtual const char* what() const throw()
        {
            std::stringstream ss;
            ss << "The heap is empty";
            return ss.str().c_str();
        }
    };
    }
}
}
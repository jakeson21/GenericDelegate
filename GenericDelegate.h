/*
 * GenericDelegate.h
 *
 *  Created on: Feb 23, 2016
 *      Author: fuguru
 */

#ifndef GENERICDELEGATE_H_
#define GENERICDELEGATE_H_

#include <cstdlib>
#include <stdint.h>
#include <stdio.h>

namespace Help {

    class GenericClass;

    template <typename ArgumentType1=void, typename ArgumentType2=void, typename ArgumentType3=void>
    class GenericDelegate
    {
        private:
            typedef void (GenericClass::*GenericFunctionPtr)(ArgumentType1, ArgumentType2, ArgumentType3);
            GenericFunctionPtr mFunction;
            GenericClass* mInstance;

            union function_to_ptr_cast {
               GenericFunctionPtr in;
               void* out;
            };

            int64_t getFunctionHash() const
            {
                function_to_ptr_cast temp;
                temp.in = mFunction;
                char str[100];
                sprintf(str, "%p",temp.out);
                return strtol(str, NULL, 16);
            }

        public:
            GenericDelegate() : mFunction(0), mInstance(0) { }

            template <class X, class Y>
            GenericDelegate(X* inInstance, void ( Y::*inFunction)(ArgumentType1, ArgumentType2, ArgumentType3))
            {
                mInstance = reinterpret_cast<GenericClass*>(inInstance);
                mFunction = reinterpret_cast<GenericFunctionPtr>(inFunction);
            }

            ~GenericDelegate() { }

            inline void operator() (ArgumentType1 inArg1, ArgumentType2 inArg2, ArgumentType3 inArg3)
            {
                (mInstance->*mFunction)(inArg1, inArg2, inArg3);
            }

            operator bool() const
            {
                return mInstance;
            }

            bool operator==(const GenericDelegate<ArgumentType1, ArgumentType2, ArgumentType3>& inRight) const
            {
                return (mInstance == inRight.mInstance && mFunction == inRight.mFunction);
            }

            bool operator<(const GenericDelegate<ArgumentType1, ArgumentType2, ArgumentType3>& inRight) const
            {
                // Implemented operator< so that GenericDelegate can be compared for use in std::set.
                // std::set uses the < operator to do equality comparisons [!(a < b) && !(b < a)]
                // Member function pointers can't be evaluated with operator< because it is considered
                // undefined behavior by the C++ standard.
                // Understanding this, the solution is to printf into a string and then interpret the
                // string as an integer to do address comparison. Behavior for comparison when using virtual
                // functions is unknown at the time. Be warned.
                return ((int64_t)this->mInstance + this->getFunctionHash() < (int64_t)inRight.mInstance + inRight.getFunctionHash());
            }
    };

    template <typename ArgumentType1, typename ArgumentType2>
    class GenericDelegate<ArgumentType1, ArgumentType2, void>
    {
        friend class AssociationMap;

        private:
            typedef void (GenericClass::*GenericFunctionPtr)(ArgumentType1, ArgumentType2);
            GenericFunctionPtr mFunction;
            GenericClass* mInstance;

            union function_to_ptr_cast {
               GenericFunctionPtr in;
               void* out;
            };

            int64_t getFunctionHash() const
            {
                function_to_ptr_cast temp;
                temp.in = mFunction;
                char str[100];
                sprintf(str, "%p",temp.out);
                return strtol(str, NULL, 16);
            }

        public:
            GenericDelegate() : mFunction(0), mInstance(0) { }

            template <class X, class Y>
            GenericDelegate(X* inInstance, void ( Y::*inFunction)(ArgumentType1, ArgumentType2))
            {
                mInstance = reinterpret_cast<GenericClass*>(inInstance);
                mFunction = reinterpret_cast<GenericFunctionPtr>(inFunction);
            }

            ~GenericDelegate() { }

            inline void operator() (ArgumentType1 inArg1, ArgumentType2 inArg2)
            {
                (mInstance->*mFunction)(inArg1, inArg2);
            }

            operator bool() const
            {
                return mInstance;
            }

            bool operator==(const GenericDelegate<ArgumentType1, ArgumentType2>& inRight) const
            {
                return (mInstance == inRight.mInstance && mFunction == inRight.mFunction);
            }

            bool operator<(const GenericDelegate<ArgumentType1, ArgumentType2>& inRight) const
            {
                // Implemented operator< so that GenericDelegate can be compared for use in std::set.
                // std::set uses the < operator to do equality comparisons [!(a < b) && !(b < a)]
                // Member function pointers can't be evaluated with operator< because it is considered
                // undefined behavior by the C++ standard.
                // Understanding this, the solution is to printf into a string and then interpret the
                // string as an integer to do address comparison. Behavior for comparison when using virtual
                // functions is unknown at the time. Be warned.
                return ((int64_t)this->mInstance + this->getFunctionHash() < (int64_t)inRight.mInstance + inRight.getFunctionHash());
            }
    };

    template <typename ArgumentType1>
    class GenericDelegate<ArgumentType1, void, void>
    {
        friend class AssociationMap;

        private:
            typedef void (GenericClass::*GenericFunctionPtr)(ArgumentType1);
            GenericFunctionPtr mFunction;
            GenericClass* mInstance;

            union function_to_ptr_cast {
               GenericFunctionPtr in;
               void* out;
            };

            int64_t getFunctionHash() const
            {
                function_to_ptr_cast temp;
                temp.in = mFunction;
                char str[100];
                sprintf(str, "%p",temp.out);
                return strtol(str, NULL, 16);
            }

        public:
            GenericDelegate() : mFunction(0), mInstance(0) { }

            template <class X, class Y>
            GenericDelegate(X* inInstance, void (Y::*inFunction)(ArgumentType1))
            {
                mInstance = reinterpret_cast<GenericClass*>(inInstance);
                mFunction = reinterpret_cast<GenericFunctionPtr>(inFunction);
            }

            ~GenericDelegate() { }

            inline void operator()(ArgumentType1 inArg1)
            {
                (mInstance->*mFunction)(inArg1);
            }

            operator bool() const
            {
                return mInstance;
            }

            bool operator==(const GenericDelegate<ArgumentType1>& inRight) const
            {
                return (mInstance == inRight.mInstance && mFunction == inRight.mFunction);
            }

            bool operator<(const GenericDelegate<ArgumentType1>& inRight) const
            {
                // Implemented operator< so that GenericDelegate can be compared for use in std::set.
                // std::set uses the < operator to do equality comparisons [!(a < b) && !(b < a)]
                // Member function pointers can't be evaluated with operator< because it is considered
                // undefined behavior by the C++ standard.
                // Understanding this, the solution is to printf into a string and then interpret the
                // string as an integer to do address comparison. Behavior for comparison when using virtual
                // functions is unknown at the time. Be warned.
                return ((int64_t)this->mInstance + this->getFunctionHash() < (int64_t)inRight.mInstance + inRight.getFunctionHash());
            }
    };

    template<>
    class GenericDelegate<void, void, void>
    {
        friend class AssociationMap;

        private:
            typedef void (GenericClass::*GenericFunctionPtr)(void);
            GenericFunctionPtr mFunction;
            GenericClass *mInstance;

            union function_to_ptr_cast {
               GenericFunctionPtr in;
               void* out;
            };

            int64_t getFunctionHash() const
            {
                function_to_ptr_cast temp;
                temp.in = mFunction;
                char str[100];
                sprintf(str, "%p",temp.out);
                return strtol(str, NULL, 16);
            }

        public:
            GenericDelegate() : mFunction(0), mInstance(0) { }

            template <class X, class Y>
            GenericDelegate(X* inInstance, void(Y::*inFunction)(void))
            {
                mInstance = reinterpret_cast<GenericClass*>(inInstance);
                mFunction = reinterpret_cast<GenericFunctionPtr>(inFunction);
            }

            ~GenericDelegate() { }

            inline void operator()()
            {
                (mInstance->*mFunction)();
            }

            operator bool() const
            {
                return mInstance;
            }

            bool operator==(const GenericDelegate<>& inRight) const
            {
                return (mInstance == inRight.mInstance && mFunction == inRight.mFunction);
            }

            bool operator<(const GenericDelegate<>& inRight) const
            {
                // Implemented operator< so that GenericDelegate can be compared for use in std::set.
                // std::set uses the < operator to do equality comparisons [!(a < b) && !(b < a)]
                // Member function pointers can't be evaluated with operator< because it is considered
                // undefined behavior by the C++ standard.
                // Understanding this, the solution is to printf into a string and then interpret the
                // string as an integer to do address comparison. Behavior for comparison when using virtual
                // functions is unknown at the time. Be warned.
                return ((int64_t)this->mInstance + this->getFunctionHash() < (int64_t)inRight.mInstance + inRight.getFunctionHash());
            }
    };
}


#endif /* GENERICDELEGATE_H_ */

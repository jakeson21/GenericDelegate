
#ifndef GENERICDELEGATE_H_
#define GENERICDELEGATE_H_

#include <cstdlib>
#include <stdint.h>
#include <stdio.h>

namespace Help {

    class GenericClass;

    template <typename... Params>
    class GenericDelegate
    {
        private:
            typedef void (GenericClass::*GenericFunctionPtr)(Params...);
            GenericFunctionPtr mFunction;
            GenericClass* mInstance;

            union function_to_ptr_cast
            {
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
            GenericDelegate(X* inInstance, void ( Y::*inFunction)(Params...))
            {
                mInstance = reinterpret_cast<GenericClass*>(inInstance);
                mFunction = reinterpret_cast<GenericFunctionPtr>(inFunction);
            }

            ~GenericDelegate() { }

            inline void operator() (Params... args)
            {
                (mInstance->*mFunction)(args...);
            }

            operator bool() const
            {
                return mInstance;
            }

            bool operator==(const GenericDelegate<Params...>& inRight) const
            {
                return (mInstance == inRight.mInstance && mFunction == inRight.mFunction);
            }

            bool operator<(const GenericDelegate<Params...>& inRight) const
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

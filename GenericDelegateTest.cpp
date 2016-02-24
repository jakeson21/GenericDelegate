//============================================================================
// Name        : MyTestProject.cpp
// Author      : Me
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C, Ansi-style
//============================================================================

#include <cstdlib>
#include <stdint.h>
#include <stdio.h>
#include <boost/thread.hpp>
#include <string>

#include "GenericDelegate.h"

class Information
{
    public:
        Information() : mStrVal("Information")
        {

        }
        virtual ~Information() {};

        std::string toString() { return mStrVal; }

    private:
        std::string mStrVal;
};

// Forward declaration
class GenericBaseObj;
// this typedef should match to function signature of what it will be used to call. This could be a different class then the one performing the callback.
typedef Help::GenericDelegate<const boost::shared_ptr<Information>&> GenericMethodDelegate;

class GenericBaseObj
{
    public:
        GenericBaseObj(GenericMethodDelegate inDel, double inVal)
        : mParam1("GenericBaseObj"),
          mParam2(inVal),
          mMethodDel(inDel),
          mMyInfo(new Information)
        {

        }

        virtual ~GenericBaseObj(){}

        void start()
        {
            std::cout << "GenericBaseObj.start()" << std::endl;
            // just give it ourself.
            doSomethingThenCallback(mMyInfo);
        }

        void doSomethingThenCallback(Information* inNewCall)
        {
            std::cout << "GenericBaseObj.doSomethingThenCallback()" << std::endl;

            boost::shared_ptr<Information> aNewCall(inNewCall);
            mMethodDel(aNewCall);
        }

        std::string mParam1;
        double mParam2;
        /// some standard parameters and methods
    private:
        GenericMethodDelegate mMethodDel;
        Information* mMyInfo;
};


class MainContainer
{
    public:
        MainContainer()
        : mBaseObj(),
          mMethodCallback(this, &MainContainer::callbackFunction)
        {}

        virtual ~MainContainer()
        {

        }

        void start()
        {
            std::cout << "MainContainer.start()" << std::endl;
            mBaseObj.reset(new GenericBaseObj(mMethodCallback, -1.0));
            mBaseObj->start();
        }

        virtual void callbackFunction(const boost::shared_ptr<Information>& inParm)
        {
            std::cout << "Callback was called with " << inParm->toString() << std::endl;
        }
    private:
        boost::shared_ptr<GenericBaseObj> mBaseObj;
        /// Delegate to callback method
        GenericMethodDelegate mMethodCallback;
};

int main(void) {
    std::cout << "Hello World!!!" << std::endl;

	MainContainer mainObj;
	mainObj.start();

	std::cout << "Finished" << std::endl;

	return EXIT_SUCCESS;
}

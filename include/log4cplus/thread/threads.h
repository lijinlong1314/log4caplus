// Module:  Log4CPLUS
// File:    threads.h
// Created: 6/2001
// Author:  Tad E. Smith
//
//
// Copyright 2001-2010 Tad E. Smith
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/** @file */

#ifndef _LOG4CPLUS_THREAD_THREADS_HEADER_
#define _LOG4CPLUS_THREAD_THREADS_HEADER_

#include <log4cplus/config.hxx>
#include <log4cplus/tstring.h>
#include <log4cplus/helpers/pointer.h>


namespace log4cplus { namespace thread {


/**
 * This is used to lock a mutex.  The dtor unlocks the mutex.
 */
class Guard
{
public:
    /** "locks" <code>mutex</code>. */
    Guard(Mutex const * mutex)
        : _mutex (mutex)
    {
        _mutex->lock ();
    }

    /** "locks" <code>mutex</code>. */
    Guard(Mutex const & mutex)
        : _mutex (&mutex)
    {
        _mutex->lock ();
    }

    /** "unlocks" <code>mutex</code>. */
    ~Guard()
    {
        _mutex->unlock ();
    }

private:
    Mutex const * _mutex;

    // disable copy
    Guard(const Guard&);
    Guard& operator=(const Guard&);
};


LOG4CPLUS_EXPORT log4cplus::tstring getCurrentThreadName();
LOG4CPLUS_EXPORT log4cplus::tstring getCurrentThreadName2();
LOG4CPLUS_EXPORT void yield();
LOG4CPLUS_EXPORT void blockAllSignals();


#ifndef LOG4CPLUS_SINGLE_THREADED

class ThreadImplBase
    : public virtual log4cplus::helpers::SharedObject
{
protected:
    virtual ~ThreadImplBase ();
};


/**
 * There are many cross-platform C++ Threading libraries.  The goal of
 * this class is not to replace (or match in functionality) those
 * libraries.  The goal of this class is to provide a simple Threading
 * class with basic functionality.
 */
class LOG4CPLUS_EXPORT AbstractThread
    : public virtual log4cplus::helpers::SharedObject
{
public:
    AbstractThread();
    bool isRunning() const;
    virtual void start();
    void join () const;
    virtual void run() = 0;

protected:
    // Force objects to be constructed on the heap
    virtual ~AbstractThread();

private:
    helpers::SharedObjectPtr<ThreadImplBase> thread;

    // Disallow copying of instances of this class.
    AbstractThread(const AbstractThread&);
    AbstractThread& operator=(const AbstractThread&);
};

typedef helpers::SharedObjectPtr<AbstractThread> AbstractThreadPtr;


#endif // LOG4CPLUS_SINGLE_THREADED


} } // namespace log4cplus { namespace thread {


#endif // _LOG4CPLUS_THREAD_THREADS_HEADER_


//   Copyright (C) 2009-2010, Vaclav Haisman. All rights reserved.
//   
//   Redistribution and use in source and binary forms, with or without modifica-
//   tion, are permitted provided that the following conditions are met:
//   
//   1. Redistributions of  source code must  retain the above copyright  notice,
//      this list of conditions and the following disclaimer.
//   
//   2. Redistributions in binary form must reproduce the above copyright notice,
//      this list of conditions and the following disclaimer in the documentation
//      and/or other materials provided with the distribution.
//   
//   THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED WARRANTIES,
//   INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
//   FITNESS  FOR A PARTICULAR  PURPOSE ARE  DISCLAIMED.  IN NO  EVENT SHALL  THE
//   APACHE SOFTWARE  FOUNDATION  OR ITS CONTRIBUTORS  BE LIABLE FOR  ANY DIRECT,
//   INDIRECT, INCIDENTAL, SPECIAL,  EXEMPLARY, OR CONSEQUENTIAL  DAMAGES (INCLU-
//   DING, BUT NOT LIMITED TO, PROCUREMENT  OF SUBSTITUTE GOODS OR SERVICES; LOSS
//   OF USE, DATA, OR  PROFITS; OR BUSINESS  INTERRUPTION)  HOWEVER CAUSED AND ON
//   ANY  THEORY OF LIABILITY,  WHETHER  IN CONTRACT,  STRICT LIABILITY,  OR TORT
//   (INCLUDING  NEGLIGENCE OR  OTHERWISE) ARISING IN  ANY WAY OUT OF THE  USE OF
//   THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#ifndef LOG4CPLUS_CONFIG_HXX
#define LOG4CPLUS_CONFIG_HXX

#include <cstddef>

#if defined (_WIN32)
#  include <log4cplus/config/win32.h>
#elif (defined(__MWERKS__) && defined(__MACOS__))
#  include <log4cplus/config/macosx.h>
#else
#  include <log4cplus/config/defines.hxx>
#endif

#if ! defined (UNICODE) && ! defined (LOG4CPLUS_HAVE_VSNPRINTF_S) \
    && ! defined (LOG4CPLUS_HAVE__VSNPRINTF_S) \
    && ! defined (LOG4CPLUS_HAVE_VSNPRINTF) \
    && ! defined (LOG4CPLUS_HAVE__VSNPRINTF)
#  undef LOG4CPLUS_USE_POOR_MANS_SNPRINTF
#  define LOG4CPLUS_USE_POOR_MANS_SNPRINTF
#endif

# if ! defined (LOG4CPLUS_WORKING_LOCALE) \
  && ! defined (LOG4CPLUS_WORKING_C_LOCALE) \
  && ! defined (LOG4CPLUS_WITH_ICONV)
# define LOG4CPLUS_POOR_MANS_CHCONV
#endif

#if !defined(_WIN32)
#  define LOG4CPLUS_USE_BSD_SOCKETS
#  if !defined(LOG4CPLUS_SINGLE_THREADED)
#    define LOG4CPLUS_USE_PTHREADS
#  endif
#  if defined (INSIDE_LOG4CPLUS)
#    define LOG4CPLUS_EXPORT LOG4CPLUS_DECLSPEC_EXPORT
#  else
#    define LOG4CPLUS_EXPORT LOG4CPLUS_DECLSPEC_IMPORT
#  endif // defined (INSIDE_LOG4CPLUS)

#endif // !_WIN32

#if defined (LOG4CPLUS_INLINES_ARE_EXPORTED)
#  define LOG4CPLUS_INLINE_EXPORT inline
#else
#  define LOG4CPLUS_INLINE_EXPORT
#endif

#if _MSC_VER+0 >= 1400
#  define LOG4CPLUS_FSTREAM_ACCEPTS_WCHAR_T
#endif
#if _MSC_VER+0 >= 1600
#  define LOG4CPLUS_HAVE_CODECVT_UTF8_FACET
#  define LOG4CPLUS_HAVE_CODECVT_UTF16_FACET
#endif

// C++11 stuff

#if ! defined (__has_feature)
//! __has_feature(X) is Clangs way for testing features.
//! Define it to 0 if it does not exist.
#  define __has_feature(X) 0
#endif

#if (_MSC_VER+0 >= 1600) \
    || defined (__GXX_EXPERIMENTAL_CXX0X__)
#  define LOG4CPLUS_HAVE_CXX11_SUPPORT
#endif

#if defined (LOG4CPLUS_HAVE_CXX11_SUPPORT) \
    || __has_feature (cxx_rvalue_references)
#  define LOG4CPLUS_HAVE_RVALUE_REFS
#endif

#include <log4cplus/helpers/thread-config.h>

namespace log4cplus
{

//! Per thread cleanup function. Users should call this function before
//! a thread ends its execution. It frees resources allocated in thread local
//! storage. It is important only for multi-threaded static library builds
//! of log4cplus and user threads. In all other cases the clean up is provided
//! automatically by other means.
LOG4CPLUS_EXPORT void threadCleanup ();

} // namespace log4cplus


#endif // LOG4CPLUS_CONFIG_HXX

//                                               -*- C++ -*-
/**
 *  @file  Log.hxx
 *  @brief Log records all user information to a file or tty
 *
 *  Copyright (C) 2005-2013 EDF-EADS-Phimeca
 *
 *  This library is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  along with this library.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  @author schueller
 *  @date   2012-07-16 10:12:54 +0200 (Mon, 16 Jul 2012)
 *  Id      Log.hxx 2600 2012-07-16 08:12:54Z schueller
 */
#ifndef OPENTURNS_LOG_HXX
#define OPENTURNS_LOG_HXX

#include <fstream>
#include <map>
#include "OTprivate.hxx"
#include "AtomicFunctions.hxx"
#include "TTY.hxx"

#ifdef WIN32
#undef ERROR
#endif

#define LOGDEBUG(st)   do { if (OT::Log::HasDebug()  ) OT::Log::Debug(st);   } while(0)
#define LOGWRAPPER(st) do { if (OT::Log::HasWrapper()) OT::Log::Wrapper(st); } while(0)
#define LOGINFO(st)    do { if (OT::Log::HasInfo()   ) OT::Log::Info(st);    } while(0)
#define LOGUSER(st)    do { if (OT::Log::HasUser()   ) OT::Log::User(st);    } while(0)
#define LOGWARN(st)    do { if (OT::Log::HasWarn()   ) OT::Log::Warn(st);    } while(0)
#define LOGERROR(st)   do { if (OT::Log::HasError()  ) OT::Log::Error(st);   } while(0)
#define LOGTRACE(st)   do { if (OT::Log::HasTrace()  ) OT::Log::Trace(st);   } while(0)


BEGIN_NAMESPACE_OPENTURNS



#ifndef SWIG
struct _Prefix
{
  typedef String Value;
  Value color_, nocolor_, prefix_;
  _Prefix() : color_(), nocolor_(), prefix_() {}
  _Prefix( const Value & color, const Value & nocolor, const Value & prefix) : color_(color), nocolor_(nocolor), prefix_(prefix) {}
};
#endif


/**
 * @class log
 * @brief Records all user information to a file or tty
 * @internal
 */

class Log
{
public:

  typedef unsigned long Severity;

private:
  static Log & GetInstance();
  static void Initialization();
  static void Release();

public:

  /** Those flags should be ORed */
  static const Severity DBG;
  static const Severity WRAPPER;
  static const Severity INFO;
  static const Severity USER;
  static const Severity WARN;
  static const Severity ERROR;
  static const Severity TRACE;

  static const Severity DEFAULT;
  static const Severity NONE;
  static const Severity ALL;

#ifndef SWIG
  /** Change the Log */
  static void Reset();
#endif

  /** Log messages according to the DBG level
   * @param msg The message to be logged
   */
  static void Debug(const String & msg);
  static inline Bool HasDebug()
  {
    return Flags() & Log::DBG;
  }

  /** Log messages according to the WRAPPER level
   * @param msg The message to be logged
   */
  static void Wrapper(const String & msg);
  static inline Bool HasWrapper()
  {
    return Flags() & Log::WRAPPER;
  }

  /** Log messages according to the INFO level
   * @param msg The message to be logged
   */
  static void Info(const String & msg);
  static inline Bool HasInfo()
  {
    return Flags() & Log::INFO;
  }

  /** Log messages according to the USER level
   * @param msg The message to be logged
   */
  static void User(const String & msg);
  static inline Bool HasUser()
  {
    return Flags() & Log::USER;
  }

  /** Log messages according to the WARN level
   * @param msg The message to be logged
   */
  static void Warn(const String & msg);
  static inline Bool HasWarn()
  {
    return Flags() & Log::WARN;
  }

  /** Log messages according to the ERROR level
   * @param msg The message to be logged
   */
  static void Error(const String & msg);
  static inline Bool HasError()
  {
    return Flags() & Log::ERROR;
  }

  /** Log messages according to the TRACE level
   * @param msg The message to be logged
   */
  static void Trace(const String & msg);
  static inline Bool HasTrace()
  {
    return Flags() & Log::TRACE;
  }

  /** Divert the output to a file
   * @param file The filename where the log will be written
   */
  static void SetFile(const FileName & file);

  /** Set the level flags for the messages logged to the file
   * @param flags An integer built from ORed level flags
   */
  static void Show(Severity flags);

  /** Get the current level flags
   * @return An integer built from ORed level flags
   */
  static Severity Flags();

  /** Flush pending messages
   * @internal
   */
  static void Flush();

  /** Does Log show repeated messages or not
   *  If repeat is false then Log shows every messages it receives
   *  even if they are identical to the previous ones.
   *  If repeat is true then Log only shows the first message
   *  and a message couting how much identical messages were
   *  received after that.
   */
  static void Repeat( Bool repeat );

  /** Color  accessors. */
  static void SetColor(const Log::Severity severity,
                       const TTY::Color color);
  static void SetColor(const Log::Severity severity,
                       const String & color);
  static String GetColor(const Log::Severity severity);

#ifndef SWIG
  struct Entry
  {
    Severity sev_;
    String msg_;
    Entry(Severity sev, String msg) : sev_(sev), msg_(msg) {}
    Entry() : sev_(0), msg_() {}
    Bool operator== (const Entry & other) const
    {
      return (this->sev_ == other.sev_) && (this->msg_ == other.msg_);
    }
  }; /* end struct Entry */
#endif
  ~Log();

private:
  Log();
  void push(const Entry & entry);
  void printRepeatedMessage(const Entry & entry);
  void flush();
  void repeat( Bool r );
  void setColor(const Log::Severity severity,
                const String & color);
  String getColor(const Log::Severity severity) const;

  /** Set the name of the log file */
  void setFile(const FileName & file);

  /** Human readable log */
  mutable std::map<Severity, _Prefix > logName_;

  /** The environment variable name */
  const char * openturnsLogSeverityVariableName_;

  /** Set Severity according to Openturns LogSeverity Variable */
  void initSeverityFromEnvironment();

  /** The file where to write messages */
  std::ostream * p_file_;

  /** Remember the previous message */
  mutable Entry previousMessage_;
  mutable UnsignedLong count_;
  mutable AtomicInt repeat_;

  friend struct Log_init; /* friendship for static member initialization */
}; /* end class Log */



/** This struct initializes all static members of Log */
struct Log_init
{
  Log_init();
  ~Log_init();
}; /* end struct Log_init */

static Log_init __Log_initializer;


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_LOG_HXX */

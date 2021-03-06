//                                               -*- C++ -*-
/**
 *  @file  Log.cxx
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
 *  @author schueller
 *  @date   2012-07-16 10:12:54 +0200 (Mon, 16 Jul 2012)
 */
#include <iostream>
#include <cstdlib>
#include <cassert>
#include <errno.h>

#include "OTthread.hxx"
#include "Log.hxx"
#include "OTconfig.hxx"
#include "MutexLock.hxx"

BEGIN_NAMESPACE_OPENTURNS



static inline
_Prefix make_prefix( const _Prefix::Value & color, const _Prefix::Value & nocolor, const _Prefix::Value & prefix)
{
  return _Prefix( color, nocolor, prefix );
}

std::ostream & operator << ( std::ostream & os, const _Prefix & pfx )
{
  return os << (TTY::ColoredOutput() ? pfx.color_ : pfx.nocolor_) << pfx.prefix_;
}


#ifndef WIN32
static pthread_mutex_t Log_InstanceMutex_;
#else
// on windows Log_InstanceMutex_once is not called before Log is used!?.
static pthread_mutex_t Log_InstanceMutex_ = PTHREAD_ERRORCHECK_MUTEX_INITIALIZER;
#endif
static Log * Log_P_instance_ = 0;
static pthread_once_t Log_InstanceMutex_once = PTHREAD_ONCE_INIT;

Log_init::Log_init()
{
  int rc = pthread_once( &Log_InstanceMutex_once, Log::Initialization );
  if (rc != 0)
    {
      perror("Log_init::Log_init once Initialization failed");
      exit(1);
    }
}

Log_init::~Log_init()
{
  Log::Release();
}


const Log::Severity Log::NONE    =  0;
const Log::Severity Log::ALL     = ~0;

const Log::Severity Log::DBG     = 1 << 0;
const Log::Severity Log::WRAPPER = 1 << 1;
const Log::Severity Log::INFO    = 1 << 2;
const Log::Severity Log::USER    = 1 << 3;
const Log::Severity Log::WARN    = 1 << 4;
const Log::Severity Log::ERROR   = 1 << 5;
const Log::Severity Log::TRACE   = 1 << 6;

const Log::Severity Log::DEFAULT = Log::USER | Log::WARN | Log::ERROR | Log::TRACE;

static volatile Log::Severity Log_Severity_ = Log::DEFAULT;


/* Constructor */
Log::Log()
  : logName_(),
    openturnsLogSeverityVariableName_("OPENTURNS_LOG_SEVERITY"),
    p_file_(0),
    previousMessage_(),
    count_(0),
    repeat_(1)
{
  logName_[NONE]    = make_prefix( String(TTY::GetColor(TTY::DEFAULT))                                    , "", "   " );
  logName_[ALL]     = make_prefix( String(TTY::GetColor(TTY::DEFAULT))                                    , "", "ALL" );
  logName_[DBG]     = make_prefix( String(TTY::GetColor(TTY::DEFAULT))                                    , "", "DBG" );
  logName_[WRAPPER] = make_prefix( String(TTY::GetColor(TTY::BLUEFG))                                     , "", "WRP" );
  logName_[INFO]    = make_prefix( String(TTY::GetColor(TTY::BLUEFG))                                     , "", "INF" );
  logName_[USER]    = make_prefix( String(TTY::GetColor(TTY::BLUEFG))                                     , "", "USR" );
  logName_[WARN]    = make_prefix( String(TTY::GetColor(TTY::PURPLEFG)) + String(TTY::GetColor(TTY::BOLD)), "", "WRN" );
  logName_[ERROR]   = make_prefix( String(TTY::GetColor(TTY::REDFG))    + String(TTY::GetColor(TTY::BOLD)), "", "ERR" );
  logName_[TRACE]   = make_prefix( String(TTY::GetColor(TTY::YELLOWFG))                                   , "", "TRA" );

  initSeverityFromEnvironment();

  // Registration of destructor at exit
  //std::atexit(Log::Release);
}


/* Destructor */
Log::~Log()
{
  delete p_file_;
  p_file_ = 0;
}


/* Set Severity according to Openturns LogSeverity Variable */
void Log::initSeverityFromEnvironment()
{
  const char * logSeverityVariableContent = getenv(openturnsLogSeverityVariableName_);
  if (logSeverityVariableContent != NULL)
    {
      String severityVariableContent(logSeverityVariableContent);
      Severity theSeverity = Log::NONE;

      const char delim = ',';
      SignedInteger begPos = 0, endPos;
      do
        {
          // search token
          endPos = severityVariableContent.find(delim, begPos);
          if (endPos == static_cast<SignedInteger>(String::npos))
            endPos = severityVariableContent.size();
          const String token(severityVariableContent.substr(begPos, endPos - begPos));

          // add severity
          std::map<Severity, _Prefix >::const_iterator iter;
          for (iter = logName_.begin(); iter != logName_.end(); ++iter)
            if (token == iter->second.prefix_)
              theSeverity |= iter->first;

          // next token
          begPos = endPos + 1;
        }
      while (endPos != static_cast<SignedInteger>(severityVariableContent.size()));

      Show( theSeverity );
    }
}


Log & Log::GetInstance()
{
  if (!Log_P_instance_)
    {
      Log_P_instance_ = new Log;
      assert(Log_P_instance_);
      Log_P_instance_->push(Entry(INFO, "*** Log Beginning ***"));
    }
  return *Log_P_instance_;
}


void Log::Initialization()
{
#ifndef WIN32
  pthread_mutexattr_t attr;
  pthread_mutexattr_init( &attr );
  //pthread_mutexattr_settype( &attr, PTHREAD_MUTEX_NORMAL );
  pthread_mutexattr_settype( &attr, PTHREAD_MUTEX_ERRORCHECK );
  //pthread_mutexattr_settype( &attr, PTHREAD_MUTEX_RECURSIVE );
  int rc = pthread_mutex_init( &Log_InstanceMutex_, &attr );
  if (rc != 0)
    {
      perror("Log::Initialization mutex initialization failed");
      exit(1);
    }
#endif
  Log_P_instance_ = 0;
}



void Log::Release()
{
  if (Log_P_instance_) Log_P_instance_->push(Entry(INFO, "*** Log End ***"));
  delete Log_P_instance_;
  Log_P_instance_ = 0;
}



void Log::Reset()
{
  //Release();
  //Initialization();
}


/* Log messages according to its relative severity */
void Log::Debug(const String & msg)
{
  MutexLock lock( Log_InstanceMutex_ );
  Log::GetInstance().push(Entry(DBG, msg));
}



/* Log messages according to its relative severity */
void Log::Wrapper(const String & msg)
{
  MutexLock lock( Log_InstanceMutex_ );
  Log::GetInstance().push(Entry(WRAPPER, msg));
}



/*  Log messages according to its relative severity */
void Log::Info(const String & msg)
{
  MutexLock lock( Log_InstanceMutex_ );
  Log::GetInstance().push(Entry(INFO, msg));
}



/*  Log messages according to its relative severity */
void Log::User(const String & msg)
{
  MutexLock lock( Log_InstanceMutex_ );
  Log::GetInstance().push(Entry(USER, msg));
}



/* Log messages according to its relative severity */
void Log::Warn(const String & msg)
{
  MutexLock lock( Log_InstanceMutex_ );
  Log::GetInstance().push(Entry(WARN, msg));
}



/* Log messages according to its relative severity */
void Log::Error(const String & msg)
{
  MutexLock lock( Log_InstanceMutex_ );
  Log::GetInstance().push(Entry(ERROR, msg));
}


/* Log messages according to its relative severity */
void Log::Trace(const String & msg)
{
  MutexLock lock( Log_InstanceMutex_ );
  Log::GetInstance().push(Entry(TRACE, msg));
}


/* Get/Set the severity flags for the messages logged to the file */
void Log::Show(Severity flags)
{
  Atomic::Reset( (int*) & Log_Severity_);
  Atomic::OrAndFetch(  (int*) & Log_Severity_, flags );
}

Log::Severity Log::Flags()
{
  return Atomic::OrAndFetch(  (int*) & Log_Severity_, 0x00 );
}

/* Flush pending messages */
void Log::Flush()
{
  MutexLock lock( Log_InstanceMutex_ );
  Log::GetInstance().flush();
}


/* Does Log show repeated messages or not
 *  If repeat is false then Log shows every messages it receives
 *  even if they are identical to the previous ones.
 *  If repeat is true then Log only shows the first message
 *  and a message couting how much identical messages were
 *  received after that.
 */
void Log::Repeat( Bool r )
{
  MutexLock lock( Log_InstanceMutex_ );
  Log::GetInstance().repeat( r );
}

void Log::repeat( Bool r )
{
  repeat_ = r ? 1 : 0;
}

void Log::flush()
{
  printRepeatedMessage( previousMessage_ );
  previousMessage_ = Entry();
  count_ = 0;
}


/* Append an entry at the end of the list */
void Log::push(const Entry & entry)
{
  std::ostream & os = p_file_ ? *p_file_ : std::clog;
  if (entry.sev_ & Log::Flags())
    {
      if (entry.sev_ != Log::TRACE && repeat_.get() && entry == previousMessage_) ++count_;
      else
        {
          printRepeatedMessage( previousMessage_ );
          previousMessage_ = entry ;
          count_ = 0;
          os << logName_[entry.sev_] << " - " << entry.msg_ << TTY::GetColor(TTY::DEFAULT) << std::endl;
        }
    }
}

void Log::printRepeatedMessage(const Entry & entry)
{
  std::ostream & os = p_file_ ? *p_file_ : std::clog;
  if (count_ > 0)
    os << logName_[entry.sev_] << " - (previous message repeated " << count_ << " time" << ((count_ == 1) ? "" : "s") << ")" << TTY::GetColor(TTY::DEFAULT) << std::endl;
}

/* Set the name of the log file */
void Log::SetFile(const FileName & file)
{
  MutexLock lock( Log_InstanceMutex_ );
  Log::GetInstance().setFile(file);
}


/* Set the name of the log file */
void Log::setFile(const FileName & file)
{
  push(Entry(INFO, String("Diverting log to file: ") + file));
  push(Entry(INFO, "*** Log End ***"));
  delete p_file_;
  TTY::ShowColors( false );
  p_file_ = new std::ofstream(file.c_str());

  push(Entry(INFO, "*** Log Beginning ***"));
}

/* Color accessor */
void Log::SetColor(const Log::Severity severity,
                   const TTY::Color color)
{
  MutexLock lock( Log_InstanceMutex_ );
  Log::GetInstance().setColor(severity, String(TTY::GetColor(color)));
}

void Log::SetColor(const Log::Severity severity,
                   const String & color)
{
  MutexLock lock( Log_InstanceMutex_ );
  Log::GetInstance().setColor(severity, color);
}


void Log::setColor(const Log::Severity severity,
                   const String & color)
{
  if (logName_.find(severity) != logName_.end()) logName_[severity].color_ = color;
}

/* Get the color */
String Log::GetColor(const Log::Severity severity)
{
  MutexLock lock( Log_InstanceMutex_ );
  return Log::GetInstance().getColor(severity);
}

String Log::getColor(const Log::Severity severity) const
{
  if (logName_.find(severity) != logName_.end()) return logName_[severity].color_;
  return String("");
}

END_NAMESPACE_OPENTURNS

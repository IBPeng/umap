//////////////////////////////////////////////////////////////////////////////
// Copyright 2017-2019 Lawrence Livermore National Security, LLC and other
// UMAP Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: LGPL-2.1-only
//////////////////////////////////////////////////////////////////////////////
#include "umap/util/Logger.hpp"

#include <iostream>   // for std::cout, std::cerr
#include <mutex>
#include <stdlib.h>   // for getenv()
#include <strings.h>  // for strcasecmp()

namespace Umap {

static const char* env_name = "UMAP_LOG_LEVEL";
static message::Level defaultLevel = message::Info;
std::mutex g_logging_mutex;
Logger* Logger::s_Logger = nullptr;

static const std::string MessageLevelName[ message::Num_Levels ] = {
  "ERROR",
  "WARNING",
  "INFO",
  "DEBUG"
};

Logger::Logger() noexcept
{
  // by default, all message streams are disabled
  for ( int i=0 ; i < message::Num_Levels ; ++i )
    m_isEnabled[ i ] = false;
}

Logger::~Logger() noexcept
{
}

void Logger::setLoggingMsgLevel( message::Level level ) noexcept
{
  for ( int i=0 ; i < message::Num_Levels ; ++i )
    m_isEnabled[ i ] = (i<= level) ? true : false;
}

void Logger::logMessage( message::Level level,
                         const std::string& message,
                         const std::string& fileName,
                         int line ) noexcept
{
  if ( !logLevelEnabled( level ) )
    return;   /* short-circuit */

  std::lock_guard<std::mutex> guard(g_logging_mutex);
  std::cout
    << "[" << MessageLevelName[ level ] << "]"
    << "[" << fileName  << ":" << line << "]:"
    << message
    << std::endl;
}

void Logger::initialize()
{
  if ( s_Logger != nullptr )
    return;

  message::Level level = defaultLevel;
  char* enval = getenv(env_name);

  if ( enval != NULL ) {
    for ( int i = 0; i < message::Num_Levels; ++i ) {
      if ( strcasecmp( enval, MessageLevelName[ i ].c_str() ) == 0 ) {
        level = (message::Level)i;
        break;
      }
    }
  }

  s_Logger = new Logger();
  s_Logger->setLoggingMsgLevel(level);
}

void Logger::finalize()
{
  delete s_Logger;
  s_Logger = nullptr;
}

Logger* Logger::getActiveLogger()
{
  if ( s_Logger == nullptr )
    Logger::initialize();

  return s_Logger;
}

} /* namespace umap */

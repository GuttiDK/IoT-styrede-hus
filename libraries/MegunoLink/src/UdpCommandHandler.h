#pragma once
#include <Arduino.h>
#include <Stream.h>

#include "UdpParser.h"
#include "UdpManager.h"

template <int MAX_COMMANDS = 10, int RECEIVE_BUFFER_SIZE = 60, int MAX_VARIABLES = 10> class UdpCommandHandler 
  : public MLP::CommandDispatcherBase, public MLP::UdpParser
{
  // Array of commands we can match & dispatch. 
  MLP::CommandCallback m_Commands[MAX_COMMANDS];

  // Array of variables we can match & set/print
  MLP::VariableMap m_Variables[MAX_VARIABLES];

  // Buffer for receiving packets into . 
  char m_achBuffer[RECEIVE_BUFFER_SIZE];

public:

  UdpCommandHandler(UdpManager& rConnectionManager)
    : CommandDispatcherBase(m_Commands, MAX_COMMANDS, m_Variables, MAX_VARIABLES)
    , UdpParser(*(static_cast<MLP::CommandDispatcherBase*>(this)), m_achBuffer, sizeof(m_achBuffer), rConnectionManager.GetConnection())
  {
  }
};

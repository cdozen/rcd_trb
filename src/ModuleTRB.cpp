/********************************************************/
/*							*/
/* Date: 24 April 2015					*/ 
/* Author: Markus Joos					*/
/*							*/
/*** C 2015 - The software with that certain something **/

#include <unistd.h>
#include <iostream>
#include "rcc_error/rcc_error.h"
#include "DFDebug/DFDebug.h"
#include "ROSUtilities/ROSErrorReporting.h"
#include "ROSModules/ModulesException.h"
#include "rcd_trb/trb.h"
#include "rcd_trb/DataChannelTrb.h"
#include "rcd_trb/ModuleTrb.h"
#include "rcd_trb/ExceptionTrb.h"


using namespace ROS;
using namespace RCD;


/**********************/
ModuleTRB::ModuleTRB()
/**********************/
{ 
  DEBUG_TEXT(DFDB_RCDEXAMPLE, 15, "ModuleTRB::constructor: Entered");
}


/**********************************/
ModuleTRB::~ModuleTRB()   noexcept
/**********************************/
{
  DEBUG_TEXT(DFDB_RCDEXAMPLE, 15, "ModuleTRB::destructor: Entered");

  while (m_dataChannels.size() > 0) 
  {
    DataChannel *channel = m_dataChannels.back();
    m_dataChannels.pop_back();
    delete channel;		// we created them
  }
}


/************************************************************/
void ModuleTRB::setup(DFCountedPointer<Config> configuration) 
/************************************************************/
{ 
  DEBUG_TEXT(DFDB_RCDEXAMPLE, 15, "ModuleTRB::setup: Entered");

  m_configuration = configuration;

  //get IP parameters for the TRB MODULE
  m_port = configuration->getInt("IPport");
  m_id = configuration->getInt("channelId", 0);
  
  DEBUG_TEXT(DFDB_RCDEXAMPLE, 20, "VMEReadoutModuleUser::setup: IPport = " << HEX(m_port));
}


/*******************************************************/
void ModuleTRB::configure(const daq::rc::TransitionCmd&)
/*******************************************************/
{
  DEBUG_TEXT(DFDB_RCDEXAMPLE, 15, "ModuleTRB::configure: Entered");

  //open UDP oport...

  DataChannelTRB *channel = new DataChannelTRB(m_id, 0, m_port, m_configuration);  //adapt to IP parameters
  m_dataChannels.push_back(channel);
  DEBUG_TEXT(DFDB_RCDEXAMPLE, 15, "ModuleTRB::configure: Done");
  
  //Initialize the TRB/FEB card
  //...  
}


/*********************************************************/
void ModuleTRB::unconfigure(const daq::rc::TransitionCmd&)
/*********************************************************/
{
  DEBUG_TEXT(DFDB_RCDEXAMPLE, 15, "ModuleTRB::unconfigure: Entered");

  //close the IP port

  DEBUG_TEXT(DFDB_RCDEXAMPLE, 15, "ModuleTRB::unconfigure: Done");
}


/*****************************************************/
void ModuleTRB::connect(const daq::rc::TransitionCmd&)
/*****************************************************/
{
  DEBUG_TEXT(DFDB_RCDEXAMPLE, 20, "ModuleTRB::connect: Entered");
  DEBUG_TEXT(DFDB_RCDEXAMPLE, 15, "ModuleTRB::connect: Done");
}


/********************************************************/
void ModuleTRB::disconnect(const daq::rc::TransitionCmd&)
/********************************************************/
{
  DEBUG_TEXT(DFDB_RCDEXAMPLE, 15, "ModuleTRB::disconnect: Entered");
  DEBUG_TEXT(DFDB_RCDEXAMPLE, 15, "ModuleTRB::disconnect: Done");
}
    

/***********************************************************/    
void ModuleTRB::prepareForRun(const daq::rc::TransitionCmd&)
/***********************************************************/    
{
  DEBUG_TEXT(DFDB_RCDEXAMPLE, 15, "ModuleTRB::prepareForRun: Entered");
 
  //Tell the FEB to forget old data thay it may have buffered
 
  DEBUG_TEXT(DFDB_RCDEXAMPLE, 15, "ModuleTRB::prepareForRun: Done");
}


/****************************************************/
void ModuleTRB::stopDC(const daq::rc::TransitionCmd&)
/****************************************************/
{
  DEBUG_TEXT(DFDB_RCDEXAMPLE, 15, "ModuleTRB::stopDC: Entered");
  DEBUG_TEXT(DFDB_RCDEXAMPLE, 15, "ModuleTRB::stopDC: Done");
}


/*********************************************/
DFCountedPointer <Config> ModuleTRB::getInfo()
/*********************************************/
{
  DEBUG_TEXT(DFDB_RCDEXAMPLE, 15, "ModuleTRB::getInfo: Entered");
  DFCountedPointer<Config> info = Config::New();

  //info->set("TRB Firmware revision", m_TRB->firmware_revision);   //replace by something useful
   
  DEBUG_TEXT(DFDB_RCDEXAMPLE, 15, "ModuleTRB::getInfo: Done");
  return(info);
}


/**************************/
void ModuleTRB::clearInfo()
/**************************/
{
  DEBUG_TEXT(DFDB_RCDEXAMPLE, 15, "ModuleTRB::clearInfo: Entered");
  DEBUG_TEXT(DFDB_RCDEXAMPLE, 15, "ModuleTRB::clearInfo: Done");
}

//FOR THE PLUGIN FACTORY
extern "C" 
{
  extern ReadoutModule* createReadoutModuleTRB();
}
ReadoutModule* createReadoutModuleTRB()
{
  return (new ModuleTRB());
}





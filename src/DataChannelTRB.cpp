/********************************************************/
/*							*/
/* Date: 24 April 2015					*/ 
/* Author: Markus Joos					*/
/*							*/
/*** C 2015 - The software with that certain something **/

#include <stdint.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "ROSUtilities/ROSErrorReporting.h"
#include "DFDebug/DFDebug.h"
#include "DFSubSystemItem/Config.h"
#include "rcd_trb/ExceptionTrb.h"
#include "rcd_trb/trb.h"
#include "rcd_trb/DataChannelTrb.h"


using namespace ROS;
using namespace RCD;


/*******************************************************************************************/
DataChannelTRB::DataChannelTRB(u_int channelId,
				 u_int channelIndex,
				 int trbsocket,
				 DFCountedPointer<Config> configuration,
				 DataChannelTRBInfo *info) :
  SingleFragmentDataChannel(channelId, channelIndex, 0, configuration, info)
/*******************************************************************************************/
{ 
  DEBUG_TEXT(DFDB_RCDEXAMPLE, 15, "DataChannelTRB::constructor: Entered");
  m_statistics = info;
  m_channelId = channelId;
  m_socket = trbsocket;
  DEBUG_TEXT(DFDB_RCDEXAMPLE, 20, "DataChannelTRB::constructor: channelId = " << m_channelId);
  DEBUG_TEXT(DFDB_RCDEXAMPLE, 20, "DataChannelTRB::constructor: sock = " << m_socket);
  DEBUG_TEXT(DFDB_RCDEXAMPLE, 15, "DataChannelTRB::constructor: Done");
}


/*********************************/
DataChannelTRB::~DataChannelTRB() 
/*********************************/
{
  DEBUG_TEXT(DFDB_RCDEXAMPLE, 15, "DataChannelTRB::destructor: Entered");
  DEBUG_TEXT(DFDB_RCDEXAMPLE, 15, "DataChannelTRB::destructor: Done");
}


/*****************************************************************************************************/
int DataChannelTRB::getNextFragment(u_int* buffer, int max_size, u_int* status, u_long /*pciAddress*/)
/*****************************************************************************************************/
{
  int fsize = 0;

  DEBUG_TEXT(DFDB_RCDEXAMPLE, 15, "DataChannelTRB::getNextFragment: Entered for channel = " << m_channel_number);
  DEBUG_TEXT(DFDB_RCDEXAMPLE, 15, "DataChannelTRB::getNextFragment: max_size = " << max_size);

  u_int* bufPtr = buffer;        // the buffer address

  //Wait until there is an event ready
  //poll UDP port
  // there should always be a packet from the TRB generated by the trigger
  // if polling and timeout is required, see udp_sender_receiver for an example of poll + timeout
  // the protocol is based on this packet format:
  // header   8 words with first word being the length of header + payload
  // trailer  8 words: copy of the first 8 words
  //

  errno = 0;
  int n_bytes = recv(m_socket,bufPtr,sizeof(unsigned int),MSG_PEEK);    // read length into bufptr[0]
  DEBUG_TEXT(DFDB_RCDEXAMPLE, 15, "DataChannelTRB::getNextFragment:  read first word: packet length = " << n_bytes);
  if (n_bytes< 0) {
    DEBUG_TEXT(DFDB_RCDEXAMPLE, 15, "DataChannelTRB::getNextFragment: Failed to receive first word");
    char* strerr = strerror(errno);
    CREATE_ROS_EXCEPTION(ex1, TRBException, UDP_RECEIVE, strerr);
    throw ex1;
  }

  int n_max_bytes = bufPtr[0]+8*sizeof(unsigned int);
  if (n_max_bytes > max_size) {
    DEBUG_TEXT(DFDB_RCDEXAMPLE, 15, "DataChannelTRB::getNextFragment: length > max buffer size");
    CREATE_ROS_EXCEPTION(ex1, TRBException, UDP_RECEIVE, "length > max buffer size");
    throw ex1;
  }

  errno = 0;
  n_bytes = recv(m_socket,bufPtr,n_max_bytes,0);    // read whole packet
  DEBUG_TEXT(DFDB_RCDEXAMPLE, 15, "DataChannelTRB::getNextFragment:  packet length = " << n_bytes);
  if (n_bytes != n_max_bytes) {
    DEBUG_TEXT(DFDB_RCDEXAMPLE, 15, "DataChannelTRB::getNextFragment: Failed to read packet");
    char* strerr = strerror(errno);
    CREATE_ROS_EXCEPTION(ex1, TRBException, UDP_RECEIVE, strerr);
    throw ex1;
  }

  fsize += n_bytes;


  *status = S_OK;

  DEBUG_TEXT(DFDB_RCDEXAMPLE, 15, "DataChannelTRB::getNextFragment getNextFragment with fsize = " << fsize);
  return fsize;	// bytes ..
}


/**********************************/
ISInfo *DataChannelTRB::getISInfo()
/**********************************/
{
  DEBUG_TEXT(DFDB_RCDEXAMPLE, 15, "TRBDataChannel::getIsInfo: called");
  DEBUG_TEXT(DFDB_RCDEXAMPLE, 15, "TRBDataChannel::getIsInfo: done");
  return m_statistics;
}


/************************************/
void DataChannelTRB::clearInfo(void)
/************************************/
{
  DEBUG_TEXT(DFDB_RCDEXAMPLE, 5, "TRBDataChannel::clearInfo: entered");
  SingleFragmentDataChannel::clearInfo();
  DEBUG_TEXT(DFDB_RCDEXAMPLE, 5, "TRBDataChannel::clearInfo: done");
}





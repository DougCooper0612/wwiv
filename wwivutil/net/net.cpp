/**************************************************************************/
/*                                                                        */
/*                          WWIV Version 5.x                              */
/*             Copyright (C)2015-2020, WWIV Software Services             */
/*                                                                        */
/*    Licensed  under the  Apache License, Version  2.0 (the "License");  */
/*    you may not use this  file  except in compliance with the License.  */
/*    You may obtain a copy of the License at                             */
/*                                                                        */
/*                http://www.apache.org/licenses/LICENSE-2.0              */
/*                                                                        */
/*    Unless  required  by  applicable  law  or agreed to  in  writing,   */
/*    software  distributed  under  the  License  is  distributed on an   */
/*    "AS IS"  BASIS, WITHOUT  WARRANTIES  OR  CONDITIONS OF ANY  KIND,   */
/*    either  express  or implied.  See  the  License for  the specific   */
/*    language governing permissions and limitations under the License.   */
/**************************************************************************/
#include "wwivutil/net/net.h"

#include <cstdio>
#include <iomanip>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include "core/command_line.h"
#include "core/file.h"
#include "core/strings.h"
#include "sdk/config.h"
#include "sdk/net/net.h"
#include "sdk/net/networks.h"

#include "wwivutil/net/dump_bbsdata.h"
#include "wwivutil/net/dump_callout.h"
#include "wwivutil/net/dump_connect.h"
#include "wwivutil/net/dump_contact.h"
#include "wwivutil/net/dump_packet.h"
#include "wwivutil/net/dump_subscribers.h"
#include "wwivutil/net/list.h"
#include "wwivutil/net/req.h"
#include "wwivutil/net/send.h"

using std::endl;
using std::make_unique;
using std::setw;
using std::string;
using std::unique_ptr;
using std::vector;
using wwiv::core::BooleanCommandLineArgument;
using namespace wwiv::sdk;

namespace wwiv {
namespace wwivutil {

bool NetCommand::AddSubCommands() {
  add(make_unique<DumpPacketCommand>());
  add(make_unique<DumpCalloutCommand>());
  add(make_unique<DumpConnectCommand>());
  add(make_unique<DumpContactCommand>());
  add(make_unique<DumpBbsDataCommand>());
  add(make_unique<wwiv::wwivutil::net::DumpSubscribersCommand>());
  add(make_unique<SubReqCommand>());
  add(make_unique<NetListCommand>());
  add(make_unique<wwiv::wwivutil::net::SubSendCommand>());
  return true;
}


}  // namespace wwivutil
}  // namespace wwiv

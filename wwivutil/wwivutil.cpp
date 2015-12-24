/**************************************************************************/
/*                                                                        */
/*                              WWIV Version 5.x                          */
/*             Copyright (C)1998-2004, WWIV Software Services             */
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
/*                                                                        */
/**************************************************************************/
#include "wwivutil.h"

#include <algorithm>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "core/command_line.h"
#include "core/file.h"
#include "core/strings.h"
#include "core/stl.h"
#include "sdk/config.h"

using std::clog;
using std::cout;
using std::endl;
using std::map;
using std::string;
using std::vector;
using namespace wwiv::core;
using namespace wwiv::strings;
using namespace wwiv::sdk;

namespace wwiv {
namespace wwivutil {

int dump_headers(const Config& config, const CommandLineCommand* command);

}  // namespace wwivutil
}  // namespace wwiv

using namespace wwiv::wwivutil;

int main(int argc, char *argv[]) {
  try {
    CommandLine cmdline(argc, argv, "network_number");
    cmdline.add({"bbsdir", "Main BBS Directory containing CONFIG.DAT", File::current_directory()});
    cmdline.add(BooleanCommandLineArgument("help", '?', "Displays Help", false));

    CommandLineCommand& messages = cmdline.add_command("messages", "Message Commands");
    CommandLineCommand& messages_dump_header = messages.add_command("dump", "Displays message header and text information");
    messages_dump_header.add({"start", "Starting message number.", "1"});
    messages_dump_header.add({"end", "Last message number..", "-1"});
    messages_dump_header.add(BooleanCommandLineArgument("all", "dumps everything, control lines too", false));
    messages.add(BooleanCommandLineArgument("help", '?', "Displays Help", false));

    try {
      if (!cmdline.Parse()) {
        clog << "Unable to parse command line." << endl;
        return 1;
      }
    } catch (const unknown_argument_error& e) {
      clog << "Unable to parse command line." << endl;
      clog << e.what() << endl;
      return 1;
    }

    if (argc <= 1 || !cmdline.subcommand_selected() || cmdline.arg("help").as_bool()) {
      cout << cmdline.GetHelp();
      return 0;
    }

    string bbsdir = cmdline.arg("bbsdir").as_string();
    Config config(bbsdir);
    if (!config.IsInitialized()) {
      clog << "Unable to load CONFIG.DAT.";
      return 1;
    }

    if (!cmdline.subcommand_selected()) {
      clog << "Unable to parse command line." << endl;
      clog << "No command selected." << endl;
      return 1;
    }

    const string command = cmdline.command()->name();

    if (command == "messages") {
      // TODO(rushfan): this may be crash here if your command line is like
      // "wwivutil messages d" since d isn't a valid command.
      if (!messages.subcommand_selected() || messages.arg("help").as_bool()) {
        cout << messages.GetHelp();
        return 1;
      }
      const string subcommand = messages.command()->name();
      if (messages.arg("help").as_bool()) {
        cout << messages.GetHelp();
      } else if (subcommand == "dump") {
        dump_headers(config, messages.command());
      } else {
        cout << "Invalid command: \"" << subcommand << "\"." << endl;
        cout << messages.GetHelp();
        return 1;
      }
    } else {
      cout << "Invalid command: \"" << command << "\"." << endl;
      cout << cmdline.GetHelp();
      return 1;
    }
  } catch (std::exception& e) {
    clog << e.what() << endl;
  }
  return 0;
}

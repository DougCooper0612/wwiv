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
#include "wwivutil/acs/acs.h"

#include "core/command_line.h"
#include "core/log.h"
#include "core/file.h"
#include "core/stl.h"
#include "core/strings.h"
#include "core/textfile.h"
#include "sdk/acs/eval.h"
#include "sdk/acs/uservalueprovider.h"
#include "sdk/acs/value.h"
#include "sdk/user.h"
#include "sdk/usermanager.h"
#include <iostream>
#include <string>

using std::cout;
using std::endl;
using wwiv::core::BooleanCommandLineArgument;
using namespace wwiv::core;
using namespace wwiv::sdk;
using namespace wwiv::sdk::acs;
using namespace wwiv::strings;

namespace wwiv::wwivutil::acs {

AcsCommand::AcsCommand() : UtilCommand("acs", "Evaluates an ACS expression for a user.") {}

std::string AcsCommand::GetUsage() const {
  std::ostringstream ss;
  ss << "Usage: " << std::endl << std::endl;
  ss << "  acs : Evaluates an ACS expression for a user." << std::endl;
  return ss.str();
}

int AcsCommand::Execute() {
  if (remaining().empty()) {
    std::cout << GetUsage() << GetHelp() << endl;
    return 2;
  }
  const auto expr = remaining().front();
  const auto user_number = iarg("user");

  UserManager userMgr(*config()->config());
  User user{};
  if (!userMgr.readuser_nocache(&user, user_number)) {
    LOG(ERROR) << "Failed to load user number " << user_number;
    return 1;
  }

  Eval eval(expr);
  eval.add("user", std::make_unique<UserValueProvider>(&user));

  bool result = eval.eval();
  std::cout << "Evaluate: '" << expr << "' ";
  if (!result) {
    std::cout << "evaluated to FALSE" << std::endl;
  } else {
    std::cout << "evaluated to TRUE" << std::endl;
  }
  std::cout << std::endl;

  if (eval.error()) {
    std::cout << "Expression has the following error: " << eval.error_text()
              << std::endl;
    return 1;
  }

  if (barg("debug")) {
    std::cout << "Execution Trace: " << std::endl;
    for (const auto& l : eval.debug_info()) {
      std::cout << "       + " << l << std::endl;
    }
  }

  return 0;
}

bool AcsCommand::AddSubCommands() {
  add_argument(BooleanCommandLineArgument{"debug", "Display expression evaluation at end of execution.", true});
  add_argument({"user", "user number to use while evaluating the expression", "1"});
  return true;
}

} // namespace wwiv

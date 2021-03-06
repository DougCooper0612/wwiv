/**************************************************************************/
/*                                                                        */
/*                            WWIV Version 5                              */
/*                Copyright (C)2020, WWIV Software Services               */
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
#ifndef __INCLUDED_SDK_ACS_VALUEPROIVDER_H__
#define __INCLUDED_SDK_ACS_VALUEPROIVDER_H__

#include "sdk/acs/value.h"
#include <memory>
#include <optional>
#include <string>

namespace wwiv::sdk::acs {


/** Provides a value for an identifier of the form "object.attribute" */
class ValueProvider {
public:
  ValueProvider(const std::string& prefix) : prefix_(prefix) {}
  virtual ~ValueProvider() = default;

  /** 
   * Optionally gets the attribute for this object.  name should just be
   * the 'attribute' and not the full object.attribute name. *
   */
  virtual std::optional<Value> value(const std::string& name) = 0;

private:
  const std::string prefix_;
};

}

#endif // __INCLUDED_SDK_ACS_EVAL_H__
/**************************************************************************/
/*                                                                        */
/*                              WWIV Version 5.x                          */
/*             Copyright (C)1998-2020, WWIV Software Services             */
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
#ifndef __INCLUDED_COMMON_IOBASE_H__
#define __INCLUDED_COMMON_IOBASE_H__

#include "core/strings.h"
#include "common/context.h"
#include "common/remote_io.h"
#include "sdk/user.h"
#include <chrono>
#include <functional>
#include <set>
#include <string>

namespace wwiv::common {

/**
 * Base class for Input and Output.
 *
 * To use this class you must set the following:
 * - LocalIO
 * - RemoteIO
 * - Context Provider
 *
 * These may be modified after being set, so RAII does not work.
 */
class IOBase {
public:
  typedef std::function<wwiv::common::Context&()> context_provider_t;
  IOBase() = default;
  ~IOBase() = default;

  virtual void SetLocalIO(LocalIO* local_io) { local_io_ = local_io; }
  [[nodiscard]] LocalIO* localIO() const noexcept { return local_io_; }

  void SetComm(RemoteIO* comm) { comm_ = comm; }
  [[nodiscard]] RemoteIO* remoteIO() const noexcept { return comm_; }

  /** Sets the provider for the session context */
  void set_context_provider(context_provider_t c) { context_provider_ = std::move(c); }

  wwiv::sdk::User& user();
  wwiv::common::SessionContext& sess();
  wwiv::common::SessionContext& sess() const;
  wwiv::common::Context& context();

protected:
  LocalIO* local_io_{nullptr};
  RemoteIO* comm_{nullptr};
  mutable context_provider_t context_provider_;
};

} // namespace wwiv::common


#endif // __INCLUDED_COMMON_IOBASE_H__

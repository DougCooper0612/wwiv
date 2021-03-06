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

#include "bbs/menuspec.h"
#include "bbs/bbs.h"
#include "bbs/bbsutl.h"
#include "bbs/chains.h"
#include "common/com.h"
#include "common/input.h"
#include "common/output.h"
#include "bbs/conf.h"
#include "bbs/confutil.h"
#include "bbs/defaults.h"
#include "bbs/instmsg.h"
#include "bbs/menusupp.h"
#include "bbs/mmkey.h"
#include "bbs/msgbase1.h"
#include "bbs/multinst.h"
#include "bbs/shortmsg.h"
#include "bbs/sr.h"
#include "bbs/sysoplog.h"
#include "bbs/utility.h"
#include "bbs/xfer.h"
#include "core/numbers.h"
#include "core/stl.h"
#include "core/strings.h"
#include "fmt/format.h"
#include "fmt/printf.h"
#include "sdk/chains.h"
#include "sdk/config.h"
#include "sdk/names.h"
#include "sdk/user.h"
#include "sdk/usermanager.h"
#include "sdk/files/files.h"
#include <string>

using std::string;
using namespace wwiv::core;
using namespace wwiv::menus;
using namespace wwiv::sdk;
using namespace wwiv::stl;
using namespace wwiv::strings;

/* ---------------------------------------------------------------------- */
/* menuspec.cpp - Menu Specific support functions                           */
/*                                                                        */
/* Functions that dont have any direct WWIV function go in here           */
/* ie, functions to help emulate other BBS's.                             */
/* ---------------------------------------------------------------------- */

static int FindDN(const std::string& dl_fn) {
  for (auto i = 0; i < a()->dirs().size(); i++) {
    if (iequals(a()->dirs()[i].filename, dl_fn)) {
      return i;
    }
  }
  return -1;
}

/**
 *  Download a file
 *
 *  dir_fn:  fname of your directory record
 *  dl_fn:   Filename to download
 *  bFreeDL: true if this is a free download
 *  bTitle:  true if title is to be shown with file info
 */
int MenuDownload(const std::string& dir_fn, const std::string& dl_fn, bool bFreeDL, bool bTitle) {
  int bOkToDL;
  User ur;
  bool abort = false;

  int dn = FindDN(dir_fn);

  if (dn == -1) {
    MenuSysopLog("DLDNF");                  /* DL - DIR NOT FOUND */
    return 0;
  }
  const auto& dir = a()->dirs()[dn];
  dliscan1(dir);

  int nRecordNumber = recno(dl_fn);
  if (nRecordNumber <= 0) {
    bin.checka(&abort);
    if (abort) {
      return -1;
    }
    MenuSysopLog("DLFNF"); /* DL - FILE NOT FOUND */
    return 0;
  }
  bool ok = true;
  while (nRecordNumber > 0 && ok && !a()->sess().hangup()) {
    a()->tleft(true);
    auto f = a()->current_file_area()->ReadFile(nRecordNumber);
    bout.nl();

    if (bTitle) {
      bout << "Directory  : " << dir.name << wwiv::endl;
    }
    bOkToDL = printfileinfo(&f.u(), dir);

    if (!ratio_ok()) {
      return -1;
    }
    if (bOkToDL || bFreeDL) {
      write_inst(INST_LOC_DOWNLOAD, a()->current_user_dir().subnum, INST_FLAGS_NONE);
      auto s1 = FilePath(dir.path, f);
      if (dir.mask & mask_cdrom) {
        s1 = FilePath(a()->sess().dirs().temp_directory(), f);
        if (!File::Exists(s1)) {
          File::Copy(FilePath(dir.path, f), s1);
        }
      }
      bool sent = false;
      if (bOkToDL == -1) {
        send_file(s1.string(), &sent, &abort, f.aligned_filename(), dn, -2L);
      } else {
        send_file(s1.string(), &sent, &abort, f.aligned_filename(), dn, f.numbytes());
      }

      if (sent) {
        if (!bFreeDL) {
          a()->user()->SetFilesDownloaded(a()->user()->GetFilesDownloaded() + 1);
          a()->user()->set_dk(a()->user()->dk() +
                                    static_cast<int>(bytes_to_k(f.numbytes())));
        }
        ++f.u().numdloads;
        if (a()->current_file_area()->UpdateFile(f, nRecordNumber)) {
          a()->current_file_area()->Save();
        }

        sysoplog() << "Downloaded '" << f << "'.";

        if (a()->config()->sysconfig_flags() & sysconfig_log_dl) {
          a()->users()->readuser(&ur, f.u().ownerusr);
          if (!ur.IsUserDeleted()) {
            if (date_to_daten(ur.GetFirstOn()) < f.u().daten) {
              const auto username_num = a()->names()->UserName(a()->usernum);
              ssm(f.u().ownerusr) << username_num << " downloaded '" << f.aligned_filename() << "' on " << date();
            }
          }
        }
      }

      bout.nl(2);
      bout.bprintf("Your ratio is now: %-6.3f\r\n", ratio());

      if (a()->sess().IsUserOnline()) {
        a()->UpdateTopScreen();
      }
    } else {
      bout << "\r\n\nNot enough time left to D/L.\r\n";
    }
    if (abort) {
      ok = false;
    } else {
      nRecordNumber = nrecno(dl_fn, nRecordNumber);
    }
  }
  return abort ? -1 : 1;
}

/**
 * Run a Door (chain)
 *
 * pszDoor = Door description to run
 * bFree  = If true, security on door will not back checked
 */
bool MenuRunDoorName(const char *pszDoor, bool bFree) {
  const auto door_number = FindDoorNo(pszDoor);
  return door_number >= 0 ? MenuRunDoorNumber(door_number, bFree) : false;
}

bool MenuRunDoorNumber(int nDoorNumber, bool bFree) {
  if (!bFree && !ValidateDoorAccess(nDoorNumber)) {
    return false;
  }

  run_chain(nDoorNumber);
  return true;
}

int FindDoorNo(const char *pszDoor) {
  for (size_t i = 0; i < a()->chains->chains().size(); i++) {
    if (iequals(a()->chains->at(i).description, pszDoor)) {
      return i;
    }
  }

  return -1;
}

bool ValidateDoorAccess(int nDoorNumber) {
  auto inst = inst_ok(INST_LOC_CHAINS, nDoorNumber + 1);
  const auto& c = a()->chains->at(nDoorNumber);
  if (inst != 0) {
    const auto inuse_msg = fmt::format("|#2Chain {} is in use on instance {}.  ", c.description, inst);
    if (!(c.multi_user)) {
      bout << inuse_msg << " Try again later.\r\n";
      return false;
    }
    bout << inuse_msg << " Care to join in? ";
    if (!(bin.yesno())) {
      return false;
    }
  }
  if (c.ansi && !okansi()) {
    return false;
  }
  if (c.local_only && a()->sess().using_modem()) {
    return false;
  }
  if (c.sl > a()->sess().effective_sl()) {
    return false;
  }
  if (c.ar && !a()->user()->HasArFlag(c.ar)) {
    return false;
  }
  if (a()->HasConfigFlag(OP_FLAGS_CHAIN_REG) 
      && a()->chains->HasRegisteredChains()
      && a()->sess().effective_sl() < 255) {
    if (c.maxage) {
      if (c.minage > a()->user()->age() || c.maxage < a()->user()->age()) {
        return false;
      }
    }
  }
  // passed all the checks, return true
  return true;
}


/* ----------------------- */
/* End of run door section */
/* ----------------------- */
void ChangeSubNumber() {
  bout << "|#7Select Sub number : |#0";

  const auto s = mmkey(MMKeyAreaType::subs);
  for (auto i = 0; i < ssize(a()->subs().subs()) && a()->usub[i].subnum != -1; i++) {
    if (s == a()->usub[i].keys) {
      a()->set_current_user_sub_num(i);
    }
  }
}

void ChangeDirNumber() {
  auto done = false;
  while (!done && !a()->sess().hangup()) {
    bout << "|#7Select Dir number : |#0";

    const auto s = mmkey(MMKeyAreaType::dirs);

    if (s[0] == '?') {
      DirList();
      bout.nl();
      continue;
    }
    for (auto i = 0; i < ssize(a()->dirs()); i++) {
      if (s == a()->udir[i].keys) {
        a()->set_current_user_dir_num(i);
        done = true;
      }
    }
  }
}


static void SetConf(ConferenceType t, int d) {
  auto info = get_conf_info(t);

  for (auto i = 0; i < ssize(info.uc) && info.uc[i].confnum != -1; i++) {
    if (d == info.confs[info.uc[i].confnum].designator) {
      setuconf(t, i, -1);
      break;
    }
  }
}

// have a little conference ability...
void SetMsgConf(int conf_designator) {
  SetConf(ConferenceType::CONF_SUBS, conf_designator);
}

void SetDirConf(int conf_designator) {
  SetConf(ConferenceType::CONF_DIRS, conf_designator);
}

void EnableConf() {
  tmp_disable_conf(false);
}

void DisableConf() {
  tmp_disable_conf(true);
}

void SetNewScanMsg() {
  sysoplog() << "Select Subs";
  config_qscan();
}

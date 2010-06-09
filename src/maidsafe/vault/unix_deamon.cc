/*
 * copyright maidsafe.net limited 2008
 * The following source code is property of maidsafe.net limited and
 * is not meant for external use. The use of this code is governed
 * by the license file LICENSE.TXT found in the root of this directory and also
 * on www.maidsafe.net.
 *
 * You are not free to copy, amend or otherwise use this source code without
 * explicit written permission of the board of directors of maidsafe.net
 *
 *  Created on: Apr 15, 2009
 *      Author: Team
 */

#include <boost/lexical_cast.hpp>

#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include "maidsafe/vault/vaultdaemon.h"
#ifdef MAIDSAFE_LINUX
  #include "google/breakpad/common/linux/linux_syscall_support.h"
  #include "google/breakpad/client/linux/handler/exception_handler.h"
#endif

#define LOGFILE "VaultService.txt"
const int kSleepTime = 10000;

static bool DumpCallback(const char*,
                         const char *dump_id,
                         void*,
                         bool succeeded) {
  if (succeeded) {
    printf("%s is dumped.\n", dump_id);
  }
  return succeeded;
}


int WriteToLog(char* str) {
  FILE* log;
  log = fopen(LOGFILE, "a+");
  if (log == NULL)
    return -1;
  fprintf(log, "%s\n", str);
  fclose(log);
  return 0;
}

int WriteToLog(std::string str) {
  FILE* log;
  log = fopen(LOGFILE, "a+");
  if (log == NULL)
    return -1;
  fprintf(log, "%s\n", str.c_str());
  fclose(log);
  return 0;
}

int main(int argc, char* argv[]) {
  if (argc != 4) {
    printf("vault: Invalid number of arguments\n");
    printf("Usage: vault [PORT] path_to_config_file K\n");
    return -1;
  } else {
    printf("arg[0]: %s\n", argv[0]);
    printf("arg[1]: %s\n", argv[1]);
    printf("arg[2]: %s\n", argv[2]);
    printf("arg[3]: %s\n", argv[2]);
  }

#ifdef MAIDSAFE_LINUX
  google_breakpad::ExceptionHandler eh(".", NULL, DumpCallback, NULL, true);
#endif

  std::string log_string;
  /* Our process ID and Session ID */
  pid_t pid, sid;

  /* Fork off the parent process */
  pid = fork();
  if (pid < 0) {
    exit(EXIT_FAILURE);
  }
  /* If we got a good PID, then
     we can exit the parent process. */
  if (pid > 0) {
    exit(EXIT_SUCCESS);
  }

  /* Change the file mode mask */
  umask(0);

  /* Open any logs here */
  /* Create a new SID for the child process */
  sid = setsid();
  if (sid < 0) {
    /* Log the failure */
    log_string = "Failed setsid()\n";
    WriteToLog(log_string);
    exit(EXIT_FAILURE);
  }

  int n = chdir("/tmp");
  /* Change the current working directory */
  if (n < 0) {
    /* Log the failure */
    log_string = "Failed chdir(/tmp): " + base::IntToString(n) + " \n";
    WriteToLog(log_string);
    exit(EXIT_FAILURE);
  }

  /* Close out the standard file descriptors */
  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);

  /* Daemon-specific initialization goes here */
  std::string prt(argv[1]), path_to_config(argv[2]), k(argv[3]);
  int port = boost::lexical_cast<int>(prt);
  boost::uint8_t K(boost::lexical_cast<boost::uint8_t>(k));
  maidsafe_vault::VaultDaemon vault_daemon(port, path_to_config, K);

  if (!vault_daemon.StartVault())
    exit(EXIT_FAILURE);
  /* The Big Loop */
  while (1) {
    /* Do some task here ... */
    /* we should check vaultdeamon still running */
    vault_daemon.Status();
    sleep(10);  /* wait 10 seconds */
  }
  exit(EXIT_SUCCESS);
}

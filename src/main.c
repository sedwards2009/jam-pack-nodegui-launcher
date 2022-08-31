/*
 * Copyright 2022 Simon Edwards <simon@simonzone.com>
 *
 * This source code is licensed under the MIT license which is detailed in the LICENSE.txt file.
 */
/**
 * Tiny program to set the CWD to match this executable
 * and then start up qode with the desired JS entry point.
 *
 */
#include <stdlib.h>
#include <stdio.h>

#if defined(_WIN32)
#include <direct.h>
#include <process.h>
#else
#include <unistd.h>
#endif

#define DG_MISC_IMPLEMENTATION
#include "DG_misc.h"


static char *targetProgramPath = "./node_modules/@nodegui/qode/binaries/qode";

// This is filled with a magic string which can be found later in the output binary exe and then overwritten.
static char *targetFirstArg =
  "4f8177788c5a4086ac9f18d8639b7717"
  "4f8177788c5a4086ac9f18d8639b7717"
  "4f8177788c5a4086ac9f18d8639b7717"
  "4f8177788c5a4086ac9f18d8639b7717"
  "4f8177788c5a4086ac9f18d8639b7717"
  "4f8177788c5a4086ac9f18d8639b7717"
  "4f8177788c5a4086ac9f18d8639b7717"
  "4f8177788c5a4086ac9f18d8639b7717";

int main(int argc, char *argv[], char *envp[]) {
  // Set cwd to the application directory.
  const char *exe_path = DG_GetExecutableDir();
  chdir(exe_path);

  // Pass on any command line arguments to qode.
  char *newArgv[argc+2];
  newArgv[0] = targetProgramPath;
  newArgv[1] = targetFirstArg;
  newArgv[2] = 0;
  for (int i=1; i<argc; i++) {
    newArgv[i+1] = argv[i];
  }
  newArgv[argc+1] = 0;

#if defined(_WIN32)
  _execve(targetProgramPath, newArgv, envp);
#else
  execve(targetProgramPath, newArgv, envp);
#endif
  /* In normal execution, this line will not be reached. */
  perror("Unable to run qode");
  return -1;
}

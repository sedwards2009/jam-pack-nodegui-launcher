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
#include <string.h>

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

#if defined(_WIN32)

/* 1KB */
static char *extraExePaths =
  "92c9c49a891d4061ba239c46fcf4c840"
  "92c9c49a891d4061ba239c46fcf4c840"
  "92c9c49a891d4061ba239c46fcf4c840"
  "92c9c49a891d4061ba239c46fcf4c840"
  "92c9c49a891d4061ba239c46fcf4c840"
  "92c9c49a891d4061ba239c46fcf4c840"
  "92c9c49a891d4061ba239c46fcf4c840"
  "92c9c49a891d4061ba239c46fcf4c840"
  "92c9c49a891d4061ba239c46fcf4c840"
  "92c9c49a891d4061ba239c46fcf4c840"
  "92c9c49a891d4061ba239c46fcf4c840"
  "92c9c49a891d4061ba239c46fcf4c840"
  "92c9c49a891d4061ba239c46fcf4c840"
  "92c9c49a891d4061ba239c46fcf4c840"
  "92c9c49a891d4061ba239c46fcf4c840"
  "92c9c49a891d4061ba239c46fcf4c840"
  "92c9c49a891d4061ba239c46fcf4c840"
  "92c9c49a891d4061ba239c46fcf4c840"
  "92c9c49a891d4061ba239c46fcf4c840"
  "92c9c49a891d4061ba239c46fcf4c840"
  "92c9c49a891d4061ba239c46fcf4c840"
  "92c9c49a891d4061ba239c46fcf4c840"
  "92c9c49a891d4061ba239c46fcf4c840"
  "92c9c49a891d4061ba239c46fcf4c840"
  "92c9c49a891d4061ba239c46fcf4c840"
  "92c9c49a891d4061ba239c46fcf4c840"
  "92c9c49a891d4061ba239c46fcf4c840"
  "92c9c49a891d4061ba239c46fcf4c840"
  "92c9c49a891d4061ba239c46fcf4c840"
  "92c9c49a891d4061ba239c46fcf4c840"
  "92c9c49a891d4061ba239c46fcf4c840"
  "92c9c49a891d4061ba239c46fcf4c840";

/* Construct a new Path variable based on the existing Path var and path fragments stored in extraExePaths. */
char *buildNewPathVar(char *oldPathVar, char *baseDir) {
  int baseDirLen = strlen(baseDir);

  if (strlen(extraExePaths) == 0) {
    return oldPathVar;
  }

  const char *scanPtr = extraExePaths;
  int numOfPaths = 1;
  while((scanPtr = strstr(scanPtr, ";")) != NULL) {
    numOfPaths++;
    scanPtr++;
  }

  int newVarLen = sizeof(char *) * ((baseDirLen + 1) * numOfPaths + strlen(extraExePaths) + strlen(oldPathVar) +
                    1 + strlen("Path="));
  char *newPathVar = (char *)malloc(newVarLen);
  newPathVar[0] = 0;
  strcat(newPathVar, "Path=");

  char *startPtr = extraExePaths;
  char *scanPtr2 = extraExePaths;
  while((scanPtr2 = strstr(scanPtr2, ";")) != NULL) {
    strcat(newPathVar, baseDir);

    *scanPtr2 = 0;
    strcat(newPathVar, startPtr);

    scanPtr2++;
    startPtr = scanPtr2;

    strcat(newPathVar, ";");
  }
  strcat(newPathVar, oldPathVar + strlen("Path="));

  return newPathVar;
}

char **patchEnvPathVar(char *envp[], char *baseDir) {
  int numVars = 0;
  while (envp[numVars] != NULL) {
    numVars++;
  }

  int envListSize = sizeof(char *) * (numVars + 1);
  char **newEnv = (char **)malloc(envListSize);
  memcpy(newEnv, envp, envListSize);

  for (int i=0; newEnv[i]!=0; i++) {
    if (strstr(newEnv[i], "Path=") == newEnv[i]) {
      newEnv[i] = buildNewPathVar(newEnv[i], baseDir);
      break;
    }
  }
  return newEnv;
}
#endif

int main(int argc, char *argv[], char *envp[]) {
  // Set cwd to the application directory.
  const char *exe_path = DG_GetExecutableDir();
#if defined(_WIN32)
  _chdir(exe_path);
#else
  chdir(exe_path);
#endif

  // Pass on any command line arguments to qode.
  char **newArgv = malloc((sizeof(char *)) * (argc+2));
  newArgv[0] = targetProgramPath;
  newArgv[1] = targetFirstArg;
  newArgv[2] = 0;
  for (int i=1; i<argc; i++) {
    newArgv[i+1] = argv[i];
  }
  newArgv[argc+1] = 0;

#if defined(_WIN32)
  char **newEnvp = patchEnvPathVar(envp, exe_path);
  _execve(targetProgramPath, newArgv, newEnvp);
  return 0;
#else
  execve(targetProgramPath, newArgv, envp);
  /* In normal execution, this line will not be reached. */
  perror("Unable to run qode");
  return -1;
#endif
}

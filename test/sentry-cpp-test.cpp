/********************************************//**
* @file sentry-cpp-test.cpp
* @brief Begin Tests for the Environment
* @details https://docs.sentry.io/clientdev/interfaces/message/
* @author James Sullivan
* @version
* @copyright CadActive Technologies, LLC
***********************************************/
// Memory Leak Handling
#include <crtdbg.h>
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

// Google Test
#include "gtest\gtest.h"

/***********************************************
*	Functions
***********************************************/
/*! @brief Start the tests
*/
int main(int argc, char *argv[]) {
  bool run_manual_tests = false;

  // Turn on debugging for memory leaks.
  HANDLE hLogFile;
  hLogFile = CreateFile("memory_leaks.txt", GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
  _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
  _CrtSetReportFile(_CRT_WARN, hLogFile);
  _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
  _CrtSetReportFile(_CRT_ERROR, hLogFile);
  _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
  _CrtSetReportFile(_CRT_ASSERT, hLogFile);

  // The method is initializes the Google framework and must be called before RUN_ALL_TESTS
  ::testing::InitGoogleTest(&argc, argv);

  // Save a snapshot of memory, so we get an accurate memory leak report
  _CrtMemState memoryState = { 0 };
  _CrtMemCheckpoint(&memoryState);

  // RUN_ALL_TESTS automatically detects and runs all the tests defined using the TEST macro.
  // It's must be called only once in the code because multiple calls lead to conflicts and,
  // therefore, are not supported.
  int output = RUN_ALL_TESTS();

  // Report Leaks
  _CrtMemDumpAllObjectsSince(&memoryState);
  return output;
}
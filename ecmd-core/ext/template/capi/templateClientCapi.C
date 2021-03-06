//IBM_PROLOG_BEGIN_TAG
/* 
 * Copyright 2003,2016 IBM International Business Machines Corp.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * 	http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
 * implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
//IBM_PROLOG_END_TAG

/* NOTE : This file has been generated from the eCMD extension temp-late */
/* DO NOT EDIT THISFILE (unless you are editing the temp-late */



//----------------------------------------------------------------------
//  Includes
//----------------------------------------------------------------------
#include <stdio.h>
#include <dlfcn.h>

#include <ecmdReturnCodes.H>
#include <ecmdUtils.H>

#include <templateClientCapi.H>
#ifndef ECMD_STATIC_FUNCTIONS
#include <templateClientEnums.H>
#else
#include <templateDllCapi.H>
#endif

//----------------------------------------------------------------------
//  User Types
//----------------------------------------------------------------------

//----------------------------------------------------------------------
//  Constants
//----------------------------------------------------------------------

//----------------------------------------------------------------------
//  Macros
//----------------------------------------------------------------------

//----------------------------------------------------------------------
//  Internal Function Prototypes
//----------------------------------------------------------------------

//----------------------------------------------------------------------
//  Global Variables
//----------------------------------------------------------------------
#ifndef ECMD_STATIC_FUNCTIONS
/* This is from ecmdClientCapiFunc.C */
extern void * dlHandle;
/* These are from templateClientCapiFunc.C */
extern void * templateDllFnTable[];
#endif
extern bool templateInitialized;

//---------------------------------------------------------------------
// Member Function Specifications
//---------------------------------------------------------------------
uint32_t templateInitExtension() {

  int rc = ECMD_SUCCESS;

#ifndef ECMD_STATIC_FUNCTIONS
  if (dlHandle == NULL) {
    fprintf(stderr,"templateInitExtension: eCMD TEMPLATE Extension Initialization function called before DLL has been loaded\n");
    exit(ECMD_DLL_INVALID);
  }
#endif

  /* Only do this if it hasn't been done already */
  if (templateInitialized) {
    return ECMD_SUCCESS;
  }

#ifndef ECMD_STATIC_FUNCTIONS
  /* look for init function */
  uint32_t (*Function)(const char *) =
    (uint32_t(*)(const char *))(void*)dlsym(dlHandle, "dllTemplateInitExtension");
  if (!Function) {
    /* This extension is not supported by this plugin */
    rc = ECMD_EXTENSION_NOT_SUPPORTED;
  } else {
    rc = (*Function)(ECMD_TEMPLATE_CAPI_VERSION);
    if (!rc) templateInitialized = true;
  }
  
  /* Clear out the function table */
  for (int func = 0; func < TEMPLATE_NUMFUNCTIONS; func ++) {
    templateDllFnTable[func] = NULL;
  }
#else

  rc = dllTemplateInitExtension(ECMD_TEMPLATE_CAPI_VERSION);
  if (!rc) templateInitialized = true;

#endif /* ECMD_STATIC_FUNCTIONS */

  /* Now as part of defect 18081 we register to the core client that we have been initialized */
  ecmdRegisterExtensionInitState(&templateInitialized);

  if (rc) {
    std::string errorString;
    errorString = ecmdGetErrorMsg(rc, false, false, false);
    if (errorString.size()) ecmdOutput(errorString.c_str());
  }

  return rc;
}

/*
 * "The contents of this file are subject to the Mozilla Public License
 * Version 1.1 (the "License"); you may not use this file except in
 * compliance with the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS"
 * basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
 * License for the specific language governing rights and limitations
 * under the License.
 *
 * The Original Code is Todd A. Fisher code.
 *
 * The Initial Developer of the Original Code is Todd A. Fisher.
 * Portions created by the Initial Developer are Copyright (C) 2005
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s): 
 *
 */
#ifndef XM_MODULE_H
#define XM_MODULE_H

//#ifdef _DEBUG
#include <stdio.h>
//#endif
#include <nsIGenericFactory.h>

//#ifdef _DEBUG
#define DEBUG_OUT( msg ) printf( msg )
//#else
//#define DEBUG_OUT( msg )
//#endif

#define XM_GENERIC_REG_FUNC( _InstanceClass ) \
static NS_METHOD _InstanceClass##RegistrationProc(nsIComponentManager *aCompMgr, \
	                                          nsIFile *aPath, \
																						const char *registryLocation, \
																						const char *componentType, \
																						const nsModuleComponentInfo *info) \
{ \
	DEBUG_OUT( "Registering "#_InstanceClass" Component!\n" ); \
	return NS_OK; \
} \
static NS_METHOD _InstanceClass##UnregistrationProc(nsIComponentManager *aCompMgr, \
																							nsIFile *aPath, \
																							const char *registryLocation, \
																							const nsModuleComponentInfo *info) \
{ \
	DEBUG_OUT( "Unregistering "#_InstanceClass" Component!\n" ); \
	return NS_OK; \
}

#define GENERIC_MODULE_SPEC(class_name)\
	NS_GENERIC_FACTORY_CONSTRUCTOR(class_name) \
	XM_GENERIC_REG_FUNC(class_name) \
	NS_DECL_CLASSINFO(class_name)

#define XM_COMPONENT_SPEC( desc, CID, CONTRACTID, class_name ) {  \
		#desc, CID, CONTRACTID, class_name##Constructor, \
		class_name##RegistrationProc, \
		class_name##UnregistrationProc,\
		NULL, \
		NS_CI_INTERFACE_GETTER_NAME(class_name), \
		NULL, \
		&NS_CLASSINFO_NAME(class_name), 0 \
	}

#endif

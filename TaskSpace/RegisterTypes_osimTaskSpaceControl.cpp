/* -------------------------------------------------------------------------- *
 *                   OpenSim:  RegisterTypes_osimTaskSpaceControl.cpp         *
 * -------------------------------------------------------------------------- *
 * The OpenSim API is a toolkit for musculoskeletal modeling and simulation.  *
 * See http://opensim.stanford.edu and the NOTICE file for more information.  *
 * OpenSim is developed at Stanford University and supported by the US        *
 * National Institutes of Health (U54 GM072970, R24 HD065690) and by DARPA    *
 * through the Warrior Web program.                                           *
 *                                                                            *
 * Copyright (c) 2005-2012 Stanford University and the Authors                *
 *                                                                            *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may    *
 * not use this file except in compliance with the License. You may obtain a  *
 * copy of the License at http://www.apache.org/licenses/LICENSE-2.0.         *
 *                                                                            *
 * Unless required by applicable law or agreed to in writing, software        *
 * distributed under the License is distributed on an "AS IS" BASIS,          *
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.   *
 * See the License for the specific language governing permissions and        *
 * limitations under the License.                                             *
 * -------------------------------------------------------------------------- */

#include <string>
#include <iostream>
#include <OpenSim/Common/Object.h>
#include "RegisterTypes_osimTaskSpaceControl.h"

#include "Controller.h"

using namespace OpenSim;
using namespace std;

static dllObjectInstantiator instantiator; 
     
//_____________________________________________________________________________
/**
 * The purpose of this routine is to register all class types exported by
 * the TaskSpaceControl library.
 */
OSIMTASKSPACECONTROL_API void RegisterTypes_osimTaskSpaceControl()
{
	Object::RegisterType( TaskSpace::Controller() );
    Object::RegisterType( TaskSpace::PriorityLevel() );
    Object::RegisterType( TaskSpace::PriorityLevelSet() );
    Object::RegisterType( TaskSpace::Task() );
    Object::RegisterType( TaskSpace::TaskSet() );
    Object::RegisterType( TaskSpace::StationTask() );
    Object::RegisterType( TaskSpace::StationTrackingTask() );
    Object::RegisterType( TaskSpace::ConstantStationTrackingTask() );
}

dllObjectInstantiator::dllObjectInstantiator() 
{ 
        registerDllClasses(); 
} 
    
void dllObjectInstantiator::registerDllClasses() 
{ 
        RegisterTypes_osimTaskSpaceControl(); 
} 

/*-------------------------------------------------------------------------
 * drawElements Quality Program Tester Core
 * ----------------------------------------
 *
 * Copyright 2014 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 *//*!
 * \file
 * \brief EGL native display abstraction
 *//*--------------------------------------------------------------------*/

#include "egluNativeDisplay.hpp"

namespace eglu
{

// NativeDisplay

NativeDisplay::NativeDisplay (Capability capabilities, EGLenum platformType, const char* platformExtension)
	: m_capabilities		(capabilities)
	, m_platformType		(platformType)
	, m_platformExtension	(platformExtension)
{
	DE_ASSERT(platformType != EGL_NONE && platformExtension);
	DE_ASSERT(capabilities & CAPABILITY_GET_DISPLAY_PLATFORM);
}

NativeDisplay::NativeDisplay (Capability capabilities)
	: m_capabilities		(capabilities)
	, m_platformType		(EGL_NONE)
	, m_platformExtension	("")
{
	DE_ASSERT(!(capabilities & CAPABILITY_GET_DISPLAY_PLATFORM));
	DE_ASSERT(capabilities & CAPABILITY_GET_DISPLAY_LEGACY);
}

NativeDisplay::~NativeDisplay (void)
{
}

EGLNativeDisplayType NativeDisplay::getLegacyNative (void)
{
	TCU_CHECK_INTERNAL((m_capabilities & CAPABILITY_GET_DISPLAY_LEGACY) == 0);
	throw tcu::NotSupportedError("eglu::NativeDisplay can't be used with eglGetDisplay()", DE_NULL, __FILE__, __LINE__);
}

void* NativeDisplay::getPlatformNative (void)
{
	TCU_CHECK_INTERNAL((m_capabilities & CAPABILITY_GET_DISPLAY_PLATFORM) == 0);
	throw tcu::NotSupportedError("eglu::NativeDisplay can't be used with eglGetPlatformDisplay()", DE_NULL, __FILE__, __LINE__);
}

// NativeDisplayFactory

NativeDisplayFactory::NativeDisplayFactory (const std::string& name, const std::string& description, NativeDisplay::Capability capabilities, EGLenum platformType, const char* platformExtension)
	: FactoryBase			(name, description)
	, m_capabilities		(capabilities)
	, m_platformType		(platformType)
	, m_platformExtension	(platformExtension)
{
	DE_ASSERT(platformType != EGL_NONE && platformExtension);
	DE_ASSERT(capabilities & NativeDisplay::CAPABILITY_GET_DISPLAY_PLATFORM);
}

NativeDisplayFactory::NativeDisplayFactory (const std::string& name, const std::string& description, NativeDisplay::Capability capabilities)
	: FactoryBase			(name, description)
	, m_capabilities		(capabilities)
	, m_platformType		(EGL_NONE)
	, m_platformExtension	("")
{
	DE_ASSERT(!(capabilities & NativeDisplay::CAPABILITY_GET_DISPLAY_PLATFORM));
	DE_ASSERT(capabilities & NativeDisplay::CAPABILITY_GET_DISPLAY_LEGACY);
}

NativeDisplayFactory::~NativeDisplayFactory (void)
{
}

} // eglu

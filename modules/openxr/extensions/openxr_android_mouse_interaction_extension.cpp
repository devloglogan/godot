/**************************************************************************/
/*  openxr_android_mouse_interaction_extension.cpp                        */
/**************************************************************************/
/*                         This file is part of:                          */
/*                             GODOT ENGINE                               */
/*                        https://godotengine.org                         */
/**************************************************************************/
/* Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md). */
/* Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.                  */
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#include "openxr_android_mouse_interaction_extension.h"

#include "../action_map/openxr_interaction_profile_metadata.h"
#include "core/config/project_settings.h"

#include <openxr/openxr.h>

OpenXRAndroidMouseInteractionExtension *OpenXRAndroidMouseInteractionExtension::singleton = nullptr;

OpenXRAndroidMouseInteractionExtension *OpenXRAndroidMouseInteractionExtension::get_singleton() {
	return singleton;
}

OpenXRAndroidMouseInteractionExtension::OpenXRAndroidMouseInteractionExtension() {
	singleton = this;
}

OpenXRAndroidMouseInteractionExtension::~OpenXRAndroidMouseInteractionExtension() {
	singleton = nullptr;
}

HashMap<String, bool *> OpenXRAndroidMouseInteractionExtension::get_requested_extensions(XrVersion p_version) {
	HashMap<String, bool *> request_extensions;

	// Only enable this extension when requested.
	// We still register our meta data or the action map editor will fail.
	if (GLOBAL_GET("xr/openxr/extensions/androidxr/mouse_interaction_profile")) {
		request_extensions["XR_ANDROID_mouse_interaction"] = &available;
	}

	return request_extensions;
}

bool OpenXRAndroidMouseInteractionExtension::is_available() {
	return available;
}

void OpenXRAndroidMouseInteractionExtension::on_register_metadata() {
	OpenXRInteractionProfileMetadata *openxr_metadata = OpenXRInteractionProfileMetadata::get_singleton();
	ERR_FAIL_NULL(openxr_metadata);

	// Mouse interaction profile.
	const String profile_path = "/interaction_profiles/android/mouse_interaction_android";
	const String user_path = "/user/mouse";
	openxr_metadata->register_interaction_profile("Android mouse interaction", profile_path, "XR_ANDROID_mouse_interaction");

	openxr_metadata->register_io_path(profile_path, "Aim pose", user_path, user_path + "/input/aim/pose", "", OpenXRAction::OPENXR_ACTION_POSE);

	openxr_metadata->register_io_path(profile_path, "Select click", user_path, user_path + "/input/select/click", "", OpenXRAction::OPENXR_ACTION_BOOL);
	openxr_metadata->register_io_path(profile_path, "Secondary click", user_path, user_path + "/input/secondary_android/click", "", OpenXRAction::OPENXR_ACTION_BOOL);
	openxr_metadata->register_io_path(profile_path, "Tertiary click", user_path, user_path + "/input/tertiary_android/click", "", OpenXRAction::OPENXR_ACTION_BOOL);

	// openxr_metadata->register_io_path(profile_path, "Scroll", user_path, user_path + "/input/scroll_android/value", "", OpenXRAction::OPENXR_ACTION_FLOAT);
	openxr_metadata->register_io_path(profile_path, "Scroll", user_path, user_path + "/input/scroll_android", "", OpenXRAction::OPENXR_ACTION_VECTOR2);
}

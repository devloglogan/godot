/**************************************************************************/
/*  openxr_meta_recommended_layer_resolution_extension_wrapper.cpp        */
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

#include "openxr_meta_recommended_layer_resolution_extension_wrapper.h"

#include "../openxr_api.h"

OpenXRMetaRecommendedLayerResolutionExtensionWrapper *OpenXRMetaRecommendedLayerResolutionExtensionWrapper::singleton = nullptr;

OpenXRMetaRecommendedLayerResolutionExtensionWrapper *OpenXRMetaRecommendedLayerResolutionExtensionWrapper::get_singleton() {
	return singleton;
}

OpenXRMetaRecommendedLayerResolutionExtensionWrapper::OpenXRMetaRecommendedLayerResolutionExtensionWrapper() {
	singleton = this;
}

OpenXRMetaRecommendedLayerResolutionExtensionWrapper::~OpenXRMetaRecommendedLayerResolutionExtensionWrapper() {
	singleton = nullptr;
}

HashMap<String, bool *> OpenXRMetaRecommendedLayerResolutionExtensionWrapper::get_requested_extensions() {
	HashMap<String, bool *> request_extensions;

	request_extensions[XR_META_RECOMMENDED_LAYER_RESOLUTION_EXTENSION_NAME] = &available;

	return request_extensions;
}

void OpenXRMetaRecommendedLayerResolutionExtensionWrapper::on_instance_created(const XrInstance p_instance) {
	if (available) {
		EXT_INIT_XR_FUNC(xrGetRecommendedLayerResolutionMETA);
	}
}

void OpenXRMetaRecommendedLayerResolutionExtensionWrapper::on_pre_render() {
	OpenXRAPI *openxr_api = OpenXRAPI::get_singleton();
	if (openxr_api == nullptr) {
		return;
	}

	XrCompositionLayerProjection *projection_layer = openxr_api->get_projection_layer();
	if (projection_layer == nullptr) {
		return;
	}

	recommended_resolution_get_info.layer = (XrCompositionLayerBaseHeader *)projection_layer;
	recommended_resolution_get_info.predictedDisplayTime = openxr_api->get_predicted_display_time();

	XrResult result = xrGetRecommendedLayerResolutionMETA(openxr_api->get_session(), &recommended_resolution_get_info, &recommended_resolution);
	if (XR_FAILED(result)) {
		print_line("xrGetRecommendedLayerResolutionMETA FAILED! Output code: ", result);
		return;
	}

	if (!recommended_resolution.isValid) {
		print_line("Invalid recommended resolution data!");
	}

	Size2i render_target_size_override = { recommended_resolution.recommendedImageDimensions.width, recommended_resolution.recommendedImageDimensions.height };
	openxr_api->set_render_target_size_override(render_target_size_override);
}

void OpenXRMetaRecommendedLayerResolutionExtensionWrapper::get_recommendation() {
	// OpenXRCompositionLayerExtension *comp_layer_ext = OpenXRCompositionLayerExtension::get_singleton();
	// if (comp_layer_ext == nullptr) {
	// 	print_line("Could not retreive composition layer ext singleton");
	// 	return;
	// }

	// int layer_count = comp_layer_ext->get_composition_layer_count();
	// print_line("Composition layer count: ", layer_count);

	// recommended_resolution_get_info.layer =
	// xrGetRecommendedLayerResolutionMETA(OpenXRAPI::get_singleton()->get_session(), &recommended_resolution_get_info, &recommended_resolution);
}

void OpenXRMetaRecommendedLayerResolutionExtensionWrapper::_bind_methods() {
	ClassDB::bind_method(D_METHOD("is_available"), &OpenXRMetaRecommendedLayerResolutionExtensionWrapper::is_available);
	ClassDB::bind_method(D_METHOD("get_recommendation"), &OpenXRMetaRecommendedLayerResolutionExtensionWrapper::get_recommendation);
}

/**************************************************************************/
/*  openxr_meta_recommended_layer_resolution_extension_wrapper.h          */
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

#ifndef OPENXR_META_RECOMMENDED_LAYER_RESOLUTION_EXTENSION_WRAPPER_H
#define OPENXR_META_RECOMMENDED_LAYER_RESOLUTION_EXTENSION_WRAPPER_H

#include "openxr_extension_wrapper.h"

#include "../openxr_api.h"

class OpenXRMetaRecommendedLayerResolutionExtensionWrapper : public Object, public OpenXRExtensionWrapper {
	GDCLASS(OpenXRMetaRecommendedLayerResolutionExtensionWrapper, Object);

public:
	static OpenXRMetaRecommendedLayerResolutionExtensionWrapper *get_singleton();

	OpenXRMetaRecommendedLayerResolutionExtensionWrapper();
	virtual ~OpenXRMetaRecommendedLayerResolutionExtensionWrapper() override;

	virtual HashMap<String, bool *> get_requested_extensions() override;

	virtual void on_instance_created(const XrInstance p_instance) override;
	virtual void on_pre_render() override;

	bool is_available() { return available; }

	void get_recommendation();

protected:
	static void _bind_methods();

private:
	static OpenXRMetaRecommendedLayerResolutionExtensionWrapper *singleton;

	bool available = false;

	XrRecommendedLayerResolutionGetInfoMETA recommended_resolution_get_info = {
		XR_TYPE_RECOMMENDED_LAYER_RESOLUTION_GET_INFO_META, // type
		nullptr, // next
		nullptr, // layer
		0, // predictedDisplayTime
	};

	XrRecommendedLayerResolutionMETA recommended_resolution = {
		XR_TYPE_RECOMMENDED_LAYER_RESOLUTION_META, // type
		nullptr, // next
		{ 0, 0 }, // recommendedImageDimensions,
		false, // isValid
	};

	// OpenXR API call wrappers
	EXT_PROTO_XRRESULT_FUNC3(xrGetRecommendedLayerResolutionMETA, (XrSession), p_session, (const XrRecommendedLayerResolutionGetInfoMETA *), p_info, (XrRecommendedLayerResolutionMETA *), p_resolution)
};

#endif // OPENXR_META_RECOMMENDED_LAYER_RESOLUTION_EXTENSION_WRAPPER_H
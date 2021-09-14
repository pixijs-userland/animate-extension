/*************************************************************************
* ADOBE CONFIDENTIAL
* ___________________
*
*  Copyright [2013] Adobe Systems Incorporated
*  All Rights Reserved.
*
* NOTICE:  All information contained herein is, and remains
* the property of Adobe Systems Incorporated and its suppliers,
* if any.  The intellectual and technical concepts contained
* herein are proprietary to Adobe Systems Incorporated and its
* suppliers and are protected by all applicable intellectual
* property laws, including trade secret and copyright laws.
* Dissemination of this information or reproduction of this material
* is strictly forbidden unless prior written permission is obtained
* from Adobe Systems Incorporated.
**************************************************************************/

/**
 * @file  Publisher.h
 *
 * @brief This file contains declarations for a Publisher plugin.
 */

#ifndef PUBLISHER_H_
#define PUBLISHER_H_

#include <vector>

#include "Version.h"
#include "FCMTypes.h"
#include "FCMPluginInterface.h"
#include "Exporter/Service/IResourcePalette.h"
#include "Exporter/Service/ITimelineBuilder2.h"
#include "Exporter/Service/ITimelineBuilderFactory.h"
#include "Publisher/IPublisher.h"
#include "FillStyle/ISolidFillStyle.h"
#include "FillStyle/IGradientFillStyle.h"
#include "FillStyle/IBitmapFillStyle.h"
#include "FrameElement/IClassicText.h"
#include "FrameElement/ITextStyle.h"
#include "Exporter/Service/IFrameCommandGenerator.h"
#include "OutputWriter.h"
#include "TimelineWriter.h"
#include "PluginConfiguration.h"
#include "TweenWriter.h"

 /* -------------------------------------------------- Forward Decl */

using namespace FCM;
using namespace Publisher;
using namespace Exporter::Service;

namespace Application
{
	namespace Service
	{
		class IOutputConsoleService;
	}
}

namespace PixiJS
{
	class CPublisher;
	class ResourcePalette;
	class TimelineBuilder;
	class TimelineBuilderFactory;
	class IOutputWriter;
	class ITimelineWriter;
}

namespace DOM
{
	namespace Service
	{
		namespace Shape
		{
			FORWARD_DECLARE_INTERFACE(IPath);
		}
	};
};


/* -------------------------------------------------- Enums */


/* -------------------------------------------------- Macros / Constants */

#ifdef USE_SWF_EXPORTER_SERVICE
#define OUTPUT_FILE_EXTENSION       "swf"
#else
#define OUTPUT_FILE_EXTENSION       "js"
#endif

/* -------------------------------------------------- Structs / Unions */


/* -------------------------------------------------- Class Decl */

namespace PixiJS
{

	class CPublisher : public IPublisher, public FCMObjectBase
	{
		BEGIN_INTERFACE_MAP(CPublisher, PIXIJS_PLUGIN_VERSION)
			INTERFACE_ENTRY(IPublisher)
		END_INTERFACE_MAP

	public:

		virtual FCM::Result _FCMCALL Publish(
			DOM::PIFLADocument flaDocument,
			const PIFCMDictionary publishSettings,
			const PIFCMDictionary dictConfig);

		virtual FCM::Result _FCMCALL Publish(
			DOM::PIFLADocument flaDocument,
			DOM::PITimeline timeline,
			const Exporter::Service::RANGE &frameRange,
			const PIFCMDictionary publishSettings,
			const PIFCMDictionary dictConfig);

		virtual FCM::Result _FCMCALL ClearCache();

		CPublisher();

		~CPublisher();

	private:

		FCM::Result GetOutputFileName(
			DOM::PIFLADocument flaDocument,
			DOM::PITimeline pITimeline,
			const PIFCMDictionary publishSettings,
			std::string& basePath,
			std::string& outFile);

		FCM::Result Export(
			DOM::PIFLADocument flaDocument,
			DOM::PITimeline timeline,
			const Exporter::Service::RANGE* pFrameRange,
			const PIFCMDictionary publishSettings,
			const PIFCMDictionary dictConfig);

		FCM::Result Init();

		FCM::Result ExportLibraryItems(FCM::FCMListPtr pLibraryItemList, PixiJS::TweenWriter *pTweenWriter);

		FCM::Result CopyRuntime(const std::string& outputFolder, const bool& compressJS, const std::string& outputVersion);

	private:

		AutoPtr<IFrameCommandGenerator> m_frameCmdGeneratorService;
		AutoPtr<IResourcePalette> m_pResourcePalette;
	};


	class ResourcePalette : public IResourcePalette, public FCMObjectBase
	{
	public:

		BEGIN_INTERFACE_MAP(ResourcePalette, PIXIJS_PLUGIN_VERSION)
			INTERFACE_ENTRY(IResourcePalette)
		END_INTERFACE_MAP

		virtual FCM::Result _FCMCALL AddSymbol(
			FCM::U_Int32 resourceId,
			FCM::StringRep16 pName,
			Exporter::Service::PITimelineBuilder timelineBuilder);

		virtual FCM::Result _FCMCALL AddShape(
			FCM::U_Int32 resourceId,
			DOM::FrameElement::PIShape pShape);

		virtual FCM::Result _FCMCALL AddSound(
			FCM::U_Int32 resourceId,
			DOM::LibraryItem::PIMediaItem pLibItem);

		virtual FCM::Result _FCMCALL AddBitmap(
			FCM::U_Int32 resourceId,
			DOM::LibraryItem::PIMediaItem pLibItem);

		virtual FCM::Result _FCMCALL AddClassicText(
			FCM::U_Int32 resourceId,
			DOM::FrameElement::PIClassicText pClassicText);

		virtual FCM::Result _FCMCALL HasResource(
			FCM::U_Int32 resourceId,
			FCM::Boolean& hasResource);

		ResourcePalette();

		~ResourcePalette();

		void Init(IOutputWriter* outputWriter);

		void Clear();

		FCM::Result HasResource(
			const std::string& name,
			FCM::Boolean& hasResource);

	private:

		FCM::Result ExportFill(DOM::FrameElement::PIShape pIShape);

		FCM::Result ExportStroke(DOM::FrameElement::PIShape pIShape);

		FCM::Result ExportStrokeStyle(FCM::PIFCMUnknown pStrokeStyle);

		FCM::Result ExportSolidStrokeStyle(DOM::StrokeStyle::ISolidStrokeStyle* pSolidStrokeStyle);

		FCM::Result ExportFillStyle(FCM::PIFCMUnknown pFillStyle);

		FCM::Result ExportFillBoundary(DOM::Service::Shape::PIPath pPath);

		FCM::Result ExportHole(DOM::Service::Shape::PIPath pPath);

		FCM::Result ExportPath(DOM::Service::Shape::PIPath pPath);

		FCM::Result ExportSolidFillStyle(
			DOM::FillStyle::ISolidFillStyle* pSolidFillStyle);

		FCM::Result ExportRadialGradientFillStyle(
			DOM::FillStyle::IGradientFillStyle* pGradientFillStyle);

		FCM::Result ExportLinearGradientFillStyle(
			DOM::FillStyle::IGradientFillStyle* pGradientFillStyle);

		FCM::Result ExportBitmapFillStyle(
			DOM::FillStyle::IBitmapFillStyle* pBitmapFillStyle);

		FCM::Result GetTextStyle(DOM::FrameElement::ITextStyle* pTextStyleItem, TEXT_STYLE& textStyle);

		FCM::Result HasFancyStrokes(DOM::FrameElement::PIShape pShape, FCM::Boolean& hasFancy);

		FCM::Result ConvertStrokeToFill(
			DOM::FrameElement::PIShape pShape,
			DOM::FrameElement::PIShape& pNewShape);

		FCM::Result GetTextBehaviour(DOM::FrameElement::ITextBehaviour* pTextBehaviour, TEXT_BEHAVIOUR& textBehaviour);

	private:

		IOutputWriter* m_outputWriter;

		std::vector<FCM::U_Int32> m_resourceList;

		std::vector<std::string> m_resourceNames;
	};


	class TimelineBuilder : public ITimelineBuilder2, public FCMObjectBase
	{
	public:

		BEGIN_INTERFACE_MAP(TimelineBuilder, PIXIJS_PLUGIN_VERSION)
			INTERFACE_ENTRY(ITimelineBuilder2)
		END_INTERFACE_MAP

		virtual FCM::Result _FCMCALL AddShape(
			FCM::U_Int32 objectId,
			SHAPE_INFO* pShapeInfo);

		virtual FCM::Result _FCMCALL AddClassicText(
			FCM::U_Int32 objectId,
			CLASSIC_TEXT_INFO* pClassicTextInfo);

		virtual FCM::Result _FCMCALL AddBitmap(
			FCM::U_Int32 objectId,
			BITMAP_INFO* pBitmapInfo);

		virtual FCM::Result _FCMCALL AddMovieClip(
			FCM::U_Int32 objectId,
			MOVIE_CLIP_INFO* pMovieClipInfo,
			DOM::FrameElement::PIMovieClip pMovieClip);

		virtual FCM::Result _FCMCALL AddGraphic(
			FCM::U_Int32 objectId,
			GRAPHIC_INFO* pGraphicInfo);

		virtual FCM::Result _FCMCALL AddSound(
			FCM::U_Int32 objectId,
			SOUND_INFO* pSoundInfo,
			DOM::FrameElement::PISound pSound);

		virtual FCM::Result _FCMCALL UpdateZOrder(
			FCM::U_Int32 objectId,
			FCM::U_Int32 placeAfterObjectId);

		virtual FCM::Result UpdateMask(
			FCM::U_Int32 objectId,
			FCM::U_Int32 maskTillObjectId);

		virtual FCM::Result _FCMCALL Remove(FCM::U_Int32 objectId);

		virtual FCM::Result _FCMCALL UpdateBlendMode(
			FCM::U_Int32 objectId,
			DOM::FrameElement::BlendMode blendMode);

		virtual FCM::Result _FCMCALL UpdateVisibility(
			FCM::U_Int32 objectId,
			FCM::Boolean visible);

		virtual FCM::Result _FCMCALL UpdateGraphicFilter(
			FCM::U_Int32 objectId,
			PIFCMList pFilterable);

		virtual FCM::Result _FCMCALL UpdateDisplayTransform(
			FCM::U_Int32 objectId,
			const DOM::Utils::MATRIX2D& matrix);

		virtual FCM::Result _FCMCALL UpdateColorTransform(
			FCM::U_Int32 objectId,
			const DOM::Utils::COLOR_MATRIX& colorMatrix);

		virtual FCM::Result _FCMCALL ShowFrame();

		virtual FCM::Result _FCMCALL AddFrameScript(FCM::CStringRep16 pScript, FCM::U_Int32 layerNum);

		virtual FCM::Result _FCMCALL RemoveFrameScript(FCM::U_Int32 layerNum);

		virtual FCM::Result _FCMCALL SetFrameLabel(FCM::StringRep16 pLabel, DOM::KeyFrameLabelType labelType);

		TimelineBuilder();

		~TimelineBuilder();

		virtual FCM::Result Build(
			FCM::U_Int32 resourceId,
			FCM::StringRep16 name,
			ITimelineWriter** timelineWriter);

		void Init(IOutputWriter* outputWriter);

	private:

		IOutputWriter* m_outputWriter;

		ITimelineWriter* m_timelineWriter;

		FCM::U_Int32 m_frameIndex;
	};


	class TimelineBuilderFactory : public ITimelineBuilderFactory, public FCMObjectBase
	{
	public:

		BEGIN_INTERFACE_MAP(TimelineBuilderFactory, PIXIJS_PLUGIN_VERSION)
			INTERFACE_ENTRY(ITimelineBuilderFactory)
		END_INTERFACE_MAP

		virtual FCM::Result _FCMCALL CreateTimelineBuilder(PITimelineBuilder& timelineBuilder);

		TimelineBuilderFactory();

		~TimelineBuilderFactory();

		void Init(IOutputWriter* outputWriter);

	private:

		IOutputWriter* m_outputWriter;
	};

	FCM::Result RegisterPublisher(PIFCMDictionary pPlugins, FCM::FCMCLSID docId);
};

#endif // PUBLISHER_H_


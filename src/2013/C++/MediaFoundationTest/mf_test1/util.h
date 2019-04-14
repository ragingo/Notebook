#pragma once
#include "stdafx.h"

#ifndef __MF_UTIL_H_
#define __MF_UTIL_H_


#define GetEnumFieldName(x) _T(#x)

LPTSTR GetMediaEventTypeName(const MediaEventType& type)
{
	switch (type)
	{
	case MEUnknown: return GetEnumFieldName(MEUnknown);
	case MEError: return GetEnumFieldName(MEError);
	case MEExtendedType: return GetEnumFieldName(MEExtendedType);
	case MENonFatalError: return GetEnumFieldName(MENonFatalError);
	case MESessionUnknown: return GetEnumFieldName(MESessionUnknown);
	case MESessionTopologySet: return GetEnumFieldName(MESessionTopologySet);
	case MESessionTopologiesCleared: return GetEnumFieldName(MESessionTopologiesCleared);
	case MESessionStarted: return GetEnumFieldName(MESessionStarted);
	case MESessionPaused: return GetEnumFieldName(MESessionPaused);
	case MESessionStopped: return GetEnumFieldName(MESessionStopped);
	case MESessionClosed: return GetEnumFieldName(MESessionClosed);
	case MESessionEnded: return GetEnumFieldName(MESessionEnded);
	case MESessionRateChanged: return GetEnumFieldName(MESessionRateChanged);
	case MESessionScrubSampleComplete: return GetEnumFieldName(MESessionScrubSampleComplete);
	case MESessionCapabilitiesChanged: return GetEnumFieldName(MESessionCapabilitiesChanged);
	case MESessionTopologyStatus: return GetEnumFieldName(MESessionTopologyStatus);
	case MESessionNotifyPresentationTime: return GetEnumFieldName(MESessionNotifyPresentationTime);
	case MENewPresentation: return GetEnumFieldName(MENewPresentation);
	case MELicenseAcquisitionStart: return GetEnumFieldName(MELicenseAcquisitionStart);
	case MELicenseAcquisitionCompleted: return GetEnumFieldName(MELicenseAcquisitionCompleted);
	case MEIndividualizationStart: return GetEnumFieldName(MEIndividualizationStart);
	case MEIndividualizationCompleted: return GetEnumFieldName(MEIndividualizationCompleted);
	case MEEnablerProgress: return GetEnumFieldName(MEEnablerProgress);
	case MEEnablerCompleted: return GetEnumFieldName(MEEnablerCompleted);
	case MEPolicyError: return GetEnumFieldName(MEPolicyError);
	case MEPolicyReport: return GetEnumFieldName(MEPolicyReport);
	case MEBufferingStarted: return GetEnumFieldName(MEBufferingStarted);
	case MEBufferingStopped: return GetEnumFieldName(MEBufferingStopped);
	case MEConnectStart: return GetEnumFieldName(MEConnectStart);
	case MEConnectEnd: return GetEnumFieldName(MEConnectEnd);
	case MEReconnectStart: return GetEnumFieldName(MEReconnectStart);
	case MEReconnectEnd: return GetEnumFieldName(MEReconnectEnd);
	case MERendererEvent: return GetEnumFieldName(MERendererEvent);
	case MESessionStreamSinkFormatChanged: return GetEnumFieldName(MESessionStreamSinkFormatChanged);
	case MESourceUnknown: return GetEnumFieldName(MESourceUnknown);
	case MESourceStarted: return GetEnumFieldName(MESourceStarted);
	case MEStreamStarted: return GetEnumFieldName(MEStreamStarted);
	case MESourceSeeked: return GetEnumFieldName(MESourceSeeked);
	case MEStreamSeeked: return GetEnumFieldName(MEStreamSeeked);
	case MENewStream: return GetEnumFieldName(MENewStream);
	case MEUpdatedStream: return GetEnumFieldName(MEUpdatedStream);
	case MESourceStopped: return GetEnumFieldName(MESourceStopped);
	case MEStreamStopped: return GetEnumFieldName(MEStreamStopped);
	case MESourcePaused: return GetEnumFieldName(MESourcePaused);
	case MEStreamPaused: return GetEnumFieldName(MEStreamPaused);
	case MEEndOfPresentation: return GetEnumFieldName(MEEndOfPresentation);
	case MEEndOfStream: return GetEnumFieldName(MEEndOfStream);
	case MEMediaSample: return GetEnumFieldName(MEMediaSample);
	case MEStreamTick: return GetEnumFieldName(MEStreamTick);
	case MEStreamThinMode: return GetEnumFieldName(MEStreamThinMode);
	case MEStreamFormatChanged: return GetEnumFieldName(MEStreamFormatChanged);
	case MESourceRateChanged: return GetEnumFieldName(MESourceRateChanged);
	case MEEndOfPresentationSegment: return GetEnumFieldName(MEEndOfPresentationSegment);
	case MESourceCharacteristicsChanged: return GetEnumFieldName(MESourceCharacteristicsChanged);
	case MESourceRateChangeRequested: return GetEnumFieldName(MESourceRateChangeRequested);
	case MESourceMetadataChanged: return GetEnumFieldName(MESourceMetadataChanged);
	case MESequencerSourceTopologyUpdated: return GetEnumFieldName(MESequencerSourceTopologyUpdated);
	case MESinkUnknown: return GetEnumFieldName(MESinkUnknown);
	case MEStreamSinkStarted: return GetEnumFieldName(MEStreamSinkStarted);
	case MEStreamSinkStopped: return GetEnumFieldName(MEStreamSinkStopped);
	case MEStreamSinkPaused: return GetEnumFieldName(MEStreamSinkPaused);
	case MEStreamSinkRateChanged: return GetEnumFieldName(MEStreamSinkRateChanged);
	case MEStreamSinkRequestSample: return GetEnumFieldName(MEStreamSinkRequestSample);
	case MEStreamSinkMarker: return GetEnumFieldName(MEStreamSinkMarker);
	case MEStreamSinkPrerolled: return GetEnumFieldName(MEStreamSinkPrerolled);
	case MEStreamSinkScrubSampleComplete: return GetEnumFieldName(MEStreamSinkScrubSampleComplete);
	case MEStreamSinkFormatChanged: return GetEnumFieldName(MEStreamSinkFormatChanged);
	case MEStreamSinkDeviceChanged: return GetEnumFieldName(MEStreamSinkDeviceChanged);
	case MEQualityNotify: return GetEnumFieldName(MEQualityNotify);
	case MESinkInvalidated: return GetEnumFieldName(MESinkInvalidated);
	case MEAudioSessionNameChanged: return GetEnumFieldName(MEAudioSessionNameChanged);
	case MEAudioSessionVolumeChanged: return GetEnumFieldName(MEAudioSessionVolumeChanged);
	case MEAudioSessionDeviceRemoved: return GetEnumFieldName(MEAudioSessionDeviceRemoved);
	case MEAudioSessionServerShutdown: return GetEnumFieldName(MEAudioSessionServerShutdown);
	case MEAudioSessionGroupingParamChanged: return GetEnumFieldName(MEAudioSessionGroupingParamChanged);
	case MEAudioSessionIconChanged: return GetEnumFieldName(MEAudioSessionIconChanged);
	case MEAudioSessionFormatChanged: return GetEnumFieldName(MEAudioSessionFormatChanged);
	case MEAudioSessionDisconnected: return GetEnumFieldName(MEAudioSessionDisconnected);
	case MEAudioSessionExclusiveModeOverride: return GetEnumFieldName(MEAudioSessionExclusiveModeOverride);
	case METrustUnknown: return GetEnumFieldName(METrustUnknown);
	case MEPolicyChanged: return GetEnumFieldName(MEPolicyChanged);
	case MEContentProtectionMessage: return GetEnumFieldName(MEContentProtectionMessage);
	case MEPolicySet: return GetEnumFieldName(MEPolicySet);
	case MEWMDRMLicenseBackupCompleted: return GetEnumFieldName(MEWMDRMLicenseBackupCompleted);
	case MEWMDRMLicenseBackupProgress: return GetEnumFieldName(MEWMDRMLicenseBackupProgress);
	case MEWMDRMLicenseRestoreCompleted: return GetEnumFieldName(MEWMDRMLicenseRestoreCompleted);
	case MEWMDRMLicenseRestoreProgress: return GetEnumFieldName(MEWMDRMLicenseRestoreProgress);
	case MEWMDRMLicenseAcquisitionCompleted: return GetEnumFieldName(MEWMDRMLicenseAcquisitionCompleted);
	case MEWMDRMIndividualizationCompleted: return GetEnumFieldName(MEWMDRMIndividualizationCompleted);
	case MEWMDRMIndividualizationProgress: return GetEnumFieldName(MEWMDRMIndividualizationProgress);
	case MEWMDRMProximityCompleted: return GetEnumFieldName(MEWMDRMProximityCompleted);
	case MEWMDRMLicenseStoreCleaned: return GetEnumFieldName(MEWMDRMLicenseStoreCleaned);
	case MEWMDRMRevocationDownloadCompleted: return GetEnumFieldName(MEWMDRMRevocationDownloadCompleted);
	case METransformUnknown: return GetEnumFieldName(METransformUnknown);
	case METransformNeedInput: return GetEnumFieldName(METransformNeedInput);
	case METransformHaveOutput: return GetEnumFieldName(METransformHaveOutput);
	case METransformDrainComplete: return GetEnumFieldName(METransformDrainComplete);
	case METransformMarker: return GetEnumFieldName(METransformMarker);
	case MEReservedMax: return GetEnumFieldName(MEReservedMax);
	default:
		return _T("");
	}
}


#endif

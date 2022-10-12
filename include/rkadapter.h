typedef enum TMessageType {
	RequestRegisterClient,        /*   0 */
	RequestQueryServer,
	RequestQueryAddress,
	RequestRegisterAddress,
	RequestUnregisterAddress,
	RequestRegisterAll,
	RequestUnregisterAll,
	RequestSetInputMask,
	RequestAgentLogin,
	RequestAgentLogout,
	RequestAgentReady,            /*  10 */
	RequestAgentNotReady,
	RequestSetDNDOn,
	RequestSetDNDOff,
	RequestMakeCall,
	RequestMakePredictiveCall,
	RequestAnswerCall,
	RequestReleaseCall,
	RequestHoldCall,
	RequestRetrieveCall,
	RequestInitiateConference,    /*  20 */
	RequestCompleteConference,
	RequestDeleteFromConference,
	RequestInitiateTransfer,
	RequestMuteTransfer,
	RequestSingleStepTransfer,
	RequestCompleteTransfer,
	RequestMergeCalls,
	RequestAlternateCall,
	RequestReconnectCall,
	RequestAttachUserData,        /*  30 */
	RequestUpdateUserData,
	RequestDeleteUserData,
	RequestDeletePair,
	RequestCallForwardSet,
	RequestCallForwardCancel,
	RequestRouteCall,
	RequestGiveMusicTreatment,
	RequestGiveSilenceTreatment,
	RequestGiveRingBackTreatment,
	RequestLoginMailBox,          /*  40 */
	RequestLogoutMailBox,
	RequestOpenVoiceFile,
	RequestCloseVoiceFile,
	RequestPlayVoiceFile,
	RequestCollectDigits,
	RequestSetMessageWaitingOn,
	RequestSetMessageWaitingOff,
	RequestDistributeUserEvent,
	RequestDistributeEvent,

	EventServerConnected,         /*  50 */
	EventServerDisconnected,
	EventError,
	EventRegistered,
	EventUnregistered,
	EventRegisteredAll,
	EventUnregisteredAll,
	EventQueued,
	EventDiverted,
	EventAbandoned,
	EventRinging,                 /*  60 */
	EventDialing,
	EventNetworkReached,
	EventDestinationBusy,
	EventEstablished,
	EventReleased,
	EventHeld,
	EventRetrieved,
	EventPartyChanged,
	EventPartyAdded,
	EventPartyDeleted,            /*  70 */
	EventRouteRequest,
	EventRouteUsed,
	EventAgentLogin,
	EventAgentLogout,
	EventAgentReady,
	EventAgentNotReady,
	EventDNDOn,
	EventDNDOff,
	EventMailBoxLogin,
	EventMailBoxLogout,           /*  80 */
	EventVoiceFileOpened,
	EventVoiceFileClosed,
	EventVoiceFileEndPlay,
	EventDigitsCollected,
	EventAttachedDataChanged,
	EventOffHook,
	EventOnHook,
	EventForwardSet,
	EventForwardCancel,
	EventMessageWaitingOn,        /*  90 */
	EventMessageWaitingOff,
	EventAddressInfo,
	EventServerInfo,
	EventLinkDisconnected,
	EventLinkConnected,
	EventUserEvent,

	RequestSendDTMF,              /*  97 */
	EventDTMFSent,

	EventResourceAllocated,       /*  99 */
	EventResourceFreed,

	EventRemoteConnectionSuccess, /* 101 */
	EventRemoteConnectionFailed,

	RequestRedirectCall,          /* 103 */
	RequestListenDisconnect,
	RequestListenReconnect,
	EventListenDisconnected,
	EventListenReconnected,
	RequestQueryCall,
	EventPartyInfo,
	RequestClearCall,

	RequestSetCallInfo,           /* 111 */
	EventCallInfoChanged,

	RequestApplyTreatment,        /* 113 */
	EventTreatmentApplied,
	EventTreatmentNotApplied,
	EventTreatmentEnd,

	EventHardwareError,           /* 117 */
	EventAgentAfterCallWork,
	EventTreatmentRequired,

	RequestSingleStepConference,  /* 120 */
	RequestQuerySwitch,
	EventSwitchInfo,

	RequestGetAccessNumber,       /* 123 */
	RequestCancelReqGetAccessNumber,
	EventAnswerAccessNumber,
	EventReqGetAccessNumberCanceled,

	RequestReserveAgent,          /* 127 */
	EventAgentReserved,
	RequestReserveAgentAndGetAccessNumber,

	RequestAgentSetIdleReason,    /* 130 */
	EventAgentIdleReasonSet,

	EventRestoreConnection,       /* 132 */
	EventPrimaryChanged,
	RequestLostBackupConnection,
	RequestSetDNInfo,

	RequestQueryLocation,         /* 136 */
	EventLocationInfo,

	EventACK,                     /* 138 */

	RequestMonitorNextCall,       /* 139 */
	RequestCancelMonitoring,
	EventMonitoringNextCall,
	EventMonitoringCancelled,

	RequestSetMuteOn,             /* 143 */
	RequestSetMuteOff,
	EventMuteOn,
	EventMuteOff,

	EventDNOutOfService,          /* 147 */
	EventDNBackInService,

	RequestPrivateService,        /* 149 */
	EventPrivateInfo,

	EventBridged, /* added by Epi 7/10/01, ER#23208 */
	EventQueueLogout,                   /* ER#53943 */

	EventReserved_1,              /* 153 */
	EventReserved_2,
	EventReserved_3,
	EventReserved_4,
	EventReserved_5,
	EventReserved_6,
	EventReserved_7,
	/*
	 * Call Monitoring events:
	 */
	EventCallCreated,             /* 160 */
	EventCallDataChanged,
	EventCallDeleted,
	EventCallPartyAdded,
	EventCallPartyState,
	EventCallPartyMoved,
	EventCallPartyDeleted,
	/*
	 * Call Monitoring requests:
	 */
	RequestStartCallMonitoring,   /* 167 */
	RequestStopCallMonitoring,
	/*
	 * Return Receipt service:
	 */
	RequestSendReturnReceipt,     /* 169 */
	EventReturnReceipt,
	/*
	 * Network Attended Transfer / Conference (ER#94506):
	 */
	RequestNetworkConsult,        /* 171 */
	RequestNetworkAlternate,
	RequestNetworkTransfer,
	RequestNetworkMerge,
	RequestNetworkReconnect,
	RequestNetworkSingleStepTransfer,
	RequestNetworkPrivateService,
	EventNetworkCallStatus,       /* 178 */
	EventNetworkPrivateInfo,

	/* begin: iscc transaction monitoring - added in 7.6 */
	RequestTransactionMonitoring,
	EventTransactionStatus,
	/* end:   iscc transaction monitoring */

	EventResourceInfo,  /* added: in 8.0 on 12/11/2008 */

	MessageIDMAX
};


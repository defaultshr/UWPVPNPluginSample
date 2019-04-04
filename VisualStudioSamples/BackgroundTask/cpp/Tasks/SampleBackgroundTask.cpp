//*********************************************************
// Copyright (c) Microsoft. All rights reserved.
// This code is licensed under the MIT License (MIT).
// THIS CODE IS PROVIDED *AS IS* WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING ANY
// IMPLIED WARRANTIES OF FITNESS FOR A PARTICULAR
// PURPOSE, MERCHANTABILITY, OR NON-INFRINGEMENT.
//
// This code is modified from https://github.com/Microsoft/Windows-universal-samples/tree/master/Samples/BackgroundTask
// to implement reference VPN plugin application. 
// Refer licencing terms above. 
// 
//*********************************************************

// SampleBackgroundTask.cpp
#include "pch.h"
#include "TunnelPlugin.h"
#include "SampleBackgroundTask.h"

using namespace Tasks;
using namespace Windows::ApplicationModel::Background;
using namespace Windows::Foundation;
using namespace Windows::Storage;
using namespace Windows::System::Threading;
using namespace Windows::Networking::Vpn;
using namespace concurrency;

Windows::Networking::Vpn::IVpnPlugIn^ SampleBackgroundTask::plugin = nullptr;

// Important Notes: 
// 1. Package.appxmanifest must have VPN capability and VPN task specified. Search for VPN in manifest.
// 2. There is no need for explicit registration of background task for VPN purpose. You wouldnt see task builder registration code getting hit in this application and yet VPN profile connect/disconnect trigger background task. Manifest changes appear to be sufficient for same.
// 3. If the main application execution task creation is unable to find task class, it could be due to App manifest not being able to identify the TASK's DLL and class. Ensure the task's 'References' in the Solution Explorer refer to the Task project. Clear all the generated folders manually in both main application and task. From 'Build' menu, 'Build full program database file for solution'. The manifest must get fixed.

// When system's VPN profile connect/disconnect done, it triggers following method in background task.
void SampleBackgroundTask::Run(IBackgroundTaskInstance^ taskInstance)
{
	// Create VPN Plugin and start the channel.
	if (this->plugin == nullptr)
		this->plugin = ref new TunnelPlugin(nullptr);

	VpnChannel::ProcessEventAsync(this->plugin, taskInstance->TriggerDetails);
}

//
// Handles background task cancellation.
//
void SampleBackgroundTask::OnCanceled(IBackgroundTaskInstance^ taskInstance, BackgroundTaskCancellationReason reason)
{
    //
    // Indicate that the background task is canceled.
    //
    CancelRequested = true;
    CancelReason = reason;
}

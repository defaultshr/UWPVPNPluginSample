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

#pragma once

namespace Tasks
{
    [Windows::Foundation::Metadata::WebHostHidden]
    public ref class SampleBackgroundTask sealed : public Windows::ApplicationModel::Background::IBackgroundTask
    {

    public:
        virtual void Run(Windows::ApplicationModel::Background::IBackgroundTaskInstance^ taskInstance);
        void OnCanceled(Windows::ApplicationModel::Background::IBackgroundTaskInstance^ taskInstance, Windows::ApplicationModel::Background::BackgroundTaskCancellationReason reason);

    private:
        Windows::ApplicationModel::Background::BackgroundTaskCancellationReason CancelReason = Windows::ApplicationModel::Background::BackgroundTaskCancellationReason::Abort;
        volatile bool CancelRequested = false;
        Platform::Agile<Windows::ApplicationModel::Background::BackgroundTaskDeferral> TaskDeferral = nullptr;
        Windows::System::Threading::ThreadPoolTimer^ PeriodicTimer = nullptr;
        unsigned int Progress = 0;
        Windows::ApplicationModel::Background::IBackgroundTaskInstance^ TaskInstance = nullptr;
		static Windows::Networking::Vpn::IVpnPlugIn^ plugin;
    };
}
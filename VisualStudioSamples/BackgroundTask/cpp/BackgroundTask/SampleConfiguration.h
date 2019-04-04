//*********************************************************
//
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

#include <collection.h>

using namespace Platform;
using namespace Windows::ApplicationModel::Background;
using namespace Windows::Storage;

#define SampleBackgroundTaskEntryPoint "Tasks.SampleBackgroundTask"
#define SampleBackgroundTaskName "SampleBackgroundTask"
#define ServicingCompleteTaskEntryPoint "Tasks.ServicingComplete"
#define ApplicationTriggerTaskName "ApplicationTriggerTask"

namespace SDKTemplate
{
    public value struct Scenario
    {
        String^ Title;
        String^ ClassName;
    };

    partial ref class MainPage
    {
    public:
        static property String^ FEATURE_NAME
        {
            String^ get()
            {
                return L"Background tasks";
            }
        }

        static property Array<Scenario>^ scenarios
        {
            Array<Scenario>^ get()
            {
                return scenariosInner;
            }
        }
    private:
        static Array<Scenario>^ scenariosInner;
    };

    class BackgroundTaskSample
    {
    public:
        static String^ GetBackgroundTaskStatus(String^ name);
        static BackgroundTaskRegistration^ RegisterBackgroundTask(String^ taskEntryPoint, String^ name, IBackgroundTrigger^ trigger, IBackgroundCondition^ condition, BackgroundTaskRegistrationGroup^ group = nullptr);
        static bool TaskRequiresBackgroundAccess(String^ name);
        static void UnregisterBackgroundTasks(String^ name, BackgroundTaskRegistrationGroup^ group = nullptr);
        static BackgroundTaskRegistrationGroup^ GetTaskGroup(String^ id, String^ groupName);
        static void UpdateBackgroundTaskRegistrationStatus(String^ name, bool registered);

        static String^ ApplicationTriggerTaskProgress;
        static bool ApplicationTriggerTaskRegistered;
        static String^ ApplicationTriggerTaskResult;
    };

}

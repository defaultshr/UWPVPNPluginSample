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

#include "pch.h"
#include "MainPage.xaml.h"
#include "SampleConfiguration.h"

using namespace SDKTemplate;
using namespace Windows::ApplicationModel::Background;

String^ BackgroundTaskSample::ApplicationTriggerTaskProgress = "";
bool BackgroundTaskSample::ApplicationTriggerTaskRegistered = false;
String^ BackgroundTaskSample::ApplicationTriggerTaskResult = "";

Array<Scenario>^ MainPage::scenariosInner = ref new Array<Scenario>
{
    // The format here is the following:
    //     { "Description for the sample", "Fully quaified name for the class that implements the scenario" }
    { "Background task with application trigger", "SDKTemplate.ApplicationTriggerTask" },
};

String^ BackgroundTaskSample::GetBackgroundTaskStatus(String^ name)
{
    auto registered = false;
	if (name == ApplicationTriggerTaskName)
    {
        registered = BackgroundTaskSample::ApplicationTriggerTaskRegistered;
    }

    String^ status = registered ? "Registered" : "Unregistered";

    auto settings = ApplicationData::Current->LocalSettings->Values;
    if (settings->HasKey(name))
    {
        status += " - " + dynamic_cast<String^>(settings->Lookup(name));
    }

    return status;
}

BackgroundTaskRegistration^ BackgroundTaskSample::RegisterBackgroundTask(String^ taskEntryPoint, String^ name, IBackgroundTrigger^ trigger, IBackgroundCondition^ condition, BackgroundTaskRegistrationGroup^ group)
{
    if (TaskRequiresBackgroundAccess(name))
    {
        BackgroundExecutionManager::RequestAccessAsync();
    }

    auto builder = ref new BackgroundTaskBuilder();

    builder->Name = name;
    builder->TaskEntryPoint = taskEntryPoint;
    builder->SetTrigger(trigger);

	// Start - Added for Vpn plugin
	builder->IsNetworkRequested = true;
	SystemCondition^ cond = ref new SystemCondition(SystemConditionType::InternetAvailable);
	builder->AddCondition(cond);
	// End - Added for Vpn plugin

    if (condition != nullptr)
    {
        builder->AddCondition(condition);

        //
        // If the condition changes while the background task is executing then it will
        // be canceled.
        //
        builder->CancelOnConditionLoss = true;
    }

    if (group != nullptr)
    {
        builder->TaskGroup = group;
    }

    auto task = builder->Register();

    UpdateBackgroundTaskRegistrationStatus(name, true);

    //
    // Remove previous completion status from local settings.
    //
    auto settings = ApplicationData::Current->LocalSettings->Values;
    settings->Remove(name);

    return task;
}

bool BackgroundTaskSample::TaskRequiresBackgroundAccess(String^ name)
{
    if (name == ApplicationTriggerTaskName)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void BackgroundTaskSample::UnregisterBackgroundTasks(String^ name, BackgroundTaskRegistrationGroup^ group)
{
    //
    // If the given task group is registered then loop through all background tasks associated with it
    // and unregister any with the name passed into this function.
    //
    if (group != nullptr)
    {
        for (auto pair : group->AllTasks)
        {
            auto task = pair->Value;
            if (task->Name == name)
            {
                task->Unregister(true);
            }
        }
    }
    else
    {
        //
        // Loop through all ungrouped background tasks and unregister any with the name passed into this function.
        //
        for (auto pair : BackgroundTaskRegistration::AllTasks)
        {
            auto task = pair->Value;
            if (task->Name == name)
            {
                task->Unregister(true);
            }
        }
    }

    UpdateBackgroundTaskRegistrationStatus(name, false);
}

BackgroundTaskRegistrationGroup^ BackgroundTaskSample::GetTaskGroup(String^ id, String^ groupName)
{
    auto group = BackgroundTaskRegistration::GetTaskGroup(id);

    if (group == nullptr)
    {
        group = ref new BackgroundTaskRegistrationGroup(id, groupName);
    }

    return group;
}

void BackgroundTaskSample::UpdateBackgroundTaskRegistrationStatus(String^ name, bool registered)
{
    if (name == ApplicationTriggerTaskName)
    {
        BackgroundTaskSample::ApplicationTriggerTaskRegistered = registered;
    }
}

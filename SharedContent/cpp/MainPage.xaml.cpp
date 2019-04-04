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

#include "pch.h"
#include "MainPage.xaml.h"

using namespace SDKTemplate;
using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Core;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Automation::Peers;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;
using namespace Windows::UI::Xaml::Interop;
using namespace Windows::Storage;

// The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409

MainPage^ MainPage::Current = nullptr;

MainPage::MainPage()
{
    InitializeComponent();

    // This is a static public property that allows downstream pages to get a handle to the MainPage instance
    // in order to call methods that are in this class.
    MainPage::Current = this;

	ApplicationData::Current->LocalSettings->Values->Insert("RemoteTunnelHost", this->RemoteTunnelHost->Text);
	ApplicationData::Current->LocalSettings->Values->Insert("RemoteTunnelPort", this->RemoteTunnelPort->Text);
	ApplicationData::Current->LocalSettings->Values->Insert("LocalTunnelHost", this->LocalTunnelHost->Text);
	ApplicationData::Current->LocalSettings->Values->Insert("LocalTunnelPort", this->LocalTunnelPort->Text);
	ApplicationData::Current->LocalSettings->Values->Insert("VPNInterfaceV4Addr", this->VPNInterfaceV4Addr->Text);
	// Only 1 single route configuration supported for now. 
	ApplicationData::Current->LocalSettings->Values->Insert("TunnelSubnetIP", this->TunnelSubnetIP->Text);
	ApplicationData::Current->LocalSettings->Values->Insert("TunnelSubnetMask", this->TunnelSubnetMask->Text);
}

void MainPage::OnNavigatedTo(NavigationEventArgs^ e)
{
    // Populate the ListBox with the scenarios as defined in SampleConfiguration.cpp.
    auto itemCollection = ref new Platform::Collections::Vector<Object^>();
    int i = 1;
    for (auto const& s : MainPage::Current->scenarios)
    {
        // Create a textBlock to hold the content and apply the ListItemTextStyle from Styles.xaml
        TextBlock^ textBlock = ref new TextBlock();
        ListBoxItem^ item = ref new ListBoxItem();
        auto style = App::Current->Resources->Lookup("ListItemTextStyle");

        textBlock->Text = (i++).ToString() + ") " + s.Title;
        textBlock->Style = safe_cast<Windows::UI::Xaml::Style ^>(style);

        item->Name = s.ClassName;
        item->Content = textBlock;
        itemCollection->Append(item);
    }
}


void MainPage::NotifyUser(String^ strMessage, NotifyType type)
{
    if (Dispatcher->HasThreadAccess)
    {
        UpdateStatus(strMessage, type);
    }
    else
    {
        Dispatcher->RunAsync(CoreDispatcherPriority::Normal, ref new DispatchedHandler([strMessage, type, this]()
        {
            UpdateStatus(strMessage, type);
        }));
    }
}

void MainPage::UpdateStatus(String^ strMessage, NotifyType type)
{
    switch (type)
    {
    case NotifyType::StatusMessage:
        //StatusBorder->Background = ref new SolidColorBrush(Windows::UI::Colors::Green);
        break;
    case NotifyType::ErrorMessage:
        //StatusBorder->Background = ref new SolidColorBrush(Windows::UI::Colors::Red);
        break;
    default:
        break;
    }

    // StatusBlock->Text = strMessage;
}

void MainPage::Footer_Click(Object^ sender, RoutedEventArgs^ e)
{
    auto uri = ref new Uri((String^)((HyperlinkButton^)sender)->Tag);
    Windows::System::Launcher::LaunchUriAsync(uri);
}

void MainPage::Button_Click(Object^ sender, RoutedEventArgs^ e)
{
}

void MainPage::Update_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	ApplicationData::Current->LocalSettings->Values->Insert("RemoteTunnelHost", this->RemoteTunnelHost->Text);
	ApplicationData::Current->LocalSettings->Values->Insert("RemoteTunnelPort", this->RemoteTunnelPort->Text);
	ApplicationData::Current->LocalSettings->Values->Insert("LocalTunnelHost", this->LocalTunnelHost->Text);
	ApplicationData::Current->LocalSettings->Values->Insert("LocalTunnelPort", this->LocalTunnelPort->Text);
	ApplicationData::Current->LocalSettings->Values->Insert("VPNInterfaceV4Addr", this->VPNInterfaceV4Addr->Text);
	// Only 1 single route configuration supported for now. 
	ApplicationData::Current->LocalSettings->Values->Insert("TunnelSubnetIP", this->TunnelSubnetIP->Text);
	ApplicationData::Current->LocalSettings->Values->Insert("TunnelSubnetMask", this->TunnelSubnetMask->Text);
}

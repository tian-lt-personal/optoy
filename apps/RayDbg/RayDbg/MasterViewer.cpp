﻿#include "pch.h"
#include "MasterViewer.h"
#if __has_include("MasterViewer.g.cpp")
#include "MasterViewer.g.cpp"
#endif

using namespace winrt;
using namespace Windows::UI::Xaml;

namespace winrt::raydbg::implementation
{
    MasterViewer::MasterViewer()
        : _dxres()
        , _graphics(&_dxres) {
        InitializeComponent();
        auto core_wnd = Window::Current().CoreWindow();
        core_wnd.VisibilityChanged([](auto, auto) {}); // TODO
        auto display_info = winrt::Windows::Graphics::Display::DisplayInformation::GetForCurrentView();
        display_info.DpiChanged([this](auto sender, auto) { _on_dpi_changed(sender); });
        winrt::Windows::Graphics::Display::DisplayInformation::DisplayContentsInvalidated([this](auto, auto) { _on_display_content_invalidated(); });
        swapchainpanel().CompositionScaleChanged([this](auto sender, auto) { _on_composition_scale_changed(sender); });
        swapchainpanel().SizeChanged([this](auto, auto args) { _on_swapchain_size_changed(args); });
        _dxres.set_swapchainpanel(swapchainpanel());
        Loaded([this](auto, auto) { _on_loaded(); });
        Unloaded([this](auto, auto) { _on_unloaded(); });
    }

    void MasterViewer::_on_dpi_changed(winrt::Windows::Graphics::Display::DisplayInformation display_info) {
        _dxres.set_dpi(display_info.LogicalDpi());
    }

    void MasterViewer::_on_swapchain_size_changed(winrt::Windows::UI::Xaml::SizeChangedEventArgs args) {
        _dxres.set_logical_size(args.NewSize());
    }

    void MasterViewer::_on_display_content_invalidated() {
        _dxres.validate_device();
    }

    void MasterViewer::_on_composition_scale_changed(winrt::Windows::UI::Xaml::Controls::SwapChainPanel panel) {
        _dxres.set_composition_scale(panel.CompositionScaleX(), panel.CompositionScaleY());
    }
    
    void MasterViewer::_on_loaded() {
        _graphics.run_async();
    }

    void MasterViewer::_on_unloaded() {
        _graphics.request_stop([]() {});
    }

    int32_t MasterViewer::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void MasterViewer::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();
    }
}

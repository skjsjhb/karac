#!/usr/bin/env bash
mkdir -p build/webview2sdk
wget https://www.nuget.org/api/v2/package/Microsoft.Web.WebView2 -O build/webview2sdk/webview2sdk.zip
unzip build/webview2sdk/webview2sdk.zip -d build/webview2sdk/
rm build/webview2sdk/webview2sdk.zip
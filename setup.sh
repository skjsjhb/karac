#!/usr/bin/env bash
mkdir -p build/webview2sdk
curl -L -o build/webview2sdk/webview2sdk.zip https://www.nuget.org/api/v2/package/Microsoft.Web.WebView2
unzip -o build/webview2sdk/webview2sdk.zip -d build/webview2sdk/
rm build/webview2sdk/webview2sdk.zip
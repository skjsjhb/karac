#include "webview.h"
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <iostream>
#include "cJSON.h"
#include "loader.hh"
#include "tweaks.hh"

static int width = 960, height = 540;

int main() {
    const char *debugStr = getenv("KARA_DEBUG");
    if (debugStr == nullptr) {
        debugStr = "0";
    }
    bool isDebug = strcmp(debugStr, "1") == 0;
    std::string id = getenv("KARA_ID");
    std::string wsToken = getenv("KARA_WS_TOKEN");
    std::string wsPort = getenv("KARA_WS_PORT");

    std::cout << "karac instance " << id << ", ws port " << wsPort << '\n';

    std::string loaderScript = KARAC_DEFAULT_LOADER;

    const char *loaderPath = getenv("KARA_LOADER_PATH");
    if (loaderPath != nullptr) {
        std::cout << "Patching loader with: " << loaderPath << '\n';
        std::ifstream loaderFile(loaderPath);
        std::stringstream loaderContent;
        loaderContent << loaderFile.rdbuf();
        loaderScript += loaderContent.str();
    }

    webview::webview w(isDebug, nullptr);

    std::cout << "Binding methods.\n";

    w.bind("_KARA_NAVIGATE_", [&w](const std::string &req) -> std::string {
        cJSON *args = cJSON_Parse(req.c_str());
        std::string url = cJSON_GetStringValue(cJSON_GetArrayItem(args, 0));
        w.navigate(url);
        cJSON_Delete(args);
        return "";
    });

    w.bind("_KARA_SET_TITLE_", [&w](const std::string &req) -> std::string {
        cJSON *args = cJSON_Parse(req.c_str());
        std::string title = cJSON_GetStringValue(cJSON_GetArrayItem(args, 0));
        w.set_title(title);
        cJSON_Delete(args);
        return "";
    });

    w.bind("_KARA_STOP_", [&w](const std::string &req) -> std::string {
        w.terminate();
        return "";
    });

    w.bind("_KARA_SET_HTML_", [&w](const std::string &req) -> std::string {
        cJSON *args = cJSON_Parse(req.c_str());
        std::string html = cJSON_GetStringValue(cJSON_GetArrayItem(args, 0));
        w.set_html(html);
        cJSON_Delete(args);
        return "";
    });

    w.bind("_KARA_EVAL_", [&w](const std::string &req) -> std::string {
        cJSON *args = cJSON_Parse(req.c_str());
        std::string html = cJSON_GetStringValue(cJSON_GetArrayItem(args, 0));
        w.eval(html);
        cJSON_Delete(args);
        return "";
    });

    w.bind("_KARA_SET_SIZE_", [&w](const std::string &req) -> std::string {
        cJSON *args = cJSON_Parse(req.c_str());
        double wx = cJSON_GetNumberValue(cJSON_GetArrayItem(args, 0));
        double wy = cJSON_GetNumberValue(cJSON_GetArrayItem(args, 1));
        w.set_size(width = static_cast<int>(wx), height = static_cast<int>(wy), WEBVIEW_HINT_NONE);
        cJSON_Delete(args);
        return "";
    });

    w.bind("_KARA_SCALE_SIZE_", [&w](const std::string &req) -> std::string {
        cJSON *args = cJSON_Parse(req.c_str());
        double xs = cJSON_GetNumberValue(cJSON_GetArrayItem(args, 0));
        double ys = cJSON_GetNumberValue(cJSON_GetArrayItem(args, 1));
        w.set_size(width = static_cast<int>(width * xs), height = static_cast<int>(height * ys), WEBVIEW_HINT_NONE);
        cJSON_Delete(args);
        return "";
    });

    w.bind("_KARA_SET_FRAME_", [&w](const std::string &req) -> std::string {
        cJSON *args = cJSON_Parse(req.c_str());
        bool frame = cJSON_IsTrue(cJSON_GetArrayItem(args, 0));
        toggleWindowFrame(w.window(), frame);
        cJSON_Delete(args);
        return "";
    });

    w.bind("_KARA_OPEN_DEVTOOLS_", [&w](const std::string &req) -> std::string {
#if defined(_WIN32) || defined(__CYGWIN__) || defined(__MINGW64__)
        w.m_webview->OpenDevToolsWindow();
#elif defined(__linux__)
        WebKitSettings *settings = webkit_web_view_get_settings(WEBKIT_WEB_VIEW(w.m_webview));
        g_object_set(G_OBJECT(settings), "enable-developer-extras", TRUE, NULL);
        WebKitWebInspector *inspector = webkit_web_view_get_inspector(WEBKIT_WEB_VIEW(w.m_webview));
        webkit_web_inspector_show(WEBKIT_WEB_INSPECTOR(inspector));
#endif
        return "";
    });

    std::cout << "Setting init script.\n";
    w.init("window._KARA_ID_ = \"" + id + "\";");
    w.init("window._KARA_WS_PORT_ = \"" + wsPort + "\";");
    w.init("window._KARA_WS_TOKEN_ = \"" + wsToken + "\";");
    w.init(loaderScript);
    w.set_size(width, height, WEBVIEW_HINT_NONE);
    w.set_title("karac");
    w.set_html("<p>karac " + id + "</p>");

    std::cout << "Starting window main loop." << std::endl;
    w.run();
    w.terminate();
    return 0;
}


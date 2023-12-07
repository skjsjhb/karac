#include "webview.h"
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <iostream>
#include "cJSON.h"

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

    const char *loaderPath = getenv("KARA_LOADER_PATH");
    if (loaderPath == nullptr) {
        loaderPath = "kara-loader.js";
    }

    std::cout << "Using loader: " << loaderPath << '\n';

    std::ifstream loaderFile(loaderPath);
    std::stringstream loaderContent;
    loaderContent << loaderFile.rdbuf();
    std::string loaderScript = loaderContent.str();

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
        double width = cJSON_GetNumberValue(cJSON_GetArrayItem(args, 0));
        double height = cJSON_GetNumberValue(cJSON_GetArrayItem(args, 1));
        w.set_size(static_cast<int>(width), static_cast<int>(height), WEBVIEW_HINT_NONE);
        cJSON_Delete(args);
        return "";
    });

    std::cout << "Patching init script.\n";
    w.init("window._KARA_ID_ = \"" + id + "\";");
    w.init("window._KARA_WS_PORT_ = \"" + wsPort + "\";");
    w.init("window._KARA_WS_TOKEN_ = \"" + wsToken + "\";");
    w.init(loaderScript);
    w.set_title("karac");
    w.set_html("<p>karac " + id + "</p>");

    std::cout << "Starting window main loop." << std::endl;
    w.run();
    w.terminate();
    return 0;
}


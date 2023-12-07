window._KARA_WS_ = new WebSocket("ws://localhost:" + window._KARA_WS_PORT_);
window._KARA_WS_.onmessage = (e) => {
    const {token, type, body} = JSON.parse(e.data.toString());
    if (token !== window._KARA_WS_TOKEN_) return;
    if (type === "system") {
        const {method, args} = JSON.parse(body);
        if (method === "navigate") {
            window._KARA_NAVIGATE_(args[0]);
        }
        if (method === "setTitle") {
            window._KARA_SET_TITLE_(args[0]);
        }
        if (method === "stop") {
            window._KARA_STOP_();
        }
        if (method === "setHTML") {
            window._KARA_SET_HTML_(args[0]);
        }
        if (method === "eval") {
            window._KARA_EVAL_(args[0]);
        }
        if (method === "setSize") {
            window._KARA_SET_SIZE_(args[0], args[1]);
        }
    }
};
window._KARA_WS_.onopen = () => {
    window._KARA_WS_.send(JSON.stringify({
        id: window._KARA_ID_,
        token: window._KARA_WS_TOKEN_,
        body: "_WS_REG_"
    }));
}
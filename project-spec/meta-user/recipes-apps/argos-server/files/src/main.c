#include <libwebsockets.h>
#include <string.h>
#include <signal.h>

static int interrupted = 0;
static void sigint_handler(int sig) { (void)sig; interrupted = 1; }

/* Per-connection state */
struct session_data {
    char buf[LWS_PRE + 512];
    size_t len;
};

static int callback_echo(
    struct lws *wsi,
    enum lws_callback_reasons reason,
    void *user,
    void *in,
    size_t len)
{
    struct session_data *pss = (struct session_data *)user;

    switch (reason) {
    case LWS_CALLBACK_ESTABLISHED:
        lwsl_user("client connected\n");
        break;

    case LWS_CALLBACK_RECEIVE:
        /* 'in'/'len' is the payload that just arrived */
        if (len > sizeof(pss->buf) - LWS_PRE)
            len = sizeof(pss->buf) - LWS_PRE;
        memcpy(pss->buf + LWS_PRE, in, len);
        pss->len = len;
        /* ask lws to call us back when the socket is writable */
        lws_callback_on_writable(wsi);
        break;

    case LWS_CALLBACK_SERVER_WRITEABLE:
        if (pss->len) {
            lws_write(wsi, (unsigned char *)pss->buf + LWS_PRE,
                       pss->len, LWS_WRITE_TEXT);
            pss->len = 0;
        }
        break;

    case LWS_CALLBACK_CLOSED:
        lwsl_user("client disconnected\n");
        break;

    default:
        break;
    }
    return 0;
}

static const struct lws_protocols protocols[] = {
    {
        .name = "echo-protocol",
        .callback = callback_echo,
        .per_session_data_size = sizeof(struct session_data),
        .rx_buffer_size = 512,
    },
    LWS_PROTOCOL_LIST_TERM
};

int main(void)
{
    signal(SIGINT, sigint_handler);

    struct lws_context_creation_info info;
    memset(&info, 0, sizeof(info));
    info.port = 7681;
    info.protocols = protocols;
    info.options = LWS_SERVER_OPTION_HTTP_HEADERS_SECURITY_BEST_PRACTICES_ENFORCE;

    struct lws_context *ctx = lws_create_context(&info);
    if (!ctx) {
        lwsl_err("failed to create lws context\n");
        return 1;
    }

    while (!interrupted)
        lws_service(ctx, 0); /* 0 = don't add an extra internal poll timeout */

    lws_context_destroy(ctx);
    return 0;
}

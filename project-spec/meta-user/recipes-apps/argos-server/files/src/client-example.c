// #include <libwebsockets.h>
// #include <string.h>

// static int connected = 0;
// static int should_close = 0;

// static int
// callback_client(struct lws *wsi, enum lws_callback_reasons reason,
//                 void *user, void *in, size_t len)
// {
//     (void)user;
//     switch (reason) {
//     case LWS_CALLBACK_CLIENT_ESTABLISHED:
//         connected = 1;
//         lws_callback_on_writable(wsi);
//         break;

//     case LWS_CALLBACK_CLIENT_RECEIVE:
//         lwsl_user("received %d bytes: %.*s\n", (int)len, (int)len, (char *)in);
//         break;

//     case LWS_CALLBACK_CLIENT_WRITEABLE: {
//         unsigned char buf[LWS_PRE + 64];
//         const char *msg = "hello from client";
//         size_t n = strlen(msg);
//         memcpy(buf + LWS_PRE, msg, n);
//         lws_write(wsi, buf + LWS_PRE, n, LWS_WRITE_TEXT);
//         break;
//     }

//     case LWS_CALLBACK_CLIENT_CONNECTION_ERROR:
//         lwsl_err("connection error: %s\n", in ? (char *)in : "(no info)");
//         should_close = 1;
//         break;

//     case LWS_CALLBACK_CLIENT_CLOSED:
//         should_close = 1;
//         break;

//     default:
//         break;
//     }
//     return 0;
// }

// static const struct lws_protocols protocols[] = {
//     { .name = "echo-protocol", .callback = callback_client },
//     LWS_PROTOCOL_LIST_TERM
// };

// int main(void)
// {
//     struct lws_context_creation_info info;
//     memset(&info, 0, sizeof(info));
//     info.port = CONTEXT_PORT_NO_LISTEN; /* client-only context */
//     info.protocols = protocols;

//     struct lws_context *ctx = lws_create_context(&info);

//     struct lws_client_connect_info ccinfo;
//     memset(&ccinfo, 0, sizeof(ccinfo));
//     ccinfo.context = ctx;
//     ccinfo.address = "127.0.0.1";
//     ccinfo.port = 7681;
//     ccinfo.path = "/";
//     ccinfo.host = ccinfo.address;
//     ccinfo.origin = ccinfo.address;
//     ccinfo.protocol = protocols[0].name;

//     struct lws *wsi = lws_client_connect_via_info(&ccinfo);
//     if (!wsi) {
//         lwsl_err("client connect failed\n");
//         return 1;
//     }

//     while (!should_close)
//         lws_service(ctx, 0);

//     lws_context_destroy(ctx);
//     return 0;
// }
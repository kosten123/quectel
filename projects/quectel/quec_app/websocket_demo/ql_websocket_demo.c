/**
 * @file ql_websocket_demo.c
 * @author jay.dong@quectel.com
 * @brief 
 * @version 0.1
 * @date 2023-04-27
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "mongoose.h"
#include<quec_osi/ql_osi.h>
#include<quec_common/ql_defines.h>

#if 1//CFG_SUPPORT_MONGOOSE
beken_thread_t tcp_thread_handle = NULL;

static const char *s_listen_on = "ws://0.0.0.0:8000";
static const char *s_web_root = ".";

static void cb(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
    if (ev == MG_EV_READ)
    {
        // bk_printf("recv:[%d]->%s\r\n", c->recv.len, c->recv.buf);
        // mg_send(c, c->recv.buf, c->recv.len); // Echo received data back

        // mg_iobuf_del(&c->recv, 0, c->recv.len); // And discard it
    }
    else if (ev == MG_EV_HTTP_MSG)
    {
        struct mg_http_message *hm = (struct mg_http_message *)ev_data;
        if (mg_http_match_uri(hm, "/websocket"))
        {
            // Upgrade to websocket. From now on, a connection is a full-duplex
            // Websocket connection, which will receive MG_EV_WS_MSG events.
            mg_ws_upgrade(c, hm, NULL);
        }
        else if (mg_http_match_uri(hm, "/rest"))
        {
            // Serve REST response
            mg_http_reply(c, 200, "", "{\"result\": %d}\n", 123);
        }
        else
        {
            // Serve static files
            struct mg_http_serve_opts opts = {.root_dir = s_web_root};
            mg_http_serve_dir(c, ev_data, &opts);
        }
    }
    else if (ev == MG_EV_WS_MSG)
    {
        // Got websocket frame. Received data is wm->data. Echo it back!
        struct mg_ws_message *wm = (struct mg_ws_message *)ev_data;
        mg_ws_send(c, wm->data.ptr, wm->data.len, WEBSOCKET_OP_TEXT);
    }
}

void tcp_server_entry(void *arg)
{
    struct mg_mgr mgr;
    mg_mgr_init(&mgr);                               // Init manager
    mg_listen(&mgr, "tcp://0.0.0.0:1234", cb, &mgr); // Setup listener
    mg_http_listen(&mgr, s_listen_on, cb, NULL);  // Create HTTP listener
    for (;;)
    {
        mg_mgr_poll(&mgr, 500); // Event loop
        ql_rtos_task_sleep_ms(500);
    }
    mg_mgr_free(&mgr); // Cleanup
    tcp_thread_handle = NULL;
    ql_rtos_task_delete(NULL);
}

/*
  1.download the program to board
  2.connect the board to the wifi
  3.check the board ip addr
  4.connect the server ;  eg: ws://192.168.50.238:8000/websocket
*/

 int tcp_server_init(void)
{

    int err = 0;

    if (tcp_thread_handle == NULL)
    {
        // todo  create new thread;
        err = ql_rtos_task_create(&tcp_thread_handle,
                              (unsigned short)8*1024,
                              BEKEN_APPLICATION_PRIORITY,
                              "tcpwebsocket_demo",
                              tcp_server_entry,
                              0);
        if (err != kNoErr)
        {
            os_printf("tcp server start error \r\n");
            return -1;
        }

        
    }
    os_printf("tcp server start success \r\n");
    return 0;
}

#endif


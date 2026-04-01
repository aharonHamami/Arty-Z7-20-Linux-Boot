#include "arty_sys/net_tcp.hpp"
#include "btn_led_controller.h"

int run_bridge_event_loop(int serial_fd, TcpServer* net_server, BtnLedController* btn_led_io);

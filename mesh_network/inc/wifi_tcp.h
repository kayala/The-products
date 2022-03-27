#ifndef _WIFI_TCP_H_
#define _WIFI_TCP_H_

#include <string.h>
#include <sys/param.h>
#include <sys/socket.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "tcpip_adapter.h"
#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include <lwip/netdb.h>

void tcp_ini_client(void);

#define OPENSSL_DEMO_THREAD_STACK_WORDS 10240
#define OPENSSL_DEMO_THREAD_PRORIOTY    8


#endif /* MAIN_TEST_H_ */
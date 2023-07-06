#include "test_common.h"
TEST_START
#if !PIKA_NANO_ENABLE

extern "C" {
#include "../package/pikascript/pikascript-lib/PikaStdDevice/pika_hal.h"
int pika_hal_platform_WIFI_open(pika_dev* dev, char* name) {
    pika_platform_printf("pika_hal_platform_WIFI_open: %s\r\n", name);
    return 0;
}

int pika_hal_platform_WIFI_close(pika_dev* dev) {
    return 0;
}

int pika_hal_platform_WIFI_ioctl_config(pika_dev* dev,
                                        pika_hal_WIFI_config* cfg) {
    return 0;
}

int pika_hal_platform_WIFI_ioctl_enable(pika_dev* dev) {
    pika_platform_printf(
        "pika_hal_platform_WIFI_ioctl_others: "
        "PIKA_HAL_IOCTL_WIFI_SET_ACTIVE:%d\r\n",
        1);
    return 0;
}

int pika_hal_platform_WIFI_ioctl_disable(pika_dev* dev) {
    return 0;
}

int pika_hal_platform_WIFI_ioctl_others(pika_dev* dev,
                                        PIKA_HAL_IOCTL_CMD cmd,
                                        void* arg) {
    if (cmd == PIKA_HAL_IOCTL_WIFI_SCAN) {
        pika_platform_printf(
            "pika_hal_platform_WIFI_ioctl_others: "
            "PIKA_HAL_IOCTL_WIFI_SCAN\r\n");
        int count = 3;
        size_t size = sizeof(pika_hal_WIFI_scan_result) +
                      count * sizeof(pika_hal_WIFI_record);
        pika_hal_WIFI_scan_result* result =
            (pika_hal_WIFI_scan_result*)pika_platform_malloc(size);
        memset(result, 0, size);
        result->count = count;
        strcpy(result->records[0].ssid, "ssid1");
        result->records[0].channel = 1;
        result->records[0].rssi = -10;
        result->records[0].authmode = 0;
        strcpy(result->records[1].ssid, "ssid2");
        result->records[1].channel = 2;
        result->records[1].rssi = -20;
        result->records[1].authmode = 1;
        strcpy(result->records[2].ssid, "ssid3");
        result->records[2].channel = 3;
        result->records[2].rssi = -30;
        result->records[2].authmode = 2;
        *(pika_hal_WIFI_scan_result**)arg = result;
        return 0;
    }
    if (cmd == PIKA_HAL_IOCTL_WIFI_GET_STATUS) {
        pika_platform_printf(
            "pika_hal_platform_WIFI_ioctl_others: "
            "PIKA_HAL_IOCTL_WIFI_GET_STATUS\r\n");
        *(PIKA_HAL_WIFI_STATUS*)arg = PIKA_HAL_WIFI_STATUS_GOT_IP;
        return 0;
    }
    if (cmd == PIKA_HAL_IOCTL_WIFI_CONNECT){
        return 0;
    }
    return -1;
}
}

TEST(network, scan) {
    g_PikaMemInfo.heapUsedMax = 0;
    PikaObj* pikaMain = newRootObj("pikaMain", New_PikaMain);
    extern unsigned char pikaModules_py_a[];
    obj_linkLibrary(pikaMain, pikaModules_py_a);
    /* run */
    __platform_printf("BEGIN\r\n");
    pikaVM_runSingleFile(pikaMain, "test/python/network/network_scan.py");
    /* assert */
    EXPECT_STREQ(log_buff[0],
                 "[('ssid1', b'', 1, -10, 0, 0), ('ssid2', b'', 2, "
                 "-20, 1, 0), ('ssid3', b'', 3, -30, 2, 0)]\r\n");
    EXPECT_STREQ(log_buff[1],
                 "pika_hal_platform_WIFI_ioctl_others: "
                 "PIKA_HAL_IOCTL_WIFI_SCAN\r\n");
    EXPECT_STREQ(log_buff[2],
                 "pika_hal_platform_WIFI_ioctl_others: "
                 "PIKA_HAL_IOCTL_WIFI_SET_ACTIVE:1\r\n");
    EXPECT_STREQ(log_buff[3], "pika_hal_platform_WIFI_open: WLAN0\r\n");
    /* deinit */
    obj_deinit(pikaMain);
    EXPECT_EQ(pikaMemNow(), 0);
}

TEST(network, connect) {
    g_PikaMemInfo.heapUsedMax = 0;
    PikaObj* pikaMain = newRootObj("pikaMain", New_PikaMain);
    extern unsigned char pikaModules_py_a[];
    obj_linkLibrary(pikaMain, pikaModules_py_a);
    /* run */
    __platform_printf("BEGIN\r\n");
    pikaVM_runSingleFile(pikaMain, "test/python/network/network_connect.py");
    /* assert */
#if 0
    pika_hal_platform_WIFI_open: WLAN0
    pika_hal_platform_WIFI_ioctl_others: PIKA_HAL_IOCTL_WIFI_SET_ACTIVE:1
    pika_hal_platform_WIFI_ioctl_others: PIKA_HAL_IOCTL_WIFI_GET_STATUS
    1
    ('192.168.1.123', '255.255.255.0', '192.168.1.1', '8.8.8.8')
#endif
    EXPECT_STREQ(log_buff[3], "pika_hal_platform_WIFI_open: WLAN0\r\n");
    EXPECT_STREQ(log_buff[2],
                 "pika_hal_platform_WIFI_ioctl_others: "
                 "PIKA_HAL_IOCTL_WIFI_SET_ACTIVE:1\r\n");
    EXPECT_STREQ(log_buff[1],
                 "pika_hal_platform_WIFI_ioctl_others: "
                 "PIKA_HAL_IOCTL_WIFI_GET_STATUS\r\n");
    EXPECT_STREQ(log_buff[0], "1\r\n");
    /* deinit */
    obj_deinit(pikaMain);
    EXPECT_EQ(pikaMemNow(), 0);
}

TEST(network, config) {
    g_PikaMemInfo.heapUsedMax = 0;
    PikaObj* pikaMain = newRootObj("pikaMain", New_PikaMain);
    extern unsigned char pikaModules_py_a[];
    obj_linkLibrary(pikaMain, pikaModules_py_a);
    /* run */
    __platform_printf("BEGIN\r\n");
    pikaVM_runSingleFile(pikaMain, "test/python/network/network_config.py");
    /* assert */
    EXPECT_STREQ(log_buff[0], "configssid 11\r\n");
    /* deinit */
    obj_deinit(pikaMain);
    EXPECT_EQ(pikaMemNow(), 0);
}

#endif
TEST_END

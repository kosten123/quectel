#include "wizchip_conf.h"
#if (_WIZCHIP_ == W5500)
#include "w5500/w5500.h"
#else
#error other wizchip not supported now.
#endif

#include<quec_osi/ql_osi.h>
#include<quec_common/ql_defines.h>
#include<quec_spi/ql_spi.h>
#include<quec_gpio/ql_gpio.h>



//#define wz_log(msg, ...)	custom_log("wz_drv", msg, ##__VA_ARGS__)
#define wz_log  bk_printf
#define SPI_TEST_PORT       QL_SPI_ID0
#define SPI_MASTER_CLK      (10000000)

extern void timerDelay(unsigned long USec);
static int(*wizchip_recvhandler)(uint8_t *data, uint32_t len) = NULL;


#define _PORT_SPI_CS_DEFAULT    GPIO_45  //CS will be init in the spi init process ,actually CS  isn't used in FCM561D
#define _PORT_EINT_PIN_DEFAULT  QL_GPIO20
#define _PORT_RESET_PIN_DEFAULT QL_GPIO21

static struct {
    int cs_pin;
    int eint_pin;
    int reset_pin;
    ql_mutex_t cris_mutex;
    ql_sem_t isr_sem;
    ql_task_t isr_handler;
} wizchip_port = {
    _PORT_SPI_CS_DEFAULT,
    _PORT_EINT_PIN_DEFAULT,
    _PORT_RESET_PIN_DEFAULT
};


static void wizchip_w5500_spi_init(void)
{
	ql_spi_config_s spi_test_cfg;
	
	spi_test_cfg.cpha =QL_SPI_CPHA_1ST_EDGE;
	spi_test_cfg.cpol =QL_SPI_CPOL_LOW;
	spi_test_cfg.role = QL_SPI_ROLE_MASTER;
	spi_test_cfg.spiclk = SPI_MASTER_CLK;
	spi_test_cfg.bit_order = QL_SPI_MSB_FIRST;
    spi_test_cfg.wire_mode   = QL_SPI_4WIRE_MODE;
    spi_test_cfg.bit_width   = QL_SPI_BIT_WIDTH_8BITS;

	ql_spi_init(SPI_TEST_PORT,&spi_test_cfg);
  }

static void ql_spi_write_read(uint8_t* wBuf, uint8_t* rBuf, uint16_t tx_len,uint16_t rx_len)
{
	int i;
	ql_spi_message_s msg;

	os_memset(&msg, 0, sizeof(ql_spi_message_s));
	
	msg.send_buf = wBuf;
	msg.send_len = tx_len;
	msg.recv_buf = rBuf;
	msg.recv_len = rx_len;

    bk_spi_master_xfer(SPI_TEST_PORT,&msg);
	for( i  = 0; i < tx_len; i++)
	{
	//	bk_printf("read buf[%d]:%d \n ", i, rBuf[i]);
	}
}

static void ql_spi_write(uint8_t* pBuf, uint16_t len)
{
	int i;
	 ql_spi_message_s msg;
    
	msg.recv_buf = NULL;
	msg.recv_len = 0;
	msg.send_buf = pBuf;
	msg.send_len = len;

    bk_spi_master_xfer(SPI_TEST_PORT,&msg);
	for( i  = 0; i < len; i++)
	{
		//bk_printf(" %s %d wbuf[%d]:%02X\n", __FUNCTION__, __LINE__, i, pBuf[i]);
	}
}


static void ql_spi_read(uint8_t* pBuf, uint16_t len)
{
	int i;
	 ql_spi_message_s msg;

	msg.recv_buf = pBuf;
	msg.recv_len = len;
	msg.send_buf = NULL;
	msg.send_len = 0;

     bk_spi_master_xfer(SPI_TEST_PORT,&msg);
	for( i  = 0; i < len; i++)
	{
	//	bk_printf(" %s %d rbuf[%d]:%02X\n", __FUNCTION__, __LINE__, i, pBuf[i]);
	}
}


static void wizchip_eint_isr(gpio_id_t gpio_id) {
    //wz_log("eint\r\n");
   // ql_eint_enable(wizchip_port.eint_pin, PIN_FALLING_EDGE);
    ql_rtos_semaphore_release(wizchip_port.isr_sem);
		
}

static void wizchip_eint_isr_handler(void) {
    //static uint32_t int_pin_high_ref = 0;
    uint8_t ir = getIR();
    uint8_t sir = getSIR();
    uint8_t s0_ir = getSn_IR(0);
    /* clear interrupt flags */
    if(s0_ir) setSn_IR(0, s0_ir);
    if(sir) setSIR(sir);
    if(ir) setIR(ir);

    /* ip conflict process */
    if(ir & IK_IP_CONFLICT) {
        
    }
    
    /* destination unreach process */
    if(ir & IK_DEST_UNREACH) {
        
    }
    
    /* wake-on-lan process */
    if(ir & IK_WOL) {
        
    }
    
    /* socket-0 process */
    if(sir & (IK_SOCK_0 >> 8)) {
        
        /* raw data send ok */
        if(s0_ir & Sn_IR_SENDOK) {
            
        }
    
        /* recv raw data  */
        if(s0_ir & Sn_IR_RECV) {
            uint16_t total_len;
            uint16_t len = 0;
            uint8_t *data = NULL;
            uint8_t head[2] = {0};
            
            while(!!(total_len =getSn_RX_RSR(0)))
            {
                //wz_log("total mac length: %d", total_len);
                //bk_printf("total mac length: %d", total_len);
                /* read first 2 bytes those saved mac-frame length */
                wiz_recv_data(0, head, 2);
                /* trigger next recving */
                setSn_CR(0, Sn_CR_RECV);
                while(getSn_CR(0));
    
                /* get mac-frame length */
                len = (head[0] << 8) + head[1] - 2;
    
                if(len) {
                    /* read mac-frame */
                    data = malloc(len);
                    wiz_recv_data(0, data, len);           
                    /* trigger next recving */
                    setSn_CR(0, Sn_CR_RECV);
                    while(getSn_CR(0));
    
                    /* deliver to lwip */
                    if(wizchip_recvhandler)
                        wizchip_recvhandler(data, len);
    
                    free(data);
                }
            }
        }
    }

    UINT8 level = 1;
    level=ql_gpio_get_level(wizchip_port.eint_pin);
    if(level == 0) {
        //wz_log("another event comes during last process");
        //int_pin_high_ref = 0;
        ql_rtos_semaphore_release(wizchip_port.isr_sem);
    }
#if 0  // when external interrupt if off, should enable this snip below
    else {
        wz_log("opus~~");
        int_pin_high_ref++;
        if(int_pin_high_ref <= 50) {
            ql_rtos_task_sleep_ms(10);
            ql_rtos_semaphore_release(wizchip_port.isr_sem);
        } else {
            int_pin_high_ref = 0;
        }
    }
#endif
}

static void wizchip_eint_isr_handler_task(void *argv) {
    (void)argv;

    while(1) {
        ql_rtos_semaphore_wait(wizchip_port.isr_sem, BEKEN_NEVER_TIMEOUT);
        wizchip_eint_isr_handler();
    }
}

static void wizchip_port_low_init(void) {
	int ret = -1;

   // ql_gpio_init(wizchip_port.cs_pin, QL_GMODE_OUTPUT,QL_GPIO_LEVEL_HIGH);
    ql_gpio_init(wizchip_port.reset_pin, QL_GMODE_OUTPUT,QL_GPIO_LEVEL_HIGH);
    ql_rtos_mutex_create(&wizchip_port.cris_mutex);
    ql_rtos_semaphore_create(&wizchip_port.isr_sem, 1);
	bk_printf("lyy debug %s %d \n", __FUNCTION__, __LINE__);
#if 1
    ret = ql_rtos_task_create(&wizchip_port.isr_handler, 4096, 5, "wz_isr", wizchip_eint_isr_handler_task, 0);
	
	  if (ret != kNoErr) {
		bk_printf("Error: Failed to create wz_isr thread: %d\r\n",ret);
		
	  }
#endif
	//ql_eint_register(wizchip_port.eint_pin, PIN_FALLING_EDGE, PIN_PULL_PU, wizchip_eint_isr, NULL);
	//ql_gpio_int_init(wizchip_port.eint_pin, QL_IRQ_TRIGGER_FALLING_EDGE, wizchip_eint_isr);
    ql_gpio_init(wizchip_port.eint_pin, QL_GMODE_IN_PU, QL_GPIO_LEVEL_HIGH);
    ql_gpio_interrupt_init(wizchip_port.eint_pin, QL_IRQ_TRIGGER_FALLING_EDGE, wizchip_eint_isr);
    ql_gpio_interrupt_enable(wizchip_port.eint_pin, true);
  //  ql_eint_enable(wizchip_port.eint_pin, PIN_FALLING_EDGE);
}
static void wizchip_port_low_destroy()
{   
	//ql_gpio_set_level(wizchip_port.cs_pin, QL_GPIO_LEVEL_HIGH); // disable spi
    ql_gpio_interrupt_init(wizchip_port.eint_pin, QL_IRQ_TRIGGER_FALLING_EDGE, NULL);

	//ql_gpio_deinit(wizchip_port.reset_pin);     // deinit gpio
    ql_gpio_deinit();  
	ql_rtos_mutex_delete(wizchip_port.cris_mutex);  // del register mutex
	ql_rtos_semaphore_delete(wizchip_port.isr_sem); // del recv sem
	ql_rtos_task_delete(wizchip_port.isr_handler);  // del recv isr
}

static void wizchip_port_hw_reset(void) {
    ql_gpio_set_level(wizchip_port.reset_pin, QL_GPIO_LEVEL_LOW);
   // timerDelay(100);
	ql_rtos_task_sleep_ms(10);

    ql_gpio_set_level(wizchip_port.reset_pin, QL_GPIO_LEVEL_HIGH);
}

static void wizchip_port_cris_enter(void) {
    ql_rtos_mutex_lock(wizchip_port.cris_mutex);
}

static void wizchip_port_cris_exit(void) {
    ql_rtos_mutex_unlock(wizchip_port.cris_mutex);
}

static void wizchip_port_spi_take(void) {
   // ql_gpio_set_level(wizchip_port.cs_pin, QL_GPIO_LEVEL_LOW);
}

static void wizchip_port_spi_release(void) {
  //  ql_gpio_set_level(wizchip_port.cs_pin, QL_GPIO_LEVEL_HIGH);
}

static uint8_t wizchip_port_spi_readbyte(void) {
    uint8_t rdata = 0;
    ql_spi_read( &rdata, 1);
    //wz_log("read byte: 0x%02X", rdata);
    return rdata;
}

static void wizchip_port_spi_writebyte(uint8_t wdata) {
    //wz_log("write byte: 0x%02X", wdata);
    ql_spi_write( &wdata, 1);
}

static void wizchip_port_spi_readburst(uint8_t* pBuf, uint16_t len) {
	int i = 0;
    for(; i<len; i++) {
     //   bk_printf("read burst[%02d]: 0x%02X", i, pBuf[i]);

    }
	ql_spi_read( pBuf, len);
}

static void wizchip_port_spi_writeburst(uint8_t* pBuf, uint16_t len) {
    int i = 0;
    for(; i<len; i++) {
     //   bk_printf("write burst[%02d]: 0x%02X   ", i, pBuf[i]);
    }
    ql_spi_write( pBuf, len);
}

static void wizchip_port_spi_wrburst(uint8_t* wBuf, uint8_t* rBuf, uint16_t len, uint16_t rlen) {
    int i;
    if(wBuf) {
        for(i = 0; i<len; i++) {
          //  wz_log("wr_w burst[%02d]: 0x%02X   ", i, wBuf[i]);
        }
    }
    
    if(wBuf != NULL && rBuf != NULL) {
        ql_spi_write_read(wBuf, rBuf, len, rlen);
    } else if(wBuf != NULL) {
        ql_spi_write(wBuf, len);
    } else if(rBuf != NULL) {
        ql_spi_read(rBuf, len);
    }
    
    if(rBuf) {
        for(i = 0; i<rlen; i++) {
         //   wz_log("wr_r burst[%02d]: 0x%02X", i, rBuf[i]);
        }
    }
}

void wizchip_set_port(int spi_port, int cs_pin, int eint_pin, int reset_pin) {
    #define CHANGED(x) (x != -1)
    if(CHANGED(cs_pin))     wizchip_port.cs_pin = cs_pin;
    if(CHANGED(eint_pin))   wizchip_port.eint_pin = eint_pin;
    if(CHANGED(reset_pin))  wizchip_port.reset_pin = reset_pin;
}

void wizchip_set_recvhandler(int(*recv_data_hdlr)(uint8_t *data, uint32_t len)) {
    wizchip_recvhandler = recv_data_hdlr;
}

void wizchip_send_data(uint8_t *data, uint32_t len) {
    uint16_t freesize = 0;
    uint16_t len_each = 0;

    /* send data when len is not 0 */
    while(len) {
        /* get freesize of TX cache */
        freesize = getSn_TX_FSR(0);
		//bk_printf("lyy debug %s %d freesize:%d \n", __FUNCTION__, __LINE__, freesize);
        /* get length of each sending */
        if(len > freesize) len_each = freesize;
        else len_each = len;

        /* sending when freesize is not 0 last time */
        if(len_each)
            wiz_send_data(0, data, len_each);

        len -= len_each;
        data += len_each;
    }

    /* trigger current sending */
   setSn_CR(0, Sn_CR_SEND);
   // Sn_CR_SEND_MAC
   getSn_CR(0);
//	bk_printf("lyy debug %s %d val:%d \n", __FUNCTION__, __LINE__, val);
    while(getSn_CR(0));
}

int wizchip_port_init(uint8_t mac[6], uint8_t ip[4], uint8_t sn[4], uint8_t gw[4]) {
    /* bsp init */
    uint8_t tx_cache_size[_WIZCHIP_SOCK_NUM_] = {16};
    uint8_t rx_cache_size[_WIZCHIP_SOCK_NUM_] = {16};
    wizchip_w5500_spi_init();
    reg_wizchip_cris_cbfunc(wizchip_port_cris_enter, wizchip_port_cris_exit);
    //reg_wizchip_cs_cbfunc(wizchip_port_spi_take, wizchip_port_spi_release);
    reg_wizchip_cs_cbfunc(NULL, NULL);
   //reg_wizchip_spi_cbfunc(wizchip_port_spi_readbyte, wizchip_port_spi_writebyte);
    reg_wizchip_spiburst_cbfunc(wizchip_port_spi_readburst, wizchip_port_spi_writeburst);
    reg_wizchip_spiwrburst_cbfunc(wizchip_port_spi_wrburst);

    wizchip_port_low_init();
    wizchip_port_hw_reset();

    /* read version */
    uint16_t ver=0;
    uint16_t  try = 0;
    ql_rtos_task_sleep_ms(5);
    while(try++ < 5 && (ver = getVERSIONR())!=0x04)
    {
		ql_rtos_task_sleep_ms(10);
		wz_log("lyy debug %s %d try:%d\n", __FUNCTION__, __LINE__, try);		
	}
    if(ver == 0x04) {
        wz_log("w5500 version: 0x%02X \n", ver);
    }
    else {
        wz_log("w5500 version read fail,ver:0x%02X \n", ver);
		wizchip_port_low_destroy();
        return -1;
    }
    //bk_printf("lyy debug %s %d \n", __FUNCTION__, __LINE__);
    /* wiznet chip reset and init */
    wizchip_init(tx_cache_size, rx_cache_size);
    //bk_printf("lyy debug %s %d \n", __FUNCTION__, __LINE__);
    /* set int level */
    setINTLEVEL(0);
    
    /* net info conf, such as mac/ip/sn/gw */
    wiz_NetInfo netinfo = {0};
    memcpy(netinfo.mac, mac, 6);
    memcpy(netinfo.ip, ip, 4);
    memcpy(netinfo.sn, sn, 4);
    memcpy(netinfo.gw, gw, 4);
    wizchip_setnetinfo(&netinfo);
    /* phy conf */
    wiz_PhyConf phyconf = {PHY_CONFBY_SW, PHY_MODE_AUTONEGO, PHY_SPEED_100, PHY_DUPLEX_FULL};
    wizphy_setphyconf(&phyconf);
    /* net mode set as wake-on-lan */
    wizchip_setnetmode(NM_WAKEONLAN,1);

    /* interrupt conf: */
        /* -- enable ip-conflict, destination-unreachable, wake-on-lan and socket0 interrupt */
    wizchip_setinterruptmask(IK_IP_CONFLICT | IK_DEST_UNREACH | IK_WOL | IK_SOCK_0);
#if 0
        /* -- enable all socket0 interrupt */
    setSn_IMR(0, 0x1F);
	int temp_imr = 0;
	temp_imr = getSn_IMR(0);
	bk_printf("lyy debug %s %d temp_imr:%02x\n", __FUNCTION__, __LINE__, temp_imr);	
#else
        /* -- enable socket0 recving interrupt */
    setSn_IMR(0, 0x07);
#endif
	// bk_printf("lyy debug %s %d \n", __FUNCTION__, __LINE__);

    
    /* set protocol as mac-raw */
    setSn_MR(0, Sn_MR_MACRAW);

    /* open protocol stack */
    setSn_CR(0, Sn_CR_OPEN);
    do{
    }while(getSn_CR(0));
 
	int temp = getSn_SR(0);
  //  bk_printf("lyy debug %s %d temp:%d \n", __FUNCTION__, __LINE__, temp);
    /* wait for open success */
    do{
    }while(getSn_SR(0)!= SOCK_MACRAW);
    
	wz_log("lyy debug %s %d temp:%d \n", __FUNCTION__, __LINE__, temp);		
    return 0;
}

void wizchip_port_destroy()
{
	wizchip_port_hw_reset();
	setSn_IMR(0, 0);
	setSn_CR(0, Sn_CR_CLOSE);
	wizchip_port_low_destroy();
	wizchip_recvhandler = NULL;
}






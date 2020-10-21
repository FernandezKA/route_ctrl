unsigned char rxCount=0U;    /*Количество принятых байтов*/
unsigned char rxData[256U];  /*Массив с принятыми данными*/
unsigned char txCount=0U;    /*Количество данных для отправки*/
unsigned char txData[256U];  /*Массив с данными для отправки*/
unsigned char sig;           /*Сигнатура*/
unsigned short dest_id;      /*dest id девайса*/
unsigned char cmd;           /*Команда в пакете*/
unsigned char sz;            /*Размер пакета с данными*/
unsigned char crc_head;      /*Crc до байта с размером данных*/
unsigned char crc_tail;      /*Crc данных с раздела data*/
unsigned char crc_head_rec = 0xFFU;
unsigned char crc_tail_rec = 0xFFU;
_Bool status = 0;   /*булевская переменная для указания наличия необработанных приянтых данных*/
_Bool completed = 0; /*булевская переменная для указания статуса распознавания данных*/
unsigned char temp;  /*Временная переменная для большинства костылей в этой программе*/
/*************************************************************************************/
static void clock_init(void); /*настраиваем тактирование*/
static void port_init(void);  /*настраиваем порты мк, в том числе для периферии*/
static void uart_init(unsigned long baud_rate, unsigned long f_master); /*инициализируем модуль UART*/
static unsigned char addr_recognize(void);/*распознаем адрес устройства для инициализиации I2C */ 
static void I2C_init(const uint8_t address);  /*Настраиваем I2C*/
static void recognize_data(unsigned char data);  /*Функция распознавания принятых данных*/
static void uart_tx_byte(unsigned char data);    /*функция передачи байта по UART*/
static void uart_tx_data(unsigned char * data, unsigned char len);  /*Функция передачи массива по UART*/
/*static unsigned char uart_rx_byte(void);*/
__interrupt void uart_recieve(void);   /*Обработчик прерывания по приему UART*/
/*******************************************************************************/

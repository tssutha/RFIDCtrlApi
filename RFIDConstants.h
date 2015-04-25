
//ABx_FAST Protocol Type Codes
const CHAR ABx_FAST_WITHOUT_CHECKSUM  = 0x00;
const CHAR ABx_FAST_WITH_CHECKSUM     = 0x01;

//Existing controller Types that uses ABx Fast Protocol
const CHAR BIS_M_62_                =0x02;
const CHAR BIS_M_410_               =0x03;
const CHAR BIS_M_411_               =0x04;
const CHAR BIS_U_62_                =0x0B;

#define CTRL_TYPE_BIS_M_62          "BIS_M_62_"
#define CTRL_TYPE_BIS_M_410         "BIS_M_410_"
#define CTRL_TYPE_BIS_M_411         "BIS_M_411_"
#define CTRL_TYPE_BIS_U_62          "BIS_U_62_"

//Existing RFID Tag Type which uses ABx_FAST_PROTOCOL
const CHAR RFID_TAG_BIS_M_1__03_            =0x01;
const CHAR RFID_TAG_BIS_M_1__07_            =0x03;
const CHAR RFID_TAG_BIS_M_1__10_            =0x05;
const CHAR RFID_TAG_BIS_M_1__02_            =0x06;


//Device Interface Type
const INT RFID_COM_RS422                    =0x00;
const INT RFID_COM_RS232_USB                =0x01;
const INT RFID_COM_RS485                    =0x02;
const INT RFID_COM_OTHER                    =0x03;


// BIS-M-410-068-001-09-S72	 RFID CONTROLLER STATION ERROR CODES
// ABx FAST communication Protocol
const INT  ABx_FAST_ERR_FILL_TAG_FAILED				 =0x04; //Fill Tag operation failed
const INT  ABx_FAST_ERR_READ_DATA_FAILED             =0x05; //Read Data Command Failed
const INT  ABx_FAST_ERR_WRITE_DATA_FAILED            =0x06; //Write Data Command Failed
const INT  ABx_FAST_ERR_READ_TAG_ID_FAILED           =0x07; //Read Tag ID or Tag Search Command Failed [Tag not found]
const INT  ABx_FAST_ERR_INVALID_SYNTAX               =0x21; //Command Contained a Syntax Error
const INT  ABx_FAST_ERR_INVALID_TAG_TYPE             =0x23; //Invalid/Unsupported Tag type or RF Command
const INT  ABx_FAST_ERR_LOCK_TAG_BLOCK_FAILED        =0x27; //Lock tag block operation failed
const INT  ABx_FAST_ERR_INTERNAL_CONTOLLER_ERROR     =0x30; //Generic internal controller error, buffer overflow
const INT  ABx_FAST_ERR_INVALID_CONTROLLER_TYPE      =0x31; //Invalid controller type, when setting configuration
const INT  ABx_FAST_ERR_INVALID_PROGRAMMING_ADDRESS  =0x32; //Invalid Tag programming Address specified in the command
const INT  ABx_FAST_ERR_CRC_ERROR                    =0x33; //Cyclic Redundancy Check Error
const INT  ABx_FAST_ERR_INVALID_SOFTWARE_VERSION     =0x34; //Invalid Software version
const INT  ABx_FAST_ERR_INVALID_RESET                =0x35; //Invalid Hardware Reset
const INT  ABx_FAST_ERR_SET_CONFIGURATION_ERROR      =0x36; //Controller Configuration not written
const INT  ABx_FAST_ERR_GET_CONGIGURATION_ERROR	     =0x37; //Controller Configuration not read

const INT  ABx_FAST_CMD_ID_SIZE                      =0x01;
const INT  ABx_FAST_START_ADDRESS_SIZE               =0x02;
const INT  ABx_FAST_READ_WRITE_BLOCK_SIZE			 =0x02;
const INT  ABx_FAST_TIMEOUT_SIZE                     =0x02; 

///////////////////Command Data Packet Size////////////////////////////

// CMD_SIZE (1) + START_ADD(2) + BLOCK(2) + TIMEOUT(2) + DATA_BYTE(1)
const INT  ABx_FAST_FILL_TAG_COMMAND_SIZE            =0x0008; 
//CMD_SIZE(1) + START_ADD(2) + BLOCK(2) + TIMEOUT(2) 
const INT ABx_FAST_READ_TAG_COMMAND_SIZE             =0x0007; 
//CMD_SIZE(1) + START_ADD(2) + BLOCK(2) + TIMEOUT(2) + DATA_BYTE(N)
const INT ABx_FAST_WRITE_TAG_COMMAND_SIZE            =0x0007 + 0x0000;   // 1+2+2+2+N
const INT ABx_FAST_READ_TAG_ID_COMMAND_SIZE          =0x0003;
const INT ABx_FAST_TAG_SEARCH_COMMAND_SIZE           =0x0003;
const INT ABx_FAST_READ_TAGID_AND_DATA_CMD_SIZE      =0x0007;
const INT ABx_FAST_LOCK_MEMORY_BLOCK_CMD_SIZE        =0x0005;
const INT ABx_FAST_RESET_CONTROLLER_CMD_SIZE		 =0x0001;
const INT ABx_FAST_GET_CONTROLLER_CONFIG_CMD_SIZE    =0x0002;
const INT ABx_FAST_SET_CONTROLLER_CONFIG_CMD_SIZE    =0x0014;
const INT ABx_FAST_GET_CONTROLLER_INFO_CMD_SIZE      =0x0001;
const INT ABx_FAST_SET_CONTROLLER_TIME_CMD_SIZE      =0x0008;
const INT ABx_FAST_EXECUTE_MACRO_CMD_SIZE            =0x0002;


///////////////////Response Data Packet Size//////////////////////////
const INT ABx_FAST_FILL_TAG_RESP_SIZE                =0x0001;
const INT ABx_FAST_READ_TAG_RESP_SIZE                =0x0001 + 0x0000; //+N, Read Block size
const INT ABx_FAST_WRITE_TAG_RESP_SIZE               =0x0001;
const INT ABx_FAST_READ_TAGID_RESP_SIZE              =0x0009;  //Tag ID 8-Byte in Size
const INT ABx_FAST_TAG_SEARCH_RESP_SIZE              =0x0001;
const INT ABx_FAST_READ_TAGID_AND_DATA_RESP_SIZE     =0x0009 + 0x0000; //+N, Read Block Size
const INT ABx_FAST_LOCK_MEMORY_BLOCK_RESP_SIZE       =0x0001;
const INT ABx_FAST_RESET_CONTROLLER_RESP_SIZE        =0x0001;
const INT ABx_FAST_GET_CONTROLLER_CONFIG_RESP_SIZE   =0x0014;
const INT ABx_FAST_SET_CONTROLLER_CONFIG_RESP_SIZE   =0x0001;
const INT ABx_FAST_GET_CONTROLLER_INFO_RESP_SIZE     =0x001B;
const INT ABx_FAST_SET_CONTROLLER_TIME_RESP_SIZE     =0x0001;
const INT ABx_FAST_EXECUTE_MACRO_RESP_SIZE           =0x0001;


const INT ABx_FAST_TIMEOUT                           =0x00FA;//0x07D0;
const INT ABx_FAST_RESPONSE_PACKET_HEADER_TERMINATOR =0x0005;

const INT RFID_NOK   = -1;
const INT RFID_OK    = 0;

//Customize Error Codes For Proper Exception Handling 
const INT  RFID_OP_CUSTOM_ERR_CREATE_PORT_FAILED     =0x60; //Create Serial Port Failed
const INT  RFID_OP_CUSTOM_ERR_PORT_IS_OPEN           =0x61; //Port is already opened
const INT  RFID_OP_CUSTOM_ERR_PORT_IS_NOT_CREATED    =0x62; //Port is not created yet
const INT  RFID_OP_CUSTOM_ERR_PORT_IS_ALREADY_CREATED=0x63; //Port is already created
const INT  RFID_OP_CUSTOM_ERR_PORT_OPEN_FAILED       =0x64; //Open Port operation failed
const INT  RFID_OP_CUSTOM_ERR_ERROR_DURING_CMD_EXECU =0x65; //Error During Command Execution
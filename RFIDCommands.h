// BIS-M-410-068-001-09-S72	 RFID CONTROLLER COMMANDS
// ABx FAST communication Protocol

const INT ABx_FAST_CMD_HEADER                               =0x02;
const INT ABx_FAST_CMD_TERMINATOR                           =0x03;


////////////////RFID Tag Commands/////////////////////////////////
const INT ABx_FAST_CMD_FILL_TAG								=0x04;
const INT ABx_FAST_CMD_READ_TAG								=0x05;
const INT ABx_FAST_CMD_WRITE_TAG							=0x06;
const INT ABx_FAST_CMD_READ_TAG_ID							=0x07;
const INT ABx_FAST_CMD_TAG_SEARCH							=0x08;
const INT ABx_FAST_CMD_START_CONTINUOUS_READ				=0x0D;
const INT ABx_FAST_CMD_READ_TAGID_AND_DATA					=0x0E;
const INT ABx_FAST_CMD_START_CONTINUOUS_READ_TAGID_AND_DATA	=0x0F;
const INT ABx_FAST_CMD_LOCK_MEMORY_BLOCK					=0x27;
const INT ABx_FAST_CMD_READ_EPC_CODE                        =0xC2;
const INT ABx_FAST_CMD_WRITE_EPC_CODE                       =0xC3;

///////////////Controller Commands////////////////////////////////
const INT ABx_FAST_CMD_RESET_CONTROLLER						=0x35;
const INT ABx_FAST_CMD_SET_CONTROLLER_CONFIGURATION			=0x36;
const INT ABx_FAST_CMD_GET_CONTROLLER_CONFIGURATION			=0x37;
const INT ABx_FAST_CMD_GET_CONTROLLER_INFO					=0x38;
const INT ABx_FAST_CMD_SET_CONTROLLER_TIME					=0x51;
const INT ABx_FAST_CMD_EXECUTE_MACRO						=0x72;

///////////////Additional UHF-Series Controller Commands//////////
const INT ABx_FAST_CMD_SET_UHF_CONTORLLER_CONFIGURATION     =0xC0;
const INT ABx_FAST_CMD_GET_UHF_CONTORLLER_CONFIGURATION     =0xC1;

////////////// I/O Commands///////////////////////////////////////
const INT ABx_FAST_CMD_SET_CLEAR_DIGITAL_OUTPUTS			=0x10;
const INT ABx_FAST_CMD_GET_DIGITAL_INPUT					=0x11;
const INT ABx_FAST_CMD_GET_DIGITAL_OUTPUTS					=0x12;

//////////////Multi-Tag RFID Commands////////////////////////////
const INT ABx_FAST_CMD_MULTI_TAG_READ_ID_AND_DATA_ALL		=0x82;
const INT ABx_FAST_CMD_MULTI_TAG_BLOCK_READ_ALL				=0x85;
const INT ABx_FAST_CMD_MULTI_TAG_BLOCK_WRITE_ALL			=0x86;
const INT ABx_FAST_CMD_MULTI_TAG_GET_INVENTORY				=0x87;
const INT ABx_FAST_CMD_MULTI_TAG_SEARCH_ALL					=0x88;
const INT ABx_FAST_CMD_MULTI_TAG_BLOCK_READ_BY_ID			=0x95;
const INT ABx_FAST_CMD_MULTI_TAG_BLOCK_WRITE_BY_ID			=0x96;
const INT ABx_FAST_CMD_MULTI_TAG_READ_EPC_CODE              =0xC4;


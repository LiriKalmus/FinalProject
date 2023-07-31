
typedef struct code
{

    int are;

    int type; // 3 types: opcode, register, value.

    union bits
    {
        // in case of DATA:
        // 10 bit's of data.
        data Data;


        // in case of OPCODE
        

        Opcode op;
        // the opcode value
        int opcode;
        // the src method
        int src_addressing;
        // the address method
        int dst_addressing;
        
        // in case of register:
        // the src register
        int reg_src;
        // the dst register
        int dst_reg;
    }
} code;


typedef struct data
{

    int value;
} data;

data Datas[MAX_DATA];
code Codes[MAX_CODE];


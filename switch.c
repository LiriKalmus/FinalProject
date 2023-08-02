if(opcode == OP_RTS || opcode == OP_STOP)
	{
		if(first_op_type == NO_OPERAND && second_op_type == NO_OPERAND)
		{
			encode_word(opcode,first_op_type,second_op_type, one_operand, bits);
			/**insert to table*/
			/*IC++*/
		}
		else{
			/*ERROR*/
			printf("the opcode not match the operands\n");
			return FALSE;
		}
	}

	if(opcode == OP_NOT || opcode == OP_CLR || opcode == OP_INC || opcode == OP_DEC || 
				opcode == OP_JMP || opcode == OP_BNE || opcode == OP_RED || opcode == OP_JSR)
	{
		if(first_op_type != NO_OPERAND && first_op_type != NUMBER && second_op_type == NO_OPERAND)
		{
			one_operand = TRUE;
			/**insert to table*/
				/*IC+2*/
		}
		else{
			/*ERROR*/
			printf("the opcode not match the operands\n");
			return FALSE;
		}
	}
	if(opcode == OP_PRN){

		if(first_op_type != NO_OPERAND && second_op_type == NO_OPERAND)
		{
			one_operand = TRUE;
			/**insert to table*/
				/*IC+2*/
		}
		else{
			/*ERROR*/
			printf("the opcode not match the operands\n");
			return FALSE;
		}
	}
	

	if(opcode == OP_MOV  || opcode == OP_ADD || opcode == OP_SUB){
	{
		if(first_op_type != NO_OPERAND && second_op_type != NO_OPERAND && second_op_type != NUMBER)
		{
			/**insert to table*/
			/*IC+3 or + 2*/ 
		}
		else{
			/*ERROR*/
			printf("the opcode not match the operands\n");
			return FALSE;
		}
	}
	if(opcode == OP_CMP)
	{
		if(first_op_type != NO_OPERAND && second_op_type != NO_OPERAND)
		{
			/**insert to table*/
			/*IC+3 or + 2*/ 
		}
		else{
			/*ERROR*/
			printf("the opcode not match the operands\n");
			return FALSE;
		}
	}
	if(opcode == OP_LEA)
	{
		if(first_op_type == LABEL && second_op_type != NO_OPERAND && second_op_type != NUMBER)
		{
			/**insert to table*/
			/*IC+3*/ 
		}
		else{
			/*ERROR*/
			printf("the opcode not match the operands\n");
			return FALSE;
		}
	}
	else{
		/*ERROR*/
		printf("the opcode is incorrect\n");
		return FALSE;
	}


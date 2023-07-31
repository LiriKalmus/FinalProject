

void first_pass(char *string)
{
    char name_command[MAX];
    char operand1[MAX];
    char operand2[MAX];

    command *ptr = search_command(name_command);

    if (ptr not found)
    {
        // error
    }

    if (addressing_type(operand1) & ptr->src_addressing == 0)
    {
        // error
    }

    // calculating the code table memory size
    if (addressing_type(operand1) == NO_OPERAND && addressing_type(operand2) == NO_OPERNAD)
    {
        IC++;
        return;
    }

    if (addressing_type(operand1) != NO_OPERAND && addressing_type(operand2) == NO_OPERNAD)
    {
        IC += 2;
        return;
    }

    if (addressing_type(operand1) == REGISTER && addressing_type(operand2) == REGISTER)
    {
        IC += 2;
        return;
    }

    IC += 3;
    return;
}

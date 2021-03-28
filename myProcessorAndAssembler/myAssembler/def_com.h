DEF_CMD(pushm, CMDFMT_RAM)
DEF_CMD(pushr, CMDFMT_REG)
DEF_CMD(popr, CMDFMT_REG)
DEF_CMD(popm, CMDFMT_RAM)
DEF_CMD(push, CMDFMT_NUM)
DEF_CMD(pop, CMDFMT_NOP)
DEF_CMD(in, CMDFMT_NOP)
DEF_CMD(out, CMDFMT_NOP)
DEF_CMD(add, CMDFMT_NOP)
DEF_CMD(mul, CMDFMT_NOP)
DEF_CMD(div, CMDFMT_NOP)
DEF_CMD(sub, CMDFMT_NOP)
DEF_CMD(end, CMDFMT_NOP)
DEF_CMD(sqrt, CMDFMT_NOP)
DEF_CMD(jmp, CMDFMT_LABEL)
DEF_CMD(jae, CMDFMT_LABEL)
DEF_CMD(ja, CMDFMT_LABEL)
DEF_CMD(jbe, CMDFMT_LABEL)
DEF_CMD(jb, CMDFMT_LABEL)
DEF_CMD(je, CMDFMT_LABEL)
DEF_CMD(jne, CMDFMT_LABEL)
DEF_CMD(call, CMDFMT_LABEL)
DEF_CMD(ret, CMDFMT_NOP)
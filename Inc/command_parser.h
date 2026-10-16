#ifndef COMMAND_PARSER_H
#define COMMAND_PARSER_H

void CommandParser_Init(void);
void CommandParser_Update(char received);
void CommandParser_NotifyDataLoss(void);

#endif

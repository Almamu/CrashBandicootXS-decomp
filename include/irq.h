#ifndef __IRQ_H__
#define __IRQ_H__

typedef void (*irq_handler_t)();

void EnableVBlankHandler(void);
s32 AddVBlankCallback(s32);

#endif /* __IRQ_H__ */
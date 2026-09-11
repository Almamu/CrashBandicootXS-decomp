#ifndef __IRQ_H__
#define __IRQ_H__

typedef void (*irq_handler_t)();

void sub_8000620(void);
s32 sub_8000680(s32);

#endif /* __IRQ_H__ */
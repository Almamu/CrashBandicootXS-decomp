#ifndef __IRQ_H__
#define __IRQ_H__

typedef void (*irq_handler_t)();

void sub_8000620(void);

#endif /* __IRQ_H__ */
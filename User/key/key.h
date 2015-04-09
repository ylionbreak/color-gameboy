#ifndef __LED_H
#define	__LED_H
#include "stm32f10x.h" 
/** the macro definition to trigger the led on or off 
  * 1 - off
  *0 - on
  */
#define ON  0
#define OFF 1
void KEY_GPIO_Config(void);
unsigned char FourDirection (void);
unsigned char Keyscan_Down  (void);
unsigned char Keyscan_Up    (void);
unsigned char Keyscan_Right (void);
unsigned char Keyscan_Left  (void);
unsigned char Keyscan_Enter (void);
unsigned char Keyscan_Esc   (void);
unsigned char Keyscan_Select(void);
unsigned char Keyscan_A (void);
unsigned char Keyscan_B   (void);
unsigned char Snake_Down  (void);
unsigned char Snake_Up    (void);
unsigned char Snake_Right (void);
unsigned char Snake_Left  (void);
#endif /* __LED_H */

#include "IIC_stuff/includesNdefines.h"
#include "IIC_stuff/oled_iic_controll.h"
#include "General Graphics/NeatGrapics.h"
#include "General Graphics/Renders.h"
#include "Font/Font_work.h"
#include "tetris/tetris_Game.h"
#include <avr/interrupt.h>
#include <util/delay.h>

int Fframe = 1;
						//IsPressed| TimeFromPressedPassed
int buttons[3] = {0};	//0		   | 000 0000

int toStartGame = 0;

ISR(TIMER1_COMPA_vect)
{
	TCNT1 = 0;
	Fframe = 1;

	PORTA ^= 2;
}

ISR(TIMER0_OVF_vect)
{
	TCNT0 = 0;
	for(int i = 0; i< 3; i++)
	{
		if(PINB & (1<<i))
		{
			buttons[i]++;
		}
		else
		{
			buttons[i] = 0; //Reset IsPressed
		}

		if((buttons[i]&0x3F) > CntToPressed)
		{
			buttons[i] |= (1<<IsPressed);
		}
	}
}

int main(void)
{
	DDRA = 0xFF;
	PORTA = 0;

	DDRB &= 0x1F;
	PORTA &= 0x1F;

	TCCR1B |= (1<<CS12);//clk/256
	TIMSK |= (1<<OCIE1A);
	OCR1A = 16000/(BLOCK_SIZE*2);

	TCCR0 |= (1<<CS00)|(1<<CS02);
	TIMSK |= (1<<TOIE0);

	OLED_INIT();
	tetris_Game t = tetris_Game();

	sei();
	while (1)
	{
		if(Fframe)
		{
			OLED_CLS();

			Fframe = 0;

			t.GameIteration(buttons);

			if(t.GameOver)
			{
				Fframe = 1;

				int disp = t.score;

				OLED_CLS();
				Font_work::DisplayNumber(pos(0,0), disp);
				OLED_RENDER();

				_delay_ms(1000);

				t = tetris_Game();
			}

			OLED_RENDER();
		}

		PORTA ^= 1;
	}
}


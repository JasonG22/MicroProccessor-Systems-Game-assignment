#include <stm32f031x6.h>
#include "display.h"
#include "musical_notes.h"

// Function signatures
void initClock(void);
void initSysTick(void);
void SysTick_Handler(void);
void delay(volatile uint32_t dly);
void setupIO();
int isInside(uint16_t x1, uint16_t y1, uint16_t w, uint16_t h, uint16_t px, uint16_t py);
void enablePullUp(GPIO_TypeDef *Port, uint32_t BitNumber);
void pinMode(GPIO_TypeDef *Port, uint32_t BitNumber, uint32_t Mode);

// Global Variable
volatile uint32_t milliseconds;

// Arrays for sprite data
const uint16_t deco1[]=
{
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,40224,40224,0,0,0,0,0,0,0,0,0,0,20156,20156,0,0,0,0,0,0,0,0,0,0,20156,20156,0,0,0,0,0,0,0,0,40224,0,20156,20156,0,40224,0,0,0,0,0,0,20156,0,20156,20156,0,20156,0,0,0,40224,0,0,20156,0,20156,20156,0,20156,0,0,40224,20156,0,0,20156,0,20156,20156,0,20156,0,0,20156,20156,0,0,20156,20156,40224,40224,20156,20156,0,0,20156,20156,20156,20156,20156,40224,20156,20156,40224,20156,20156,20156,20156,20156,20156,20156,20156,20156,20156,20156,20156,20156,20156,20156,20156,0,0,0,0,0,40224,40224,0,0,0,0,0,0,0,0,0,40224,20156,20156,40224,0,0,0,0,
};
const uint16_t deco2[]= 
	{
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,20156,20156,0,0,0,0,0,0,0,0,0,0,20156,20156,0,0,0,0,0,0,0,0,0,0,20156,20156,0,0,0,0,0,0,0,0,20156,0,20156,20156,0,20156,0,0,0,0,0,0,20156,0,20156,20156,0,20156,0,0,0,20156,0,0,20156,0,20156,20156,0,20156,0,0,20156,20156,0,0,20156,0,20156,20156,0,20156,0,0,20156,20156,0,0,20156,20156,20156,20156,20156,20156,0,0,20156,20156,20156,20156,20156,20156,20156,20156,20156,20156,20156,20156,20156,20156,20156,20156,20156,20156,20156,20156,20156,20156,20156,20156,20156,0,0,0,0,0,20156,20156,0,0,0,0,0,0,0,0,0,20156,0,0,20156,0,0,0,0,
	};
const uint16_t deco3[]= 
{
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,40224,40224,0,0,0,0,0,0,0,0,0,0,20156,20156,0,0,0,0,0,0,0,0,0,0,20156,20156,0,0,0,0,0,0,0,0,40224,0,20156,20156,0,40224,0,0,0,0,0,0,20156,0,20156,20156,0,20156,0,0,0,40224,0,0,20156,0,20156,20156,0,20156,0,0,40224,20156,0,0,20156,0,20156,20156,0,20156,0,0,20156,20156,0,0,20156,20156,40224,40224,20156,20156,0,0,20156,20156,20156,20156,20156,40224,20156,20156,40224,20156,20156,20156,20156,20156,20156,20156,20156,20156,20156,20156,20156,20156,20156,20156,20156,0,0,0,0,0,40224,40224,0,0,0,0,0,0,0,0,0,40224,20156,20156,40224,0,0,0,0,
};
const uint16_t deco4[]= 
{
13474,0,0,0,0,0,0,13474,0,13474,0,0,0,0,13474,0,0,0,13474,0,0,13474,0,0,0,13474,13474,13474,13474,13474,13474,0,13474,13474,0,13474,13474,0,13474,13474,13474,36465,13474,13474,13474,13474,36465,13474,13474,13474,13474,0,0,13474,13474,13474,13474,13474,0,13474,13474,0,13474,13474,0,13474,0,0,0,0,13474,0,0,0,13474,13474,13474,13474,0,0,
};
const uint16_t projectile[]=
{
	65535,65535,65535,65535,
};

const uint16_t dg1[]=
{
	0,0,16142,16142,16142,16142,16142,16142,16142,16142,0,0,0,0,0,16142,16142,16142,16142,16142,16142,0,0,0,0,0,16142,16142,16142,16142,16142,16142,16142,16142,0,0,0,0,16142,16142,16142,1994,1994,16142,16142,16142,0,0,0,0,16142,16142,16142,1994,16142,1994,16142,16142,0,0,0,0,16142,16142,16142,1994,16142,1994,16142,16142,0,0,0,0,16142,16142,16142,1994,16142,1994,16142,16142,0,0,0,0,16142,16142,16142,1994,1994,16142,16142,16142,0,0,0,0,16142,16142,16142,16142,16142,16142,16142,16142,0,0,0,0,16142,16142,16142,1994,1994,1994,16142,16142,0,0,0,0,16142,16142,16142,1994,16142,16142,16142,16142,0,0,0,0,16142,16142,16142,1994,16142,16142,16142,16142,0,0,0,0,16142,16142,16142,1994,16142,1994,16142,16142,0,0,0,0,16142,16142,16142,1994,1994,1994,16142,16142,0,0,0,0,0,16142,16142,16142,16142,16142,16142,0,0,0,0,0,0,16142,16142,16142,16142,16142,16142,0,0,0,
};

// Main Function
int main()
{
	// Initialize Variables
	int hinverted = 0;
	int vinverted = 0;
	int toggle = 0;
	int hmoved = 0;
	int vmoved = 0;
	uint16_t x = 50;
	uint16_t y = 140;
	uint16_t projectile_x = x+6;
	uint16_t projectile_y = y;
	uint16_t oldx = x;
	uint16_t oldy = y;
	uint16_t deco4_x = 10;
	uint16_t deco4_y = 15;
	uint16_t deco4_width = 8;
	uint16_t deco4_height = 11;
	uint16_t screen_width = 128;
	uint16_t screen_height = 160;
	uint16_t points = 0;
	uint16_t deco4_oldx = deco4_x;
	uint16_t deco4_oldy = deco4_y;
	uint16_t deco1_width = 12;
	uint16_t deco1_height = 16;
	uint16_t lives = 3;
	uint16_t run = 0;
	uint16_t highscore =0;
	uint16_t delay1 = 50;
	uint16_t difficulty_text = 1;
	
	initClock();
	initSysTick();
	setupIO();

	//main loop / main menu
	while(1)
	{

		//all the main menu code
		if (run == 0){

			//printing text on the main menu 
			printTextX2("Space", 30, 10, RGBToWord(000,000,200), 0);
			printTextX2("Shooter", 20, 30, RGBToWord(000,000,200), 0);

			printText("Press UP ",33,60,RGBToWord(200,200,200), 0);
			printText("to start",33,70,RGBToWord(200,200,200), 0);

			printText("Press Down",30,125,RGBToWord(250,000,250),0);
			printText("for credits",25,135,RGBToWord(250,000,250),0);
			
			printText("Left & Right to",10,90,RGBToWord(200,200,200),0);
			printText("change dificulty",10,100,RGBToWord(200,200,200),0);

			//printing weather the dificulty is easy or hard
			if (difficulty_text ==1)
			{
				printText("easy",50,110,RGBToWord(0,200,0),0);
			}
			else if (difficulty_text == 2)
			{
				printText("hard",50,110,RGBToWord(200,0,0),0);
			}
			
			//selects the dificulty
			if ((GPIOB->IDR & (1 << 5))==0){//left pressed
				delay1 = 50;
				difficulty_text = 1;
			}
			else if ((GPIOB->IDR & (1 << 4))==0){//right pressed
				delay1 = 25;
				difficulty_text = 2;
			}

			printText("HI-SCORE:",15,150,RGBToWord(200,200,200),0);
			printNumber(highscore ,80, 150, RGBToWord(200,200,200), 0);

			if ( (GPIOA->IDR & (1 << 8)) == 0 ) // up pressed
			{		
				fillRectangle(0, 0, screen_width, screen_height, RGBToWord(0, 0, 0)); // Fill screen with black
				run = 1;
				lives =3;
				points = 0;
			}

			if ( (GPIOA->IDR & (1 << 11)) == 0) // down pressed
			{
				fillRectangle(0, 0, screen_width, screen_height, RGBToWord(0, 0, 0)); // Fill screen with black
				run = 2;
				delay(300);

			}//end if
			
		}

		//code for credits screen
		while (run == 2)
		{
			
			printTextX2("Credits",25,30,RGBToWord(200,200,200), 0);

			printText("Jason G",15,60,RGBToWord(200,000,000), 0);
			printText("Sean B",15,80,RGBToWord(0,0,200), 0);
			printText("Uti A",15,100,RGBToWord(250,300,000), 0);

			printText("Press down to exit",1,150,RGBToWord(200,200,200), 0);
			

			if ( (GPIOA->IDR & (1 << 11)) == 0) // down pressed
			{
				fillRectangle(0, 0, screen_width, screen_height, RGBToWord(0, 0, 0)); // Fill screen with black
				run = 0;
				delay(300);

			}//end if
		}//end while
		

		//Game Loop
		while(run != 0)
		{

			// Clear the previous position of the projectile
			fillRectangle(projectile_x, projectile_y, 2, 2, 0);

			// Update projectile position
			projectile_y -= 5;
			

			// Check if the projectile has moved out of bounds
			if (projectile_y < 1) 
			{
				// If the projectile has moved out of bounds, reset its position
				projectile_x = x + 6; // Set x position of the projectile to the current x position of the ship
				projectile_y = y;     // Set y position of the projectile to the current y position of the ship
			}
			else 
			{
				// If the projectile is still within bounds, redraw it at the new position
				putImage(projectile_x, projectile_y, 2, 2, projectile, 0, 0);
				
			}//end else

			// Check for collision with deco4
			if (isInside(deco4_x, deco4_y, deco4_width, deco4_height, projectile_x, projectile_y))
			{
					// Collision detected
					// Delete the current "deco" sprite
					fillRectangle(deco4_x, deco4_y, deco4_width, deco4_height, 0);
					
					// Increment point tally-
					points++;

					// Generate new random coordinates for the "deco" sprite
					deco4_x = 10 + rand(10) % (screen_width - deco4_width- 20); // Generate random x-coordinate
					deco4_y = 15;

					// Draw the new "deco" sprite at the new coordinates
					putImage(deco4_x, deco4_y, deco4_width, deco4_height, deco4, 0, 0);
			}//end if

		
			void fillRectangle( uint16_t x, uint16_t y,uint16_t width, uint16_t height, uint16_t colour);
			
			hmoved = vmoved = 0;
			hinverted = vinverted = 0;
			if ((GPIOB->IDR & (1 << 4))==0) // right pressed
			{					
				if (x < 110)
				{
					x = x + 4;
					hmoved = 1;
					hinverted=0;
					
				}//end nested if					
			}//end if
			if ((GPIOB->IDR & (1 << 5))==0) // left pressed
			{			
				
				if (x > 10)
				{
					x = x - 4;
					hmoved = 1;
					hinverted=1;
					
				}//end nested if		
			}//end if
			
			
			if (deco4_y < 161)// monster fall
			{

				deco4_y = deco4_y + 2;
				vmoved = 1;
				vinverted = 0;
			}//end if
			else{
				deco4_y = 15;
			}//end else


			//collision of deco4 to deco1
			if (isInside(deco4_x,deco4_y,deco4_width,deco4_height,x,y) || isInside(deco4_x,deco4_y,deco4_width,deco4_height,x+12,y) || isInside(deco4_x,deco4_y,deco4_width,deco4_height,x,y+16) || isInside(deco4_x,deco4_y,deco4_width,deco4_height,x+12,y+16) )
			{
				x = 50;
				y = 140;

				deco4_x = 10 + rand() % (screen_width - deco4_width- 20); // Generate random x-coordinate
				deco4_y = 15;

				fillRectangle(oldx,oldy,12,16,0);
				oldx = x;
				oldy = y;	

				fillRectangle(deco4_oldx,deco4_oldy-2,8,15,0);
				deco4_oldx = deco4_x;
				deco4_oldy = deco4_y;
				lives--;
			}//end if
			
			//collision of deco1 to deco4
			if (isInside(x,y,deco1_width,deco1_height,deco4_x,deco4_y) || isInside(x,y,deco1_width,deco1_height,deco4_x+deco4_width,deco4_y) || isInside(x,y,deco1_width,deco1_height,deco4_x,deco4_y+deco4_height) || isInside(x,y,deco1_width,deco1_height,deco4_x+deco4_width,deco4_height) )
			{
				x = 50;
				y = 140;

				deco4_x = 10 + rand() % (screen_width - deco4_width- 20); // Generate random x-coordinate
				deco4_y = 15;

				fillRectangle(oldx,oldy,12,16,0);
				oldx = x;
				oldy = y;	

				fillRectangle(deco4_oldx,deco4_oldy-2,8,15,0);
				deco4_oldx = deco4_x;
				deco4_oldy = deco4_y;
				lives--;
			}//end if
			

			
			if ((vmoved) || (hmoved))
			{
				// only redraw if there has been some movement (reduces flicker)
				fillRectangle(oldx,oldy,12,16,0);
				oldx = x;
				oldy = y;		
				fillRectangle(deco4_oldx,deco4_oldy-2,8,15,0);
				deco4_oldx = deco4_x;
				deco4_oldy = deco4_y;	
				putImage(deco4_x, deco4_y, deco4_width, deco4_height, deco4,0, 0);	
				if (hmoved)
				{
					if (toggle){
						putImage(x,y,12,16,deco1,hinverted,0);
					}//end nested if
					else{
						putImage(x,y,12,16,deco2,hinverted,0);
					}//end nested else
					
					toggle = toggle ^ 1;
				}//end if
				else
				{
					putImage(x,y,12,16,deco3,0,vinverted);
				}//end else

				
			}//end if	
			
			//Score tally 
		printText("Score:", 2, 2, RGBToWord(200,200,200), 0);
		printNumber(points,45, 2, RGBToWord(200,200,200), 0);
		
		//if statements for the lives
		if (lives == 3)//3 circles for 3 lives
		{
			fillCircle(110, 6, 3, RGBToWord(200,0,0));
			fillCircle(116, 6, 3, RGBToWord(200,0,0));
			fillCircle(122, 6, 3, RGBToWord(200,0,0));
		}//end if
		else if (lives == 2)//2 circles
		{
			fillCircle(110, 6, 3, RGBToWord(0,0,0));
			fillCircle(116, 6, 3, RGBToWord(200,0,0));
			fillCircle(122, 6, 3, RGBToWord(200,0,0));
		}//end elif
		else if (lives ==1)//1 circle
		{
			fillCircle(110, 6, 3, RGBToWord(0,0,0));
			fillCircle(116, 6, 3, RGBToWord(0,0,0));
			fillCircle(122, 6, 3, RGBToWord(200,0,0));
		}//end elif
		else{// no lives

			fillCircle(110, 6, 3, RGBToWord(0,0,0));
			fillCircle(116, 6, 3, RGBToWord(0,0,0));
			fillCircle(122, 6, 3, RGBToWord(0,0,0));

		}//end else


		//lives = 0;
		if (lives <= 0)
		{
			if(points>highscore)
			{
				highscore = points;
			}
			fillRectangle(0, 0, screen_width, screen_height, RGBToWord(0, 0, 0)); // Fill screen with black
			printTextX2("Game Over", 10, 60, RGBToWord(200,200,200), 0);
			printText("Main menu in ", 20, 80, RGBToWord(200,200,200), 0);
			printText("Score:", 2, 2, RGBToWord(200,200,200), 0);
			printNumber(points,45, 2, RGBToWord(200,200,200), 0);
			printText("3", 60, 100, RGBToWord(200,200,200), 0);
			delay(1000);
			printText("2", 60, 100, RGBToWord(200,200,200), 0);
			delay(1000);
			printText("1", 60, 100, RGBToWord(200,200,200), 0);
			delay(1000);
			fillRectangle(0, 0, screen_width, screen_height, RGBToWord(0, 0, 0)); // Fill screen with black
			delay(20);
			run = 0;
			break;
			
		
		}

		delay(delay1);

		}
		

	}//end menu while loop
	return  0;
	

}//end main


		
void initSysTick(void)
{
	SysTick->LOAD = 48000;
	SysTick->CTRL = 7;
	SysTick->VAL = 10;
	__asm(" cpsie i "); // enable interrupts
}
void SysTick_Handler(void)
{
	milliseconds++;
}
void initClock(void)
{
// This is potentially a dangerous function as it could
// result in a system with an invalid clock signal - result: a stuck system
        // Set the PLL up
        // First ensure PLL is disabled
        RCC->CR &= ~(1u<<24);
        while( (RCC->CR & (1 <<25))); // wait for PLL ready to be cleared
        
// Warning here: if system clock is greater than 24MHz then wait-state(s) need to be
// inserted into Flash memory interface
				
        FLASH->ACR |= (1 << 0);
        FLASH->ACR &=~((1u << 2) | (1u<<1));
        // Turn on FLASH prefetch buffer
        FLASH->ACR |= (1 << 4);
        // set PLL multiplier to 12 (yielding 48MHz)
        RCC->CFGR &= ~((1u<<21) | (1u<<20) | (1u<<19) | (1u<<18));
        RCC->CFGR |= ((1<<21) | (1<<19) ); 

        // Need to limit ADC clock to below 14MHz so will change ADC prescaler to 4
        RCC->CFGR |= (1<<14);

        // and turn the PLL back on again
        RCC->CR |= (1<<24);        
        // set PLL as system clock source 
        RCC->CFGR |= (1<<1);
}
void delay(volatile uint32_t dly)
{
	uint32_t end_time = dly + milliseconds;
	while(milliseconds != end_time)
		__asm(" wfi "); // sleep
}

void enablePullUp(GPIO_TypeDef *Port, uint32_t BitNumber)
{
	Port->PUPDR = Port->PUPDR &~(3u << BitNumber*2); // clear pull-up resistor bits
	Port->PUPDR = Port->PUPDR | (1u << BitNumber*2); // set pull-up bit
}
void pinMode(GPIO_TypeDef *Port, uint32_t BitNumber, uint32_t Mode)
{
	/*
	*/
	uint32_t mode_value = Port->MODER;
	Mode = Mode << (2 * BitNumber);
	mode_value = mode_value & ~(3u << (BitNumber * 2));
	mode_value = mode_value | Mode;
	Port->MODER = mode_value;
}
int isInside(uint16_t x1, uint16_t y1, uint16_t w, uint16_t h, uint16_t px, uint16_t py)
{
	// checks to see if point px,py is within the rectange defined by x,y,w,h
	uint16_t x2,y2;
	x2 = x1+w;
	y2 = y1+h;
	int rvalue = 0;
	if ( (px >= x1) && (px <= x2))
	{
		// ok, x constraint met
		if ( (py >= y1) && (py <= y2))
			rvalue = 1;
	}
	return rvalue;
}

void setupIO()
{
	RCC->AHBENR |= (1 << 18) + (1 << 17); // enable Ports A and B
	display_begin();
	pinMode(GPIOB,4,0);
	pinMode(GPIOB,5,0);
	pinMode(GPIOA,8,0);
	pinMode(GPIOA,11,0);
	enablePullUp(GPIOB,4);
	enablePullUp(GPIOB,5);
	enablePullUp(GPIOA,11);
	enablePullUp(GPIOA,8);
}
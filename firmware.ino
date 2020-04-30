#include <LiquidCrystal.h>
#include <EEPROM.h>
#include "students.h"
#include "sum.h"

LiquidCrystal lcd( 8, 9, 4, 5, 6, 7 );

int lcd_key     = 0;
int adc_key_in  = 0;

int flag        = 1;
int index       = 0;
int value       = 0;

int id_votation = 0;
int id_user     = 0;

#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

int read_buttons()
{
    adc_key_in = analogRead( 0 );

    if( adc_key_in > 1000 )
    {
        return btnNONE;
    }

    if( adc_key_in < 50 )
    {
        return btnRIGHT;
    }

    if( adc_key_in < 250 )
    {
        return btnUP;
    }

    if( adc_key_in < 450 )
    {
        return btnDOWN;
    }

    if( adc_key_in < 650 )
    {
        return btnLEFT;
    }

    if( adc_key_in < 850 )
    {
        return btnSELECT;
    }

    return btnNONE;
}

void keyboard()
{
   
    lcd_key = read_buttons();
        
    if( lcd_key == btnDOWN )
    {
        if( index >= 0 && flag == 1 || flag == 2 )
        {
            screen_increment();
        }
    }

    else if( lcd_key == btnUP )
    {
        
		if( index < 31 && flag == 1 || flag == 2 )
        {
           screen_decrement();
        }
    }

    else if( lcd_key == btnLEFT )
    {
        index = 0;
        flag  = 1;

        if( flag == 1 )
        {
           lcd_format();
        }
		
  		else if( flag == 3 )
  		{
  			clearEEPROM();
  		}
    }

    else if( lcd_key == btnRIGHT )
    {
        if( flag == 1 )
        {
            screen_super();
        }

        else if( flag == 3 )
        {
            screen_main();
        }

        else if( flag == 4 )
        {
            screen_cancel();
        }
    }

    else if( lcd_key == btnSELECT )
    {
        if( flag == 1 )
        {
            if( sum[index][1] == 1 )
            {
                screen_validate_user();
            }

            else
            {
                id_user = index;
                index   = 0;
                screen_votation_open();
            }
        }

        else if( flag == 2 )
        {
            id_votation = index - 1;
            index       = 0;
            screen_confirm();
        }

        else if( flag == 4 )
        {
            int c = sum[id_votation][0];
            c++;
            sum[id_votation][0] = c;
            sum[id_user][1] = 1;

            EEPROM.write( id_votation * 2, c );
            EEPROM.write( id_votation * 2 + 1, 1 );
            
            index = 0;            
            screen_register();
        }
    }
}

void screen_init()
{
    flag = 1;
    lcd.begin( 16, 2 );
    lcd.clear();
    lcd.setCursor( 0, 0 );
    lcd.print( "SISTEMA VOTACAO" );
    lcd.setCursor( 0, 1 );
    lcd.print( "ENTRE SEU CODIGO" );
}

void screen_increment()
{
    int cont = index++;
    
    lcd.clear();
    lcd.setCursor( 0, 1 );
    lcd.print( students[cont][0] );
    delay(500);
}

void screen_decrement()
{
    int desc = index--;
    
    lcd.clear();
    lcd.setCursor( 0, 1 );
    lcd.println( students[desc][0] ); 
    delay(500);
}

void screen_validate()
{
    flag = 1;
    lcd.clear();
    lcd.setCursor( 0, 0 );
    lcd.print( "CODIGO INVALIDO" );
    lcd.setCursor( 0, 1 );
    lcd.print( "VOTACAO FECHADA" );
    delay( 5000 );
}

void screen_validate_user()
{
    lcd.clear();
    lcd.setCursor( 0, 0 );
    lcd.print( "USUARIO JA VOTOU" );
    lcd.setCursor( 0, 1 );
    lcd.print( "VOTACAO FECHADA" );
    delay( 5000 );
    flag = 1;
}

void screen_votation_open()
{
    flag = 2;
    lcd.clear();
    lcd.setCursor( 0, 0 );
    lcd.print( "VOTACAO ABERTA" );
    lcd.setCursor( 0, 1 );
    delay(500);
}

void screen_super()
{
    flag = 3;
    lcd.clear();
    lcd.setCursor( 0, 0 );
    lcd.print( "  ADMINSTRADOR" );
    lcd.setCursor( 0, 1 );
    lcd.print( "<ZERA  APURACAO>" );
    delay(500);
}

void screen_confirm()
{
    flag = 4;
    lcd.clear();
    lcd.setCursor( 0, 0 );
    lcd.print( students[id_votation][1] );
    lcd.setCursor( 0, 1 );
    lcd.print( "<CONFIRM CANCEL>" );
    delay(500);
	  interrupts();
}

void screen_cancel()
{
    flag = 1;
    lcd.clear();
    lcd.setCursor( 0, 0 );
    lcd.print( "VOTO CANCELADO" );
    lcd.setCursor( 0, 1 );
    delay( 5000 );
    screen_init();
}

void screen_register()
{
    flag = 1;
    lcd.clear();
    lcd.setCursor( 0, 0 );
    lcd.print( "VOTO REGISTRADO" );
    lcd.setCursor( 0, 1 );
    delay( 5000 );
    screen_init();
}

void screen_time()
{
    lcd.clear();
    lcd.setCursor( 0, 0 );
    lcd.print( "TEMPO EXCEDIDO" );
    lcd.setCursor( 0, 1 );
    lcd.print( "VOTACAO FECHADA" );
    delay( 5000 );
}

void lcd_format()
{
    lcd.clear();
    lcd.setCursor( 0, 0 );
    clearEEPROM();
    lcd.print( "VOTACAO ZERADA" );
    delay(500);
}

void screen_main()
{
    int super = 0;
    int root = 0;

    for( value = 0; value < 32; value++ )
    {
        if( sum[super][0] < sum[value][0] )
        {
            super = value;
        }
    }

    for( value = 0; value < 32; value++ )
    {
        if( sum[root][0] < sum[value][0] && value != super )
        {
            root = value;
        }
    }

    lcd.clear();
    lcd.setCursor( 0, 0 );
    lcd.print( "REPRESENT:" );
    lcd.print( students[super][0] );

    lcd.setCursor( 0, 1 );
    lcd.print( "VICE-REPR:" );
    lcd.print( students[root][0] );
        
    delay(500);
}

void load()
{
  int j = 0;
  for (j = 0; j < 32; j++) 
  {
    sum[j][0] = EEPROM.read( j * 2);
    if(sum[j][0] == 0xff)
    { 
      sum[j][0] = 0;
      EEPROM.write(j*2,0);
    }
    
    sum[j][1] = EEPROM.read( j * 2 + 1);
    if(sum[j][1] == 0xff)
    { 
      sum[j][1] = 0;
      EEPROM.write(j*2,0);
    }
  }
}

void clearEEPROM()
{
    for ( int i = 0 ; i < EEPROM.length() ; i++ )
    {
      EEPROM.write( i, 0 );
    }
}

void setup() 
{
    screen_init();
    load();
}

void loop() 
{
    keyboard();
}

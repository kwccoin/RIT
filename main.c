#include <stdio.h>
#include <locale.h>
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <time.h>
#include "formas.c"
#include "save.c"
#include "portas_e_utils"


int posix = 0,posiy = 0;
int tecla;
int *MEM_XY;
int *MEM_POSI;
int *MEMORIAFODA;
char leitura;
char SOLO_SALVO = '*';
int PORTAS[10][3];
char ITEM[5];
int ITEM_CONTADOR = 0;
int ITEM_ATUAL = 0;

void Teclado()
{
        mvprintw(posiy, posix,"@");
        move(posiy, posix) ;
        tecla = getch();

        switch(tecla)
        {
//--------------------------------------------------------------------------------
            
                case KEY_RIGHT:
                {
                	mvinch(posiy,posix+1);
                	leitura = inch();
                    if( posix >= MEM_XY[1]-1 && posiy <= MEM_XY[0]-1 && leitura != '#'&&leitura != '?')
                    {
                            mvaddch (posiy,posix,SOLO_SALVO);
                            posix = 0;
                            mvinch(posiy,posix);
                        	inch();
                        	SOLO_SALVO = inch() ;
                  	}
                	else
                    {	
                    	if (leitura == '?')
                		{
                			ABRIR_PORTA(posiy,posix,tecla);
                		}
                		
                        if(posix < MEM_XY[1]-1&& leitura != '#'&&leitura != '?')
                        {
                        	mvaddch (posiy,posix,SOLO_SALVO);
                        	posix = posix + 1;
                        	mvinch(posiy,posix);
                        	inch();
                        	SOLO_SALVO = inch();
                    	}
                    }
                move(posiy,posix);
                refresh();
                }
                break;

                case KEY_UP:
                {
                	mvinch(posiy-1,posix);
                	leitura = inch();
                	if (leitura == '?')
                	{
						ABRIR_PORTA(posiy,posix,tecla);
                	}
                    if(posiy >= 1&& leitura != '#'&& leitura != '?')
                    {
                        mvaddch (posiy,posix,SOLO_SALVO);
                        posiy = posiy - 1;
                        mvinch(posiy,posix);
                        inch();
                        SOLO_SALVO = inch();
                    }
                    move(posiy,posix);
                    refresh();
                }
                break;

                case KEY_LEFT:
                {
                	mvinch(posiy,posix-1);
                	leitura = inch();
                	if (leitura == '?')
                	{
                		ABRIR_PORTA(posiy,posix,tecla);
                	}
                    if (posix != 0||posiy!=0&& leitura != '#'&&leitura != '?'){
                        if( posix < 1&& leitura != '#')
                        {
                            	mvaddch (posiy,posix,SOLO_SALVO);
                            	posix = MEM_XY[1] -1;
                            	mvinch(posiy,posix);
                        		inch();
                        		SOLO_SALVO = inch() ;
                        }

                        else{
                            if(posix>0&& leitura != '#'&&leitura != '?')
                            {
                            	mvaddch (posiy,posix,SOLO_SALVO);
                            	posix = posix - 1;
                            	mvinch(posiy,posix);
                       		 	inch();
                        		SOLO_SALVO = inch() ;
                            }
                        }
                    move(posiy,posix);
                        }
                    refresh();
                }
                break;

                case KEY_DOWN:
                {
                	mvinch(posiy+1,posix);
                	leitura = inch();
                	if (leitura == '?')
                	{
                		ABRIR_PORTA(posiy,posix,tecla);
                	}
                    if (posiy < MEM_XY[0]-1&& leitura != '#'&&leitura != '?')
                    {
                    	mvaddch (posiy,posix,SOLO_SALVO);
                    	posiy = posiy+1;
                    	mvinch(posiy,posix);
                    	inch();
                    	SOLO_SALVO = inch();
                    }
                    move(posiy,posix);
                    refresh();
                }
                break;
                
//------------------------------------------------------------------------------------------
//NUMEROS------
//-----------------------------------------------------------------------------------------
                case '1':
                {
                  if(ITEM[0]!='0')
                  {
                    ITEM[0] = DROPAR_ITEM(posiy,posix,ITEM[0]);
                    if(ITEM[0] == '0')
                    {
                        ITEM_CONTADOR--;
                    }
                  }
                  else
                  {
                    
                  }
                }


//----------------------------------------------------------------------------------
//              FUN??OES
//-----------------------------------------------------------------------------------
                case  's':
                {
                    save(leitura,MEM_XY);
                }
                break;
                
                case 'c':
                {
                    
                   FECHAR_PORTA(posiy,posix,tecla);
                }
                break;
                
                 case 'g':
                {
                   ITEM[ITEM_CONTADOR] = PEGAR_ITEM(posiy,posix,tecla);
                   ITEM_CONTADOR++;
                }
                break;
        }
	}


// MAIN

int main()
{
    int RES_Y,RES_X;
    leitura = inch();
    MEM_XY = (int *) malloc(2 * sizeof(int));
    MEM_POSI = (int *) malloc(2 * sizeof(int));
    initscr();
    cbreak();
    noecho();
    keypad(stdscr,true);
    raw();
    curs_set(0);
    getmaxyx(stdscr,MEM_XY[0], MEM_XY[1]);
    resizeterm(MEM_XY[0], MEM_XY[1]);
    
  //PRINTA A GRAMA
	GERAR_GRAMA(MEM_XY);
    
    // RODA O SCRIPT DE GERAR O MAPA
    GERAR_MAPA(MEM_XY[0],MEM_XY[1],posiy,posix,MEM_XY,MEM_POSI);
    GERAR_ARMA(MEM_XY,leitura);
   
   //POSICIONA PERSONAGEM E ENUMERA AS PORTAS(ALEM DE DISTRIBUIR E ALOCAR) 
   	int perso = 0;
   	int num_porta = 0;
    for(int y = 0;y<MEM_XY[0];y++)
        {
        	for(int x = 0;x<MEM_XY[1];x++)
       	  {
            mvinch(y,x);
            inch();
            leitura = inch();
            if(leitura == '*'&&perso == 0)
            {
            	posiy = y;
            	posix = x;
            	move(y,x);
				perso++;
            }
            else
            {
            	if(leitura == '?')
            	{
            		DEFINIR_PORTA(y,x,num_porta);
            		num_porta++;
            	}
            }
            
    	   }
    	}


	// LOOP DO TECLADO

    while(tecla != 'q')
    {
        mvprintw(posiy, posix,"@");
        move(posiy, posix);

        if(MEM_XY[0] != RES_Y||MEM_XY[1] != RES_X)
        {
            getmaxyx(stdscr,MEM_XY[0], MEM_XY[1]);
            resizeterm(MEM_XY[0], MEM_XY[1]);
            RES_Y= MEM_XY[0];
            RES_X= MEM_XY[1];
        }
         
        else
        {
       		RES_Y= MEM_XY[0];
   			RES_X= MEM_XY[1];
            Teclado();
        }
        MEM_POSI[0] = posix;
        MEM_POSI[1] = posiy;
    }
    free(MEM_XY);
    free(MEM_POSI);
    MEM_POSI= NULL;
    MEM_XY = NULL;
    endwin();
}

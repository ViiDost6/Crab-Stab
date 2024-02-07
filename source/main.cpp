//Librerías----------------------------------------------------------------------
#include <nds.h>
#include <stdio.h>
#include <nf_lib.h>
#include <time.h>
#include <filesystem.h>
#include <string>
//-------------------------------------------------------------------------------

//Variables-----------------------------------------------------------------------
touchPosition touch;                    //Declara el ojeto de eventos de toque
#define TIMER_SPEED (BUS_CLOCK/1024)    //Temporizador
int min = 30;                           //Variable de tiempo
//--------------------------------------------------------------------------------

//Clases implementadas------------------------------------------------------------
#include "GameState.h"
#include "Action.h"
#include "ForwardModel.h"
//--------------------------------------------------------------------------------

void InitBuffers() //-----------------------------------------------------------------------------
{
    //Inicia el modo 2D de las dos pantallas
    NF_Set2D(0, 5);
    NF_Set2D(1, 5);

    //Inicia el acceso rápido a la carpeta raíz definida
    nitroFSInit(NULL);
    NF_SetRootFolder("NITROFS");

    //Buffers de los fondos teselados
    NF_InitTiledBgBuffers();
    NF_InitTiledBgSys(0);   //pantalla superior
    NF_InitTiledBgSys(1);   //pantalla inferior

    //Buffers de los sprites
    NF_InitSpriteBuffers();
    NF_InitSpriteSys(0);
    NF_InitSpriteSys(1);

    //Buffers de texto
    NF_InitTextSys(0);
    NF_InitTextSys(1);
    NF_LoadTextFont("fnt/default", "normal", 256, 256, 0);

    // Inicializa los fondos en modo "BITMAP"
	NF_InitBitmapBgSys(0, 1);
	NF_InitBitmapBgSys(1, 1);

	// Inicializa los buffers para guardar fondos en formato BITMAP
	NF_Init16bitsBgBuffers();

	// Inicializa el BackBuffer de 16 bits
	NF_Init16bitsBackBuffer(0);
	NF_Init16bitsBackBuffer(1);

	// Habilita el backbuffer en las dos pantallas
	NF_Enable16bitsBackBuffer(0);
	NF_Enable16bitsBackBuffer(1);
}

void Update(GameState& gs, ForwardModel& fm) //----------------------------------------------------------------------
{
    scanKeys();
	int held = keysHeld();
    int down = keysDown();
    Action action;

	if(down & KEY_START)    {action.SetStart();}

	if(held & KEY_TOUCH) 
    {
        touchRead(&touch);
        action.SetTouch();
        gs.stylus_x = touch.px;
        gs.stylus_y = touch.py;
	}

    fm.ChangeLvl(gs, action);
    fm.MarceloMoves(gs, action);

    //Movimiento de los sprites en el primer nivel de gamestate
    if (gs.lvl == 2)
    {
        //Funciones de puntuación
        fm.Flexing(gs, action);

        //Cuerpo de marcelo
        NF_MoveSprite(1, 21, gs.marcelo_x, gs.marcelo_y);
        NF_MoveSprite(1, 22, gs.marcelo_x + 64, gs.marcelo_y);

        //Brazos
        NF_MoveSprite(1, 19, gs.marcelo_x - 32, gs.marcelo_y - 36);
        NF_MoveSprite(1, 20, gs.marcelo_x + 96, gs.marcelo_y - 36);

        //Nodos
        NF_MoveSprite(1, 23, gs.marcelo_x - 24, gs.marcelo_y + 16);
        NF_MoveSprite(1, 24, gs.marcelo_x + 120, gs.marcelo_y + 16);

        NF_SpriteRotScale(1, 0, (59 - gs.marcelo_y) * 3, 256, 256);
        NF_SpriteRotScale(1, 1, -((59 - gs.marcelo_y) * 3), 256, 256);
    }

    if (gs.lvl == 3)
    {
        NF_MoveSprite(1, 0, gs.marcelo_x, gs.marcelo_y);
    }
    
    NF_SpriteOamSet(0);
    NF_SpriteOamSet(1);

    swiWaitForVBlank();

    NF_UpdateTextLayers();

    oamUpdate(&oamMain);
    oamUpdate(&oamSub);
}

void CinematicManager(GameState& gs) //-------------------------------------------------------------------------------------
{
    //Según nivel (cinemática), decide cúantos frames
    if (gs.lvl == 1)
    {
        for (int i = 1; i <= 212; i++)
        {
            //Cambia la imagen a mostrar en el modo backbuffer

            //Crea una cadena de formato para el nombre del archivo
            std::string filename = "bmp/Cinem1-" + std::to_string(i);

            //Carga RAW el bitmap en RAM
            NF_Load16bitsBg(filename.c_str(), 0);

            //Pasa la imagen al backbuffer
            NF_Copy16bitsBuffer(0, 1, 0);

            //Dibuja la imagen directamente en la pantalla seleccionada
            //Y lo pasa a la VRAM
            NF_Flip16bitsBackBuffer(0);

            //Al sobreescribir el mismo slot de VRAM, sólo se ocupa un puesto,
            //y se evitan sobrecargas
        }
        gs.lvl = 2;
    }
}

void Drawscene(GameState& gs) //------------------------------------------------------------------------------------
{
    if (gs.lvl == 0)
    {
        //Carga de los fondos teselados
        NF_LoadTiledBg("bg/Title", "titulo", 256, 256);
        NF_LoadTiledBg("bg/Menu", "menu", 256, 256);
        NF_LoadTiledBg("bg/Logo", "logo", 256, 256);

        NF_CreateTiledBg(0, 0, "logo");
        NF_CreateTiledBg(0, 1, "titulo");
        NF_CreateTiledBg(1, 1, "menu");

    }

    else if (gs.lvl == 1)
    {
        NF_LoadTiledBg("bg/Menu", "menu", 256, 256);
        NF_CreateTiledBg(1, 0, "menu");

        //se llama a la función de manejo de cinemáticas
        CinematicManager(gs);
    }

    else if (gs.lvl == 2)
    {
        //fondos en pantalla
        NF_LoadTiledBg("bg/Bg1_inferior", "inferior", 256, 256);
        NF_LoadTiledBg("bg/Bg1_superior", "superior", 256, 256);

        NF_CreateTiledBg(0, 1, "superior");
        NF_CreateTiledBg(1, 0, "inferior");

        //sprites invocados
        //Marcelo son dos sprites de 64x64 unidos y varios segmentos en modo cadena
        //A número más bajo cargado en la vram, más al frente se encontrará el sprite

        //Pinzas en el plano más cercano a cámara
        NF_LoadSpriteGfx("sprite/lvl1/pinza", 1, 32, 32);
        NF_LoadSpritePal("sprite/lvl1/pinza", 1);

        NF_VramSpriteGfx(1, 1, 1, false);
        NF_VramSpritePal(1, 1, 1);

        NF_CreateSprite(1, 1, 1, 1, gs.marcelo_x, gs.marcelo_y - 62);
        NF_CreateSprite(1, 2, 1, 1, gs.marcelo_x + 96, gs.marcelo_y - 62);

        //La barra sobre la cual se sostiene Marcelo
        NF_LoadSpriteGfx("sprite/lvl1/barra", 2, 16, 16);
        NF_LoadSpritePal("sprite/lvl1/barra", 2);

        NF_VramSpriteGfx(1, 2, 2, false);
        NF_VramSpritePal(1, 2, 2);

        for (int i = 0; i < 16; i++)
        {
            NF_CreateSprite(1, 3 + i, 2, 2, 16*i, 11);
        }
        //hay 16 teselas de barra, con lo cual 15 + 3 = 18 sprites (empieza en 0)

        //Brazos rotatorios de Marcelo
        NF_LoadSpriteGfx("sprite/lvl1/arm", 3, 64, 64);
        NF_LoadSpritePal("sprite/lvl1/arm", 3);

        NF_VramSpriteGfx(1, 3, 3, false);
        NF_VramSpritePal(1, 3, 3);

        NF_CreateSprite(1, 19, 3, 3, gs.marcelo_x -32, gs.marcelo_y - 36);
        NF_CreateSprite(1, 20, 3, 3, gs.marcelo_x + 96, gs.marcelo_y - 36);

        //Cuerpo de Marcelo, como mide 128x64, utiliza dos sprites dependientes de las mismas coordenadas
        NF_LoadSpriteGfx("sprite/lvl1/Marcelo_L", 4, 64, 64);
        NF_LoadSpritePal("sprite/lvl1/Marcelo_L", 4);

        NF_VramSpriteGfx(1, 4, 4, true);
        NF_VramSpritePal(1, 4, 4);

        NF_CreateSprite(1, 21, 4, 4, gs.marcelo_x, gs.marcelo_y);

        NF_LoadSpriteGfx("sprite/lvl1/Marcelo_R", 5, 64, 64);
        NF_LoadSpritePal("sprite/lvl1/Marcelo_R", 5);

        NF_VramSpriteGfx(1, 5, 5, true);
        NF_VramSpritePal(1, 5, 5);

        NF_CreateSprite(1, 22, 5, 5, gs.marcelo_x + 64, gs.marcelo_y);

        //Nodos de los hombros
        NF_LoadSpriteGfx("sprite/lvl1/nodes", 6, 32, 32);
        NF_LoadSpritePal("sprite/lvl1/nodes", 6);

        NF_VramSpriteGfx(1, 6, 6, false);
        NF_VramSpritePal(1, 6, 6);

        NF_CreateSprite(1, 23, 6, 6, gs.marcelo_x, gs.marcelo_y + 16);
        NF_CreateSprite(1, 24, 6, 6, gs.marcelo_x + 112, gs.marcelo_y +16);

        //Activa el uso de matrices de rotación para los sprites de los brazos
        NF_EnableSpriteRotScale(1, 19, 0, false);
        NF_EnableSpriteRotScale(1, 20, 1, false);
    }

    if (gs.lvl == 3)
    {
        //Según el récord, decide el resultado del juego
        if (gs.flex >= 100)
        {
            NF_LoadTiledBg("bg/goodending1", "sup", 256, 256);
            NF_LoadTiledBg("bg/goodending2", "inf", 256, 256);

            NF_CreateTiledBg(0, 0, "sup");
            NF_CreateTiledBg(1, 0, "inf");
        }
        else
        {
            NF_LoadTiledBg("bg/badending1", "sup", 256, 256);
            NF_LoadTiledBg("bg/badending2", "inf", 256, 256);

            NF_CreateTiledBg(0, 0, "sup");
            NF_CreateTiledBg(1, 0, "inf");
        }
    }
}

void TextMaker(GameState& gs) //------------------------------------------------------------------------
{
    if (gs.lvl == 0)
    {
        //Creación del texto
        NF_CreateTextLayer(1, 0, 0, "normal");
        NF_WriteText(1, 0, 11, 11, "Press Start");
    }

    if (gs.lvl == 2)
    {   
        NF_ClearTextLayer(1, 0);
        NF_CreateTextLayer(0, 0, 0, "normal");

        char text[32];
        snprintf(text, sizeof(text), "Segundos restantes: %d", min);

        char text2[32];
        snprintf(text2, sizeof(text2), "Flexiones: %d", gs.flex);

        NF_WriteText(0, 0, 4, 4, text);
        NF_WriteText(0, 0, 4, 8, text2);

    }
    NF_UpdateTextLayers();
}

void LevelClear(GameState& gs) //---------------------------------------------------------------
{
    if (gs.lvl == 1)
    {
        //Eliminación de los fondos teselados
        NF_DeleteTiledBg(0, 0);
        NF_DeleteTiledBg(0, 1);
        NF_DeleteTiledBg(1, 1);
    }

    if (gs.lvl == 2)
    {
        NF_Unload16bitsBg(0);
        NF_Reset16bitsBgBuffers();

    }
    if (gs.lvl == 3)
    {
        NF_DeleteTiledBg(1, 0);
        NF_DeleteTiledBg(0, 1);

        for (int i = 1; i <= 24; i++)
        {
            NF_DeleteSprite(1, i);
        }

        NF_ResetSpriteBuffers();
        NF_ClearTextLayer(0, 0);
    }
}

//---------------------------------------------------------------------------------
int main(void) {
//---------------------------------------------------------------------------------
	InitBuffers();
    GameState gs;
    ForwardModel fm;
    gs.Reset();

    uint ticks = 0;
	int seconds = 0;
	int next = 1;
    int frame = 0;
    

    Drawscene(gs);
    int prev_lvl = 0;

	while(1) 
	{
        ticks += timerElapsed(0);
		seconds = (int) (ticks/TIMER_SPEED);

        if (gs.lvl == 2)
        {
            //Maneja los eventos de tiempo en el segundo nivel
            timerStart(0, ClockDivider_1024, 0, NULL);

            if (seconds >= next)
            {
                next += 1;
                min -= 1;
            }

            if (min == 0)
            {
                timerStop(0);
                gs.lvl = 3;
            }
        }

        if (prev_lvl != gs.lvl)
        {
            LevelClear(gs);
            prev_lvl = gs.lvl;  //guarda el nivel como el nuevo previo
            Drawscene(gs);
        }

        TextMaker(gs);
        Update(gs, fm);
	}

	return 0;
}

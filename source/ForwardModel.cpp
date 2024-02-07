#include "ForwardModel.h"
ForwardModel::ForwardModel()
{}

void ForwardModel::ChangeLvl(GameState& gs, Action& action)
{
    //Maneja el cambio de escena siguiendo los puntos de control definidos para
    //pasar de un nivel a otro
    if (gs.lvl >= 0 && gs.lvl < 2)
    {
        if (action.IsStart())
        {
            gs.lvl += 1;
        }
    }
}

void ForwardModel::MarceloMoves(GameState& gs, Action& action)
{
    //La función se reutiliza para cambiar las comandas de movimiento según el
    //nivel actual en el que se encuentre el jugador, según gamestate
    if (gs.lvl == 2)
    {
        //Gravedad implementada en el nivel primero
        if (gs.marcelo_y < 118)
        {
            gs.marcelo_y += 3;
        }

        //Mecánicas de arrastre al toque
        if (action.IsTouch())
        {
            //Como el sprite de Marcelo es de 128x64, el área de toque del mismo se encontrará
            //entre estos intérvalos
            if ((gs.stylus_x >= gs.marcelo_x) && (gs.stylus_x <= gs.marcelo_x +128) &&
                (gs.stylus_y >= gs.marcelo_y) && (gs.stylus_y <= gs.marcelo_y + 64))
            {
                //coordenadas de arrastre según Marcelo dentro de los límites de movimiento dados
                if (gs.stylus_y <= 59 + 64)
                {
                    //Coordenadas nuevas por arrastre
                    gs.marcelo_y = gs.stylus_y - 32;
                }
                else
                {
                    //Límite de la altura de Marcelo
                    gs.marcelo_y = 59;
                }
            }
            else
            {
                gs.marcelo_y = 59;
            }
        }
        else    //Regresa a la posición adecuada a Marcelo si no se le está arrastrando fuera
        {
            if (gs.marcelo_y > 59)
            {
                gs.marcelo_y = 59;
            }
        }
    }
}

void ForwardModel::Flexing(GameState& gs, Action& action)
{
    if (gs.lvl == 2)
    {
        if (gs.marcelo_y == 0)
        {
            gs.flex += 1;
        }
    }
}
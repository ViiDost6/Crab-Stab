#include "Action.h"

Action::Action()
{
    action = 0;
}

void Action::SetStart()     {action = 1;}
void Action::SetTouch()     {action = 2;}

bool Action::IsStart()      {return action == 1;}
bool Action::IsTouch()      {return action == 2;}

void Action::SetUp()        {action = 3;}
void Action::SetDown()      {action = 4;}
void Action::SetLeft()      {action = 5;}
void Action::SetRight()     {action = 6;}

bool Action::IsUp()         {return action == 3;}
bool Action::IsDown()       {return action == 4;}
bool Action::IsLeft()       {return action == 5;}
bool Action::IsRight()      {return action == 6;}

void Action::SetA()         {action = 7;}
void Action::SetB()         {action = 8;}
void Action::SetY()         {action = 9;}
void Action::SetX()         {action = 10;}

bool Action::IsA()          {return action == 7;}
bool Action::IsB()          {return action == 8;}
bool Action::IsY()          {return action == 9;}
bool Action::IsX()          {return action == 10;}
#ifndef ACTION_H
#define ACTION_H

class Action
{
    public:
        int action;
        Action();

        void SetUp();
        void SetDown();
        void SetLeft();
        void SetRight();

        void SetStart();
        void SetTouch();

        void SetA();
        void SetB();
        void SetY();
        void SetX();

        bool IsUp();
        bool IsDown();
        bool IsLeft();
        bool IsRight();

        bool IsStart();
        bool IsTouch();

        bool IsA();
        bool IsB();
        bool IsY();
        bool IsX();
};
#endif
#ifndef REDSQUARE_CONSTANTS_H
#define REDSQUARE_CONSTANTS_H


namespace redsquare {

    constexpr int TOP = 160;
    constexpr int BOTTOM = 160;

    constexpr int TOPLEFTCORNER = 181; // intern
    constexpr int TOPRIGTHCORNER = 182; // intern

    constexpr int BOTTOMLEFTCORNER = 201; // intern
    constexpr int BOTTOMRIGTHCORNER = 202 ;// intern


    constexpr int TOPLEFTCORNEREXT = 181; // extern
    constexpr int TOPRIGTHCORNEREXT = 182; // extern

    constexpr int BOTTOMLEFTCORNEREXT = 201; // extern
    constexpr int BOTTOMRIGTHCORNEREXT = 202; // extern

    constexpr int LEFT = 200;
    constexpr int RIGHT = 200;

    constexpr int FLOOR1 = 172;
    constexpr int FLOOR2 = 173;
    constexpr int FLOOR3 = 192;
    constexpr int FLOOR4 = 193;

    constexpr int FLOOR5 = 174;
    constexpr int FLOOR6 = 175;
    constexpr int FLOOR7 = 194;
    constexpr int FLOOR8 = 195;

    constexpr int FLOOR9 = 176;
    constexpr int FLOOR10 = 177;
    constexpr int FLOOR11 = 196;
    constexpr int FLOOR12 = 197;


    constexpr int PILLARTOP = 220;
    constexpr int PILLARBOTTOM = 200;
    constexpr int STAIR = 327;
    //constexpr int VOID  = 83; // replaced this tile by nothing because there was white stripe randomly appearing on screen between these tiles

    constexpr int MAX_SPELL_PER_PLAYER = 8;
    constexpr int MAX_SIZE_MESSAGE_CHAT = 512;
    constexpr int MAX_SIZE_FROM_CHAT = 30;

    constexpr double POTRATIO1 = 0.25;
    constexpr double POTRATIO2 = 0.50;
    constexpr double POTRATIO3 = 0.70;

    constexpr double BOOSTRATIO1 = 0.10;
    constexpr double BOOSTRATIO2 = 0.15;
    constexpr double BOOSTRATIO3 = 0.20;

    constexpr int MINSIZE = 6;

    constexpr int NBTYPEPLAYER = 4;

    constexpr int NBMONSTER = 17;

    constexpr int EQUIPMENT_TYPE = 9; 
}

#endif // REDSQUARE_CONSTANTS_H

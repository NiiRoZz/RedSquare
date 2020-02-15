#ifndef REDSQUARE_CONSTANTS_H
#define REDSQUARE_CONSTANTS_H

namespace redsquare
{
    constexpr int MIN_PLAYERS = 1;
    constexpr int MAX_PLAYERS = 9;

    constexpr gf::Vector2i InitialSize = { 1024, 576 };
    constexpr const char *Service = "6000";

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

    constexpr double BOOSTRATIO1 = 0.05;
    constexpr double BOOSTRATIO2 = 0.10;
    constexpr double BOOSTRATIO3 = 0.15;

    constexpr int MINSIZE = 6;

    constexpr int NBTYPEPLAYER = 4;

    constexpr int NBMONSTER = 17;

    constexpr int EQUIPMENT_TYPE = 9; 
    constexpr int POTION_TIER1 = 8; 
    constexpr int POTION_TIER2 = 16; 
    constexpr int POTION_TIER3 = 24; 

    constexpr int TIER1 = 5;
    constexpr int TIER2 = 10;
    constexpr int TIER3 = 15;

    constexpr int MAGUS_LIFEPOINT = 200; 
    constexpr int MAGUS_MANAPOINT = 30; 
    constexpr int MAGUS_MAXLIFEPOINT = 200;
    constexpr int MAGUS_MAXMANAPOINT = 30; 
    constexpr int MAGUS_ATTACKPOINT = 60; 
    constexpr int MAGUS_DEFENSEPOINT = 10; 
    constexpr int MAGUS_MAXATTACKPOINT = 60; 
    constexpr int MAGUS_MAXDEFENSEPOINT = 10; 
    constexpr int MAGUS_RANGE = 2; 

    constexpr int WARRIOR_LIFEPOINT = 300; 
    constexpr int WARRIOR_MANAPOINT = 20; 
    constexpr int WARRIOR_MAXLIFEPOINT = 300;
    constexpr int WARRIOR_MAXMANAPOINT = 20; 
    constexpr int WARRIOR_ATTACKPOINT = 70; 
    constexpr int WARRIOR_DEFENSEPOINT = 20; 
    constexpr int WARRIOR_MAXATTACKPOINT = 70; 
    constexpr int WARRIOR_MAXDEFENSEPOINT = 20; 
    constexpr int WARRIOR_RANGE = 1; 

    constexpr int ROGUE_LIFEPOINT = 250; 
    constexpr int ROGUE_MANAPOINT = 25; 
    constexpr int ROGUE_MAXLIFEPOINT = 250;
    constexpr int ROGUE_MAXMANAPOINT = 25; 
    constexpr int ROGUE_ATTACKPOINT = 80; 
    constexpr int ROGUE_DEFENSEPOINT = 15; 
    constexpr int ROGUE_MAXATTACKPOINT = 80; 
    constexpr int ROGUE_MAXDEFENSEPOINT = 15; 
    constexpr int ROGUE_RANGE = 1; 

    constexpr int RANGER_LIFEPOINT = 190; 
    constexpr int RANGER_MANAPOINT = 20; 
    constexpr int RANGER_MAXLIFEPOINT = 190;
    constexpr int RANGER_MAXMANAPOINT = 20; 
    constexpr int RANGER_ATTACKPOINT = 45; 
    constexpr int RANGER_DEFENSEPOINT = 10; 
    constexpr int RANGER_MAXATTACKPOINT = 45; 
    constexpr int RANGER_MAXDEFENSEPOINT = 10; 
    constexpr int RANGER_RANGE = 3;

    constexpr int HEALER_LIFEPOINT = 200; 
    constexpr int HEALER_MANAPOINT = 40; 
    constexpr int HEALER_MAXLIFEPOINT = 190;
    constexpr int HEALER_MAXMANAPOINT = 40; 
    constexpr int HEALER_ATTACKPOINT = 55; 
    constexpr int HEALER_DEFENSEPOINT = 10; 
    constexpr int HEALER_MAXATTACKPOINT = 55; 
    constexpr int HEALER_MAXDEFENSEPOINT = 10; 
    constexpr int HEALER_RANGE = 2; 
}

#endif // REDSQUARE_CONSTANTS_H

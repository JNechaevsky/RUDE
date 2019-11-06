//
// Copyright(C) 2019 James Canete
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//

#include "d_compat.h"
#include "sounds.h"
#include "d_mode.h"

#include "doomstat.h"

#include "deh_misc.h"

#include "m_argv.h"

extern fixed_t finetangent1_2[4096];
extern fixed_t finesine1_2[10240];
extern angle_t tantoangle1_2[2049];

#define COARSEANGLES1_0		256
#define COARSEMASK1_0		(COARSEANGLES1_0-1)

// 0x100000000 to 0x0100
#define ANGLETOCOARSESHIFT1_0	24		

// Effective size is 320.
const fixed_t coarsesine1_0[5*COARSEANGLES1_0/4] =
{
    0,1608,3215,4821,6423,8022,9616,11204,
    12785,14359,15923,17479,19024,20557,22078,23586,
    25079,26557,28020,29465,30893,32302,33692,35061,
    36409,37736,39039,40319,41575,42806,44011,45189,
    46340,47464,48558,49624,50660,51665,52639,53581,
    54491,55368,56212,57022,57797,58538,59243,59913,
    60547,61144,61705,62228,62714,63162,63571,63943,
    64276,64571,64826,65043,65220,65358,65457,65516,
    65536,65516,65457,65358,65220,65043,64826,64571,
    64276,63943,63571,63162,62714,62228,61705,61144,
    60547,59913,59243,58538,57797,57022,56212,55368,
    54491,53581,52639,51665,50660,49624,48558,47464,
    46340,45189,44011,42806,41575,40319,39039,37736,
    36409,35061,33692,32302,30893,29465,28020,26557,
    25079,23586,22078,20557,19024,17479,15923,14359,
    12785,11204,9616,8022,6423,4821,3215,1608,
    0,-1608,-3215,-4821,-6423,-8022,-9616,-11204,
    -12785,-14359,-15923,-17479,-19024,-20557,-22078,-23586,
    -25079,-26557,-28020,-29465,-30893,-32302,-33692,-35061,
    -36409,-37736,-39039,-40319,-41575,-42806,-44011,-45189,
    -46340,-47464,-48558,-49624,-50660,-51665,-52639,-53581,
    -54491,-55368,-56212,-57022,-57797,-58538,-59243,-59913,
    -60547,-61144,-61705,-62228,-62714,-63162,-63571,-63943,
    -64276,-64571,-64826,-65043,-65220,-65358,-65457,-65516,
    -65536,-65516,-65457,-65358,-65220,-65043,-64826,-64571,
    -64276,-63943,-63571,-63162,-62714,-62228,-61705,-61144,
    -60547,-59913,-59243,-58538,-57797,-57022,-56212,-55368,
    -54491,-53581,-52639,-51665,-50660,-49624,-48558,-47464,
    -46340,-45189,-44011,-42806,-41575,-40319,-39039,-37736,
    -36409,-35061,-33692,-32302,-30893,-29465,-28020,-26557,
    -25079,-23586,-22078,-20557,-19024,-17479,-15923,-14359,
    -12785,-11204,-9616,-8022,-6423,-4821,-3215,-1608,
    0,1608,3215,4821,6423,8022,9616,11204,
    12785,14359,15923,17479,19024,20557,22078,23586,
    25079,26557,28020,29465,30893,32302,33692,35061,
    36409,37736,39039,40319,41575,42806,44011,45189,
    46340,47464,48558,49624,50660,51665,52639,53581,
    54491,55368,56212,57022,57797,58538,59243,59913,
    60547,61144,61705,62228,62714,63162,63571,63943,
    64276,64571,64826,65043,65220,65358,65457,65516
};

// These default to finesine, but change to coarsesine with Doom v1.0
int angletocoarseshift = ANGLETOFINESHIFT;
const fixed_t *coarsesine = &finesine1_2[0];
const fixed_t *coarsecosine = &finesine1_2[FINEANGLES/4];

struct {
    unsigned short index;
    short delta;
} finesine_to_1_0[33] = {
    {455,-1},
    {1080,-1},
    {2268,1},
    {3212,-1},
    {3324,-1},
    {3640,-1},
    {3822,1},
    {4147,1},
    {4959,-1},
    {5174,-1},
    {6722,-1},
    {7111,1},
    {7140,-1},
    {7170,-1},
    {7328,-1},
    {7396,-1},
    {7420,1},
    {7947,-1},
    {8077,-1},
    {8140,1},
    {8262,-1},
    {8306,1},
    {8647,1},
    {8821,-1},
    {8823,-1},
    {8963,-1},
    {9055,1},
    {9259,-1},
    {9272,-1},
    {9325,1},
    {9451,-1},
    {9465,-1},
    {10019,1}
};

const short finetangent_to_1_0[] = {
    19,18234,1920,652,314,178,111,74,51,37,27,20,14,11,8,5,
    4,2,2,1,2,43,-1,-2,-2,-2,-2,-2,-3,-3,-2,-3,
    -2,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,
    3,3,3,2,2,2,2,1,1,2,2,1,1,1,1,1,
    1,2,3,1,0,1,2,1,1,9,1,-1,2,1,-1,2,
    4,-1,-1,-1,-1,2,8,-1,0,-1,0,-1,0,-1,-1,7,
    1,1,2,1,1,2,1,1,3,1,1,3,1,1,10,1,
    -1,12,2,-1,-1,3,1,-1,4,1,1,3,3,1,0,1,
    6,1,1,10,2,1,1,15,1,-1,7,1,-1,10,1,1,
    12,1,1,34,1,-1,7,1,1,274,1,1,51,1,1,11,
    1,1,13,1,1,2844,1,-1,13,1,-1,11,1,-1,51,1,
    -1,274,1,-1,7,1,1,34,1,-1,12,1,-1,10,1,1,
    7,1,1,15,2,-1,-1,10,1,-1,6,3,-1,0,-1,3,
    1,-1,4,1,1,3,2,1,1,12,1,1,10,1,-1,3,
    1,-1,3,1,-1,2,1,-1,2,1,-1,7,8,1,1,0,
    1,0,1,0,1,2,4,1,1,1,1,2,1,1,2,1,
    1,9,1,-1,2,3,-1,0,-1,2,43,-1,-1,-1,-1,-1,
    -1,-2,-2,-1,-1,-2,-2,-2,-2,-3,-3,-3,3,3,3,3,
    3,3,3,3,3,3,3,3,3,3,3,2,3,2,3,3,
    2,2,2,2,2,1,2,19,-1,-2,-2,-4,-5,-8,-11,-14,
    -20,-27,-37,-51,-74,-111,-178,-314,-652,-1920,-18234,0
};

const char tantoangle_to_1_0[2049] = {
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-1,0,-1,0,
    -1,0,-1,-1,-1,0,0,0,0,0,0,-1,0,-1,-1,0,0,-1,-1,0,-1,0,-1,-1,
    -1,-1,-1,0,-1,0,0,0,-1,-1,-1,-1,-1,0,0,-1,-1,0,-1,0,0,-1,-1,0,
    0,0,-1,0,-1,0,-1,-1,-1,0,-1,0,0,-1,-1,0,0,0,0,0,-1,0,-1,0,
    0,-1,0,0,0,1,-1,1,-1,0,1,1,0,-2,-2,-2,-1,-1,1,1,-1,-1,0,-2,
    -2,-2,0,-2,1,0,0,-1,-1,-2,1,-1,1,-2,1,0,1,1,0,0,-1,0,-2,-2,
    -1,-2,-2,-1,1,-1,-2,1,1,0,0,-1,-2,1,-2,-1,-1,-2,-1,-1,1,-2,0,1,
    -1,-1,0,-1,-1,0,-2,0,-1,-2,-2,-2,1,1,0,-2,0,1,1,0,-2,-2,1,-2,
    1,-2,1,-2,0,-1,0,-2,0,0,-4,0,-3,2,-1,-4,1,-3,0,3,-3,-2,-2,-3,
    1,-3,-2,-2,-4,-1,0,-2,2,3,1,-4,-4,0,1,-2,-1,-4,-3,2,2,-2,-3,-1,
    -4,-4,-1,-4,-4,-3,1,-1,-1,0,3,-1,-4,3,2,3,-4,-3,-1,1,3,-3,-2,0,
    0,1,0,-2,-4,0,3,-4,3,1,-3,-1,-2,3,-2,-2,3,-3,-4,0,1,-2,-1,-3,
    -1,-4,-3,2,3,-1,-2,0,-3,-3,0,-3,-4,-2,2,-1,-1,0,3,-1,-4,3,3,3,
    -4,-2,0,2,-3,-1,2,-4,-2,-1,0,0,-1,-4,1,-4,-2,-2,-4,1,3,-4,1,-3,
    -2,-4,0,0,-3,-1,-2,1,1,-4,-4,0,-1,3,1,-4,1,2,-3,-4,-3,1,-1,0,
    3,0,-1,0,2,-2,-4,2,2,3,-3,0,3,-1,3,0,-4,1,-3,1,-3,0,3,-3,
    -2,-2,-3,2,-2,1,1,0,-3,0,1,0,-4,-3,-4,0,1,-1,2,2,-2,-2,3,-4,
    -7,2,-1,-1,3,-6,5,3,4,-8,-1,-8,4,2,3,6,-5,2,-5,5,1,-1,-3,-2,
    -1,1,4,-8,-4,1,6,-4,1,7,-4,2,7,-5,-1,2,5,6,7,6,4,0,-5,5,
    -4,2,6,7,7,4,-2,6,-5,-2,-3,-6,3,-7,-4,-5,7,-2,2,2,-2,6,-7,-8,
    3,-7,-6,6,-3,-1,-4,4,6,3,-6,-5,6,-5,-6,3,6,2,-8,7,-1,1,-5,-2,
    -6,-1,-3,2,1,7,6,-4,-5,1,-1,5,2,6,2,5,-1,0,-8,7,-4,-8,-6,3,
    1,6,1,2,-7,5,7,-2,-5,-3,4,1,2,-8,3,2,6,-1,-4,-3,2,-4,-7,-6,
    -1,-8,4,3,6,-4,6,2,1,3,-8,0,-6,6,5,6,-7,-2,6,-1,-6,6,4,4,
    5,7,-6,-1,4,-6,1,-7,2,-6,4,-3,7,1,-5,5,-1,-7,2,-5,3,-6,1,7,
    -3,1,4,6,7,6,4,0,-5,4,-5,0,3,5,4,0,-5,3,-8,-5,-5,-8,2,-6,
    -2,-1,-3,7,-1,2,2,-1,7,-4,-3,-7,2,7,7,2,-6,-3,-5,4,-7,-7,3,-7,
    -6,5,-5,-4,7,-3,-3,-8,-4,-5,4,6,3,-7,-7,3,6,3,-7,-7,1,3,-2,2,
    -1,4,3,-6,-7,1,2,-5,-4,5,7,0,2,-5,-3,6,6,-2,-2,5,4,-6,7,-5,
    6,-8,1,1,-8,5,-7,3,4,-5,-7,-3,7,7,-3,-7,-5,3,0,4,-4,-5,-2,7,
    6,-7,2,0,3,-5,7,-7,-1,-6,-6,-2,6,3,4,-6,4,1,3,-7,3,1,2,7,
    0,-3,-3,0,7,1,-2,-1,3,-7,3,0,0,2,7,-1,-7,6,6,-8,-4,2,-5,6,
    3,2,4,7,-4,2,-5,5,1,-2,-3,-3,-1,2,6,-5,2,-7,2,-4,6,2,-2,-5,
    -8,6,5,4,4,4,5,5,6,7,-8,-7,-5,-4,-3,-3,-2,-2,-2,-2,-3,-5,-7,7,
    3,-1,-5,5,-2,7,-1,5,-5,-1,2,4,4,4,1,-2,-8,2,-7,-1,4,6,7,6,
    3,-2,7,-2,2,5,6,4,0,-7,0,5,7,7,4,-1,-10,11,-2,13,-6,3,10,14,
    15,12,7,-2,-14,3,-16,-6,1,4,3,0,-8,13,-2,12,-10,-4,-2,-4,-10,13,-1,13,
    -9,-3,-1,-3,-10,11,-4,8,-16,-12,-13,13,3,-12,1,9,12,10,4,-8,8,-13,-7,-6,
    -11,12,-2,10,-15,-13,-16,8,-6,6,14,-16,12,3,-12,0,6,6,1,-10,5,14,-14,-16,
    8,-7,5,11,11,5,-7,6,13,15,10,-2,13,-11,-10,-14,7,-11,-3,-2,-7,13,-5,2,
    3,-4,15,-5,1,-1,-9,8,-14,-11,-15,6,-12,-5,-5,-12,5,-16,-13,15,4,-14,-8,-9,
    15,-1,8,10,4,-10,1,5,1,-11,2,7,5,-5,9,15,14,5,-12,-5,-6,-14,2,9,
    9,1,-15,-7,-7,-16,0,8,7,-1,14,-11,-12,10,-7,-1,-3,-14,0,4,1,-11,1,4,
    -1,-15,-5,-3,-11,6,14,13,3,-15,-9,-13,8,-13,-10,-16,1,9,9,0,14,-13,-16,3,
    14,-16,9,-7,0,-2,-12,0,3,-3,14,-9,-10,12,-7,-3,-8,9,-14,-15,7,-12,-8,-13,
    4,13,12,1,14,-15,11,-4,3,1,-10,1,3,-5,11,-15,13,0,10,10,0,14,-15,12,
    -3,4,2,-10,1,2,-6,8,13,8,-6,2,0,-11,0,2,-7,8,12,8,-7,1,-1,-13,
    -2,-1,-10,4,8,2,-13,-6,-9,10,-12,-12,10,-10,-7,-14,1,6,2,-12,-4,-6,14,-7,
    -6,-15,-2,1,-5,11,-15,13,-1,8,7,-5,7,8,-1,13,-16,10,-6,0,-4,15,-8,-10,
    11,-10,-9,15,-4,0,-6,9,-16,12,-2,7,5,-6,5,6,-3,11,14,8,-8,-2,-6,12,
    -11,-13,8,-13,-12,11,-7,-4,-10,6,12,9,-5,4,3,-8,4,5,-3,11,15,10,-6,1,
    -2,-14,-5,-5,-15,-2,0,-7,9,14,10,-4,4,3,-8,3,5,-3,11,15,10,-4,3,1,
    -10,1,2,-7,7,12,6,-8,-1,-3,-14,-3,-2,-10,5,10,5,-9,-1,-2,-12,0,2,-5,
    11,-15,14,2,12,12,4,-15,-10,-15,3,12,11,1,14,-15,11,-4,4,2,-9,3,6,-1,
    -16,-9,-11,10,-11,-8,-15,2,9,7,-3,9,12,6,-9,-1,-2,-12,2,6,1,-13,-3,-3,
    -11,4,10,7,-5,7,10,4,-11,-3,-3,-12,2,8,5,-7,5,8,2,-12,-3,-3,-11,5,
    12,10,0,14,-13,-16,4,-16,-13,14,1,11,13,7,-8,1,2,-6,11,-13,-13,11,-6,2,
    1,-8,8,15,14,5,-12,-5,-6,-15,0,8,7,-2,14,-10,-10,14,-2,7,8,1,-14,-4,
    -2,-8,11,-10,-7,-11,10,-9,-4,-6,-16,-1,7,7,-1,-15,-5,-3,-7,13,-6,-1,-3,-11,
    5,13,15,10,-3,9,15,13,4,-12,-2,0,-4,-16,-2,5,5,-2,-16,-4,1,-1,-10,7,
    -15,-11,-14,8,-8,1,4,0,-10,6,14,-15,13,3,-13,-4,-1,-5,-14,2,12,-16,14,6,
    -9,3,9,8,2,-11,3,11,13,9,-1,15,-6,-1,-2,-9,11,-7,2,5,2,-6,12,-7,
    0,2,-2,-11,7,-13,-6,-4,-8,15,1,14,-10,-8,-10,14,1,-16,-7,-3,-4,-9,12,-3,
    9,-16,-14,-16,9,-3,12,-9,-3,-1,-4,-12,8,-8,3,10,12,10,3,-7,10,-9,-1,4,
    4,0,-8,12,-4,7,15,-13,-13,15,7,-4,12,-7,2,7,9,7,1,-8,11,-5,7,15,
    -12,-10,-12,15,7,-5,12,-7,4,11,15,-16,14,9,1,-10,8,-9,3,12,-14,-11,-10,-12,
    15,7,-3,-16,1,15,-6,3,10,14,15,14,11,6,-2,-12,7,-7,8,-10,1,10,-15,-10,
    -7,-6,-7,-10,-14,11,3,-7,14,0,-15,1,15,-5,6,15,-9,-3,2,6,8,9,9,7,
    5,1,-4,-10,15,7,-2,-13,8,-4,-16,2,-12,4,-12,4,-14,1,14,-5,7,-13,-1,10,
    -12,-2,8,-14,-5,4,13,-10,-1,7,-16,-8,1,10,-14,-5,4,13,-10,0,10,-12,-2,9,
    -11,0,13,-7,7,-11,3,-13,3,-12,5,-9,11,-1,-12,9,0,-8,-15,11,6,3,0,-1,
    -1,0,3,7,12,-13,-4,6,-15,-2,13,-3,15,3,-8,15,8,3,0,-1,0,3,7,14,
    -9,3,-16,0,-14,6,-4,-11,-15,14,15,-15,-9,-1,9,-10,6,-7,14,7,2,0,1,4,
    11,-11,2,-14,5,-4,-11,-14,-14,-10,-4,6,-12,5,-6,-14,14,13,-16,-9,1,15,1,-9,
    -15,14,-16,-11,-1,13,-2,-12,14,13,15,-10,1,-15,5,-3,-6,-5,1,11,-5,14,7,4,
    6,13,-8,9,-2,-7,-8,-3,6,-11,9,2,0,3,12,-6,14,7,5,9,-14,1,-10,-16,
    -16,-10,1,-14,9,6,9,-14,1,-11,-16,-15,-8,6,-7,-13,-13,-7,6,-7,-13,-13,-6,7,
    -5,-11,-10,-2,13,2,-2,1,11,-4,-12,-13,-7,6,-5,-10,-7,3,-12,12,12,-12,2,-7,
    -9,-4,9,-2,-6,-2,10,-3,-7,-4,7,-5,-10,-7,4,-8,-13,-9,3,-9,-12,-7,6,-4,
    -6,0,-16,7,8,-15,3,-3,0,12,1,-1,6,-10,14,-16,-5,15,12,-13,2,-5,-3,9,
    -2,-4,4,-10,-15,-11,3,-5,-3,8,-2,-3,5,-8,-11,-4,12,7,12,-5,-11,-8,6,-2,
    0,13,4,6,-14,9,11,-10,13,14,-5,-15,-13,0,-9,-7,7,-1,3,-15,11,-16,0,-5,
    1,-13,-15,-7,14,13,-7,15,-14,0,-7,-1,-16,13,-10,11,12,-8,-15,-10,6,3,13,2,
    3,-15,12,-14,5,4,-16,9,14,-1,-3,8,0,4,-10,-12,-1,-10,-5,13,12,-8,-15,-9,
    10,11,-7,-12,-3,-13,-10,7,6,-14,11,-14,7,10,-6,-7,5,-2,6,-4,0,-14,-15,-1,
    -5,6,-2,5,-6,-2,-16,-16,-1
};

void ChangeTrigTablesToDoom1_0()
{
    int i, j, count;

    for (i = 0; i < 33; i++)
    {
        finesine1_2[finesine_to_1_0[i].index] += finesine_to_1_0[i].delta;
    }

    for(i = 0, j = 0, count = 1; count; j += count)
    {
        if(!(count = finetangent_to_1_0[i++]))
        {
            break;
        }

        while(count--)
        {
            finetangent1_2[j++] += finetangent_to_1_0[i++];
        }

        count = finetangent_to_1_0[i++];
    }

    for (i = 0; i < 2049; i++)
    {
        tantoangle1_2[i] += tantoangle_to_1_0[i];
    }

    angletocoarseshift = ANGLETOCOARSESHIFT1_0;
    coarsesine = coarsesine1_0;
    coarsecosine = &coarsesine1_0[64];
}

extern fixed_t forwardmove[2], sidemove[2];

// Doom 1.0 speed values, scaled by 1/900
fixed_t         forwardmove10[2] = {51200/900, 102400/900}; 
fixed_t         sidemove10[2] = {49152/900, 81920/900}; 

fixed_t turbo_scale = 100;
fixed_t cmd_move_scale = 2048;

int sfx_getpow_1_2 = sfx_getpow;

void D_SetConstantsForGameversion()
{
    // Each entry is the last version where the statement is true.
    switch (gameversion)
    {
        case exe_doom_1_0:
            // Different trig tables
            ChangeTrigTablesToDoom1_0();

            // Skull keys still spawn in deathmatch
            mobjinfo[MT_MISC7].flags &= ~MF_NOTDMATCH;
            mobjinfo[MT_MISC8].flags &= ~MF_NOTDMATCH;
            mobjinfo[MT_MISC9].flags &= ~MF_NOTDMATCH;

        // fallthrough
        case exe_doom_1_1:
            // Env suits count for item %
            // https://tcrf.net/Doom_(PC,_1993)/Revisional_Differences#v1.2
            mobjinfo[MT_MISC14].flags |= MF_COUNTITEM;

            // Player move is different in Doom v1.0/v1.1
            forwardmove[0] = forwardmove10[0]*turbo_scale/100;
            forwardmove[1] = forwardmove10[1]*turbo_scale/100;
            sidemove[0] = sidemove10[0]*turbo_scale/100;
            sidemove[1] = sidemove10[1]*turbo_scale/100;
            cmd_move_scale = 900;

        // fallthrough
        case exe_doom_1_2:
            // sfx_getpow does not exist, use sfx_itemup instead
            // FIXME: not sure about last version, true at least in v1.2
            sfx_getpow_1_2 = sfx_itemup;

            // Boss spider is not fullbright when attacking
            // FIXME: not sure about last version, true at least in v1.2
            states[S_SPID_ATK1].frame &= ~FF_FULLBRIGHT;
            states[S_SPID_ATK2].frame &= ~FF_FULLBRIGHT;
            states[S_SPID_ATK3].frame &= ~FF_FULLBRIGHT;
            states[S_SPID_ATK4].frame &= ~FF_FULLBRIGHT;

            // Powerups are not fullbright
            // FIXME: not sure about last version, true at least in v1.2
            states[S_SOUL].frame &= ~FF_FULLBRIGHT;
            states[S_SOUL2].frame &= ~FF_FULLBRIGHT;
            states[S_SOUL3].frame &= ~FF_FULLBRIGHT;
            states[S_SOUL4].frame &= ~FF_FULLBRIGHT;
            states[S_SOUL5].frame &= ~FF_FULLBRIGHT;
            states[S_SOUL6].frame &= ~FF_FULLBRIGHT;
            states[S_PINV].frame &= ~FF_FULLBRIGHT;
            states[S_PINV2].frame &= ~FF_FULLBRIGHT;
            states[S_PINV3].frame &= ~FF_FULLBRIGHT;
            states[S_PINV4].frame &= ~FF_FULLBRIGHT;
            states[S_PSTR].frame &= ~FF_FULLBRIGHT;
            states[S_PINS].frame &= ~FF_FULLBRIGHT;
            states[S_PINS2].frame &= ~FF_FULLBRIGHT;
            states[S_PINS3].frame &= ~FF_FULLBRIGHT;
            states[S_PINS4].frame &= ~FF_FULLBRIGHT;
            states[S_SUIT].frame &= ~FF_FULLBRIGHT;
            states[S_PMAP].frame &= ~FF_FULLBRIGHT;
            states[S_PMAP2].frame &= ~FF_FULLBRIGHT;
            states[S_PMAP3].frame &= ~FF_FULLBRIGHT;
            states[S_PMAP4].frame &= ~FF_FULLBRIGHT;
            states[S_PMAP5].frame &= ~FF_FULLBRIGHT;
            states[S_PMAP6].frame &= ~FF_FULLBRIGHT;

            // https://doomwiki.org/wiki/Health_limited_to_199
            // FIXME: trcf claims this changed in v1.6, please check
            // https://tcrf.net/Doom_(PC,_1993)/Revisional_Differences#v1.6
            deh_max_health = 199;
            deh_max_soulsphere = 199;

        // fallthrough
        //case exe_doom_1_3:
        //case exe_doom_1_25:
            // Deathmatch 2.0 not supported
            // https://doomwiki.org/wiki/Versions_of_Doom_and_Doom_II#v1.4
            if (deathmatch == 2)
            {
                deathmatch = 0;
                if (M_CheckParm ("-deathmatch"))
                {
                    deathmatch = 1;
                }
            }

        // fallthrough
        //case exe_doom_1_4:
        //case exe_doom_1_5:
        //case exe_doom_1_6:
            // lost souls count for kill %
            // https://doomwiki.org/wiki/Versions_of_Doom_and_Doom_II#v1.666
            mobjinfo[MT_SKULL].flags |= MF_COUNTKILL;

        // fallthrough
        case exe_doom_1_666:
        case exe_doom_1_7:
        case exe_doom_1_8:
        case exe_doom_1_9:
        case exe_hacx:
            // 4th episode (retail) not supported
            // https://doomwiki.org/wiki/Versions_of_Doom_and_Doom_II#Ultimate_Doom
            if (gamemode == retail)
            {
                gamemode = registered;
            }

        // fallthrough
        case exe_ultimate:
            // Final Doom not supported
            // https://doomwiki.org/wiki/Versions_of_Doom_and_Doom_II#Final_Doom
            if (gamemode == commercial 
             && (gamemission == pack_tnt || gamemission == pack_plut))
            {
                gamemission = doom2;
            }

        // fallthrough
        case exe_final:
        case exe_final2:
        case exe_chex:
        default:
            break;
    }
}

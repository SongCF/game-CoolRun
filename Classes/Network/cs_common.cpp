#include "cs_common.h"
#include "protocol.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#define _CLIENT_
#endif

#ifdef  _CLIENT_
typedef void (*LogFunPt)(const char * pszFormat, ...);
//LogFunPt LogMsg = CCLOG;
#else
#include "logs.h"
#include <assert.h>
#endif


RankBonusID CS::get_rank_bonus_id(int order)
{
    if(order <= 30 && order >20) {
        return RankBonusID_21_30;
    }
    else if(order <= 20 && order >10) {
        return RankBonusID_11_20;
    }
    else if(order <= 10 && order >5) {
        return RankBonusID_6_10;
    }
    else if(order <= 5 && order >3) {
        return RankBonusID_4_5;
    }
    else if(order == 3) {
        return RankBonusID_3;
    }
    else if(order == 2) {
        return RankBonusID_2;
    }
    else if(order == 1) {
        return RankBonusID_1;
    }
    else {
        return RankBonusID_Invalid;
    }
}

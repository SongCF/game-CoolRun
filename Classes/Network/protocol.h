
#ifndef SIMPLEPROTOCOLCMD_H
#define SIMPLEPROTOCOLCMD_H

// #include <arpa/inet.h>
// #include "utils.h"
#include "cocos2d.h"
using namespace cocos2d;
#if ( CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include <winsock2.h>
#else
#include <arpa/inet.h>
#endif

#include <assert.h>

#define Current_ProtocolVersion 1


#define SimpleProtocol_Tag_Server 21335
#define SimpleProtocol_Tag_Client 21336

// 无效Id
#define INVALID_ID 0
    
//客户端登录
#define ClientAccountMaxLen 32
#define ClientPswMaxLen     16


/* 返回包消息错误代码 */
enum MessageErrorCode {
    Success             	= 0,	//成功
    Error               	= 1,	//错误
};


/* 协议指令 */
enum MessageCmd {
    /* 请求命令 */
    ReqCmdStart             = 0,
    
    // 登陆
    Req_Regist				= ReqCmdStart + 1,
    Req_Login,
    Req_AlterInfo,
    Req_UpdateInfo,

    //邮件
    Req_QueryMail			= ReqCmdStart + 10,
    Req_ReadMail,
    
    //排行榜
    Req_QueryRankBonus		= ReqCmdStart + 20,
    Req_QueryRankList,
    
    //系统奖励
    Req_QuerySystemBonus	= ReqCmdStart + 30,
    Req_GetSystemBonus,
    
    //礼包
    Req_QueryGiftBag	    = ReqCmdStart + 40,
    
    ReqCmdEnd,


    /* 返回命令 */
    RspCmdStart             = 20000,
    
    //登陆
    Rsp_Regist				= RspCmdStart + 1,
    Rsp_Login,
    Rsp_AlterInfo,
    Rsp_UpdateInfo,
    
    //邮件
    Rsp_QueryMail			= RspCmdStart + 10,
    Rsp_ReadMail,
    
    //排行榜
    Rsp_QueryRankBonus		= RspCmdStart + 20,
    Rsp_QueryRankList,
    
    //系统奖励
    Rsp_QuerySystemBonus	= RspCmdStart + 30,
    Rsp_GetSystemBonus,
    
    //礼包
    Rsp_QueryGiftBag	    = RspCmdStart + 40,
    
    RspCmdEnd,
        
        
    /* 其他命令 */
    OtherCmdStart           = 60000,
    
    //统一分配地址
    Req_Unify	            = OtherCmdStart + 1,
    Rsp_Unify,
    
    //重新加载配置模板
    Req_Reconfigure         = OtherCmdStart + 10,
    Rsp_Reconfigure,

    OtherCmdEnd,
};


#pragma pack(1)

/* 包头 */
struct SPHead
{
    unsigned short protocol_tag;
    unsigned short protocol_ver;
    unsigned short cmd;
    unsigned char not_use1;
    unsigned char not_use2;
    int data_len;
    //char protocol_buf_message[];
    //int check_sum;
};

inline void hton_SPHead(SPHead* head)
{
    head->protocol_tag = htons(head->protocol_tag);
    head->protocol_ver = htons(head->protocol_ver);
    head->cmd = htons(head->cmd);
    head->data_len = htonl(head->data_len);
}

inline void ntoh_SPHead(SPHead* head)
{
    head->protocol_tag = ntohs(head->protocol_tag);
    head->protocol_ver = ntohs(head->protocol_ver);
    head->cmd = ntohs(head->cmd);
    head->data_len = ntohl(head->data_len);
}

inline bool isValiedServerHead(SPHead* head)
{
	unsigned short tag = ntohs(head->protocol_tag);
	unsigned short ver = ntohs(head->protocol_ver);
	unsigned short cmd = ntohs(head->cmd);
	if (tag != SimpleProtocol_Tag_Server) return false;
	if (ver != Current_ProtocolVersion) return false;
	if ((cmd > RspCmdStart && cmd < RspCmdEnd) || (cmd > OtherCmdStart && cmd < OtherCmdEnd)) return true;
	return false;
}
inline bool isValiedClientHead(SPHead* head)
{
	unsigned short tag = ntohs(head->protocol_tag);
	unsigned short ver = ntohs(head->protocol_ver);
	unsigned short cmd = ntohs(head->cmd);
	if (tag != SimpleProtocol_Tag_Client) return false;
	if (ver != Current_ProtocolVersion) return false;
	if ((cmd > ReqCmdStart && cmd < ReqCmdEnd) || (cmd > OtherCmdStart && cmd < OtherCmdEnd)) return true;
	return false;
}

#pragma pack() //结束


#endif //SIMPLEPROTOCOLCMD_H


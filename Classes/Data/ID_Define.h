//============================================================
//* Unit ID 对应的宏定义
//* 如果配置文件里的unit id有改动 则必须更改响应的宏定义
//============================================================

#pragma once


// 	1）ID(001-100)特殊物品special（如：货币）
// 	2）ID(101-200)角色actor
// 	3）ID(201-300)宠物pet
// 	4）ID(301-400)魔法物magic
// 	5）ID(401-500)陆地land
// 	6）ID(501-600)物品item（游戏场景内能看到，能直接拾取的道具，包括增益、减益）
// 	7）ID(601-700)障碍obstacle（不可拾取，但可消除或不可消除类障碍物）
enum UnitID
{
	unitID_None				= 0,

	//special
	unitID_FuHuoDaoJu		= 10,


	unitID_Actor2Fragment	= 92, //碎片
	unitID_RedJewel			= 93, //红宝石（货币）
	unitID_DanGao			= 94, //蛋糕
	unitID_GuoDong			= 95, //甜筒
	unitID_TangGuo			= 96, //糖果
	unitID_RandomItem		= 97, //商店随机道具（随机宝箱）
	unitID_Gold				= 98, //金币（货币）
	unitID_Diamond			= 99, //钻石（货币）
	unitID_RMB				= 100, //人名币

	//actor
	unitID_Actor1			= 101, //
	unitID_Actor2			= 102, //

	//pet
	unitID_PiPiZhu			= 201, //皮皮猪
	unitID_XiXiaoYang       = 202, //嘻小羊
	unitID_BuGuNiao			= 203, //布谷鸟
	unitID_ZhanHongLong		= 204, //战红龙
	unitID_PetEnd			= 205, //

	//magic
	unitID_FaGuangWuXing	= 301, //发光五星
	unitID_JiXingHuoJian	= 302, //急行火箭(暂无)
	unitID_MoLiYingHuo		= 303, //魔力萤火(暂无)

	//land
	unitID_JianXi			= 401, //间隙、彩虹桥
	unitID_LuMian_1			= 411, //路面1
	unitID_LuMian_2			= 412, //路面2
	unitID_LuMian_3			= 413, //路面3
	unitID_LuMian_4			= 414, //路面4
	

	//item
	unitID_TongBi			= 501, //铜币(暂无)
	unitID_JinDan			= 502, //金蛋
	unitID_JinBi			= 503, //金币（分数）
	unitID_ChongCi			= 504, //冲刺
	unitID_KaiChangChongCi  = 505, //开场冲刺
	unitID_SiWangChongCi    = 506, //死亡冲刺
	unitID_WuDi             , //无敌(暂无)
	unitID_CiTie			= 508, //磁铁
	unitID_DianJin			= 509, //点金
	unitID_HuDun			= 510, //护盾
	

	unitID_HongBaoShi		= 520, //红宝石（分数）
	unitID_ZiDan			= 521, //子弹 （战斗模式）
	unitID_DengLong			= 524, //灯笼
	unitID_ZhongDuJieYao	= 525, //中毒解药



	unitID_KaiChangChongCi400			= 526, //开场冲刺400米
	unitID_KaiChangChongCi800			= 527, //开场冲刺800米
	unitID_KaiChangChongCi1000			= 528, //开场冲刺1000米
	unitID_JinBiJiaCheng				= 530, //金币加成道具
	unitID_BiaoXianJiaCheng1			= 531, //表现分加成道具1
	unitID_BiaoXianJiaCheng2			= 532, //表现分加成道具2
	unitID_ZhanDouLiJiaCheng			= 533, //战斗力加成道具
	unitID_ShiYongYiCiChongWu1			= 534, //5级浣熊
	unitID_ShiYongYiCiChongWu2			= 535, //5级猫头鹰

	unitID_DianZha_KaiGuan				= 536, //电闸-开关

	unitID_ZiDanXiao					= 521, //子弹小
	unitID_ZiDanDa						= 550, //子弹大
	unitID_ZiDanChaoDa					= 551, //子弹超大

	//obstacle
	unitID_SuiJiWu						= 601, //出现随机物
	unitID_Boss1						= 602, //boss1
	unitID_JiaoxueLong					= 603, //对话龙
	unitID_Boss2						= 604, //boss2
	unitID_MuZhuang_Ai					= 611, //木桩（矮）
	unitID_MuZhuang_Gao					= 612, //木桩（高）
	unitID_JingTaiGuaiWu_Xiao			= 613, //静态怪物（小）
	unitID_MiFeng						= 614, //蜜蜂
	
	unitID_YiDongGuaiWu_Xiao			= 616, //移动怪物（小）
	unitID_YiDongGuaiWu_Da				= 617, //移动怪物（大）
	unitID_XuanFuZhangAi				= 618, //悬浮障碍
	unitID_KeCaiTa						= 619, //可踩踏
	unitID_DianZha						= 620, //电闸
	unitID_ShanDianLian					= 621, //闪电链，上下晃动

	unitID_MuZhuang_Ai_Ainmation		= 622, //带动画矮障碍
	unitID_MuZhuang_Gao_Ainmation		= 623, //带动画高障碍
	unitID_XuanFuZhangAi_Down_Before	= 624, //下落悬浮障碍(前)
	unitID_XuanFuZhangAi_Down_After		= 625, //下落悬浮障碍(后)
	unitID_XuanFuZhangAi_Up_Before		= 626, //上升悬浮障碍(前)
	unitID_JingTaiGuaiWu_Da				= 627, //静态怪物（大）
	unitID_FeiXingGuai_Xiao				= 628, //飞行怪（小）
	unitID_FeiXingGuai_Da				= 629, //飞行怪（大）
	unitID_CaiTa_Xiao					= 630, //踩踏怪物（小）
	unitID_CaiTa_Da						= 631, //踩踏怪物（大）
	unitID_ZhongDu						= 632,	//中毒
	unitID_XuanFuZhangAi_Up_After		= 633, //上升悬浮障碍(后)

	unitID_XuanFuZhangAi_DownToUp		= 634, //悬浮障碍结束点（让玩家自动变成奔跑状态）

	//tools
	unitID_TanHuang			= 1001, //弹簧
	unitID_TaXian			= 1002, //塌陷路面


	//decorative


// 		unitID_GameEnd = 1001,					//触碰通关
// 		unitID_TeachUp,					//教学触发点1
// 		unitID_TeachDown,				//教学触发点2
// 		unitID_TeachdoubleUP,			//教学触发点3
// 		unitID_FuHuoDaoJu,               //FuHuoDaoJu
//

	//成就ID
	achieveID_ChuPaoZhe = 10001,		//初跑者（完成新手教学）	
	achieveID_XiaoFuHao,				//小富豪（拥有20000金币）
	achieveID_DaTuHao,					//大土豪（拥有50000金币）
	achieveID_ShouCangJia,				//收藏家（拥有所有角色，宠物）
	achieveID_GaoFuShuai,				//高富帅（任意一个角色满级）
	achieveID_BaiFuMei,					//白富美（任意一个宠物满级）
	achieveID_JiaYou,					//加油！（登录2天）
	achieveID_TuanZhang,				//团长（登录7天）


	achieveID_KuaiPaoI,					//快跑I（单局达到1000米）
	achieveID_KuaiPaoII,				//快跑II（单局达到2000米）
	achieveID_KuaiPaoIII,				//快跑III（单局达到5000米）
	achieveID_KuaiPaoIV,				//快跑IV（单局达到10000米）

	achieveID_ChiHuoI,					//吃货I（单局到达10000分）
	achieveID_ChiHuoII,					//吃货II（单局到达20000分）
	achieveID_ChiHuoIII,				//吃货III（单局到达30000分）
	achieveID_ChiHuoIV,					//吃货IV（单局到达50000分）

	achieveID_ChiKuangI,				//痴狂I（开始游戏1次）				
	achieveID_ChiKuangII,				//痴狂II（开始游戏10次）
	achieveID_ChiKuangIII,				//痴狂III（开始游戏50次）
	achieveID_ChiKuangIV,				//痴狂IV（开始游戏100次）

	achieveID_ShouCangI,				//收藏I（拥有2个角色）				
	achieveID_ShouCangII,				//收藏II（拥有2个宠物）
	achieveID_ShouCangIII,				//收藏III（拥有3个角色）
	achieveID_ShouCangIV,				//收藏IV（拥有3个宠物）

	achieveID_JianChiI,					//坚持I（累计复活10次）				
	achieveID_JianChiII,				//坚持II（累计复活20次）
	achieveID_JianChiIII,				//坚持III（累计复活30次）
	achieveID_JianChiIV,				//坚持IV（累计复活50次）

	achieveID_FangYuI,					//防御I（累计使用10次护盾）				
	achieveID_FangYuII,					//防御II（累计使用10次护盾）
	achieveID_FangYuIII,				//防御III（累计使用10次护盾）
	achieveID_FangYuIV,					//防御IV（累计使用10次护盾）

	achieveID_ZhanDouI,					//战斗I（战斗力达到6）				
	achieveID_ZhanDouII,				//战斗II（战斗力达到12）
	achieveID_ZhanDouIII,				//战斗III（战斗力达到16）
	achieveID_ZhanDouIV,				//战斗IV（战斗力达到24）

	achieveID_ShaShouI,					//杀手I（累计击败任意BOSS 10次）				
	achieveID_ShaShouII,				//杀手II（累计击败任意BOSS 30次）
	achieveID_ShaShouIII,				//杀手III（累计击败任意BOSS 50次）
	achieveID_ShaShouIV,				//杀手IV（累计击败任意BOSS 100次）
};

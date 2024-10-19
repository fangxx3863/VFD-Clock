#ifndef __GFX_H__
#define __GFX_H__
#include "main.h"

#define ICON_REC 0x00000200
#define ICON_CLOCK 0x00000400
#define ICON_3D 0x00000800
#define ICON_WIFI 0x00001000

#define ICON_G1_ALL 0xFFE1FE
#define ICON_G1_STYLE_1 0x010000
#define ICON_G1_STYLE_2 0x040000
#define ICON_G1_STYLE_3 0x080000
#define ICON_G1_STYLE_4 0x100000
#define ICON_G1_STYLE_5 0x200000
#define ICON_G1_STYLE_6 0x400000
#define ICON_G1_STYLE_7 0x800000
#define ICON_G1_STYLE_8 0x020000
#define ICON_G1_STYLE_9 0x000100

#define ICON_NONE 0  // 清空所有ICON显示

// VFD位数
#define VFD_DIG_LEN 6

/**
 * 初始化
 */
void VFD_GfxInit();

/**
 * 停止关闭显示、灯丝将停止驱动
 */
void VFD_GfxStop();

/**
 * 清空VFD屏幕显示,循环刷新如果使用VFD_GfxSet_text方法不需要使用它。
 */
void VFD_GfxClear();

/**
 * 在指定位置显示一个char字符,index从1~6
 */
void VFD_GfxSetOneText(size_t index, char oneChar);

/**
 * 显示一串文字，从0位开始。
 * (自动清空覆盖显示，方便每次不用调用clear防止闪屏出现)
 */
uint8_t VFD_GfxSetText(const char* string);

/**
 * 要点亮的ICON图标，宏定义传参
 * @param is_save_state 是否保存本次ICON图标到变量中
 */
void VFD_GfxSetIcon(uint32_t buf, uint8_t is_save_state);

/**
 * 获取保存的icon图标
 */
uint32_t VFD_GfxGetSaveIcon(void);

/**
 * 背光开关
 */
void VFD_GfxSetBck(uint8_t onOff);

/**
 * 设置亮度等级 1~7
 */
void VFD_GfxSetBlkLevel(size_t level);

/**
 * 第一位冒号，参数bool类型
 */
void VFD_GfxSetColon1(uint8_t open);

/**
 * 第二位冒号，参数bool类型
 */
void VFD_GfxSetColon2(uint8_t open);

/**
 * 循环滚动展示所有文字,可显示任意长字符内容
 * @param string 要展示的内容字符串
 * @param delay_ms 循环展示刷新频率单位 Ms
 * @param loop_count 循环播放的次数
 **/
void VFD_GfxSetLongText(const char* string, uint32_t delay_ms, size_t loop_count);
/**
 * 如果调用VFD_GfxSet_long_text有存在循环播放未结束的调用此方法将会强制结束
 */
void VFD_GfxCancelLongText();

/**
 * 获取字库值，内部用
 */
uint32_t gui_get_font(char c);

/**
 * G1动画特效
 * @param isAuto 是否自动完成触发
 * 1自动循环执行播放,0.外部给时钟触发(方法调用一次执行动画一帧)
 */
void VFD_GfxAnnoForG1(uint8_t isAuto);

#endif // !__GFX_H__
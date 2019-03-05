/** 
* @file    hci_hwr.h 
* @brief   HCI_HWR SDK 公共头文件  
*/  

#ifndef __HCI_HWR_HEADER__
#define __HCI_HWR_HEADER__

#include "hci_sys.h"

#ifdef __cplusplus
extern "C"
{
#endif

	/** @defgroup HCI_HWR 灵云HWR能力API */
	/* @{ */

    /** @defgroup HCI_HWR_STRUCT  结构体 */
    /* @{ */
    
    /**
	*@brief	模块名称
	*/
    #define HWR_MODULE    "HCI_HWR"

	/**
	* @brief	HWR识别候选结果条目
	*/
	typedef struct _tag_HWR_RECOG_RESULT_ITEM 
	{
		/// 每个字对应的笔迹位置，pusPointOffset[0]表示第一个字的开始位置(单字能力和笔势识别时此值无效)
		unsigned short *	pusPointOffset;

		/// 笔迹位置的个数，也即本结果中有多少个字(单字能力和笔势识别时此值无效)
		unsigned int		uiOffsetCount;

		/// 候选结果字符串，UTF-8编码，以'\0'结束(笔势识别时该字符串表示笔势识别结果的索引值，见@ref gestures_list )
		char *				pszResult;
	} HWR_RECOG_RESULT_ITEM;


	/**
	*	@brief	HWR识别函数的返回结果
	*/
	typedef struct _tag_HWR_RECOG_RESULT 
	{  
		/// 识别候选结果列表
		HWR_RECOG_RESULT_ITEM *	psResultItemList;

		/// 识别候选结果的数目
		unsigned int		uiResultItemCount;
	} HWR_RECOG_RESULT;

	/**
	* @brief	上传的HWR确认结果信息
	*/
	typedef struct _tag_HWR_CONFIRM_ITEM
	{
		/// 确认的识别结果内容，以'\0'结束(笔势识别时，请传入笔势的索引值，同样以'\0'结束)
		char * pszText;
	} HWR_CONFIRM_ITEM;

	/**
	* @brief	拼音条目
	*/
	typedef struct _tag_PINYIN_RESULT_ITEM
	{
		/// 拼音串，UTF-8编码，以'\0'结束
		char * pszPinyin;
	} PINYIN_RESULT_ITEM;

	/**
	* @brief	查询拼音函数的返回结果，若为多音字，则会包含多个结果条目
	*/
	typedef struct _tag_PINYIN_RESULT
	{
		/// 拼音结果列表
		PINYIN_RESULT_ITEM * pItemList;

		/// 拼音结果的数目
		unsigned int uiItemCount;
	} PINYIN_RESULT;

	/**
	* @brief	联想词条目
	*/
	typedef struct _tag_ASSOCIATE_WORDS_RESULT_ITEM
	{
		/// 联想词，UTF-8编码，以'\0'结束，结果不包含输入的字符串
		char * pszWord;
	} ASSOCIATE_WORDS_RESULT_ITEM;

	/**
	* @brief	查询联想词函数的返回结果
	*/
	typedef struct _tag_ASSOCIATE_WORDS_RESULT
	{
		/// 联想词结果列表
		ASSOCIATE_WORDS_RESULT_ITEM * pItemList;

		/// 联想词结果的数目
		unsigned int uiItemCount;
	} ASSOCIATE_WORDS_RESULT;

	/**
	* @brief  笔型结果条目
	*/
	typedef struct _tage_PEN_SCRIPT_RESULT_ITEM
	{
		/// 位图数据标志位
		short * psPageImg;

		/// 位图横坐标
		int x;

		/// 位图纵坐标
		int y;

		/// 位图宽度
		int nWidth;

		/// 位图高度
		int nHeight;

		/// 位图颜色
		unsigned long unPenColor; 
	} PEN_SCRIPT_RESULT_ITEM;

	/**
	* @brief  笔型结果条目
	*/
	typedef struct _tage_PEN_SCRIPT_RESULT
	{
		/// 笔型结果列表
		PEN_SCRIPT_RESULT_ITEM * pItemList;

		/// 笔型结果的数目
		unsigned int uiItemCount;
	} PEN_SCRIPT_RESULT;

    /* @} */

    /** @defgroup HCI_HWR_FUNC  函数 */
    /* @{ */

	/**  
	* @brief	灵云HWR能力 初始化
	* @param	pszConfig		初始化配置串,ASCII字符串，以'\0'结束
	* @return
	* @n
	*	<table>
	*		<tr><td>@ref HCI_ERR_NONE</td><td>操作成功</td></tr>
	*		<tr><td>@ref HCI_ERR_SYS_NOT_INIT</td><td>HCI SYS 尚未初始化</td></tr>
	*		<tr><td>@ref HCI_ERR_HWR_ALREADY_INIT</td><td>HWR重复初始化</td></tr>
	*		<tr><td>@ref HCI_ERR_CONFIG_INVALID</td><td>配置项非法</td></tr>
	*		<tr><td>@ref HCI_ERR_CONFIG_DATAPATH_MISSING</td><td>配置中有initCapkeys却没有dataPath</td></tr>
	*		<tr><td>@ref HCI_ERR_CONFIG_CAPKEY_NOT_MATCH</td><td>输入的不是HWR的能力KEY</td></tr>
	*		<tr><td>@ref HCI_ERR_CAPKEY_NOT_FOUND</td><td>传入的能力key未找到</td></tr>
	*		<tr><td>@ref HCI_ERR_HWR_ENGINE_INIT_FAILED</td><td>本地引擎初始化失败</td></tr>
	*		<tr><td>@ref HCI_ERR_LOCAL_LIB_MISSING</td><td>本地引擎缺少字典</td></tr>
	*	</table>
	*
	* @par 配置串定义：
	* 配置串是由"字段=值"的形式给出的一个字符串，多个字段之间以','隔开。字段名不分大小写。本配置串用于本地识别的字典载入和配置。
	* 若不需要使用本地能力，此配置串可以传NULL或者""。
	* @n@n
	*	<table>
	*		<tr>
	*			<td><b>字段</b></td>
	*			<td><b>取值或示例</b></td>
	*			<td><b>缺省值</b></td>
	*			<td><b>含义</b></td>
	*			<td><b>详细说明</b></td>
	*		</tr>
	*		<tr>
	*			<td>dataPath</td>
	*			<td>字符串，如：./data</td>
	*			<td>无</td>
	*			<td>手写识别本地资源所在路径</td>
	*			<td>如果不使用本地手写识别能力，可以不传此项</td>
	*		</tr>
	*		<tr>
	*			<td>initCapKeys</td>
	*			<td>字符串，参考 @ref hci_hwr_page </td>
	*			<td>无</td>
	*			<td>初始化所需的本地能力</td>
	*			<td>多个能力以';'隔开，忽略传入的云端能力key。如果不使用本地识别能力，可以不传此项</td>
	*		</tr>
	*		<tr>
	*			<td>fileFlag</td>
	*			<td>字符串，有效值{none, android_so}</td>
	*			<td>none</td>
	*			<td>获取本地文件名的特殊标记</td>
	*			<td>参见下面的注释</td>
	*		</tr>
	*	</table>
	*
	*  <b>Android特殊配置</b>
	*  @n
	*  当fileFlag为android_so时，加载本地资源文件(字典和领域库)时会将正常的库文件名更改为so文件名进行加载。
	*  例如，当使用的库为file.dat时，则实际打开的文件名为libfile.dat.so，这样在Android系统下，
	*  开发者可以按照此规则将本地资源改名后, 放在libs目录下打包入apk。在安装后，这些资源文件
	*  就会放置在/data/data/包名/lib目录下。则可以直接将dataPath配置项指为此目录即可。
	*/ 
	HCI_ERR_CODE HCIAPI hci_hwr_init(
		_MUST_ _IN_ const char * pszConfig
		);

	/**  
	* @brief	开始会话
	* @param	pszConfig		会话配置串,ASCII字符串，以'\0'结束
	* @param	pnSessionId		成功时返回会话ID
	* @return
	* @n
	*	<table>
	*		<tr><td>@ref HCI_ERR_NONE</td><td>操作成功</td></tr>
	*		<tr><td>@ref HCI_ERR_HWR_NOT_INIT</td><td>HCI HWR尚未初始化</td></tr>
	*		<tr><td>@ref HCI_ERR_PARAM_INVALID</td><td>传入的参数不合法</td></tr>
	*		<tr><td>@ref HCI_ERR_CONFIG_CAPKEY_MISSING</td><td>缺少必需的capKey配置项</td></tr>
	*		<tr><td>@ref HCI_ERR_CONFIG_INVALID</td><td>配置串中的值不合法</td></tr>
	*		<tr><td>@ref HCI_ERR_CONFIG_CAPKEY_NOT_MATCH</td><td>输入的不是HWR的能力KEY</td></tr>
	*		<tr><td>@ref HCI_ERR_CONFIG_UNSUPPORT</td><td>不支持传入的配置</td></tr>
	*		<tr><td>@ref HCI_ERR_TOO_MANY_SESSION</td><td>创建的Session数量超出限制(256)</td></tr>
	*		<tr><td>@ref HCI_ERR_CAPKEY_NOT_FOUND</td><td>传入的能力key未找到</td></tr>
	*		<tr><td>@ref HCI_ERR_URL_MISSING</td><td>未找到有效的云服务地址</td></tr>
	*		<tr><td>@ref HCI_ERR_HWR_ENGINE_SESSION_START_FAILED</td><td>本地引擎启动会话失败</td></tr>
	*		<tr><td>@ref HCI_ERR_LOCAL_LIB_MISSING</td><td>本地引擎缺少字典</td></tr>
	*		<tr><td>@ref HCI_ERR_LOAD_FUNCTION_FROM_DLL</td><td>要载入的模块不存在，或者需要的功能在该模块不存在</td></tr>
	*		<tr><td>@ref HCI_ERR_CONFIG_DATAPATH_MISSING</td><td>缺少必需的dataPath配置项</td></tr>
	*	</table>
	*
	* @par 配置串定义：
	* 配置串是由"字段=值"的形式给出的一个字符串，多个字段之间以','隔开。字段名不分大小写。
	* @n@n
	*	<table>
	*		<tr>
	*			<td><b>字段</b></td>
	*			<td><b>取值或示例</b></td>
	*			<td><b>缺省值</b></td>
	*			<td><b>含义</b></td>
	*			<td><b>详细说明</b></td>
	*		</tr>
	*		<tr>
	*			<td>capKey</td>
	*			<td>字符串，参考 @ref hci_hwr_page </td>
	*			<td>无</td>
	*			<td>手写识别能力key</td>
	*			<td>此项必填。每个session只能定义一种能力，并且过程中不能改变。</td>
	*		</tr>
    *		<tr>
    *			<td>resPrefix</td>
    *			<td>用户自定义的字符串，如："temp_"，<br/>
	*				比如所需资源文件为a.dic,添加resPrefix后需要将其改为temp_a.dic<br/>
	*				参考 @ref hci_hwr_page 本地端部分</td>
    *			<td>无</td>
    *			<td>资源加载前缀</td>
    *			<td>如果不同会话需要使用同一路径下资源时，可以使用该字段对统一路径下的资源进行区分。如temp1_wa.system.dct和temp2_wa.system.dct</td>
    *		</tr>
	*		<tr>
	*			<td>userDataPath</td>
	*			<td>字符串，联想用户字典保存路径(具体到文件名)</td>
	*			<td>默认使用授权文件路径和默认文件名，授权文件路径参考\ref hci_init</td>
	*			<td>用户字典保存路径</td>
	*			<td>hwr.local.associateword能力需要此参数，用于存储用户自造词文件，此目录必须有可读写权限</td>
	*		</tr>
	*		<tr>
	*			<td>realtime</td>
	*			<td>字符串，有效值{no, yes}</td>
	*			<td>no</td>
	*			<td>是否启动实时识别，仅本地多字手写能力支持(hwr.local.freestylus.v7暂不支持)</td>
	*			<td>参见 hci_hwr_recog() 函数的注释</td>
	*		</tr>
	*	</table>
	* @n@n
	* 另外，这里还可以传入识别的配置项(除了subLang)，作为默认配置项。参见 hci_hwr_recog() , hci_hwr_pen_script() , hci_hwr_associate_words() , hci_hwr_pinyin() 。
	*/ 
	HCI_ERR_CODE HCIAPI hci_hwr_session_start(
		_MUST_ _IN_ const char * pszConfig,
		_MUST_ _OUT_ int * pnSessionId
		);

	/**  
	* @brief	识别函数
	* @param	nSessionId			会话ID
	* @param	psStrokingData		要识别的笔迹数据，最大可传入64KB。由若干坐标点组成，每个坐标点形式为（x，y），x 和 y 都是short类型，
	*								有效值是0~32767。有两个分隔符是特例，(-1，0)是一个笔画结束的标记，(-1, -1) 是笔迹结束的标记
	* @param	uiStrokingDataLen	要识别的笔迹数据长度, 字节为单位
	* @param	pszConfig			识别参数配置串,ASCII字符串，以'\0'结束，可为NULL
	* @param	psHwrRecogResult	识别结果，使用完成后，需使用 hci_hwr_free_recog_result() 函数进行释放
	* @return
	* @n
	*	<table>
	*		<tr><td>@ref HCI_ERR_NONE</td><td>操作成功</td></tr>
	*		<tr><td>@ref HCI_ERR_HWR_NOT_INIT</td><td>HCI HWR尚未初始化</td></tr>
	*		<tr><td>@ref HCI_ERR_PARAM_INVALID</td><td>传入的参数不合法</td></tr>
	*		<tr><td>@ref HCI_ERR_DATA_SIZE_TOO_LARGE</td><td>传入的点数超过可处理的上限</td></tr>
	*		<tr><td>@ref HCI_ERR_CONFIG_INVALID</td><td>配置串中的值不合法</td></tr>
	*		<tr><td>@ref HCI_ERR_SESSION_INVALID</td><td>传入的Session非法</td></tr>
	*		<tr><td>@ref HCI_ERR_SYS_NOT_INIT</td><td>HCI SYS未初始化</td></tr>
	*		<tr><td>@ref HCI_ERR_CAPKEY_NOT_FOUND</td><td>传入的能力key未找到</td></tr>
	*		<tr><td>@ref HCI_ERR_CONFIG_UNSUPPORT</td><td>不支持传入的配置</td></tr>
	*		<tr><td>@ref HCI_ERR_HWR_ENGINE_FAILED</td><td>本地引擎识别失败(引擎返回结果为空也被认为是失败)</td></tr>
	*		<tr><td>@ref HCI_ERR_URL_MISSING</td><td>未找到有效的云服务地址</td></tr>
	*		<tr><td>@ref HCI_ERR_SERVICE_CONNECT_FAILED</td><td>连接服务器失败，服务器无响应</td></tr>
	*		<tr><td>@ref HCI_ERR_SERVICE_TIMEOUT</td><td>服务器访问超时</td></tr>
	*		<tr><td>@ref HCI_ERR_SERVICE_DATA_INVALID</td><td>服务器返回的数据格式不正确</td></tr>
	*		<tr><td>@ref HCI_ERR_SERVICE_RESPONSE_FAILED</td><td>服务器返回识别失败</td></tr>
	*		<tr><td>@ref HCI_ERR_UNSUPPORT</td><td>暂不支持</td></tr>
	*		<tr><td>@ref HCI_ERR_HWR_CONFIG_SUBLANG_MISSING</td><td>多语种能力时未传入subLang配置</td></tr>
	*	</table>
	*
	* @par 配置串定义：
	* 配置串是由"字段=值"的形式给出的一个字符串，多个字段之间以','隔开。字段名不分大小写。
	* @n@n
	*	以下配置，与能力相关，并非所有的能力都支持，参见 @ref hci_hwr_page 。具体使用上可咨询捷通华声公司。
	*	<table>
	*		<tr>
	*			<td><b>字段</b></td>
	*			<td><b>取值或示例</b></td>
	*			<td><b>缺省值</b></td>
	*			<td><b>含义</b></td>
	*			<td><b>详细说明</b></td>
	*		</tr>
	*		<tr>
	*			<td>candNum</td>
	*			<td>正整数，范围[1,10]</td>
	*			<td>10</td>
	*			<td>识别候选结果个数</td>
	*			<td></td>
	*		</tr>
    *		<tr>
    *			<td>subLang</td>
    *			<td>字符串，有效值参见@ref latin_sublang_list , @ref cyrillic_sublang_list ,@ref arabic_sublang_list </td>
    *			<td>无</td>
    *			<td>多语种能力时选择语言，只能传入1个</td>
    *			<td>当使用拉丁语系，西里尔语系，阿拉伯语系时，需设置此项</td>
    *		</tr>
	*		<tr>
	*			<td>recogRange</td>
	*			<td>字符串，有效值参见 @ref common_recogrange ,@ref lang_list </td>
	*			<td>其他语言在能力列表中给出</td>
	*			<td>识别范围</td>
	*			<td>特别的，若传入非法参数，返回22暂不支持，而不是3参数错误</td>
	*		</tr>
	*		<tr>
	*			<td>openSlant</td>
	*			<td>字符串，有效值{no, yes}</td>
	*			<td>no</td>
	*			<td>倾斜矫正开关</td>
	*			<td>no: 不做倾斜矫正<br/>yes: 做倾斜矫正<br/></td>
	*		</tr> 
	*		<tr>
	*			<td>splitMode</td>
	*			<td>字符串，有效值{line, overlap}<br/>
	*				注意：目前英文只支持line</td>
	*			<td>line</td>
	*			<td>设置书写模式</td>
	*			<td>line: 行识别<br/>overlap: 叠字识别</td>
	*		</tr> 
	*		<tr>
	*			<td>wordMode</td>
	*			<td>字符串，有效值{mixed, capital, lowercase, initial}</td>
	*			<td>mixed</td>
	*			<td>设置英文单词的大小写形式<br/>
	*				目前hwr.local.freestylus.v7能力无此项配置</td>
	*			<td>mixed: 字母大小写混合<br/>
	*			    capital: 全部字母大写<br/>
	*			    lowercase: 全部字母小写<br/>
	*			    initial: 首字母大写
	*			</td>
	*		</tr>
	*		<tr>
	*			<td>dispCode</td>
	*			<td>字符串，有效值{nochange, tosimplified, totraditional}</td>
	*			<td>nochange</td>
	*			<td>设置输出结果简繁体转换</td>
	*			<td>nochange: 简繁体不做变化<br/>
    *               tosimplified: 写繁得简<br/>
    *               totraditional: 写简得繁</td>
	*		</tr> 
	*		<tr>
	*			<td>fullHalf</td>
	*			<td>字符串，有效值{full, half}</td>
	*			<td>half</td>
	*			<td>设置中文识别模式下输出符号是全角还是半角</td>
	*			<td>full: 全角<br/>half: 半角</td>
	*		</tr>
	*	</table>
	* @n@n
	* 这里没有定义的配置项，会使用 session_start 时的定义。如果 session_start 时也没有定义，则使用缺省值。此配置串可以传NULL或者""。
	*
	* @note
	* <b>实时识别(realtime配置值为yes)</b>
	* @n@n
	* 当不启动实时识别时，每次调用本函数时所输入的笔迹数据被认为是完整的数据，
	* 如果不是(-1,0)(-1,-1)结束，会认为数据非法。
	* @n@n
	* 当启用实时识别时，对于每次连续的识别内容，可以多次调用本函数，每次调用追加输入新的数据，每次输入的数据
	* 以(-1,0)结束，也即每次输入的笔画是完整的，可以一次输入多个笔画。最后一次以(-1,0)(-1,-1)结束，
	* 表示整次识别结束。每次调用本函数都会返回从头开始的完整结果，新输入的数据会导致切分发生变化，
	* 因此后一次结果不一定是前次结果再追加字符，可能会更改掉部分前次结果。实时识别中每次识别的结果都需要释放。
	* @n@n
	* 实时识别只使用每次新开启时的pszConfig配置，在实时识别过程中再传入的pszConfig总是会被忽略。
	*/ 
	HCI_ERR_CODE HCIAPI hci_hwr_recog(
		_MUST_ _IN_ int nSessionId,
		_MUST_ _IN_ short * psStrokingData,
		_MUST_ _IN_ unsigned int uiStrokingDataLen,
		_OPT_ _IN_ const char * pszConfig,
		_MUST_ _OUT_ HWR_RECOG_RESULT * psHwrRecogResult
		);

	/**  
	* @brief	释放识别结果内存
	* @param	psHwrRecogResult	需要释放的识别结果内存指针
	* @return	
	* @n
	*	<table>
	*		<tr><td>@ref HCI_ERR_NONE</td><td>操作成功</td></tr>
	*		<tr><td>@ref HCI_ERR_PARAM_INVALID</td><td>输入参数不合法</td></tr>
	*	</table>
	*/ 
	HCI_ERR_CODE HCIAPI hci_hwr_free_recog_result(
		_MUST_ _IN_ HWR_RECOG_RESULT * psHwrRecogResult
		);

	/**  
	* @brief	提交确认结果，私有云暂不支持
	* @param	nSessionId			会话ID
	* @param	psHwrConfirmItem	要提交的确认结果,UTF8格式，以'\0'结束，不能超过2048字节（包括'\0'）
	* @return	
	* @n
	*	<table>
	*		<tr><td>@ref HCI_ERR_NONE</td><td>操作成功</td></tr>
	*		<tr><td>@ref HCI_ERR_HWR_NOT_INIT</td><td>HCI HWR尚未初始化</td></tr>
	*		<tr><td>@ref HCI_ERR_PARAM_INVALID</td><td>传入的参数不合法</td></tr>
	*		<tr><td>@ref HCI_ERR_DATA_SIZE_TOO_LARGE</td><td>传入的确认文本超过可处理的上限</td></tr>
	*		<tr><td>@ref HCI_ERR_SESSION_INVALID</td><td>传入的Session非法</td></tr>
	*		<tr><td>@ref HCI_ERR_HWR_CONFIRM_NO_TASK</td><td>没有可用来提交的任务，例如尚未识别就调用了本函数</td></tr>
	*		<tr><td>@ref HCI_ERR_SERVICE_CONNECT_FAILED</td><td>连接服务器失败，服务器无响应</td></tr>
	*		<tr><td>@ref HCI_ERR_SERVICE_TIMEOUT</td><td>服务器访问超时</td></tr>
	*		<tr><td>@ref HCI_ERR_SERVICE_DATA_INVALID</td><td>服务器返回的数据格式不正确</td></tr>
	*		<tr><td>@ref HCI_ERR_UNSUPPORT</td><td>暂不支持</td></tr>
	*		<tr><td>@ref HCI_ERR_SERVICE_RESPONSE_FAILED</td><td>服务器返回识别失败</td></tr>
	*	</table>
	*/ 
	HCI_ERR_CODE HCIAPI hci_hwr_confirm(
		_MUST_ _IN_ int nSessionId,
		_MUST_ _IN_ HWR_CONFIRM_ITEM * psHwrConfirmItem
		);

	/**  
	* @brief	结束会话
	* @param	nSessionId		会话ID
	* @return
	* @n
	*	<table>
	*		<tr><td>@ref HCI_ERR_NONE</td><td>操作成功</td></tr>
	*		<tr><td>@ref HCI_ERR_HWR_NOT_INIT</td><td>HCI HWR尚未初始化</td></tr>
	*		<tr><td>@ref HCI_ERR_SESSION_INVALID</td><td>传入的Session非法</td></tr>
	*	</table>
	*/ 
	HCI_ERR_CODE HCIAPI hci_hwr_session_stop(
		_MUST_ _IN_ int nSessionId
		);

	/**  
	* @brief	获取拼音 （仅Windows平台有效）
	* @param	nSessionId		会话ID
	* @param	pszConfig		获取拼音配置串,ASCII字符串，以'\0'结束
	* @param	pszWord			字符串指针，UTF-8格式，字符串以'\0'结束。如果多于一个汉字，只返回第一个汉字的拼音结果
	* @param	psPinyin		返回的拼音信息，使用完毕后，需使用 hci_hwr_free_pinyin_result() 函数进行释放
	*                          应该将结构内容拷贝出来使用
	* @return	
	* @n
	*	<table>
	*		<tr><td>@ref HCI_ERR_NONE</td><td>操作成功</td></tr>
	*		<tr><td>@ref HCI_ERR_HWR_NOT_INIT</td><td>HCI HWR尚未初始化</td></tr>
	*		<tr><td>@ref HCI_ERR_PARAM_INVALID</td><td>传入的参数不合法</td></tr>
	*		<tr><td>@ref HCI_ERR_LOCAL_LIB_MISSING</td><td>本地拼音字典丢失</td></tr>
	*		<tr><td>@ref HCI_ERR_HWR_ENGINE_INIT_FAILED</td><td>本地引擎初始化失败</td></tr>
	*		<tr><td>@ref HCI_ERR_HWR_ENGINE_FAILED</td><td>本地引擎获取拼音失败</td></tr>
	*		<tr><td>@ref HCI_ERR_LOAD_FUNCTION_FROM_DLL</td><td>要载入的模块不存在，或者需要的功能在该模块不存在</td></tr>
	*	</table>
	*@par 配置串定义：
	* 配置串是由"字段=值"的形式给出的一个字符串，多个字段之间以','隔开。字段名不分大小写。
	* @n@n
	*	以下配置，与能力相关，并非所有的能力都支持，参见 @ref hci_hwr_page 。具体使用上可咨询捷通华声公司。
	*	<table>
	*		<tr>
	*			<td><b>字段</b></td>
	*			<td><b>取值或示例</b></td>
	*			<td><b>缺省值</b></td>
	*			<td><b>含义</b></td>
	*			<td><b>详细说明</b></td>
	*		</tr>
	*		<tr>
	*			<td>pinyinmodel</td>
	*			<td>字符串，有效值{withtone, withouttone}</td>
	*			<td>withtone</td>
	*			<td>控制显示的拼音结果是否标出音调</td>
	*			<td>如果此项为withtone，则拼音结果标出音调。如果此项为withouttone，则拼音结果不标出音调。</td>
	*		</tr>
	*	</table>
	*/ 
	HCI_ERR_CODE HCIAPI hci_hwr_pinyin(
        _MUST_ _IN_ int nSessionId,
        _OPT_ _IN_ const char * pszConfig,
		_MUST_ _IN_ const char * pszWord,
		_MUST_ _OUT_ PINYIN_RESULT * psPinyin
		);

	/**  
	* @brief	释放拼音结果内存
	* @param	psPinyin	需要释放的拼音内存指针
	* @return	
	* @n
	*	<table>
	*		<tr><td>@ref HCI_ERR_NONE</td><td>操作成功</td></tr>
	*		<tr><td>@ref HCI_ERR_PARAM_INVALID</td><td>输入参数不合法</td></tr>
	*	</table>
	*/ 
	HCI_ERR_CODE HCIAPI hci_hwr_free_pinyin_result(
		_MUST_ _IN_ PINYIN_RESULT * psPinyin
		);


	/**  
	* @brief	获取联想词
	* @details	获取联想词的时候会先按照输入串整体进行联想，然后依次从前去除字符进行联想，
	*			例如，输入"中华人民"的时候，会给出"共和国"(按照"中华人民"匹配)，"大学"(按照"人民"匹配），
	*			"族"(按照"民"匹配)等。但总的联想词的累计字数有一定限制。@n
	*			当输入串为全部英文时，会进行英文词的联想功能。
	* @param	nSessionId		会话ID
	* @param	pszConfig		初始化配置串,ASCII字符串，以'\0'结束
	* @param	pszWord			非空的字符串指针，UTF-8格式，以'\0'为结束符。
	* @param	psAssocWords	返回的联想词结构，使用完毕后，需使用 hci_hwr_free_associate_words_result() 函数进行释放
	* @return	
	* @n
	*	<table>
	*		<tr><td>@ref HCI_ERR_NONE</td><td>操作成功</td></tr>
	*		<tr><td>@ref HCI_ERR_HWR_NOT_INIT</td><td>HCI HWR尚未初始化</td></tr>
	*		<tr><td>@ref HCI_ERR_PARAM_INVALID</td><td>传入的参数不合法</td></tr>
	*		<tr><td>@ref HCI_ERR_HWR_ENGINE_FAILED</td><td>本地引擎获取联想词失败</td></tr>
	*		<tr><td>@ref HCI_ERR_SESSION_INVALID</td><td>传入的Session非法</td></tr>
	*		<tr><td>@ref HCI_ERR_CONFIG_INVALID</td><td>配置串中的值不合法</td></tr>
	*		<tr><td>@ref HCI_ERR_UNSUPPORT</td><td>暂不支持</td></tr>
	*		<tr><td>@ref HCI_ERR_LOAD_FUNCTION_FROM_DLL</td><td>要载入的模块不存在，或者需要的功能在该模块不存在</td></tr>
	*	</table>
	*@par 配置串定义：
	* 配置串是由"字段=值"的形式给出的一个字符串，多个字段之间以','隔开。字段名不分大小写。
	* @n@n
	*	以下配置，与能力相关，并非所有的能力都支持，参见 @ref hci_hwr_page 。具体使用上可咨询捷通华声公司。
	*	<table>
	*		<tr>
	*			<td><b>字段</b></td>
	*			<td><b>取值或示例</b></td>
	*			<td><b>缺省值</b></td>
	*			<td><b>含义</b></td>
	*			<td><b>详细说明</b></td>
	*		</tr>
	*		<tr>
	*			<td>associateModel</td>
	*			<td>字符串，有效值{multi, single}</td>
	*			<td>multi</td>
	*			<td>控制联想结果是多字还是单字，该配置只支持中文联想，英文联想忽略</td>
	*			<td>如果此项为multi，则不会对联想结果词的长度进行限制。如果此项为single，则表示限定联想词返回结果为单字。</td>
	*		</tr>
	*		<tr>
	*			<td>recursive</td>
	*			<td>字符串，有效值{yes, no}</td>
	*			<td>yes</td>
	*			<td>是否递归联想。该配置中文联想时有效，英文联想忽略</td>
	*			<td>设为yes时，首先对整个字段进行联想，然后从开头逐字去掉，对剩余部分联想</td>
	*		</tr>
	*	</table>
	*/ 
	HCI_ERR_CODE HCIAPI hci_hwr_associate_words(
		_MUST_ _IN_ int nSessionId,
		_OPT_ _IN_ const char * pszConfig,
		_MUST_ _IN_ const char * pszWord,
		_MUST_ _OUT_ ASSOCIATE_WORDS_RESULT * psAssocWords
		);


	/**  
	* @brief	联想词动态调整，提前pszWord的出现位置
	* @details	支持中文的联想词动态调整，若输入的是词库中已有的词，则会使其出现位置提前； 若输入的是词库中没有
	*			的词，则插入词库，存于字典内的预留空间，字典大小不变，预留空间写满后较早插入的新词将被覆盖。@n
	*			不支持英文联想词动态调整，当输入串为全部英文时，返回失败。@n
	*			输入举例：输入"人民"进行联想，有对应"人民"的联想结果"大学"，想使这个结果靠前，此处应该提高
	*			"人民大学"的词频，即pszWord应该是"人民大学"对应的UTF-8字符串，以'\0'结束。
	* @param	nSessionId		会话ID
	* @param	pszConfig		初始化配置串,ASCII字符串，以'\0'结束，保留接口，暂时无配置项
	* @param	pszWord			字符串指针，UTF-8格式，以'\0'为结束符，最少2个字符，最多15个字符(注意：不是15个字节)。
	* @return	
	* @n
	* @n
	*	<table>
	*		<tr><td>@ref HCI_ERR_NONE</td><td>操作成功</td></tr>
	*		<tr><td>@ref HCI_ERR_HWR_NOT_INIT</td><td>HCI HWR尚未初始化</td></tr>
	*		<tr><td>@ref HCI_ERR_PARAM_INVALID</td><td>传入的参数不合法</td></tr>
	*		<tr><td>@ref HCI_ERR_DATA_SIZE_TOO_LARGE</td><td>传入的文本超过15个字符</td></tr>
	*		<tr><td>@ref HCI_ERR_HWR_ENGINE_FAILED</td><td>联想词动态调整失败</td></tr>
	*		<tr><td>@ref HCI_ERR_UNSUPPORT</td><td>暂不支持</td></tr>
	*		<tr><td>@ref HCI_ERR_SESSION_INVALID</td><td>传入的Session非法</td></tr>
	*		<tr><td>@ref HCI_ERR_LOAD_FUNCTION_FROM_DLL</td><td>要载入的模块不存在，或者需要的功能在该模块不存在</td></tr>
	*	</table>
	*/ 
	HCI_ERR_CODE HCIAPI hci_hwr_associate_words_adjust(
		_MUST_ _IN_ int nSessionId,
		_OPT_ _IN_ const char * pszConfig,
		_MUST_ _IN_ const char * pszWord
		);

	/**  
	* @brief	释放联想词结果内存
	* @param	psAssocWords	需要释放的联想词内存指针
	* @return	
	* @n
	*	<table>
	*		<tr><td>@ref HCI_ERR_NONE</td><td>操作成功</td></tr>
	*		<tr><td>@ref HCI_ERR_PARAM_INVALID</td><td>输入参数不合法</td></tr>
	*	</table>
	*/ 
	HCI_ERR_CODE HCIAPI hci_hwr_free_associate_words_result(
		_MUST_ _IN_ ASSOCIATE_WORDS_RESULT * psAssocWords
		);


	/**  
	* @brief	获取笔型
	* @details	传入笔迹点获取笔型位图。
	*			此函数会根据前后两次传入的点的位置关系进行位图的生成，如果此次传入的坐标点是第一个传入的坐标点，或者与之前传入的坐标点
	*			完全相同则不会生成位图结果。
	*			每一笔结束的时候要传入(-1,0),结束本次笔型生成。
	*			此函数的配置项只在两种情况下生效：(1)第一次调用此函数；(2)传入结束笔迹标记(-1,0)后下一次调用此函数。
	*			
	* @param	nSessionId		会话ID
	* @param	pszConfig		识别参数配置串,ASCII字符串，以'\0'结束，可为NULL
	* @param	nX				笔迹点的横坐标 取值>= 0或者 -1
	* @param	nY				笔迹点的纵坐标取值 >= 0
	* @param	psPenScript		返回的笔型结果结构，使用完毕后，需使用 hci_hwr_free_pen_script_result() 函数进行释放
	* @return	
	* @n
	*	<table>
	*		<tr><td>@ref HCI_ERR_NONE</td><td>操作成功</td></tr>
	*		<tr><td>@ref HCI_ERR_HWR_NOT_INIT</td><td>HCI HWR尚未初始化</td></tr>
	*		<tr><td>@ref HCI_ERR_PARAM_INVALID</td><td>传入的参数不合法</td></tr>
	*		<tr><td>@ref HCI_ERR_CONFIG_INVALID</td><td>配置串中的值不合法</td></tr>
	*		<tr><td>@ref HCI_ERR_HWR_ENGINE_INIT_FAILED</td><td>初始化笔形库失败</td></tr>
	*		<tr><td>@ref HCI_ERR_HWR_ENGINE_FAILED</td><td>本地引擎获取笔型失败</td></tr>
	*		<tr><td>@ref HCI_ERR_LOAD_FUNCTION_FROM_DLL</td><td>要载入的模块不存在，或者需要的功能在该模块不存在</td></tr>
	*	</table>
	* @par 配置串定义：
	* 配置串是由"字段=值"的形式给出的一个字符串，多个字段之间以','隔开。字段名不分大小写。
	* @n@n
	*	以下配置，与能力相关，并非所有的能力都支持，参见 @ref hci_hwr_page 。具体使用上可咨询捷通华声公司。
	*	<table>
	*		<tr>
	*			<td><b>字段</b></td>
	*			<td><b>取值或示例</b></td>
	*			<td><b>缺省值</b></td>
	*			<td><b>含义</b></td>
	*			<td><b>详细说明</b></td>
	*		</tr>
	*		<tr>
	*			<td>penMode</td>
	*			<td>字符串，有效值{pencil, pen, brush}</td>
	*			<td>pencil</td>
	*			<td>设置笔型模式</td>
	*			<td>pencil: 铅笔<br/>pen: 钢笔<br/>brush: 毛笔</td>
	*		</tr>
	*		<tr>
	*			<td>penColor</td>
	*			<td>字符串，有效值{[#000000,#FFFFFF],rainbow}</td>
	*			<td>rainbow</td>
	*			<td>设置笔迹颜色</td>
	*			<td>RGB颜色值，rainbow为彩色，其他值为单色</td>
	*		</tr>
	*		<tr>
	*			<td>penWidth</td>
	*			<td>正整数，范围[1-15]</td>
	*			<td>1</td>
	*			<td>设置笔宽</td>
	*			<td></td>
	*		</tr> 
	*		<tr>
	*			<td>penSpeed</td>
	*			<td>正整数，范围[1,10]</td>
	*			<td>1</td>
	*			<td>设置笔速，仅对毛笔有效</td>
	*			<td>通常情况下使用默认笔速即可，如果设备屏幕能够取到的点过于密集，影响到了笔形库的效率可以适当调大笔速(笔速的机制是通过舍弃部分点来实现的，笔速越大被忽略的点越多)。</td>
	*		</tr> 
	*	</table>
	*
	* @note
	* 此接口不支持多线程操作,nX的取值范围大于等于-1的整数(nX取值-1的时候，nY的值只能是0)，nY的取值范围是大于等于0的整数
	* @n@n
	*/ 
	HCI_ERR_CODE HCIAPI hci_hwr_pen_script(
        _MUST_ _IN_ int nSessionId,
		_OPT_ _IN_ const char *pszConfig,
		_MUST_ _IN_ int nX,
		_MUST_ _IN_ int nY,
		_MUST_ _IN_ PEN_SCRIPT_RESULT * psPenScript
		);

	/**  
	* @brief	释放笔型结果内存
	* @param	psPenScript		需要释放的笔型结果内存指针
	* @return	
	* @n
	*	<table>
	*		<tr><td>@ref HCI_ERR_NONE</td><td>操作成功</td></tr>
	*		<tr><td>@ref HCI_ERR_PARAM_INVALID</td><td>输入参数不合法</td></tr>
	*	</table>
	*/ 
	HCI_ERR_CODE HCIAPI hci_hwr_free_pen_script_result(
		_MUST_ _IN_ PEN_SCRIPT_RESULT * psPenScript
		);

	/**  
	* @brief	灵云HWR能力 反初始化
	* @return
	* @n
	*	<table>
	*		<tr><td>@ref HCI_ERR_NONE</td><td>操作成功</td></tr>
	*		<tr><td>@ref HCI_ERR_HWR_NOT_INIT</td><td>HCI HWR尚未初始化</td></tr>
	*		<tr><td>@ref HCI_ERR_ACTIVE_SESSION_EXIST</td><td>尚有未stop的Sesssion，无法结束</td></tr>
	*	</table>
	*/ 
	HCI_ERR_CODE HCIAPI hci_hwr_release();

    /* @} */

	/* @} */
	//////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
};
#endif


#endif

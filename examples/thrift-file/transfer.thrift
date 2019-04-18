

/*
    当 userType 为 -1的时候，user 填入的是 userID(用户ID，以字符串的形式填入)
    pwd 应该填入你的票，为之前返回的ticket, 此时， tick 字段忽略
    一旦票不正确，应该改为密码登陆
*/
struct LoginParam {
    1:  byte    userType            // 帐号类型 1：手机号 2：第三方关联,3：邮箱 4：用户ID,6：豆豆号，7:使用自定义拓展字段来进行登陆 -1：传用户ID但票验证 100:湖北公安第三方认证
    2:  string  user
    3:  string  pwd
    4:  string  macAddr             // 必填不能为空
    5:  byte    deviceType          // 设备类型 1：PC；2：手机；3：pad；4：网页

    16: optional byte   tick        // 踢人策略 1: 强制踢(登录) 2: 不踢人(重连)
    17: optional string deviceInfo
    18: optional byte   status      /*TODO(H.) 该字段将会替换 PluginParam, reportPluginStatus 将会被缺消 */

    20: optional byte status_flag   // -1：离线，1:在线；2:隐身,3:忙碌,4:离开,5:请勿打扰,6:想聊天, 默认在线
    21: optional string clientIP    //客户端IP
    22: optional string version     // 版本号
    23: optional i64 sdkid          // SDKID 默认为0，注意：开放平台应用请忽略该字段
    24: optional string markSign    // 注意：对于正常应用忽略该字段，如果是开放平台应用则填入标识
    25: optional string redirectUri //auth回调地址
    200: optional string ip        // 客户端不要使用该字段
}

struct LoginResult {
    1: i32 code
    2: optional string what         // 可能保存了image信息

    3: optional i32 sessionTimeout // session失效时间(秒),请在这段时间内关闭连接重新发送空请求

    4: optional i64    sessionID   // 每次登录临时分配一ID
    5: optional i64    userID
    6: optional i64    serverTime  // 服务器当前时间

    7: optional string privateKey
    8: optional string ticket     // 对于pc端跳转网页可以直接使用此票
    9: optional string area       // 返回区域码
    10: optional string captcha   //验证码，当需要验证码登录时会填充该字段
    11: optional i64 remainLockTime;     //剩余锁定时长 ，毫秒
}

struct SendEmptyRequestReq {
      1: i64 sessionID;
      2: string ticket;
      3: i64 notifyID;
      4: optional i32 timeout;  //提前返回的时间，单位：秒
      5: optional i32 nextReqInterval;  //如果收到了ap的推送，发送下一次空请求的间隔时间，注意前提
}

struct OneFile {
	1: string name,
	2: string file_type,
	3: binary file_buffer,
	4: i64 file_size,
	5: string file_hsh,
}



struct OneFileDeriveEx {
        1: OneFile one_file_obj;
        2: string str_extent_1,
        3: i32 int_extent_1,
}




service Photo {

      /// 登陆接口
      /// macAddr,version字段必填
      LoginResult login_im(1: LoginParam param)

      bool SendPhoto(1: OneFile onefile)

      i32  Add(1: i32 add1, 2: i32 add2 )

      /* 以下为系统通信消息,不为应用层所使用*/
      oneway void SendEmptyRequest(1:SendEmptyRequestReq req);

}




/**
 * 演示程序当前的 “注册/登录” 等操作，是基于 “本地存储” 完成的
 * 当您要参考这个演示程序进行相关 app 开发时，
 * 请注意将相关方法调整成 “基r于服务端Service” 的实现。
 **/
// var SERVER = "http://blueheart00001.java.cdnjsp.wang";

var SERVER 			= "http://202.200.179.58:8082/test";
//var SERVER 			= "http://192.168.1.213";
//var SERVER 			= "http://101.200.81.163:8080/zc";
var CONTENT 	 	= '';
var SHOWPOSITION 	= "";
var default_pos     = '临潼校区';
function closeOtherWindow() {
    var curr = plus.webview.currentWebview();
    var wvs = plus.webview.all(); 
    for (var i = 0, len = wvs.length; i < len; i++) {
         console.log("page id = " + wvs[i].id);
        //关闭除setting页面外的其他页面 
        if (wvs[i].id != curr.id){
                plus.webview.close(wvs[i]);
        }
    }
}

function openNew(obj){
	window.location.href=$(obj).attr("href");
}

// function getUid() {
//     var uid = window.localStorage.getItem("uid");
//     if (uid != null) {
//         //如果本地uid不为空
//         return uid;
//     } else {
//         mui.alert("请重新登录",'','',function(){},'div');
//         mui.openWindow({
//             url: "login.html",
// 			id:"login-001"
//         })
//     }
// }
(function($, owner) {
    /**
     * 用户登录
     **/

    owner.login = function(loginInfo, callback) {
		
        mui.ajax({
            url: SERVER + "/mobile/user/accountDoLogin",
            data: {
                accountNo: loginInfo.uname,
                password: loginInfo.passwd
            },
            dataType: 'json',
            type: 'post',
            timeout: 30000, //超时时间设置为30秒；
          
            success: function(data) {
                if (data.status == "success") {
                    window.localStorage.setItem("uid", data.uid);
					var self = plus.webview.currentWebview();
					self.back();
					                 
                } else {
                    mui.toast("账号或密码错误");

                }

            },
            error: function(xhr, type, errorThrown) {
                plus.nativeUI.closeWaiting();
                var _error = "";
                switch (type) {
                    case "timeout":
                        _error = "服务器响应超时";
                        break;
                    default:
                        _error = "异常信息:" + xhr.responseText;
                        break;
                }
                mui.toast(_error);

            }
        });





    };

    owner.createState = function(name, callback) {
        var state = owner.getState();
        state.account = name;
        state.token = "token123456789";
        owner.setState(state);
        return callback();
    };

    /**
     * 新用户注册
     **/
    owner.reg = function(regInfo, callback) {
        callback = callback || $.noop;
        regInfo = regInfo || {};
        regInfo.account = regInfo.account || '';
        regInfo.password = regInfo.password || '';
        if (regInfo.account.length < 5) {
            return callback('用户名最短需要 5 个字符');
        }
        if (regInfo.password.length < 6) {
            return callback('密码最短需要 6 个字符');
        }
        if (!checkEmail(regInfo.email)) {
            return callback('邮箱地址不合法');
        }
        var users = JSON.parse(localStorage.getItem('$users') || '[]');
        users.push(regInfo);
        localStorage.setItem('$users', JSON.stringify(users));
        return callback();
    };

    /**
     * 获取当前状态
     **/
    owner.getState = function() {
        var stateText = localStorage.getItem('$state') || "{}";
        return JSON.parse(stateText);
    };

    /**
     * 设置当前状态
     **/
    owner.setState = function(state) {
        state = state || {};
        localStorage.setItem('$state', JSON.stringify(state));
        //var settings = owner.getSettings();
        //settings.gestures = '';
        //owner.setSettings(settings);
    };

    var checkEmail = function(email) {
        email = email || '';
        return (email.length > 3 && email.indexOf('@') > -1);
    };

    /**
     * 找回密码
     **/
    owner.forgetPassword = function(email, callback) {
        callback = callback || $.noop;
        if (!checkEmail(email)) {
            return callback('邮箱地址不合法');
        }
        return callback(null, '新的随机密码已经发送到您的邮箱，请查收邮件。');
    };

    /**
     * 获取应用本地配置
     **/
    owner.setSettings = function(settings) {
        settings = settings || {};
        localStorage.setItem('$settings', JSON.stringify(settings));
    }

    /**
     * 设置应用本地配置
     **/
    owner.getSettings = function() {
        var settingsText = localStorage.getItem('$settings') || "{}";
        return JSON.parse(settingsText);
    }
    /**
     * 获取本地是否安装客户端
     **/
    owner.isInstalled = function(id) {
        if (id === 'qihoo' && mui.os.plus) {
            return true;
        }
        if (mui.os.android) {
            var main = plus.android.runtimeMainActivity();
            var packageManager = main.getPackageManager();
            var PackageManager = plus.android.importClass(packageManager)
            var packageName = {
                "qq": "com.tencent.mobileqq",
                "weixin": "com.tencent.mm",
                "sinaweibo": "com.sina.weibo"
            }
            try {
                return packageManager.getPackageInfo(packageName[id], PackageManager.GET_ACTIVITIES);
            } catch (e) {}
        } else {
            switch (id) {
                case "qq":
                    var TencentOAuth = plus.ios.import("TencentOAuth");
                    return TencentOAuth.iphoneQQInstalled();
                case "weixin":
                    var WXApi = plus.ios.import("WXApi");
                    return WXApi.isWXAppInstalled()
                case "sinaweibo":
                    var SinaAPI = plus.ios.import("WeiboSDK");
                    return SinaAPI.isWeiboAppInstalled()
                default:
                    break;
            }
        }
    }
}(mui, window.app = {}));

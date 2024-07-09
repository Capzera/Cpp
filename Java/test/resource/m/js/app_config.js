var remoteAdrr = "http://192.168.1.215:82"

/**
 * TODO:在主页面中，打开新的子页面pageUrl，eg: race_list.html
 * @param {Object} pageUrl
 */

var aniShow = {};
function openSubPage(pageUrl,subpage_style) {
	self = plus.webview.currentWebview();
	var temp = {};
	var sub = plus.webview.create(pageUrl, pageUrl, );
	temp[0] = "true";
	mui.extend(aniShow, temp);
	self.append(sub);
	plus.webview.show(pageUrl, "fade-in", 300);
}

/**
 * @param {Object} obj
 * obj={url:'http://192.168.1.28/test.do',data:{},success:function( data ){}}
 */
function getResposeData( obj ){
	var mask = mui.createMask();
	mui.ajax({
			url: obj.url, 
			data: obj.data,
			dataType: 'json', //服务器返回json格式数据
			type: 'post', //HTTP请求类型
			timeout: 10000, //超时时间设置为10秒；
			beforeSend: function() {
				plus.nativeUI.showWaiting('数据加载中...', '数据请求中');
				mask.show();
			},
			success: function(data) {
				plus.nativeUI.closeWaiting();
				mask.close(); //关闭遮罩层
				if( obj.success ){ //表示obj的success回调函数存在,则调用obj的success函数
					obj.success(data);
				}
			},
			error: function(xhr, type, errorThrown) {
				mask.close();
				plus.nativeUI.closeWaiting();
				mui.alert("请检查网络", '', '', function() {}, 'div');
			}
	});
}

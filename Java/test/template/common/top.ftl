<link href="${base}/resource/common/css/wbox/wbox.css" rel="stylesheet" type="text/css" />
<script type="text/javascript" src="${base}/resource/common/wbox/wbox.js"></script>
<div class="top">
	<DIV class=top_top>
		<SPAN style="FLOAT: left; MARGIN-LEFT: 14px">${loginUser.CP_NAME}  ${loginUser.USER_NAME} 您好 欢迎光临中国华电集团公司福建分公司技术监督系统！</SPAN>
			<SPAN style="FLOAT: right; MARGIN-RIGHT: 6px">
				<FONT color=#808080> 今天是：
					<SCRIPT>
					    var clickObj;
					    //单击链接，判断用户是否有访问权限，有则弹出新的窗体。
						function gotoPage(obj,target){
									var url = $(obj).attr("ref");
									$.ajax({
										type:"post",
										url:url,
										dataType:"json",
										success:function(data){
											if(data.status == "error"){
												art.dialog.alert("对不起，您没有访问权限！");
											}
										},
										error:function(data){
												window.open(url,target);
										}
									})
						}
						today=new Date();
						var hours = today.getHours();
						var minutes = today.getMinutes();
						var seconds = today.getSeconds();
						function initArray(){
						this.length=initArray.arguments.length
						for(var i=0;i<this.length;i++)
						this[i+1]=initArray.arguments[i] }
						var d=new initArray("星期日","星期一","星期二","星期三","星期四","星期五","星期六");
						document.write(today.getYear(),"年",today.getMonth()+1,"月",today.getDate(),"日 ",d[today.getDay()+1]," ");
					
					</SCRIPT>
			 	</FONT>
			</SPAN>
		 <span>
		 	<a style="cursor:pointer" onclick="if(confirm('您确定要推出系统吗？')) window.location='${base}/logout'">退出系统</a>
		 </span>
		 <span>
		 	<a href="javascript:;" class="updatePasswd">修改密码</a>
		 </span>
		 <span>
		 	<#if loginUser.U_TYPE=="1"><a href="${base}/executeLogin">管理中心</a></#if>
		 </span>
		 <script>
		    var msg;
		 	$(document).ready(function(){
		 		$(".updatePasswd").click(function(){
	    				msg = $(".updatePasswd").wBox({	
					  		show:true,
					  		requestType:"ajax",
							target:"${base}/editPasswd",
							title:"修改密码",
							wBoxURL:"${base}/resource/common/css/wbox/"
						});
	    			})
		 	})
		 </script>
		<DIV class=clear></DIV>
	</DIV>
		<DIV class=logo_bg>
			<IMG style="MARGIN: 18px 0px 0px 25px; FLOAT: left" src="/resource/web/images/logo.png" width=601 height=54> 
			<DIV class=clear></DIV>
		</DIV>
		<DIV class=muen>
			<DIV class=xgsm>
				<DIV id=menu1 class=menu>
					<UL>
						<LI><A href="${base}/<#if loginUser.U_TYPE!="1">executeLogin<#else>index</#if>" ><SPAN style="TOP: 0px" class=out>首　　页</SPAN></A></LI>
						<LI><A onclick="gotoPage(this,'_top')" href="javascript:void(0)" ref="${base}/user/standard/center?companyId=<#if COMPANY_TYPE!="1">${COMPANY_ID}</#if>"><SPAN style="TOP: 0px" class=out>专业标准</SPAN></A></LI>
						<LI><A onclick="gotoPage(this,'_top')" href="javascript:void(0)" ref="${base}/user/report/center?companyId=<#if COMPANY_TYPE!="1">${COMPANY_ID}</#if>"><SPAN style="TOP: 0px" class=out>报表中心</SPAN></A></LI>
						
					<#list coteList as list>
						<LI><A onclick="gotoPage(this,'_top')" ref="${base}/user/document/${list.SID}/center?companyId=<#if COMPANY_TYPE!="1">${COMPANY_ID}</#if>"><SPAN style="TOP: 0px" class=out>${list.NAME}</SPAN></A></LI>
					</#list>
						<LI><A onclick="gotoPage(this,'_top')" ref="${base}/user/bbs/index"><SPAN style="TOP: 0px" class=out>技术论坛</SPAN></A></LI>
					</UL>
					<DIV class=cls></DIV>
				</DIV>
			</DIV>
			<DIV class=clear></DIV>
		</DIV>
</div>
<div style="z-index:-10000;width:1002px;">
	<div style="z-index:-10000;">
	
		<object id="FlashID" classid="clsid:D27CDB6E-AE6D-11cf-96B8-444553540000" width="1002" height="150" style="padding:0px;" >
		    <param name="movie" value="${base}/resource/flash/banner1.swf" />
		    <param name="quality" value="high" />
		    <param name="wmode" value="transparent" />
		    <param name="swfversion" value="9.0.45.0" />
		    <!-- 此 param 标签提示使用 Flash Player 6.0 r65 和更高版本的用户下载最新版本的 Flash Player。如果您不想让用户看到该提示，请将其删除。 -->
		    <param name="expressinstall" value="CommJS/expressInstall.swf" />
		    <!-- 下一个对象标签用于非 IE 浏览器。所以使用 IECC 将其从 IE 隐藏。 -->
		    <!--[if !IE]>-->
			    <object type="application/x-shockwave-flash" data="${base}/resource/flash/banner1.swf" width="812" height="193px" style="padding:0px;">
			      <!--<![endif]-->
			      <param name="quality" value="high" />
			      <param name="wmode" value="transparent" />
			      <param name="swfversion" value="9.0.45.0" />
			      <param name="expressinstall" value="${base}/resource/flash/expressInstall.swf" />
			      <!-- 浏览器将以下替代内容显示给使用 Flash Player 6.0 和更低版本的用户。 -->
			      <div>
			        <h4>此页面上的内容需要较新版本的 Adobe Flash Player。</h4>
			        <p><a href="http://www.adobe.com/go/getflashplayer"><img src="http://www.adobe.com/images/shared/download_buttons/get_flash_player.gif" alt="获取 Adobe Flash Player" width="112" height="33" /></a></p>
			      </div>
			      <!--[if !IE]>-->
			     </object><!--<![endif]-->
    	 </object>
	    
		
	</div>
</div>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Frameset//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-frameset.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>西安工程大学管理学院-经济与管理实验教学中心-管理系统</title>
<script type="text/javascript" src="${base}/resource/common/js/jquery-1.4.4.min.js"></script>
<script>

   $.ajaxSetup({
      type: 'POST',
      contentType:"application/x-www-form-urlencoded;charset=utf-8",
      complete: function(xhr,status) {
          var sessionStatus = xhr.getResponseHeader('sessionstatus');
          if(sessionStatus == 'timeout') {
             var top = getTopWinow();
             var yes = confirm('由于您长时间没有操作, session已过期, 请重新登录.');
             if (yes) {
                 top.location.href = '${base}/login';            
             }
         }
     }
   });
   function getTopWinow(){
     var p = window;
     while(p != p.parent){
         p = p.parent;
     }
     return p;
   }
 </script>
</head>

<frameset rows="127,*,11" frameborder="no" border="0" framespacing="0">
  <frame src="${base}/admin/top.do" name="topFrame" scrolling="no" noresize="noresize" id="topFrame" />
  <frame src="${base}/admin/center.do" name="mainFrame" id="mainFrame" />
  <frame src="${base}/admin/down.do" name="bottomFrame" scrolling="no" noresize="noresize" id="bottomFrame" />
</frameset>
<noframes>
<body>
</body>
</noframes>
</html>

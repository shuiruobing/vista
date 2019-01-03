//获取屏幕缩放比
function detectZoom (){ 
  var ratio = 0,
    screen = window.screen,
    ua = navigator.userAgent.toLowerCase();
 
   if (window.devicePixelRatio !== undefined) {
      ratio = window.devicePixelRatio;
  }
  else if (~ua.indexOf('msie')) {  
    if (screen.deviceXDPI && screen.logicalXDPI) {
      ratio = screen.deviceXDPI / screen.logicalXDPI;
    }
  }
  else if (window.outerWidth !== undefined && window.innerWidth !== undefined) {
    ratio = window.outerWidth / window.innerWidth;
  }
   
   if (ratio){
    ratio = Math.round(ratio * 100);
  }
   
   return ratio;
};
//时间间隔内执行
function interval(func, wait){
  var interv = function(){
    func.call(null);
    setTimeout(interv, wait);
  };

  setTimeout(interv, wait);
};
//数字前补零
function PrefixInteger(num, n) {
    //用0作分隔符，然后从-n开始取字符串
  return (Array(n).join(0) + num).slice(-n);
};
//日期格式化
Date.prototype.Format = function (fmt) {    
    var o = {    
        "M+": this.getMonth() + 1, //月份     
        "d+": this.getDate(), //日     
        "H+": this.getHours(), //小时     
        "m+": this.getMinutes(), //分     
        "s+": this.getSeconds(), //秒     
        "q+": Math.floor((this.getMonth() + 3) / 3), //季度     
        "S": this.getMilliseconds() //毫秒     
    };    
    if (/(y+)/.test(fmt)) fmt = fmt.replace(RegExp.$1, (this.getFullYear() + "").substr(4 - RegExp.$1.length));    
    for (var k in o)    
    if (new RegExp("(" + k + ")").test(fmt)) fmt = fmt.replace(RegExp.$1, (RegExp.$1.length == 1) ? (o[k]) : (("00" + o[k]).substr(("" + o[k]).length)));    
    return fmt;    
};
//字符串格式化
String.prototype.format = function(){    
	    var args = arguments;    
	    return this.replace(/\{(\d+)\}/g,                    
	        function(m,i){    
	            return args[i];    
	        });    
};
//通过URL获取参数
function GetQueryString(name)
{
     var reg = new RegExp("(^|&)"+ name +"=([^&]*)(&|$)");
     var r = window.location.search.substr(1).match(reg);
     if(r!=null)return  unescape(r[2]); return null;
};
//比较两个时间的大小
function CompareDate(d1,d2)
{
  return ((new Date(d1.replace(/-/g,"\/"))) > (new Date(d2.replace(/-/g,"\/"))));
};
//测试
$(function(){
    //$.showParams();
    //503899.371:3391925.809->408161.83479595004,3390908.672767809
    //console.log(transformABToXY_([-75.916,3166.092]));
    //console.log(transformToUTM_Green([1283.342,203.941]));
    //坐标系验证
    //653842  203941

    // editor.getView().animate({center:ol.proj.transform([407592.9943826774, 3391289.8442695336],"EPSG:32651","EPSG:3857")},{zoom:22});
});
//用户信息cookie
//写cookies
function setCookie(name,value)
{
   var expiredays=1;
   var exdate=new Date();
   exdate.setDate(exdate.getDate()+expiredays);
   document.cookie=name+ "=" +escape(value)+
   ((expiredays==null) ? "" : ";expires="+exdate.toGMTString()+";path=/")
}
//读取cookies
function getCookie(name)
{
    if (document.cookie.length>0)
  {
  c_start=document.cookie.indexOf(name + "=")
  if (c_start!=-1)
    { 
    c_start=c_start + name.length+1 
    c_end=document.cookie.indexOf(";",c_start)
    if (c_end==-1) c_end=document.cookie.length
    return unescape(document.cookie.substring(c_start,c_end))
    } 
  }
return ""
}
//删除cookies
function delCookie(name)
{
    var exp = new Date();
    exp.setTime(exp.getTime() - 1);
    var cval=getCookie(name);
    if(cval!=null)
        document.cookie= name + "="+cval+";expires="+exp.toGMTString();
}

$.showInfo = function(str) {
    // 调用show方法
    BootstrapDialog.show({
        type : BootstrapDialog.TYPE_INFO,
        title : '提示',
        message : str,
        size : BootstrapDialog.SIZE_SMALL//size为小，默认的对话框比较宽
    });
};
//弹出错误提示的登录框
$.showErr = function(str, func) {
    // 调用show方法
    BootstrapDialog.show({
        type : BootstrapDialog.TYPE_DANGER,
        title : '错误 ',
        message : str,
        size : BootstrapDialog.SIZE_SMALL,//size为小，默认的对话框比较宽
        buttons : [ {// 设置关闭按钮
            label : '关闭',
            action : function(dialogItself) {
                dialogItself.close();
            }
        } ],
        // 对话框关闭时带入callback方法
        onhide : func
    });
};
//确认对话框
$.showConfirm = function(str, funcok, funcclose) {
    BootstrapDialog.confirm({
        title : '确认',
        message : str,
        type : BootstrapDialog.TYPE_WARNING, // <-- Default value is
        // BootstrapDialog.TYPE_PRIMARY
        closable : true, // <-- Default value is false，点击对话框以外的页面内容可关闭
        draggable : true, // <-- Default value is false，可拖拽
        btnCancelLabel : '取消', // <-- Default value is 'Cancel',
        btnOKLabel : '确定', // <-- Default value is 'OK',
        btnOKClass : 'btn-warning', // <-- If you didn't specify it, dialog type
        size : BootstrapDialog.SIZE_SMALL,
        // 对话框关闭的时候执行方法
        onhide : funcclose,
        callback : function(result) {
            // 点击确定按钮时，result为true
            if (result) {
                // 执行方法
                funcok.call();
            }
        }
    });
};
//成功对话框
$.showSuccessTimeout = function(str, func) {
    BootstrapDialog.show({
        type : BootstrapDialog.TYPE_SUCCESS,
        title : '成功 ',
        message : str,
        size : BootstrapDialog.SIZE_SMALL,
        buttons : [ {
            label : '确定',
            action : function(dialogItself) {
                dialogItself.close();
            }
        } ],
        // 指定时间内可自动关闭
        onshown : function(dialogRef) {
            setTimeout(function() {
                dialogRef.close();
            }, YUNM._set.timeout);
        },
        onhide : func
    });
};
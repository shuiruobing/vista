/*20160504增加设置值后的回调onClose,close: function (a)中修改更新完参数后执行onClose回调，传入$dp和event事件参数*/var $c;if($FF){Event.prototype.__defineSetter__("returnValue",function(a){if(!a){this.preventDefault();}return a;});Event.prototype.__defineGetter__("srcElement",function(){var a=this.target;while(a.nodeType!=1){a=a.parentNode;}return a;});HTMLElement.prototype.attachEvent=function(b,a){var d=b.replace(/on/,"");a._ieEmuEventHandler=function(e){window.event=e;return a();};this.addEventListener(d,a._ieEmuEventHandler,false);};}function My97DP(){$c=this;this.QS=[];$d=document.createElement("div");$d.className="WdateDiv";$d.innerHTML='<div id=dpTitle><div class="navImg NavImgll"><a></a></div><div class="navImg NavImgl"><a></a></div><div style="float:left"><div class="menuSel MMenu"></div><input class=yminput></div><div style="float:left"><div class="menuSel YMenu"></div><input class=yminput></div><div class="navImg NavImgrr"><a></a></div><div class="navImg NavImgr"><a></a></div><div style="float:right"></div></div><div style="position:absolute;overflow:hidden"></div><div></div><div id=dpTime><div class="menuSel hhMenu"></div><div class="menuSel mmMenu"></div><div class="menuSel ssMenu"></div><table cellspacing=0 cellpadding=0 border=0><tr><td rowspan=2><span id=dpTimeStr></span>&nbsp;<input class=tB maxlength=2><input value=":" class=tm readonly><input class=tE maxlength=2><input value=":" class=tm readonly><input class=tE maxlength=2></td><td><button id=dpTimeUp></button></td></tr><tr><td><button id=dpTimeDown></button></td></tr></table></div><div id=dpQS></div><div id=dpControl><input class=dpButton id=dpClearInput type=button><input class=dpButton id=dpTodayInput type=button><input class=dpButton id=dpOkInput type=button></div>';attachTabEvent($d,function(){hideSel();});d();$dp.focusArr=[document,$d.MI,$d.yI,$d.HI,$d.mI,$d.sI,$d.clearI,$d.todayI,$d.okI];for(var a=0;a<$dp.focusArr.length;a++){var e=$dp.focusArr[a];e.nextCtrl=a==$dp.focusArr.length-1?$dp.focusArr[1]:$dp.focusArr[a+1];$dp.attachEvent(e,"onkeydown",_tab);}this.init();b();_inputBindEvent("y,M,H,m,s");$d.upButton.onclick=function(){updownEvent(1);};$d.downButton.onclick=function(){updownEvent(-1);};$d.qsDiv.onclick=function(){if($d.qsDivSel.style.display!="block"){$c._fillQS();showB($d.qsDivSel);}else{hide($d.qsDivSel);}};document.body.appendChild($d);function d(){var f=g("a");divs=g("div"),ipts=g("input"),btns=g("button"),spans=g("span");$d.navLeftImg=f[0];$d.leftImg=f[1];$d.rightImg=f[3];$d.navRightImg=f[2];$d.rMD=divs[9];$d.MI=ipts[0];$d.yI=ipts[1];$d.titleDiv=divs[0];$d.MD=divs[4];$d.yD=divs[6];$d.qsDivSel=divs[10];$d.dDiv=divs[11];$d.tDiv=divs[12];$d.HD=divs[13];$d.mD=divs[14];$d.sD=divs[15];$d.qsDiv=divs[16];$d.bDiv=divs[17];$d.HI=ipts[2];$d.mI=ipts[4];$d.sI=ipts[6];$d.clearI=ipts[7];$d.todayI=ipts[8];$d.okI=ipts[9];$d.upButton=btns[0];$d.downButton=btns[1];$d.timeSpan=spans[0];function g(h){return $d.getElementsByTagName(h);}}function b(){$d.navLeftImg.onclick=function(){$ny=$ny<=0?$ny-1:-1;if($ny%5==0){$d.yI.focus();return;}$d.yI.value=$dt.y-1;$d.yI.onblur();};$d.leftImg.onclick=function(){$dt.attr("M",-1);$d.MI.onblur();};$d.rightImg.onclick=function(){$dt.attr("M",1);$d.MI.onblur();};$d.navRightImg.onclick=function(){$ny=$ny>=0?$ny+1:1;if($ny%5==0){$d.yI.focus();return;}$d.yI.value=$dt.y+1;$d.yI.onblur();};}}My97DP.prototype={init:function(){$ny=0;$dp.cal=this;if($dp.readOnly&&$dp.el.readOnly!=null){$dp.el.readOnly=true;$dp.el.blur();}a();this._dealFmt();$dt=this.newdate=new DPDate();$tdt=new DPDate();$sdt=this.date=new DPDate();this.dateFmt=this.doExp($dp.dateFmt);this.autoPickDate=$dp.autoPickDate==null?($dp.has.st&&$dp.has.st?false:true):$dp.autoPickDate;$dp.autoUpdateOnChanged=$dp.autoUpdateOnChanged==null?($dp.isShowOK&&$dp.has.d?false:true):$dp.autoUpdateOnChanged;this.ddateRe=this._initRe("disabledDates");this.ddayRe=this._initRe("disabledDays");this.sdateRe=this._initRe("specialDates");this.ssdateRe=this._initRe("sspecialDates");this.sdayRe=this._initRe("specialDays");this.minDate=this.doCustomDate($dp.minDate,$dp.minDate!=$dp.defMinDate?$dp.realFmt:$dp.realFullFmt,$dp.defMinDate);this.maxDate=this.doCustomDate($dp.maxDate,$dp.maxDate!=$dp.defMaxDate?$dp.realFmt:$dp.realFullFmt,$dp.defMaxDate);if(this.minDate.compareWith(this.maxDate)>0){$dp.errMsg=$lang.err_1;}if(this.loadDate()){this._makeDateInRange();this.oldValue=$dp.el[$dp.elProp];}else{this.mark(false,2);}_setAll($dt);$d.timeSpan.innerHTML=$lang.timeStr;$d.clearI.value=$lang.clearStr;$d.todayI.value=$lang.todayStr;$d.okI.value=$lang.okStr;$d.okI.disabled=!$c.checkValid($sdt);this.initShowAndHide();this.initBtn();if($dp.errMsg){alert($dp.errMsg);}this.draw();if($dp.el.nodeType==1&&$dp.el.My97Mark===undefined){$dp.attachEvent($dp.el,"onkeydown",_tab);$dp.attachEvent($dp.el,"onblur",function(){if($dp.dd.style.display=="none"){$c.close();}});}$c.currFocus=$dp.el;hideSel();function a(){var b,d;for(b=0;(d=document.getElementsByTagName("link")[b]);b++){if(d.rel.indexOf("style")!=-1&&d.title){d.disabled=true;if(d.title==$dp.skin){d.disabled=false;}}}}},_makeDateInRange:function(){var a=this.checkRange();if(a!=0){var b;if(a>0){b=this.maxDate;}else{b=this.minDate;}if($dp.has.sd){$dt.y=b.y;$dt.M=b.M;$dt.d=b.d;}if($dp.has.st){$dt.H=b.H;$dt.m=b.m;$dt.s=b.s;}}},splitDate:function(g,o,e,u,q,s,t,f,n){var p;if(g&&g.loadDate){p=g;}else{p=new DPDate();if(g!=""){o=o||$dp.dateFmt;var j,d=0,k,r=/yyyy|yyy|yy|y|MMMM|MMM|MM|M|dd|d|%ld|HH|H|mm|m|ss|s|DD|D|WW|W|w/g,b=o.match(r);r.lastIndex=0;if(n){k=g.split(/\W+/);}else{var a=0,m="^";while((k=r.exec(o))!==null){if(a>=0){m+=o.substring(a,k.index);}a=r.lastIndex;switch(k[0]){case"yyyy":m+="(\\d{4})";break;case"yyy":m+="(\\d{3})";break;case"MMMM":case"MMM":case"DD":case"D":m+="(\\D+)";break;default:m+="(\\d\\d?)";break;}}m+=".*$";k=new RegExp(m).exec(g);d=1;}if(k){for(j=0;j<b.length;j++){var h=k[j+d];if(h){switch(b[j]){case"MMMM":case"MMM":p.M=l(b[j],h);break;case"y":case"yy":h=pInt2(h,0);if(h<50){h+=2000;}else{h+=1900;}p.y=h;break;case"yyy":p.y=pInt2(h,0)+$dp.yearOffset;break;default:p[b[j].slice(-1)]=h;break;}}}}else{p.d=32;}}}p.coverDate(e,u,q,s,t,f);return p;function l(x,v){var y=x=="MMMM"?$lang.aLongMonStr:$lang.aMonStr;for(var w=0;w<12;w++){if(y[w].toLowerCase()==v.substr(0,y[w].length).toLowerCase()){return w+1;}}return -1;}},_initRe:function(e){var a,b=$dp[e],d="(?:";if(b){for(a=0;a<b.length;a++){d+=this.doExp(b[a]);if(a!=b.length-1){d+="|";}}d=new RegExp(d+")");}else{d=null;}return d;},update:function(){var a=this.getNewDateStr();if($dp.el[$dp.elProp]!=a){$dp.el[$dp.elProp]=a;}this.setRealValue();},setRealValue:function(b){var a=$dp.$($dp.vel),b=rtn(b,this.getNewDateStr($dp.realFmt));if(a){a.value=b;}$dp.el.realValue=b;},doExp:function(s){var ps="yMdHms",arr,tmpEval,re=/#?\{(.*?)\}/;s=s+"";for(var i=0;i<ps.length;i++){s=s.replace("%"+ps.charAt(i),this.getP(ps.charAt(i),null,$tdt));}if(s.substring(0,3)=="#F{"){s=s.substring(3,s.length-1);if(s.indexOf("return ")<0){s="return "+s;}s=$dp.win.eval('new Function("'+s+'");');s=s();}else{while((arr=re.exec(s))!=null){arr.lastIndex=arr.index+arr[1].length+arr[0].length-arr[1].length-1;tmpEval=pInt(eval(arr[1]));if(tmpEval<0){tmpEval="9700"+(-tmpEval);}s=s.substring(0,arr.index)+tmpEval+s.substring(arr.lastIndex+1);}}return s;},doCustomDate:function(d,a,e){var b;d=this.doExp(d);if(!d||d==""){d=e;}if(typeof d=="object"){b=d;}else{b=this.splitDate(d,a,null,null,1,0,0,0,true);b.y=(""+b.y).replace(/^9700/,"-");b.M=(""+b.M).replace(/^9700/,"-");b.d=(""+b.d).replace(/^9700/,"-");b.H=(""+b.H).replace(/^9700/,"-");b.m=(""+b.m).replace(/^9700/,"-");b.s=(""+b.s).replace(/^9700/,"-");if(d.indexOf("%ld")>=0){d=d.replace(/%ld/g,"0");b.d=0;b.M=pInt(b.M)+1;}b.refresh();}return b;},loadDate:function(){var a,b;if($dp.alwaysUseStartDate||($dp.startDate!=""&&$dp.el[$dp.elProp]=="")){a=this.doExp($dp.startDate);b=$dp.realFmt;}else{a=$dp.el[$dp.elProp];b=this.dateFmt;}$dt.loadFromDate(this.splitDate(a,b));if(a!=""){var d=1;if($dp.has.sd&&!this.isDate($dt)){$dt.y=$tdt.y;$dt.M=$tdt.M;$dt.d=$tdt.d;d=0;}if($dp.has.st&&!this.isTime($dt)){$dt.H=$tdt.H;$dt.m=$tdt.m;$dt.s=$tdt.s;d=0;}return d&&this.checkValid($dt);}return 1;},isDate:function(a){if(a.y!=null){a=doStr(a.y,4)+"-"+a.M+"-"+a.d;}return a.match(/^((\d{2}(([02468][048])|([13579][26]))[\-\/\s]?((((0?[13578])|(1[02]))[\-\/\s]?((0?[1-9])|([1-2][0-9])|(3[01])))|(((0?[469])|(11))[\-\/\s]?((0?[1-9])|([1-2][0-9])|(30)))|(0?2[\-\/\s]?((0?[1-9])|([1-2][0-9])))))|(\d{2}(([02468][1235679])|([13579][01345789]))[\-\/\s]?((((0?[13578])|(1[02]))[\-\/\s]?((0?[1-9])|([1-2][0-9])|(3[01])))|(((0?[469])|(11))[\-\/\s]?((0?[1-9])|([1-2][0-9])|(30)))|(0?2[\-\/\s]?((0?[1-9])|(1[0-9])|(2[0-8]))))))(\s(((0?[0-9])|([1-2][0-3]))\:([0-5]?[0-9])((\s)|(\:([0-5]?[0-9])))))?$/);},isTime:function(a){if(a.H!=null){a=a.H+":"+a.m+":"+a.s;}return a.match(/^([0-9]|([0-1][0-9])|([2][0-3])):([0-9]|([0-5][0-9])):([0-9]|([0-5][0-9]))$/);},checkRange:function(b,d){b=b||$dt;var a=b.compareWith(this.minDate,d);if(a>0){a=b.compareWith(this.maxDate,d);if(a<0){a=0;}}return a;},checkValid:function(a,d,b){d=d||$dp.has.minUnit;var e=this.checkRange(a,d);if(e==0){e=1;if(d=="d"&&b==null){b=new Date(a.y,a.M-1,a.d).getDay();}e=!this.testDisDay(b)&&!this.testDisDate(a,d);}else{e=0;}return e;},checkAndUpdate:function(){var a=$dp.el,d=this,b=$dp.el[$dp.elProp];if(b!=null){if(b!=""&&!$dp.readOnly){d.date.loadFromDate(d.splitDate(b,d.dateFmt));}if(b==""||(d.isDate(d.date)&&d.isTime(d.date)&&d.checkValid(d.date))){if(b!=""){d.newdate.loadFromDate(d.date);d.update();}else{d.setRealValue("");}}else{return false;}}return true;},close:function(a){hideSel();if(this.checkAndUpdate()){this.mark(true);$dp.hide();var b=$dp.onClose;if(b&&typeof b=="function"){b($dp, a);}}else{if(a){_cancelKey(a);this.mark(false,2);}else{this.mark(false);}$dp.show();}},_fd:function(){var l,n,f,h,b,m=new sb(),e=$lang.aWeekStr,d=$dp.firstDayOfWeek,k="",o="",g=new DPDate($dt.y,$dt.M,$dt.d,0,0,0),j=g.y,a=g.M;b=1-new Date(j,a-1,1).getDay()+d;if(b>1){b-=7;}m.a("<table class=WdayTable width=100% border=0 cellspacing=0 cellpadding=0>");m.a("<tr class=MTitle align=center>");if($dp.isShowWeek){m.a("<td class=WdayTableWeek>"+e[0]+"</td>");}for(l=0;l<7;l++){m.a("<td class=WdayTableWeek>"+e[(d+l)%7+1]+"</td>");}m.a("</tr>");for(l=1,n=b;l<7;l++){m.a("<tr>");for(f=0;f<7;f++){g.loadDate(j,a,n++);g.refresh();if(g.M==a){h=true;if(g.compareWith($sdt,"d")==0){k="Wselday";}else{if(g.compareWith($tdt,"d")==0){k="Wtoday";}else{k=($dp.highLineWeekDay&&(0==(d+f)%7||6==(d+f)%7)?"Wwday":"Wday");}}o=($dp.highLineWeekDay&&(0==(d+f)%7||6==(d+f)%7)?"WwdayOn":"WdayOn");}else{if($dp.isShowOthers){h=true;k="WotherDay";o="WotherDayOn";}else{h=false;}}if($dp.isShowWeek&&f==0&&(l<4||h)){m.a("<td class=Wweek>"+getWeek(g,$dp.firstDayOfWeek==0?1:0)+"</td>");}m.a("<td ");if(h){if(this.checkValid(g,"d",f)){if(this.testSpeDay(new Date(g.y,g.M-1,g.d).getDay())||this.testSpeDate(g)){k="WspecialDay";}if(this.testSSpeDay(new Date(g.y,g.M-1,g.d).getDay())||this.testSSpeDate(g)){k="WsspecialDay";}if(this.testSSpeDate(g) && g.compareWith($sdt, "d") == 0) {k = "WWsspecialDay";}if(this.testSpeDate(g) && g.compareWith($sdt, "d") == 0) {k = "WWspecialDay";}m.a('onclick="day_Click('+g.y+","+g.M+","+g.d+');" ');m.a("onmouseover=\"this.className='"+o+"'\" ");m.a("onmouseout=\"this.className='"+k+"'\" ");}else{k="WinvalidDay";}m.a("class="+k);m.a(">"+g.d+"</td>");}else{m.a("></td>");}}m.a("</tr>");}m.a("</table>");return m.j();},testSSpeDate:function(a){return this.testDate(a,this.ssdateRe);},testDisDate:function(a,d){var b=this.testDate(a,this.ddateRe,d);return(this.ddateRe&&$dp.opposite)?!b:b;},testDisDay:function(a){return this.testDay(a,this.ddayRe);},testSpeDate:function(a){return this.testDate(a,this.sdateRe);},testSpeDay:function(a){return this.testDay(a,this.sdayRe);},testSSpeDay:function(a){return this.testDay(a,this.ssdayRe);},testDate:function(a,b,d){var e=d=="d"?$dp.realDateFmt:$dp.realFmt;return b?b.test(this.getDateStr(e,a)):0;},testDay:function(a,b){return b?b.test(a):0;},_f:function(p,c,r,e,isR){var s=new sb(),fp=isR?"r"+p:p;bak=$dt[p];s.a("<table cellspacing=0 cellpadding=3 border=0");for(var i=0;i<r;i++){s.a('<tr nowrap="nowrap">');for(var j=0;j<c;j++){s.a("<td nowrap ");$dt[p]=eval(e);if(($dp.opposite&&this.checkRange($dt,p)==0)||this.checkValid($dt,p)){s.a("class='menu' onmouseover=\"this.className='menuOn'\" onmouseout=\"this.className='menu'\" onmousedown=\"");s.a("hide($d."+p+"D);$d."+fp+"I.value="+$dt[p]+";$d."+fp+'I.blur();"');}else{s.a("class='invalidMenu'");}s.a(">"+(p=="M"?$lang.aMonStr[$dt[p]-1]:$dt[p])+"</td>");}s.a("</tr>");}s.a("</table>");$dt[p]=bak;return s.j();},_fMyPos:function(b,a){if(b){var d=b.offsetLeft;if($IE){d=b.getBoundingClientRect().left;}a.style.left=d;}},_fM:function(a){this._fMyPos(a,$d.MD);$d.MD.innerHTML=this._f("M",2,6,"i+j*6+1",a==$d.rMI);},_fy:function(a,d){var b=new sb();d=rtn(d,$dt.y-5);b.a(this._f("y",2,5,d+"+i+j*5",a==$d.ryI));b.a("<table cellspacing=0 cellpadding=3 border=0 align=center><tr><td ");b.a(this.minDate.y<d?"class='menu' onmouseover=\"this.className='menuOn'\" onmouseout=\"this.className='menu'\" onmousedown='if(event.preventDefault)event.preventDefault();event.cancelBubble=true;$c._fy(0,"+(d-10)+")'":"class='invalidMenu'");b.a(">←</td><td class='menu' onmouseover=\"this.className='menuOn'\" onmouseout=\"this.className='menu'\" onmousedown=\"hide($d.yD);$d.yI.blur();\">\xd7</td><td ");b.a(this.maxDate.y>d+10?"class='menu' onmouseover=\"this.className='menuOn'\" onmouseout=\"this.className='menu'\" onmousedown='if(event.preventDefault)event.preventDefault();event.cancelBubble=true;$c._fy(0,"+(d+10)+")'":"class='invalidMenu'");b.a(">→</td></tr></table>");this._fMyPos(a,$d.yD);$d.yD.innerHTML=b.j();},_fHMS:function(d,a,b){$d[d+"D"].innerHTML=this._f(d,6,a,b);},_fH:function(){this._fHMS("H",4,"i * 6 + j");},_fm:function(){this._fHMS("m",2,"i * 30 + j * 5");},_fs:function(){this._fHMS("s",1,"j * 10");},_fillQS:function(d){this.initQS();var e=this.QS,f=e.style,a=new sb();a.a("<table class=WdayTable width=100% height=100% border=0 cellspacing=0 cellpadding=0>");a.a('<tr class=MTitle><td><div style="float:left">'+$lang.quickStr+"</div>");if(!d){a.a('<div style="float:right;cursor:pointer" onclick="hide($d.qsDivSel);">\xd7</div>');}a.a("</td></tr>");for(var b=0;b<e.length;b++){if(e[b]){a.a("<tr><td style='text-align:left' nowrap='nowrap' class='menu' onmouseover=\"this.className='menuOn'\" onmouseout=\"this.className='menu'\" onclick=\"");a.a("day_Click("+e[b].y+", "+e[b].M+", "+e[b].d+","+e[b].H+","+e[b].m+","+e[b].s+');">');a.a("&nbsp;"+this.getDateStr(null,e[b]));a.a("</td></tr>");}else{a.a("<tr><td class='menu'>&nbsp;</td></tr>");}}a.a("</table>");$d.qsDivSel.innerHTML=a.j();},_dealFmt:function(){a(/w/);a(/WW|W/);a(/DD|D/);a(/yyyy|yyy|yy|y/);a(/MMMM|MMM|MM|M/);a(/dd|d/);a(/HH|H/);a(/mm|m/);a(/ss|s/);$dp.has.sd=($dp.has.y||$dp.has.M||$dp.has.d)?true:false;$dp.has.st=($dp.has.H||$dp.has.m||$dp.has.s)?true:false;$dp.realFullFmt=$dp.realFullFmt.replace(/%Date/,$dp.realDateFmt).replace(/%Time/,$dp.realTimeFmt);if($dp.has.sd){if($dp.has.st){$dp.realFmt=$dp.realFullFmt;}else{$dp.realFmt=$dp.realDateFmt;}}else{$dp.realFmt=$dp.realTimeFmt;}function a(b){var d=(b+"").slice(1,2);$dp.has[d]=b.exec($dp.dateFmt)?($dp.has.minUnit=d,true):false;}},initShowAndHide:function(){var a=0;$dp.has.y?(a=1,show($d.yI,$d.navLeftImg,$d.navRightImg)):hide($d.yI,$d.navLeftImg,$d.navRightImg);$dp.has.M?(a=1,show($d.MI,$d.leftImg,$d.rightImg)):hide($d.MI,$d.leftImg,$d.rightImg);a?show($d.titleDiv):hide($d.titleDiv);if($dp.has.st){show($d.tDiv);disHMS($d.HI,$dp.has.H);disHMS($d.mI,$dp.has.m);disHMS($d.sI,$dp.has.s);}else{hide($d.tDiv);}shorH($d.clearI,$dp.isShowClear);shorH($d.todayI,$dp.isShowToday);shorH($d.okI,$dp.isShowOK);shorH($d.qsDiv,!$dp.doubleCalendar&&$dp.has.d&&$dp.qsEnabled);if($dp.eCont||!($dp.isShowClear||$dp.isShowToday||$dp.isShowOK)){hide($d.bDiv);}else{show($d.bDiv);}},mark:function(g,d){var b=$dp.el,e=$FF?"class":"className";if(g){f(b);}else{if(d==null){d=$dp.errDealMode;}switch(d){case 0:if(confirm($lang.errAlertMsg)){b[$dp.elProp]=this.oldValue;f(b);}else{a(b);}break;case 1:b[$dp.elProp]=this.oldValue;f(b);break;case 2:a(b);break;}}function f(k){var h=k.className;if(h){var j=h.replace(/WdateFmtErr/g,"");if(h!=j){k.setAttribute(e,j);}}}function a(h){h.setAttribute(e,h.className+" WdateFmtErr");}},getP:function(f,j,l){l=l||$sdt;var g,k=[f+f,f],h,b=l[f],e=function(m){return doStr(b,m.length);};switch(f){case"w":b=getDay(l);break;case"D":var d=getDay(l)+1;e=function(m){return m.length==2?$lang.aLongWeekStr[d]:$lang.aWeekStr[d];};break;case"W":b=getWeek(l);break;case"y":k=["yyyy","yyy","yy","y"];j=j||k[0];e=function(m){return doStr((m.length<4)?(m.length<3?l.y%100:(l.y+2000-$dp.yearOffset)%1000):b,m.length);};break;case"M":k=["MMMM","MMM","MM","M"];e=function(m){return(m.length==4)?$lang.aLongMonStr[b-1]:(m.length==3)?$lang.aMonStr[b-1]:doStr(b,m.length);};break;}j=j||f+f;if("yMdHms".indexOf(f)>-1&&f!="y"&&!$dp.has[f]){if("Hms".indexOf(f)>-1){b=0;}else{b=1;}}var a=[];for(g=0;g<k.length;g++){h=k[g];if(j.indexOf(h)>=0){a[g]=e(h);j=j.replace(h,"{"+g+"}");}}for(g=0;g<a.length;g++){j=j.replace(new RegExp("\\{"+g+"\\}","g"),a[g]);}return j;},getDateStr:function(e,g){g=g||this.splitDate($dp.el[$dp.elProp],this.dateFmt)||$sdt;e=e||this.dateFmt;if(e.indexOf("%ld")>=0){var b=new DPDate();b.loadFromDate(g);b.d=0;b.M=pInt(b.M)+1;b.refresh();e=e.replace(/%ld/g,b.d);}var a="ydHmswW";for(var d=0;d<a.length;d++){var f=a.charAt(d);e=this.getP(f,e,g);}if($dp.has.D){e=e.replace(/DD/g,"%dd").replace(/D/g,"%d");e=this.getP("M",e,g);e=e.replace(/\%dd/g,this.getP("D","DD")).replace(/\%d/g,this.getP("D","D"));}else{e=this.getP("M",e,g);}return e;},getNewP:function(a,b){return this.getP(a,b,$dt);},getNewDateStr:function(a){return this.getDateStr(a,$dt);},draw:function(){if($dp.eCont){$c.getMyDate();}$d.rMD.innerHTML="";if($dp.doubleCalendar){$c.autoPickDate=true;$dp.isShowOthers=false;$d.className="WdateDiv WdateDiv2";var a=new sb();a.a("<table class=WdayTable2 width=100% cellspacing=0 cellpadding=0 border=1><tr><td valign=top>");a.a(this._fd());a.a("</td><td valign=top>");$dt.attr("M",1);a.a(this._fd());$d.rMI=$d.MI.cloneNode(true);$d.ryI=$d.yI.cloneNode(true);$d.rMD.appendChild($d.rMI);$d.rMD.appendChild($d.ryI);$d.rMI.value=$lang.aMonStr[$dt.M-1];$d.rMI.realValue=$dt.M;$d.ryI.value=$dt.y;_inputBindEvent("rM,ry");$d.rMI.className=$d.ryI.className="yminput";$dt.attr("M",-1);a.a("</td></tr></table>");$d.dDiv.innerHTML=a.j();}else{$d.className="WdateDiv";$d.dDiv.innerHTML=this._fd();}if(!$dp.has.d||$dp.autoShowQS){this._fillQS(true);showB($d.qsDivSel);}else{hide($d.qsDivSel);}this.autoSize();},autoSize:function(){var e=parent.document.getElementsByTagName("iframe");for(var f=0;f<e.length;f++){var a=$d.style.height;$d.style.height="";var d=$d.offsetHeight;if(e[f].contentWindow==window&&d){e[f].style.width=$d.offsetWidth+"px";var b=$d.tDiv.offsetHeight;if(b&&$d.bDiv.style.display=="none"&&$d.tDiv.style.display!="none"&&document.body.scrollHeight-d>=b){d+=b;$d.style.height=d;}else{$d.style.height=a;}e[f].style.height=Math.max(d,$d.offsetHeight)+"px";}}$d.qsDivSel.style.width=$d.dDiv.offsetWidth;$d.qsDivSel.style.height=$d.dDiv.offsetHeight;},pickDate:function(){$dt.d=Math.min(new Date($dt.y,$dt.M,0).getDate(),$dt.d);$sdt.loadFromDate($dt);this.update();if(!$dp.eCont){if(this.checkValid($dt)){$c.mark(true);elFocus();hide($dp.dd);}else{$c.mark(false);}}if($dp.onpicked){callFunc("onpicked");}else{if(this.oldValue!=$dp.el[$dp.elProp]&&$dp.el.onchange){fireEvent($dp.el,"change");}}},getMyDate:function(){$dt.d=Math.min(new Date($dt.y,$dt.M,0).getDate(),$dt.d);this.update();if(!$dp.eCont){if(this.checkValid($dt)){$c.mark(true);elFocus();hide($dp.dd);}else{$c.mark(false);}}if($dp.onpicked){callFunc("onpicked");}else{if(this.oldValue!=$dp.el[$dp.elProp]&&$dp.el.onchange){fireEvent($dp.el,"change");}}},initBtn:function(){$d.clearI.onclick=function(){if(!callFunc("onclearing")){$dp.el[$dp.elProp]="";$c.setRealValue("");elFocus();hide($dp.dd);if($dp.oncleared){callFunc("oncleared");}else{if($c.oldValue!=$dp.el[$dp.elProp]&&$dp.el.onchange){fireEvent($dp.el,"change");}}}};$d.okI.onclick=function(){day_Click();};if(this.checkValid($tdt)){$d.todayI.disabled=false;$d.todayI.onclick=function(){$dt.loadFromDate($tdt);day_Click();};}else{$d.todayI.disabled=true;}},initQS:function(){var g,d,b,e,k=[],l=5,h=$dp.quickSel.length,j=$dp.has.minUnit;if(h>l){h=l;}else{if(j=="m"||j=="s"){k=[0,15,30,45,59,-60,-45,-30,-15,-1];}else{for(g=0;g<l*2;g++){k[g]=$dt[j]-l+1+g;}}}for(g=d=0;g<h;g++){b=this.doCustomDate($dp.quickSel[g]);if(this.checkValid(b)){this.QS[d++]=b;}}var a="yMdHms",f=[1,1,1,0,0,0];for(g=0;g<=a.indexOf(j);g++){f[g]=$dt[a.charAt(g)];}for(g=0;d<l;g++){if(g<k.length){b=new DPDate(f[0],f[1],f[2],f[3],f[4],f[5]);b[j]=k[g];b.refresh();if(this.checkValid(b)){this.QS[d++]=b;}}else{this.QS[d++]=null;}}}};function elFocus(){var a=$dp.el;try{if(a.style.display!="none"&&a.type!="hidden"&&(a.nodeName.toLowerCase()=="input"||a.nodeName.toLowerCase()=="textarea")){if($dp.srcEl==a){$dp.el.My97Mark=true;}$dp.el.focus();return;}}catch(b){}a.My97Mark=false;}function sb(){this.s=new Array();this.i=0;this.a=function(a){this.s[this.i++]=a;};this.j=function(){return this.s.join("");};}function getWeek(a,b){b=b||0;var e=new Date(a.y,a.M-1,a.d+b);e.setDate(e.getDate()-(e.getDay()+6)%7+$dp.whichDayIsfirstWeek-1);var d=e.valueOf();e.setMonth(0);e.setDate(4);return Math.round((d-e.valueOf())/(7*86400000))+1;}function getDay(b){var a=new Date(b.y,b.M-1,b.d);return a.getDay();}function show(){setDisp(arguments,"");}function showB(){setDisp(arguments,"block");}function hide(){setDisp(arguments,"none");}function setDisp(a,b){for(i=0;i<a.length;i++){a[i].style.display=b;}}function shorH(a,b){b?show(a):hide(a);}function disHMS(a,b){if(b){a.disabled=false;}else{a.disabled=true;a.value="00";}}function c(p,pv){if(p=="M"){pv=makeInRange(pv,1,12);}else{if(p=="H"){pv=makeInRange(pv,0,23);}else{if("ms".indexOf(p)>=0){pv=makeInRange(pv,0,59);}}}if(1){var func='sv("'+p+'",'+pv+")",rv=$c.checkRange();if(rv==0){eval(func);}else{if(rv<0){_setFrom($c.minDate);}else{if(rv>0){_setFrom($c.maxDate);}}}$d.okI.disabled=!$c.checkValid($sdt);if("yMd".indexOf(p)>=0){$c.draw();}callFunc(p+"changed");}function _setFrom($){_setAll($c.checkValid($)?$:$sdt);}}function _setAll(a){sv("y",a.y);sv("M",a.M);sv("d",a.d);sv("H",a.H);sv("m",a.m);sv("s",a.s);}function day_Click(a,g,d,f,e,j){var h=new DPDate($dt.y,$dt.M,$dt.d,$dt.H,$dt.m,$dt.s);$dt.loadDate(a,g,d,f,e,j);if(!callFunc("onpicking")){var b=h.y==a&&h.M==g&&h.d==d;if(!b&&arguments.length!=0){c("y",a);c("M",g);c("d",d);$c.currFocus=$dp.el;if($dp.autoUpdateOnChanged){$c.update();}}if($c.autoPickDate||b||arguments.length==0){$c.pickDate();}}else{$dt=h;}}function callFunc(b){var a;if($dp[b]){a=$dp[b].call($dp.el,$dp);}return a;}function sv(a,b){if(b==null){b=$dt[a];}$sdt[a]=$dt[a]=b;if("yHms".indexOf(a)>=0){$d[a+"I"].value=b;}if(a=="M"){$d.MI.realValue=b;$d.MI.value=$lang.aMonStr[b-1];}}function makeInRange(a,b,d){if(a<b){a=b;}else{if(a>d){a=d;}}return a;}function attachTabEvent(b,a){b.attachEvent("onkeydown",function(){var d=event,e=(d.which==undefined)?d.keyCode:d.which;if(e==9){a();}});}function doStr(b,a){b=b+"";while(b.length<a){b="0"+b;}return b;}function hideSel(){hide($d.yD,$d.MD,$d.HD,$d.mD,$d.sD);}function updownEvent(a){if($c.currFocus==undefined){$c.currFocus=$d.HI;}switch($c.currFocus){case $d.HI:c("H",$dt.H+a);break;case $d.mI:c("m",$dt.m+a);break;case $d.sI:c("s",$dt.s+a);break;}if($dp.autoUpdateOnChanged){$c.update();}}function DPDate(d,b,g,f,a,e){this.loadDate(d,b,g,f,a,e);}DPDate.prototype={loadDate:function(b,g,e,d,f,a){var h=new Date();this.y=pInt3(b,this.y,h.getFullYear());this.M=pInt3(g,this.M,h.getMonth()+1);this.d=$dp.has.d?pInt3(e,this.d,h.getDate()):1;this.H=pInt3(d,this.H,h.getHours());this.m=pInt3(f,this.m,h.getMinutes());this.s=pInt3(a,this.s,h.getSeconds());},loadFromDate:function(a){if(a){this.loadDate(a.y,a.M,a.d,a.H,a.m,a.s);}},coverDate:function(b,g,e,d,f,a){var h=new Date();this.y=pInt3(this.y,b,h.getFullYear());this.M=pInt3(this.M,g,h.getMonth()+1);this.d=$dp.has.d?pInt3(this.d,e,h.getDate()):1;this.H=pInt3(this.H,d,h.getHours());this.m=pInt3(this.m,f,h.getMinutes());this.s=pInt3(this.s,a,h.getSeconds());},compareWith:function(g,f){var b="yMdHms",e,a;f=b.indexOf(f);f=f>=0?f:5;for(var d=0;d<=f;d++){a=b.charAt(d);e=this[a]-g[a];if(e>0){return 1;}else{if(e<0){return -1;}}}return 0;},refresh:function(){var a=new Date(this.y,this.M-1,this.d,this.H,this.m,this.s);this.y=a.getFullYear();this.M=a.getMonth()+1;this.d=a.getDate();this.H=a.getHours();this.m=a.getMinutes();this.s=a.getSeconds();return !isNaN(this.y);},attr:function(a,b){if("yMdHms".indexOf(a)>=0){var d=this.d;if(a=="M"){this.d=1;}this[a]+=b;this.refresh();this.d=d;}}};function pInt(a){return parseInt(a,10);}function pInt2(b,a){return rtn(pInt(b),a);}function pInt3(b,d,a){return pInt2(b,rtn(d,a));}function rtn(b,a){return b==null||isNaN(b)?a:b;}function fireEvent(d,b){if($IE){d.fireEvent("on"+b);}else{var a=document.createEvent("HTMLEvents");a.initEvent(b,true,true);d.dispatchEvent(a);}}function _foundInput(a){var d,b,e="y,M,H,m,s,ry,rM".split(",");for(b=0;b<e.length;b++){d=e[b];if($d[d+"I"]==a){return d.slice(d.length-1,d.length);}}return 0;}function _focus(b){var a=_foundInput(this);if(!a){return;}$c.currFocus=this;if(a=="y"){this.className="yminputfocus";}else{if(a=="M"){this.className="yminputfocus";this.value=this["realValue"];}}this.select();$c["_f"+a](this);showB($d[a+"D"]);}function _blur(showDiv){var p=_foundInput(this),isR,mStr,v=this.value,oldv=$dt[p];if(p==0){return;}if(p=="y"){$dt[p]=Number(v)>=1?Number(v):$dt[p];}if(p=="M"){$dt[p]=(Number(v)>=1&&Number(v)<=12)?Number(v):$dt[p];}else{$dt[p]=Number(v)>=0?Number(v):$dt[p];}if(p=="y"){isR=this==$d.ryI;if(isR&&$dt.M==12){$dt.y-=1;}}else{if(p=="M"){isR=this==$d.rMI;if(isR){mStr=$lang.aMonStr[$dt[p]-1];if(oldv==12){$dt.y+=1;}$dt.attr("M",-1);}if($sdt.M==$dt.M){this.value=mStr||$lang.aMonStr[$dt[p]-1];}if(($sdt.y!=$dt.y)){c("y",$dt.y);}}}eval('c("'+p+'",'+$dt[p]+")");if(showDiv!==true){if(p=="y"||p=="M"){this.className="yminput";}hide($d[p+"D"]);}if($dp.autoUpdateOnChanged){$c.update();}}function _cancelKey(a){if(a.preventDefault){a.preventDefault();a.stopPropagation();}else{a.cancelBubble=true;a.returnValue=false;}if($OPERA){a.keyCode=0;}}function _inputBindEvent(a){var d=a.split(",");for(var b=0;b<d.length;b++){var e=d[b]+"I";$d[e].onfocus=_focus;$d[e].onblur=_blur;}}function _tab(t){var q=t.srcElement||t.target,j=t.which||t.keyCode;isShow=$dp.dd.style.display!="none";if(j>57){j-=48;}if($dp.enableKeyboard&&isShow){if(!q.nextCtrl){q.nextCtrl=$dp.focusArr[1];$c.currFocus=$dp.el;}if(q==$dp.el){$c.currFocus=$dp.el;}if(j==27){if(q==$dp.el){$c.close();return;}else{$dp.el.focus();}}if(j>=37&&j<=40){var s;if($c.currFocus==$dp.el||$c.currFocus==$d.okI){if($dp.has.d){U="d";if(j==38){$dt[U]-=7;}else{if(j==39){$dt[U]+=1;}else{if(j==37){$dt[U]-=1;}else{$dt[U]+=7;}}}$dt.refresh();c("y",$dt.y);c("M",$dt.M);c("d",$dt[U]);_cancelKey(t);return;}else{U=$dp.has.minUnit;$d[U+"I"].focus();}}U=U||_foundInput($c.currFocus);if(U){if(j==38||j==39){$dt[U]+=1;}else{$dt[U]-=1;}$dt.refresh();$c.currFocus.value=$dt[U];_blur.call($c.currFocus,true);$c.currFocus.select();}}else{if(j==9){var f=q.nextCtrl;for(var e=0;e<$dp.focusArr.length;e++){if(f.disabled==true||f.offsetHeight==0){f=f.nextCtrl;}else{break;}}if($c.currFocus!=f){$c.currFocus=f;f.focus();}}else{if(j==13){_blur.call($c.currFocus);if($c.currFocus.type=="button"){$c.currFocus.click();}else{$c.pickDate();}$c.currFocus=$dp.el;}}}}else{if(j==9&&q==$dp.el){$c.close();}}if($dp.enableInputMask&&!$OPERA&&!$dp.readOnly&&$c.currFocus==$dp.el&&(j>=48&&j<=57)){var m=$dp.el,g=m.value,z=Y(m),p={str:"",arr:[]},e=0,n,s=0,b=0,r=0,o,d=/yyyy|yyy|yy|y|MM|M|dd|d|%ld|HH|H|mm|m|ss|s|WW|W|w/g,u=$dp.dateFmt.match(d),w,x,a,k,l,y,o=0;if(g!=""){r=g.match(/[0-9]/g);r=r==null?0:r.length;for(e=0;e<u.length;e++){r-=Math.max(u[e].length,2);}r=r>=0?1:0;if(r==1&&z>=g.length){z=g.length-1;}}g=g.substring(0,z)+String.fromCharCode(j)+g.substring(z+r);z++;for(e=0;e<g.length;e++){var v=g.charAt(e);if(v>=0&&v<=9){p.str+=v;}else{p.arr[p.str.length]=1;}}g="";d.lastIndex=0;while((n=d.exec($dp.dateFmt))!==null){b=n.index-(n[0]=="%ld"?1:0);if(s>=0){g+=$dp.dateFmt.substring(s,b);if(z>=s+o&&z<=b+o){z+=b-s;}}s=d.lastIndex;y=s-b;w=p.str.substring(0,y);x=n[0].charAt(0);a=pInt(w.charAt(0));if(p.str.length>1){k=p.str.charAt(1);l=a*10+pInt(k);}else{k="";l=a;}if(p.arr[b]||x=="M"&&l>12||x=="d"&&l>31||x=="H"&&l>23||"ms".indexOf(x)>=0&&l>59){if(n[0].length==2){w="0"+a;y=1;}else{w=a;}z++;}else{if(y==1){w+=k;o++;}}g+=w;p.str=p.str.substring(y);if(p.str==""){break;}}m.value=g;h(m,z);_cancelKey(t);}if(isShow&&$c.currFocus!=$dp.el&&!((j>=48&&j<=57)||j==8||j==46)){_cancelKey(t);}function Y(D){var B=0;if(document.selection){D.focus();var C=document.selection.createRange();C.moveStart("character",-D.value.length);B=C.text.length;}else{if(D.selectionStart||D.selectionStart=="0"){B=D.selectionStart;}}return(B);}function h(B,D){if(B.setSelectionRange){B.focus();B.setSelectionRange(D,D);}else{if(B.createTextRange){var C=B.createTextRange();C.collapse(true);C.moveEnd("character",D);C.moveStart("character",D);C.select();}}}}
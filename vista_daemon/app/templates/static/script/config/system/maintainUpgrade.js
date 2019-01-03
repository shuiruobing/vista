define(function(require,exports,module){function e(){}var t,i,a,n,o,r,s,l,d,m,c,u,p,g,f,h;t=require("jquery"),require("ui.slider"),require("ui.jquery"),i=require("common"),a=require("translator"),n=require("utils"),o=require("dialog"),r=require("isapi/response"),s=require("isapi/device"),l=require("common/plugin"),require("lib/base64"),e.prototype={init:function(){var e=this;m={config:"",ipcConfig:"",upgrade:"",calibration:""},c={config:"",exportParam:"",ipcConfig:"",ipcExportParam:"",upgrade:"",calibration:""},u={config:"error",exportParam:"error",ipcConfig:"error",ipcExportParam:"error",upgrade:"error",calibration:"error"},h={bSupportIpcImport:s.m_oDeviceCapa.bSupportIpcImport,bSupportUploadCalibrationData:s.m_oDeviceCapa.bSupportUploadCalibrationData},p="",e.initPlugin(),e.initController()},initController:function(){var e=this;e.upgradeFlag(),angular.module("maintainUpgradeApp",["ui.jquery"]).controller("maintainUpgradeController",function($scope,$compile){d=$scope,$scope.oLan=a.oLastLanguage,$scope.oBrowseFilePath=m,$scope.oStatusTip=c,$scope.oIcon=u,$scope.iUpgradeMode=0,$scope.iUpgradeProcess=0,$scope.szUpgradeFlag=p,$scope.bUpgrading=!1,$scope.oCapSupport=h,$scope.restart=function(){r.restart()},$scope.restoreDefault=function(t){e.restoreDefault(t)},$scope.browseFilePath=function(e){var t=1,i="";1==$scope.iUpgradeMode&&"upgrade"==e&&(t=0),"ipcConfig"==e&&(i="xls"),l.browseFilePath(t,i,m,e)},$scope.importParam=function(){e.importParam()},$scope.exportParam=function(){e.exportParam()},$scope.importCalibration=function(){e.importCalibration()},$scope.importIPCParam=function(){e.importIPCParam()},$scope.exportIPCParam=function(){e.exportIPCParam()},$scope.startUpgrade=function(){e.startUpgrade($compile)},$scope.changeUpgradeMode=function(){$scope.oBrowseFilePath.upgrade=""}}),angular.bootstrap(angular.element("#maintainUpgrade"),["maintainUpgradeApp"])},initPlugin:function(){l.initPluginEvent(),l.initPlugin("0")},upgradeFlag:function(){WebSDK.WSDK_GetDeviceConfig(i.m_szHostName,"upgradeFlag",null,{async:!1,success:function(e,t){p=n.nodeValue(t,"flag"),"true"===n.nodeValue(t,"isSupportSystemBackup")&&(p+="_sptDoubleBack")}})},restoreDefault:function(e){o.confirm(a.getValue("restoreTip"),300,function(){var t=null;t=o.wait("",a.getValue("restoring")),WebSDK.WSDK_SetDeviceConfig(i.m_szHostName,"restore",{mode:e},{timeout:6e4,success:function(i,s,l){t.close();var d=n.nodeValue(s,"statusString");if("OK"==d)r.saveState(l);else if("Reboot Required"==d){var m=o.wait(null,a.getValue("rebooting"));if("full"==e)setTimeout(function(){m.close()},9e4);else{var c=n.nodeValue(s,"subStatusCode");"autoReboot"===c?setTimeout(function(){r.reconnect(m)},6e4):(m.close(),r.toRestart())}}else r.saveState(l)},error:function(e,i,a){t.close(),r.saveState(a)}})})},importParam:function(){var e=this;u.config="error",o.confirm(a.getValue("importParamTip"),300,function(){var t=m.config,n="";if(""==t)c.config=a.getValue("importCfgFailed"),setTimeout(function(){c.config="",d.$apply()},3e3);else{var o=i.m_szHttpProtocol+i.m_szHostName+":"+i.m_iHttpPort+"/ISAPI/System/configurationData",r=l.importDeviceConfig(o,i.getPluginAuth(),t);if(-1==r){var s=l.getLastError();n=403===s?a.getValue("noPermission"):a.getValue("importCfgFailed"),c.config=n,setTimeout(function(){c.config="",d.$apply()},3e3)}else e.getImportResult()}d.$apply()})},getImportResult:function(){var e=o.wait(null,a.getValue("importing")),i=setInterval(function(){var s=l.getImportResult(),m=n.parseXmlFromStr(s),p=t(m).find("statusString").eq(0).text();if(u.config="error",p)if(clearInterval(i),l.stopImportDeviceConfig(),e.close(),"Reboot Required"===t(m).find("statusString").eq(0).text()){if(1===seajs.iDeviceType)r.toRestart();else{var g=o.wait(null,a.getValue("rebooting"));setTimeout(function(){r.reconnect(g)},6e4)}c.config=""}else"OK"===t(m).find("statusString").eq(0).text()?(c.config=a.getValue("importCfgSucceeded"),u.config="success",setTimeout(function(){c.config="",d.$apply()},3e3)):(c.config=a.getValue("importCfgFailed"),setTimeout(function(){c.config="",d.$apply()},3e3));d.$apply()},5e3)},importCalibration:function(){var e=this;u.calibration="error";var t=m.calibration,n="";if(""==t)c.calibration=a.getValue("importCalibrationFailed"),setTimeout(function(){c.calibration="",d.$apply()},3e3);else{var o=i.m_szHttpProtocol+i.m_szHostName+":"+i.m_iHttpPort+"/ISAPI/System/calibrationData",r=l.importDeviceConfig(o,i.m_szPluginNamePwd,t);if(-1==r){var s=l.getLastError();n=403===s?a.getValue("noPermission"):a.getValue("importCalibrationFailed"),c.calibration=n,setTimeout(function(){c.calibration="",d.$apply()},3e3)}else e.getCalibrationResult()}d.$apply()},getCalibrationResult:function(){var e=o.wait(null,a.getValue("importing")),i=setInterval(function(){var o=l.getImportResult(),s=n.parseXmlFromStr(o),m=t(s).find("statusString").eq(0).text();u.calibration="error",m&&(clearInterval(i),l.stopImportDeviceConfig(),e.close(),"Reboot Required"===t(s).find("statusString").eq(0).text()?(r.toRestart(),c.calibration=""):"OK"===t(s).find("statusString").eq(0).text()?(c.calibration=a.getValue("importCalibrationSucc"),u.calibration="success",setTimeout(function(){c.calibration="",d.$apply()},3e3)):(c.calibration=a.getValue("importCalibrationFailed"),setTimeout(function(){c.calibration="",d.$apply()},3e3))),d.$apply()},5e3)},exportParam:function(){var e="",t="",n=-1;if(u.exportParam="error",t=i.m_szHttpProtocol+i.m_szHostName+":"+i.m_iHttpPort+"/ISAPI/System/configurationData",n=l.exportDeviceConfig(t,i.getPluginAuth(),"",0),0>n){var o=l.getLastError();e=403===o?a.getValue("noPermission"):a.getValue("exportCfgFailed"),c.exportParam=e}else 0==n&&(c.exportParam=a.getValue("exportCfgSucceeded"),u.exportParam="success");setTimeout(function(){c.exportParam="",d.$apply()},3e3)},importIPCParam:function(){var e=m.ipcConfig;if(u.ipcConfig="error",""==e)c.ipcConfig=a.getValue("importCfgFailed"),setTimeout(function(){c.ipcConfig="",d.$apply()},3e3);else{var o=i.m_szHttpProtocol+i.m_szHostName+":"+i.m_iHttpPort+"/ISAPI/ContentMgmt/InputProxy/ipcConfig";o=WebSDK.getSecurityVersion(o);var r=l.importIpcConfig(o,i.getPluginAuth(),e);if(0>r){var s=l.getLastError();return 403===s?(c.ipcConfig=a.getValue("noPermission"),setTimeout(function(){c.ipcConfig="",d.$digest()},3e3)):(c.ipcConfig=a.getValue("importCfgFailed"),setTimeout(function(){c.ipcConfig="",d.$digest()},3e3)),void 0}var p=l.getIpcImportErrorInfo(),g=n.parseXmlFromStr(p);if(n.nodeValue(g,"existError","b")){var f=n.nodeValue(g,"errorCode");if("badDevType"===f?c.ipcConfig=a.getValue("modelMismatch"):"badLanguage"===f?c.ipcConfig=a.getValue("lanMismatch"):"chanNumReachLimit"===f?c.ipcConfig=a.getValue("channelNumLimit"):"importFail"===f&&(c.ipcConfig=a.getValue("importCfgFailed")),"badDevType"===f||"badLanguage"===f||"chanNumReachLimit"===f||"importFail"===f)return setTimeout(function(){c.ipcConfig="",d.$apply()},3e3),void 0;for(var h=[],v=0;15>v;v++)h[v]="";t(g).find("IpcError").each(function(){var e=n.nodeValue(this,"errorType"),t=n.nodeValue(this,"errorRowNo");"channelNoInvalid"===e?h[0]+=1===h[0].split(",").length?a.getValue("line")+t:","+t:"channelNoConflict"===e?h[1]+=1===h[1].split(",").length?a.getValue("line")+t:","+t:"channel IP/Domain invalid"===e?h[2]+=1===h[2].split(",").length?a.getValue("line")+t:","+t:"channel IP/Domain conflict"===e?h[3]+=1===h[3].split(",").length?a.getValue("line")+t:","+t:"channel IP conflict with local IP"===e?h[4]+=1===h[4].split(",").length?a.getValue("line")+t:","+t:"protocolError"===e?h[5]+=1===h[5].split(",").length?a.getValue("line")+t:","+t:"adminPortError"===e?h[6]+=1===h[6].split(",").length?a.getValue("line")+t:","+t:"channelError"===e?h[7]+=1===h[7].split(",").length?a.getValue("line")+t:","+t:"UserNameInvalid"===e?h[8]+=1===h[8].split(",").length?a.getValue("line")+t:","+t:"passwordInvalid"===e?h[9]+=1===h[9].split(",").length?a.getValue("line")+t:","+t:"transProtocalError"===e?h[10]+=1===h[10].split(",").length?a.getValue("line")+t:","+t:"ipcResolutionNotSupport"===e&&(h[11]+=1===h[11].split(",").length?a.getValue("line")+t:","+t)});for(var S="",v=0;15>v;v++)""!==h[v]&&(0===v?h[0]+=": "+a.getValue("channelNoError")+";":1===v?h[1]+=": "+a.getValue("channelNoConflict")+";":2===v?h[2]+=": "+a.getValue("ipAddrError")+"/"+a.getValue("domainError")+";":3===v?h[3]+=": "+a.getValue("ipAddrConflicted")+"/"+a.getValue("domainConflicted")+";":4===v?h[4]+=": "+a.getValue("ipAddrConflictWithDev")+";":5===v?h[5]+=": "+a.getValue("protocolError")+";":6===v?h[6]+=": "+a.getValue("managePortError")+";":7===v?h[7]+=": "+a.getValue("channelNoError")+";":8===v?h[8]+=": "+a.getValue("userPwdError")+";":9===v?h[9]+=": "+a.getValue("userPwdError")+";":10===v?h[10]+=": "+a.getValue("transProtocolError")+";":11===v&&(h[11]+=": "+a.getValue("resolutionNotSupported")+";")),S+=h[v];c.ipcConfig=""===S?a.getValue("importCfgFailed"):S,setTimeout(function(){c.ipcConfig="",d.$apply()},3e3)}else c.ipcConfig=a.getValue("importCfgSucceeded"),u.ipcConfig="success",setTimeout(function(){c.ipcConfig="",d.$apply()},3e3)}},exportIPCParam:function(){var e="",t="",n=-1;if(u.ipcExportParam="error",t=i.m_szHttpProtocol+i.m_szHostName+":"+i.m_iHttpPort+"/ISAPI/ContentMgmt/InputProxy/ipcConfig",t=WebSDK.getSecurityVersion(t),n=l.exportDeviceConfig(t,i.getPluginAuth(),"",1),0>n){var o=l.getLastError();e=403===o?a.getValue("noPermission"):a.getValue("exportCfgFailed"),c.ipcExportParam=e}else 0==n&&(c.ipcExportParam=a.getValue("exportCfgSucceeded"),u.ipcExportParam="success");setTimeout(function(){c.ipcExportParam="",d.$apply()},3e3)},startUpgrade:function($compile){var e=this;u.upgrade="error",o.confirm(a.getValue("upgradeTip"),300,function(){var t=m.upgrade,n="";if(""==t)c.upgrade=a.getValue("upgradeFailed"),setTimeout(function(){c.upgrade="",d.$apply()},3e3);else{var r=i.m_szHttpProtocol+i.m_szHostName+":"+i.m_iHttpPort+"/ISAPI/System/updateFirmware",s=i.m_szHttpProtocol+i.m_szHostName+":"+i.m_iHttpPort+"/ISAPI/System/upgradeStatus",u=l.startUpgradeEx(r,s,i.getPluginAuth(),t,p);if(-1==u){var h=l.getLastError();n=403===h?a.getValue("noPermission"):7003==h?a.getValue("deviceUpgrading"):7032==h?a.getValue("deviceFormat"):a.getValue("upgradeFailed"),c.upgrade=n,setTimeout(function(){c.upgrade="",d.$apply()},3e3)}else{c.upgrade="",d.iUpgradeProcess=0;var v='<div id="upgrade-slider" class="upgrade-slider"><div slider type="process" show-box="true" current-value="iUpgradeProcess" step="1" min="0" max="100"></div></div>';f=o.htmlNoButton(a.getValue("upgrading"),v),$compile(angular.element("#upgrade-slider"))(d),d.$apply(),g=setInterval(function(){d.$apply(function(){e.getUpgradeStatus()})},2e3)}}d.$apply()})},getUpgradeStatus:function(){var e=l.upgradeStatus();if(0==e){var t=l.upgradeProgress();0>t?(clearInterval(g),f.close(),c.upgrade=a.getValue("upgradeProgressFailed")):100>t?d.iUpgradeProcess=t:(clearInterval(g),l.stopUpgrade(),f.close(),c.upgrade=a.getValue("upgradeSucceeded"),u.upgrade="success",setTimeout(function(){c.upgrade="",d.$apply()},3e3),r.toRestart())}else{clearInterval(g),l.stopUpgrade(),f.close();var i="";i=1==e?a.getValue("upgradeProgressFailed"):2==e?a.getValue("lanMismatch"):4==e?a.getValue("deviceUpgrading"):5==e?a.getValue("noMemory"):6==e?a.getValue("deviceFormat"):7==e?a.getValue("upgradeFileMismatch"):8==e?a.getValue("upgradeVersionMismatch"):9==e?a.getValue("upgradeFailed"):a.getValue("upgradeStatusFailed"),c.upgrade=i}}},module.exports=new e});
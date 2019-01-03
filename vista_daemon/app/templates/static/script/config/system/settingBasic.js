define(function(require,exports,module){function e(){d={},c={oXmlDoc:null,szDeviceName:"",szDeviceNo:"",szDeviceModel:"",szSerialNo:"",szFirmwareVersion:"",szEncoderVersion:"",szWebVersion:seajs.web_version.replace("build"," build "),szPluginVersion:seajs.plugin_version,iChannelNum:0,iHDDNum:0,iAlarmInNum:0,iAlarmOutNum:0,bSupportOnlineUpgrade:!1,szNewFirmwareVersion:"",bHasNewVersion:!1,bNeedManualReboot:!1,szUpgradeStatus:"",iUpgradeProcess:0,szFirmwareVersionInfo:""}}var t,n,a,i,o,r,s,l,d,c,u,m,p,g;t=require("jquery"),require("ui.slider"),require("ui.jquery"),require("config/ui.config"),n=require("common"),a=require("translator"),i=require("utils"),o=require("dialog"),r=require("isapi/device"),s=require("isapi/response"),e.prototype={init:function(){var e=this;e.initController()},initController:function(){var e=this;angular.module("settingBasicApp",["ui.config","ui.jquery"]).controller("settingBasicController",function($scope,$timeout,$compile){l=$scope,g=$compile,e.inputValid(),e.getDeviceInfoCab(),$scope.oLan=a.oLastLanguage,$scope.oSettingBasicInfo=c,$scope.getOnlineUpgradeVersion=function(t){e.getOnlineUpgradeVersion(t)},$scope.startOnlineUpgrade=function(){e.startOnlineUpgrade()},$scope.save=function(){e.save()},$timeout(function(){e.getDeviceInfo(),e.getOnlineUpgradeCap()},10)}),angular.bootstrap(angular.element("#settingBasic"),["settingBasicApp"])},inputValid:function(){l.oUtils=i,l.oDeviceNameValid={oMaxLength:{value:32,error:a.getValue("noMoreLength",[32])}},l.oDeviceNoValid={oEmpty:{value:!1,error:a.getValue("nullTips")},oMinValue:{value:1,error:a.getValue("range",[1,255])},oMaxValue:{value:255,error:a.getValue("range",[1,255])}}},getDeviceInfoCab:function(){WebSDK.WSDK_GetDeviceConfig(n.m_szHostName,"deviceInfoCapa",null,{async:!1,success:function(e,n){d.iMin=parseInt(t(n).find("telecontrolID").eq(0).attr("min"),10),d.iMax=parseInt(t(n).find("telecontrolID").eq(0).attr("max"),10),l.oDeviceNoValid.oMinValue={value:d.iMin,error:a.getValue("range",[d.iMin,d.iMax])},l.oDeviceNoValid.oMaxValue={value:d.iMax,error:a.getValue("range",[d.iMin,d.iMax])}}})},getDeviceInfo:function(){var e=this;c.iChannelNum=0,c.iAlarmInNum=0,c.iAlarmOutNum=0,WebSDK.WSDK_GetDeviceConfig(n.m_szHostName,"deviceInfo",null,{success:function(n,a){var o=null;c.oXmlDoc=a,c.szDeviceName=i.nodeValue(a,"deviceName"),c.szDeviceNo=i.nodeValue(a,"telecontrolID"),c.szDeviceModel=i.nodeValue(a,"model"),c.szSerialNo=i.nodeValue(a,"serialNumber"),c.szFirmwareVersion=i.nodeValue(a,"firmwareVersion")+" "+i.nodeValue(a,"firmwareReleasedDate"),c.szEncoderVersion=i.nodeValue(a,"encoderVersion")+" "+i.nodeValue(a,"encoderReleasedDate"),o=r.getChannel(r.m_oChannelType.ANALOG),null!=o&&(c.iChannelNum=t(o).find("VideoInputChannel").length),o=r.getChannel(r.m_oChannelType.DIGITAL),null!=o&&(c.iChannelNum+=t(o).find("InputProxyChannel").length),c.iHDDNum=e.getHDDNum(),o=r.getAlarmInput(r.m_oAlarmInputType.ANALOG),c.iAlarmInNum=t(o).find("IOInputPort").length,o=r.getAlarmInput(r.m_oAlarmInputType.DIGITAL),c.iAlarmInNum+=t(o).find("IOProxyInputPort").length,o=r.getAlarmOutput(r.m_oAlarmOutputType.ANALOG),c.iAlarmOutNum=t(o).find("IOOutputPort").length,o=r.getAlarmOutput(r.m_oAlarmOutputType.DIGITAL),c.iAlarmOutNum+=t(o).find("IOProxyOutputPort").length,c.szFirmwareVersionInfo=i.nodeValue(a,"firmwareVersionInfo"),l.$apply()}})},getHDDNum:function(){var e=0;return WebSDK.WSDK_GetDeviceConfig(n.m_szHostName,"storage",null,{async:!1,success:function(n,a){e=t(a).find("hdd").length,t.each(t(a).find("nas"),function(){"offline"!==i.nodeValue(this,"status")&&e++})}}),e},getOnlineUpgradeCap:function(){var e=this;WebSDK.WSDK_GetDeviceConfig(n.m_szHostName,"onlineUpgradeCapa",null,{async:!1,success:function(t,n){c.bSupportOnlineUpgrade=!0,c.bNeedManualReboot="manual"==i.nodeValue(n,"rebootAfterUpgrade"),e.getOnlineUpgradeVersion(!1)},error:function(){c.bSupportOnlineUpgrade=!1,c.bNeedManualReboot=!1}})},getOnlineServerStatus:function(){WebSDK.WSDK_GetDeviceConfig(n.m_szHostName,"onlineServerStatus",null,{async:!1,success:function(){}})},getOnlineUpgradeVersion:function(e){var t=this;WebSDK.WSDK_GetDeviceConfig(n.m_szHostName,"onlineNewVersion",{custom:e},{async:!1,success:function(r,s){c.bHasNewVersion=i.nodeValue(s,"newVersionAvailable","b"),c.szNewFirmwareVersion=i.nodeValue(s,"newVersion"),c.bHasNewVersion?WebSDK.WSDK_GetDeviceConfig(n.m_szHostName,"onlineUpgradeStatus",null,{success:function(e,n){var e=i.nodeValue(n,"status");if("upgrading"===e){c.iUpgradeProcess=0;var r='<div id="upgrade-slider" class="upgrade-slider"><div slider type="process" show-box="true" current-value="oSettingBasicInfo.iUpgradeProcess" step="1" min="0" max="100"></div></div>';m=o.htmlNoButton(a.getValue("upgrading"),r),g(angular.element("#upgrade-slider"))(l),l.$apply(),u=setInterval(function(){l.$apply(function(){t.getUpgradeStatus()})},2e3)}}}):e&&o.tip(a.getValue("upgradeNoNewVersion")),l.$apply()}})},startOnlineUpgrade:function(){var e=this;o.confirm(a.getValue("upgradeTip"),300,function(){WebSDK.WSDK_SetDeviceConfig(n.m_szHostName,"onlineUpgrade",null,{success:function(){c.iUpgradeProcess=0;var t='<div id="upgrade-slider" class="upgrade-slider"><div slider type="process" show-box="true" current-value="oSettingBasicInfo.iUpgradeProcess" step="1" min="0" max="100"></div></div>';m=o.htmlNoButton(a.getValue("upgrading"),t),g(angular.element("#upgrade-slider"))(l),setTimeout(function(){u=setInterval(function(){l.$apply(function(){e.getUpgradeStatus()})},2e3),l.$apply()},3e3)},error:function(){c.szUpgradeStatus=a.getValue("upgradeFailed"),setTimeout(function(){c.szUpgradeStatus="",l.$apply()},5e3),l.$apply()}})})},getUpgradeStatus:function(){var e=this;WebSDK.WSDK_GetDeviceConfig(n.m_szHostName,"onlineUpgradeStatus",null,{success:function(t,n){var r=!1,t=i.nodeValue(n,"status"),d=i.nodeValue(n,"percent","i");"upgrading"===t?0>d?clearInterval(u):100>d?c.iUpgradeProcess=d:clearInterval(u):"successful"===t||100===d?(c.szUpgradeStatus=a.getValue("upgradeSucceeded"),clearInterval(u),m.close(),setTimeout(function(){c.szUpgradeStatus="",l.$apply()},8e3),c.bNeedManualReboot?s.toRestart():(p=o.wait(null,a.getValue("rebooting")),setTimeout(function(){var t=function(){e.getDeviceInfo(),e.getOnlineUpgradeVersion(!1)};s.reconnect(p,t)},6e4))):"languageMismatch"===t?(r=!0,c.szUpgradeStatus=a.getValue("lanMismatch")):"writeFlashError"===t?(r=!0,c.szUpgradeStatus=a.getValue("writeFlashError")):"packageTypeMismatch"===t?(r=!0,c.szUpgradeStatus=a.getValue("upgradeFileMismatch")):"packageVersionMismatch"===t?(r=!0,c.szUpgradeStatus=a.getValue("upgradeVersionMismatch")):"netUnreachable"===t?(r=!0,c.szUpgradeStatus=a.getValue("netUnreachable")):"upgradeServerErr"===t?(r=!0,c.szUpgradeStatus=a.getValue("upgradeServerError")):"readErr"===t?(r=!0,c.szUpgradeStatus=a.getValue("readError")):"noPackage"===t?(r=!0,c.szUpgradeStatus=a.getValue("noPackage")):"dataErr"===t?(r=!0,c.szUpgradeStatus=a.getValue("dataError")):"noMemory"===t?(r=!0,c.szUpgradeStatus=a.getValue("noMemory")):"deviceUpgrading"===t?(r=!0,c.szUpgradeStatus=a.getValue("upgradingError")):"notUpgrade"===t?(r=!0,c.szUpgradeStatus=a.getValue("upgradeFailed")):(r=!0,c.szUpgradeStatus=a.getValue("upgradeFailed")),r&&(clearInterval(u),m.close(),setTimeout(function(){c.szUpgradeStatus="",l.$apply()},5e3)),l.$apply()},error:function(){clearInterval(u),m.close(),c.szUpgradeStatus=a.getValue("upgradeProgressFailed"),setTimeout(function(){c.szUpgradeStatus="",l.$apply()},5e3),l.$apply()}})},save:function(){l.bInputValid&&(t(c.oXmlDoc).find("deviceName").eq(0).text(c.szDeviceName),t(c.oXmlDoc).find("telecontrolID").eq(0).text(c.szDeviceNo),WebSDK.WSDK_SetDeviceConfig(n.m_szHostName,"deviceInfo",null,{data:c.oXmlDoc,complete:function(e,t,n){s.saveState(n)}}))}},module.exports=new e});
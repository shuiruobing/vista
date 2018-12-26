define(function(require,exports,module){function e(e){var i=null,n=typeof e;if(i="string"===n?t.parseXmlFromStr(e):"object"===n?e:t.createXml(),this.m_oXmlDoc=i,this.m_oRootNode=null,this.m_oXmlDoc.childNodes.length>0)this.m_oRootNode=this.m_oXmlDoc.documentElement;else{var a=this.m_oXmlDoc.createProcessingInstruction("xml","version='1.0' encoding='utf-8'");this.m_oXmlDoc.appendChild(a)}}var t=require("utils");e.prototype={createNode:function(e,t){var i=this.m_oXmlDoc.createElement(e);if(t){var n=this.m_oXmlDoc.createTextNode(t);i.appendChild(n)}return i},insert:function(e,t){if(!e)return void 0;if("string"==typeof e){var i=e.indexOf(":");e=-1===i?this.createNode(e):this.createNode(e.substring(0,i),e.substring(i+1))}return"string"==typeof t&&(t=this.m_oRootNode.hasChildNodes(node)?this.m_oRootNode.getElementsByTagName(node)[0]:void 0),null===this.m_oRootNode?t===void 0?(this.m_oXmlDoc.appendChild(e),this.m_oRootNode=e):(this.m_oXmlDoc.appendChild(t),t.appendChild(e)):t===void 0?this.m_oRootNode.appendChild(e):t.appendChild(e),e},insertBefore:function(e,t){if(!e)return void 0;if("string"==typeof e){var i=e.indexOf(":");e=-1===i?this.createNode(e):this.createNode(e.substring(0,i),e.substring(i+1))}if("string"==typeof t){var n=t.split("->");t=this.m_oRootNode;for(var a=0;n.length>a;a++){if(0===t.getElementsByTagName(n[a]).length){t=void 0;break}t=t.getElementsByTagName(n[a])[0]}}return t===void 0?void 0:(this.m_oRootNode.insertBefore(e,t),e)},insertAfter:function(e,t){if(!e)return void 0;if("string"==typeof e){var i=e.indexOf(":");e=-1===i?this.createNode(e):this.createNode(e.substring(0,i),e.substring(i+1))}if("string"==typeof t){var n=t.split("->");t=this.m_oRootNode;for(var a=0;n.length>a;a++){if(0===t.getElementsByTagName(n[a]).length){t=void 0;break}t=t.getElementsByTagName(n[a])[0]}}return t===void 0?void 0:(null===t.nextSibling?t.parentNode.appendChild(e):this.m_oRootNode.insertBefore(e,t.nextSibling),e)},getNodeValue:function(e){if(!e)return"";if("string"==typeof e){var t=e.split("->");e=this.m_oRootNode;for(var i=0;t.length>i;i++){if(0===e.getElementsByTagName(t[i]).length){e=void 0;break}e=e.getElementsByTagName(t[i])[0]}}return e===void 0?"":e.childNodes.length>0?e.childNodes[0].nodeValue:""},setNodeValue:function(e,t){if(!e)return void 0;if("string"==typeof e){var i=e.split("->");e=this.m_oRootNode;for(var n=0;i.length>n;n++){if(0===e.getElementsByTagName(i[n]).length){e=void 0;break}e=e.getElementsByTagName(i[n])[0]}}return e===void 0?void 0:(e.childNodes.length>0&&(e.childNodes[0].nodeValue=t),e)},removeNode:function(e){if(e){if("string"==typeof e){var t=e.split("->");e=this.m_oRootNode;for(var i=0;t.length>i;i++){if(0===e.getElementsByTagName(t[i]).length){e=void 0;break}e=e.getElementsByTagName(t[i])[0]}}void 0!==e&&this.m_oRootNode.removeChild(e)}}},module.exports=e});
(function(){function e(e){var t=15&e>>>4;return t.toString(16)+(15&e).toString(16)}function t(e){var t=e.charCodeAt(0);if(t>=48&&57>=t)t-=48;else if(t>=65&&70>=t)t-=55;else{if(!(t>=97&&102>=t))return window.alert(e.charAt(1)+" is not a valid hex digit"),-1;t-=87}var i=e.charCodeAt(1);if(i>=48&&57>=i)i-=48;else if(i>=65&&70>=i)i-=55;else{if(!(i>=97&&102>=i))return window.alert(e.charAt(2)+" is not a valid hex digit"),-1;i-=87}return 16*t+i}function i(e,t){return 4*e+t}function n(e,i){var n,a=Array(16);if(i)if(e.length>16&&window.alert("is too long, using the first 16 ASCII characters"),e.length>=16)for(n=0;16>n;n++)a[n]=e.charCodeAt(n);else{for(n=0;e.length>n;n++)a[n]=e.charCodeAt(n);for(n=e.length;16>n;n++)a[n]=0}else{if(32!=e.length)return window.alert("length wrong: Is "+e.length+" hex digits, but must be 128 bits (32 hex digits)"),a[0]=-1,a;for(n=0;16>n;n++)if(a[n]=t(e.substr(2*n,2)),0>a[n])return a[0]=-1,a}return a}function a(e,t){for(var i=0;e>0;)0!=(1&e)&&(i^=t),e>>>=1,t<<=1;for(var n=65536,a=72448;n>=256;)0!=(i&n)&&(i^=a),n>>=1,a>>=1;return i}function o(e){var t;for(t=0;16>t;t++)e[t]=R[e[t]];return e}function r(e){return[e[1],e[2],e[3],e[0]]}function s(e){var t=2,i=1;for(e--;e>0;)0!=(1&e)&&(i=a(i,t)),t=a(t,t),e>>=1;return i}function l(e){var t,i,n=Array(4),a=Array(4*(P+1));for(t=0;16>t;t++)a[t]=e[t];for(t=4;4*(P+1)>t;){for(i=0;4>i;i++)n[i]=a[4*(t-1)+i];for(0==t%4&&(n=r(n),n=o(n),n[0]^=s(t>>>2)),i=0;4>i;i++)a[4*t+i]=a[4*(t-4)+i]^n[i];t++}return a}function c(e,t){var i;for(i=0;16>i;i++)e[i]=t[e[i]];return e}function u(e){var t,i,n,a;return t=e[C],i=e[b],n=e[I],a=e[_],e[C]=i,e[b]=n,e[I]=a,e[_]=t,t=e[y],i=e[T],n=e[D],a=e[L],e[y]=n,e[T]=a,e[D]=t,e[L]=i,t=e[A],i=e[V],n=e[x],a=e[N],e[A]=a,e[V]=t,e[x]=i,e[N]=n,e}function d(e){var t,i,n,a;return t=e[C],i=e[b],n=e[I],a=e[_],e[C]=a,e[b]=t,e[I]=i,e[_]=n,t=e[y],i=e[T],n=e[D],a=e[L],e[y]=n,e[T]=a,e[D]=t,e[L]=i,t=e[A],i=e[V],n=e[x],a=e[N],e[A]=i,e[V]=n,e[x]=a,e[N]=t,e}function m(e){var t,n,o,r,s;for(t=0;4>t;t++)n=e[i(0,t)],o=e[i(1,t)],r=e[i(2,t)],s=e[i(3,t)],e[i(0,t)]=a(2,n)^a(3,o)^r^s,e[i(1,t)]=n^a(2,o)^a(3,r)^s,e[i(2,t)]=n^o^a(2,r)^a(3,s),e[i(3,t)]=a(3,n)^o^r^a(2,s);return e}function p(e){var t,n,o,r,s;for(t=0;4>t;t++)n=e[i(0,t)],o=e[i(1,t)],r=e[i(2,t)],s=e[i(3,t)],e[i(0,t)]=a(14,n)^a(11,o)^a(13,r)^a(9,s),e[i(1,t)]=a(9,n)^a(14,o)^a(11,r)^a(13,s),e[i(2,t)]=a(13,n)^a(9,o)^a(14,r)^a(11,s),e[i(3,t)]=a(11,n)^a(13,o)^a(9,r)^a(14,s);return e}function g(e,t,n){var a;for(a=0;4>a;a++)e[i(0,a)]^=t[n+4*a],e[i(1,a)]^=t[n+4*a+1],e[i(2,a)]^=t[n+4*a+2],e[i(3,a)]^=t[n+4*a+3];return e}function f(e){var t,n,a=Array(16);for(t=0;4>t;t++)for(n=0;4>n;n++)a[i(t,n)]=e[i(n,t)];return a}function h(t){var i,n="";if(t)for(i=0;16>i;i++)n+=String.fromCharCode(M[i]);else for(n=e(M[0]),i=1;16>i;i++)n+=""+e(M[i]);return n}function v(e,t,i){var a,o=Array(4*(P+1)),r=Array(16),s=n(e,i);if(!(0>s[0])){var t=n(t,!1);if(!(0>t[0])){for(o=l(t),r=f(s),r=g(r,o,0),a=1;P>a;a++)r=c(r,R),r=u(r),r=m(r),r=g(r,o,4*4*a);c(r,R),u(r),g(r,o,4*4*P),M=f(r);var d=h(!i);return d}}}function S(e,t,i){var a,o=Array(4*(P+1)),r=Array(16),s=n(e,i);if(!(0>s[0])){var t=n(t,!1);if(!(0>t[0])){for(o=l(t),r=f(s),r=g(r,o,4*4*P),a=P-1;a>=1;a--)r=d(r),r=c(r,k),r=g(r,o,4*4*a),r=p(r);d(r),c(r,k),g(r,o,0),M=f(r);var u=h(!i);return u}}}var P=10,C=4,b=5,I=6,_=7,y=8,T=9,D=10,L=11,A=12,V=13,x=14,N=15,R=[99,124,119,123,242,107,111,197,48,1,103,43,254,215,171,118,202,130,201,125,250,89,71,240,173,212,162,175,156,164,114,192,183,253,147,38,54,63,247,204,52,165,229,241,113,216,49,21,4,199,35,195,24,150,5,154,7,18,128,226,235,39,178,117,9,131,44,26,27,110,90,160,82,59,214,179,41,227,47,132,83,209,0,237,32,252,177,91,106,203,190,57,74,76,88,207,208,239,170,251,67,77,51,133,69,249,2,127,80,60,159,168,81,163,64,143,146,157,56,245,188,182,218,33,16,255,243,210,205,12,19,236,95,151,68,23,196,167,126,61,100,93,25,115,96,129,79,220,34,42,144,136,70,238,184,20,222,94,11,219,224,50,58,10,73,6,36,92,194,211,172,98,145,149,228,121,231,200,55,109,141,213,78,169,108,86,244,234,101,122,174,8,186,120,37,46,28,166,180,198,232,221,116,31,75,189,139,138,112,62,181,102,72,3,246,14,97,53,87,185,134,193,29,158,225,248,152,17,105,217,142,148,155,30,135,233,206,85,40,223,140,161,137,13,191,230,66,104,65,153,45,15,176,84,187,22],k=[82,9,106,213,48,54,165,56,191,64,163,158,129,243,215,251,124,227,57,130,155,47,255,135,52,142,67,68,196,222,233,203,84,123,148,50,166,194,35,61,238,76,149,11,66,250,195,78,8,46,161,102,40,217,36,178,118,91,162,73,109,139,209,37,114,248,246,100,134,104,152,22,212,164,92,204,93,101,182,146,108,112,72,80,253,237,185,218,94,21,70,87,167,141,157,132,144,216,171,0,140,188,211,10,247,228,88,5,184,179,69,6,208,44,30,143,202,63,15,2,193,175,189,3,1,19,138,107,58,145,17,65,79,103,220,234,151,242,207,206,240,180,230,115,150,172,116,34,231,173,53,133,226,249,55,232,28,117,223,110,71,241,26,113,29,41,197,137,111,183,98,14,170,24,190,27,252,86,62,75,198,210,121,32,154,219,192,254,120,205,90,244,31,221,168,51,136,7,199,49,177,18,16,89,39,128,236,95,96,81,127,169,25,181,74,13,45,229,122,159,147,201,156,239,160,224,59,77,174,42,245,176,200,235,187,60,131,83,153,97,23,43,4,126,186,119,214,38,225,105,20,99,85,33,12,125],M=Array(16);window.aes_encrypt=v,window.aes_decrypt=S})();
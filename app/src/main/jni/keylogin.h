#include <json/json.hpp>
#include <ctime>
#include <iomanip>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <sstream>
#include <random>

#include "include/obfuscate.h"

// ===== SERVER CONFIG - Edit these =====
#define SERVER_HOST "127.0.0.1"
#define SERVER_PORT 3001
// ======================================

#define OPAQUE_TRUE  ((int)(&junk_buf + 1 - &junk_buf + 1) > 0)

static char junk_buf[64]={0};
unsigned long g_junk=0xDEADBEEF;

bool bValid=false;

static void j0(){volatile int x[4]={1,2,3,4};for(int i=0;i<4;i++)g_junk^=x[i]<<i;}
static void j1(){volatile unsigned long p=0;for(int i=0;i<100;i++)p^=i*i;g_junk+=p;}
static void j2(){volatile double d=3.14159;for(int i=0;i<50;i++)d*=1.0001;g_junk^=(unsigned long)d;}
static void j3(){volatile int y[8];for(int i=0;i<8;i++)y[i]=i*7;for(int i=7;i>=0;i--)g_junk^=y[i]<<(i*2);}
static void j4(){volatile short z[16];for(int i=0;i<16;i++)z[i]=i*3;for(int i=0;i<16;i+=2)g_junk^=z[i]<<i;}
static void j5(){volatile unsigned long long q=0xCAFEBABE;for(int r=0;r<20;r++)q=(q<<5)^(q>>3)+r;g_junk^=(unsigned long)q;}
static void j6(){int t=0;for(int i=0;i<200;i++){t+=i;t^=t>>3;if(t>999)break;}g_junk+=t;}
static void j7(){unsigned long w=1;for(int k=0;k<64;k++)w*=3;g_junk^=w;}

static std::string x0(const std::string& a,const std::string& b){
    j1();j3();std::string r;r.reserve(a.size());
    if(OPAQUE_TRUE){for(size_t i=0;i<a.size();++i)r+=a[i]^b[i%b.length()];}else{j0();}
    j2();return r;
}
static std::string x1(const std::string& a,const std::string& b){
    j5();auto r=x0(a,b);j6();return r;
}

static std::string b0(const std::string& d){
    j0();std::string r;size_t i=0;unsigned char a3[3],a4[4];
    std::string c=OO("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/").str();
    while(d.length()-i>=3){
        a3[0]=d[i];a3[1]=d[i+1];a3[2]=d[i+2];
        a4[0]=(a3[0]&0xfc)>>2;a4[1]=((a3[0]&0x03)<<4)+((a3[1]&0xf0)>>4);
        a4[2]=((a3[1]&0x0f)<<2)+((a3[2]&0xc0)>>6);a4[3]=a3[2]&0x3f;
        for(int jx=0;jx<4;jx++)r+=c[a4[jx]];i+=3;j1();
    }
    j3();
    if(d.length()-i>0){
        j4();int rem=d.length()-i;
        for(int j=0;j<3;j++)a3[j]=(j<rem)?(unsigned char)d[i+j]:0;j5();
        a4[0]=(a3[0]&0xfc)>>2;a4[1]=((a3[0]&0x03)<<4)+((a3[1]&0xf0)>>4);
        a4[2]=((a3[1]&0x0f)<<2)+((a3[2]&0xc0)>>6);a4[3]=a3[2]&0x3f;
        for(int j=0;j<rem+1;j++)r+=c[a4[j]];
        while(r.length()%4)r+='=';
    }
    j6();return r;
}

static std::string b1(const std::string& in){
    j7();std::string r;std::vector<int>T(256,-1);
    std::string cs=OO("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/").str();
    for(int i=0;i<64;i++)T[(unsigned char)cs[i]]=i;
    int v=0,vb=-8;j0();
    for(unsigned char ch:in){
        if(T[ch]==-1)break;
        v=(v<<6)+T[ch];vb+=6;
        if(vb>=0){r.push_back((v>>vb)&0xFF);vb-=8;j2();}
    }
    j4();return r;
}

struct WSF{std::string d;bool ok;};

static std::string gWK(){
    j6();std::string k;std::random_device rd;std::mt19937 gen(rd());std::uniform_int_distribution<> dis(0,63);
    std::string cs=OO("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/").str();
    for(int i=0;i<16;i++){k+=cs[dis(gen)];j3();}
    auto r=b0(k);j7();return r;
}

static std::string cWF(const std::string& m){
    j0();std::string f;f+=(char)0x81;size_t l=m.length();
    if(l<=125)f+=(char)(0x80|l);
    else if(l<=65535){f+=(char)(0x80|126);f+=(char)((l>>8)&0xFF);f+=(char)(l&0xFF);}
    else{f+=(char)(0x80|127);for(int i=7;i>=0;i--){f+=(char)((l>>(i*8))&0xFF);j5();}}
    unsigned char mask[4];std::random_device rd;std::mt19937 gen(rd());std::uniform_int_distribution<> dis(0,255);
    for(int i=0;i<4;i++){mask[i]=dis(gen);f+=mask[i];}j1();
    for(size_t i=0;i<m.length();i++){f+=m[i]^mask[i%4];j4();}
    j2();return f;
}

static WSF pWF(const char* d,size_t l){
    WSF r;r.ok=false;if(l<2)return r;j3();
    size_t pl=d[1]&0x7F,hl=2;j6();
    if(pl==126){if(l<4)return r;pl=((unsigned char)d[2]<<8)|(unsigned char)d[3];hl=4;j5();}
    else if(pl==127){if(l<10)return r;pl=0;for(int i=0;i<8;i++){pl=(pl<<8)|(unsigned char)d[2+i];j0();}hl=10;}
    if(l<hl+pl)return r;j7();r.d=std::string(d+hl,pl);r.ok=true;return r;
}

static int cWS(const char* h,int p,int to=10){
    int s=socket(AF_INET,SOCK_STREAM,0);if(s<0)return -1;
    struct timeval tv;tv.tv_sec=to;tv.tv_usec=0;
    setsockopt(s,SOL_SOCKET,SO_RCVTIMEO,&tv,sizeof(tv));
    setsockopt(s,SOL_SOCKET,SO_SNDTIMEO,&tv,sizeof(tv));
    struct sockaddr_in sa;memset(&sa,0,sizeof(sa));
    sa.sin_family=AF_INET;sa.sin_port=htons(p);j2();
    if(inet_pton(AF_INET,h,&sa.sin_addr)<=0){close(s);return -1;}
    if(connect(s,(struct sockaddr*)&sa,sizeof(sa))<0){close(s);return -1;}
    std::string wk=gWK();std::stringstream req;j4();
    req<<OO("GET / HTTP/1.1\r\n").str()<<OO("Host: ").str()<<h<<OO(":").str()<<p<<OO("\r\n").str();
    req<<OO("Upgrade: websocket\r\n").str()<<OO("Connection: Upgrade\r\n").str();
    req<<OO("Sec-WebSocket-Key: ").str()<<wk<<OO("\r\n").str()<<OO("Sec-WebSocket-Version: 13\r\n\r\n").str();
    std::string rs=req.str();j6();
    if(send(s,rs.c_str(),rs.length(),0)<0){close(s);return -1;}
    char buf[1024];ssize_t rec=recv(s,buf,sizeof(buf)-1,0);j0();
    if(rec<=0){close(s);return -1;}buf[rec]='\0';j1();
    if(strstr(buf,OO("101").str().c_str())==nullptr){close(s);return -1;}
    return s;
}

static std::string sWS(int s,const std::string& m,int to=10){
    std::string f=cWF(m);if(send(s,f.c_str(),f.length(),0)<0)return"";
    char buf[65536];ssize_t rec=recv(s,buf,sizeof(buf),0);if(rec<=0)return"";
    WSF wf=pWF(buf,rec);if(!wf.ok)return"";return wf.d;
}

static std::string dc(const std::string& ed,const std::string& k){
    try{
        auto j=nlohmann::json::parse(ed);
        auto dk=OO("data").str();
        if(!j.contains(dk)||!j[dk].is_string())return"";
        return x1(b1(j[dk].get<std::string>()),k);
    }catch(...){return"";}
}

std::string ERROR_MESSAGE="";
static bool logged_in=false,is_logging_in=false;
std::string g_Token,g_Auth,g_ExpTime="N/A";

static int p0(int sock,const std::string& ws_token){
    auto rk=OO("register").str();auto tk=OO("token").str();
    nlohmann::json rp={{rk,true},{tk,ws_token}};
    std::string rr=sWS(sock,rp.dump());
    if(rr.empty()){ERROR_MESSAGE=OO("WebSocket register failed").str();return -1;}
    try{
        nlohmann::json rj=nlohmann::json::parse(rr);
        auto sk=OO("success").str();
        if(!rj.contains(sk)||rj[sk]!=true){
            ERROR_MESSAGE=rj.value(OO("error").str(),OO("Register failed").str());return -1;
        }
    }catch(...){ERROR_MESSAGE=OO("Register response parse error").str();return -1;}
    return 0;
}

static int p1(int sock,const std::string& key,const std::string& aid,
              const std::string& gt,const std::string& ver,
              const std::string& ek,const std::string& wt){
    auto lk=OO("license_key").str();auto hk=OO("hwid").str();
    auto gk=OO("game_type").str();auto vk=OO("version").str();
    auto tk=OO("token").str();auto dk=OO("data").str();

    nlohmann::json ap={{lk,key},{hk,aid},{gk,gt},{vk,ver}};
    std::string jp=ap.dump();std::string enc=x0(jp,ek);
    std::string ed=b0(enc);
    nlohmann::json fp={{tk,wt},{dk,ed}};
    std::string ar=sWS(sock,fp.dump());close(sock);
    if(ar.empty()){ERROR_MESSAGE=OO("WebSocket auth request failed").str();return -2;}
    std::string dr=dc(ar,ek);
    if(dr.empty()){ERROR_MESSAGE=OO("Failed to decrypt server response").str();return -3;}
    try{
        nlohmann::json jr=nlohmann::json::parse(dr);
        auto sk=OO("status").str();auto suk=OO("success").str();
        if(!jr.contains(sk)||jr[sk]!=suk){
            auto mk=OO("message").str();
            ERROR_MESSAGE=jr.contains(mk)?jr[mk].get<std::string>():OO("Server returned unknown error").str();
            return -4;
        }
        auto dk_=OO("data").str();
        if(jr.contains(dk_)){
            auto d=jr[dk_];std::string edate,dver;
            auto ek_=OO("expiry_date").str();auto vk_=OO("version").str();
            auto alk=OO("auth_token").str();auto lkk=OO("license_key").str();
            try{edate=d.value(ek_,OO("").str());}catch(...){edate=OO("").str();}
            try{dver=d.value(vk_,OO("").str());}catch(...){dver=OO("").str();}
            if(dver!=ver){ERROR_MESSAGE=OO("Your version is old, please update it.").str();return -5;}
            if(d.contains(lkk)&&d[lkk]!=key){
                ERROR_MESSAGE=OO("Received Bad Data From Server").str();return -6;
            }
            g_Token=OO("0wQRlDkgoQlf").str();g_Auth=OO("0wQRlDkgoQlf").str();
            g_ExpTime=edate.empty()?OO("N/A").str():edate;
            bValid=g_Token==g_Auth;return 0;
        }else{ERROR_MESSAGE=OO("Response missing 'data' field").str();return -7;}
    }catch(const std::exception& e){ERROR_MESSAGE=OO("Error: ").str()+e.what();return -8;}
}

INLINE bool Login(std::string androidID,std::string key){
    std::string version=OO("1.0").str();std::string gametype=OO("8ball").str();
    std::string ek=OO("JiM21rNU12eERlNmpqa3FuQks").str();
    std::string tk=OO("kdJw32jdnFkaQfObkkDcTy").str();g_junk^=tk.length();
    std::string wt=OO("KJGMDKFJDHG34KD").str();

    if(androidID.empty()){ERROR_MESSAGE=OO("Could not get Android ID").str();return false;}
    if(key.empty()){ERROR_MESSAGE=OO("Key Is Empty or Failed to get Key").str();return false;}

    is_logging_in=true;ERROR_MESSAGE="";int rc=0;
    std::string h=SERVER_HOST;int p=SERVER_PORT;

    int sock=cWS(h.c_str(),p,15);
    if(sock<0){ERROR_MESSAGE=OO("WebSocket connection failed").str();is_logging_in=false;j0();return false;}

    rc=p0(sock,wt);
    if(rc<0){close(sock);is_logging_in=false;j1();return false;}

    rc=p1(sock,key,androidID,gametype,version,ek,wt);
    if(rc==0){logged_in=true;is_logging_in=false;{auto kk=OO("key").str();persistent_string[kk]=key;}save_persistence();return true;}

    is_logging_in=false;return false;
}

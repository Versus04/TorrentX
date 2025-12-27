#include <iostream>
#include <fstream>
#include <vector>
#include "sha1.h"

#include "parser.cpp"
char hex_tostring(uint16_t& hex)
{
    char hex_char =' ';
    if(hex<10)
    {
        hex_char = '0'+hex;
    }
    else hex_char= 'A'+(hex-10);
    return hex_char;
}
bool is_http(std::string& s)
{
    if(s.substr(0,4)=="http")return true;
    return false;
}
void missing_announce(std::string& s)
{
    size_t pos = s.find("/announce");
    if(pos==std::string::npos)
    {
        s+="/announce";
    }
    
}
int main()
{
    std::ifstream file("Whiplash.torrent",std::ios_base::binary);
    if (!file) {
        std::cerr << "Error opening file: " << "Whiplash.torrent" << std::endl;
    
    }
    std::string content((std::istreambuf_iterator<char>(file)),
                         std::istreambuf_iterator<char>());
    file.close();
     i=0;
    data = content;
    
    Bvalue root = master_parser();
   Bvalue info = root.dict["info"];
    std::string ans = "";
    ans=serializer(info);
    Bvalue links = root.dict["announce-list"];
    std::string link="";
    for(auto it : links.list)
    {
        for(auto s : it.list)
        {
            if(is_http(s.s)){link=s.s;break;}
        }
    }
    missing_announce(link);
   // std::cout<<"Selected Tracker : "<<link<<std::endl;
    
 SHA1_CTX ctx;
uint8_t hash[20];

SHA1Init(&ctx);
SHA1Update(&ctx,
    reinterpret_cast<const uint8_t*>(ans.data()),
    ans.size());
SHA1Final(hash, &ctx);

// print hex
/*for (int i = 0; i < 20; i++)
    printf("%02x", hash[i]);
printf("\n");
*/
std::string info_hash ="";

for(int i =0 ;i < 20 ;i++)
{
    uint8_t b = hash[i];
    uint16_t high = (b>>4)&0xF;
    uint16_t low = b&0xF;
    
    info_hash+="%";
    info_hash+=hex_tostring(high);
    info_hash+=hex_tostring(low);
}
    //std::cout<<info_hash<<std::endl;
    char peer_id_str[]  = "-TX0001-123456789012";
    std::string peer_id="";
    for(int i =0 ;i < 20;i++)
    {
        uint8_t a = static_cast<uint8_t>(peer_id_str[i]);
        uint16_t high = static_cast<uint16_t>((a>>4)&0xF);
        
        uint16_t low = static_cast<uint16_t>(a&0xF);
        peer_id+="%";
        peer_id+=hex_tostring(high);
        
        peer_id+=hex_tostring(low);
    }
   // std::cout<<"Peer id : "<<peer_id<<std::endl;
   bool multi_file=false;
  if(info.dict.find("files")!=info.dict.end())
  {
   multi_file=true;
  }
  int left =0;
  if(!multi_file)left=info.dict["length"].i ;
  else{
    for(auto it : info.dict["files"].list){
        left+=it.dict["length"].i;
    }
  }
std::string finallink = link+"?info_hash="+info_hash+"&peer_id="
+peer_id+"&port=6881"+
"&uploaded=0&downloaded=0&left="
+std::to_string(left)+"&compact=1&event=started";
std::cout<<finallink;
    if(i!=data.size())
    {
         throw std::runtime_error("Trailing bytes after valid torrent");
    }
    return 0;
}

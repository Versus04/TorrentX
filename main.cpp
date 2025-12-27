#include <iostream>
#include <fstream>
#include <vector>
#include "sha1.h"

#include "parser.cpp"
bool is_http(std::string& s)
{
    if(s.substr(0,4)=="http")return true;
    return false;
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
   Bvalue s = root.dict["info"];
    std::string ans = "";
    ans=serializer(s);
    Bvalue links = root.dict["announce-list"];
    for(auto it : links.list)
    {
        for(auto s : it.list)
        {
            if(is_http(s.s))std::cout<<s.s<<std::endl;
        }
    }
    
    /* SHA1_CTX ctx;
uint8_t hash[20];

SHA1Init(&ctx);
SHA1Update(&ctx,
    reinterpret_cast<const uint8_t*>(ans.data()),
    ans.size());
SHA1Final(hash, &ctx);

// print hex
for (int i = 0; i < 20; i++)
    printf("%02x", hash[i]);
printf("\n");
*/




    if(i!=data.size())
    {
         throw std::runtime_error("Trailing bytes after valid torrent");
    }
    return 0;
}
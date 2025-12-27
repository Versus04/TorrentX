#include <vector>
#include <map>
#include <String>
#include <vector>
#include <stdexcept>
std::string data=" ";
enum class Type{
  INT , BSTRING, LIST , DICT   
};
size_t i=0;
 class Bvalue{
    public:
    Type type;
    int i;
    std::string s;
    std::vector<Bvalue> list;
    std::map<std::string,Bvalue> dict;
    public:
    Bvalue() : type(Type::INT), i(0) {}
    Bvalue(int value)
    {
        type=Type::INT;
        i=value;
    };
    Bvalue(std::string value)
    {
        type=Type::BSTRING;
        s=value;
    };
    Bvalue(std::vector<Bvalue> l)
    {
        type=Type::LIST;
        list=l;
    }
    Bvalue(std::map<std::string,Bvalue> d)
    {
        type=Type::DICT;
        dict = d;
    };
};
Bvalue master_parser();
std::vector<Bvalue> parse_list()
{
    std::vector<Bvalue> list;
    while(data[i]!='e')
    {
        list.push_back(master_parser());  
    }
    i++; //skip e
    return list;
}
int parse_int()
{
  
    int answer =0;
    bool neg =false;
    if(data[i]=='-'){neg=true;i++;}
    while((std::isdigit(static_cast<unsigned char>(data[i]))))
    {
        answer = answer*10+(data[i]-'0');
        i++;
    }
    i++; //skip e
    return neg?-answer:answer;
}
std::string parse_string(int len)
{
    std::string s="";
    for(int j=0;j<len;j++)
    {
        s+=data[i];
        i++;
    }
    return s;
}

std::map<std::string,Bvalue> parse_dict()
{
    std::map<std::string,Bvalue> dict;
    while(data[i]!='e')
    {
    int len=0;
    while((std::isdigit(static_cast<unsigned char>(data[i]))))
    {
        len = len*10+(data[i]-'0');
        i++;
    }
    i++; //skip semicolon
    std::string key = parse_string(len);
    dict.emplace(key, master_parser());
    
    }
    i++; //skip e
    return dict;
}
Bvalue master_parser()
{
    if(data[i]=='d')
    {
        i++; //skip d 
        
        return  Bvalue(parse_dict());
    }
    else if(data[i]=='i')
    {
        i++; //skip i
        return Bvalue(parse_int());
    }
    else if(data[i]=='l')
    {
        i++; //skip l
        return Bvalue(parse_list());
    }
    else if((std::isdigit(static_cast<unsigned char>(data[i]))))
    {
        int len=0;
    while((std::isdigit(static_cast<unsigned char>(data[i]))))
    {
        len = len*10+(data[i]-'0');
        i++;
    }
        i++; //skip semicolon 
        return Bvalue(parse_string(len));
    }
   throw std::runtime_error(
        "Invalid bencode at position " + std::to_string(i) +
        " (char: '" + std::string(1, data[i]) + "')"
    );
}
std::string serializer(const Bvalue& b)
{
    std::string s="";
    if(b.type==Type::INT)
    {
        s+='i';
        s+=std::to_string(b.i);
        s+='e';
    }
    else if(b.type==Type::BSTRING)
    {
        s+=std::to_string(b.s.size());
        s+=':'+b.s;
    }
    else if(b.type==Type::LIST)
    {
        s+='l';
        for(auto it : b.list)
        {
            s+=serializer(it);
        }
        s+='e';
    }
    else if(b.type==Type::DICT)
    {
        s+='d';
        for(auto it : b.dict)
        {
            s+=std::to_string(it.first.size());
            s+=':'+it.first;
            s+=serializer(it.second);
        }
        s+='e';
    }
    return s;
}


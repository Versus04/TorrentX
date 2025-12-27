#include "parser.cpp"
std::string serializer(const Bvalue& b)
{
    std::string s ="";
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
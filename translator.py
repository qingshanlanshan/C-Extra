import sys
import re
import logging
# usage 
# python translator.py <srcfile> (<dstfile>)
# srcfile: input file
# dstfile: line file (optional)
if len(sys.argv) < 2:
    print("usage: python translator.py <srcfile> (<dstfile>)")
    sys.exit(1)
    
logging.basicConfig(stream=sys.stderr, level=logging.DEBUG)   
def debug(msg):
    logging.debug(msg)
    
srcfile = sys.argv[1]
dstfile = srcfile + ".c"
if len(sys.argv) == 3:
    dstfile = sys.argv[2]

class vector:
    name:str
    type:str
    def __init__(self):
        pass
vector_map:dict[str,vector] = {}

class string:
    name:str
    def __init__(self):
        pass
string_map:dict[str,string] = {}

class hashmap:
    name:str
    key_type:str
    value_type:str
    def __init__(self):
        pass
hashmap_map:dict[str,hashmap] = {}

def multi_split(string:str, delimiters:list):
    for delimiter in delimiters:
        string = " ".join(string.split(delimiter))
    return string.split()

f=open(srcfile, "r")
lines = f.readlines()

index=-1
line="#include \"../include/cext.h\"\n"
item_cnt=0
with open(dstfile, "w") as f:
    while True:
        f.write(line)
        index += 1
        if index > len(lines)-1:
            break
        line = lines[index]
        
        matchObj = re.findall(r'([a-zA-Z_]\w*).get\(([a-zA-Z_]\w*)\)',line)
        if len(matchObj)>0:
            debug("get {}".format(matchObj))
            for m in matchObj:
                if m[0] in hashmap_map:
                    name=m[0]
                    key=m[1]
                    line=re.sub(r'([a-zA-Z_]\w*).get\(([a-zA-Z_]\w*)\)', f"hashmap_get({name},{key})", line,count=1)
                    
        matchObj = re.findall(r'([a-zA-Z_]\w*).size\(\)',line)
        if len(matchObj)>0:
            debug("get {}".format(matchObj))
            for m in matchObj:
                if m in hashmap_map:
                    line=re.sub(r'([a-zA-Z_]\w*).size\(\)', f"hashmap_size({m})", line,count=1)
                elif m in vector_map:
                    line=re.sub(r'([a-zA-Z_]\w*).size\(\)', f"vector_size({m})", line,count=1)
                elif m in string_map:
                    line=re.sub(r'([a-zA-Z_]\w*).size\(\)', f"string_size({m})", line,count=1)            
        
        matchObj = re.findall( r'([a-zA-Z_]\w*).len\(\)', line)
        if len(matchObj)>0:
            debug("len {}".format(matchObj))
            for m in matchObj:
                if m in vector_map:
                    v=vector_map[m]
                    line=re.sub(r'([a-zA-Z_]\w*).len\(\)', f"vector_len({v.name})", line,count=1)
                elif m in string_map:
                    s=string_map[m]
                    line=re.sub(r'([a-zA-Z_]\w*).len\(\)', f"string_len({s.name})", line,count=1)
        
        matchObj = re.findall( r'([a-zA-Z_]\w*).compare\(([a-zA-Z_]\w*)\)', line)
        if len(matchObj)>0:
            debug("compare {}".format(matchObj))
            for m in matchObj:
                if m[0] in string_map and m[1] in string_map:
                    s1=string_map[m[0]]
                    s2=string_map[m[1]]
                    line=re.sub(r'([a-zA-Z_]\w*).compare\(([a-zA-Z_]\w*)\)', f"string_compare({s1.name},string_get({s2.name}))", line,count=1)
                    
        matchObj = re.findall( r'([a-zA-Z_]\w*).compare\(\"([^\"]*)\"\)', line)
        if len(matchObj)>0:
            debug("compare {}".format(matchObj))
            for m in matchObj:
                if m[0] in string_map:
                    s1=string_map[m[0]]
                    line=re.sub(r'([a-zA-Z_]\w*).compare\(([a-zA-Z_]\w*)\)', f"string_compare({s1.name},{m[1]})", line,count=1)

        # at  
        matchObj = re.findall( r'([a-zA-Z_]\w*)\.at\((\d+)\)', line)
        if len(matchObj)>0:
            for m in matchObj:
                if m[0] in vector_map:
                    v=vector_map[m[0]]
                    idx=m[1]
                    line=re.sub(r'([a-zA-Z_]\w*)\.at\((\d+)\)', f"*(*{v.type})vector_at({v.name},{idx})", line,count=1)
                elif m[0] in string_map:
                    s=string_map[m[0]]
                    idx=m[1]
                    line=re.sub(r'([a-zA-Z_]\w*)\.at\((\d+)\)', f"string_at({s.name},{idx})", line,count=1)
        
        # back
        matchObj = re.findall( r'([a-zA-Z_]\w*)\.back\(\)', line)
        if len(matchObj)>0:
            output=""
            for m in matchObj:
                if m not in vector_map:
                    continue
                debug("back {}".format(m))
                v=vector_map[m]
                output+=f"{v.type} value_back_{item_cnt};\n if(vector_back({m},&value_back_{item_cnt})){{fprintf(stderr, \"Error at %s:%d:%s\\n\", __FILE__, __LINE__, __func__);return -1;}}\n"
                line=re.sub(r'([a-zA-Z_]\w*)\.back\(\)', f"value_back_{item_cnt}", line,count=1)
                item_cnt+=1
            line=output+line
        
        matchObj = re.findall( r'([a-zA-Z_]\w*)\[(\d+)\]', line)
        if len(matchObj)>0:
            output=""
            for m in matchObj:
                if m[0] not in vector_map:
                    raise Exception(f"vector {m[0]} not found")
                debug("[] {}".format(m))
                v=vector_map[m[0]]
                idx=m[1]
                output+=f"""{v.type} value_idx_{item_cnt};
                if(vector_getN({v.name},{idx},&value_idx_{item_cnt})){{
                    fprintf(stderr, \"Error at %s:%d:%s\\n\", __FILE__, __LINE__, __func__);
                    return -1;
                }}\n"""
                line=re.sub(r'([a-zA-Z_]\w*)\[(\d+)\]', f"value_idx_{item_cnt}", line,count=1)
                item_cnt+=1
            line=output+line
        
        # declaration
        matchObj = re.search( r'vector<([a-zA-Z_]\w*)>\s*([a-zA-Z_]\w*)\s*=\s*vector<([a-zA-Z_]\w*)>\s*\((\d+)\);', line)
        if matchObj:
            v = vector()
            v.name=matchObj.group(2)
            v.type=matchObj.group(1)
            vector_map[v.name]=v
            debug("declaration 1 {}".format(matchObj.groups()))
            size=int(matchObj.group(4))
            line=f"vector *{v.name} = new_vector({size},sizeof({v.type}));\n"
            continue
        
        matchObj = re.search( r'vector<([a-zA-Z_]\w*)>\s*([a-zA-Z_]\w*)', line)
        if matchObj:
            v = vector()
            debug("declaration 2 {}".format(matchObj.groups()))
            v.name=matchObj.group(2)
            v.type=matchObj.group(1)
            vector_map[v.name]=v
            line=f"vector *{v.name};\n"
            continue
        
        matchObj = re.search( r'([a-zA-Z_]\w*)\s*=\s*vector<([a-zA-Z_]\w*)>\s*\((\d+)\);', line)
        if matchObj:
            v = vector()
            debug("declaration 3 {}".format(matchObj.groups()))
            v.name=matchObj.group(1)
            v.type=matchObj.group(2)
            size=int(matchObj.group(3))
            vector_map[v.name]=v
            line=f"{v.name} = new_vector({size},sizeof({v.type}));\n"
            continue
        
        # push_back
        matchObj = re.search( r'([a-zA-Z_]\w*)\.push_back\(([a-zA-Z_]\w*)\);', line)
        if matchObj:
            if matchObj.group(1) not in vector_map:
                continue
            debug("push_back 1 {}".format(matchObj.groups()))
            v=vector_map[matchObj.group(1)]
            item=matchObj.group(2)
            line=f"if(vector_push_back({v.name},sizeof({v.type}),&{item})){{fprintf(stderr, \"Error at %s:%d:%s\\n\", __FILE__, __LINE__, __func__);return -1;}}\n"
            continue
        
        matchObj = re.search( r'([a-zA-Z_]\w*)\.push_back\((\d+)\);', line)
        if matchObj:
            if matchObj.group(1) in vector_map:
                continue
            debug("push_back 2 {}".format(matchObj.groups()))
            v=vector_map[matchObj.group(1)]
            item=matchObj.group(2)
            line=f"""{v.type} value_{item_cnt}={item};
            if(vector_push_back({v.name},sizeof({v.type}),&value_{item_cnt})){{fprintf(stderr, \"Error at %s:%d:%s\\n\", __FILE__, __LINE__, __func__);return -1;}}\n"""
            item_cnt+=1
            continue
        
        matchObj = re.search(r'([a-zA-Z_]\w*)\.push_back\(\"([^\"]*)\"\);', line)
        if matchObj:
            if matchObj.group(1) not in string_map:
                continue
            debug("push_back 3 {}".format(matchObj.groups()))
            line=f"""if(string_push_back({matchObj.group(1)},\"{matchObj.group(2)}\")){{
                fprintf(stderr, \"Error at %s:%d:%s\\n\", __FILE__, __LINE__, __func__);
                return -1;
                }}\n"""
            continue
        
        # pop_back
        matchObj = re.search( r'([a-zA-Z_]\w*)\.pop_back\(\);', line)
        if matchObj:
            if matchObj.group(1) in vector_map:
                debug("pop_back {}".format(matchObj.groups()))
                line=f"if(vector_pop_back({matchObj.group(1)})){{fprintf(stderr, \"Error at %s:%d:%s\\n\", __FILE__, __LINE__, __func__);return -1;}}\n"
                continue
            elif matchObj.group(1) in string_map:
                debug("pop_back {}".format(matchObj.groups()))
                line=f"if(string_pop_back({matchObj.group(1)})){{fprintf(stderr, \"Error at %s:%d:%s\\n\", __FILE__, __LINE__, __func__);return -1;}}\n"
                continue

        
            
        # string declaration
        matchObj = re.search( r'string \s*([a-zA-Z_]\w*)\s*\(\"([^\"]*)\"\);', line)
        if matchObj:
            debug("string 1 {}".format(matchObj.groups()))
            s=string()
            s.name=matchObj.group(1)
            string_map[s.name]=s
            line=f"string *{s.name} = new_string();\nstring_set({s.name},\"{matchObj.group(2)}\");\n"
            continue
        
        matchObj = re.search( r'string \s*([a-zA-Z_]\w*)\s*;', line)
        if matchObj:
            debug("string 2 {}".format(matchObj.groups()))
            s=string()
            s.name=matchObj.group(1)
            string_map[s.name]=s
            line=f"string *{s.name} = new_string();\n"
            continue
        
        matchObj = re.search( r'string \s*([a-zA-Z_]\w*)\s*=\s*string\(\s*\"([^\"]*)\"\s*\);', line)
        if matchObj:
            debug("string 3 {}".format(matchObj.groups()))
            s=string()
            s.name=matchObj.group(1)
            string_map[s.name]=s
            line=f"string *{matchObj.group(1)} = new_string();\nstring_set({s.name},\"{matchObj.group(2)}\");\n"
            continue
        
        matchObj = re.search( r'([a-zA-Z_]\w*)\s*=\s*string\(\s*\"([^\"]*)\"\s*\);', line)
        if matchObj:
            debug("string 4 {}".format(matchObj.groups()))
            if matchObj.group(1) not in string_map:
                continue
            s=string_map[matchObj.group(1)]
            line=f"string_set({s.name},\"{matchObj.group(2)}\");\n"
            continue
        
        matchObj = re.search( r'([a-zA-Z_]\w*)\s*=\"([^\"]*)\";', line)
        if matchObj:
            debug("string 5 {}".format(matchObj.groups()))
            if matchObj.group(1) not in string_map:
                continue
            s=string_map[matchObj.group(1)]
            line=f"string_set({s.name},\"{matchObj.group(2)}\");\n"
            continue
     
        matchObj = re.search( r'([a-zA-Z_]\w*).append\(([a-zA-Z_]\w*)\)', line) 
        if matchObj:
            debug("append 1 {}".format(matchObj.groups()))
            if matchObj.group(1) not in string_map or matchObj.group(2) not in string_map:
                continue
            line=f"string_append({matchObj.group(1)},string_get({matchObj.group(2)}));\n"
            continue      
        
        matchObj = re.search( r'([a-zA-Z_]\w*).append\(\"([^\"]*)\"\)', line)
        if matchObj:
            debug("append 2 {}".format(matchObj.groups()))
            if matchObj.group(1) not in string_map:
                continue
            line=f"string_append({matchObj.group(1)},\"{matchObj.group(2)}\");\n"
            continue

        matchObj = re.search(r'printf\s*\("([^"]*)"\s*,\s*([^);]*)\)', line)
        if matchObj:
            debug("printf {}".format(matchObj.groups()))
            for item in multi_split(matchObj.group(2), [","]):
                if item in string_map:
                    s=string_map[item]
                    line=re.sub(item, f"string_get({s.name})", line, count=1)
            continue
        
        # hashmap declaration
        matchObj = re.search( r'hashmap\s*<\s*([a-zA-Z_]\w*)\s*,\s*([a-zA-Z_]\w*)\s*>\s*([a-zA-Z_]\w*)\s*=\s*hashmap\s*<\s*([a-zA-Z_]\w*)\s*,\s*([a-zA-Z_]\w*)\s*>\s*\(\d*\)', line)
        if matchObj:
            debug("hashmap 1 {}".format(matchObj.groups()))
            h=hashmap()
            h.name=matchObj.group(3)
            h.key_type=matchObj.group(1)
            h.value_type=matchObj.group(2)
            matchObj = re.search( r'hashmap\s*<\s*([a-zA-Z_]\w*)\s*,\s*([a-zA-Z_]\w*)\s*>\s*([a-zA-Z_]\w*)\s*=\s*hashmap\s*<\s*([a-zA-Z_]\w*)\s*,\s*([a-zA-Z_]\w*)\s*>\s*\((\d+)\)', line)
            size=16
            if matchObj:
                size=int(matchObj.group(6))
            hashmap_map[h.name]=h
            line=f"hashmap *{h.name} = new_hashmap_size({size},sizeof({h.key_type}),sizeof({h.value_type}));\n"
            continue
        
        matchObj = re.search(r'([a-zA-Z_]\w*).insert\(([a-zA-Z_]\w*),([a-zA-Z_]\w*)\)\s*;',line)
        if matchObj:
            debug("insert {}".format(matchObj.groups()))
            name=matchObj.group(1)
            key=matchObj.group(2)
            value=matchObj.group(3)
            if not name in hashmap_map:
                continue
            line=f"""if(hashmap_insert({name},{key},{value}))
            {{
                fprintf(stderr, \"Error at %s:%d:%s\\n\", __FILE__, __LINE__, __func__);
                return -1;
            }}\n"""
            continue
        
        matchObj = re.search(r'([a-zA-Z_]\w*).remove\(([a-zA-Z_]\w*)\)\s*;',line)
        if matchObj:
            debug("remove {}".format(matchObj.groups()))
            name=matchObj.group(1)
            key=matchObj.group(2)
            if not name in hashmap_map:
                continue
            line=f"""if(hashmap_remove({name},{key}))
            {{
                fprintf(stderr, \"Error at %s:%d:%s\\n\", __FILE__, __LINE__, __func__);
                return -1;
            }}\n"""
            continue     
        
        if line.strip() == "return 0;":
            for i in vector_map:
                f.write(f"vector_free({i});\n")
            for i in string_map:
                f.write(f"string_free({i});\n")
            f.write("return 0;}\n")
            break
            
    
            

        

        


    
    
    
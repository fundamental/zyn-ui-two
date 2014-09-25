#include <yaml-cpp/yaml.h>
#include <vector>
#include <string>
#include <stdexcept>
#include <fstream>
#include <map>
#include "widget.h"

using std::vector;
using std::map;
using std::string;

struct X
{
    YAML::Node n;
    operator string() const { return n.as<string>(); }
};
struct F
{
    YAML::Node n;
    operator float() const { return n.as<float>(); }
};

//A generator of widgets from symbol based names
map<string, Properties> names;//Things that we should be able to construct

template<class T>
YAML::Node &operator|=(YAML::Node &&n, const T &t)
{
    if(!n.IsDefined())
        n = t;
    return n;
}

void loadViewData(void)
{
    YAML::Node config = YAML::LoadFile("input.yml");
    size_t elms = config.size();
    for(unsigned i=0; i<elms; ++i) {
        auto node = config[i]["Widget"];
        if(node.IsDefined()) {
            node["class"]  |= node["name"];
            node["scale"]  |= 1.0;
            node["aspect"] |= 1.0;
        }
        names[node["name"].as<string>()] = node;
    }
}

template<class T>
Widget *Spawn(NVGcontext *vg, Properties &p)
{
    return new T(vg, p);
}

string nameRemap(string name, Properties &p)
{
    //Merge onto properties the metaproperties
    
    if(names.find(name) == names.end())
        throw std::invalid_argument("Uknown widget type '"+name+"'");
    auto metaclass = names[name];
    name = X{metaclass["class"]};
    for(auto prop:metaclass)
        p[prop.first] |= prop.second;

    return name;
}

Widget *SpawnByName(string name, NVGcontext *vg, Properties &p)
{
    name = nameRemap(name, p);
    if(name == "Knob")
        return Spawn<Knob>(vg, p);
    else if(name == "KnobDetail")
        return Spawn<KnobDetail>(vg, p);
    else if(name == "DropDown")
        return Spawn<DropDown>(vg, p);
    else if(name == "Button")
        return Spawn<Button>(vg, p);

    throw std::invalid_argument("Unknown Widget "+name+"...");
}

YAML::Node FindModule(string mod)
{
    YAML::Node config = YAML::LoadFile("input.yml");
    size_t elms = config.size();
    for(unsigned i=0; i<elms; ++i) {
        auto node = config[i]["Def"];
        if(node["name"].as<string>() == mod)
            return node;
    }
    throw std::invalid_argument("Unknown module '"+mod+"'");
}


Module *Generate(const char *mod, NVGcontext *vg)
{
    loadViewData();
    auto node = FindModule(mod);
    Module *module = new Module(vg, node);
    if(node["children"]) {
        for(auto x:node["children"]) {
            printf("Inserting Child\n");
            auto    props = x.second;
            Widget *child = SpawnByName(X{x.first}, vg, props);
            module->inner.add(child, F{props["aspect"]}, F{props["scale"]});
        }
    }
    return module;
}
